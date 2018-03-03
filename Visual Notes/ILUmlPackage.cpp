#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILUmlPackage, CILShape, VERSIONABLE_SCHEMA | 1 );

CSize CILUmlPackage::ms_TagSize = CSize( 20, 10 );

CILUmlPackage::CILUmlPackage()
{}

CILUmlPackage::CILUmlPackage( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{}

void CILUmlPackage::DrawSymbol( CDC *pDC )
{
	CBrush BGBrush, SecondaryBrush;
	CPen EdgePen;
	int nSavedDC = pDC->SaveDC();
	CRect BaseRect, TagRect;

	CILShape::DrawSymbol( pDC );

	BGBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );
	SecondaryBrush.CreateSolidBrush( m_dwSecondaryColor );	

	TagRect = m_BoundingRect;
	TagRect.bottom = m_BoundingRect.top + ms_TagSize.cy;
	TagRect.right = TagRect.left + ms_TagSize.cx;
	BaseRect = m_BoundingRect;
	BaseRect.top = TagRect.bottom - 1;

	TagRect.OffsetRect( m_CanvasOrigin );
	BaseRect.OffsetRect( m_CanvasOrigin );
	pDC->SelectObject( &BGBrush );
	pDC->SelectObject( &EdgePen );
	
	pDC->Rectangle( &BaseRect );
	pDC->SelectObject( SecondaryBrush );
	pDC->Rectangle( &TagRect );

	_RenderCaption( pDC );
	
	BGBrush.DeleteObject();
	EdgePen.DeleteObject();
	SecondaryBrush.DeleteObject();
	pDC->RestoreDC( nSavedDC );
}

void CILUmlPackage::_New( CILSymbol **ppSymbol )
{  *ppSymbol = new CILUmlPackage; }


