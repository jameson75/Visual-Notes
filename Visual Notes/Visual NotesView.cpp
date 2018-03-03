// Visual NotesView.cpp : implementation of the CVisualNotesView class
//

#include "stdafx.h"
#include "stdil.h"

#include "Visual Notes.h"
#include "Visual NotesDoc.h"
#include "Visual NotesView.h"
#include "DiagramViewDlg.h"
#include "NewNoteDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDUSER_POPUP_OPENNOTELINK		50001
#define IDUSER_POPUP_OPENWEBLINK		50003
#define IDUSER_POPUP_OPENEMBEDDEDLINK	50004
#define IDUSER_POPUP_CREATEEMBEDDEDLINK	50005
#define IDUSER_POPUP_DELETEEMBEDDEDLINK	50006
#define IDUSER_POPUP_ASCENDDIAGRAM		50007
#define IDUSER_POPUP_DELETETHISDIAGRAM	50008
#define IDUSER_POPUP_INSERTEMBEDDEDLINK	50009
#define IDUSER_POPUP_NAVIGATEFOWARD		50010
#define IDUSER_POPUP_NAVIGATEBACK		50011
#define IDUSER_POPUP_REMOVEEMBEDDEDLINK	50012
#define IDUSER_POPUP_CREATENOTELINK		50013
#define IDUSER_POPUP_REMOVENOTELINK		50014

extern CVisualNotesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesView

IMPLEMENT_DYNCREATE(CVisualNotesView, CScrollView)

BEGIN_MESSAGE_MAP(CVisualNotesView, CScrollView)
	//{{AFX_MSG_MAP(CVisualNotesView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND( IDUSER_POPUP_OPENEMBEDDEDLINK, OnOpenEmbeddedLink )
	ON_COMMAND( IDUSER_POPUP_CREATEEMBEDDEDLINK, OnCreateEmbeddedLink )
	ON_COMMAND( IDUSER_POPUP_DELETEEMBEDDEDLINK, OnDeleteEmbeddedLink )
	ON_COMMAND( IDUSER_POPUP_DELETETHISDIAGRAM, OnDeleteThisDiagram )
	ON_COMMAND( IDUSER_POPUP_OPENNOTELINK, OnOpenNoteLink )
	ON_COMMAND( IDUSER_POPUP_OPENWEBLINK, OnOpenWebLink )
	ON_COMMAND( IDUSER_POPUP_INSERTEMBEDDEDLINK, OnInsertEmbeddedLink )
	ON_WM_SIZE()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND( IDUSER_POPUP_NAVIGATEFOWARD, OnNavigateFoward )
	ON_COMMAND( IDUSER_POPUP_NAVIGATEBACK, OnNavigateBack )
	ON_COMMAND( IDUSER_POPUP_REMOVEEMBEDDEDLINK, OnRemoveEmbeddedLink )
	ON_COMMAND( IDUSER_POPUP_CREATENOTELINK, OnCreateNoteLink )
	ON_COMMAND( IDUSER_POPUP_REMOVENOTELINK, OnRemoveNoteLink )
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(GN_CONTEXTMENU, OnContextMenu)
	ON_COMMAND(ID_TOOLS_EXPORT, OnToolsExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesView construction/destruction

CVisualNotesView::CVisualNotesView()
{
	// TODO: add construction code here
	m_pLinkChild = NULL;
	m_pwndPrevCapture = NULL;
	m_bDragSelection = FALSE;
	m_bHitSelection = FALSE;
	m_pPRHitSymbol = NULL;
	m_bMultiSelectMode = FALSE;
	m_bMultiPickMode = FALSE;
}

CVisualNotesView::~CVisualNotesView()
{
}

BOOL CVisualNotesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesView drawing

void CVisualNotesView::OnDraw(CDC* pDC)
{
	CVisualNotesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here.
	CILList<CILSymbol*,CILSymbol*> *pSymbolTable = pDoc->m_pDBSet->m_DB.GetSymbolTable();
	CILList<CILSymbol*,CILSymbol*> *pJointTable = pDoc->m_pDBSet->m_DB.GetJointTable();
	CILSymbol *pSymbol = NULL;

	ILPOSITION JPos = pJointTable->GetHeadPosition();
	while( JPos )
	{
		pSymbol = pJointTable->GetAt( JPos );
		pSymbol->SetCanvasOrigin( CanvasToClient( &CPoint(0,0) ) );
		
		if( !pSymbol->IsHidden() )
		{
			pSymbol->DrawSymbol( pDC );	

			if( CILBitOp::IsOn( pSymbol->m_dwExtraData, theApp.m_fPrevJointInBuild, TRUE ) )
				pSymbol->Highlight( TRUE, this, PS_DASH, RGB( 100, 100, 200) );
		}

		pJointTable->GetNext( JPos );
	}
	
	ILPOSITION Pos = pSymbolTable->GetHeadPosition();
	while( Pos )
	{
		pSymbol = pSymbolTable->GetAt( Pos );
		pSymbol->SetCanvasOrigin( CanvasToClient( &CPoint(0,0) ) );

		if( !pSymbol->IsHidden() )
		{
			pSymbol->DrawSymbol( pDC );	
		
			//highlight intermedidates state.
			if( CILBitOp::IsOn( pSymbol->m_dwExtraData, theApp.m_fLinkChild, TRUE) )
				pSymbol->Highlight( TRUE, this, PS_DASH, RGB(200,50,50) );
			
			if( CILBitOp::IsOn( pSymbol->m_dwExtraData, theApp.m_fPrevShapeInBuild, TRUE ) )
				pSymbol->Highlight( TRUE, this, PS_DASH, RGB(50,50,200) );
		}

		pSymbolTable->GetNext( Pos );
	}
}

void CVisualNotesView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	
	/* I AM REMOVING APP WIZARD CODE 
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	*/
	
	SetScrollSizes( MM_TEXT, theApp.m_CanvasSize );
	
	CMainFrame *pMainFrame = (CMainFrame*)GetParentFrame();
	pMainFrame->m_bShowTools = TRUE;
	//pMainFrame->ResetToolUI();
	//pMainFrame->AdjustViewFromTools();	
}

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesView diagnostics

#ifdef _DEBUG
void CVisualNotesView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CVisualNotesView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CVisualNotesDoc* CVisualNotesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisualNotesDoc)));
	return (CVisualNotesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesView message handlers
int CVisualNotesView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	int iResult = CView::OnCreate(lpCreateStruct);

	if(iResult == 0)
		ShowDoneButton(TRUE);

	return iResult;
}
void CVisualNotesView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	//::SHSipPreference(m_hWnd, SIP_UP);
}

void CVisualNotesView::OnDestroy()
{
	
	CMainFrame *pMainFrame = (CMainFrame*)GetParentFrame();
	if( pMainFrame )
	{
		pMainFrame->ResetToolUI();
		pMainFrame->m_CreateToolBar.ShowWindow( SW_HIDE );
		pMainFrame->m_GeneralToolBar.ShowWindow( SW_HIDE );
	}
	
	//::SHSipPreference(m_hWnd, SIP_FORCEDOWN);
	CView::OnDestroy();
}

BOOL CVisualNotesView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CVisualNotesView::OnContextMenu( NMHDR *pNotifyStruct, LRESULT *pLResult )
{

	CMenu PopupMenu;
	CPoint ptLastCanvasDown( m_ptLastDown ); 
	CILSymbol *pHitSymbol = NULL;
	CString strNoteLink = _T("");
	CILList<CILSymbol*,CILSymbol*> SelectionList;
	
	ClientToCanvas( &ptLastCanvasDown );

	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	
	//if user selected an non-joint symbol while in application is in "Zero Mode"...
	if( theApp.m_AppMode == CVisualNotesApp::APPMODE_NIL && 
		theApp.GetSymbolDatabase()->QueryHit( ptLastCanvasDown, &pHitSymbol ) &&
		!pHitSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
	{
		m_pPRHitSymbol = pHitSymbol; //we may need to know what hit symbol was during processing of popup command.

		PopupMenu.CreatePopupMenu();

		//If symbol's associated with an external note file, add option to open note.
		pHitSymbol->GetNoteLink( &strNoteLink ); 
		if( strNoteLink != _T("") )
		{
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_OPENNOTELINK, TEXT("Open Note") );
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_REMOVENOTELINK, TEXT("Remove Attachment") );
		}

		/***** NOTE ATTACHMENT WILL NOT BE IMPLEMENTED THIS VERSION.
		else
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_CREATENOTELINK, TEXT("Attach Note") );
		***********************************************************/
			
		//if the symbol references an embeded id, add options to view it and to destroy it.
		//else add option to create a new one. 
		if( pHitSymbol->GetEmbeddedID() != ILDBSET_ID_NULL ) 
		{
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_OPENEMBEDDEDLINK, TEXT("View Embedded") );
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_REMOVEEMBEDDEDLINK, TEXT("Remove Embedded") );

			//the root diagram cannot be deleted.
			if( pHitSymbol->GetEmbeddedID() != ILDBSET_ID_FIRST )
				PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_DELETEEMBEDDEDLINK, TEXT("Delete Embedded") );
		}

		else
		{
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_CREATEEMBEDDEDLINK, TEXT("Create Embedded") );
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_INSERTEMBEDDEDLINK, TEXT("Insert Embedded") );
		}		
		
		PopupMenu.TrackPopupMenu( TPM_LEFTALIGN , m_ptLastDown.x, m_ptLastDown.y, this, NULL );
		PopupMenu.DestroyMenu();
	}
	
	//else if user hit absolutely nothing while application is in "Zero Mode" and 
	//if the current diagram in the view is an embedded diagram, add an option to delete it and
	//to ascend to the container diagram. (the top level diagram view cannot be deleted)
	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_NIL && !pHitSymbol )
	{
		PopupMenu.CreatePopupMenu();

		if( pDoc->m_pDBSet->GetID() != ILDBSET_ID_FIRST )
			PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_DELETETHISDIAGRAM, TEXT("Delete Diagram") );
		
		PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_NAVIGATEFOWARD, TEXT("Foward") );
		PopupMenu.AppendMenu( MF_STRING, IDUSER_POPUP_NAVIGATEBACK, TEXT("Back") );
		PopupMenu.TrackPopupMenu( TPM_LEFTALIGN, m_ptLastDown.x, m_ptLastDown.y, this, NULL );
		PopupMenu.DestroyMenu();
	}

	//if application is in selection mode, and the user is holding down on a SELECTED symbol,
	//then fire up the multi-selection rectangle
	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL &&
			 pDoc->m_pDBSet->m_DB.QueryHit( ptLastCanvasDown, &pHitSymbol ) &&
			 pDoc->m_pDBSet->m_DB.QuerySelected( &SelectionList ) &&
			 SelectionList.Find( pHitSymbol ) )
	{
		//calculate extents of entire selection.
		//--------------------------------------

		//initialize current extents to the extents of the hit symbol.
		int MinY = pHitSymbol->m_BoundingRect.top;
		int MaxY = pHitSymbol->m_BoundingRect.bottom;
		int MinX = pHitSymbol->m_BoundingRect.left;
		int MaxX = pHitSymbol->m_BoundingRect.right;
		
		CILSymbol *pSelSymbol = NULL;
		ILPOSITION Pos = SelectionList.GetHeadPosition();
		while( Pos )
		{
			pSelSymbol = SelectionList.GetAt( Pos );
			
			if( MinX > pSelSymbol->m_BoundingRect.left ) { MinX = pSelSymbol->m_BoundingRect.left; }
			if( MinY > pSelSymbol->m_BoundingRect.top ) { MinY = pSelSymbol->m_BoundingRect.top; }
			if( MaxX < pSelSymbol->m_BoundingRect.right ) { MaxX = pSelSymbol->m_BoundingRect.right; }
			if( MaxY < pSelSymbol->m_BoundingRect.bottom ) { MaxY = pSelSymbol->m_BoundingRect.bottom; }

			SelectionList.GetNext( Pos );
		}

		m_MultiSelectRect.SetRect( MinX, MinY, MaxX, MaxY );
		//rectangle was created with canvas coords, we must convert it to window coords.
		m_MultiSelectRect.OffsetRect( CanvasToClient( &CPoint( 0, 0 ) ) ); 
		m_MultiSelectRect.InflateRect( 10, 10 );
		m_bMultiSelectMode = TRUE;
		m_MultiSelectRect.Draw( GetDC(), PS_SOLID, RGB( 50, 50, 50 ) );
		m_MultiSelectRect.Enable( TRUE );
		m_ptLastMultiDrag = m_ptLastDown;

		SelectionList.RemoveAll();
	}
}

void CVisualNotesView::OnSize( UINT nType, int cx, int cy )
{
	CScrollView::OnSize( nType, cx, cy );
	

	CRect AdjustRect;
	CMainFrame *pMainFrame = (CMainFrame*)GetParentFrame();

	if( pMainFrame && pMainFrame->GetActiveView() == this && pMainFrame->AdjustViewFromTools( &AdjustRect ))
		MoveWindow( &AdjustRect );

}

void CVisualNotesView::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
	if( uFlags == SPI_SETSIPINFO )
	{
		//Solving a quirk:
		//during full screen updating ( due to change in SIP or user returning to this application )
		//the view ( only when part of CCeDocListDocTemplate ) is resized inaccurately ( ever so slightly )
		//with respect to the top and bottom tool bars. So we manually adjust view here.
	
		CRect AdjustRect;
		if( GetParentFrame()->GetActiveView() == this &&
		    ((CMainFrame*)GetParentFrame())->AdjustViewFromTools( &AdjustRect ) )
			MoveWindow( &AdjustRect );
	}	
}


void CVisualNotesView::OnLButtonDown(UINT nFlags, CPoint Point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint CanvasPoint(Point);
	CPoint CanvasOrigin(0,0);
	CILSymbol *pNewSymbol = NULL; 
	CVisualNotesDoc *pDoc = GetDocument();
	CMainFrame *pMainFrame = (CMainFrame*)GetParentFrame();

	ReleaseCapture();
	
	m_ptLastDown = Point;
	ClientToCanvas( &CanvasPoint );
	CanvasToClient( &CanvasOrigin );	

	if( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATESYMBOL )
		m_CreateRect.Anchor( Point ); //look at OnMouseMove and OnLButtonUp for actual creation of symbol.

	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATEEDGE )
		theApp.CreateEdge( CanvasPoint );
	
	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_TRACKSYMBOL )
		theApp.TrackSymbol( Point );
		
	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL )
	{
			
		if( !pDoc->m_pDBSet->m_DB.QueryHit( CanvasPoint , &pNewSymbol /*WE'RE NOT USING THIS*/) )
		{
			m_bMultiPickMode = TRUE; //we are in multipick mode the instant the user taps nothing.
			m_MultiPickRect.Anchor( Point );
		}

		else theApp.SelectSymbol( CanvasPoint );
	}
	
	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATELABEL )
		theApp.CreateLabel( CanvasPoint );

	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_LINKSYMBOLS )
		theApp.LinkSymbols( CanvasPoint );
	
	//Only capture the mouse when drag operations are neccessary (determined by application's modes)
	if( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATESYMBOL ||
		theApp.m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL  )
		m_pwndPrevCapture = SetCapture();
	
	CScrollView::OnLButtonDown(nFlags, Point);
}

void CVisualNotesView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CVisualNotesDoc *pDoc = GetDocument();
	CILDragRect CanvasCreateRect;
	CILSymbol *pNewSymbol = NULL, *pNewSymbol2 = NULL;
	CPoint CanvasPoint(point);
	CPoint CanvasOrigin(0,0);
	
	ClientToCanvas( &CanvasPoint );
	CanvasToClient( &CanvasOrigin );

	// TODO: Add your message handler code here and/or call default
	if( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATESYMBOL && m_CreateRect.IsEnabled() )
	{
		CanvasCreateRect = m_CreateRect;
		ClientToCanvas( (CRect*)&CanvasCreateRect );
		
		CanvasCreateRect.NormalizeRect(); //rect for symbol creation must be suitable for GDI drawing.

		//Remember, just because the application is in "creation" mode
		//does NOT neccessarily mean the user has chosen a symbol to create.
		theApp.CreateShape( CanvasCreateRect );
		m_CreateRect.Enable( FALSE );
		InvalidateRect( &m_CreateRect );
	}

	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL && m_bMultiSelectMode 
		     && m_MultiSelectRect.IsEnabled() )
	{		
		theApp.MultiSelectMove( point - m_ptLastDown );
		m_MultiSelectRect.Enable( FALSE );
		m_bMultiSelectMode = FALSE;
		m_MultiSelectRect.NormalizeRect();
		InvalidateRect( &m_MultiSelectRect );
	}

	else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL && m_bMultiPickMode ) 
	{
		//if the user created any kind of a multi-pick-rect, check to see if the rect is large enough
		//to warrant multiselection. 
		if( m_MultiPickRect.IsEnabled() )
		{
			m_MultiPickRect.NormalizeRect();
			
			//if rectangle is too small peform single pick. (really we would be performing deselection since
			//this state occured from the user hitting nothing ).
			if( (m_MultiPickRect.Size().cx < CILSymbol::ms_MinSize.cx && 
				 m_MultiPickRect.Size().cy < CILSymbol::ms_MinSize.cy) )
				theApp.SelectSymbol( CanvasPoint );

			//else attempt to append a new selection to the list of selected symbols.
			else
			{				
				theApp.MultiSelectSymbol( &m_MultiPickRect );
				m_MultiPickRect.Enable( FALSE );
			}
			
			InvalidateRect( &m_MultiPickRect ); //too small or large enough, it must ALWAYS be validated.
		}

		else //else the user did not create a multi-pick-rect so we deselect previous selected ( we
			 //already know the user hit "nothing"
			theApp.SelectSymbol( CanvasPoint );
		
		m_bMultiPickMode = FALSE;
	}

	//If we have captured the mouse from another window on the last LButtonDown, give it back
	//before we release it.
	if( m_pwndPrevCapture )
	{
		m_pwndPrevCapture->SetCapture();
		m_pwndPrevCapture = NULL;
	}
	 
	ReleaseCapture();	
	CScrollView::OnLButtonUp(nFlags, point);

}

void CVisualNotesView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if( (nFlags & MK_LBUTTON) )
	{//BEGIN IF DRAG SESSION
		CRect ClientRect;
		GetClientRect( &ClientRect );
		m_ptLastDrag = point;	
		
		//the first part of the condition may be redundant since the ONLY way
		//for the point not to be in the client during this message is IF this
		//view has captured the mouse. Anyway, better safe than sorry.
		if( GetCapture() == this  && !ClientRect.PtInRect( point ) )
		{
			//if this view has captured the mouse, any movement out of the view 
			//simulates a 'mouse up' at that point.
			OnLButtonUp( nFlags, point );
			return;

			//OffScreenScroll( point );
		}

		//The second part of the  condition ensures that LBUTTONDOWN for the current 
		//"drag" session occured in this window.
		if( theApp.m_AppMode == CVisualNotesApp::APPMODE_CREATESYMBOL && GetCapture() == this )
		{
			CILDragRect NormCreateRect, NormPrevCreateRect;
				
			//save the previious creation rect (this will only be meaningful if the prev rect was enabled).
			NormPrevCreateRect = m_CreateRect;
			NormPrevCreateRect.NormalizeRect();
			
			m_CreateRect.Drag( point );
			//create a rectangle suitable for GDI drawing.
			NormCreateRect = m_CreateRect;
			NormCreateRect.NormalizeRect();

			//if the Create rect is currently disabled, it means that this is the first time
			//one is being drawn since the last LButton down message. Which means there wouldn't
			//be a previous rect.
			if( NormPrevCreateRect.IsEnabled() )
				NormPrevCreateRect.Clear( this );
			
			NormCreateRect.Draw( GetDC(), PS_SOLID, RGB(50,50,50) );
			m_CreateRect.Enable( TRUE );
		}
		
		else if( theApp.m_AppMode == CVisualNotesApp::APPMODE_SELECTSYMBOL )
		{
			if(	m_bMultiSelectMode ) 
			{
				CILDragRect PrevMultiRect;

				//save the previous multiselection rectangle.
				PrevMultiRect = m_MultiSelectRect;
				
				//we must make sure that we are only clearing a rectangle that was previously drawn.
				if( PrevMultiRect.IsEnabled() )
					PrevMultiRect.Clear( this );
				
				m_MultiSelectRect.OffsetRect( point - m_ptLastMultiDrag );
				m_ptLastMultiDrag = point;			
				
				m_MultiSelectRect.Draw( GetDC(), PS_SOLID, RGB( 125, 125, 125 ) );
				m_MultiSelectRect.Enable( TRUE ); //so that we know next time that this rect has been previously drawn.
			}

			else if( m_bMultiPickMode ) 
			{
				CILDragRect NormMultiPickRect, NormPrevMultiPickRect;
				
				//save the previious multi pick rect (this will only be meaningful if the prev rect was enabled).
				NormPrevMultiPickRect = m_MultiPickRect;
				NormPrevMultiPickRect.NormalizeRect();
				
				m_MultiPickRect.Drag( point );
				//create a rectangle suitable for GDI drawing.
				NormMultiPickRect = m_MultiPickRect;
				NormMultiPickRect.NormalizeRect();

				//if the MultiPick rect is currently disabled, it means that this is the first time
				//one is being drawn since the last LButton down message. Which means there wouldn't
				//be a previous rect.
				if( NormPrevMultiPickRect.IsEnabled() )
					NormPrevMultiPickRect.Clear( this );
				
				NormMultiPickRect.Draw( GetDC(), PS_SOLID, RGB(125,125,125) );
				m_MultiPickRect.Enable( TRUE );
			}
		}		
		
	}//END IF DRAG SESSION

	CScrollView::OnMouseMove(nFlags, point);
}	


void CVisualNotesView::OffScreenScroll( CPoint point )
{
	/*
	CRect ClientRect;
	int nScrollX = 0, nScrollY = 0;

	///*************************************************************************
	/// The rectangle I SHOULD be using is the one 
	/// returned by AdjustViewFromTools(). But I've abondoned this
	/// method for this implementation.
	/// If this mehtod is salvaged again, use the RIGHT rectangle instead of the
	/// one below.
	///**************************************************************************
	GetClientRect( &ClientRect );

	if( point.x > ClientRect.bottom )
		nScrollX = 5;

	else if( point.x < ClientRect.top )
		nScrollX = -5;

	if( point.y > ClientRect.right )
		nScrollY = 5;

	else if( point.y < ClientRect.left )
		nScrollY = - 5;

	ScrollToPosition( GetScrollPosition() + CPoint( nScrollX, nScrollY ) );
	*/
}

void CVisualNotesView::OnOpenEmbeddedLink()
{	
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	pDoc->SetCurrentDBSet( m_pPRHitSymbol->GetEmbeddedID() );
	m_DViewStack.PushNewSite( m_pPRHitSymbol->GetEmbeddedID() );

	Invalidate();
	UpdateWindow();
}

void CVisualNotesView::OnCreateEmbeddedLink()
{
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	CILDBSet *pNewSet = new CILDBSet;
	
	pDoc->m_DBSetList.AddTail( pNewSet );
	m_pPRHitSymbol->SetEmbeddedID( pNewSet->GetID() );
	pNewSet->AddLinkedSymbol( m_pPRHitSymbol );
	pDoc->SetCurrentDBSet( pNewSet->GetID() ); //The newly create diagram view will becom the current view.
	m_DViewStack.PushNewSite( pNewSet->GetID() );

	Invalidate();
	UpdateWindow();
	GetParentFrame()->GetActiveDocument()->SetModifiedFlag();
}

void CVisualNotesView::OnInsertEmbeddedLink()
{
	theApp.DoModalDiagramView( CDiagramViewDlg::modeAttach, m_pPRHitSymbol );
}

void CVisualNotesView::OnDeleteEmbeddedLink()
{	
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	//We only request a change of view if user decided to go through with the 
	//deletion of the diagram.
	if( IDCANCEL != theApp.ConfirmedDeleteDBSet( m_pPRHitSymbol->GetEmbeddedID() ) )
	{
		pDoc->SetCurrentDBSet( m_DViewStack.UpdateDeletedDiagrams( pDoc ) );
		Invalidate();
		UpdateWindow();
		pDoc->SetModifiedFlag();
	}
}

void CVisualNotesView::OnDeleteThisDiagram()
{

	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	//We only request a change of view if user decided to go through with the 
	//deletion of the diagram.
	if( IDCANCEL != theApp.ConfirmedDeleteDBSet( pDoc->m_pDBSet->GetID() ) )
	{
		pDoc->SetCurrentDBSet( m_DViewStack.UpdateDeletedDiagrams( pDoc ) );
		Invalidate();
		UpdateWindow();	
		pDoc->SetModifiedFlag();
	}
}

void CVisualNotesView::OnRemoveEmbeddedLink()
{
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );


	UINT UserChoice = theApp.ConfirmedRemoveLinks( m_pPRHitSymbol, 0 );
	CILDBSet *pSet = NULL;
	//give user a chance to revert his/her action.
	if( UserChoice == IDCANCEL )
		return;

	//if user decided to leave the orphaned diagram, we must explicitly
	//remove the link symbol from it's respective diagram.
	else if( UserChoice == IDNO )
	{
		pSet = pDoc->FindDBSet( m_pPRHitSymbol->GetEmbeddedID() );
		pSet->RemoveLinkedSymbol( m_pPRHitSymbol );
		m_pPRHitSymbol->SetEmbeddedID( ILDBSET_ID_NULL );
		pDoc->SetModifiedFlag();
	}

	m_DViewStack.UpdateDeletedDiagrams( pDoc );
}

void CVisualNotesView::OnNavigateBack()
{
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	UINT id;

	if( ( id = m_DViewStack.PopBack() ) )
	{
		pDoc->SetCurrentDBSet( id );
		Invalidate();
		UpdateWindow();
	}
}

void CVisualNotesView::OnNavigateFoward()
{
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	UINT id;

	if( ( id = m_DViewStack.PopFoward() ) )
	{
		pDoc->SetCurrentDBSet( id );
		Invalidate();
		UpdateWindow();
	}	
}

void CVisualNotesView::OnCreateNoteLink()
{
	CVisualNotesDoc *pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	CNewNoteDlg TestDlg;
		
	if( IDOK == TestDlg.DoModal() )
	{
		if( TestDlg.m_strSelectedPath != _T("") )
			m_pPRHitSymbol->SetNoteLink( TestDlg.m_strSelectedPath );
		pDoc->SetModifiedFlag();
	}
}	

void CVisualNotesView::OnOpenNoteLink()
{
	CString strNoteLink;
	PROCESS_INFORMATION ProcInfo;

	m_pPRHitSymbol->GetNoteLink( &strNoteLink );

	CreateProcess( _T("notes.exe"), strNoteLink, 
				   NULL, NULL, NULL, NULL, NULL, NULL, NULL, &ProcInfo );
}

void CVisualNotesView::OnRemoveNoteLink()
{
	m_pPRHitSymbol->SetNoteLink( CString(_T("")) );
}

void CVisualNotesView::OnOpenWebLink()
{
	PROCESS_INFORMATION ProcInfo;
	CreateProcess( TEXT("iexplore.exe"), TEXT("http://www.msn.com"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, &ProcInfo );
}

CRect CVisualNotesView::ClientToCanvas( CRect *pRect )
{	pRect->OffsetRect( GetScrollPosition() ); return *pRect; }

CPoint CVisualNotesView::ClientToCanvas( CPoint *pPoint )
{	pPoint->Offset( GetScrollPosition() ); return *pPoint; }

CRect CVisualNotesView::CanvasToClient( CRect *pRect )
{	pRect->OffsetRect( -GetScrollPosition() ); return *pRect; }

CPoint CVisualNotesView::CanvasToClient( CPoint *pPoint )
{	pPoint->Offset( -GetScrollPosition() ); return *pPoint; }

void CVisualNotesView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonDblClk(nFlags, point);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	CILNavigationStack Implementation.
//

CILNavigationStack::CILNavigationStack(): m_nIndex(0), m_nTop(0)
{

	m_stack.SetSize( 10, 10 );
	m_stack[m_nIndex] = ILDBSET_ID_FIRST;
}

CILNavigationStack::~CILNavigationStack()
{
	m_stack.RemoveAll();
}

UINT CILNavigationStack::PopBack()
{

	if( m_nIndex == 0 )
		return ILDBSET_ID_NULL;

	return m_stack[--m_nIndex];
}

UINT CILNavigationStack::PopFoward()
{

	if( m_nIndex == m_nTop )
		return ILDBSET_ID_NULL;

	return m_stack[++m_nIndex];
}
		
void CILNavigationStack::PushNewSite( UINT id )
{
	m_stack.SetAtGrow( ++m_nIndex, id );
	m_nTop = m_nIndex;
}

UINT CILNavigationStack::Reset()
{
	m_nIndex = 0;
	m_nTop = 0;
	return m_stack[m_nIndex];
}

UINT CILNavigationStack::UpdateDeletedDiagrams( CVisualNotesDoc *pDoc )
{
	ASSERT_VALID( pDoc );

	CILArray<UINT,UINT> TempStack;
	TempStack.SetSize( 0, 10 );
	int NewTop = -1, nIndexDropCount = 0;

	for( int i = 0; i <= m_nTop; i++ )
	{
		//retaining the same order, copy only id's that still exist into temp array. 
		if( pDoc->FindDBSet( m_stack[i] ) )
		{
			TempStack.Add( m_stack[i] );
			NewTop++;
		}
		
		//count the diagrams that were deleted below and up to the current
		//index in the old stack. This is how much the new index will decrease by.
		else if( i <= m_nIndex )
			nIndexDropCount++;
	}
	
	m_stack.RemoveAll();
	m_stack.Copy( TempStack );	
	
	m_nTop = NewTop;
	m_nIndex-= nIndexDropCount;
	
	return m_stack[m_nIndex];
}

void CVisualNotesView::OnToolsExport() 
{
	// TODO: Add your command handler code here
	CFileDialog ExportDlg(FALSE);
	TCHAR szFilter[256] = TEXT( "Windows Bitmap (.bmp)\0*.bmp\0" );
	TCHAR szFileTitle[256] = TEXT( "Export Bitmap" );
	CFile fileBmp;
	SHORT nBytesPerPixel = sizeof( BYTE ) * 4;
	
	ExportDlg.m_ofn.lpstrFileTitle = szFileTitle;
	ExportDlg.m_ofn.nMaxFileTitle = 256;
	ExportDlg.m_ofn.lpstrFilter = szFilter;
	ExportDlg.m_ofn.nFilterIndex  = 1;
	ExportDlg.m_ofn.nMaxFile = 256;
	wcscpy( ExportDlg.m_ofn.lpstrFile, GetDocument()->GetTitle() );
	
	if( IDOK != ExportDlg.DoModal() )
		return;

	if( !fileBmp.Open( ExportDlg.GetPathName(), 
					   CFile::modeWrite | 
					   CFile::shareDenyNone | 
					   CFile::modeCreate  ) )
		return;

	BITMAPFILEHEADER bmfilehdr;
	BITMAPINFOHEADER bminfohdr;
	CRect rectDiagram;
	DWORD dwPixel;
	SHORT nBytePadding;
	DWORD dwZeroPad = 0;
	CWnd TempWindow;

	rectDiagram.top = rectDiagram.left = 0;
	rectDiagram.right = GetTotalSize().cx;
	rectDiagram.bottom = GetTotalSize().cy;

	nBytePadding = (rectDiagram.Width() *nBytesPerPixel) % 4;

	bmfilehdr.bfType = 0x4D42;
	bmfilehdr.bfSize=  sizeof( BITMAPFILEHEADER ) + 
					   sizeof( BITMAPINFOHEADER ) + 
					   nBytesPerPixel *
					   rectDiagram.Height() *
					   ( rectDiagram.Width() + nBytePadding );
					   
	bmfilehdr.bfReserved1 = bmfilehdr.bfReserved2 = 0;
	bmfilehdr.bfOffBits = sizeof( BITMAPINFOHEADER );

	bminfohdr.biSize = sizeof( BITMAPINFOHEADER );
	bminfohdr.biBitCount = nBytesPerPixel * 8;
	bminfohdr.biWidth = rectDiagram.Width();
	bminfohdr.biHeight = rectDiagram.Height();
	bminfohdr.biPlanes = 1;
	bminfohdr.biCompression = BI_RGB;
	bminfohdr.biSizeImage = 0;
	bminfohdr.biXPelsPerMeter = 2835; //72 dots per inch.
	bminfohdr.biYPelsPerMeter = 2835; //72 dots per inch.
	bminfohdr.biClrImportant = 0;
	bminfohdr.biClrUsed = 0;
	
	fileBmp.Write( &bmfilehdr, sizeof( BITMAPFILEHEADER ) );
	fileBmp.Write( &bminfohdr, sizeof( BITMAPINFOHEADER ) );

	CBitmap TempBitmap;
	CDC BitmapDC;

	TempBitmap.CreateCompatibleBitmap( GetDC(), rectDiagram.Width(), rectDiagram.Height() );
	BitmapDC.CreateCompatibleDC( GetDC() );
	BitmapDC.SelectObject( &TempBitmap );
	BitmapDC.FillRect( &rectDiagram, &CBrush( RGB(255,255,255) ) );
	OnDraw( &BitmapDC );

	//BYTE *pScanLine = new BYTE[bminfohdr.biWidth + nBytePadding];
	CProgressCtrl ProgressBar;
	CRect rectProgress( 60, 50, 160, 70 );
	CRect rectView;
	DWORD *pScanLine = NULL;

	GetClientRect( &rectView );
	GetDC()->FillRect( &rectView, &CBrush( RGB( 255, 255, 255 ) ) ); //clear view for progress bar.
	ProgressBar.Create( PBS_SMOOTH, rectProgress, this, 0 );
	ProgressBar.SetRange( 0, (short)bminfohdr.biHeight );
	ProgressBar.SetStep( 1 );
	ProgressBar.SetWindowText( TEXT( "Exporting" ) );
	ProgressBar.ShowWindow( SW_SHOW );
	rectProgress.OffsetRect( 0, -rectProgress.Height() );
	GetDC()->DrawText( TEXT( "Exporting..." ), &rectProgress, DT_CENTER );

	pScanLine = new DWORD[bminfohdr.biWidth] ;
	for( int i = 0; i < bminfohdr.biHeight; i++ )
	{
		for( int j = 0; j < bminfohdr.biWidth; j++ )
		{
			//we will be reading surface scanlines bottom-up.
			dwPixel = BitmapDC.GetPixel( j, bminfohdr.biHeight - 1 - i ); 
			dwPixel = RGB( GetBValue( dwPixel ), GetGValue( dwPixel ), GetRValue( dwPixel ) );
			*(pScanLine + j ) = dwPixel;
			//fileBmp.Write( &dwPixel, nBytesPerPixel );
		}
	
		fileBmp.Write( pScanLine, sizeof( DWORD ) * bminfohdr.biWidth );		

		//Each scan line may have to be zero padded.
		if( nBytePadding )
			fileBmp.Write( &dwZeroPad, nBytePadding );

		ProgressBar.StepIt();
	}
	
	fileBmp.Close();	
	BitmapDC.DeleteDC();
	TempBitmap.DeleteObject();
	ProgressBar.DestroyWindow();
	delete[] pScanLine;
}
