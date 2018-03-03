#if !defined(AFX_SYMBOLPROPERTYDLG_H__D1870932_FE89_46A2_9F8A_5DFA704038B6__INCLUDED_)
#define AFX_SYMBOLPROPERTYDLG_H__D1870932_FE89_46A2_9F8A_5DFA704038B6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SymbolPropertyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShapePropBox dialog

#include "SymbolProperty2.h"
#include "ColorButton.h"

class CShapePropBox : public CDialog
{
// Construction
public:
	CShapePropBox(CWnd* pParent = NULL );   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CShapePropBox)
	enum { IDD = IDD_SYMBOLPROPERTY };
	UINT	m_uEdgeWidth;
	UINT	m_uSymbolHeight;
	UINT	m_uSymbolWidth;
	BOOL	m_bMakeCurrent;
	int		m_nDashedEdge;
	//}}AFX_DATA

	DWORD	m_dwEdgeClr;
	DWORD	m_dwBkgndClr;
	DWORD	m_dwCaptionClr;
	DWORD	m_dwSecondaryClr;
	UINT	m_uPenStyle;
	
	CSymbolProperty2 m_ShapePropBox2; //page 2.

	DWORD m_dwDefEdgeClr;
	DWORD m_dwDefBkgndClr;
	DWORD m_dwDefSecondaryClr;
	DWORD m_dwDefCaptionClr;
	UINT  m_uDefPenStyle;

	CColorButton m_btnEdgeClr;
	CColorButton m_btnBkgndClr;
	CColorButton m_btnCaptionClr;
	CColorButton m_btnSecondaryClr;

	CDialog *m_pExtdDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapePropBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CShapePropBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMore();
	afx_msg void OnButtonRestoreDefault();
	afx_msg void OnClrbuttonEdge();
	afx_msg void OnClrbuttonCaption();
	afx_msg void OnClrbuttonBkgnd();
	afx_msg void OnClrbuttonSecondary();
	afx_msg void OnButtonExtddlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLPROPERTYDLG_H__D1870932_FE89_46A2_9F8A_5DFA704038B6__INCLUDED_)
