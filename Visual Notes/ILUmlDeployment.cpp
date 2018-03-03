#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILUmlDeployment, CILShape, VERSIONABLE_SCHEMA | 1 );

CILUmlDeployment::CILUmlDeployment() : CILShape()
{}

CILUmlDeployment::CILUmlDeployment( UINT l, UINT t, UINT r, UINT b ) : CILShape( l,t,r,b )
{}

void CILUmlDeployment::DrawSymbol( CDC *pDC )
{
	CBrush BkgndBrush;
	CPen EdgePen;
	int nSavedDC = pDC->SaveDC();
	CRect rect;
	CPoint pt3DBox[7], pt3DEdge1[3], pt3DEdge2[2];

	CILShape::DrawSymbol( pDC );

	rect = m_BoundingRect;
	rect.OffsetRect( m_CanvasOrigin );

	BkgndBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( m_uPenStyle, 1, m_dwEdgeColor );

	pt3DBox[0] = CPoint( rect.right - 1, rect.top );
	pt3DBox[1] = CPoint( rect.left + rect.Width() / 4, rect.top );
	pt3DBox[2] = CPoint( rect.left, rect.top + rect.Height() / 4 );
	pt3DBox[3] = CPoint( rect.left, rect.bottom  - 1 );
	pt3DBox[4] = CPoint( rect.right - rect.Width() / 4, rect.bottom - 1 );
	pt3DBox[5] = CPoint( rect.right - 1, rect.bottom - rect.Height() / 4 );
	pt3DBox[6] = CPoint( pt3DBox[4].x, pt3DBox[2].y );

	pt3DEdge1[0] = pt3DBox[0];
	pt3DEdge1[1] = pt3DBox[6];
	pt3DEdge1[2] = pt3DBox[2];
	
	pt3DEdge2[0] = pt3DBox[4];
	pt3DEdge2[1] = pt3DBox[6];

	pDC->SelectObject( &BkgndBrush );
	pDC->SelectObject( &EdgePen );

	pDC->Polygon( pt3DBox, 6 );
	pDC->Polyline( pt3DEdge1, 3 );
	pDC->Polyline( pt3DEdge2, 2 );
	
	_RenderCaption( pDC );

	BkgndBrush.DeleteObject();
	EdgePen.DeleteObject();
	
	pDC->RestoreDC( nSavedDC );
}

void CILUmlDeployment::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILUmlDeployment; }

void CILUmlDeployment::_RenderCaption( CDC *pDC )
{
	CRect rectCaption;
	int nSavedDC = pDC->SaveDC();

	rectCaption = m_BoundingRect;
	rectCaption.OffsetRect( m_CanvasOrigin );

	rectCaption.right-= ( rectCaption.Width() / 4 );
	rectCaption.top+= ( rectCaption.Height() / 4 );

	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( m_strCaption, &rectCaption, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
	
	pDC->RestoreDC( nSavedDC );
}

