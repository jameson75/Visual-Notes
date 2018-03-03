//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/23/01
//*********************************************

class CILShape : public CILSymbol
{
protected:
	static UINT					m_uDefEdgeWidth;
	static DWORD				m_dwDefEdgeColor;
	static DWORD				m_dwDefBGColor;
	static DWORD				m_dwDefCaptionColor;
	static DWORD			    m_dwDefSecondaryColor;
	static UINT					m_uDefPenStyle;
	
	CILSymbolRectTracker		m_Tracker;
	CILDragRect					m_SelectRect;
						
public:
	
	UINT	m_uEdgeWidth;
	DWORD	m_dwEdgeColor;
	DWORD	m_dwBGColor;
	DWORD	m_dwSecondaryColor;
	DWORD	m_dwCaptionColor;
	UINT    m_SelectPadding;
	UINT	m_uPenStyle;

public:
	CILShape();
	CILShape(UINT l, UINT t, UINT b, UINT r );
	
	UINT DoModalPropBox( CWnd *pWnd = NULL, CDialog *pExtdDlg = NULL );
	BOOL HitTest( CPoint CanvasPoint );
	BOOL Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList,
				CILList<CILSymbol*,CILSymbol*> *pJointList );
	void ShowTracker( BOOL bShow, CWnd *pWnd = NULL );
	void ShowSelection( BOOL bSelection, CWnd *pWnd );
	void DrawSymbol( CDC *pDC );
	void SetBoundingRect( CRect rect );
	BOOL TrackerHitTest( CPoint CanvasPoint );
	BOOL IsTracked();
	BOOL IsSelected();
	void SetCanvasOrigin( CPoint CanvasOrigin );
	void InvalidateSymbol( CWnd *pWnd );
	void Highlight( BOOL bHighlight, CWnd *pWnd, int PenStyle, DWORD dwColor );
	void Resize( CRect NewRect, CILList<CILSymbol*,CILSymbol*> *pJointList );

	void Serialize( CArchive &ar );
	
protected:
	void _CopyBaseData( CILSymbol **ppSymbol );
	void _RenderCaption( CDC *pDC );

	DECLARE_DYNAMIC( CILShape );
};
	
