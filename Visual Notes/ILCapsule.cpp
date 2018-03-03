#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILCapsule, CILShape, VERSIONABLE_SCHEMA | 1 );

CILCapsule::CILCapsule() : CILShape()
{}

CILCapsule::CILCapsule( UINT l, UINT t, UINT r, UINT b ) : CILShape( l,t,r,b )
{}

void CILCapsule::DrawSymbol( CDC *pDC )
{
	CBrush BkgndBrush;
	CPen EdgePen, ErasePen;
	CRect rectEllipse1, rectEllipse2, rectBody;
	CPoint ptLeftEdge[2], ptRightEdge[2];
	int nSavedDC = pDC->SaveDC();

	CILShape::DrawSymbol( pDC );

	BkgndBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( m_uPenStyle, 1, m_dwEdgeColor );
	ErasePen.CreatePen( PS_SOLID, 1, m_dwBGColor );

	pDC->SelectObject( &BkgndBrush );
	pDC->SelectObject( &EdgePen );

	rectEllipse1 = rectEllipse2 = m_BoundingRect;
	rectEllipse1.right = rectEllipse1.left + 20;
	rectEllipse2.left = rectEllipse2.right - 20;
	rectBody = m_BoundingRect;
	rectBody.left+=10;
	rectBody.right-=10;

	rectEllipse1.OffsetRect( m_CanvasOrigin );
	rectEllipse2.OffsetRect( m_CanvasOrigin );
	rectBody.OffsetRect( m_CanvasOrigin );
	
	ptLeftEdge[0] = CPoint( rectBody.left, rectBody.top + 1 );
	ptLeftEdge[1] = CPoint( rectBody.left, rectBody.bottom - 1 );
	ptRightEdge[0] = CPoint( rectBody.right - 1, rectBody.top + 1);
	ptRightEdge[1] = CPoint( rectBody.right - 1, rectBody.bottom - 1 );

	pDC->Ellipse( &rectEllipse1 );
	pDC->Ellipse( &rectEllipse2 );
	pDC->Rectangle( &rectBody );

	pDC->SelectObject( &ErasePen );
	pDC->Polyline( ptLeftEdge, 2 );
	pDC->Polyline( ptRightEdge, 2 );

	_RenderCaption( pDC );
	
	pDC->RestoreDC( nSavedDC );
	ErasePen.DeleteObject();
	BkgndBrush.DeleteObject();
	EdgePen.DeleteObject();
}

void CILCapsule::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILCapsule; }
