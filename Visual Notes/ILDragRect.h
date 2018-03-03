

class CILDragRect : public CRect
{
protected:
	BOOL m_bEnabled;
	BOOL m_bUseHandles;
	CSize m_sizeHandle;

public:
	CPoint m_DeviceOrigin;

public:
	
	CILDragRect();
	CILDragRect( UINT l, UINT t, UINT r, UINT b );
	CILDragRect( CPoint ptTopLeft, CPoint ptBottomRight );
	CILDragRect( CRect& rRect );
	CILDragRect( CILDragRect& rRect );

	void Anchor( CPoint AnchorPt );
	void Drag( CPoint DragPt );
	void Draw( CDC *pDC, int PenStyle, DWORD dwColor );
	void Clear( CWnd *pWnd );

	inline void SetHandleSize( CSize size )
	{ m_sizeHandle = size; }
	
	inline void UseHandles( BOOL bUseHandles = TRUE)
	{ m_bUseHandles = bUseHandles; }
	
	inline void Enable( BOOL bEnable )
	{ m_bEnabled = bEnable; }

	inline BOOL IsEnabled()
	{ return m_bEnabled; }
	
	CILDragRect& operator = ( CILDragRect& rRect );
	CILDragRect& operator = ( CRect &rRect );
};