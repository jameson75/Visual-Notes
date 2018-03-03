// DiagramViewDlg.cpp : implementation file
//
#include "stdafx.h"
#include "illist.h"
#include "resource.h"
#include "DiagramViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramViewDlg dialog


CDiagramViewDlg::CDiagramViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagramViewDlg::IDD, pParent), m_DVMode( modeNormal ), m_nSelectedID(0), m_bVisitSelected( FALSE )
{
	//{{AFX_DATA_INIT(CDiagramViewDlg)
	m_strCurrentDiagram = _T("");
	m_strSelectedDiagram = _T("");
	//}}AFX_DATA_INIT
}

void CDiagramViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagramViewDlg)
	DDX_Text(pDX, IDC_EDIT_CURRENTDIAGRAM, m_strCurrentDiagram);
	DDX_Text(pDX, IDC_EDIT_SELECTEDDIAGRAM, m_strSelectedDiagram);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDiagramViewDlg, CDialog)
	//{{AFX_MSG_MAP(CDiagramViewDlg)
	ON_LBN_SELCHANGE(IDC_LIST_DIAGRAMS, OnSelchangeListDiagrams)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_VISIT, OnButtonVisit)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagramViewDlg message handlers

BOOL CDiagramViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CListBox *plbDiagrams = (CListBox*)GetDlgItem( IDC_LIST_DIAGRAMS );
	CEdit *peditSelectedDiagram = (CEdit*)GetDlgItem( IDC_EDIT_SELECTEDDIAGRAM );
	CButton *pbtnRename = (CButton*)GetDlgItem( IDC_BUTTON_RENAME );
	UINT nIndex;
	
	ILPOSITION StrPos = m_strlistDiagrams.GetHeadPosition();
	ILPOSITION DatPos = m_idlistDiagrams.GetHeadPosition();
	while( StrPos )
	{
		nIndex = plbDiagrams->AddString( m_strlistDiagrams.GetAt( StrPos ) );
		
		//We'll ASSUME that caller properly initialzed both lists with 
		//the same amount of data.
		plbDiagrams->SetItemData( nIndex, m_idlistDiagrams.GetAt( DatPos ) );
		m_strlistDiagrams.GetNext( StrPos );
		m_idlistDiagrams.GetNext( DatPos );
	}

	//Initialize Attach Mode.
	//-----------------------
	//If we are just attaching a diagram view to a link-symbol, only give user ability
	//to rename diagram from selected-diagram edit box ( disable every thing else. )
	//Also, we need to remove the current diagram from the list box to prevent
	//"self-linking".
	if( m_DVMode == modeAttach )
	{
		_AttachModeDisableWindows();
		plbDiagrams->DeleteString( plbDiagrams->FindStringExact( -1, m_strCurrentDiagram ) );

		if( plbDiagrams->GetCount() )
		{
			//we may have just removed the root, so...
			//if root diagram is in the list box, select it and leave selected-diagram edit box disabled.
			//else select the first item in the list box and enable the selected-diagram edit box.
			nIndex = plbDiagrams->FindStringExact( -1, TEXT("_root") );
			if( nIndex != LB_ERR )
			{
				plbDiagrams->SetCurSel( nIndex );
				_RootSelDisableWindows();
			}
		
			else
			{
				nIndex = 0;
				plbDiagrams->SetCurSel( nIndex );
				peditSelectedDiagram->EnableWindow();
			}

			plbDiagrams->GetText( nIndex, m_strSelectedDiagram );
			m_nSelectedID = plbDiagrams->GetItemData( nIndex );
		}
	
		else
			_NoItemDisableWindows();
	}

	//Init normal Mode
	//----------------
	else
	{
		//we can make the current diagram the current seletion.
		plbDiagrams->SetCurSel( plbDiagrams->FindStringExact( -1, m_strCurrentDiagram ) );
		m_strSelectedDiagram = m_strCurrentDiagram;

		if( m_strSelectedDiagram == TEXT( "_root" ) )
			_RootSelDisableWindows();
	}	

	UpdateData( FALSE );		

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE.
}

void CDiagramViewDlg::OnSelchangeListDiagrams() 
{
	// TODO: Add your control notification handler code here
	
	//Display the name of the current diagram in the current-diagram edit box
	//so user may have access to change it's name.
	//========================================================================

	CListBox *plbDiagrams = (CListBox*)GetDlgItem( IDC_LIST_DIAGRAMS );
		
	plbDiagrams->GetText( plbDiagrams->GetCurSel(), m_strSelectedDiagram );
	m_nSelectedID = plbDiagrams->GetItemData( plbDiagrams->GetCurSel() );

	//root diagram's name cannot be changed by user.
	if( m_strSelectedDiagram == TEXT( "_root" ) )
		_RootSelDisableWindows();		
	
	else
		_RootSelDisableWindows( FALSE );
		
	UpdateData( FALSE );	
}

void CDiagramViewDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbDiagrams = (CListBox*)GetDlgItem( IDC_LIST_DIAGRAMS );
	CEdit *peditSelectedDiagram = (CEdit*)GetDlgItem( IDC_EDIT_SELECTEDDIAGRAM );
	CString strSelection;
	ILPOSITION Pos;
	int nSelIndex;
	UINT DiagramID;

	//we use current selection in listbox instead of the selected-diagram textbox string
	//because user may have changed the selected textbox string without clicking "rename" as yet.
	if( ( nSelIndex = plbDiagrams->GetCurSel() ) == LB_ERR )
	{
		MessageBox( TEXT("You must select a diagram to be deleted."), TEXT("Error"), MB_ICONERROR );
		return;
	}
	
	plbDiagrams->GetText( nSelIndex, strSelection );
	DiagramID = plbDiagrams->GetItemData( nSelIndex );

	//deletion of root diagram not permitted.
	if( strSelection == TEXT("_root") )
		return;
	
	//we now need to delete the diagram item from BOTH the list box and the stringlist.
	//the caller of this modal dialog box will be able to look at the stringlist to determine which diagram
	//has been deleted.
	Pos = m_strlistDiagrams.Find( strSelection );
	ASSERT( Pos );
	m_strlistDiagrams.RemoveAt( Pos );
	m_idlistDiagrams.RemoveAt( m_idlistDiagrams.Find( DiagramID ) );
	plbDiagrams->DeleteString( nSelIndex );
	
	//if the diagram user wants to delete is the current diagram. then specify "deleted" in the 
	//current-diagram text box.
	if( strSelection == m_strCurrentDiagram )
		m_strCurrentDiagram = TEXT( "<deleted>" );
		
	//if the user deleted the last diagram in the listbox, disable appropiate controls.
	//At the current time of the implementation this condition should never happen because the root diagram
	//is ALWAYS in the project.
	if( !plbDiagrams->GetCount() )
	{
		m_strSelectedDiagram = _T("");
		m_nSelectedID = 0;
		_NoItemDisableWindows();
	}

	//otherwise, select the first item in the list box.
	else
	{
		plbDiagrams->GetText( 0, m_strSelectedDiagram );
		plbDiagrams->SetCurSel( 0 );
		m_nSelectedID = plbDiagrams->GetItemData( 0 );
	}
	
	UpdateData( FALSE );
}

void CDiagramViewDlg::OnButtonVisit() 
{
	// TODO: Add your control notification handler code here	
	m_bVisitSelected = TRUE;
	OnOK();
}

void CDiagramViewDlg::OnButtonRename() 
{
	// TODO: Add your control notification handler code here
	int nNewIndex, nOldIndex;
	UINT DiagramID;

	CListBox *plbDiagrams = (CListBox*)GetDlgItem( IDC_LIST_DIAGRAMS );
	CEdit *peditSelectedDiagram = (CEdit*)GetDlgItem( IDC_EDIT_SELECTEDDIAGRAM );
	CString strPrevName;

	//Make sure item is selected.
	if( ( nOldIndex = plbDiagrams->GetCurSel() ) == LB_ERR )
	{
		MessageBox( TEXT("Select one item from list box to rename"), TEXT( "Visual Notes" ), MB_OK );
		return;
	}

	//save original name.
	plbDiagrams->GetText( nOldIndex, strPrevName );

	//get "selected diagram" name user wants to change to.
	UpdateData( TRUE );

	//Get rid of white space characters in the beginning and end of name.
	m_strSelectedDiagram.TrimLeft();
	m_strSelectedDiagram.TrimRight();

	//if user made no changes, validation passes.
	if( strPrevName == m_strSelectedDiagram )
		return;

	//Perform neccessary validation
	//-----------------------------
	//1. Validate Empty string.
	if( m_strSelectedDiagram == _T("") )
	{
		MessageBox( TEXT("Diagram must have a name."), TEXT("Diagram Validation"), MB_ICONERROR );
		peditSelectedDiagram->SetFocus();
		return;
	}

	//2. Validate Duplicate names. Incase we are in attach mode, we need to compare test for
	//	 duplicates against "current diagram" string since it's removed from lb in attach mode.
	if( plbDiagrams->FindStringExact( -1, m_strSelectedDiagram) != LB_ERR ||
		m_strSelectedDiagram == m_strCurrentDiagram )
	{
		MessageBox( TEXT("A diagram already exists with this name."), TEXT("Diagram Validatiion"), MB_ICONERROR );
		peditSelectedDiagram->SetFocus();
		//reset "slected diagram" textbox to it's original value.
		m_strSelectedDiagram = strPrevName;
		UpdateData( FALSE );
		return;
	}

	//3. Validate resreved diagram names
	if( m_strSelectedDiagram == TEXT("<deleted>") ||
		m_strSelectedDiagram.Find( TEXT("_delete_") ) != -1 )
	{
		MessageBox( TEXT( "You have specified a reserved diagram name. Please choose anohter"), TEXT( "Diagram Validation" ), 
			        MB_ICONERROR );
		peditSelectedDiagram->SetFocus();
		m_strSelectedDiagram = strPrevName;
		UpdateData( FALSE );
		return;
	}

	//Allow the user to rename a diagram via "selected diagram" edit box.
	//===================================================================
	//Out with the old...
	DiagramID = plbDiagrams->GetItemData( nOldIndex );
	plbDiagrams->DeleteString( nOldIndex );
	
	//In with the new...
	nNewIndex = plbDiagrams->AddString( m_strSelectedDiagram );
	plbDiagrams->SetItemData( nNewIndex, DiagramID );
	plbDiagrams->SetCurSel( nNewIndex );

	//if user has changed the name of the current diagram, update the diagram name in the "current diagram" text box.
	if( strPrevName == m_strCurrentDiagram )
	{
		m_strCurrentDiagram = m_strSelectedDiagram;
		UpdateData( FALSE );
	}

	//now we need to update the string list.
	ILPOSITION Pos = m_strlistDiagrams.Find( strPrevName );
	ASSERT( Pos ); //previous name should always be in the string list.
	m_strlistDiagrams.SetAt( m_strSelectedDiagram, Pos ); 
}

void CDiagramViewDlg::_NoItemDisableWindows()
{
	//We don't need an argument for this method since at the time of this implementation, it is impossible
	//to add any items once there are no items left.

	//EVERY THING GETS DISABLE ( except for cancel button ofcourse )
	CButton *pbtnRename = (CButton*)GetDlgItem( IDC_BUTTON_RENAME );
	CButton *pbtnDelete = (CButton*)GetDlgItem( IDC_BUTTON_DELETE );
	CButton *pbtnVisit = (CButton*)GetDlgItem( IDC_BUTTON_VISIT );
	CListBox *plbDiagrams = (CListBox*)GetDlgItem( IDC_LIST_DIAGRAMS );
	CEdit *peditSelectedDiagram = (CEdit*)GetDlgItem( IDC_EDIT_SELECTEDDIAGRAM );
	
	pbtnRename->EnableWindow( FALSE );
	pbtnDelete->EnableWindow( FALSE );
	pbtnVisit->EnableWindow( FALSE );
	plbDiagrams->EnableWindow( FALSE );
	peditSelectedDiagram->EnableWindow( FALSE );
}

void CDiagramViewDlg::_RootSelDisableWindows( BOOL bDisable )
{
	CButton *pbtnRename = (CButton*)GetDlgItem( IDC_BUTTON_RENAME );
	CButton *pbtnDelete = (CButton*)GetDlgItem( IDC_BUTTON_DELETE );
	CEdit *peditSelectedDiagram = (CEdit*)GetDlgItem( IDC_EDIT_SELECTEDDIAGRAM );
	
	if( bDisable )
	{
		//no need to disable "delete" we are already in attach mode. ( they should already be disabled )
		if( m_DVMode != modeAttach )
			pbtnDelete->EnableWindow( FALSE );
			
		peditSelectedDiagram->EnableWindow( FALSE );
		pbtnRename->EnableWindow( FALSE );
	}

	else
	{
		//if we are in attach mode, then "delete" doesn't get enabled.
		if( m_DVMode != modeAttach )
			pbtnDelete->EnableWindow( TRUE );
			
		peditSelectedDiagram->EnableWindow( TRUE );
		pbtnRename->EnableWindow( TRUE );
	}
}

void CDiagramViewDlg::_AttachModeDisableWindows()
{
	//these are the following windows that are PERMANENTLY disabled during attach mode.
	CButton *pbtnDelete = (CButton*)GetDlgItem( IDC_BUTTON_DELETE );
	CButton *pbtnVisit = (CButton*)GetDlgItem( IDC_BUTTON_VISIT );

	pbtnDelete->EnableWindow( FALSE );
	pbtnVisit->EnableWindow( FALSE );
}





	
