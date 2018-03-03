// NewNoteDlg.cpp : implementation file
//
#include "stdafx.h"
#include "illist.h"
#include "resource.h"
#include "NewNoteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDUSER_DOCLIST 50300

/////////////////////////////////////////////////////////////////////////////
// CNewNoteDlg dialog


CNewNoteDlg::CNewNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewNoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewNoteDlg)
	//}}AFX_DATA_INIT

	m_strRootFolder = _T("");
}


void CNewNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewNoteDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewNoteDlg, CDialog)
	//{{AFX_MSG_MAP(CNewNoteDlg)
	ON_CBN_CLOSEUP(IDC_COMBO_LOOKIN, OnCloseupComboLookin)
	ON_LBN_SELCHANGE(IDC_LIST_FILES, OnSelchangeListFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewNoteDlg message handlers

BOOL CNewNoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	Initialize();
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewNoteDlg::RefreshFileBox()
{
	CComboBox *pcmbLookIn = (CComboBox*)GetDlgItem( IDC_COMBO_LOOKIN );
	CListBox *plbFiles = (CListBox*)GetDlgItem( IDC_LIST_FILES );
	CString strCurSel;
	CString	strEnumFolder;
	int nCmbSelIndex;

		
	plbFiles->ResetContent();
	nCmbSelIndex = pcmbLookIn->GetCurSel();
	pcmbLookIn->GetLBText( nCmbSelIndex, strCurSel );

	//"All Folders" refers to all files in "My Documents" and the files in it's
	//immediate directories.
	if( strCurSel == TEXT( "All Folders" ) )
	{
		m_strCurrentFolder = m_strRootFolder;
		EnumFiles( _T(""), nCmbSelIndex ); 
		
		//since we are showing "all files" we need to enumerate all immediate
		//subdirectories.
		for( int i = 0; i < pcmbLookIn->GetCount(); i++ )
		{
			pcmbLookIn->GetLBText( i, strEnumFolder );
			
			//skip "my documents" alias item, we just enumerated this folder.
			//we want to enumerate it's immediate subdirectories.
			if( strEnumFolder == TEXT( "All Folders" ) )
				continue;

			EnumFiles( strEnumFolder, i );
		}
	}

	else
	{
		EnumFiles( strCurSel, nCmbSelIndex );
		m_strCurrentFolder = strCurSel;
	}
}

void CNewNoteDlg::EnumFiles( CString strSubFolder, int nComboIndex )
{
	CListBox *plbFiles = (CListBox*)GetDlgItem( IDC_LIST_FILES );
	CString strSearchPath;
	HANDLE hFileSearch;
	WIN32_FIND_DATA FindData;
	int index;

	//if the caller wants to search the root folder, we simply exclude the sub-folder
	//from our search path.
	if( strSubFolder == _T("") )
		strSearchPath = _T("\\") + m_strRootFolder + _T("\\") + _T("*.pwi");
	else
		strSearchPath = _T("\\") + m_strRootFolder + _T("\\") + strSubFolder +
						_T("\\") + _T("*.pwi");
	
	hFileSearch = FindFirstFile( strSearchPath, &FindData );
	if( hFileSearch != INVALID_HANDLE_VALUE )
	{
		index = plbFiles->AddString( FindData.cFileName );
		plbFiles->SetItemData( index, nComboIndex );
			
		while( FindNextFile( hFileSearch, &FindData ) )
		{
			index = plbFiles->AddString( FindData.cFileName );
			plbFiles->SetItemData( index, nComboIndex );
		}

		FindClose( hFileSearch );
	}
}

void CNewNoteDlg::Initialize()
{
	WIN32_FIND_DATA FindData;
	CComboBox *pcmbLookIn = (CComboBox*)GetDlgItem( IDC_COMBO_LOOKIN );
	CString strSearchPath;
	HANDLE hFileSearch;
	CILList<CString,CString&> strlstSubDirectories, strlstFullPaths;

	m_strRootFolder = _T("My Documents");

	strSearchPath = _T("\\") + m_strRootFolder + _T("\\") + _T("*.*");
	hFileSearch = FindFirstFile( strSearchPath, &FindData );
	if( hFileSearch != INVALID_HANDLE_VALUE )
	{
		if( FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
			pcmbLookIn->AddString( FindData.cFileName );
		
		while( FindNextFile( hFileSearch, &FindData ) )
		{
			if( FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
				pcmbLookIn->AddString( FindData.cFileName );
		}

		FindClose( hFileSearch );
	}

	pcmbLookIn->AddString( TEXT("All Folders") );
	pcmbLookIn->SelectString( -1, TEXT("All Folders") );
	//m_strCurrentFolder = m_strRootFolder;

	RefreshFileBox();
}

void CNewNoteDlg::OnCloseupComboLookin() 
{
	// TODO: Add your control notification handler code here
	RefreshFileBox();
}

void CNewNoteDlg::OnSelchangeListFiles() 
{
	// TODO: Add your control notification handler code here
	CListBox *plbFiles = (CListBox*)GetDlgItem( IDC_LIST_FILES );
	CComboBox *pcmbLookIn = (CComboBox*)GetDlgItem( IDC_COMBO_LOOKIN );
	CString strCurSel;
	CString strFileDir;

	plbFiles->GetText( plbFiles->GetCurSel(), strCurSel );
	pcmbLookIn->GetLBText( plbFiles->GetItemData( plbFiles->GetCurSel() ), strFileDir );

	//"All folders" in this case in the represents the root folder, "My Documents".
	if( strFileDir == TEXT("All Folders") )
		m_strSelectedPath = _T("\\") + m_strRootFolder + _T("\\") + strCurSel;

	else //else we need to supply the sub folder in the "My Documents" directory.
		m_strSelectedPath = _T("\\") + m_strRootFolder + _T("\\") + strFileDir + _T("\\") + strCurSel;

	OnOK();
}
