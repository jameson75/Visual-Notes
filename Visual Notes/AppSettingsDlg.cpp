// AppSettingsDlg.cpp : implementation file
//
#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "AppSettingsDlg.h"
#include "Visual Notes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CVisualNotesApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CAppSettingsDlg dialog


CAppSettingsDlg::CAppSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppSettingsDlg)
	m_bAllowStrechingBMP = FALSE;
	m_bUnlinkOnCD = FALSE;
	m_bRemoveChildren = FALSE;
	m_CanvasX = 0;
	m_CanvasY = 0;
	m_bSaveSettings = FALSE;
	m_bAutoRemoveLink = FALSE;
	m_strTreeDirSel = _T("");
	m_bDirectEdgeAA = FALSE;
	//}}AFX_DATA_INIT
}


void CAppSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppSettingsDlg)
	DDX_Check(pDX, IDC_CHECK_ALLOWSTRECHING, m_bAllowStrechingBMP);
	DDX_Check(pDX, IDC_CHECK_UNLINKONCD, m_bUnlinkOnCD);
	DDX_Check(pDX, IDC_CHECK_REMOVECHILDREN, m_bRemoveChildren);
	DDX_Text(pDX, IDC_EDIT_CANVASX, m_CanvasX);
	DDV_MinMaxUInt(pDX, m_CanvasX, 400, 3000);
	DDX_Text(pDX, IDC_EDIT_CANVASY, m_CanvasY);
	DDV_MinMaxUInt(pDX, m_CanvasY, 400, 3000);
	DDX_Check(pDX, IDC_CHECK_SAVESETTINGS, m_bSaveSettings);
	DDX_Check(pDX, IDC_CHECK_AUTOREMOVELINK, m_bAutoRemoveLink);
	DDX_CBString(pDX, IDC_COMBO_TREEDIR, m_strTreeDirSel);
	DDX_Check(pDX, IDC_CHECK_DIRECTEDGEAA, m_bDirectEdgeAA);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAppSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CAppSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOADDEFAULT, OnButtonLoadDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppSettingsDlg message handlers

void CAppSettingsDlg::OnButtonLoadDefault() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	m_bRemoveChildren = theApp.m_bDefRemoveChildren;
	m_bAllowStrechingBMP = theApp.m_bDefAllowStrechingBMP;
	m_bUnlinkOnCD = theApp.m_bDefUnlinkOnCD;
	m_CanvasX = theApp.m_DefCanvasSize.cx;
	m_CanvasY = theApp.m_DefCanvasSize.cy;
	m_bAutoRemoveLink = theApp.m_bDefAutoRemoveLink;
	
	m_bSaveSettings = TRUE;
	UpdateData( FALSE );
}

BOOL CAppSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox *pcmbTreeDir = ( CComboBox* )GetDlgItem( IDC_COMBO_TREEDIR );
	
	ILPOSITION Pos = m_strlstTreeDir.GetHeadPosition();
	while( Pos )
	{
		pcmbTreeDir->AddString( m_strlstTreeDir.GetAt( Pos ) );
		m_strlstTreeDir.GetNext( Pos );
	}

	pcmbTreeDir->SelectString( -1, m_strTreeDirSel );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

