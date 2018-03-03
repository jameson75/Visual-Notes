#if !defined(AFX_ABOUTDLG1_H__4F1C100E_5380_4429_943B_303558843155__INCLUDED_)
#define AFX_ABOUTDLG1_H__4F1C100E_5380_4429_943B_303558843155__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AboutDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg1 dialog

class CAboutDlg1 : public CDialog
{
// Construction
public:
	CAboutDlg1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDlg1)
	enum { IDD = IDD_ABOUTBOX1 };
	CString	m_strAuthCode;
	CString	m_strUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG1_H__4F1C100E_5380_4429_943B_303558843155__INCLUDED_)
