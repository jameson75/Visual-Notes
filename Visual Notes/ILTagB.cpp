#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILTagB, CILPolygon, VERSIONABLE_SCHEMA | 1 );

CILTagB::CILTagB() : CILPolygon()
{}

CILTagB::CILTagB( UINT l, UINT t, UINT r, UINT b ) : CILPolygon( l, t, r, b )
{}

void CILTagB::CreatePoints( POINT **ppPoints, UINT *pNPoints )
{
	*pNPoints = 5;
	*ppPoints = new POINT[*pNPoints];

	*( (*ppPoints) + 0 ) = m_BoundingRect.TopLeft();
	*( (*ppPoints) + 1 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.top );
	*( (*ppPoints) + 2 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.bottom - 1 );
	*( (*ppPoints) + 3 ) = CPoint( m_BoundingRect.left, m_BoundingRect.bottom - 1 );
	*( (*ppPoints) + 4 ) = CPoint( m_BoundingRect.left + m_BoundingRect.Width() / 4, 
								   m_BoundingRect.top + m_BoundingRect.Height() / 2 );
}

void CILTagB::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILTagB; }


