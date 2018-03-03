// Visual Notes.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "stdil.h"

#include "Visual Notes.h"
#include "MainFrm.h"
#include "Visual NotesDoc.h"
#include "Visual NotesView.h"
#include "RegistrationDlg.h"
#include "AppSettingsDlg.h"
#include "DiagramViewDlg.h"
#include "NewNoteDlg.h"
#include "AboutDlg1.h"
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define VN_TRIAL_DAYS		6
#define VN_TRIAL_HOURS		0
#define VN_TRIAL_MINUTES	0
#define VN_MAJOR_VERSION	0x01
#define VN_MINOR_VERSION	0x00
#define IDUSER_NOCONFIRM	50200

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesApp

BEGIN_MESSAGE_MAP(CVisualNotesApp, CWinApp)
	//{{AFX_MSG_MAP(CVisualNotesApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOLS_SETTINGS, OnToolsSettings)
	ON_COMMAND(ID_TOOLS_DIAGRAM, OnToolsDiagram)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_HELP, OnHelp )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesApp construction

CVisualNotesApp::CVisualNotesApp() : CWinApp(), m_fPrevShapeInBuild(1),
									   m_fLinkChild(2),
									   m_fPrevJointInBuild(4), 
									   m_fLastCreatedShape(8),
									   m_fControlShape(16),
									   m_bLockDocument(FALSE),
									   m_TreeDir( treedirDown )
	
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_strAppKey.Format( TEXT("Software\\InvisibleLabs\\VisualNotes") );
	m_strUserName = _T("");
	m_strAuthCode = _T("");
	m_strFileFilter = _T("All Files|*.vnf||");
	m_strFileFolder = _T("All Folders");

	m_bDefRemoveChildren = TRUE;
	m_bDefAllowStrechingBMP = FALSE;
	m_bDefUnlinkOnCD = FALSE;
	m_bDefLockDocument = FALSE;
	m_bDefAutoRemoveLink = FALSE;
	m_bDefDirectEdgeAA = FALSE;
	
	m_DefCanvasSize = CSize( 400, 400 );
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVisualNotesApp object

CVisualNotesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesApp initialization

BOOL CVisualNotesApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CCeDocListDocTemplate* pDocTemplate;
	pDocTemplate = new CCeDocListDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVisualNotesDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CVisualNotesView),
		m_strFileFilter );
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	BOOL bResult = ProcessShellCommand(cmdInfo);
	if (!bResult)
	{
		pDocTemplate->ShowDocList();
		CCeDocList* pDL = pDocTemplate->m_pWndDocList;
	
		((CMainFrame*)m_pMainWnd)->m_GeneralToolBar.ShowWindow( SW_HIDE );
		((CMainFrame*)m_pMainWnd)->m_CreateToolBar.ShowWindow( SW_HIDE );

		pDL->Invalidate();
		pDL->UpdateWindow();
		
		/*
		if( IDYES == MessageBox( NULL, TEXT( "Delete Registry Keys" ), NULL, MB_YESNO ) )
			RegDeleteKey( HKEY_LOCAL_MACHINE, m_strAppKey );
			*/
		
		if( !VerifyLicense() )
			return FALSE;

		LoadUserSettings();
		
		// if there is no file in the doclist, we will create a new one.
		if (pDL->GetItemCount() == 0) 
		{
			pDL->OnClose();
			OnFileNew();
		}
	}

	return TRUE;
}

void CVisualNotesApp::OnHelp()
{
	//Setup application will have installed our help files in \windows directory.
	
	::CreateProcess( _T( "PegHelp.exe" ), _T( "\\windows\\vnhlp.htm#Main_Contents" ), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
}

BOOL CVisualNotesApp::CreateShape( CRect CreationRect )
{
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;
	CILSymbol *pNewSymbol = NULL;
	CILDBSet *pDBSet;
	
	//we don't like small symbols.
	if( (CreationRect.Size().cx < CILSymbol::ms_MinSize.cx && CreationRect.Size().cy < CILSymbol::ms_MinSize.cy) )
		return FALSE;

	//Note: This algorithm assumes, some toolset is active ( the current tool set ).
	if( pMainFrame->m_CurrentSTS == CMainFrame::SYMBOLTOOLSET_FLOWCHART )
	{
		if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_RECTANGLE )
			pNewSymbol = new CILRectangle( CreationRect.left, CreationRect.top, CreationRect.right , CreationRect.bottom );
					
		else if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_CIRCLE )
			pNewSymbol = new CILCircle( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom  );
		
		else if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_SHEAR )
			pNewSymbol = new CILShear( CreationRect.left, CreationRect.top, CreationRect.right , CreationRect.bottom );
		
		else if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_DIAMOND )
			pNewSymbol = new CILDiamond( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_TAGA )
			pNewSymbol = new CILTagA( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_TAGB )
			pNewSymbol = new CILTagB( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_FlowChartSymbol == CMainFrame::FLOWCHARTSYMBOL_GENERALIZE )
			pNewSymbol = new CILGeneralize( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			
		else 
			return FALSE;
	}

	else if( pMainFrame->m_CurrentSTS == CMainFrame::SYMBOLTOOLSET_UML )
	{
		if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_NOTE )
			pNewSymbol = new CILUmlNote( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_PACKAGE )
			pNewSymbol = new CILUmlPackage( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_COMPONENT )
			pNewSymbol = new CILUmlComponent( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
		
		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_CLASS )
			pNewSymbol = new CILUmlClass( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_CASE )
			pNewSymbol = new CILCircle( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_ACTOR )
			pNewSymbol = new CILUmlActor( CreationRect.left, CreationRect.top );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_DEPLOYMENT )
			pNewSymbol = new CILUmlDeployment( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_CAPSULE )
			pNewSymbol = new CILCapsule( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_UmlSymbol == CMainFrame::UMLSYMBOL_ROUNDRECT )
			pNewSymbol = new CILRoundRect( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else
			return FALSE;
	}

	else if( pMainFrame->m_CurrentSTS == CMainFrame::SYMBOLTOOLSET_NET1 )
	{
		BOOL bBmpInitialize = TRUE;

		if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_CLIENT )
			pNewSymbol = new CILNetClient( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
		
		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_SERVER )
			pNewSymbol = new CILNetServer( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			
		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_DATABASE )
			pNewSymbol = new CILNetDatabase( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			
		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_CLOUD )
		{
			pNewSymbol = new CILNetCloud( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			bBmpInitialize = FALSE;
		}

		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_PRINTER )
			pNewSymbol = new CILNetPrinter( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
		
		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_HUB )
			pNewSymbol = new CILNetHub( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
		
		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_MAINFRAME )
			pNewSymbol = new CILNetMainframe( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_Net1Symbol == CMainFrame::NET1SYMBOL_RING )
		{
			pNewSymbol = new CILNetRing( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			bBmpInitialize = FALSE;
		}


		else
			return FALSE;	
		
		//Certain net symbols, like the cloud symbol, are NOT bitmap shapes.
		if( bBmpInitialize )
			((CILBitmapShape*)pNewSymbol)->Initialize();
		
	}

	else if( pMainFrame->m_CurrentSTS == CMainFrame::SYMBOLTOOLSET_NET2 )
	{	
		//TODO: instantiate the satellite object.

		//TODO: instantiate the dish object.

		if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_CELLPHONE )
			pNewSymbol = new CILNetCellPhone( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			
		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_PDA )
			pNewSymbol = new CILNetPDA( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			
		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_THINKPAD )
			pNewSymbol = new CILNetThinkPad( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
				
		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_CAMERA )
			pNewSymbol = new CILNetCamera( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
			
		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_FAX )
			pNewSymbol = new CILNetFax( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
				
		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_WIREPHONE )
			pNewSymbol = new CILNetModem( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_SATELLITE )
			pNewSymbol = new CILNetSatellite( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );

		else if( pMainFrame->m_Net2Symbol == CMainFrame::NET2SYMBOL_DISH )
			pNewSymbol = new CILNetDish( CreationRect.left, CreationRect.top, CreationRect.right, CreationRect.bottom );
				
		else return FALSE;
		

		((CILBitmapShape*)pNewSymbol)->Initialize();
	}

	
	else 
		return FALSE;

	pDBSet = GetDBSet();
	pNewSymbol->SetDiagramID( pDBSet->GetID() );
	SetLastCreatedShape( pNewSymbol );
	
	CILTransaction *pTransaction = new CILTransaction;
	pDBSet->m_DB.Insert( pNewSymbol, &pTransaction );
	pDBSet->m_TC.Commit( pTransaction );

	return TRUE;
}

BOOL CVisualNotesApp::CreateEdge( CPoint ptCanvasHit )
{
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;

	static CPoint ptPrevHit;
	//CPoint ptCanvasHit( ptHit );
	CILJoint *pControlJoint = NULL, *pControlJointSS = NULL;
	CILJoint *pSymbiantJoint = NULL, *pSymbiantJointSS = NULL;
	CILSymbol *pHitSymbol = NULL, *pHitSnapshot = NULL;
	CILSymbol *pPrevNeighbor = NULL, *pPrevNeighborSS = NULL;	
	CILTransaction *pTransaction = NULL;
	CILDBSet *pDBSet = GetDBSet();
	
	if( pMainFrame->m_EdgeSet1 != CMainFrame::EDGESET1_DEPBASICEDGE  &&
		pMainFrame->m_EdgeSet1 != CMainFrame::EDGESET1_BASICEDGE &&
		pMainFrame->m_EdgeSet1 != CMainFrame::EDGESET1_COMPLEXEDGE )
		return FALSE;

	//First test to see if the user hit a symbol.
	if( pDBSet->m_DB.QueryHit( ptCanvasHit, &pHitSymbol ) )
	{
		//if user hit multi joint edge, we'll highlight it to let user know application is ready
		//to add another branch to multi-edge control joint.
		if( pHitSymbol->IsKindOf( RUNTIME_CLASS( CILJointTree ) ) )
		{
			//user cannot select multi-edge as "second-neighbor". 
			//if user makes an attempt, we will NOT change ANY states of this applciation.
			if( pDBSet->m_DB.QueryFlag( m_fPrevJointInBuild, &pPrevNeighbor ) || 
				pDBSet->m_DB.QueryFlag( m_fPrevShapeInBuild, &pPrevNeighbor ) )
				return FALSE;

			pTransaction = new CILTransaction;
			pHitSymbol->GetSnapshot( &pHitSnapshot );
			CILBitOp::TurnOn( &pHitSnapshot->m_dwExtraData, m_fPrevJointInBuild );			
		
			pDBSet->m_DB.Update( pHitSymbol, pHitSnapshot, &pTransaction );
			pDBSet->m_TC.Commit( pTransaction );
			delete pHitSnapshot;
		}

		//else if user hit any other kind of non-joint symbol that doesn't require special handling...
		else if( !pHitSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
		{
			pTransaction = new CILTransaction;
			pHitSymbol->GetSnapshot( &pHitSnapshot );

			//...and this symbol is the second symbol in a single-edge build, 
			//create control joint for first symbol, create symbiant joint for second symbol. 
			if( pDBSet->m_DB.QueryFlag( m_fPrevShapeInBuild, &pPrevNeighbor ) )
			{
				//..however, if the second neighbor is the same as the first, deny any connection
				//attempt.
				if( pPrevNeighbor == pHitSymbol )
				{
					delete pTransaction; //we won't be using this any more.
					return FALSE;
				}

				if( pMainFrame->m_EdgeSet1 == CMainFrame::EDGESET1_BASICEDGE )
				{
					pControlJoint = new CILJoint( ptPrevHit );
					pSymbiantJoint = new CILJoint( ptCanvasHit );
				}

				else if( pMainFrame->m_EdgeSet1 == CMainFrame::EDGESET1_DEPBASICEDGE )
				{
					pControlJoint = new CILJointCorner( ptPrevHit );
					pSymbiantJoint = new CILJoint( ptCanvasHit );
				}

				else if( pMainFrame->m_EdgeSet1 == CMainFrame::EDGESET1_COMPLEXEDGE )
				{
					pControlJoint = new CILJointTree( ptPrevHit );
					pSymbiantJoint = new CILJoint( ptCanvasHit );
					
					if( m_TreeDir == treedirDown )
						CILBitOp::TurnOn( &pControlJoint->m_dwExtraJointData, JOINTDATA_BOTTOM | JOINTDATA_HCENTER );
					else if( m_TreeDir == treedirUp )
						CILBitOp::TurnOn( &pControlJoint->m_dwExtraJointData, JOINTDATA_TOP | JOINTDATA_HCENTER );
					else if( m_TreeDir == treedirLeft )
						CILBitOp::TurnOn( &pControlJoint->m_dwExtraJointData, JOINTDATA_LEFT | JOINTDATA_VCENTER );
					else if( m_TreeDir == treedirRight )
						CILBitOp::TurnOn( &pControlJoint->m_dwExtraJointData, JOINTDATA_RIGHT | JOINTDATA_VCENTER );
				}
						
				else 
				{
					delete pTransaction; //we won't be needing this anymore.
					return FALSE;
				}

				//Add control joint to first neighbor. 
				//dd symbiant joint to second neighbor.
				//any changes to previously added joints of both neighbors.

				pPrevNeighbor->GetSnapshot( &pPrevNeighborSS );
				CILBitOp::TurnOff( &pPrevNeighborSS->m_dwExtraData, m_fPrevShapeInBuild );
				
				pControlJoint->AddSymbiantJoint( pSymbiantJoint );
				pSymbiantJoint->SetControlJoint( pControlJoint );
				
				pHitSnapshot->AddJoint( pSymbiantJoint );
				pSymbiantJoint->SetMasterShape( pHitSymbol );
				pPrevNeighborSS->AddJoint( pControlJoint );
				pControlJoint->SetMasterShape( pPrevNeighbor );

				//if the user wants to force basic edges to be axis-aligned, adjust the 
				//control joint to aligne with the 
				if( m_bDirectEdgeAA && pMainFrame->m_EdgeSet1 == CMainFrame::EDGESET1_BASICEDGE )
					 pControlJoint->AxisAlign( pSymbiantJoint );
								
				pDBSet->m_DB.Insert( pControlJoint, &pTransaction );
				pDBSet->m_DB.Insert( pSymbiantJoint, &pTransaction );
				pDBSet->m_DB.Update( pPrevNeighbor, pPrevNeighborSS, &pTransaction );
			
				delete pPrevNeighborSS;
			}
			
			//else if user has selected second symbol in a multi-edge build, we only need to 
			//create symbiant joint and add it to the second edge.
			else if( pDBSet->m_DB.QueryFlag( m_fPrevJointInBuild, &pPrevNeighbor ) )
			{
				//we do not want the user to be able to add a symbiant tree joint to the 
				//control joint's master shape.
				if( ((CILJoint*)pPrevNeighbor)->GetMasterShape() == pHitSymbol )
				{
					delete pTransaction;
					return FALSE;
				}

				pControlJoint = (CILJoint*)pPrevNeighbor;
				pControlJoint->GetSnapshot( (CILSymbol**)&pControlJointSS );
				
				pSymbiantJoint = new CILJoint( ptCanvasHit );
				pControlJointSS->AddSymbiantJoint( pSymbiantJoint );
				pSymbiantJoint->SetControlJoint( pControlJoint );
				pHitSnapshot->AddJoint( pSymbiantJoint );
				pSymbiantJoint->SetMasterShape( pHitSymbol );

				pDBSet->m_DB.Insert( pSymbiantJoint, &pTransaction );
				pDBSet->m_DB.Update( pControlJoint, pControlJointSS, &pTransaction );
			
				delete pControlJointSS;
			}

			//else user has selected hit symbol as FIRST neighbor for the edge.
			else
			{	
				CILBitOp::TurnOn( &pHitSnapshot->m_dwExtraData, m_fPrevShapeInBuild );
				ptPrevHit = ptCanvasHit;
			}

			pDBSet->m_DB.Update( pHitSymbol, pHitSnapshot, &pTransaction );
			pDBSet->m_TC.Commit( pTransaction );

			delete pHitSnapshot;
		}

		//else user hit nothing meaningful. 
		//We don't unmark any first neighbors here since user may have been TRYING to connect the neighbor.
		//We don't want to penalize the user for trying, do we.
		else
			return FALSE;
	}

	//else user hit nothing at all.
	//user is trying to unmark first neighbor so lets do that.
	else
	{
		//clear any "first-nieghbor" flags.
		if( pDBSet->m_DB.QueryFlag( m_fPrevShapeInBuild, &pPrevNeighbor ) ||
			pDBSet->m_DB.QueryFlag( m_fPrevJointInBuild, &pPrevNeighbor ) )
		{
			pTransaction = new CILTransaction; 
			pPrevNeighbor->GetSnapshot( &pPrevNeighborSS );
			CILBitOp::TurnOff( &pPrevNeighborSS->m_dwExtraData, m_fPrevShapeInBuild | m_fPrevJointInBuild );
			pDBSet->m_DB.Update( pPrevNeighbor, pPrevNeighborSS, &pTransaction );
			pDBSet->m_TC.Commit( pTransaction );			
			delete pPrevNeighborSS;
		}		

		return FALSE;
	}

	return TRUE;
}

void CVisualNotesApp::TrackSymbol( CPoint MousePoint )
{
	CILSymbol *pTrackedSymbol = NULL;
	CILSymbol *pTrackedSymbolSS = NULL;
	CILSymbol *pNewSymbol = NULL;
	CILList<CILSymbol*,CILSymbol*> TrackedChildren, TrackedChildrenSS;
	CILList<CILSymbol*,CILSymbol*> TrackedJoints, TrackedJointsSS;
	CILTransaction *pTransaction;
	CILDBSet *pDBSet = GetDBSet();
	
	CPoint CanvasOrigin(0,0);
	CPoint TrackPoint( MousePoint );
	
	
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;
	CVisualNotesView *pView = (CVisualNotesView*)pMainFrame->GetActiveView();

	ASSERT( pView  );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );
	pView->ClientToCanvas( &TrackPoint );
	
	pDBSet->m_DB.QueryTracked( &pTrackedSymbol );
	
	//It's important to first check to see if the hit was inside any current 
	//recttracker object. The reason behind this being that a symbol hit test
	//would not register an outer-resize handle selection.
	if( pTrackedSymbol  && pTrackedSymbol->TrackerHitTest( TrackPoint ) != - 1)
	{
		pTransaction = new CILTransaction;

		CILSymbol *pChildSymbol = NULL;
		//before we automatically track a symbol, let's make sure the user was not selecting one of
		//the symbols children.
		if( pDBSet->m_DB.QueryHit( TrackPoint, &pChildSymbol ) )
		{
			if( pTrackedSymbol->IsChild( pChildSymbol ) )
			{
				pTrackedSymbol->GetSnapshot( &pTrackedSymbolSS );
				pTrackedSymbolSS->SetCanvasOrigin( CanvasOrigin );
				pTrackedSymbolSS->ShowTracker( FALSE, pView );
				pDBSet->m_DB.Update( pTrackedSymbol, pTrackedSymbolSS, &pTransaction );
				pTrackedSymbol = pChildSymbol;
				delete pTrackedSymbolSS;
			}
		}
		
		pTrackedSymbol->GetSnapshot( &pTrackedSymbolSS );
		pTrackedSymbol->GetChildren( &TrackedChildren, TRUE );
		pTrackedSymbol->GetJoints( &TrackedJoints, TRUE );
		CreateSnapshotList( &TrackedChildren, &TrackedChildrenSS );
		CreateSnapshotList( &TrackedJoints, &TrackedJointsSS );
		pTrackedSymbolSS->SetCanvasOrigin( CanvasOrigin );
		pTrackedSymbolSS->Track( pView, MousePoint, &TrackedChildrenSS, &TrackedJointsSS ); 
		pDBSet->m_DB.Update( pTrackedSymbol, pTrackedSymbolSS, &pTransaction );
		pDBSet->m_DB.Update( &TrackedChildren, &TrackedChildrenSS, &pTransaction );
		pDBSet->m_DB.Update( &TrackedJoints, &TrackedJointsSS, &pTransaction );
		pDBSet->m_TC.Commit( pTransaction );
		delete pTrackedSymbolSS;
		DeleteSnapshotList( &TrackedChildrenSS );
		DeleteSnapshotList( &TrackedJointsSS );
	}
			
	//else if new symbol has been hit...
	else if( pDBSet->m_DB.QueryHit( TrackPoint, &pNewSymbol ) )
	{	
		pTransaction = new CILTransaction;
		pDBSet->m_DB.ZOrderSymbol( pNewSymbol );
		
		//if a previous tracked symbol exists and it is not the symbol the user just hit,
		//deselect the previous track symbol.
		if( pTrackedSymbol && pTrackedSymbol != pNewSymbol )
		{
			pTrackedSymbol->GetSnapshot( &pTrackedSymbolSS );
			pTrackedSymbolSS->SetCanvasOrigin( CanvasOrigin );
			pTrackedSymbolSS->ShowTracker( FALSE, pView );
			pDBSet->m_DB.Update( pTrackedSymbol, pTrackedSymbolSS, &pTransaction );
			delete pTrackedSymbolSS;
		}
		
		pTrackedSymbol = pNewSymbol;
		pTrackedSymbol->GetSnapshot( &pTrackedSymbolSS );
		pTrackedSymbol->GetChildren( &TrackedChildren, TRUE );
		pTrackedSymbol->GetJoints( &TrackedJoints, TRUE );
		CreateSnapshotList( &TrackedChildren, &TrackedChildrenSS );
		CreateSnapshotList( &TrackedJoints, &TrackedJointsSS );
		pTrackedSymbolSS->SetCanvasOrigin( CanvasOrigin );
		pTrackedSymbolSS->Track( pView, MousePoint, &TrackedChildrenSS, &TrackedJointsSS );
		pDBSet->m_DB.Update( pTrackedSymbol, pTrackedSymbolSS, &pTransaction );
		pDBSet->m_DB.Update( &TrackedChildren, &TrackedChildrenSS, &pTransaction );
		pDBSet->m_DB.Update( &TrackedJoints, &TrackedJointsSS, &pTransaction );
		pDBSet->m_TC.Commit( pTransaction );
		delete pTrackedSymbolSS;
		DeleteSnapshotList( &TrackedChildrenSS );
		DeleteSnapshotList( &TrackedJointsSS );
	}
	
	//else unassociate the tracker if the user clicked "nothing". AND a previous
	//tracked symbol existed.
	else if( pTrackedSymbol )
	{
		pTransaction = new CILTransaction;
		pTrackedSymbol->GetSnapshot( &pTrackedSymbolSS );
		pTrackedSymbolSS->SetCanvasOrigin( CanvasOrigin );
		pTrackedSymbolSS->ShowTracker( FALSE, pView );
		pDBSet->m_DB.Update( pTrackedSymbol, pTrackedSymbolSS, &pTransaction );
		pDBSet->m_TC.Commit( pTransaction );		
		delete pTrackedSymbolSS;
	}

	TrackedJoints.RemoveAll();
	TrackedChildren.RemoveAll();
}

void CVisualNotesApp::CreateLabel( CPoint CreatePoint )
{
	
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;
	CVisualNotesView *pView = (CVisualNotesView*)pMainFrame->GetActiveView();
	CPoint CanvasOrigin(0,0);
	
	ASSERT( pView );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );

	CPoint LabelCenter, ParentCenter;
	CILSymbol *pNewLabel = NULL;
	CILSymbol *pHostSymbol = NULL;
	CILSymbol *pHostSymbolSS = NULL;
	CILTransaction *pTransaction = new CILTransaction;
	CILDBSet *pDBSet = GetDBSet();
	pNewLabel = new CILLabel( CreatePoint.x, CreatePoint.y, CreatePoint.x + 35, CreatePoint.y + 15 );

	//If user has clicked on symbol while creating a label, the user wishes to automatically
	//make the symbol the labels parent. Let's accomodate the user.
	if( pDBSet->m_DB.QueryHit( CreatePoint, &pHostSymbol ) )
	{		
	
		LabelCenter = CPoint(  pNewLabel->m_BoundingRect.left + pNewLabel->m_BoundingRect.Width() / 2,
							   pNewLabel->m_BoundingRect.top + pNewLabel->m_BoundingRect.Height() / 2 );
		ParentCenter = CPoint( pHostSymbol->m_BoundingRect.left + pHostSymbol->m_BoundingRect.Width() / 2,
							   pHostSymbol->m_BoundingRect.top + pHostSymbol->m_BoundingRect.Height() /2 );

		/*
		//center child label on parent symbol.
		CRect CenteredRect = pNewLabel->m_BoundingRect;
		CenteredRect.OffsetRect( ParentCenter - LabelCenter );
		pNewLabel->SetBoundingRect( CenteredRect );
		*/
	
		pHostSymbol->GetSnapshot( &pHostSymbolSS );
		pHostSymbolSS->AddChild( pNewLabel );
		pNewLabel->SetParent( pHostSymbol );
		pDBSet->m_DB.Update( pHostSymbol, pHostSymbolSS, &pTransaction );
		delete pHostSymbolSS;
	}

	pNewLabel->SetDiagramID( pDBSet->GetID() );
	SetLastCreatedShape( pNewLabel );
	pDBSet->m_DB.Insert( pNewLabel, &pTransaction );
	pDBSet->m_TC.Commit( pTransaction );
}

BOOL CVisualNotesApp::SelectSymbol( CPoint SelectPoint )
{
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;
	CVisualNotesView *pView = (CVisualNotesView*)pMainFrame->GetActiveView();
	CPoint CanvasOrigin(0,0);
	CILDBSet *pDBSet = GetDBSet();
	BOOL bRetVal;
	
	ASSERT( pView   );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );
	
	CILList<CILSymbol*,CILSymbol*> SelectList, SelectListSS;
	CILSymbol *pHitSymbol = NULL, *pHitSymbolSS = NULL;
	CILSymbol *pSelSymbol = NULL, *pSelSymbolSS = NULL;
	CILSymbol *pLastControlShape = NULL,	*pLastControlSS = NULL;
	
	CILTransaction *pTransaction = NULL;

	//get all previously selected symbols.
	pDBSet->m_DB.QuerySelected( &SelectList );
	
	//if the user hit a symbol, check to see whether the symbol was a previously 
	//selected symbol.
	if( pDBSet->m_DB.QueryHit( SelectPoint, &pHitSymbol ) )
	{
		BOOL bHitPrevious = FALSE;
		bRetVal = TRUE;
		pDBSet->m_DB.ZOrderSymbol( pHitSymbol );

		ILPOSITION SelPos = SelectList.GetHeadPosition();
		while( SelPos )
		{
			if( pHitSymbol == SelectList.GetAt( SelPos ) )
			{
				bHitPrevious = TRUE;
				break;
			}
			
			SelectList.GetNext( SelPos );
		}

		//if user did NOT hit a previously selected symbol, SELECT the hit symbol.
		if( !bHitPrevious )
		{
			pTransaction = new CILTransaction;
			pHitSymbol->GetSnapshot( &pHitSymbolSS );
			pHitSymbolSS->SetCanvasOrigin( CanvasOrigin );
			pHitSymbolSS->ShowSelection( TRUE, pView );

			//if the new selection is a shape ( any symbol that's not a joint symbol )...
			//let it be the new control shape.
			if( !pHitSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) ) 
			{
				//unmark the previous control shape if one exists.
				if( pDBSet->m_DB.QueryFlag( m_fControlShape, &pLastControlShape ) )
				{
					pLastControlShape->GetSnapshot( &pLastControlSS );
					CILBitOp::TurnOff( &pLastControlSS->m_dwExtraData, m_fControlShape );
					pDBSet->m_DB.Update( pLastControlShape, pLastControlSS, &pTransaction );
					delete pLastControlSS;
				}

				CILBitOp::TurnOn( &pHitSymbolSS->m_dwExtraData, m_fControlShape );
			}

			pDBSet->m_DB.Update( pHitSymbol, pHitSymbolSS, &pTransaction );
			pDBSet->m_TC.Commit( pTransaction );
			delete pHitSymbolSS;
		}

		SelectList.RemoveAll();	
		return TRUE;
	}

	//else the user did not hit anything, so deslect ALL previously selected symbols.
	else
	{
		bRetVal = FALSE;
		ILPOSITION SelPos = SelectList.GetHeadPosition();
		while( SelPos )
		{
			pSelSymbol = SelectList.GetAt( SelPos );
			pSelSymbol->GetSnapshot( &pSelSymbolSS );
			pSelSymbol->SetCanvasOrigin( CanvasOrigin );
			pSelSymbolSS->ShowSelection( FALSE, pView );
			SelectListSS.AddTail( pSelSymbolSS );
			pSelSymbolSS = NULL;
			SelectList.GetNext( SelPos );
		}

		if( !SelectListSS.IsEmpty() )
		{
			pTransaction = new CILTransaction;
			pDBSet->m_DB.Update( &SelectList, &SelectListSS, &pTransaction );
			pDBSet->m_TC.Commit( pTransaction );

			SelPos = SelectListSS.GetHeadPosition();
			while( SelPos )
			{
				delete SelectListSS.GetAt( SelPos );
				SelectListSS.GetNext( SelPos );
			}
			
				SelectListSS.RemoveAll();
		
			//obviously, if no symbols are selected, no control shape should exist.
			if( pDBSet->m_DB.QueryFlag( m_fControlShape, &pLastControlShape ) )
			{
				pLastControlShape->GetSnapshot( &pLastControlSS );
				CILBitOp::TurnOff( &pLastControlSS->m_dwExtraData, m_fControlShape );
				pDBSet->m_DB.Update( pLastControlShape, pLastControlSS, &pTransaction );
				delete pLastControlSS;
			}
		}
		
		SelectList.RemoveAll();	
		return FALSE;
	}		
}

void CVisualNotesApp::LinkSymbols( CPoint ptHit )
{
	CILSymbol *pHitSymbol = NULL, *pHitSymbolSS = NULL;
	CILSymbol *pLinkChild = NULL, *pLinkChildSS = NULL;
	CILSymbol *pOldParent = NULL, *pOldParentSS = NULL;
	CILSymbol *pCircleParent = NULL, *pCircleParentSS = NULL;
	CILDBSet *pDBSet = GetDBSet();

	CILTransaction *pTransaction = new CILTransaction;

	if( pDBSet->m_DB.QueryHit( ptHit, &pHitSymbol ) )
	{
		pHitSymbol->GetSnapshot( &pHitSymbolSS );
		
		//if a previous link child exists, let it's  newly hit symbol become its new parent.
		//so long as..
		//1.the previous link child and the new hit symbol are not one in the same.
		//2.user has not committed a circular dependency exception.
		//3.the Linksymbol, is not already the hitsymbols immediate child.
		if( pDBSet->m_DB.QueryFlag( m_fLinkChild, &pLinkChild ) && pLinkChild != pHitSymbol &&
			!pLinkChild->IsChild( pHitSymbol ) && pLinkChild->GetParent() != pHitSymbol )
		{			
			if( ( pOldParent = pLinkChild->GetParent() ) )
			{
				//record change to OLD parent.				
				pOldParent->GetSnapshot( &pOldParentSS );
				pOldParentSS->RemoveChild( pLinkChild );
				pDBSet->m_DB.Update( pOldParent, pOldParentSS, &pTransaction );
				delete pOldParentSS;
			}
		
			//record change to child
			pLinkChild->GetSnapshot( &pLinkChildSS );
			pLinkChildSS->SetParent( pHitSymbol );
			pHitSymbolSS->AddChild( pLinkChild );
			CILBitOp::TurnOff( &pLinkChildSS->m_dwExtraData, m_fLinkChild );
			pDBSet->m_DB.Update( pLinkChild, pLinkChildSS, &pTransaction );
			delete pLinkChildSS;
		}

		//if user is committing a circular dependency error...
		else if( pLinkChild && pLinkChild->IsChild( pHitSymbol ) )
		{
			//if user has chosen to automatically unlink the new parent (the hitsymbol) during a circular dependency
			//error, then orphan hitsymbol without any notification. Otherwise, notify the user of her error and
			//do nothing (change NO states).
			if( m_bUnlinkOnCD )
			{
				if( pOldParent = pLinkChild->GetParent() )
				{
					pOldParent->GetSnapshot( &pOldParentSS );
					pOldParentSS->RemoveChild( pLinkChild );
					pDBSet->m_DB.Update( pOldParent, pOldParentSS, &pTransaction );
					delete pOldParentSS;
				}

				pCircleParent = pHitSymbol->GetParent(); //ofcourse it has a parent (circular dependency).
				pLinkChild->GetSnapshot( &pLinkChildSS );
				
				//We have to handle the special case where the user is attempting to make a symbol's 
				//IMMEDIATE child it's new parent separately from other cases.
				if( pCircleParent == pLinkChild )
					pLinkChildSS->RemoveChild( pHitSymbol );
				

				else 
				{
					pCircleParent->GetSnapshot( &pCircleParentSS );
					pCircleParentSS->RemoveChild( pHitSymbol );
					pDBSet->m_DB.Update( pCircleParent, pCircleParentSS, &pTransaction );
					delete pCircleParentSS;
				}
				
				pHitSymbolSS->SetParent( NULL );
				pHitSymbolSS->AddChild( pLinkChild );
				pLinkChildSS->SetParent( pHitSymbol );
				CILBitOp::TurnOff( &pLinkChildSS->m_dwExtraData, m_fLinkChild );
				pDBSet->m_DB.Update( pLinkChild, pLinkChildSS, &pTransaction );	
				delete pLinkChildSS;
			}

			else
			{
				m_pMainWnd->MessageBox( TEXT("Circular dependency not permitted." ), TEXT("Linking Error"), MB_ICONERROR );
				delete pTransaction; //we won't be recording any state changes.
				delete pHitSymbolSS;
				return;
			}
		}

	
		//if the user is trying to make linkchild's parent it's parent again, don't do anything.
		else if( pLinkChild && pLinkChild->GetParent() == pHitSymbol )
		{
			delete pTransaction; //we won't be recording any state changes.
			delete pHitSymbolSS;
			return;
		}
	
		//if the user selected the same symbol to be both parent and child, simply deselect the link child.
		else if( pLinkChild && pLinkChild == pHitSymbol )
		{
			//link child is hitsymbol in this situation so we really only have one symbol to update for this
			//transaction.
			CILBitOp::TurnOff( &pHitSymbolSS->m_dwExtraData, m_fLinkChild );
		}

		else //else newly hit symbol becomes the link child
			CILBitOp::TurnOn( &pHitSymbolSS->m_dwExtraData, m_fLinkChild );
		
		//record change to NEW parent and complete transaction.
		pDBSet->m_DB.Update( pHitSymbol, pHitSymbolSS, &pTransaction );
		pDBSet->m_TC.Commit( pTransaction );
		delete pHitSymbolSS;
	}

	//else handle, case where user hit nothing.
	else 
	{
		if( pDBSet->m_DB.QueryFlag( m_fLinkChild, &pLinkChild ) )
		{
			//when the user hits nothing or same symbol AFTER selecting a link child,
			//user wishes for this link child to have no parent. So if the link child 
			//has a parent, orphan it.
			if( ( pOldParent = pLinkChild->GetParent() ) )
			{
				//record change to OLD parent AND Insertion of child into top level
				pOldParent->GetSnapshot( &pOldParentSS );
				pOldParentSS->RemoveChild( pLinkChild );
				pDBSet->m_DB.Update( pOldParent, pOldParentSS, &pTransaction ); 
				//pDBSet->m_DB.Insert( pLinkChild, &pTransaction );
				delete pOldParentSS;
			}
			
			//record change to child and complete transaction.	
			pLinkChild->GetSnapshot( &pLinkChildSS );
			pLinkChildSS->SetParent( NULL );
			CILBitOp::TurnOff( &pLinkChildSS->m_dwExtraData, m_fLinkChild );
			pDBSet->m_DB.Update( pLinkChild, pLinkChildSS, &pTransaction );
			pDBSet->m_TC.Commit( pTransaction );
			delete pLinkChildSS;
		}

		else //we won't be using the transaction object.
			delete pTransaction;
	}	
}

void CVisualNotesApp::DisplayProperties()
{
	CView *pView = ((CMainFrame*)m_pMainWnd)->GetActiveView();
	CILTransaction *pTransaction = NULL;
	CILDBSet *pDBSet = GetDBSet();
	
	//****** DISPLAYING A TRACKED SYMBOLS PROPERTIES IS NO LONGER BEING IMPLEMENTED ********
	/*
	if( m_AppMode == CVisualNotesApp::APPMODE_TRACKSYMBOL )
	{
		CILSymbol *pTrackedSymbol = NULL;
		CILSymbol *pTrackedSS = NULL;
		CILSymbolDB *pDB = theApp.GetSymbolDatabase();
		
		if( pDBSet->m_DB.QueryTracked( &pTrackedSymbol ) )
		{
			pTrackedSymbol->GetSnapshot( &pTrackedSS );
			if( IDOK ==pTrackedSS->DoModalPropBox( pView ) )
			{
				pTransaction = new CILTransaction;
				pDBSet->m_DB.Update( pTrackedSymbol, pTrackedSS, &pTransaction );
				pDBSet->m_TC.Commit( pTransaction );
			}

			delete pTrackedSS;
		}
	}
	
	else
	*/
	
	if( m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL )
	{
		CILSymbol *pSelSymbol = NULL;
		CILSymbol *pSelectSS = NULL;
		CILList<CILSymbol*,CILSymbol*> SelectionList;
		
		if( pDBSet->m_DB.QuerySelected( &SelectionList ) )
		{
			if( SelectionList.GetCount() == 1 ) 
			{
				pSelSymbol = SelectionList.GetHead();
				pSelSymbol->GetSnapshot( &pSelectSS );
				if( IDOK == pSelectSS->DoModalPropBox( pView ) )
				{
					pTransaction = new CILTransaction;
					pDBSet->m_DB.Update( pSelSymbol, pSelectSS, &pTransaction );
					pDBSet->m_TC.Commit( pTransaction );
				}
				delete pSelectSS;
			}

			SelectionList.RemoveAll();
		}
	}
 
	else if( m_AppMode == CVisualNotesApp::APPMODE_CREATESYMBOL ||
			 m_AppMode == CVisualNotesApp::APPMODE_CREATELABEL )
	{
		CILSymbol *pLastShapeCreated = NULL;
		CILSymbol *pLastShapeCreatedSS = NULL;
		
		if( pDBSet->m_DB.QueryFlag( m_fLastCreatedShape, &pLastShapeCreated ) )
		{
			pLastShapeCreated->GetSnapshot( &pLastShapeCreatedSS );
			if( IDOK == pLastShapeCreatedSS->DoModalPropBox( pView ) )
			{
				pTransaction = new CILTransaction;
				pDBSet->m_DB.Update( pLastShapeCreated, pLastShapeCreatedSS, &pTransaction );
				pDBSet->m_TC.Commit( pTransaction );
			}

			delete pLastShapeCreatedSS;
		}
	}
}

void CVisualNotesApp::Delete()
{
	CILTransaction *pTransaction;
	CILSymbol *pTrackedSymbol = NULL;
	CILList<CILSymbol*,CILSymbol*> DeleteList, DeleteLinkList;
	CILDBSet *pSet = NULL;
	UINT UserChoice;
	CILDBSet *pDBSet = GetDBSet();

	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );

	CVisualNotesView *pView = (CVisualNotesView*)((CMainFrame*)m_pMainWnd)->GetActiveView();
	ASSERT( pView );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );

	if( m_AppMode == APPMODE_SELECTSYMBOL && pDBSet->m_DB.QuerySelected( &DeleteList ) )
	{
		//extract link symbols from the symbols selected for deletion.
		ILPOSITION Pos = DeleteList.GetHeadPosition();
		while( Pos )
		{
			if( DeleteList.GetAt( Pos )->GetEmbeddedID() )
				DeleteLinkList.AddTail( DeleteList.GetAt( Pos ) );
			DeleteList.GetNext( Pos );
		}
		
		//if one or more of the symbols selected for deletion are link-symbols,
		//give user a chance to revert action. 
		if( !DeleteLinkList.IsEmpty() )
		{
			UserChoice = ConfirmedRemoveLinks( &DeleteLinkList, 1 );
			
			//if user reverts, donot delete anything.
			if( UserChoice == IDCANCEL )
			{
				DeleteList.RemoveAll();
				DeleteLinkList.RemoveAll();
				return;
			}

			//if user wishes to leave the linking diagram intact, we must explicitly remove
			//link symbols from their respective diagram.
			else if( UserChoice == IDNO )
			{
				Pos = DeleteLinkList.GetHeadPosition();
				while( Pos )
				{
					pSet = pDoc->FindDBSet( DeleteLinkList.GetAt( Pos )->GetEmbeddedID() );
					pSet->RemoveLinkedSymbol( DeleteLinkList.GetAt( Pos ) );
					DeleteLinkList.GetAt( Pos )->SetEmbeddedID( ILDBSET_ID_NULL ); //Note: this update is NOT reversable.
					DeleteLinkList.GetNext( Pos );
				}
				
				pView->m_DViewStack.UpdateDeletedDiagrams( pDoc );
			}
		}
		
		//delete symbols and record this action.
		pTransaction = new CILTransaction;
		pDBSet->m_DB.Remove( &DeleteList, m_bRemoveChildren, &pTransaction );
		pDBSet->m_TC.Commit( pTransaction );			
		
		DeleteLinkList.RemoveAll();
		DeleteList.RemoveAll();
	}
	

	//**** DELETION OF A SYMBOL USING A TRACKER IS NO LONGER BEING IMPLEMENTED. **** //
	/*
	else if( m_AppMode == APPMODE_TRACKSYMBOL && pDBSet->m_DB.QueryTracked( &pTrackedSymbol ) )
	{
		//if the symbol selected for deletion is a link-symbol, give user a chance to revert action. 
		if( pTrackedSymbol->GetEmbeddedID() )
		{
			UserChoice = ConfirmedRemoveLinks( pTrackedSymbol, 1 );
			
			//if user reverts, donot delete anything.
			if( UserChoice == IDCANCEL )
				return;

			//if user wishes to leave the linking diagram intact, we must explicitly remove
			//link-symbol from the diagram.			
			else if( UserChoice == IDNO )
			{
				pSet = FindDBSet( pTrackedSymbol->GetEmbeddedID() );
				pSet->RemoveLinkedSymbol( pTrackedSymbol );
				pTrackedSymbol->SetEmbeddedID( ILDBSET_ID_NULL ); //Note: this update is not reversable
			}
		}
		
		//delete symbol and record this action.
		pTransaction = new CILTransaction;
		pDBSet->m_DB.Remove( pTrackedSymbol, m_bRemoveChildren, &pTransaction );
		pDBSet->m_TC.Commit( pTransaction );		
	}
	*/
}

void CVisualNotesApp::Undo()
{ GetSymbolTransactionCoord()->RollBack(); }

void CVisualNotesApp::Redo()
{ GetSymbolTransactionCoord()->RollFoward(); }

void CVisualNotesApp::OnChangeUI( WPARAM wParamNextCommand )
{
	
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;
	CVisualNotesView *pView = (CVisualNotesView*)pMainFrame->GetActiveView();
	CPoint CanvasOrigin(0,0);
	CILDBSet *pDBSet = GetDBSet();
	
	ASSERT( pView );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );

	CILList<CILSymbol*,CILSymbol*> m_SelectionList;
	CILSymbol *pSelected = NULL;
	CILSymbol *pTracked = NULL;
	CILSymbol *pPrevNeighbor = NULL;
	CILSymbol *pLinkChild = NULL;

	//if we are turning on the selection mode or we are turning off selection mode, 	//we need to invalidate all selected symbols, so their UI can be updated by the view.
	if( m_AppMode == APPMODE_SELECTSYMBOL && pDBSet->m_DB.QuerySelected( &m_SelectionList ) )
	{
		ILPOSITION Pos = m_SelectionList.GetHeadPosition();
		while( Pos )
		{
			pSelected = m_SelectionList.GetAt( Pos );
			pSelected->SetCanvasOrigin( CanvasOrigin );
			pSelected->ShowSelection( FALSE, pView );
			pSelected->InvalidateSymbol( pView );
			m_SelectionList.GetNext( Pos );
		}
	}

	//if  we are turning off track mode, 
	//we need to invalidate all selected symbols, so their UI can be updated by the view.
	else if( m_AppMode == APPMODE_TRACKSYMBOL && pDBSet->m_DB.QueryTracked( &pTracked ) )
	{
		pTracked->SetCanvasOrigin( CanvasOrigin );
		pTracked->ShowTracker( FALSE, pView );
		pTracked->InvalidateSymbol( pView );
	}

	//if we are selecting a new tool while in the middle of
	//creating an edge an edge creation mode, and there is 
	//a previous neighbor in the edge build,the "select-first-neighbor" 
	//was the last object added to the stack. We'll rollback that action.
	else if( m_AppMode == APPMODE_CREATEEDGE )
	{
		if( pDBSet->m_DB.QueryFlag( m_fPrevJointInBuild, &pPrevNeighbor ) )
		{
			CILBitOp::TurnOff( &pPrevNeighbor->m_dwExtraData, m_fPrevJointInBuild );
			pPrevNeighbor->InvalidateSymbol( pView );
			pDBSet->m_TC.Disreguard( 1 );
		}

		else if( pDBSet->m_DB.QueryFlag( m_fPrevShapeInBuild, &pPrevNeighbor ) )
		{
			CILBitOp::TurnOff( &pPrevNeighbor->m_dwExtraData, m_fPrevShapeInBuild );
			pPrevNeighbor->InvalidateSymbol( pView );
			pDBSet->m_TC.Disreguard( 1 );
		}
	}

	//if we are selecting a new tool while linking to a parent, then, 
	else if( m_AppMode == APPMODE_LINKSYMBOLS && 
			 pDBSet->m_DB.QueryFlag( m_fLinkChild, &pLinkChild ) )
	{
		CILBitOp::TurnOff( &pLinkChild->m_dwExtraData, m_fLinkChild );
		pLinkChild->InvalidateSymbol( pView );
		pDBSet->m_TC.Disreguard( 1 );
	}
}

void CVisualNotesApp::CreateSnapshotList( CILList<CILSymbol*,CILSymbol*> *pOriginalList, CILList<CILSymbol*,CILSymbol*> *pSnapshotList )
{
	CILSymbol *pNewSnapshot = NULL;

	ILPOSITION Pos = pOriginalList->GetHeadPosition();
	while( Pos )
	{
		pOriginalList->GetAt( Pos )->GetSnapshot( &pNewSnapshot );
		pSnapshotList->AddTail( pNewSnapshot );
		pOriginalList->GetNext( Pos );
		pNewSnapshot = NULL;
	}
}

void CVisualNotesApp::DeleteSnapshotList( CILList<CILSymbol*,CILSymbol*> *pSnapshotList )
{
	ILPOSITION Pos = pSnapshotList->GetHeadPosition();
	while( Pos )
	{
		delete pSnapshotList->GetAt( Pos );
		pSnapshotList->GetNext( Pos );
	}

	pSnapshotList->RemoveAll();
}

void CVisualNotesApp::SaveUserSettings()
{
	HKEY hAppKey, hSettingsKey;
	DWORD dwDisposition;
	CString strAppKey;

	//open up applciation key, create new one if it does not exist.
	RegOpenKeyEx( HKEY_LOCAL_MACHINE, m_strAppKey, 0, 0, &hAppKey );
	
	//Create new settings key.
	RegCreateKeyEx( hAppKey, TEXT( "Settings" ), 0, NULL, 0, 0, NULL, &hSettingsKey, &dwDisposition );
	
	//Save User's settings to registry.
	//=================================
	RegSetValueEx( hSettingsKey, TEXT("Canvas Width"), 0, REG_DWORD, (BYTE*)&m_CanvasSize.cx, sizeof( DWORD ) );
	RegSetValueEx( hSettingsKey, TEXT("Canvas Height"), 0, REG_DWORD, (BYTE*)&m_CanvasSize.cy, sizeof( DWORD ) );
	RegSetValueEx( hSettingsKey, TEXT("Unlink On CD"), 0, REG_BINARY, (BYTE*)&m_bUnlinkOnCD, sizeof( BOOL ) );
	RegSetValueEx( hSettingsKey, TEXT("Allow Streching" ), 0, REG_BINARY, (BYTE*)&m_bAllowStrechingBMP, sizeof( BOOL ) );
	RegSetValueEx( hSettingsKey, TEXT("Remove Children"), 0, REG_BINARY, (BYTE*)&m_bRemoveChildren, sizeof( BOOL ) );
	RegSetValueEx( hSettingsKey, TEXT("Auto Remove Link"), 0, REG_BINARY, (BYTE*)&m_bAutoRemoveLink, sizeof( BOOL ) );
	RegSetValueEx( hSettingsKey, TEXT("DirectEdgeAA"), 0, REG_BINARY, (BYTE*)&m_bDirectEdgeAA, sizeof( BOOL ) );

		
	RegCloseKey( hSettingsKey );
	RegCloseKey( hAppKey );
}

BOOL CVisualNotesApp::LoadUserSettings()
{ 
	HKEY hAppKey, hSettingsKey;
	DWORD dwSizeOfWord = sizeof( DWORD );
	DWORD dwSizeOfBool = sizeof( BOOL );

	//open up application key, create new one if it does not exist.
	RegOpenKeyEx( HKEY_LOCAL_MACHINE, m_strAppKey, 0, 0, &hAppKey );
	
	//if user did not save any previous settings, use default setings.
	if( ERROR_SUCCESS != RegOpenKeyEx( hAppKey, TEXT( "Settings" ), 0, 0, &hSettingsKey ) )
	{
		RegCloseKey( hAppKey ); //note, app key must be closed before restoring default settings.
		RestoreDefaultSettings();
		return FALSE;
	}		

	//Load User's saved settings from registry.
	//=========================================
	RegQueryValueEx( hSettingsKey, TEXT("Canvas Width"), 0, NULL, (BYTE*)&m_CanvasSize.cx, &dwSizeOfWord );
	RegQueryValueEx( hSettingsKey, TEXT("Canvas Height"), 0, NULL, (BYTE*)&m_CanvasSize.cy, &dwSizeOfWord );
	RegQueryValueEx( hSettingsKey, TEXT("Unlink On CD"), 0, NULL, (BYTE*)&m_bUnlinkOnCD, &dwSizeOfBool );
	RegQueryValueEx( hSettingsKey, TEXT("Allow Streching" ), 0, NULL, (BYTE*)&m_bAllowStrechingBMP, &dwSizeOfBool );
	RegQueryValueEx( hSettingsKey, TEXT("Remove Children"), 0, NULL, (BYTE*)&m_bRemoveChildren, &dwSizeOfBool );
	RegQueryValueEx( hSettingsKey, TEXT("Auto Remove Link"), 0, NULL, (BYTE*)&m_bAutoRemoveLink, &dwSizeOfBool );
	RegQueryValueEx( hSettingsKey, TEXT("DirectEdgeAA"), 0, NULL, (BYTE*)&m_bDirectEdgeAA, &dwSizeOfBool );
	
	RegCloseKey( hSettingsKey );
	RegCloseKey( hAppKey );

	return TRUE; 
}

void CVisualNotesApp::RestoreDefaultSettings()
{
	m_CanvasSize = m_DefCanvasSize;
	m_bRemoveChildren = m_bDefRemoveChildren;
	m_bUnlinkOnCD = m_bDefUnlinkOnCD;
	m_bAllowStrechingBMP = m_bDefAllowStrechingBMP;
	m_bAutoRemoveLink =m_bDefAutoRemoveLink;

	HKEY hAppKey;
	RegOpenKeyEx( HKEY_LOCAL_MACHINE, m_strAppKey, 0, 0, &hAppKey );
	RegDeleteKey( hAppKey, TEXT("Settings") );
	RegCloseKey( hAppKey );	
}

BOOL CVisualNotesApp::VerifyLicense()
{
	CTime TimeCurrent = CTime::GetCurrentTime();
	
	HKEY hLicenseKey, hAppKey;
	CString strLicense( TEXT("License" ) );
	CString strAuthVal( TEXT( "Authorization" ) );
	IL_AUTHORIZATION_INFO AuthInfo;
	DWORD dwDisposition;
	DWORD dwAuthDataType,dwAuthDataSize;
	
	//We will be checking the registry for authorization information.
	//---------------------------------------------------------------
	
	ZeroMemory( &AuthInfo, sizeof( IL_AUTHORIZATION_INFO ) );
	RegCreateKeyEx( HKEY_LOCAL_MACHINE, m_strAppKey, 0, NULL, 0, 0, NULL, &hAppKey, &dwDisposition );
	RegCreateKeyEx( hAppKey, strLicense, 0, NULL, 0, 0, NULL, &hLicenseKey, &dwDisposition );

	//If this is the first time user is running this application since the very first time user installed application,
	//create new licensing info.
	//(if a previous license key did not exist it means the application was never executed).
	
	if( dwDisposition == REG_CREATED_NEW_KEY )
	{
		//save time stamp so we can indicate when trial version will be over.
		TimeCurrent.GetAsSystemTime( AuthInfo.SysTime );

		AuthInfo.m_MajorVersion = VN_MAJOR_VERSION;
		AuthInfo.m_MinorVersion = VN_MINOR_VERSION;
		RegSetValueEx( hLicenseKey, strAuthVal, 0, REG_BINARY, (BYTE*)&AuthInfo, sizeof( IL_AUTHORIZATION_INFO ) );
	}
		
	else
	{
		dwAuthDataSize = sizeof( IL_AUTHORIZATION_INFO );
		RegQueryValueEx( hLicenseKey, strAuthVal, 0, &dwAuthDataType, (BYTE*)&AuthInfo, &dwAuthDataSize );
	}
	
	
	//if this software is registered, then we simply return "success".
	if( VerifyAuthorizationCode( AuthInfo.szAuthCode ) )
	{
		RegCloseKey( hLicenseKey );
		RegCloseKey( hAppKey );
		m_strUserName = AuthInfo.szUserName;
		m_strAuthCode = AuthInfo.szAuthCode;
		return TRUE;
	}

	//since this software is not registered, give user a chance to register.
	CRegistrationDlg RegDlg;
	UINT RegStatus = RegDlg.DoModal();
	if( RegStatus == IDREGISTER_SUCCESS )
	{		
		//registration was a success! Save new authorization info in system registry. 
		wcscpy( AuthInfo.szAuthCode, RegDlg.m_AuthCode );
		wcscpy( AuthInfo.szUserName, RegDlg.m_UserName );
		RegSetValueEx( hLicenseKey, strAuthVal, 0, REG_BINARY, (BYTE*)&AuthInfo, sizeof( IL_AUTHORIZATION_INFO ) );
		RegCloseKey( hLicenseKey );
		RegCloseKey( hAppKey );	

		m_strUserName = RegDlg.m_UserName;
		m_strAuthCode = RegDlg.m_AuthCode;
				
		::MessageBeep( MB_ICONINFORMATION );
		::MessageBox( NULL, TEXT( "Thank you for purchasing Visual Notes 1.0" ), TEXT( "Congratulations!" ), MB_OK );
		return TRUE;
	}

	//since authorization code did not pass, check to see if trial period is up.
	//if it is, indicate this to the user then return FALSE.
	CTime TimeSetup( AuthInfo.SysTime );
	CTimeSpan TrialSpan( VN_TRIAL_DAYS, VN_TRIAL_HOURS, VN_TRIAL_MINUTES, 0); 
	CTimeSpan SpanSetup;
	CTimeSpan SpanLeft;

	SpanSetup = TimeCurrent - TimeSetup;
	SpanLeft = TrialSpan - SpanSetup;

	//<if the time since intial setup is greater than the given trial length, trial period is over.>
	if( TrialSpan < SpanSetup )
	{
		::MessageBox( NULL, TEXT("Your trial period is over. You may purchase a registration key at www.handango.com."),
								TEXT("Registration Status."), MB_ICONINFORMATION );
		return FALSE;
	}			

	wchar_t szTrialLengthMsg[256];
	wsprintf( szTrialLengthMsg, TEXT("You have %d days left to use this trial version"), SpanLeft.GetDays() );
	::MessageBox( NULL, szTrialLengthMsg, TEXT( "Registration Status" ), MB_ICONINFORMATION );
	
	return TRUE;
}

BOOL CVisualNotesApp::VerifyAuthorizationCode( CString string )
{
	//for right now, we will just use static registration.
	//We'll get the dynamic hash information from our publisher, handango, in a latter.
	string.MakeUpper();
	return ( string == TEXT("NXFORLIFE") || string == TEXT("6X7EFY-DRBX-7F2B") );
}

void CVisualNotesApp::OnToolsSettings() 
{
	// TODO: Add your command handler code here
	CAppSettingsDlg SettingsDlg;
	CVisualNotesView *pView = (CVisualNotesView*)((CMainFrame*)m_pMainWnd)->GetActiveView();
	CILDBSet *pDBSet = GetDBSet();

	SettingsDlg.m_bAllowStrechingBMP = m_bAllowStrechingBMP;
	SettingsDlg.m_bRemoveChildren = m_bRemoveChildren;
	SettingsDlg.m_bUnlinkOnCD = m_bUnlinkOnCD;
	SettingsDlg.m_CanvasX = m_CanvasSize.cx;
	SettingsDlg.m_CanvasY = m_CanvasSize.cy;
	SettingsDlg.m_bAutoRemoveLink = m_bAutoRemoveLink;
	SettingsDlg.m_bDirectEdgeAA = m_bDirectEdgeAA;

	CString strTreeDir;
	strTreeDir.LoadString( IDS_TREEDIR_UP );
	SettingsDlg.m_strlstTreeDir.AddTail( strTreeDir );
	if( m_TreeDir == treedirUp ) {SettingsDlg.m_strTreeDirSel = strTreeDir; }
	
	strTreeDir.LoadString( IDS_TREEDIR_DOWN );
	SettingsDlg.m_strlstTreeDir.AddTail( strTreeDir );
	if( m_TreeDir == treedirDown ) { SettingsDlg.m_strTreeDirSel = strTreeDir; }
	
	strTreeDir.LoadString( IDS_TREEDIR_RIGHT );
	SettingsDlg.m_strlstTreeDir.AddTail( strTreeDir );
	if( m_TreeDir == treedirRight) { SettingsDlg.m_strTreeDirSel = strTreeDir; }

	strTreeDir.LoadString( IDS_TREEDIR_LEFT );
	SettingsDlg.m_strlstTreeDir.AddTail( strTreeDir );
	if( m_TreeDir == treedirLeft ) { SettingsDlg.m_strTreeDirSel = strTreeDir; }

	if( IDOK == SettingsDlg.DoModal() )
	{
		m_bRemoveChildren = SettingsDlg.m_bRemoveChildren;
		m_bAllowStrechingBMP = SettingsDlg.m_bAllowStrechingBMP;
		m_bUnlinkOnCD = SettingsDlg.m_bUnlinkOnCD;
		m_bAutoRemoveLink = SettingsDlg.m_bAutoRemoveLink;
		m_bDirectEdgeAA = SettingsDlg.m_bDirectEdgeAA;
				
		if( CSize( SettingsDlg.m_CanvasX, SettingsDlg.m_CanvasY ) != m_CanvasSize )
		{
			m_CanvasSize = CSize( SettingsDlg.m_CanvasX, SettingsDlg.m_CanvasY );
			pView->SetScrollSizes( MM_TEXT, m_CanvasSize );
		}

	
		strTreeDir.LoadString( IDS_TREEDIR_UP );
		if( strTreeDir == SettingsDlg.m_strTreeDirSel )
			m_TreeDir = treedirUp;

		strTreeDir.LoadString( IDS_TREEDIR_DOWN );
		if( strTreeDir == SettingsDlg.m_strTreeDirSel )
			m_TreeDir = treedirDown;

		strTreeDir.LoadString( IDS_TREEDIR_RIGHT );
		if( strTreeDir == SettingsDlg.m_strTreeDirSel )
			m_TreeDir = treedirRight;

		strTreeDir.LoadString( IDS_TREEDIR_LEFT );
		if( strTreeDir == SettingsDlg.m_strTreeDirSel )
			m_TreeDir = treedirLeft;
	
		if( SettingsDlg.m_bSaveSettings )
		{
			//if the settings we are saving are the default settings, 
			//we simply remove the "settings" key from the registry.
			//else, save users settings to the registry.
			if( SettingsDlg.m_bAllowStrechingBMP == m_bDefAllowStrechingBMP &&
				SettingsDlg.m_bRemoveChildren == m_bDefRemoveChildren &&
				SettingsDlg.m_bUnlinkOnCD == m_bDefUnlinkOnCD &&
				SettingsDlg.m_bDirectEdgeAA == m_bDirectEdgeAA &&
				CSize( SettingsDlg.m_CanvasX, SettingsDlg.m_CanvasY ) == m_DefCanvasSize )
			{
				RestoreDefaultSettings();
			}

			else
				SaveUserSettings();
		}
	}
}



UINT CVisualNotesApp::ConfirmedDeleteDBSet( UINT id )
{
	ILPOSITION Pos;
	CILList<CILDBSet*,CILDBSet*> OrphanList;
	CILList<CILSymbol*,CILSymbol*> LinkedSymbolList;
	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	CILDBSet *pSet = NULL;
	
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );

	pSet =  pDoc->FindDBSet( id );
	ASSERT( pSet );

	pSet->GetLinkedSymbols( &LinkedSymbolList );

	//Since this action cannot be undone, confirm deletion with user.
	//If user confirms, permanently delete diagram page (CILDBSet) from document.
	if( IDYES == m_pMainWnd->MessageBox( TEXT("Are you sure you wish to delete embedded diagram?"),
							 TEXT("Delete Confirmation"), MB_YESNO ) )
	{
		//check to see if any diagrams will be orphaned if this diagram set (particularly it's link-symbols).
		if( pDoc->PeekOrphanage( pSet, &LinkedSymbolList, &OrphanList ) && !m_bAutoRemoveLink )
		{
			//if there is a possible orphanage, allow user to either remove the orphans, leave the orphans
			//or cancel the deletion operation.
			int UserChoice = m_pMainWnd->MessageBox( TEXT( "One or more diagrams will be orphaned. Remove orphans also?" ),
										 TEXT( "Orphan Confirmation" ), MB_YESNOCANCEL );
			if( UserChoice == IDCANCEL )
				return IDCANCEL;

			else if( UserChoice == IDYES ) //delete dbset and all orphans as well.
			{
				pDoc->DeleteDBSet( id );
				Pos = OrphanList.GetHeadPosition();
				while( Pos )
				{
					pDoc->DeleteDBSet( OrphanList.GetAt( Pos )->GetID() );
					OrphanList.GetNext( Pos );
				}

				return IDYES;
			}

			else /* IDNO */ //just delete specified dbset.
			{
				pDoc->DeleteDBSet( id );
				return IDNO;
			}
		}

		else //delete dbset and delete orphans if any exist.
		{
			pDoc->DeleteDBSet( id );
			Pos = OrphanList.GetHeadPosition();
			while( Pos )
			{
				pDoc->DeleteDBSet( OrphanList.GetAt( Pos )->GetID() );
				OrphanList.GetNext( Pos );
			}

			//We simulate user response <see ConfirmedRemoveLinks for detailed info>
			UINT ConfirmID = ( OrphanList.IsEmpty() ) ? IDNO : IDYES;
			return ConfirmID;
		}
	}
	
	return IDCANCEL;
}


UINT CVisualNotesApp::ConfirmedRemoveLinks( CILSymbol *pSymbolRL, int ConfirmMode )
{
	CILList<CILSymbol*,CILSymbol*> ListRL;
	ListRL.AddTail( pSymbolRL );
	return ConfirmedRemoveLinks( &ListRL, ConfirmMode );
	ListRL.RemoveAll();
}

UINT CVisualNotesApp::ConfirmedRemoveLinks( CILList<CILSymbol*,CILSymbol*> *pListRL, int ConfirmMode )
{
	CString strInitialConfirmation;
	CILList<CILDBSet*,CILDBSet*> OrphanList;
	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	int UserChoice;
	
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );

	if( ConfirmMode == 0 )
		strInitialConfirmation = TEXT( "Are you sure you wish to remove these link symbols?" );
	else
		strInitialConfirmation = TEXT( "One or more symbols contain embedded links. Delete Anyway?" );

	if( IDYES == m_pMainWnd->MessageBox( strInitialConfirmation, TEXT( "Link Confirmation" ), MB_YESNO ) )
	{
		if( pDoc->PeekOrphanage( NULL, pListRL, &OrphanList ) && !m_bAutoRemoveLink )
		{
			UserChoice = m_pMainWnd->MessageBox( TEXT("One or more diagrams may be orphaned. Remove Orphans Also?"),
												 TEXT("Link Confirmation"), MB_YESNOCANCEL );
			if( UserChoice == IDCANCEL )
			{
				OrphanList.RemoveAll();
				return IDCANCEL;
			}

			else if( UserChoice == IDYES )					
			{
				ILPOSITION Pos = OrphanList.GetHeadPosition();
				while( Pos )
				{
					pDoc->DeleteDBSet( OrphanList.GetAt( Pos )->GetID() );
					OrphanList.GetNext(Pos);
				}

				OrphanList.RemoveAll();
				return IDYES; 
			}

			else /* UserChoice == IDNO */
			{
				OrphanList.RemoveAll();
				return IDNO;
			}
		}

		else
		{
			ILPOSITION Pos = OrphanList.GetHeadPosition();
			while( Pos )
			{
				pDoc->DeleteDBSet( OrphanList.GetAt( Pos )->GetID() );
				OrphanList.GetNext(Pos);
			}
			
			//IDYES indicates that orphan diagrams have been deleted ( all orphans deleted ).
			//IDNO indicates that orphan diagrams have been left intact ( no orphans deleted ).
			//Since in this condition removal is automatic, we need to simulate a user-response.
			UINT ConfirmID = ( OrphanList.IsEmpty() ) ? IDNO : IDYES;
			OrphanList.RemoveAll();
			return ConfirmID; 
		}
	}

	return IDCANCEL; //IDCANCEL means user wishes to revert what ever action is about to take place outside this method.
}

void CVisualNotesApp::SetLastCreatedShape( CILSymbol *pSymbol )
{
	CILSymbol *pLastCreatedShape = NULL;
	CILDBSet *pDBSet = GetDBSet();

	if( pDBSet->m_DB.QueryFlag( m_fLastCreatedShape, &pLastCreatedShape ) )
		CILBitOp::TurnOff( &pLastCreatedShape->m_dwExtraData, m_fLastCreatedShape );

	CILBitOp::TurnOn( &pSymbol->m_dwExtraData, m_fLastCreatedShape );
}

void CVisualNotesApp::AlignSymbols( ALIGNTYPE AlignType )
{	
	CILList<CILSymbol*,CILSymbol*> SelList, AlignList;
	CILList<CILSymbol*,CILSymbol*> JointList, JointListSS, ChildList, ChildListSS;
	CILSymbol *pSelSymbol = NULL;
	ILPOSITION Pos;
	int nOffset;
	CRect NewBoundingRect;
	CPoint ptControlCenter, ptAlignCenter;
	
	CILSymbol *pControlShape = NULL;
	CILSymbol *pAlignShape = NULL, *pAlignShapeSS = NULL;
	CILTransaction *pTransaction;
	CILDBSet *pDBSet = GetDBSet();
	
	pDBSet->m_DB.QuerySelected( &SelList );

	// Weed out non-prospective symbols for this operation.
	//only non-joint symbols may be aligned.
	Pos = SelList.GetHeadPosition();
	while( Pos )
	{
		pSelSymbol = SelList.GetAt( Pos );
		if( !pSelSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
		{
			//only non-bitmap symbols can be size-aligned. 
			//NOTE: a bitmap is allowed to be the control shape in this operation.
			if( AlignType != AS_SIZE || !pSelSymbol->IsKindOf( RUNTIME_CLASS( CILBitmapShape ) ) )
				AlignList.AddTail( pSelSymbol ); //notice we are keeping order intact.
		}

		SelList.GetNext( Pos );
	}

	//At least 2 prospects must exist for this operation.
	if( AlignList.GetCount() < 2 )
		return;
	
	//since there is a selection, a control shape should exist, but let's make sure anyway!
	if( !pDBSet->m_DB.QueryFlag( m_fControlShape, &pControlShape ) )
		return;	

	//take out control shape from align list, if it is part of align list.
	if( (Pos = AlignList.Find( pControlShape )) )
		AlignList.RemoveAt( Pos );

	//*********************************************************************************************************
	//VERY VERY IMPORTANT:
	//At the time of this implemenatation. A child was ALWAYS infront of it's parent in Z-Order.
	//since QuerySelected() returns symbols in Z-Order, this method performs alignment operations on symbols
	//from back to front in the Z-Order. This meant that parent and children could be in alignment list and 
	//aligned similary without hierarchial displacement. However, if a child is a control shape and it's parent
	//is an align symbol, alignment does not work.
	//**********************************************************************************************************
			
	switch( AlignType )
	{
	case AS_TOP:
		pTransaction = new CILTransaction;
		Pos = AlignList.GetHeadPosition();
		while( Pos )
		{
			pAlignShape = AlignList.GetAt( Pos );
			
			nOffset = pControlShape->m_BoundingRect.top - pAlignShape->m_BoundingRect.top;
			
			pAlignShape->GetSnapshot( &pAlignShapeSS );
			NewBoundingRect = pAlignShapeSS->m_BoundingRect;
			NewBoundingRect.OffsetRect( 0, nOffset );
			pAlignShapeSS->SetBoundingRect( NewBoundingRect );
			
			pAlignShape->GetJoints( &JointList, TRUE );
			pAlignShape->GetChildren( &ChildList, TRUE );
			CreateSnapshotList( &JointList, &JointListSS );
			CreateSnapshotList( &ChildList, &ChildListSS );
			OffsetList( CSize( 0, nOffset ), &JointListSS );
			OffsetList( CSize( 0, nOffset ), &ChildListSS );

			pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );
			pDBSet->m_DB.Update( &ChildList, &ChildListSS, &pTransaction );
			pDBSet->m_DB.Update( pAlignShape, pAlignShapeSS, &pTransaction );			
			
			DeleteSnapshotList( &JointListSS );
			DeleteSnapshotList( &ChildListSS );
			ChildList.RemoveAll();
			JointList.RemoveAll();
			delete pAlignShapeSS; pAlignShapeSS = NULL;
			
			AlignList.GetNext( Pos );
		}		

		pDBSet->m_TC.Commit( pTransaction );		
		break;

	case AS_RIGHT:

		pTransaction = new CILTransaction;
		Pos = AlignList.GetHeadPosition();
		while( Pos )
		{
			pAlignShape = AlignList.GetAt( Pos );
			
			nOffset = pControlShape->m_BoundingRect.right - pAlignShape->m_BoundingRect.right;
			
			pAlignShape->GetSnapshot( &pAlignShapeSS );
			NewBoundingRect = pAlignShapeSS->m_BoundingRect;
			NewBoundingRect.OffsetRect( nOffset, 0 );
			pAlignShapeSS->SetBoundingRect( NewBoundingRect );
			
			pAlignShape->GetJoints( &JointList, TRUE );
			pAlignShape->GetChildren( &ChildList, TRUE );
			CreateSnapshotList( &JointList, &JointListSS );
			CreateSnapshotList( &ChildList, &ChildListSS );
			OffsetList( CSize( nOffset, 0 ), &JointListSS );
			OffsetList( CSize( nOffset, 0 ), &ChildListSS );
		
			pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );
			pDBSet->m_DB.Update( &ChildList, &ChildListSS, &pTransaction );
			pDBSet->m_DB.Update( pAlignShape, pAlignShapeSS, &pTransaction );
						
			DeleteSnapshotList( &JointListSS );
			DeleteSnapshotList( &ChildListSS );
			ChildList.RemoveAll();
			JointList.RemoveAll();
			delete pAlignShapeSS; pAlignShapeSS = NULL;
			
			AlignList.GetNext( Pos );
		}		
		
		pDBSet->m_TC.Commit( pTransaction );
		break;		

	case AS_BOTTOM:

		pTransaction = new CILTransaction;
		Pos = AlignList.GetHeadPosition();
		while( Pos )
		{
			pAlignShape = AlignList.GetAt( Pos );
			
			nOffset = pControlShape->m_BoundingRect.bottom - pAlignShape->m_BoundingRect.bottom;
			
			pAlignShape->GetSnapshot( &pAlignShapeSS );
			NewBoundingRect = pAlignShapeSS->m_BoundingRect;
			NewBoundingRect.OffsetRect( 0, nOffset );
			pAlignShapeSS->SetBoundingRect( NewBoundingRect );
			
			pAlignShape->GetJoints( &JointList, TRUE );
			pAlignShape->GetChildren( &ChildList, TRUE );
			CreateSnapshotList( &JointList, &JointListSS );
			CreateSnapshotList( &ChildList, &ChildListSS );
			OffsetList( CSize( 0, nOffset ), &JointListSS );
			OffsetList( CSize( 0, nOffset ), &ChildListSS );
			
			pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );
			pDBSet->m_DB.Update( &ChildList, &ChildListSS, &pTransaction );
			pDBSet->m_DB.Update( pAlignShape, pAlignShapeSS, &pTransaction );
						
			DeleteSnapshotList( &JointListSS );
			DeleteSnapshotList( &ChildListSS );
			ChildList.RemoveAll();
			JointList.RemoveAll();
			delete pAlignShapeSS; pAlignShapeSS = NULL;
			
			AlignList.GetNext( Pos );
		}	
		
		pDBSet->m_TC.Commit( pTransaction );	
		break;

	case AS_LEFT:

		pTransaction = new CILTransaction;
		Pos = AlignList.GetHeadPosition();
		while( Pos )
		{
			pAlignShape = AlignList.GetAt( Pos );
			
			nOffset = pControlShape->m_BoundingRect.left - pAlignShape->m_BoundingRect.left;
			
			pAlignShape->GetSnapshot( &pAlignShapeSS );
			NewBoundingRect = pAlignShapeSS->m_BoundingRect;
			NewBoundingRect.OffsetRect( nOffset, 0 );
			pAlignShapeSS->SetBoundingRect( NewBoundingRect );
			
			pAlignShape->GetJoints( &JointList, TRUE );
			pAlignShape->GetChildren( &ChildList, TRUE );
			CreateSnapshotList( &JointList, &JointListSS );
			CreateSnapshotList( &ChildList, &ChildListSS );
			OffsetList( CSize( nOffset, 0 ), &JointListSS );
			OffsetList( CSize( nOffset, 0 ), &ChildListSS );
			
			pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );
			pDBSet->m_DB.Update( &ChildList, &ChildListSS, &pTransaction );
			pDBSet->m_DB.Update( pAlignShape, pAlignShapeSS, &pTransaction );
				
			
			DeleteSnapshotList( &JointListSS );
			DeleteSnapshotList( &ChildListSS );
			ChildList.RemoveAll();
			JointList.RemoveAll();
			delete pAlignShapeSS; pAlignShapeSS = NULL;
			
			AlignList.GetNext( Pos );
		}
		
		pDBSet->m_TC.Commit( pTransaction );
		break;

	case AS_SIZE:

		pTransaction = new CILTransaction;
		Pos = AlignList.GetHeadPosition();
		while( Pos )
		{
			pAlignShape = AlignList.GetAt( Pos );

			pAlignShape->GetSnapshot( &pAlignShapeSS );
			NewBoundingRect = pAlignShapeSS->m_BoundingRect;
			NewBoundingRect.right = NewBoundingRect.left + pControlShape->m_BoundingRect.Width();
			NewBoundingRect.bottom = NewBoundingRect.top + pControlShape->m_BoundingRect.Height();
			pAlignShapeSS->GetJoints( &JointList );
			CreateSnapshotList( &JointList, &JointListSS );
			pAlignShapeSS->Resize( NewBoundingRect, &JointListSS );

			pDBSet->m_DB.Update( pAlignShape, pAlignShapeSS, &pTransaction );
			pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );

			DeleteSnapshotList( &JointListSS );
			JointList.RemoveAll();
			delete pAlignShapeSS; pAlignShapeSS = NULL;

			AlignList.GetNext( Pos );		
		}

		pDBSet->m_TC.Commit( pTransaction );
		break;

	case AS_CENTER:

		pTransaction = new CILTransaction;
		Pos = AlignList.GetHeadPosition();
		while( Pos )
		{
			pAlignShape = AlignList.GetAt( Pos );

			pAlignShape->GetSnapshot( &pAlignShapeSS );
			NewBoundingRect = pAlignShapeSS->m_BoundingRect;

			ptControlCenter = CPoint( pControlShape->m_BoundingRect.left + pControlShape->m_BoundingRect.Width() / 2,
									  pControlShape->m_BoundingRect.top + pControlShape->m_BoundingRect.Height() /2 );
			ptAlignCenter = CPoint( NewBoundingRect.left + NewBoundingRect.Width() / 2,
									NewBoundingRect.top + NewBoundingRect.Height() / 2 );

			NewBoundingRect.OffsetRect( ptControlCenter - ptAlignCenter );
			
			pAlignShapeSS->GetJoints( &JointList, TRUE );
			pAlignShapeSS->GetChildren( &ChildList, TRUE );
			CreateSnapshotList( &JointList, &JointListSS );
			CreateSnapshotList( &ChildList, &ChildListSS );

			pAlignShapeSS->SetBoundingRect( NewBoundingRect );
			OffsetList( ptControlCenter - ptAlignCenter, &JointListSS ); 
			OffsetList( ptControlCenter - ptAlignCenter, &ChildListSS );

			pDBSet->m_DB.Update( pAlignShape, pAlignShapeSS, &pTransaction );
			pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );
			pDBSet->m_DB.Update( &ChildList, &ChildListSS, &pTransaction );

			DeleteSnapshotList( &JointListSS );
			DeleteSnapshotList( &ChildListSS );
			JointList.RemoveAll();
			ChildList.RemoveAll();
			delete pAlignShapeSS; pAlignShapeSS = NULL;

			pDBSet->m_DB.ZOrderSymbol( pAlignShape );

			AlignList.GetNext( Pos );
		}
		
		pDBSet->m_TC.Commit( pTransaction );
		break;
	}
}

void CVisualNotesApp::OffsetList( CSize Offset, CILList<CILSymbol*,CILSymbol*> *pList )
{
	CRect NewBoundingRect;
	CILSymbol *pSymbol = NULL;

	ILPOSITION Pos = pList->GetHeadPosition();
	while( Pos )
	{
		pSymbol = pList->GetAt( Pos );
		NewBoundingRect = pSymbol->m_BoundingRect;
		NewBoundingRect.OffsetRect( Offset );
		pSymbol->SetBoundingRect( NewBoundingRect );
		
		pList->GetNext( Pos );
	}
}

void CVisualNotesApp::OnToolsDiagram() 
{
	// TODO: Add your command handler code here
	DoModalDiagramView( CDiagramViewDlg::modeNormal, NULL );
}

void CVisualNotesApp::DoModalDiagramView( int DVMode, CILSymbol *pAttachSymbol )
{
	CDiagramViewDlg DViewDlg;
	CString strDiagramName;
	CILDBSet *pSet = NULL;
	CVisualNotesView *pView = (CVisualNotesView*) ((CMainFrame*)m_pMainWnd)->GetActiveView();
	CILList<CILDBSet*,CILDBSet*> OrphanList;
	CILList<CILSymbol*,CILSymbol*> LinkedSymbolList;
	CILDBSet *pDBSet = GetDBSet();
	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );

	ILPOSITION Pos = pDoc->m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		pDoc->m_DBSetList.GetAt( Pos )->GetName( strDiagramName );
		
		//NOTE: diagram view expects string list and idlist to be same size always.
		DViewDlg.m_strlistDiagrams.AddTail( strDiagramName );
		DViewDlg.m_idlistDiagrams.AddTail( pDoc->m_DBSetList.GetAt( Pos )->GetID() );
		
		pDoc->m_DBSetList.GetNext( Pos );
	}

	pDBSet->GetName( DViewDlg.m_strCurrentDiagram );
	DViewDlg.m_DVMode = ( enum CDiagramViewDlg::DVMODE ) DVMode;
	
	if( IDOK == DViewDlg.DoModal() )
	{
		//Update all diagram views that have been renamed.
		//------------------------------------------------
		ILPOSITION StrPos = DViewDlg.m_strlistDiagrams.GetHeadPosition();
		ILPOSITION DatPos = DViewDlg.m_idlistDiagrams.GetHeadPosition();
		while( DatPos )
		{
			UINT id = DViewDlg.m_idlistDiagrams.GetAt( DatPos ); 
			CString string = DViewDlg.m_strlistDiagrams.GetAt( StrPos );
			pSet = pDoc->FindDBSet( id );
			CString dbstring;

			pSet->GetName( dbstring );
			if( string != dbstring )
				pSet->SetName( string );

			DViewDlg.m_strlistDiagrams.GetNext( StrPos );
			DViewDlg.m_idlistDiagrams.GetNext( DatPos );
		}

		//if we are in attach mode, embed the selected diagram in the attach-symbol.
		if( DVMode == CDiagramViewDlg::modeAttach )
		{
			ASSERT( pAttachSymbol );
			//there may have been no diagrams to select ( current diagram not included ).
			if( DViewDlg.m_nSelectedID != ILDBSET_ID_NULL )
			{
				pAttachSymbol->SetEmbeddedID( DViewDlg.m_nSelectedID );
				pSet = pDoc->FindDBSet( DViewDlg.m_nSelectedID );
				pSet->AddLinkedSymbol( pAttachSymbol );
			}		
		}

		//else delete diagram user specified for deletion and/or visit the diagram user
		//wishes to jump to (so long as it's not the current diagram).
		else
		{
			ILPOSITION Pos = pDoc->m_DBSetList.GetHeadPosition();
			while( Pos )
			{
				pSet = pDoc->m_DBSetList.GetAt( Pos );
				if( !DViewDlg.m_idlistDiagrams.Find( pSet->GetID() ) )
				{
					if( m_bAutoRemoveLink )
					{
						pSet->GetLinkedSymbols( &LinkedSymbolList );
						pDoc->PeekOrphanage( pSet, &LinkedSymbolList, &OrphanList );
						pDoc->DeleteDBSet( pSet->GetID() );
						ILPOSITION OrphPos = OrphanList.GetHeadPosition();
						while( OrphPos )
						{
							pDoc->DeleteDBSet( OrphanList.GetAt( OrphPos )->GetID() );
							OrphanList.GetNext( OrphPos );
						}
					}

					else
						pDoc->DeleteDBSet( pSet->GetID() );
					
					pDoc->SetCurrentDBSet( pView->m_DViewStack.UpdateDeletedDiagrams( pDoc ) );
					Pos = pDoc->m_DBSetList.GetHeadPosition(); //reset search.
					continue;
				}

				pDoc->m_DBSetList.GetNext( Pos );
			}

			if( DViewDlg.m_bVisitSelected && 
				DViewDlg.m_nSelectedID != ILDBSET_ID_NULL && 
				DViewDlg.m_nSelectedID != pDBSet->GetID() )
			{
				pDoc->SetCurrentDBSet( DViewDlg.m_nSelectedID );
				pView->m_DViewStack.PushNewSite( DViewDlg.m_nSelectedID );
			}
		}

		pDoc->SetModifiedFlag();
	}
}

void CVisualNotesApp::JointListToSymbolList( CILList<CILJoint*,CILJoint*> *pJointList, 
											  CILList<CILSymbol*,CILSymbol*> *pSymbolList )
{
	ILPOSITION Pos = pJointList->GetHeadPosition();
	while( Pos )
	{
		pSymbolList->AddTail( pJointList->GetAt( Pos ) );
		pJointList->GetNext( Pos );
	}
}

void CVisualNotesApp::MultiSelectMove( CSize Offset )
{
	CILList<CILSymbol*,CILSymbol*> SelectionList;
	CILList<CILSymbol*,CILSymbol*> ChildList, ChildListSS, JointList, JointListSS;
	CILSymbol *pSelSymbol = NULL,	*pSelSnapshot = NULL;
	CILTransaction *pTransaction = NULL;
	CRect NewBoundingRect;
	CILDBSet *pDBSet = GetDBSet();

	//if there are no selected symbols, no need to bother performing this operation.
	if( !pDBSet->m_DB.QuerySelected( &SelectionList ) )
		return;
	
	pTransaction = new CILTransaction;

	ILPOSITION Pos = SelectionList.GetHeadPosition();
	while( Pos )
	{
		pSelSymbol = SelectionList.GetAt( Pos );
		pSelSymbol->GetSnapshot( &pSelSnapshot );

		pSelSnapshot->GetChildren( &ChildList, TRUE );
		pSelSnapshot->GetJoints( &JointList, TRUE );
		CreateSnapshotList( &JointList, &JointListSS );
		CreateSnapshotList( &ChildList, &ChildListSS );
		
		NewBoundingRect = pSelSnapshot->m_BoundingRect;
		NewBoundingRect.OffsetRect( Offset );

		pSelSnapshot->SetBoundingRect( NewBoundingRect );
		OffsetList( Offset, &JointListSS );
		OffsetList( Offset, &ChildListSS );

		pDBSet->m_DB.Update( pSelSymbol, pSelSnapshot, &pTransaction );
		pDBSet->m_DB.Update( &JointList, &JointListSS, &pTransaction );
		pDBSet->m_DB.Update( &ChildList, &ChildListSS, &pTransaction );
		
		DeleteSnapshotList( &JointListSS );
		DeleteSnapshotList( &ChildListSS );
		JointList.RemoveAll();
		ChildList.RemoveAll();
		delete pSelSnapshot; pSelSnapshot = NULL;

		SelectionList.GetNext( Pos );
	}

	pDBSet->m_TC.Commit( pTransaction );
}

CILDBSet* CVisualNotesApp::GetDBSet()
{
	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );
	ASSERT( pDoc->m_pDBSet );
	return pDoc->m_pDBSet;
}

CILSymbolDB* CVisualNotesApp::GetSymbolDatabase()
{ return &GetDBSet()->m_DB; }

CILTransactionCoord* CVisualNotesApp::GetSymbolTransactionCoord()
{ return &GetDBSet()->m_TC; }

CILDBSet* CVisualNotesApp::FindDBSet( UINT id )
{
	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );

	return pDoc->FindDBSet( id );
}

BOOL CVisualNotesApp::MultiSelectSymbol( CRect *pRect )
{
	CILList<CILSymbol*,CILSymbol*> *pSymbolList = NULL; 
	CILList<CILSymbol*,CILSymbol*> MultiSelectList;
	CILSymbol *pSymbol = NULL, *pSymbolSS = NULL;
	CRect rectSymbol;
	CVisualNotesView *pView = (CVisualNotesView*)((CMainFrame*)m_pMainWnd)->GetActiveView();

	CVisualNotesDoc *pDoc = (CVisualNotesDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf( RUNTIME_CLASS( CVisualNotesDoc ) ) );
	
	ASSERT( pView );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );

	pSymbolList = pDoc->m_pDBSet->m_DB.GetSymbolTable();

	ILPOSITION Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		pSymbol = pSymbolList->GetAt( Pos );
		rectSymbol = pSymbol->m_BoundingRect;
		pView->CanvasToClient( &rectSymbol );
		
		//only select unselected, non-joint, top level symbols whose bounding rect lie 
		//completely inside the muliti-pick-rect.
		if( !pSymbol->GetParent() && 
			!pSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) &&
			!pSymbol->IsSelected() &&
			rectSymbol.top > pRect->top &&
			rectSymbol.bottom < pRect->bottom &&
			rectSymbol.right < pRect->right &&
			rectSymbol.left > pRect->left )
		{
			MultiSelectList.AddTail( pSymbol );
		}
			
		pSymbolList->GetNext( Pos );
	}

	if( !MultiSelectList.IsEmpty() )
	{
		CILList<CILSymbol*,CILSymbol*> MultiSelectListSS;
		CreateSnapshotList( &MultiSelectList, &MultiSelectListSS );

		CILTransaction *pTransaction = new CILTransaction;
		Pos = MultiSelectListSS.GetHeadPosition();
		while( Pos )
		{
			pSymbol= MultiSelectListSS.GetAt( Pos );
			pSymbol->SetCanvasOrigin( pView->CanvasToClient( &CPoint( 0, 0 ) ) );
			pSymbol->ShowSelection( TRUE, pView );			
			MultiSelectListSS.GetNext( Pos );
		}

		pDoc->m_pDBSet->m_DB.Update( &MultiSelectList, &MultiSelectListSS, &pTransaction );
		pDoc->m_pDBSet->m_TC.Commit( pTransaction );

		DeleteSnapshotList( &MultiSelectListSS );
		MultiSelectList.RemoveAll();

		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();		// Added for WCE apps
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVisualNotesApp::OnAppAbout()
{
	CAboutDlg1 aboutDlg;
	aboutDlg.m_strAuthCode = m_strAuthCode;
	aboutDlg.m_strUserName = m_strUserName;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesApp commands
// Added for WCE apps

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CenterWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
