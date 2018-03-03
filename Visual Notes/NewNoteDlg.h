#if !defined(AFX_NEWNOTEDLG_H__D5E0DC28_0C52_46DB_BD1A_651C88402A70__INCLUDED_)
#define AFX_NEWNOTEDLG_H__D5E0DC28_0C52_46DB_BD1A_651C88402A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewNoteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewNoteDlg dialog

class CNewNoteDlg : public CDialog
{
// Construction
public:
	CNewNoteDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_strSelectedPath;
	
// Dialog Data
	//{{AFX_DATA(CNewNoteDlg)
	enum { IDD = IDD_NEWNOTE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewNoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strCurrentFolder;
	CString m_strRootFolder;
		
	void RefreshFileBox();
	void Initialize();
	void EnumFiles( CString strSubFolder, int ComboIndex );

	// Generated message map functions
	//{{AFX_MSG(CNewNoteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseupComboLookin();
	afx_msg void OnSelchangeListFiles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWNOTEDLG_H__D5E0DC28_0C52_46DB_BD1A_651C88402A70__INCLUDED_)

