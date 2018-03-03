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

IMPLEMENT_SERIAL( CILDiamond, CILPolygon, VERSIONABLE_SCHEMA | 1 )

CILDiamond::CILDiamond()
{}

CILDiamond::CILDiamond( UINT l, UINT t, UINT r, UINT b ) : CILPolygon(l,t,r,b)
{}

void CILDiamond::CreatePoints( POINT **ppPoints, UINT *pNPoints )
{
	*ppPoints = new POINT[4];
	*pNPoints = 4;

	*( (*ppPoints) + 0 ) = CPoint( m_BoundingRect.left + ( m_BoundingRect.Width() / 2 ), m_BoundingRect.top );
	*( (*ppPoints) + 1 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.top + (m_BoundingRect.Height() / 2) );
	*( (*ppPoints) + 2 ) = CPoint( m_BoundingRect.left + ( m_BoundingRect.Width() / 2 ), m_BoundingRect.bottom - 1);
	*( (*ppPoints) + 3 ) = CPoint( m_BoundingRect.left, m_BoundingRect.top + (m_BoundingRect.Height() / 2) );
}

void CILDiamond::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILDiamond; }
