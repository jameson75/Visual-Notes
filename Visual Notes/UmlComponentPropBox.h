#if !defined(AFX_UMLCOMPONENTPROPBOX_H__EF07F88A_79D2_41A1_A6E1_AF75B62EA348__INCLUDED_)
#define AFX_UMLCOMPONENTPROPBOX_H__EF07F88A_79D2_41A1_A6E1_AF75B62EA348__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UmlComponentPropBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUmlComponentPropBox dialog

class CUmlComponentPropBox : public CDialog
{
// Construction
public:
	CUmlComponentPropBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUmlComponentPropBox)
	enum { IDD = IDD_UMLCOMPONENTPROPERTY };
	CString	m_strSelInterface;
	//}}AFX_DATA

	UINT m_retID;
	CILList<CString,CString&> m_strlstInterfaces;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUmlComponentPropBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CUmlComponentPropBox)
	afx_msg void OnAddNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonDeleteAll();
	afx_msg void OnButtonRename();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListInterfaces();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLCOMPONENTPROPBOX_H__EF07F88A_79D2_41A1_A6E1_AF75B62EA348__INCLUDED_)
