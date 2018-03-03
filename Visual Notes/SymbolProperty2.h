#if !defined(AFX_SYMBOLPROPERTY2_H__95CB9E98_58C8_4880_B09A_501FDA6E6FE6__INCLUDED_)
#define AFX_SYMBOLPROPERTY2_H__95CB9E98_58C8_4880_B09A_501FDA6E6FE6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SymbolProperty2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSymbolProperty2 dialog

class CSymbolProperty2 : public CDialog
{
// Construction
public:
	CSymbolProperty2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSymbolProperty2)
	enum { IDD = IDD_SYMBOLPROPERTY2 };
	CString	m_strDiagramName;
	CString	m_strNoteFile;
	CString	m_strCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolProperty2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	UINT	m_nDiagramID;

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSymbolProperty2)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLPROPERTY2_H__95CB9E98_58C8_4880_B09A_501FDA6E6FE6__INCLUDED_)
