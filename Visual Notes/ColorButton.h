#if !defined(AFX_COLORBUTTON_H__C26F1555_0572_4C20_89ED_8BBA2BBA1FB4__INCLUDED_)
#define AFX_COLORBUTTON_H__C26F1555_0572_4C20_89ED_8BBA2BBA1FB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorButton window

class CColorButton : public CButton
{
// Construction
public:
	CColorButton();

// Attributes
public:
	DWORD m_dwButtonColor;
	UINT  m_BitmapResID;
	BOOL  m_bUseBitmap;

	
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(  CWnd *pParent, UINT resID, DWORD dwColor = 0 );
	virtual ~CColorButton();

	void SetBitmap( UINT ResID );

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__C26F1555_0572_4C20_89ED_8BBA2BBA1FB4__INCLUDED_)
