#include "stdafx.h"
#include "stdil.h"


IMPLEMENT_SERIAL( CILUmlNote, CILShape, VERSIONABLE_SCHEMA | 1 );

CSize CILUmlNote::ms_FoldSize = CSize(10,10);

CILUmlNote::CILUmlNote()
{	m_bCenterCaption = FALSE; }

CILUmlNote::CILUmlNote( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{	m_bCenterCaption = FALSE; }

void CILUmlNote::DrawSymbol( CDC *pDC )
{
	CILShape::DrawSymbol( pDC );

	CBrush BGBrush;
	CPen EdgePen;
	int nSavedDC = pDC->SaveDC();
	CRect NoteRect; 
	POINT NotePoints[5];
	POINT FoldPoints[3];
	
	BGBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );
	pDC->SelectObject( &BGBrush );
	pDC->SelectObject( &EdgePen );

	NoteRect = m_BoundingRect;
	NoteRect.OffsetRect( m_CanvasOrigin );
	NotePoints[0] = NoteRect.TopLeft();
	NotePoints[1] = CPoint( NoteRect.left, NoteRect.bottom - 1 );
	NotePoints[2] = CPoint( NoteRect.right - 1, NoteRect.bottom - 1 );
	NotePoints[3] = CPoint( NoteRect.right - 1, NoteRect.top + ms_FoldSize.cy );
	NotePoints[4] = CPoint( NoteRect.right - ms_FoldSize.cx - 1, NotePoints[0].y );
	pDC->Polygon( NotePoints, 5 );
		
	FoldPoints[0] = NotePoints[4];
	FoldPoints[1] = CPoint( NotePoints[4].x, NotePoints[3].y );
	FoldPoints[2] = NotePoints[3];
	pDC->Polygon( FoldPoints, 3 );

	_RenderCaption( pDC );

	BGBrush.DeleteObject();
	EdgePen.DeleteObject();
	pDC->RestoreDC( nSavedDC );
}

void CILUmlNote::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILUmlNote; } 

	
