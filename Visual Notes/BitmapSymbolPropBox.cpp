// BitmapSymbolPropBox.cpp : implementation file
//
#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "ColorButton.h"
#include "BitmapSymbolPropBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapSymbolPropBox dialog


CBitmapSymbolPropBox::CBitmapSymbolPropBox(CWnd* pParent /*=NULL*/)
	: CDialog(CBitmapSymbolPropBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBitmapSymbolPropBox)
	m_bMakeCurrent = FALSE;
	m_bAllowStreching = FALSE;
	//}}AFX_DATA_INIT
}

void CBitmapSymbolPropBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBitmapSymbolPropBox)
	DDX_Check(pDX, IDC_CHECK_MAKECURRENT, m_bMakeCurrent);
	DDX_Check(pDX, IDC_CHECK_ALLOWSTRECHING, m_bAllowStreching);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBitmapSymbolPropBox, CDialog)
	//{{AFX_MSG_MAP(CBitmapSymbolPropBox)
	ON_LBN_SELCHANGE(IDC_LIST_AVAILABLEBMPS, OnSelchangeListAvailablebmps)
	ON_LBN_DBLCLK(IDC_LIST_AVAILABLEBMPS, OnDblclkListAvailablebmps)
	ON_BN_CLICKED(IDC_BUTTON_RESTOREDEFAULT, OnButtonRestoreDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapSymbolPropBox message handlers
BOOL CBitmapSymbolPropBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here.
	InitBmpProperties();

	UpdateData( FALSE );
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBitmapSymbolPropBox::InitBmpProperties()
{
	CListBox *plbAvailableBmps = (CListBox*)GetDlgItem( IDC_LIST_AVAILABLEBMPS );
	

	ILPOSITION Pos = m_BitmapResourceList.GetHeadPosition();
	int i = 0;
	while( Pos )
	{
		plbAvailableBmps->InsertString( i, m_BitmapResourceList.GetAt( Pos ).m_strBmpName );
		m_BitmapResourceList.GetNext( Pos );
		i++;
	}

	m_btnCurrentBmp.Create( this, IDC_BMPBUTTON_CURRENT );
	m_btnPreviewBmp.Create( this, IDC_BMPBUTTON_PREVIEW );
	
	if( m_CurBmpResItem.m_ResID )
		m_btnCurrentBmp.SetBitmap( m_CurBmpResItem.m_ResID );
		
	//The first available bitmap resource in the list will be the default.
	if( plbAvailableBmps->GetCount() )
	{
		plbAvailableBmps->SetCurSel( 0 );
		m_btnPreviewBmp.SetBitmap( m_BitmapResourceList.GetAt( m_BitmapResourceList.GetHeadPosition() ).m_ResID );
	}		
}

void CBitmapSymbolPropBox::OnSelchangeListAvailablebmps() 
{
	// TODO: Add your control notification handler code here
	CString BmpName;
	CListBox *plbAvailableBmps = (CListBox*)GetDlgItem( IDC_LIST_AVAILABLEBMPS );
	CILBmpResItem ResItem;

	plbAvailableBmps->GetText( plbAvailableBmps->GetCurSel(), BmpName );
	ILPOSITION Pos = m_BitmapResourceList.GetHeadPosition();
	while( Pos )
	{
		if( BmpName == m_BitmapResourceList.GetAt( Pos ).m_strBmpName )
		{
			ResItem = m_BitmapResourceList.GetAt( Pos );
			m_btnPreviewBmp.SetBitmap( ResItem.m_ResID );
			m_btnPreviewBmp.RedrawWindow();
			break;
		}

		m_BitmapResourceList.GetNext( Pos );
	}
}

void CBitmapSymbolPropBox::OnDblclkListAvailablebmps() 
{
	// TODO: Add your control notification handler code here
	CString BmpName;
	CListBox *plbAvailableBmps = (CListBox*)GetDlgItem( IDC_LIST_AVAILABLEBMPS );
	
	plbAvailableBmps->GetText( plbAvailableBmps->GetCurSel(), BmpName );
	ILPOSITION Pos = m_BitmapResourceList.GetHeadPosition();
	while( Pos )
	{
		if( BmpName == m_BitmapResourceList.GetAt( Pos ).m_strBmpName )
		{
			m_CurBmpResItem = m_BitmapResourceList.GetAt( Pos );
			m_btnCurrentBmp.SetBitmap( m_CurBmpResItem.m_ResID );
			m_btnCurrentBmp.RedrawWindow();
			break;
		}

		m_BitmapResourceList.GetNext( Pos );
	}	
}

void CBitmapSymbolPropBox::OnButtonRestoreDefault() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbAvailableBmps = (CListBox*)GetDlgItem( IDC_LIST_AVAILABLEBMPS );
	
	m_CurBmpResItem  = m_DefBmpResItem;
	m_bAllowStreching = m_bDefAllowStreching;
	
	m_btnCurrentBmp.SetBitmap( m_CurBmpResItem.m_ResID );
	m_btnCurrentBmp.RedrawWindow();
	
	m_btnPreviewBmp.SetBitmap( m_CurBmpResItem.m_ResID );
	m_btnPreviewBmp.RedrawWindow();

	plbAvailableBmps->SelectString( -1, m_CurBmpResItem.m_strBmpName );
	
	m_bMakeCurrent = TRUE;

	UpdateData( FALSE );
}
