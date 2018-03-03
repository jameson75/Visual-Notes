#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetCloud, CILShape, VERSIONABLE_SCHEMA | 1 );

CILNetCloud::CILNetCloud()
{}

CILNetCloud::CILNetCloud( UINT l, UINT t, UINT r, UINT b ) : CILShape( l,t,r,b )
{}

void CILNetCloud::DrawSymbol( CDC *pDC )
{

	CBrush BGBrush;
	CPen EdgePen, BGPen;
	int nSavedDC = pDC->SaveDC();
	CRect BubbleRect[6];
	CRect EraseRect;
	
	CILShape::DrawSymbol( pDC );

	BGBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );
	BGPen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwBGColor );
	pDC->SelectObject( &BGBrush );
	pDC->SelectObject( &EdgePen );

	BubbleRect[0] = m_BoundingRect;
	BubbleRect[0].left+= ( m_BoundingRect.Width() / 4 );
	BubbleRect[0].right-= ( m_BoundingRect.Width() / 2 );
	BubbleRect[0].bottom-= ( m_BoundingRect.Height() / 2 );
	BubbleRect[0].right+= 2;
	
	BubbleRect[1].left-= 2;
	BubbleRect[1] = BubbleRect[0];
	BubbleRect[1].OffsetRect( BubbleRect[0].Width(), 0 );
	
	BubbleRect[2] = BubbleRect[0];
	BubbleRect[2].OffsetRect( 0, BubbleRect[0].Height() - 1 );
	
	BubbleRect[3] = BubbleRect[0];
	BubbleRect[3].OffsetRect( BubbleRect[0].Width(), BubbleRect[0].Height() - 1 );
	
	BubbleRect[4] = BubbleRect[0];
	BubbleRect[4].OffsetRect( -BubbleRect[0].Width() / 2, BubbleRect[0].Height() / 2 );
	BubbleRect[5] = BubbleRect[1];
	BubbleRect[5].OffsetRect( BubbleRect[1].Width() / 2, BubbleRect[1].Height() / 2 );

	EraseRect = BubbleRect[4];
	EraseRect.left = BubbleRect[0].left;
	EraseRect.right = BubbleRect[1].right;

	for( int i = 0; i < 6; i++ )
	{
		BubbleRect[i].OffsetRect( m_CanvasOrigin );
		pDC->Ellipse( &BubbleRect[i] );
	}

	pDC->SelectObject( &BGPen );
	EraseRect.OffsetRect( m_CanvasOrigin );
	pDC->Rectangle( &EraseRect );

	_RenderCaption( pDC );

	pDC->RestoreDC( nSavedDC );
	BGBrush.DeleteObject();
	EdgePen.DeleteObject();	
	BGPen.DeleteObject();
}

void CILNetCloud::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetCloud; }
