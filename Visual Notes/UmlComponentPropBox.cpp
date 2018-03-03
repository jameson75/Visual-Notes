// UmlComponentPropBox.cpp : implementation file
//
#include "stdafx.h"
#include "illist.h"
#include "resource.h"
#include "UmlComponentPropBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUmlComponentPropBox dialog

CUmlComponentPropBox::CUmlComponentPropBox(CWnd* pParent /*=NULL*/)
	: CDialog(CUmlComponentPropBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUmlComponentPropBox)
	m_strSelInterface = _T("");
	//}}AFX_DATA_INIT
}


void CUmlComponentPropBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUmlComponentPropBox)
	DDX_Text(pDX, IDC_EDIT_SELINTERFACE, m_strSelInterface);
	DDV_MaxChars(pDX, m_strSelInterface, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUmlComponentPropBox, CDialog)
	//{{AFX_MSG_MAP(CUmlComponentPropBox)
	ON_BN_CLICKED(IDC_ADDNEW, OnAddNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL, OnButtonDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	ON_LBN_SELCHANGE(IDC_LIST_INTERFACES, OnSelchangeListInterfaces)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUmlComponentPropBox message handlers

void CUmlComponentPropBox::OnAddNew() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );
	CEdit *peditSelInterface = (CEdit*)GetDlgItem( IDC_EDIT_SELINTERFACE );
	CString strWindowText;

	//before we add the new item, we must make sure it does not already exist
	peditSelInterface->GetWindowText( strWindowText );

	strWindowText.TrimLeft();
	strWindowText.TrimRight();

	if( plbInterfaces->FindStringExact( -1, strWindowText ) != LB_ERR )
	{
		MessageBox( TEXT("Duplicate Interfaces are not allowed"), TEXT("Add New"), MB_ICONERROR );
		return;
	}

	if( strWindowText == _T("") )
	{
		MessageBox( TEXT("New Interface must have a name"), TEXT( "Add New"), MB_ICONERROR );
		return;
	}

	plbInterfaces->SetCurSel( plbInterfaces->AddString( strWindowText ) );
	
	UpdateData( TRUE );
	
}

void CUmlComponentPropBox::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );
	CEdit* peditSelInterface = (CEdit*)GetDlgItem( IDC_EDIT_SELINTERFACE );
	CString strSelString;
	int nIndex;

	if( plbInterfaces->GetCount() && ( nIndex = plbInterfaces->GetCurSel() ) == LB_ERR )
	{
		MessageBox( TEXT("No item is currently selected"), TEXT( "Delete" ), MB_ICONERROR );
		return;
	}

	plbInterfaces->DeleteString( nIndex );

	if( plbInterfaces->SetCurSel(0) != LB_ERR )
	{
		plbInterfaces->GetText( 0, strSelString );
		peditSelInterface->SetWindowText( strSelString );
	}

	else
		peditSelInterface->SetWindowText( _T("") );


	UpdateData( TRUE );
}

void CUmlComponentPropBox::OnButtonDeleteAll() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );
	CEdit *peditSelInterface = (CEdit*)GetDlgItem( IDC_EDIT_SELINTERFACE );

	plbInterfaces->ResetContent();
	peditSelInterface->SetWindowText( _T("") );
	
	UpdateData( TRUE );
	
}

void CUmlComponentPropBox::OnButtonRename() 
{
	// TODO: Add your control notification handler code here
	CString strWindowText;
	CEdit *peditSelInterface = (CEdit*)GetDlgItem( IDC_EDIT_SELINTERFACE );
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );
	int nIndex;

	//If there are no Interfaces to be renamed, we should return now.
	if( !plbInterfaces->GetCount() )
		return;

	//Before renaming an item, we must perform a few validation rules.
	peditSelInterface->GetWindowText( strWindowText );

	strWindowText.TrimLeft();
	strWindowText.TrimRight();

	if( strWindowText == m_strSelInterface )
		return;

	//1.Name cannot be an empty string.
	if( strWindowText == _T("") )
	{
		MessageBox( TEXT("Interface must have a name"), TEXT( "Rename"), MB_ICONERROR );
		return;
	}

	//2.Duplicate Names not allowed.
	if( plbInterfaces->FindStringExact( -1, strWindowText ) != LB_ERR )
	{
		MessageBox( TEXT("Duplicate Interfaces not allowed"), TEXT( "Rename" ), MB_ICONERROR );
		peditSelInterface->SetWindowText( m_strSelInterface ); //in this case we restore the previous name.
		return;
	}

	nIndex = plbInterfaces->FindStringExact( -1,  m_strSelInterface );
	plbInterfaces->DeleteString( nIndex );
	plbInterfaces->AddString( strWindowText );
	UpdateData( TRUE );	
}

BOOL CUmlComponentPropBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );

	ILPOSITION Pos = m_strlstInterfaces.GetHeadPosition();
	while( Pos )
	{
		plbInterfaces->AddString( m_strlstInterfaces.GetAt( Pos ) );
		m_strlstInterfaces.GetNext( Pos );
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUmlComponentPropBox::OnSelchangeListInterfaces() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );

	plbInterfaces->GetText( plbInterfaces->GetCurSel(), m_strSelInterface );
	UpdateData( FALSE );
}

void CUmlComponentPropBox::OnOK()
{
	CListBox *plbInterfaces = (CListBox*)GetDlgItem( IDC_LIST_INTERFACES );
	CString strInterface;
	
	m_strlstInterfaces.RemoveAll();
	for( int i = 0; i < plbInterfaces->GetCount(); i++ )
	{
		plbInterfaces->GetText( i, strInterface );
		m_strlstInterfaces.AddTail( strInterface );
	}

	m_retID = IDOK;
	CDialog::OnOK();
}
