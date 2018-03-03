// SymbolTextDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ColorButton.h"
#include "ILColorPicker.h"
#include "SymbolTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolTextDlg dialog


CSymbolTextDlg::CSymbolTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolTextDlg)
	m_SymbolText = _T("");
	m_strFontStyle = _T("");
	m_strFontSize = _T("");
	m_bBold = FALSE;
	m_bCentered = FALSE;
	m_bItalic = FALSE;
	m_bUnderline = FALSE;
	//}}AFX_DATA_INIT

	m_MinFontSize = m_MaxFontSize = m_dwFontColor = 0;
}


void CSymbolTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolTextDlg)
	DDX_Text(pDX, IDC_EDIT_SYMBOLTEXT, m_SymbolText);
	DDV_MaxChars(pDX, m_SymbolText, 100);
	DDX_CBString(pDX, IDC_COMBO_FONTSTYLE, m_strFontStyle);
	DDX_CBString(pDX, IDC_COMBO_FONTSIZE, m_strFontSize);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	DDX_Check(pDX, IDC_CHECK_CENTERED, m_bCentered);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bItalic);
	DDX_Check(pDX, IDC_CHECK_UNDERLINE, m_bUnderline);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolTextDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolTextDlg)
	ON_BN_CLICKED(IDC_BUTTON_FONTCOLOR, OnButtonFontcolor)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolTextDlg message handlers

void CSymbolTextDlg::OnButtonFontcolor() 
{
	// TODO: Add your control notification handler code here
	CILColorPicker ColorPicker;

	ColorPicker.m_dwResultColor = m_dwFontColor;

	if( IDOK == ColorPicker.DoModal() )
	{
		m_dwFontColor = ColorPicker.m_dwResultColor;
		m_ColorBtn.m_dwButtonColor = m_dwFontColor;
		m_ColorBtn.RedrawWindow();
	}
}

BOOL CSymbolTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CComboBox *pcmbFontSize = NULL, *pcmbFontStyle;
	CString strFontSize, strFontStyle;
	CStatic *pstcLabelProp = (CStatic*)GetDlgItem( IDC_STATIC_LABELPROPERTIES );
	
	// TODO: Add extra initialization here
	m_ColorBtn.Create( this, IDC_BUTTON_FONTCOLOR );
	
	m_ColorBtn.m_dwButtonColor = m_dwFontColor;

	pcmbFontSize = (CComboBox*)GetDlgItem( IDC_COMBO_FONTSIZE );
	pcmbFontStyle = (CComboBox*)GetDlgItem( IDC_COMBO_FONTSTYLE );

	pstcLabelProp->SetWindowPos( &wndBottom, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE );
	
	for( int i = m_MinFontSize; i <= m_MaxFontSize; i++ )
	{
		strFontSize.Format( TEXT("%d"), i );
		pcmbFontSize->InsertString( i - m_MinFontSize, strFontSize );
	}

	EnumFontFamilies( GetParent()->GetDC()->m_hDC, NULL, &CSymbolTextDlg::EnumFontFamProc, (LPARAM)pcmbFontStyle );

	UpdateData( FALSE );

	SHSipPreference( m_hWnd, SIP_UP );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CALLBACK CSymbolTextDlg::EnumFontFamProc(  const LOGFONT FAR *lpelf, const TEXTMETRIC FAR *lpntm,
											   unsigned long FontType, LPARAM lParam )
{
	CComboBox *pcmbFontStyle = (CComboBox*)lParam;
	pcmbFontStyle->InsertString( pcmbFontStyle->GetCount(), lpelf->lfFaceName );

	//continue enumerating all font types.
	return TRUE;
}



void CSymbolTextDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SHSipPreference( m_hWnd, SIP_DOWN );
	
}
