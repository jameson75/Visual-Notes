// Visual NotesView.h : interface of the CVisualNotesView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUALNOTESVIEW_H__0379377D_ACC4_4C69_9570_C8FC1C3FCD35__INCLUDED_)
#define AFX_VISUALNOTESVIEW_H__0379377D_ACC4_4C69_9570_C8FC1C3FCD35__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ILArray.h"

class CILNavigationStack
{
protected:
	CILArray<UINT,UINT>	m_stack;
	LONG				m_nIndex;
	LONG				m_nTop;

public:
	CILNavigationStack();
	~CILNavigationStack();

	UINT PopBack();
	UINT PopFoward();
	void PushNewSite( UINT id );
	UINT Reset();
	UINT UpdateDeletedDiagrams( CVisualNotesDoc *pDoc );
};

class CVisualNotesView : public CScrollView
{
protected: // create from serialization only
	CVisualNotesView();
	DECLARE_DYNCREATE(CVisualNotesView)
// Attributes	

	CILDragRect				m_CreateRect;
	CILDragRect				m_MultiSelectRect;
	CILDragRect				m_MultiPickRect;
	CILSymbol				*m_pLinkChild;
	CWnd					*m_pwndPrevCapture;
	BOOL					m_bDragSelection;
	BOOL					m_bHitSelection;
	CPoint					m_ptLastDown;
	CPoint					m_ptLastDrag;
	BOOL					m_bMultiSelectMode;
	BOOL					m_bMultiPickMode;
	CPoint					m_ptLastMultiDrag;

	//{{POPUP RESOURCES
	CILSymbol*	m_pPRHitSymbol;
	//}}POPUP RESOURCES;

public:
	CILNavigationStack		m_DViewStack;

public:
	CVisualNotesDoc* GetDocument();
	CRect ClientToCanvas( CRect *pRect );
	CPoint ClientToCanvas( CPoint *pRect );
	CRect CanvasToClient( CRect *pRect );
	CPoint CanvasToClient( CPoint *pPoint );
	void OffScreenScroll( CPoint point );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualNotesView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVisualNotesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	
	void OnContextMenu( NMHDR *pNMHDR, LRESULT *pResult );
	
	//{{AFX_MSG(CVisualNotesView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnOpenEmbeddedLink();
	afx_msg void OnCreateEmbeddedLink();
	afx_msg void OnDeleteEmbeddedLink();
	afx_msg void OnDeleteThisDiagram();
	afx_msg void OnOpenNoteLink();
	afx_msg void OnOpenWebLink();
	afx_msg void OnInsertEmbeddedLink();
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );
	afx_msg void OnNavigateFoward();
	afx_msg void OnNavigateBack();
	afx_msg void OnRemoveEmbeddedLink();
	afx_msg void OnCreateNoteLink();
	afx_msg void OnRemoveNoteLink();
	afx_msg void OnToolsExport();
	//}}AFX_MSG
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Visual NotesView.cpp
inline CVisualNotesDoc* CVisualNotesView::GetDocument()
   { return (CVisualNotesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALNOTESVIEW_H__0379377D_ACC4_4C69_9570_C8FC1C3FCD35__INCLUDED_)
