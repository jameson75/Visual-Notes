//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/20/01
//*********************************************

class CILSymbolRectTracker : public CRectTracker
{
public:
	CPoint m_DeviceOrigin;

protected:
	CRect *m_pAssociatedRect;
	UINT   m_UniformRectPadding;
	
public:
	CILSymbolRectTracker();
	CILSymbolRectTracker( CRect *pRect );
	
	//logically connects rect trakcer object to symbol object.
	void AssociateRect( CRect *pRect );
	//A CRectTracker override.
	void OnChangedRect( const CRect& rOldRect );
	//Gets address of the symbol object associated w/ this rect tracker object.
	CRect* GetAssociatedRect();
	//logically disconnects rect-tracker object to symbol object.
	void UnassociateRect();
	//Renders tracker GUI about the associated symbol
	void DrawTracker( CDC *pDC );
	//Tracks rect across device with respect to specified device origin.
	void Track( CWnd *pWnd, CPoint WindowPoint );
	//Erases the tracker rect from the specified window.
	void ClearTracker( CWnd *pWnd );

	CILSymbolRectTracker& operator = ( CILSymbolRectTracker &rTracker );

protected:
};
