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

IMPLEMENT_DYNAMIC( CILPolygon, CILShape )

CILPolygon::CILPolygon()
{}

CILPolygon::CILPolygon( UINT l, UINT t, UINT r, UINT b ): CILShape(l,t,r,b)
{}
 
void CILPolygon::DrawSymbol( CDC *pDC )
{
	CPen SymbolPen;
	CBrush SymbolBrush;
	POINT *pPoints = NULL;
	UINT nPoints;
	int nSavedDC = pDC->SaveDC();

	CILShape::DrawSymbol( pDC );

	//Transform to canvas space.
	m_BoundingRect.OffsetRect( m_CanvasOrigin );
	//Define the points that make up this polygon.
	CreatePoints( &pPoints, &nPoints );
	//Transform back to diagram space.
	m_BoundingRect.OffsetRect( -m_CanvasOrigin );

	//create drawing objects.
	SymbolPen.CreatePen( m_uPenStyle, m_uEdgeWidth, m_dwEdgeColor );
	SymbolBrush.CreateSolidBrush( m_dwBGColor );

	//set new device state for drawiing symbol.
	pDC->SelectObject( SymbolPen );
	pDC->SelectObject( SymbolBrush );
	
	//Draw symbol ( edged polygon )
	pDC->Polygon( pPoints, nPoints );

	_RenderCaption( pDC );
	
	//restore old device state.
	pDC->RestoreDC( nSavedDC );

	delete[] pPoints; pPoints = NULL;
	SymbolBrush.DeleteObject();
	SymbolPen.DeleteObject();
}

