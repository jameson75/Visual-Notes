//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/16/01
//*********************************************

#include "StdAfx.h"
#include "Stdil.h"

IMPLEMENT_SERIAL( CILRectangle, CILPolygon, VERSIONABLE_SCHEMA | 1 );

CILRectangle::CILRectangle()
{}

CILRectangle::CILRectangle( UINT l, UINT t, UINT r, UINT b ) : CILPolygon(l,t,r,b)
{}

void CILRectangle::CreatePoints( POINT **ppPoints, UINT *pNPoints )
{	
	*ppPoints = new POINT[4];
	*pNPoints = 4;

	//Establish symbol (rectangle's) vertices.
	*( (*ppPoints) + 0 ) = m_BoundingRect.TopLeft();
	*( (*ppPoints) + 1 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.top );
	*( (*ppPoints) + 2 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.bottom - 1 );
	*( (*ppPoints) + 3 ) = CPoint( m_BoundingRect.left, m_BoundingRect.bottom - 1 );
}

void CILRectangle::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILRectangle; }



	




