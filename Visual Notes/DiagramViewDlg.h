#if !defined(AFX_DIAGRAMVIEWDLG_H__A44B2570_37ED_4320_8D41_9895617E6DDE__INCLUDED_)
#define AFX_DIAGRAMVIEWDLG_H__A44B2570_37ED_4320_8D41_9895617E6DDE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DiagramViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiagramViewDlg dialog

class CDiagramViewDlg : public CDialog
{
// Construction
public:
	CDiagramViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiagramViewDlg)
	enum { IDD = IDD_DIAGRAMVIEW };
	CString	m_strCurrentDiagram;
	CString	m_strSelectedDiagram;
	//}}AFX_DATA

	CILList<CString,CString&> m_strlistDiagrams;
	CILList<UINT,UINT> m_idlistDiagrams;
	UINT m_nSelectedID;
	enum DVMODE { modeAttach, modeNormal } m_DVMode;
	BOOL m_bVisitSelected;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiagramViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	


// Implementation
protected:

	//called to disable appropiate dialog windows on the event that there are no itmes in the list box.
	void _NoItemDisableWindows();
	//called to disable/enable appropiate windows depending whether the root diagram is the current selection.
	void _RootSelDisableWindows( BOOL bDisable = TRUE );	
	//called to disable appropiate dialog windows when dialog is in attach mode.
	void _AttachModeDisableWindows();

	// Generated message map functions
	//{{AFX_MSG(CDiagramViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditSelectedDiagram();
	afx_msg void OnSelchangeListDiagrams();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonVisit();
	afx_msg void OnButtonRename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGRAMVIEWDLG_H__A44B2570_37ED_4320_8D41_9895617E6DDE__INCLUDED_)
