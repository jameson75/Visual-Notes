// SymbolProperty2.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SymbolProperty2.h"
#include "stdil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Visual Notes.h"

extern CVisualNotesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSymbolProperty2 dialog


CSymbolProperty2::CSymbolProperty2(CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolProperty2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolProperty2)
	m_strDiagramName = _T("");
	m_strNoteFile = _T("");
	m_strCaption = _T("");
	//}}AFX_DATA_INIT

	m_nDiagramID = 0;
}


void CSymbolProperty2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolProperty2)
	DDX_Text(pDX, IDC_EDIT_EMBEDDEDLINK, m_strDiagramName);
	DDX_Text(pDX, IDC_EDIT_NOTELINK, m_strNoteFile);
	DDX_Text(pDX, IDC_EDIT_CAPTION, m_strCaption);
	DDV_MaxChars(pDX, m_strCaption, 75);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolProperty2, CDialog)
	//{{AFX_MSG_MAP(CSymbolProperty2)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolProperty2 message handlers

BOOL CSymbolProperty2::OnInitDialog() 
{
	CDialog::OnInitDialog();
			
	// TODO: Add extra initialization here
	CILDBSet *pSet = NULL;
	CEdit *peditCaption = (CEdit*)GetDlgItem( IDC_EDIT_CAPTION );

	if( m_nDiagramID != ILDBSET_ID_NULL )
	{
		pSet = theApp.FindDBSet( m_nDiagramID );
		ASSERT( pSet );
		pSet->GetName( m_strDiagramName );
		UpdateData( FALSE );
	}

	SHSipPreference( m_hWnd, SIP_UP );
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolProperty2::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SHSipPreference( m_hWnd, SIP_DOWN );
	
}
