#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetRing, CILShape, VERSIONABLE_SCHEMA | 1 );

CILNetRing::CILNetRing() : CILShape()
{}

CILNetRing::CILNetRing( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{}

void CILNetRing::DrawSymbol( CDC *pDC )
{
	CILShape::DrawSymbol( pDC );

	CBrush BGBrush;
	CBrush SecondaryBrush;
	CPen EdgePen;
	int nSavedDC = pDC->SaveDC();
	CRect rectRing;
	CPoint ptArrow1[3], ptArrow2[3];

	BGBrush.CreateStockObject( NULL_BRUSH );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );
	SecondaryBrush.CreateSolidBrush( m_dwSecondaryColor );

	pDC->SelectObject( &BGBrush );
	pDC->SelectObject( &EdgePen );

	rectRing = m_BoundingRect;
	rectRing.OffsetRect( m_CanvasOrigin );
	rectRing.DeflateRect( 4, 4 );
	pDC->Ellipse( &rectRing );

	//draw direction arrows, ( exist in a logical 6 x 6 box )
	ptArrow1[0] = CPoint( rectRing.left, rectRing.top + rectRing.Height() / 2 );
	ptArrow1[1] = CPoint( ptArrow1[0].x - 3, ptArrow1[0].y + 6 );
	ptArrow1[2] = CPoint( ptArrow1[0].x + 3, ptArrow1[0].y + 6 );
	
	ptArrow2[0] = CPoint( rectRing.right, rectRing.top + rectRing.Height() / 2 );
	ptArrow2[1] = CPoint( ptArrow2[0].x - 3, ptArrow2[0].y - 6 );
	ptArrow2[2] = CPoint( ptArrow2[0].x + 3, ptArrow2[0].y - 6 );

	pDC->SelectObject( &SecondaryBrush );

	pDC->Polygon( ptArrow1, 3 );
	pDC->Polygon( ptArrow2, 3 );

	pDC->RestoreDC( nSavedDC );
	BGBrush.DeleteObject();
	EdgePen.DeleteObject();
	SecondaryBrush.DeleteObject();
}

void CILNetRing::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetRing; }


	




