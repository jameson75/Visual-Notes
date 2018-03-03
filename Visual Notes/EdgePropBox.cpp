// EdgePropBox.cpp : implementation file
//
#include "stdafx.h"
#include "illist.h"
#include "resource.h"
#include "EdgePropBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ILColorPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CEdgePropBox dialog


CEdgePropBox::CEdgePropBox(CWnd* pParent /*=NULL*/)
	: CDialog(CEdgePropBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdgePropBox)
	m_bMakeCurrent = FALSE;
	m_strSelMasterSymbol = _T("");
	m_strSelSlaveSymbol = _T("");
	m_nEdgeType = -1;
	//}}AFX_DATA_INIT

	m_nCurEdgeWidth = 0;
}


void CEdgePropBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdgePropBox)
	DDX_Check(pDX, IDC_CHECK_MAKECURRENT, m_bMakeCurrent);
	DDX_CBString(pDX, IDC_COMBO_MASTERSYMBOL, m_strSelMasterSymbol);
	DDX_CBString(pDX, IDC_COMBO_SLAVESYMBOL, m_strSelSlaveSymbol);
	DDX_Radio(pDX, IDC_RADIO_SOLID, m_nEdgeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdgePropBox, CDialog)
	//{{AFX_MSG_MAP(CEdgePropBox)
	ON_BN_CLICKED(IDC_BUTTON_RESTOREDEFAULT, OnButtonRestoreDefault)
	ON_BN_CLICKED(IDC_BUTTON_JOINTCOLOR, OnButtonJointColor)
	ON_BN_CLICKED(IDC_BUTTON_EDGECOLOR, OnButtonEdgeColor)
	ON_CBN_CLOSEUP(IDC_COMBO_EDGEWIDTH, OnCloseupComboEdgeWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdgePropBox message handlers

void CEdgePropBox::OnButtonRestoreDefault() 
{
	// TODO: Add your control notification handler code here.
	CComboBox *pcmbMasterSymbol = (CComboBox*)GetDlgItem( IDC_COMBO_MASTERSYMBOL );
	CComboBox *pcmbSlaveSymbol = (CComboBox*)GetDlgItem( IDC_COMBO_SLAVESYMBOL );
	CComboBox *pcmbEdgeWidth = (CComboBox*)GetDlgItem( IDC_COMBO_EDGEWIDTH );
	CString strEdgeWidth;

	m_btnJointClr.m_dwButtonColor = m_dwDefJointBkgndClr;
	m_btnJointClr.RedrawWindow();

	m_btnEdgeClr.m_dwButtonColor = m_dwDefJointEdgeClr;
	m_btnEdgeClr.RedrawWindow();

	pcmbMasterSymbol->SelectString( -1, m_strDefMasterSymbol );
	pcmbSlaveSymbol->SelectString( -1, m_strDefSlaveSymbol );
	UpdateData( TRUE );
	
	strEdgeWidth.Format( TEXT("%ld"), m_nDefEdgeWidth );
	pcmbEdgeWidth->SelectString( -1, strEdgeWidth );
	m_nCurEdgeWidth = m_nDefEdgeWidth;

	m_nEdgeType = ( m_uDefEdgeStyle == PS_SOLID ) ? 0 : 1;
	
	m_bMakeCurrent = FALSE;
	UpdateData( FALSE );
}

void CEdgePropBox::OnButtonJointColor() 
{
	// TODO: Add your control notification handler code here
	CILColorPicker ColorPicker;

	ColorPicker.m_dwResultColor = m_btnJointClr.m_dwButtonColor;
	if( IDOK == ColorPicker.DoModal() )
	{
		m_btnJointClr.m_dwButtonColor = ColorPicker.m_dwResultColor;	
		m_btnJointClr.RedrawWindow();
	}
}

void CEdgePropBox::OnButtonEdgeColor() 
{
	// TODO: Add your control notification handler code here
	CILColorPicker ColorPicker;

	ColorPicker.m_dwResultColor = m_btnEdgeClr.m_dwButtonColor;
	if( IDOK == ColorPicker.DoModal() )
	{
		m_btnEdgeClr.m_dwButtonColor = ColorPicker.m_dwResultColor;	
		m_btnEdgeClr.RedrawWindow();
	}
}

BOOL CEdgePropBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CComboBox *pcmbMasterSymbol = (CComboBox*)GetDlgItem( IDC_COMBO_MASTERSYMBOL );
	CComboBox *pcmbSlaveSymbol = (CComboBox*)GetDlgItem( IDC_COMBO_SLAVESYMBOL );
	CComboBox *pcmbEdgeWidth = (CComboBox*)GetDlgItem( IDC_COMBO_EDGEWIDTH );
	CStatic *pstcGroupBox = (CStatic*)GetDlgItem( IDC_STATIC_JOINTCOLORGROUPBOX );
	CButton *pbtnSolid = (CButton*)GetDlgItem( IDC_RADIO_SOLID );
	CButton *pbtnDash = (CButton*)GetDlgItem( IDC_RADIO_DASH );

	ILPOSITION Pos;
	int nIndexEdgeWidth;
	CString strEdgeWidth, strCurEdgeWidth;

	m_btnJointClr.Create( this, IDC_BUTTON_JOINTCOLOR, m_btnJointClr.m_dwButtonColor );
	m_btnEdgeClr.Create( this, IDC_BUTTON_EDGECOLOR, m_btnEdgeClr.m_dwButtonColor );
	pstcGroupBox->SetWindowPos( &wndBottom, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE );

	//load entire selection of joint symbols.
	Pos = m_strlstSymbols.GetHeadPosition();
	while( Pos )
	{
		pcmbMasterSymbol->AddString( m_strlstSymbols.GetAt( Pos ) );
		pcmbSlaveSymbol->AddString( m_strlstSymbols.GetAt( Pos ) );
		m_strlstSymbols.GetNext( Pos );
	}

	//load entire selection of edge widths.
	Pos = m_lstEdgeWidths.GetHeadPosition();
	while( Pos )
	{
		strEdgeWidth.Format( TEXT("%ld"), m_lstEdgeWidths.GetAt( Pos ) );
		nIndexEdgeWidth = pcmbEdgeWidth->AddString( strEdgeWidth );
		pcmbEdgeWidth->SetItemData( nIndexEdgeWidth, m_lstEdgeWidths.GetAt( Pos ) );

		//save string of current edge width so we can make it the current selection.
		if( m_nCurEdgeWidth == m_lstEdgeWidths.GetAt( Pos ) )
			strCurEdgeWidth = strEdgeWidth;

		m_lstEdgeWidths.GetNext( Pos );
	}

	//now that all the strings are in the edge-width combo box, we can make a current selection.
	pcmbEdgeWidth->SelectString( -1, strCurEdgeWidth );

	//only edges of size 1 can be dash type.
	//--------------------------------------
	if( m_nCurEdgeWidth == 1 )
	{
		pbtnSolid->EnableWindow();
		pbtnDash->EnableWindow();
	}

	else
	{
		pbtnSolid->SetCheck( TRUE );
		pbtnDash->SetCheck( FALSE );
		pbtnSolid->EnableWindow( FALSE );
		pbtnDash->EnableWindow( FALSE );
	}

	UpdateData( TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEdgePropBox::OnCloseupComboEdgeWidth() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pcmbEdgeWidth = (CComboBox*)GetDlgItem( IDC_COMBO_EDGEWIDTH );
	CButton *pbtnSolid = (CButton*)GetDlgItem( IDC_RADIO_SOLID );
	CButton *pbtnDash = (CButton*)GetDlgItem( IDC_RADIO_DASH );

	//we're concerned with the numeric edge with value, not the string value.
	m_nCurEdgeWidth = pcmbEdgeWidth->GetItemData( pcmbEdgeWidth->GetCurSel() );

	if( m_nCurEdgeWidth == 1 )
	{
		pbtnSolid->EnableWindow();
		pbtnDash->EnableWindow();
	}

	else
	{
		pbtnSolid->SetCheck( TRUE );
		pbtnDash->SetCheck( FALSE );
		pbtnSolid->EnableWindow( FALSE );
		pbtnDash->EnableWindow( FALSE );
	}
}


