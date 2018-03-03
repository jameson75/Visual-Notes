
#include "stdafx.h"
#include "stdil.h"

CILDragRect::CILDragRect(): CRect(), m_bEnabled(FALSE), m_bUseHandles( FALSE )
{ m_DeviceOrigin = CPoint(0,0); }

CILDragRect::CILDragRect( CPoint ptTopLeft, CPoint ptBottomRight ): CRect( ptTopLeft, ptBottomRight ),
																		m_bEnabled(FALSE), m_bUseHandles( FALSE )
{ m_DeviceOrigin = CPoint(0,0); }

CILDragRect::CILDragRect( UINT l, UINT t, UINT r, UINT b ): CRect(t,l,r,b), m_bEnabled(FALSE), m_bUseHandles( FALSE )
{ m_DeviceOrigin = CPoint(0,0); }

CILDragRect::CILDragRect( CRect& rRect ): CRect(rRect), m_bEnabled(FALSE), m_bUseHandles( FALSE )
{ m_DeviceOrigin = CPoint(0,0); }

CILDragRect::CILDragRect( CILDragRect &rRect ): CRect( rRect.TopLeft(), rRect.BottomRight() ), m_bEnabled(FALSE),
											    m_bUseHandles( FALSE )
{ m_DeviceOrigin = CPoint(0,0); }

void CILDragRect::Anchor( CPoint AnchorPt )
{
	top = AnchorPt.y;
	left = AnchorPt.x;
}

void CILDragRect::Drag( CPoint DragPt  )
{
	bottom = DragPt.y;
	right = DragPt.x;
}

void CILDragRect::Clear( CWnd *pWnd )
{
	CILDragRect LastInnerRect = *this;
	CILDragRect LastOuterRect  = *this;
	LastInnerRect.DeflateRect(1,1);
	
	LastInnerRect.OffsetRect( m_DeviceOrigin );
	LastOuterRect.OffsetRect( m_DeviceOrigin );

	pWnd->InvalidateRect( &LastOuterRect );
		
	if( LastOuterRect.Width() != 1 && LastOuterRect.Height() != 1 )
		pWnd->ValidateRect( &LastInnerRect );
				
	pWnd->UpdateWindow();
}

CILDragRect& CILDragRect::operator = ( CILDragRect &rRect )
{
	*((CRect*)this) = *((CRect*)&rRect);
	m_bEnabled = rRect.m_bEnabled;
	m_DeviceOrigin = rRect.m_DeviceOrigin;
	m_bUseHandles = rRect.m_bUseHandles;
	return *this;
}

CILDragRect& CILDragRect::operator = ( CRect &rRect )
{
	top = rRect.top;
	left = rRect.left;
	bottom = rRect.bottom;
	right = rRect.right;

	return *this;
}

void CILDragRect::Draw( CDC *pDC, int PenStyle, DWORD dwColor )
{
	CPen BlackPen, RedPen; 
	CBrush NullBrush, BlackBrush;
	int nSavedDC;
	CRect DrawRect;

	//Draw this rect to the device context.
	if( Height() != 0 && Width() != 0 )
	{	
		nSavedDC = pDC->SaveDC();
		BlackPen.CreatePen( PenStyle, 1, dwColor );
		NullBrush.CreateStockObject( NULL_BRUSH );
		pDC->SelectObject( &NullBrush );
		pDC->SelectObject( &BlackPen );

		DrawRect = *this;
		DrawRect.OffsetRect( m_DeviceOrigin );
		pDC->Rectangle( &DrawRect );
	
		/*
		if( m_bUseHandles )
		{
			CRect rectHandle;
			CBrush BlackBrush;
			CPen SolidPen;

			BlackBrush.CreateStockObject( BLACK_BRUSH );
			SolidPen.CreatePen( PS_SOLID, 1, 0 );
			pDC->SelectObject( &BlackBrush );
			pDC->SelectObject( &SolidPen );
			rectHandle.SetRect( TopLeft(), TopLeft() + CPoint( m_sizeHandle ) );
			
			rectHandle.OffsetRect( m_DeviceOrigin );
			pDC->Rectangle( &rectHandle );

			rectHandle.OffsetRect( Width() - rectHandle.Width(), 0 );
			pDC->Rectangle( &rectHandle );

			rectHandle.OffsetRect( 0, Height() - rectHandle.Height() );
			pDC->Rectangle( &rectHandle );

			rectHandle.OffsetRect( -( Width() - rectHandle.Width() ), 0 );
			pDC->Rectangle( &rectHandle );

			BlackBrush.DeleteObject();
			SolidPen.DeleteObject();
		}
		*/
		
		pDC->RestoreDC( nSavedDC );
		BlackPen.DeleteObject();
		NullBrush.DeleteObject();
	}
}


