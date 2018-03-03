#if !defined(AFX_SYMBOLTEXTDLG_H__C7E0F3D4_1928_4FAD_B358_C9F2FF2C6D0D__INCLUDED_)
#define AFX_SYMBOLTEXTDLG_H__C7E0F3D4_1928_4FAD_B358_C9F2FF2C6D0D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SymbolTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSymbolTextDlg dialog

class CSymbolTextDlg : public CDialog
{
// Construction
public:
	CSymbolTextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSymbolTextDlg)
	enum { IDD = IDD_SYMBOLTEXT };
	CString	m_SymbolText;
	CString	m_strFontStyle;
	CString	m_strFontSize;
	BOOL	m_bBold;
	BOOL	m_bCentered;
	BOOL	m_bItalic;
	BOOL	m_bUnderline;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	static int CALLBACK EnumFontFamProc(  const LOGFONT FAR *lpelf,  const TEXTMETRIC FAR *lpntm, 
										  unsigned long FontType,  LPARAM lParam ); 
	
public:
	DWORD m_dwFontColor;
	int   m_MinFontSize;
	int	  m_MaxFontSize;
	CColorButton m_ColorBtn;

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CSymbolTextDlg)
	afx_msg void OnButtonFontcolor();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLTEXTDLG_H__C7E0F3D4_1928_4FAD_B358_C9F2FF2C6D0D__INCLUDED_)
