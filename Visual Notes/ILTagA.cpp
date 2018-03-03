#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILTagA, CILPolygon, VERSIONABLE_SCHEMA | 1 );

CILTagA::CILTagA() : CILPolygon()
{}

CILTagA::CILTagA( UINT l, UINT t, UINT r, UINT b ) : CILPolygon( l,t,r,b )
{}

void CILTagA::CreatePoints( POINT **ppPoints, UINT *pNPoints )
{
	*pNPoints = 5;
	*ppPoints = new POINT[*pNPoints];

	*( (*ppPoints) + 0 ) = m_BoundingRect.TopLeft();
	*( (*ppPoints) + 1 ) = CPoint( m_BoundingRect.left, m_BoundingRect.bottom - 1 );
	*( (*ppPoints) + 2 ) = CPoint( m_BoundingRect.right - m_BoundingRect.Width() / 4, m_BoundingRect.bottom - 1  );
	*( (*ppPoints) + 3 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.top + m_BoundingRect.Height() / 2 );
	*( (*ppPoints) + 4 ) = CPoint( m_BoundingRect.right - m_BoundingRect.Width() / 4, m_BoundingRect.top );

}

void CILTagA::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILTagA; }