#if !defined(AFX_REGISTRATIONDLG_H__3164AB05_A3A6_478C_BAAC_112C90499111__INCLUDED_)
#define AFX_REGISTRATIONDLG_H__3164AB05_A3A6_478C_BAAC_112C90499111__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RegistrationDlg.h : header file
//

#define IDREGISTER_SUCCESS 50100

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDlg dialog

class CRegistrationDlg : public CDialog
{
// Construction
public:
	CRegistrationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegistrationDlg)
	enum { IDD = IDD_REGISTRATIONDLG };
	CString	m_UserName;
	CString	m_AuthCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegistrationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void OnOK();
	void OnCancel();

//Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegistrationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRegister();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTRATIONDLG_H__3164AB05_A3A6_478C_BAAC_112C90499111__INCLUDED_)
