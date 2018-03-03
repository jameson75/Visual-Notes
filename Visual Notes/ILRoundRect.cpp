#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILRoundRect, CILShape, VERSIONABLE_SCHEMA | 1 );

CILRoundRect::CILRoundRect() : CILShape()
{}

CILRoundRect::CILRoundRect( UINT l, UINT t, UINT r, UINT b ) : CILShape( l,t,r,b )
{}

void CILRoundRect::DrawSymbol( CDC *pDC )
{
	int nSavedDC = pDC->SaveDC();
	CBrush BkgndBrush;
	CPen EdgePen;
	CRect rect = m_BoundingRect;

	CILShape::DrawSymbol( pDC );

	rect = m_BoundingRect;
	rect.OffsetRect( m_CanvasOrigin );

	BkgndBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( m_uPenStyle, 1, m_dwEdgeColor );

	pDC->SelectObject( &BkgndBrush );
	pDC->SelectObject( &EdgePen );

	pDC->RoundRect( &rect, CPoint(10,10) );

	_RenderCaption( pDC );

	pDC->RestoreDC( nSavedDC );
	BkgndBrush.DeleteObject();
	EdgePen.DeleteObject();
}

void CILRoundRect::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILRoundRect; }