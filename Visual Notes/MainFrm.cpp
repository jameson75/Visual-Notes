// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "stdil.h"
#include "Visual Notes.h"
#include "MainFrm.h"
#include "ColorButton.h"
#include "ILColorPicker.h"
#include "ILShapePropBox.h"
#include "Visual NotesDoc.h"
#include "Visual NotesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define PSPC_TOOLBAR_HEIGHT 24

const DWORD dwAdornmentFlags = 0; // exit button
extern CVisualNotesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_TOOLBTN_CIRCLE, OnToolbtnCircle)
	ON_COMMAND(ID_TOOLBTN_DIAMOND, OnToolbtnDiamond)
	ON_COMMAND(ID_TOOLBTN_RECTANGLE, OnToolbtnRectangle)
	ON_COMMAND(ID_TOOLBTN_SHEAR, OnToolbtnShear)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_CIRCLE, OnUpdateToolbtnCircle)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_DIAMOND, OnUpdateToolbtnDiamond)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_RECTANGLE, OnUpdateToolbtnRectangle)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_SHEAR, OnUpdateToolbtnShear)
	ON_COMMAND(ID_TOOLBTN_TEXT, OnToolbtnText)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_TEXT, OnUpdateToolbtnText)
	ON_COMMAND(ID_TOOLBTN_SELECT, OnToolbtnSelect)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_SELECT, OnUpdateToolbtnSelect)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_CREATE, OnUpdateToolbtnCreate)
	ON_COMMAND(ID_TOOLBTN_CREATE, OnToolbtnCreate)
	ON_COMMAND(ID_TOOLBTN_PROPERTIES, OnToolbtnProperties)
	ON_COMMAND(ID_TOOLBTN_DELETE, OnToolbtnDelete)
	ON_COMMAND(ID_TOOLBTN_LINK, OnToolbtnLink)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_LINK, OnUpdateToolbtnLink)
	ON_COMMAND(ID_TOOLBTN_EDGE, OnToolbtnEdge)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_EDGE, OnUpdateToolbtnEdge)
	ON_COMMAND(ID_TOOLBTN_TRACK, OnToolbtnTrack)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_TRACK, OnUpdateToolbtnTrack)
	ON_COMMAND(ID_TOOLBTN_SHOWHIDETOOLS, OnToolbtnShowhidetools)
	ON_COMMAND(ID_TOOLBTN_BASICEDGE, OnToolbtnBasicedge)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_BASICEDGE, OnUpdateToolbtnBasicedge)
	ON_COMMAND(ID_TOOLBTN_CMPLXEDGE, OnToolbtnCmplxedge)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_CMPLXEDGE, OnUpdateToolbtnCmplxedge)
	ON_COMMAND(ID_TOOLBTN_REDO, OnToolbtnRedo)
	ON_COMMAND(ID_TOOLBTN_UNDO, OnToolbtnUndo)
	ON_COMMAND(ID_TOOLBTN_UMLACTOR, OnToolbtnUmlactor)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UMLACTOR, OnUpdateToolbtnUmlactor)
	ON_COMMAND(ID_TOOLBTN_UMLCASE, OnToolbtnUmlcase)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UMLCASE, OnUpdateToolbtnUmlcase)
	ON_COMMAND(ID_TOOLBTN_UMLCLASS, OnToolbtnUmlclass)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UMLCLASS, OnUpdateToolbtnUmlclass)
	ON_COMMAND(ID_TOOLBTN_UMLCOMPONENT, OnToolbtnUmlcomponent)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UMLCOMPONENT, OnUpdateToolbtnUmlcomponent)
	ON_COMMAND(ID_TOOLBTN_UMLNOTE, OnToolbtnUmlnote)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UMLNOTE, OnUpdateToolbtnUmlnote)
	ON_COMMAND(ID_TOOLBTN_UMLPACKAGE, OnToolbtnUmlpackage)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UMLPACKAGE, OnUpdateToolbtnUmlpackage)
	ON_COMMAND(ID_TOOLBTN_NET2CAMERA, OnToolbtnNet2camera)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2CAMERA, OnUpdateToolbtnNet2camera)
	ON_COMMAND(ID_TOOLBTN_NET2CELLPHONE, OnToolbtnNet2cellphone)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2CELLPHONE, OnUpdateToolbtnNet2cellphone)
	ON_COMMAND(ID_TOOLBTN_NET2DISH, OnToolbtnNet2dish)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2DISH, OnUpdateToolbtnNet2dish)
	ON_COMMAND(ID_TOOLBTN_NET2FAX, OnToolbtnNet2fax)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2FAX, OnUpdateToolbtnNet2fax)
	ON_COMMAND(ID_TOOLBTN_NET2PDA, OnToolbtnNet2pda)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2PDA, OnUpdateToolbtnNet2pda)
	ON_COMMAND(ID_TOOLBTN_NET2SATELLITE, OnToolbtnNet2satellite)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2SATELLITE, OnUpdateToolbtnNet2satellite)
	ON_COMMAND(ID_TOOLBTN_NET2THINKPAD, OnToolbtnNet2thinkpad)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2THINKPAD, OnUpdateToolbtnNet2thinkpad)
	ON_COMMAND(ID_TOOLBTN_NET2WIREPHONE, OnToolbtnNet2wirephone)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NET2WIREPHONE, OnUpdateToolbtnNet2wirephone)
	ON_COMMAND(ID_TOOLBTN_NETCLIENT, OnToolbtnNetclient)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETCLIENT, OnUpdateToolbtnNetclient)
	ON_COMMAND(ID_TOOLBTN_NETCLOUD, OnToolbtnNetcloud)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETCLOUD, OnUpdateToolbtnNetcloud)
	ON_COMMAND(ID_TOOLBTN_NETDATABASE, OnToolbtnNetdatabase)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETDATABASE, OnUpdateToolbtnNetdatabase)
	ON_COMMAND(ID_TOOLBTN_NETHUB, OnToolbtnNethub)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETHUB, OnUpdateToolbtnNethub)
	ON_COMMAND(ID_TOOLBTN_NETMAINFRAME, OnToolbtnNetmainframe)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETMAINFRAME, OnUpdateToolbtnNetmainframe)
	ON_COMMAND(ID_TOOLBTN_NETPRINTER, OnToolbtnNetprinter)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETPRINTER, OnUpdateToolbtnNetprinter)
	ON_COMMAND(ID_TOOLBTN_NETSERVER, OnToolbtnNetserver)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETSERVER, OnUpdateToolbtnNetserver)
	ON_COMMAND(ID_TOOLBTN_DEPBASICEDGE, OnToolbtnDepbasicedge)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_DEPBASICEDGE, OnUpdateToolbtnDepbasicedge)
	ON_COMMAND(ID_TOOLBTN_ALIGN_BOTTOM, OnToolbtnAlignBottom)
	ON_COMMAND(ID_TOOLBTN_ALIGN_LEFT, OnToolbtnAlignLeft)
	ON_COMMAND(ID_TOOLBTN_ALIGN_SIZE, OnToolbtnAlignSize)
	ON_COMMAND(ID_TOOLBTN_ALIGN_TOP, OnToolbtnAlignTop)
	ON_COMMAND(ID_TOOBTN_ALIGN_RIGHT, OnToobtnAlignRight)
	ON_COMMAND(ID_TOOLBTN_GENERALIZE, OnToolbtnGeneralize)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_GENERALIZE, OnUpdateToolbtnGeneralize)
	ON_COMMAND(ID_TOOLBTN_TAGA, OnToolbtnTaga)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_TAGA, OnUpdateToolbtnTaga)
	ON_COMMAND(ID_TOOLBTN_TAGB, OnToolbtnTagb)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_TAGB, OnUpdateToolbtnTagb)
	ON_COMMAND(ID_TOOLBTN_DEPLOYMENT, OnToolbtnDeployment)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_DEPLOYMENT, OnUpdateToolbtnDeployment)
	ON_COMMAND(ID_TOOLBTN_ROUNDRECT, OnToolbtnRoundrect)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_ROUNDRECT, OnUpdateToolbtnRoundrect)
	ON_COMMAND(ID_TOOLBTN_CAPSULE, OnToolbtnCapsule)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_CAPSULE, OnUpdateToolbtnCapsule)
	ON_COMMAND(ID_TOOLBTN_ALIGNCENTER, OnToolbtnAligncenter)
	ON_COMMAND(ID_TOOLBTN_SHOW_HIDDEN, OnToolbtnShowHidden)
	ON_COMMAND(ID_TOOLBTN_HIDE_SELECTED, OnToolbtnHideSelected)
	ON_NOTIFY( NM_CUSTOMDRAW, IDUSER_TOOLBAR_GENERAL, OnHighlightToolbtn )
	ON_NOTIFY( NM_CUSTOMDRAW, IDUSER_TOOLBAR_CREATE, OnHighlightToolbtn )
	ON_COMMAND(ID_TOOLBTN_NETRING, OnToolbtnNetring)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NETRING, OnUpdateToolbtnNetring)
	//}}AFX_MSG_MAP
	ON_NOTIFY(DLN_CE_CREATE, AFXCE_ID_DOCLIST, OnCreateDocList)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE( COMBO_TOOLSET_ID, OnSelectToolSet )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
	m_UmlSymbol = UMLSYMBOL_NIL;
	m_Net1Symbol = NET1SYMBOL_NIL;
	m_Net2Symbol = NET2SYMBOL_NIL;
	m_AlignSymbol = ALIGNSYMBOL_NIL;
		
	m_EdgeSet1 = EDGESET1_NIL;

	m_GeneralTool = GENERALTOOL_NIL;
	
	m_CurrentSTS = SYMBOLTOOLSET_FLOWCHART;
	m_CurrentETS = EDGETOOLSET_EDGESET1;
	m_CurrentSelTS = SELTOOLSET_ALIGN;
	m_CreateBarType = CBT_SYMBOL;

	m_CurrentSTSSel = 0;
	m_CurrentETSSel = 0;
	m_CurrentSelTSSel = 0;

	m_bShowTools = TRUE;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndCommandBar.m_bShowSharedNewButton = TRUE;
	
	if(!m_wndCommandBar.Create(this) ||
	   !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	   !m_wndCommandBar.AddAdornments() ||
	   !(m_pSTSSelection = m_wndCommandBar.InsertComboBox( 60, COMBO_TOOLSET_ID ) ) ||
	   !m_wndCommandBar.LoadToolBar(IDR_TOOLBAR_SHOWHIDETOOLS  ) ) 
	   
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	EnableDocking( CBRS_ALIGN_ANY );	
	
	m_CreateToolBar.Create( this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | TBSTYLE_TOOLTIPS );
	UseCreateBarType( CBT_SYMBOL ); //Tool bar must be loaded BEFORE its initially docked.
	m_CreateToolBar.EnableDocking( CBRS_ALIGN_ANY );		
	DockControlBar( &m_CreateToolBar );	

	m_GeneralToolBar.Create( this, WS_CHILD | WS_VISIBLE | CBRS_TOP | TBSTYLE_CUSTOMERASE | TBSTYLE_TOOLTIPS );
	m_GeneralToolBar.LoadToolBar( IDR_TOOLBAR_GENERAL );
	m_GeneralToolBar.EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar( &m_GeneralToolBar );
	
	m_GeneralToolBar.GetToolBarCtrl().SetDlgCtrlID( IDUSER_TOOLBAR_GENERAL );
	m_CreateToolBar.GetToolBarCtrl().SetDlgCtrlID( IDUSER_TOOLBAR_CREATE );	
	
	//SetWindowText( TEXT("Visual Notes") );

	return 0;
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CFrameWnd::OnSetFocus(pOldWnd);
	

	// TODO: Add your message handler code here
	CVisualNotesView *pView = (CVisualNotesView*)GetActiveView();
	CRect AdjustRect;
	if( pView && pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) &&
		AdjustViewFromTools( &AdjustRect ) )
		pView->MoveWindow( &AdjustRect );
}

void CMainFrame::OnSelectToolSet()
{
	//TODO: Add your combo box notification handler here.
	CString EditString, TestString;

	m_pSTSSelection->GetWindowText( EditString );
	
	//if the current create type is a symbolic tool set,
	//determine symbolic combo box selection.
	if( m_CreateBarType == CBT_SYMBOL )
	{

		TestString.LoadString( IDS_FLOWCHARTSET );
		if( EditString == TestString && m_CurrentSTSSel != SelIndexFlow )
		{
			LoadNewToolSet( SYMBOLTOOLSET_FLOWCHART );	
			m_CurrentSTSSel = SelIndexFlow;
		}

		TestString.LoadString( IDS_UMLSET );
		if( EditString == TestString && m_CurrentSTSSel != SelIndexUml )
		{
			LoadNewToolSet( SYMBOLTOOLSET_UML );
			m_CurrentSTSSel = SelIndexUml;
		}

		TestString.LoadString( IDS_NET1SET );
		if( EditString == TestString && m_CurrentSTSSel != SelIndexNet1 )
		{
			LoadNewToolSet( SYMBOLTOOLSET_NET1 );
			m_CurrentSTSSel = SelIndexNet1;
		}

		TestString.LoadString( IDS_NET2SET );
		if( EditString == TestString && m_CurrentSTSSel != SelIndexNet2 )
		{
			LoadNewToolSet( SYMBOLTOOLSET_NET2 );
			m_CurrentSTSSel = SelIndexNet2;
		}		
	}

	//else if the current create type is a edge type tool set,
	//determine edge type combo box selection.
	else if( m_CreateBarType == CBT_EDGE )
	{
		TestString.LoadString( IDS_EDGESET1 );
		if( EditString == TestString && m_CurrentSTSSel != 0 )
		{
			LoadNewToolSet( EDGETOOLSET_EDGESET1 );
			m_CurrentETSSel = 0;
		}
	}

	else if( m_CreateBarType == CBT_SELECT )
	{
		TestString.LoadString( IDS_ALIGNSET );
		if( EditString == TestString && m_CurrentSelTSSel != 0 )
		{
			LoadNewToolSet( SELTOOLSET_ALIGN );
			m_CurrentSelTSSel = 0;
		}

		TestString.LoadString( IDS_SHOWHIDESET );
		if( EditString == TestString && m_CurrentSelTSSel != 1 )
		{
			LoadNewToolSet( SELTOOLSET_SHOWHIDE );
			m_CurrentSelTSSel = 1;
		}
	}
}

void CMainFrame::LoadNewToolSet( SYMBOLTOOLSET NewToolSet )
{

	switch( NewToolSet )
	{
	
	case SYMBOLTOOLSET_FLOWCHART:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_FLOWCHART );
		break;

	case SYMBOLTOOLSET_UML:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_UML );
		break;

	case SYMBOLTOOLSET_NET1:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_NET1 );
		break;

	case SYMBOLTOOLSET_NET2:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_NET2 );
		break;
	}	

	m_CurrentSTS = NewToolSet;
}

void CMainFrame::LoadNewToolSet( EDGETOOLSET NewToolSet )
{
	switch( NewToolSet )
	{
	case EDGETOOLSET_EDGESET1:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_EDGESET1 );
		break;
	}

	m_CurrentETS = NewToolSet;
}

void CMainFrame::LoadNewToolSet( SELTOOLSET NewToolSet )
{
	switch( NewToolSet )
	{
	case SELTOOLSET_ALIGN:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_ALIGNMENT );
		break;
			
	case SELTOOLSET_SHOWHIDE:
		m_CreateToolBar.LoadToolBar( IDR_TOOLBAR_SHOWHIDE );
	}

	m_CurrentSelTS = NewToolSet;
}

void CMainFrame::UseCreateBarType( CREATEBARTYPE cbt )
{
	CString strItem;
		
	if( cbt == CBT_SYMBOL  )
	{
		//Reloadd teh previously selected symbol set tool bar.
		LoadNewToolSet( m_CurrentSTS );

		//load the selection combo box with symbol set choices.
		m_pSTSSelection->ResetContent();
		
		strItem.LoadString( IDS_FLOWCHARTSET );
		m_pSTSSelection->InsertString(SelIndexFlow, strItem );
		
		strItem.LoadString( IDS_UMLSET );
		m_pSTSSelection->InsertString(SelIndexUml, strItem );

		strItem.LoadString( IDS_NET1SET );
		m_pSTSSelection->InsertString(SelIndexNet1, strItem );

		strItem.LoadString( IDS_NET2SET );
		m_pSTSSelection->InsertString(SelIndexNet2, strItem );

		m_pSTSSelection->SetCurSel( m_CurrentSTSSel );
	}

	else if( cbt == CBT_EDGE  )
	{
		//Reload the previously selected edge set tool bar.
		LoadNewToolSet( m_CurrentETS );
		
		//load the selection combo box with edge set choices.
		m_pSTSSelection->ResetContent();
		strItem.LoadString( IDS_EDGESET1 );
		m_pSTSSelection->InsertString(0, strItem );
		m_pSTSSelection->SetCurSel( m_CurrentETSSel );
	}

	else if( cbt == CBT_SELECT )
	{
		LoadNewToolSet( m_CurrentSelTS );

		//Load the selection combo box with select set choices.
		m_pSTSSelection->ResetContent();

		strItem.LoadString( IDS_ALIGNSET );
		m_pSTSSelection->InsertString( 0, strItem );

		strItem.LoadString( IDS_SHOWHIDESET );
		m_pSTSSelection->InsertString( 1, strItem );

		m_pSTSSelection->SetCurSel( m_CurrentSelTSSel );
	}

	m_CreateBarType = cbt;
}

BOOL CMainFrame::AdjustViewFromTools( CRect *pAdjustRect )
{
	CRect GTRect, STRect, MainRect, NewViewRect, NewSTRect;
	CRect CurrentViewRect;

	CVisualNotesView *pView = (CVisualNotesView*)GetActiveView();

	ASSERT( pView );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	
	m_CreateToolBar.GetWindowRect( &STRect );
	m_GeneralToolBar.GetWindowRect( &GTRect );
	GetClientRect( &MainRect );

	//Initially, the view is the same as the client rect.
	//This changes only if we are showing the tool bars.
	NewViewRect = MainRect;

	//resize window, according to the toolbars we are showing.
	if( m_bShowTools )
	{
		//since we are showing the tools, the view area is equal to the main frame's
		//client area minus the area of the top and bottom tool bars.
		NewViewRect.top+= GTRect.Height();
		NewViewRect.bottom-= STRect.Height();

		//When showing tools, we have to explicitly define position of 
		//the bottom tool bar since its position may have been affected by
		//the SIP window.
		NewSTRect = MainRect;
		NewSTRect.top = MainRect.bottom - STRect.Height();
		m_GeneralToolBar.ShowWindow( SW_SHOW );
		m_CreateToolBar.ShowWindow( SW_SHOW );
		m_CreateToolBar.MoveWindow( &NewSTRect ); //Note: MoveWindow only affects "shown" windows.
	}
	
	else
	{
		//make sure tool bars donot attempt to draw themselves.
		m_CreateToolBar.ShowWindow( SW_HIDE );
		m_GeneralToolBar.ShowWindow( SW_HIDE );
	}

	pView->GetWindowRect( &CurrentViewRect );
	ScreenToClient( &CurrentViewRect );

	*pAdjustRect = NewViewRect;
	return ( CurrentViewRect != NewViewRect ); //return TRUE only if view needs to be adjusted.
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return TRUE;
}


void CMainFrame::OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result)
{
	CCeDocList* pDocList = (CCeDocList*)FromHandle(pNotifyStruct->nmhdr.hwndFrom);
	ASSERT_KINDOF(CCeDocList, pDocList);

	CCeCommandBar* pDocListCB = pDocList->GetCommandBar();
	ASSERT(pDocListCB != NULL);

	pDocListCB->InsertMenuBar(IDM_DOCLIST);
	//pDocListCB->SendMessage(TB_SETTOOLTIPS, (WPARAM)(1), (LPARAM)(m_ToolTipsTable));
	CFrameWnd::OnCreateDocList(pNotifyStruct, result);	
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
}

void CMainFrame::OnHighlightToolbtn( NMHDR *pNMHDR, LRESULT *pLResult )
{
	NMCUSTOMDRAW *pnmcs = (NMCUSTOMDRAW*)(pNMHDR);
	CDC *pDC;
	CBrush Brush;

	*pLResult = CDRF_NOTIFYITEMDRAW;

	switch( pnmcs->dwDrawStage )
	{
	
	case CDDS_ITEMPREPAINT:
		*pLResult = CDRF_NOTIFYPOSTPAINT;

		break;

	case CDDS_ITEMPOSTPAINT:

		if( pnmcs->uItemState == CDIS_CHECKED )
		{
			pDC = CDC::FromHandle( pnmcs->hdc );
			Brush.CreateSolidBrush( RGB( 255, 50, 50 ) );
			pDC->FrameRect( &pnmcs->rc, &Brush );	
			Brush.DeleteObject();
		}
		
		break;	
	}
}

void CMainFrame::ResetToolUI()
{
	m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
	m_UmlSymbol = UMLSYMBOL_NIL;
	m_Net1Symbol = NET1SYMBOL_NIL;
	m_Net2Symbol = NET2SYMBOL_NIL;
	m_AlignSymbol = ALIGNSYMBOL_NIL;
		
	m_EdgeSet1 = EDGESET1_NIL;

	m_GeneralTool = GENERALTOOL_NIL;
	
	m_CurrentSTS = SYMBOLTOOLSET_FLOWCHART;
	m_CurrentETS = EDGETOOLSET_EDGESET1;
	m_CurrentSelTS = SELTOOLSET_ALIGN;
	m_CreateBarType = CBT_SYMBOL;

	m_CurrentSTSSel = 0;
	m_CurrentETSSel = 0;
	m_CurrentSelTSSel = 0;

	m_bShowTools = TRUE;

	UseCreateBarType( CBT_SYMBOL ); 

	theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
}

//------------------------------------------------------------------------------------------------------------

void CMainFrame::OnToolbtnCircle() 
{
	// TODO: Add your command handler code here.
	
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_CIRCLE )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_CIRCLE;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
}

void CMainFrame::OnToolbtnDiamond() 
{
	// TODO: Add your command handler code here.
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_DIAMOND )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_DIAMOND;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
}

void CMainFrame::OnToolbtnRectangle() 
{
	// TODO: Add your command handler code here
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_RECTANGLE )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_RECTANGLE;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
}

void CMainFrame::OnToolbtnShear() 
{
	// TODO: Add your command handler code here
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_SHEAR )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_SHEAR;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
}
void CMainFrame::OnUpdateToolbtnCircle(CCmdUI* pCmdUI) 
{
	//Add your command update UI handler code here.
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_CIRCLE );	
}

void CMainFrame::OnUpdateToolbtnDiamond(CCmdUI* pCmdUI) 
{
	//Add your command update UI handler code here.
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_DIAMOND );	

	
}

void CMainFrame::OnUpdateToolbtnRectangle(CCmdUI* pCmdUI) 
{
	//Add your command update UI handler code here.
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_RECTANGLE );	

}

void CMainFrame::OnUpdateToolbtnShear(CCmdUI* pCmdUI) 
{
	//Add your command update UI handler code here.
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_SHEAR );	
}

//--------------------------------------------------------------------------------------------------------------

void CMainFrame::OnToolbtnText() 
{
	// TODO: Add your command handler code here.
	if( m_GeneralTool != GENERALTOOL_TEXT )
	{
		m_GeneralTool = GENERALTOOL_TEXT;
		theApp.m_AppMode = CVisualNotesApp::APPMODE_CREATELABEL;
	}

	else
	{
		m_GeneralTool = GENERALTOOL_NIL;
		theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
	}
}

void CMainFrame::OnUpdateToolbtnText(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here.
	pCmdUI->SetCheck( m_GeneralTool == GENERALTOOL_TEXT );
}

void CMainFrame::OnToolbtnSelect() 
{
	// TODO: Add your command handler code here

	if( m_GeneralTool != GENERALTOOL_SELECT )
	{
		m_GeneralTool = GENERALTOOL_SELECT;
		theApp.m_AppMode = CVisualNotesApp::APPMODE_SELECTSYMBOL;

		if( m_CreateBarType != CBT_SELECT )
			UseCreateBarType( CBT_SELECT );
	}

	else
	{
		m_GeneralTool = GENERALTOOL_NIL;
		theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
	}
}

void CMainFrame::OnUpdateToolbtnSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_GeneralTool == GENERALTOOL_SELECT );

}

void CMainFrame::OnUpdateToolbtnCreate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	pCmdUI->SetCheck( m_GeneralTool == GENERALTOOL_CREATE );
}

void CMainFrame::OnToolbtnCreate() 
{
	// TODO: Add your command handler code here
	if( m_GeneralTool != GENERALTOOL_CREATE )
	{
		m_GeneralTool = GENERALTOOL_CREATE;
		theApp.m_AppMode = CVisualNotesApp::APPMODE_CREATESYMBOL;

		if( m_CreateBarType != CBT_SYMBOL )
			UseCreateBarType( CBT_SYMBOL );
	}

	else
	{
		m_GeneralTool = GENERALTOOL_NIL;
		theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
	}
}

void CMainFrame::OnToolbtnProperties() 
{
	// TODO: Add your command handler code here
	theApp.DisplayProperties();
	//Handle the mystery of the "dissappearing input panel" on the command bar.
	m_wndCommandBar.RedrawWindow();
}


void CMainFrame::OnToolbtnDelete() 
{
	// TODO: Add your command handler code here
	theApp.Delete();
}

void CMainFrame::OnToolbtnLink() 
{
	// TODO: Add your command handler code here
	if( theApp.m_AppMode != CVisualNotesApp::APPMODE_LINKSYMBOLS )
	{
		theApp.m_AppMode = CVisualNotesApp::APPMODE_LINKSYMBOLS;
		m_GeneralTool = GENERALTOOL_LINK;
	}

	else
	{
		theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
		m_GeneralTool = GENERALTOOL_NIL;
	}	
}

void CMainFrame::OnUpdateToolbtnLink(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_GeneralTool == GENERALTOOL_LINK );	
}

void CMainFrame::OnToolbtnEdge() 
{
	// TODO: Add your command handler code here
	if( theApp.m_AppMode != CVisualNotesApp::APPMODE_CREATEEDGE )
	{
		theApp.m_AppMode = CVisualNotesApp::APPMODE_CREATEEDGE;
		m_GeneralTool = GENERALTOOL_EDGE;

		if( m_CreateBarType != CBT_EDGE )
			UseCreateBarType( CBT_EDGE );
	}

	else
	{
		theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
		m_GeneralTool = GENERALTOOL_NIL;
	}	
}

void CMainFrame::OnUpdateToolbtnEdge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetCheck( m_GeneralTool == GENERALTOOL_EDGE  );

	
}

void CMainFrame::OnToolbtnTrack() 
{
	// TODO: Add your command handler code here
	if( theApp.m_AppMode != CVisualNotesApp::APPMODE_TRACKSYMBOL )
	{
		theApp.m_AppMode = CVisualNotesApp::APPMODE_TRACKSYMBOL;
		m_GeneralTool = GENERALTOOL_TRACK;
	}

	else
	{
		theApp.m_AppMode = CVisualNotesApp::APPMODE_NIL;
		m_GeneralTool = GENERALTOOL_NIL;
	}	
}

void CMainFrame::OnUpdateToolbtnTrack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_GeneralTool == GENERALTOOL_TRACK );
}

void CMainFrame::OnToolbtnShowhidetools() 
{
	// TODO: Add your command handler code here
	CVisualNotesView* pView = (CVisualNotesView*)GetActiveView();
	CRect AdjustRect;

	ASSERT( pView );
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	
	m_bShowTools = !m_bShowTools;
	AdjustViewFromTools(&AdjustRect);
	pView->MoveWindow( &AdjustRect );
}

void CMainFrame::OnToolbtnBasicedge() 
{
	// TODO: Add your command handler code here
	if( m_EdgeSet1 != EDGESET1_BASICEDGE )
		m_EdgeSet1 = EDGESET1_BASICEDGE;

	else
		m_EdgeSet1 = EDGESET1_NIL;
}

void CMainFrame::OnUpdateToolbtnBasicedge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_EdgeSet1 == EDGESET1_BASICEDGE );


}

void CMainFrame::OnToolbtnCmplxedge() 
{
	// TODO: Add your command handler code here
	if( m_EdgeSet1 != EDGESET1_COMPLEXEDGE )
		m_EdgeSet1 = EDGESET1_COMPLEXEDGE;

	else
		m_EdgeSet1 = EDGESET1_NIL;
}

void CMainFrame::OnUpdateToolbtnCmplxedge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_EdgeSet1 == EDGESET1_COMPLEXEDGE );
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( wParam == ID_TOOLBTN_CREATE ||
		wParam == ID_TOOLBTN_EDGE   ||
		wParam == ID_TOOLBTN_TRACK  ||
		wParam == ID_TOOLBTN_SELECT ||
		wParam == ID_TOOLBTN_TEXT   ||
		wParam == ID_TOOLBTN_LINK   ||
		( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATEEDGE  &&
		  lParam == (LPARAM)m_CreateToolBar.GetToolBarCtrl().GetSafeHwnd() ) )
	{ theApp.OnChangeUI( wParam ); }

	return CFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::OnToolbtnRedo() 
{
	// TODO: Add your command handler code here
	theApp.Redo();	
}

void CMainFrame::OnToolbtnUndo() 
{
	// TODO: Add your command handler code here
	theApp.Undo();
}

void CMainFrame::OnToolbtnUmlactor() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_ACTOR )
		m_UmlSymbol = UMLSYMBOL_ACTOR;
	
	else
		m_UmlSymbol = UMLSYMBOL_NIL;
}

void CMainFrame::OnUpdateToolbtnUmlactor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_ACTOR );

}

void CMainFrame::OnToolbtnUmlcase() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_CASE )
		m_UmlSymbol = UMLSYMBOL_CASE;
	
	else
		m_UmlSymbol = UMLSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnUmlcase(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_CASE );

}

void CMainFrame::OnToolbtnUmlclass() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_CLASS )
		m_UmlSymbol = UMLSYMBOL_CLASS;
	
	else
		m_UmlSymbol = UMLSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnUmlclass(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_CLASS );


}

void CMainFrame::OnToolbtnUmlcomponent() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_COMPONENT )
		m_UmlSymbol = UMLSYMBOL_COMPONENT;
	
	else
		m_UmlSymbol = UMLSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnUmlcomponent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_COMPONENT );


}

void CMainFrame::OnToolbtnUmlnote() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_NOTE )
		m_UmlSymbol = UMLSYMBOL_NOTE;
	
	else
		m_UmlSymbol = UMLSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnUmlnote(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_NOTE );

	
}

void CMainFrame::OnToolbtnUmlpackage() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_PACKAGE )
		m_UmlSymbol = UMLSYMBOL_PACKAGE;
	
	else
		m_UmlSymbol = UMLSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnUmlpackage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_PACKAGE );


}



void CMainFrame::OnToolbtnNet2camera() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_CAMERA )
		m_Net2Symbol = NET2SYMBOL_CAMERA;

	else
		m_Net2Symbol = NET2SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNet2camera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_CAMERA );	


}

void CMainFrame::OnToolbtnNet2cellphone() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_CELLPHONE )
		m_Net2Symbol = NET2SYMBOL_CELLPHONE; 

	else
		m_Net2Symbol = NET2SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNet2cellphone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_CELLPHONE );


}

void CMainFrame::OnToolbtnNet2dish() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_DISH )
		m_Net2Symbol = NET2SYMBOL_DISH;

	else
		m_Net2Symbol = NET2SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNet2dish(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_DISH );


}

void CMainFrame::OnToolbtnNet2fax() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_FAX )
		m_Net2Symbol = NET2SYMBOL_FAX;

	else
		m_Net2Symbol = NET2SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNet2fax(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_FAX );

}

void CMainFrame::OnToolbtnNet2pda() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_PDA )
		m_Net2Symbol = NET2SYMBOL_PDA; 

	else
		m_Net2Symbol = NET2SYMBOL_NIL;	
	
}

void CMainFrame::OnUpdateToolbtnNet2pda(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_PDA );


}

void CMainFrame::OnToolbtnNet2satellite() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_SATELLITE )
		m_Net2Symbol = NET2SYMBOL_SATELLITE;

	else
		m_Net2Symbol = NET2SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNet2satellite(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_SATELLITE );

}

void CMainFrame::OnToolbtnNet2thinkpad() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_THINKPAD )
		m_Net2Symbol = NET2SYMBOL_THINKPAD;

	else
		m_Net2Symbol = NET2SYMBOL_NIL;		
}

void CMainFrame::OnUpdateToolbtnNet2thinkpad(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_THINKPAD );
	
}

void CMainFrame::OnToolbtnNet2wirephone() 
{
	// TODO: Add your command handler code here
	if( m_Net2Symbol != NET2SYMBOL_WIREPHONE )
		m_Net2Symbol = NET2SYMBOL_WIREPHONE;

	else
		m_Net2Symbol = NET2SYMBOL_NIL;		
}

void CMainFrame::OnUpdateToolbtnNet2wirephone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net2Symbol == NET2SYMBOL_WIREPHONE );


}

void CMainFrame::OnToolbtnNetclient() 
{
	// TODO: Add your command handler code here	
	if( m_Net1Symbol != NET1SYMBOL_CLIENT )
		m_Net1Symbol = NET1SYMBOL_CLIENT;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;		
}

void CMainFrame::OnUpdateToolbtnNetclient(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_CLIENT );


}

void CMainFrame::OnToolbtnNetcloud() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_CLOUD )
		m_Net1Symbol = NET1SYMBOL_CLOUD;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
}

void CMainFrame::OnUpdateToolbtnNetcloud(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_CLOUD );


}

void CMainFrame::OnToolbtnNetdatabase() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_DATABASE )
		m_Net1Symbol = NET1SYMBOL_DATABASE;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnNetdatabase(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_DATABASE );


}
/*
void CMainFrame::OnToolbtnNetfirewall() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_FIREWALL )
		m_Net1Symbol = NET1SYMBOL_FIREWALL;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
}


void CMainFrame::OnUpdateToolbtnNetfirewall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_FIREWALL );

	if( m_Net1Symbol == NET1SYMBOL_FIREWALL )
		HighLightToolBtn( pCmdUI, RGB( 0, 0, 255 ) );
	
}

void CMainFrame::OnToolbtnNetgateway() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_GATEWAY )
		m_Net1Symbol = NET1SYMBOL_GATEWAY;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNetgateway(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_GATEWAY );

	if( m_Net1Symbol == NET1SYMBOL_GATEWAY )
		HighLightToolBtn( pCmdUI, RGB( 0, 0, 255 ) );
}
*/

void CMainFrame::OnToolbtnNethub() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_HUB )
		m_Net1Symbol = NET1SYMBOL_HUB;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
}

void CMainFrame::OnUpdateToolbtnNethub(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_HUB );

}

void CMainFrame::OnToolbtnNetmainframe() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_MAINFRAME)
		m_Net1Symbol = NET1SYMBOL_MAINFRAME;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnNetmainframe(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_MAINFRAME );

	
}

void CMainFrame::OnToolbtnNetprinter() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_PRINTER )
		m_Net1Symbol = NET1SYMBOL_PRINTER;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnNetprinter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_PRINTER );
	

}

void CMainFrame::OnToolbtnNetserver() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_SERVER )
		m_Net1Symbol = NET1SYMBOL_SERVER;

	else
		m_Net1Symbol = NET1SYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnNetserver(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_SERVER );	
}

void CMainFrame::OnToolbtnDepbasicedge() 
{
	// TODO: Add your command handler code here
	if( m_EdgeSet1 != EDGESET1_DEPBASICEDGE )
		m_EdgeSet1 = EDGESET1_DEPBASICEDGE;

	else
		m_EdgeSet1 = EDGESET1_NIL;
}

void CMainFrame::OnUpdateToolbtnDepbasicedge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_EdgeSet1 == EDGESET1_DEPBASICEDGE );	
}

void CMainFrame::OnToolbtnDepcomplexedge() 
{
	// TODO: Add your command handler code here
	if( m_EdgeSet1 != EDGESET1_DEPCOMPLEXEDGE )
		m_EdgeSet1 = EDGESET1_DEPCOMPLEXEDGE;

	else
		m_EdgeSet1 = EDGESET1_NIL;
}

void CMainFrame::OnUpdateToolbtnDepcomplexedge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_EdgeSet1 == EDGESET1_DEPCOMPLEXEDGE );
}

void CMainFrame::OnToolbtnAlignBottom() 
{
	// TODO: Add your command handler code here
	theApp.AlignSymbols( CVisualNotesApp::AS_BOTTOM );
	
}

void CMainFrame::OnToolbtnAlignLeft() 
{
	// TODO: Add your command handler code here
	theApp.AlignSymbols( CVisualNotesApp::AS_LEFT );	
}


void CMainFrame::OnToolbtnAlignSize() 
{
	// TODO: Add your command handler code here
	theApp.AlignSymbols( CVisualNotesApp::AS_SIZE );	
}

void CMainFrame::OnToolbtnAlignTop() 
{
	// TODO: Add your command handler code here
	theApp.AlignSymbols( CVisualNotesApp::AS_TOP );	
}


void CMainFrame::OnToobtnAlignRight() 
{
	// TODO: Add your command handler code here
	theApp.AlignSymbols( CVisualNotesApp::AS_RIGHT );
	
}

void CMainFrame::OnToolbtnGeneralize() 
{
		// TODO: Add your command handler code here.
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_GENERALIZE )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_GENERALIZE;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnGeneralize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_GENERALIZE );	
}

void CMainFrame::OnToolbtnTaga() 
{
		// TODO: Add your command handler code here.
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_TAGA )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_TAGA;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;

	
}

void CMainFrame::OnUpdateToolbtnTaga(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_TAGA );
	
}

void CMainFrame::OnToolbtnTagb() 
{
		// TODO: Add your command handler code here.
	if( m_FlowChartSymbol != FLOWCHARTSYMBOL_TAGB )
		m_FlowChartSymbol = FLOWCHARTSYMBOL_TAGB;
	else
		m_FlowChartSymbol = FLOWCHARTSYMBOL_NIL;
	
}

void CMainFrame::OnUpdateToolbtnTagb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	pCmdUI->SetCheck( m_FlowChartSymbol == FLOWCHARTSYMBOL_TAGB );
}

void CMainFrame::OnToolbtnDeployment() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_DEPLOYMENT )
		m_UmlSymbol = UMLSYMBOL_DEPLOYMENT;
	else
		m_UmlSymbol = UMLSYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnDeployment(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_DEPLOYMENT );	
}

void CMainFrame::OnToolbtnRoundrect() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_ROUNDRECT )
		m_UmlSymbol = UMLSYMBOL_ROUNDRECT;
	else
		m_UmlSymbol = UMLSYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnRoundrect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_ROUNDRECT );	
}

void CMainFrame::OnToolbtnCapsule() 
{
	// TODO: Add your command handler code here
	if( m_UmlSymbol != UMLSYMBOL_CAPSULE )
		m_UmlSymbol = UMLSYMBOL_CAPSULE;
	else
		m_UmlSymbol = UMLSYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnCapsule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_UmlSymbol == UMLSYMBOL_CAPSULE );	
}

void CMainFrame::OnToolbtnAligncenter() 
{
	// TODO: Add your command handler code here
	theApp.AlignSymbols( CVisualNotesApp::AS_CENTER );	
}


void CMainFrame::OnToolbtnShowHidden() 
{
	// TODO: Add your command handler code here
	CILList<CILSymbol*,CILSymbol*> HideList;
	theApp.GetSymbolDatabase()->QueryHidden( &HideList );
	ILPOSITION Pos =  HideList.GetHeadPosition();
	while( Pos )
	{
		HideList.GetAt( Pos )->HideSymbol( FALSE );
		HideList.GetAt( Pos )->InvalidateSymbol( GetActiveView() );
		HideList.GetNext( Pos );
	}

	HideList.RemoveAll();
}

void CMainFrame::OnToolbtnHideSelected() 
{
	// TODO: Add your command handler code here
	CILList<CILSymbol*,CILSymbol*> SelList;

	theApp.GetSymbolDatabase()->QuerySelected( &SelList );
	ILPOSITION Pos = SelList.GetHeadPosition();
	while( Pos )
	{
		SelList.GetAt( Pos )->HideSymbol( TRUE );
		SelList.GetAt( Pos )->InvalidateSymbol( GetActiveView() );
		SelList.GetNext( Pos );
	}

	SelList.RemoveAll();		
}

void CMainFrame::OnToolbtnNetring() 
{
	// TODO: Add your command handler code here
	if( m_Net1Symbol != NET1SYMBOL_RING )
		m_Net1Symbol = NET1SYMBOL_RING;
	else
		m_Net1Symbol = NET1SYMBOL_NIL;	
}

void CMainFrame::OnUpdateToolbtnNetring(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_Net1Symbol == NET1SYMBOL_RING );
}

BOOL CMainFrame::OnHelpInfo( HELPINFO *lpHelpInfo )
{

	

	return TRUE;
}

  

 
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
