#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILUmlActor, CILShape, VERSIONABLE_SCHEMA | 1 );

CSize CILUmlActor::ms_FixedSize = CSize( 20, 35 );

CILUmlActor::CILUmlActor()
{}

CILUmlActor::CILUmlActor( UINT x, UINT y ) : CILShape(x, y, x + ms_FixedSize.cx, y + ms_FixedSize.cy )
{}

CILUmlActor::CILUmlActor( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{}

void CILUmlActor::DrawSymbol( CDC *pDC )
{
	CILShape::DrawSymbol( pDC );
	
	int nSavedDC = pDC->SaveDC();
	CBrush BGBrush; 
	CPen EdgePen;
	CRect HeadRect;
	
	BGBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );

	//Draw actors head.
	HeadRect.left = m_BoundingRect.left + ( m_BoundingRect.Width() / 4 );
	HeadRect.top = m_BoundingRect.top;
	HeadRect.right = m_BoundingRect.right - ( m_BoundingRect.Width() / 4 );
	HeadRect.bottom = m_BoundingRect.top + ( m_BoundingRect.Height() / 3 );
	HeadRect.OffsetRect( m_CanvasOrigin );
	pDC->SelectObject( &EdgePen );
	pDC->SelectObject( &BGBrush );
	pDC->Ellipse( &HeadRect );
	
	//Draw arms.
	CPoint ptArm[2];
	CPoint ptBody[2];
	CPoint ptLeftLeg[2];
	CPoint ptRightLeg[2];

	ptArm[0] = CPoint( m_BoundingRect.left, m_BoundingRect.top + m_BoundingRect.Height() / 3 + 3 );
	ptArm[1] = CPoint( m_BoundingRect.right, ptArm[0].y );
	ptBody[0] = CPoint( m_BoundingRect.left + m_BoundingRect.Width() / 2, m_BoundingRect.top + m_BoundingRect.Height() / 3 );
	ptBody[1] = CPoint( ptBody[0].x, m_BoundingRect.bottom - m_BoundingRect.Height() / 3 );
	ptLeftLeg[0] = ptBody[1];
	ptLeftLeg[1] = CPoint( m_BoundingRect.left, m_BoundingRect.bottom );
	ptRightLeg[0] = ptBody[1];
	ptRightLeg[1] = CPoint( m_BoundingRect.right, m_BoundingRect.bottom );

	for( int i = 0; i < 2; i++ )
	{
		ptArm[i].Offset( m_CanvasOrigin );
		ptBody[i].Offset( m_CanvasOrigin );
		ptLeftLeg[i].Offset( m_CanvasOrigin );
		ptRightLeg[i].Offset( m_CanvasOrigin );
	}
		
	pDC->Polyline( ptArm, 2 );
	pDC->Polyline( ptBody, 2 );
	pDC->Polyline( ptLeftLeg, 2 );
	pDC->Polyline( ptRightLeg, 2 );

	BGBrush.DeleteObject();
	EdgePen.DeleteObject();
	
	pDC->RestoreDC( nSavedDC );
}

void CILUmlActor::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILUmlActor(); }