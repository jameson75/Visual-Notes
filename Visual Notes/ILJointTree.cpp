#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILJointTree, CILJoint, VERSIONABLE_SCHEMA | 1 );

int CILJointTree::m_ForkDistance = 15;


CILJointTree::CILJointTree() : CILJoint()
{}

CILJointTree::CILJointTree( CPoint ptOrigin ) : CILJoint( ptOrigin )
{}

BOOL CILJointTree::GetUpdateRect( CRect *pUpdateRect )
{
	int maxRight, minLeft, maxBottom, minTop;
	CRect SymbiantRect;

	if( m_SymbiantList.IsEmpty() )
		return FALSE;

	minTop = m_BoundingRect.top;		
	maxBottom = m_BoundingRect.bottom;
	maxRight = m_BoundingRect.right;
	minLeft = m_BoundingRect.left;

	ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
	while( Pos )
	{
		SymbiantRect = m_SymbiantList.GetAt( Pos )->m_BoundingRect;
		if( SymbiantRect.left < minLeft ) minLeft = SymbiantRect.left;
		if( SymbiantRect.right > maxRight ) maxRight = SymbiantRect.right;
		if( SymbiantRect.top < minTop ) minTop = SymbiantRect.top;
		if( SymbiantRect.bottom > maxBottom ) maxBottom = SymbiantRect.bottom;

		m_SymbiantList.GetNext( Pos );
	}

	pUpdateRect->SetRect( minLeft, minTop, maxRight, maxBottom );
	pUpdateRect->NormalizeRect();

	return TRUE;
}

BOOL CILJointTree::HitTest( CPoint CanvasPoint )
{
	if( CILShape::HitTest( CanvasPoint ) )
		return TRUE;

	CPoint RootEdge[2], BranchEdge[2], LeafEdge[2];

	if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_BOTTOM, TRUE ) )
	{
		RootEdge[0] = GetOrigin();
		RootEdge[1] = CPoint( RootEdge[0].x, RootEdge[0].y + m_ForkDistance );

		if( _HitTestEdge( RootEdge, CanvasPoint ) )
			return TRUE;

		ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
		while( Pos )
		{
			BranchEdge[0] = RootEdge[1];
			BranchEdge[1] = CPoint( m_SymbiantList.GetAt(Pos)->GetOrigin().x, BranchEdge[0].y );

			if( _HitTestEdge( BranchEdge, CanvasPoint ) )
				return TRUE;

			LeafEdge[0] = BranchEdge[1];
			LeafEdge[1] = m_SymbiantList.GetAt( Pos )->GetOrigin();
			
			if( _HitTestEdge( LeafEdge, CanvasPoint ) )
				return TRUE;

			m_SymbiantList.GetNext( Pos );
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_TOP, TRUE ) )
	{
		RootEdge[0] = GetOrigin();
		RootEdge[1] = CPoint( RootEdge[0].x, RootEdge[0].y - m_ForkDistance );

		if( _HitTestEdge( RootEdge, CanvasPoint ) )
			return TRUE;

		ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
		while( Pos )
		{
			BranchEdge[0] = RootEdge[1];
			BranchEdge[1] = CPoint( m_SymbiantList.GetAt(Pos)->GetOrigin().x, BranchEdge[0].y );

			if( _HitTestEdge( BranchEdge, CanvasPoint ) )
				return TRUE;

			LeafEdge[0] = BranchEdge[1];
			LeafEdge[1] = m_SymbiantList.GetAt( Pos )->GetOrigin();
			
			if( _HitTestEdge( LeafEdge, CanvasPoint ) )
				return TRUE;

			m_SymbiantList.GetNext( Pos );
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_LEFT, TRUE ) )
	{
		RootEdge[0] = GetOrigin();
		RootEdge[1] = CPoint( RootEdge[0].x - m_ForkDistance, RootEdge[0].y );

		if( _HitTestEdge( RootEdge, CanvasPoint ) )
			return TRUE;

		ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
		while( Pos )
		{
			BranchEdge[0] = RootEdge[1];
			BranchEdge[1] = CPoint( BranchEdge[0].x, m_SymbiantList.GetAt(Pos)->GetOrigin().y  );

			if( _HitTestEdge( BranchEdge, CanvasPoint ) )
				return TRUE;

			LeafEdge[0] = BranchEdge[1];
			LeafEdge[1] = m_SymbiantList.GetAt( Pos )->GetOrigin();
			
			if( _HitTestEdge( LeafEdge, CanvasPoint ) )
				return TRUE;

			m_SymbiantList.GetNext( Pos );
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_RIGHT, TRUE ) )
	{
		RootEdge[0] = GetOrigin();
		RootEdge[1] = CPoint( RootEdge[0].x + m_ForkDistance, RootEdge[0].y );

		if( _HitTestEdge( RootEdge, CanvasPoint ) )
			return TRUE;

		ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
		while( Pos )
		{
			BranchEdge[0] = RootEdge[1];
			BranchEdge[1] = CPoint( BranchEdge[0].x, m_SymbiantList.GetAt(Pos)->GetOrigin().y  );

			if( _HitTestEdge( BranchEdge, CanvasPoint ) )
				return TRUE;

			LeafEdge[0] = BranchEdge[1];
			LeafEdge[1] = m_SymbiantList.GetAt( Pos )->GetOrigin();
			
			if( _HitTestEdge( LeafEdge, CanvasPoint ) )
				return TRUE;

			m_SymbiantList.GetNext( Pos );
		}
	}

	return FALSE;
}

void CILJointTree::_RenderJointEdge( CDC *pDC )
{
	ILPOSITION Pos;
	CPoint points[4];

	points[0] = GetOrigin(); //start point.
	points[0].Offset( m_CanvasOrigin );
	
	Pos = m_SymbiantList.GetHeadPosition();
	if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_BOTTOM, TRUE ) )
	{
		points[1] = CPoint( GetOrigin().x, GetOrigin().y + m_ForkDistance ); //fork.
		points[1].Offset( m_CanvasOrigin );
					
		while( Pos )
		{
			points[3] = m_SymbiantList.GetAt( Pos )->GetOrigin(); //final destination.
			points[3].Offset( m_CanvasOrigin );
			points[2] = CPoint( points[3].x, points[1].y ); //corner.
			//Note: since points[2] is created from offset coordinates, there's no need 
			//		to offset it.
					
			//only draw edge if the target symbol is below the fork.
			if( points[1].y < points[3].y )
				pDC->Polyline( points, 4 );
			
			m_SymbiantList.GetNext( Pos );
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_TOP, TRUE ) )
	{
		points[1] = CPoint( GetOrigin().x, GetOrigin().y - m_ForkDistance ); //fork.
		points[1].Offset( m_CanvasOrigin );
		
		while( Pos )
		{
			points[3] = m_SymbiantList.GetAt( Pos )->GetOrigin(); //final dest.
			points[3].Offset( m_CanvasOrigin );
			points[2] = CPoint( points[3].x, points[1].y ); //corner.
			//Note: since points[2] is created from offset coordinates, there's no need 
			//		to offset it.

			//only draw edge if the target symbol is above the fork.
			if( points[1].y > points[3].y )
			pDC->Polyline( points, 4 );

			m_SymbiantList.GetNext( Pos );
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_LEFT, TRUE ) )
	{
		points[1] = CPoint( GetOrigin().x - m_ForkDistance, GetOrigin().y ); //fork.
		points[1].Offset( m_CanvasOrigin );

		while( Pos )
		{
			points[3] = m_SymbiantList.GetAt( Pos )->GetOrigin(); //final dest.
			points[3].Offset( m_CanvasOrigin );
			points[2] = CPoint( points[1].x, points[3].y ); //corner
			//Note: since points[2] is created from offset coordinates, there's no need 
			//		to offset it.

			//only draw edge if the target symbol is left of the fork.
			if( points[1].x > points[3].x )
				pDC->Polyline( points, 4 );

			m_SymbiantList.GetNext( Pos );
		}
	}

	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_RIGHT, TRUE ) )
	{
		points[1] = CPoint( GetOrigin().x + m_ForkDistance, GetOrigin().y ); //fork.
		points[1].Offset( m_CanvasOrigin );

		while( Pos )
		{
			points[3] = m_SymbiantList.GetAt( Pos )->GetOrigin(); //final dest.
			points[3].Offset( m_CanvasOrigin );
			points[2] = CPoint( points[1].x, points[3].y ) ;//corner
			//Note: since points[2] is created from offset coordinates, there's no need 
			//		to offset it.

			//only draw edge if the target symbol is right of the fork.
			if( points[1].x < points[3].x )
				pDC->Polyline( points, 4 );

			m_SymbiantList.GetNext( Pos );
		}
	}
}

void CILJointTree::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILJointTree; }





