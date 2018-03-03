// AboutDlg1.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "AboutDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg1 dialog


CAboutDlg1::CAboutDlg1(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg1)
	m_strAuthCode = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
}


void CAboutDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg1)
	DDX_Text(pDX, IDC_EDIT_AUTHCODE, m_strAuthCode);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg1, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg1 message handlers

BOOL CAboutDlg1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow();
	
	if( m_strAuthCode == _T("") )
	{
		m_strAuthCode = _T("Trial Version");
		m_strUserName = _T("Trial User");
	}

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
