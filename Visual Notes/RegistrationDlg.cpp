// RegistrationDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "RegistrationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "stdafx.h"
#include "stdil.h"

#include "Visual Notes.h"

extern CVisualNotesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDlg dialog


CRegistrationDlg::CRegistrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegistrationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegistrationDlg)
	m_UserName = _T("");
	m_AuthCode = _T("");
	//}}AFX_DATA_INIT
}


void CRegistrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegistrationDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 30);
	DDX_Text(pDX, IDC_EDIT_KEY, m_AuthCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegistrationDlg, CDialog)
	//{{AFX_MSG_MAP(CRegistrationDlg)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDlg message handlers

BOOL CRegistrationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SHSipPreference( m_hWnd, SIP_UP );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegistrationDlg::OnButtonRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_UserName.GetLength() < 3 )
		MessageBox( TEXT( "User Name/Title must be at least three characters long" ), TEXT( "Registration Error" ),
					MB_ICONERROR );
	
	else if( theApp.VerifyAuthorizationCode( m_AuthCode ) )
	{
		//SHSipPreference( m_hWnd, SIP_DOWN );
		EndDialog( IDREGISTER_SUCCESS );
	}
		

	else
		MessageBox( TEXT("The key you specified is invalid. \nConsult your vendor for technical problems"), 
					TEXT("Registration Error"), MB_ICONERROR );
}

void CRegistrationDlg::OnOK()
{ 
	OnButtonRegister(); 
}

void CRegistrationDlg::OnCancel()
{
	//SHSipPreference( m_hWnd, SIP_DOWN );
	CDialog::OnCancel();
}

void CRegistrationDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SHSipPreference( m_hWnd, SIP_FORCEDOWN );	
}
