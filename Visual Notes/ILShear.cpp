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

IMPLEMENT_SERIAL( CILShear, CILPolygon, VERSIONABLE_SCHEMA | 1 );

CILShear::CILShear() : m_ShearFactor(8)
{}

CILShear::CILShear( UINT l, UINT t, UINT r, UINT b ) : CILPolygon(l,t,r,b), m_ShearFactor(8)
{} 

void CILShear::CreatePoints( POINT **ppPoints, UINT *pNPoints )
{
	*ppPoints = new POINT[4];
	*pNPoints = 4;

	*( (*ppPoints) + 0 ) = m_BoundingRect.TopLeft() + CPoint(m_ShearFactor,0);
	*( (*ppPoints) + 1 ) = CPoint( m_BoundingRect.right - 1, m_BoundingRect.top );
	*( (*ppPoints) + 2 ) = m_BoundingRect.BottomRight() + CPoint(-m_ShearFactor -1,-1);
	*( (*ppPoints) + 3 ) = CPoint( m_BoundingRect.left, m_BoundingRect.bottom - 1 );
}

void CILShear::_CopyBaseData( CILSymbol **ppSymbol )
{
	CILShape::_CopyBaseData( ppSymbol );
	
	((CILShear*)(*ppSymbol))->m_ShearFactor = m_ShearFactor;
}	

void CILShear::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILShear; }
