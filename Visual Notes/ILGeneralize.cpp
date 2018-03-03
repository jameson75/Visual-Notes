#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILGeneralize, CILShape, VERSIONABLE_SCHEMA | 1 );

CILGeneralize::CILGeneralize() : CILPolygon()
{}

CILGeneralize::CILGeneralize( UINT l, UINT t, UINT r, UINT b ) : CILPolygon( l,t,r,b )
{}

void CILGeneralize::CreatePoints( POINT **ppPoints, UINT *pNPoints )
{
	*pNPoints = 3;

	*ppPoints = new POINT[*pNPoints];

	*( (*ppPoints) + 0 ) = CPoint( m_BoundingRect.left + m_BoundingRect.Width() / 2, m_BoundingRect.top );
	*( (*ppPoints) + 1 ) = CPoint( m_BoundingRect.left, m_BoundingRect.bottom - 1 );
	*( (*ppPoints) + 2 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.bottom - 1 );
}

void CILGeneralize::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILGeneralize; }