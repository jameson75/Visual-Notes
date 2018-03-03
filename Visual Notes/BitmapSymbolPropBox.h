#if !defined(AFX_BITMAPSYMBOLPROPBOX_H__0B58952E_2ABC_43CC_86B1_707D58011D80__INCLUDED_)
#define AFX_BITMAPSYMBOLPROPBOX_H__0B58952E_2ABC_43CC_86B1_707D58011D80__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BitmapSymbolPropBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapSymbolPropBox dialog

class CBitmapSymbolPropBox : public CDialog
{
// Construction
public:
	CBitmapSymbolPropBox(CWnd* pParent = NULL);   // standard constructor
	void InitBmpProperties();

public:

	CILList<CILBmpResItem,CILBmpResItem&> m_BitmapResourceList;
	CILBmpResItem m_CurBmpResItem;
	
	CILBmpResItem m_DefBmpResItem;
	BOOL		  m_bDefAllowStreching;

	CColorButton m_btnCurrentBmp;
	CColorButton m_btnPreviewBmp;

// Dialog Data
	//{{AFX_DATA(CBitmapSymbolPropBox)
	enum { IDD = IDD_BMSYMBOLPROPERTY };
	BOOL	m_bMakeCurrent;
	BOOL	m_bAllowStreching;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapSymbolPropBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBitmapSymbolPropBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListAvailablebmps();
	afx_msg void OnDblclkListAvailablebmps();
	afx_msg void OnButtonRestoreDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPSYMBOLPROPBOX_H__0B58952E_2ABC_43CC_86B1_707D58011D80__INCLUDED_)
