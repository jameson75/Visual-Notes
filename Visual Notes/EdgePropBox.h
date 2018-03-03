#if !defined(AFX_EDGEPROPBOX_H__9239CAAC_A18E_4CF9_932E_2BCD29881C34__INCLUDED_)
#define AFX_EDGEPROPBOX_H__9239CAAC_A18E_4CF9_932E_2BCD29881C34__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EdgePropBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdgePropBox dialog

#include "ColorButton.h"

class CEdgePropBox : public CDialog
{
// Construction
public:
	CEdgePropBox(CWnd* pParent = NULL);   // standard constructor

	CColorButton m_btnJointClr;
	CColorButton m_btnEdgeClr;
	CILList<CString,CString&> m_strlstSymbols;
	CILList<int,int> m_lstEdgeWidths;
	int m_nCurEdgeWidth;

	DWORD	m_dwDefJointBkgndClr;
	DWORD	m_dwDefJointEdgeClr;
	CString m_strDefMasterSymbol;
	CString m_strDefSlaveSymbol;
	int		m_nDefEdgeWidth;
	UINT	m_uDefEdgeStyle;


// Dialog Data
	//{{AFX_DATA(CEdgePropBox)
	enum { IDD = IDD_EDGEPROPERTY };
	BOOL	m_bMakeCurrent;
	CString	m_strSelMasterSymbol;
	CString	m_strSelSlaveSymbol;
	int		m_nEdgeType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdgePropBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEdgePropBox)
	afx_msg void OnButtonRestoreDefault();
	afx_msg void OnButtonJointColor();
	afx_msg void OnButtonEdgeColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseupComboEdgeWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDGEPROPBOX_H__9239CAAC_A18E_4CF9_932E_2BCD29881C34__INCLUDED_)
