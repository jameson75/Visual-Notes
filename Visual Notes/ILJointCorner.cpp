#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILJointCorner, CILJoint, VERSIONABLE_SCHEMA | 1 )

CILJointCorner::CILJointCorner() : CILJoint(), m_nOutDistance( 15 )
{}

CILJointCorner::CILJointCorner( CPoint ptOrigin ) : CILJoint( ptOrigin ), m_nOutDistance( 15 )
{}

BOOL CILJointCorner::HitTest( CPoint CanvasPoint )
{
	CPoint Edge[4];
	CILJoint *pSymbiant = NULL;
	int nEdgePoints;
	int nHalfwayY, nHalfwayX;
	
	if( CILShape::HitTest( CanvasPoint ) )
		return TRUE;
	
	if( m_SymbiantList.IsEmpty() )
		return FALSE;

	pSymbiant = m_SymbiantList.GetHead();
	
	Edge[0] = GetOrigin();

	//if the control and the symbiant are on adjacent sides, then there are two
	//segments to test. else they are on parallel sides, and there are four segments
	//to hit test.
	if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_BOTTOM | JOINTDATA_TOP, FALSE ) )
	{
		if( CILBitOp::IsOn( pSymbiant->m_dwExtraJointData, JOINTDATA_LEFT | JOINTDATA_RIGHT, FALSE ) )
		{
			nEdgePoints = 3;
			Edge[2] = pSymbiant->GetOrigin();
			Edge[1]	= CPoint( Edge[0].x, Edge[2].y );
		}

		else
		{
			nEdgePoints = 4;
			Edge[3] = pSymbiant->GetOrigin();
			nHalfwayY = Edge[0].y + ( Edge[3].y - Edge[0].y ) / 2;
			Edge[1] = CPoint( Edge[0].x, nHalfwayY );
			Edge[2] = CPoint( Edge[3].x, nHalfwayY );
		}
	
		for( int i = 0; i < nEdgePoints - 1; i++ )
		{
			if( _HitTestEdge( &Edge[i], CanvasPoint ) )
				return TRUE;
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_RIGHT | JOINTDATA_LEFT, FALSE ) )
	{
		if( CILBitOp::IsOn( pSymbiant->m_dwExtraJointData, ( JOINTDATA_TOP | JOINTDATA_BOTTOM ), FALSE ) )
		{
			nEdgePoints = 3;
			Edge[2] = pSymbiant->GetOrigin();
			Edge[1]	= CPoint( Edge[2].x, Edge[0].y );
		}

		else
		{
			nEdgePoints = 4;
			Edge[3] = pSymbiant->GetOrigin();
			nHalfwayX = Edge[0].x + ( Edge[3].x - Edge[0].x ) / 2;
			Edge[1] = CPoint( nHalfwayX, Edge[0].y );
			Edge[2] = CPoint( nHalfwayX, Edge[3].y );
		}
		
		for( int i = 0; i < nEdgePoints - 1; i++ )
		{
			if( _HitTestEdge( &Edge[i], CanvasPoint ) )
				return TRUE;
		}
	}

	return FALSE;
}

void CILJointCorner::_RenderJointEdge( CDC *pDC )
{
	CPoint Edge[4];
	CILJoint *pSymbiant = NULL;
	int nEdgePoints;
	int nHalfwayY, nHalfwayX;
	
	if( m_SymbiantList.IsEmpty() )
		return;

	pSymbiant = m_SymbiantList.GetHead();
	
	Edge[0] = GetOrigin();

	if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_BOTTOM | JOINTDATA_TOP, FALSE ) )
	{
		if( CILBitOp::IsOn( pSymbiant->m_dwExtraJointData, JOINTDATA_LEFT | JOINTDATA_RIGHT, FALSE ) )
		{
			nEdgePoints = 3;
			Edge[2] = pSymbiant->GetOrigin();
			Edge[1]	= CPoint( Edge[0].x, Edge[2].y );
		}

		else
		{
			nEdgePoints = 4;
			Edge[3] = pSymbiant->GetOrigin();
			nHalfwayY = Edge[0].y + ( Edge[3].y - Edge[0].y ) / 2;
			Edge[1] = CPoint( Edge[0].x, nHalfwayY );
			Edge[2] = CPoint( Edge[3].x, nHalfwayY );
		}
		
		for( int i = 0; i < nEdgePoints; i++ )
			Edge[i].Offset( m_CanvasOrigin );

		pDC->Polyline( Edge, nEdgePoints );
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_RIGHT | JOINTDATA_LEFT, FALSE ) )
	{
		if( CILBitOp::IsOn( pSymbiant->m_dwExtraJointData, ( JOINTDATA_TOP | JOINTDATA_BOTTOM ), FALSE ) )
		{
			nEdgePoints = 3;
			Edge[2] = pSymbiant->GetOrigin();
			Edge[1]	= CPoint( Edge[2].x, Edge[0].y );
		}

		else
		{
			nEdgePoints = 4;
			Edge[3] = pSymbiant->GetOrigin();
			nHalfwayX = Edge[0].x + ( Edge[3].x - Edge[0].x ) / 2;
			Edge[1] = CPoint( nHalfwayX, Edge[0].y );
			Edge[2] = CPoint( nHalfwayX, Edge[3].y );
		}
		
		for( int i = 0; i < nEdgePoints; i++ )
			Edge[i].Offset( m_CanvasOrigin );

		pDC->Polyline( Edge, nEdgePoints );		
	}
}

void CILJointCorner::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILJointCorner; }
