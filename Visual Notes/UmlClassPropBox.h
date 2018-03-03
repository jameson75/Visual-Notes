#if !defined(AFX_UMLCLASSPROPBOX_H__7E4F3077_08EC_4792_8231_F3DAA751E1DF__INCLUDED_)
#define AFX_UMLCLASSPROPBOX_H__7E4F3077_08EC_4792_8231_F3DAA751E1DF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UmlClassPropBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUmlClassPropBox dialog

class CUmlClassPropBox : public CDialog
{
// Construction
public:
	CUmlClassPropBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUmlClassPropBox)
	enum { IDD = IDD_UMLCLASSPROPERTY };
	CString	m_strClassTitle;
	CString	m_strSelMethod;
	//}}AFX_DATA


	CILList<CString,CString&> m_strlstMethods;
	UINT m_retID;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUmlClassPropBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CUmlClassPropBox)
	afx_msg void OnButtonAddNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonDeleteAll();
	afx_msg void OnButtonRename();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListMethods();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLCLASSPROPBOX_H__7E4F3077_08EC_4792_8231_F3DAA751E1DF__INCLUDED_)
