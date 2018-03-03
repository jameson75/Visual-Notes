#if !defined(AFX_ILCOLORPICKER_H__D7FD95E0_5F4E_4BBA_BB06_8565AE4AFDD4__INCLUDED_)
#define AFX_ILCOLORPICKER_H__D7FD95E0_5F4E_4BBA_BB06_8565AE4AFDD4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ILColorPicker.h : header file
//

#define NCOLORBTNS 49
/////////////////////////////////////////////////////////////////////////////
// CILColorPicker dialog

class CILColorPicker : public CDialog
{
// Construction
public:
	CILColorPicker(CWnd* pParent = NULL);   // standard constructor
	DWORD		 m_dwResultColor;
	
// Dialog Data
	//{{AFX_DATA(CILColorPicker)
	enum { IDD = IDD_COLORPICKER };
	UINT	m_uGreen;
	UINT	m_uBlue;
	UINT	m_uRed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CILColorPicker)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CColorButton m_ClrBtn[NCOLORBTNS];
	CColorButton m_CurrentClrBtn;

	void OnChooseColor();
	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CILColorPicker)
	afx_msg void OnButtonClrpick();
	virtual BOOL OnInitDialog();
	afx_msg void OnClrbutton01();
	afx_msg void OnClrbutton02();
	afx_msg void OnClrbutton03();
	afx_msg void OnClrbutton04();
	afx_msg void OnClrbutton05();
	afx_msg void OnClrbutton06();
	afx_msg void OnClrbutton07();
	afx_msg void OnClrbutton08();
	afx_msg void OnClrbutton09();
	afx_msg void OnClrbutton10();
	afx_msg void OnClrbutton11();
	afx_msg void OnClrbutton12();
	afx_msg void OnClrbutton13();
	afx_msg void OnClrbutton14();
	afx_msg void OnClrbutton15();
	afx_msg void OnClrbutton16();
	afx_msg void OnClrbutton17();
	afx_msg void OnClrbutton18();
	afx_msg void OnClrbutton19();
	afx_msg void OnClrbutton20();
	afx_msg void OnClrbutton21();
	afx_msg void OnClrbutton22();
	afx_msg void OnClrbutton23();
	afx_msg void OnClrbutton24();
	afx_msg void OnClrbutton25();
	afx_msg void OnClrbutton26();
	afx_msg void OnClrbutton27();
	afx_msg void OnClrbutton28();
	afx_msg void OnClrbutton29();
	afx_msg void OnClrbutton30();
	afx_msg void OnClrbutton31();
	afx_msg void OnClrbutton32();
	afx_msg void OnClrbutton33();
	afx_msg void OnClrbutton34();
	afx_msg void OnClrbutton35();
	afx_msg void OnClrbutton36();
	afx_msg void OnClrbutton37();
	afx_msg void OnClrbutton38();
	afx_msg void OnClrbutton39();
	afx_msg void OnClrbutton40();
	afx_msg void OnClrbutton41();
	afx_msg void OnClrbutton42();
	afx_msg void OnClrbutton43();
	afx_msg void OnClrbutton44();
	afx_msg void OnClrbutton45();
	afx_msg void OnClrbutton46();
	afx_msg void OnClrbutton47();
	afx_msg void OnClrbutton48();
	afx_msg void OnClrbutton49();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ILCOLORPICKER_H__D7FD95E0_5F4E_4BBA_BB06_8565AE4AFDD4__INCLUDED_)
