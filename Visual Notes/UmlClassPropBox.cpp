// UmlClassPropBox.cpp : implementation file
//
#include "stdafx.h"
#include "illist.h"
#include "resource.h"
#include "UmlClassPropBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUmlClassPropBox dialog


CUmlClassPropBox::CUmlClassPropBox(CWnd* pParent /*=NULL*/)
	: CDialog(CUmlClassPropBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUmlClassPropBox)
	m_strClassTitle = _T("");
	m_strSelMethod = _T("");
	//}}AFX_DATA_INIT
}


void CUmlClassPropBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUmlClassPropBox)
	DDX_Text(pDX, IDC_EDIT_CLASSTITLE, m_strClassTitle);
	DDV_MaxChars(pDX, m_strClassTitle, 30);
	DDX_Text(pDX, IDC_EDIT_SELMETHOD, m_strSelMethod);
	DDV_MaxChars(pDX, m_strSelMethod, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUmlClassPropBox, CDialog)
	//{{AFX_MSG_MAP(CUmlClassPropBox)
	ON_BN_CLICKED(IDC_BUTTON_ADDNEW, OnButtonAddNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL, OnButtonDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	ON_LBN_SELCHANGE(IDC_LIST_METHODS, OnSelchangeListMethods)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUmlClassPropBox message handlers

void CUmlClassPropBox::OnButtonAddNew() 
{
	// TODO: Add your control notification handler code here.
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );
	CEdit *peditSelMethod = (CEdit*)GetDlgItem( IDC_EDIT_SELMETHOD );
	CString strWindowText;

	//before we add the new item, we must make sure it does not already exist
	peditSelMethod->GetWindowText( strWindowText );

	strWindowText.TrimLeft();
	strWindowText.TrimRight();

	if( plbMethods->FindStringExact( -1, strWindowText ) != LB_ERR )
	{
		MessageBox( TEXT("Duplicate methods are not allowed"), TEXT("Add New"), MB_ICONERROR );
		return;
	}

	if( strWindowText == _T("") )
	{
		MessageBox( TEXT("New method must have a name"), TEXT( "Add New"), MB_ICONERROR );
		return;
	}

	plbMethods->SetCurSel( plbMethods->AddString( strWindowText ) );
	
	UpdateData( TRUE );
}

void CUmlClassPropBox::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );
	CEdit* peditSelMethod = (CEdit*)GetDlgItem( IDC_EDIT_SELMETHOD );
	CString strSelString;
	int nIndex;

	if( plbMethods->GetCount() && ( nIndex = plbMethods->GetCurSel() ) == LB_ERR )
	{
		MessageBox( TEXT("No item is currently selected"), TEXT( "Delete" ), MB_ICONERROR );
		return;
	}

	plbMethods->DeleteString( nIndex );

	if( plbMethods->SetCurSel(0) != LB_ERR )
	{
		plbMethods->GetText( 0, strSelString );
		peditSelMethod->SetWindowText( strSelString );
	}

	else
		peditSelMethod->SetWindowText( _T("") );

	UpdateData( TRUE );
}

void CUmlClassPropBox::OnButtonDeleteAll() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );
	CEdit *peditSelMethod = (CEdit*)GetDlgItem( IDC_EDIT_SELMETHOD );

	plbMethods->ResetContent();
	peditSelMethod->SetWindowText( _T("") );
	
	UpdateData( TRUE );
}

void CUmlClassPropBox::OnButtonRename() 
{
	// TODO: Add your control notification handler code here
	CString strWindowText;
	CEdit *peditSelMethod = (CEdit*)GetDlgItem( IDC_EDIT_SELMETHOD );
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );
	int nIndex;

	//if there are no methods to be renamed, we should return now.
	if( !plbMethods->GetCount() )
		return;

	//Before renaming an item, we must perform a few validation rules.
	peditSelMethod->GetWindowText( strWindowText );

	strWindowText.TrimLeft();
	strWindowText.TrimRight();

	if( strWindowText == m_strSelMethod )
		return;

	//1.Name cannot be an empty string.
	if( strWindowText == _T("") )
	{
		MessageBox( TEXT("Method must have a name"), TEXT( "Rename"), MB_ICONERROR );
		return;
	}

	//2.Duplicate Names not allowed.
	if( plbMethods->FindStringExact( -1, strWindowText ) != LB_ERR )
	{
		MessageBox( TEXT("Duplicate methods not allowed"), TEXT( "Rename" ), MB_ICONERROR );
		peditSelMethod->SetWindowText( m_strSelMethod ); //in this case we restore the previous name.
		return;
	}

	nIndex = plbMethods->FindStringExact( -1, m_strSelMethod );
	plbMethods->DeleteString( nIndex );
	plbMethods->AddString( strWindowText );
	UpdateData( TRUE );
}

BOOL CUmlClassPropBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );
	
	
	ILPOSITION Pos = m_strlstMethods.GetHeadPosition();
	while( Pos )
	{
		plbMethods->AddString( m_strlstMethods.GetAt( Pos ) );
		m_strlstMethods.GetNext( Pos );
	}

	if( m_strSelMethod != _T("") )
		plbMethods->SelectString( -1, m_strSelMethod );

	UpdateData( FALSE );

	m_retID = IDCANCEL;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUmlClassPropBox::OnSelchangeListMethods() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );

	plbMethods->GetText( plbMethods->GetCurSel(), m_strSelMethod );
	UpdateData( FALSE );
}

void CUmlClassPropBox::OnOK()
{
	CListBox *plbMethods = (CListBox*)GetDlgItem( IDC_LIST_METHODS );
	CString strMethod;
	
	m_strlstMethods.RemoveAll();
	for( int i = 0; i < plbMethods->GetCount(); i++ )
	{
		plbMethods->GetText( i, strMethod );
		m_strlstMethods.AddTail( strMethod );
	}

	m_retID = IDOK;
	CDialog::OnOK();
}
