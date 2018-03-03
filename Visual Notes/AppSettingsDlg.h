#if !defined(AFX_APPSETTINGSDLG_H__C0FAD525_9B31_46C5_9461_83402F1EFEC0__INCLUDED_)
#define AFX_APPSETTINGSDLG_H__C0FAD525_9B31_46C5_9461_83402F1EFEC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AppSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAppSettingsDlg dialog

class CAppSettingsDlg : public CDialog
{
// Construction
public:
	CAppSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAppSettingsDlg)
	enum { IDD = IDD_APPSETTINGS };
	BOOL	m_bAllowStrechingBMP;
	BOOL	m_bUnlinkOnCD;
	BOOL	m_bRemoveChildren;
	UINT	m_CanvasX;
	UINT	m_CanvasY;
	BOOL	m_bSaveSettings;
	BOOL	m_bAutoRemoveLink;
	CString	m_strTreeDirSel;
	BOOL	m_bDirectEdgeAA;
	//}}AFX_DATA

	CILList<CString,CString&> m_strlstTreeDir;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAppSettingsDlg)
	afx_msg void OnButtonLoadDefault();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPSETTINGSDLG_H__C0FAD525_9B31_46C5_9461_83402F1EFEC0__INCLUDED_)
