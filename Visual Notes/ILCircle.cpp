//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/20/01
//*********************************************

#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILCircle, CILShape, VERSIONABLE_SCHEMA | 1 );

CILCircle::CILCircle()
{}

CILCircle::CILCircle( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{}

void CILCircle::DrawSymbol( CDC *pDC )
{
	CBrush SymbolBrush;
	CPen SymbolPen;
	int nSavedDC = pDC->SaveDC();

	CILShape::DrawSymbol(pDC);


	SymbolBrush.CreateSolidBrush( m_dwBGColor );
	SymbolPen.CreatePen( m_uPenStyle, m_uEdgeWidth,  m_dwEdgeColor );
	pDC->SelectObject( SymbolBrush );
	pDC->SelectObject( SymbolPen );

	m_BoundingRect.OffsetRect( m_CanvasOrigin ); //Transform circle to canvas space.
	pDC->Ellipse( &m_BoundingRect );
	m_BoundingRect.OffsetRect( -m_CanvasOrigin ); //Transform circle back to diagram space.

	_RenderCaption( pDC );

	pDC->RestoreDC( nSavedDC );
	SymbolPen.DeleteObject();
	SymbolBrush.DeleteObject();
}

void CILCircle::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILCircle; } 


