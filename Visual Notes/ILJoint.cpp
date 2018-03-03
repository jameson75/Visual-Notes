
#include "stdafx.h"
#include "stdil.h"

#include "EdgePropBox.h"

IMPLEMENT_SERIAL( CILJoint, CILShape, VERSIONABLE_SCHEMA | 1 )

int CILJoint::m_nDefJointEdgeWidth = ILJOINT_DEFAULT_EDGEWIDTH;
DWORD CILJoint::m_dwDefJointBkgndColor = ILJOINT_DEFAULT_BKGNDCLR;
DWORD CILJoint::m_dwDefJointEdgeColor = ILJOINT_DEFAULT_EDGECLR;
CSize CILJoint::m_sizeJointRect = CSize( 11, 11 );
UINT CILJoint::m_uDefEdgeStyle = ILJOINT_DEFAULT_EDGESTYLE;

CILJoint::JOINTSYMBOL CILJoint::m_DefJointSymbol = ILJOINT_DEFAULT_MASTERSYMBOL;
CILJoint::JOINTSYMBOL CILJoint::m_DefSymbiantJointSymbol = ILJOINT_DEFAULT_SLAVESYMBOL;

CILJoint::CILJoint() : CILShape(), m_nEdgeStyle(PS_SOLID), m_dwExtraJointData(0),
					   m_pControlJoint( NULL ), m_dwSelectionWidth(m_uEdgeWidth + 5 ), 
					   m_dwSelectionColor( RGB(200,200,200) ), m_HitTolerance(5),
					   m_BoundPadding(0), m_uPointSize(30), m_pMasterShape(NULL), m_bEdgeSelection(FALSE),
					   m_nJointEdgeWidth( m_nDefJointEdgeWidth ), m_dwJointEdgeColor( m_dwDefJointEdgeColor ),
					   m_dwJointBkgndColor( m_dwDefJointBkgndColor ), m_JointSymbol( m_DefJointSymbol ), 
					   m_SymbiantJointSymbol( m_DefSymbiantJointSymbol ), m_uEdgeStyle( m_uDefEdgeStyle )
{
	m_Tracker.m_nHandleSize = 4;
}

CILJoint::CILJoint( CPoint Origin ) : CILShape(), m_nEdgeStyle(PS_SOLID), m_dwExtraJointData(0),
									  m_pControlJoint( NULL ), m_dwSelectionWidth(m_uEdgeWidth  + 5 ), 
									  m_dwSelectionColor( RGB(200,200,200) ), m_HitTolerance(5),
									  m_BoundPadding(0), m_uPointSize(30), m_pMasterShape(NULL), m_bEdgeSelection(FALSE),
									  m_nJointEdgeWidth( m_nDefJointEdgeWidth ), m_dwJointBkgndColor( m_dwDefJointBkgndColor ),
									  m_dwJointEdgeColor( m_dwDefJointEdgeColor ), m_JointSymbol( m_DefJointSymbol ), 
									  m_SymbiantJointSymbol( m_DefSymbiantJointSymbol ), m_uEdgeStyle( m_uDefEdgeStyle )
{ 
	m_BoundingRect = CRect( 0, 0, m_sizeJointRect.cx, m_sizeJointRect.cy );
	SetOrigin( Origin );
	m_Tracker.m_nHandleSize = 4;
}

void CILJoint::SetOrigin( CPoint Origin )
{
	CRect NewRect = m_BoundingRect;

	NewRect.left = Origin.x - m_BoundingRect.Width() / 2;
	NewRect.right = Origin.x + m_BoundingRect.Width() / 2;
	NewRect.top = Origin.y - m_BoundingRect.Height() / 2;
	NewRect.bottom = Origin.y + m_BoundingRect.Height() / 2;

	SetBoundingRect( NewRect );
}

void CILJoint::DrawSymbol( CDC *pDC )
{
	CRect JointRect, JSRect;
	CPen NullPen, EdgePen, JSEdgePen;
	CBrush JointBrush, BkgndBrush;
	CPoint ptArrow[3], ptAggregation[4], ptPlus[4], ptMinus[4], ptContEdge[2];
	int nSavedDC = pDC->SaveDC();

	DWORD dwJointColor = ( m_pControlJoint ) ? m_pControlJoint->m_dwJointBkgndColor : m_dwJointBkgndColor;

	CILShape::DrawSymbol( pDC );

	if( m_bEdgeSelection )
		_RenderSelection( pDC );

	EdgePen.CreatePen( m_uEdgeStyle, m_nJointEdgeWidth, m_dwJointEdgeColor );
	pDC->SelectObject( &EdgePen );
	_RenderJointEdge( pDC );
	
	NullPen.CreateStockObject( NULL_PEN );
	JointBrush.CreateSolidBrush( dwJointColor );
	BkgndBrush.CreateSolidBrush( pDC->GetBkColor() );
	JSEdgePen.CreatePen( PS_SOLID, 1, m_dwJointEdgeColor );

	
	pDC->SelectObject( &NullPen );
	pDC->SelectObject( &JointBrush );

	//only attempt to draw body if one exists.
	if( m_BoundingRect.Width() && m_BoundingRect.Height() )
	{
		JointRect = m_BoundingRect;
		JointRect.OffsetRect( m_CanvasOrigin );
		JSRect = JointRect;

		JOINTSYMBOL JSymbol = ( m_pControlJoint ) ? m_pControlJoint->m_SymbiantJointSymbol : m_JointSymbol;

		//****************************************************************************
		// Some calculations for rendering belowing indicate they are tweaked.
		// This means that, I made adjustments soley based on esthetic observations.
		//****************************************************************************
		switch( JSymbol )
		{
		case jointBlock:

			JSRect.DeflateRect( 2, 2 );
			JSRect.OffsetRect( 1, 1 );//<tweak>
			pDC->Rectangle( &JSRect );
			break;

		case jointDot:
			JSRect.DeflateRect( 1, 1 );
			JSRect.OffsetRect( 1, 1 ); //<tweak>
			pDC->Ellipse( &JSRect );
			break;

		case jointArrow:
		case jointDep:
			JSRect.DeflateRect( 1, 1 );
			//direction of arrow is determined by the side of master shape this joint is anchored to.
			if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_TOP, TRUE ) )
			{
				ptArrow[0] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.bottom );
				ptArrow[1] = CPoint( JSRect.right, JSRect.top + JSRect.Height() / 2 );
				ptArrow[2] = CPoint( JSRect.left, ptArrow[1].y );
			}

			else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_BOTTOM, TRUE ) )
			{
				ptArrow[0] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.top );
				ptArrow[1] = CPoint( JSRect.left, JSRect.top + JSRect.Height() / 2 + 1 ); //"+1" is a tweak.
				ptArrow[2] = CPoint( JSRect.right, ptArrow[1].y );
			}

			else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_RIGHT, TRUE ) )
			{
				ptArrow[0] = CPoint( JSRect.left, JSRect.top + JSRect.Height() / 2 );
				ptArrow[1] = CPoint( JSRect.left + JSRect.Width() / 2 + 1, JSRect.bottom ); //"+1" is a tweak 
				ptArrow[2] = CPoint( ptArrow[1].x, JSRect.top );
			}

			else
			{
				ptArrow[0] = CPoint( JSRect.right, JSRect.top + JSRect.Height() / 2 );
				ptArrow[1] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.top );
				ptArrow[2] = CPoint( ptArrow[1].x, JSRect.bottom );
			}
			
			if( JSymbol == jointDep )
			{
				pDC->SelectObject( &JSEdgePen );
				pDC->SelectObject( &BkgndBrush );
			}
			
			pDC->Polygon( ptArrow, 3 );
			break;

		case jointOne:
			pDC->DrawText( TEXT("1"), &JointRect, DT_VCENTER | DT_CENTER );
			break;

		case jointMany:
			//we logically divide the JSRect down the middle and use the two rectnagles to 
			//draw each ellispse of "many" symbol.

			JSRect.DeflateRect( 0, 1 );
			pDC->SelectObject( &JSEdgePen );
			pDC->SelectObject( &BkgndBrush );

			pDC->Ellipse( &CRect( JSRect.TopLeft(), 
						   CPoint( JSRect.left + JSRect.Width() / 2, JSRect.bottom ) ) );
			
			pDC->Ellipse( &CRect( CPoint( JSRect.left + JSRect.Width() / 2, JSRect.top ), 
						   JSRect.BottomRight() ) ) ;
			break;

		case jointAggregation:
			JSRect.DeflateRect( 0, 0 );
			ptAggregation[0] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.top );
			ptAggregation[1] = CPoint( JSRect.left, JSRect.top + JSRect.Height() / 2 );
			ptAggregation[2] = CPoint( ptAggregation[0].x, JSRect.bottom );
			ptAggregation[3] = CPoint( JSRect.right, ptAggregation[1].y );

			pDC->SelectObject( &JSEdgePen );
			pDC->SelectObject( &BkgndBrush );
			pDC->Polygon( ptAggregation, 4 );

			break;

		case jointNone:

			//For this case we draw an edge continuation from the orign of the joint to the master shape.
			//The direction of the continued edge depends upon which side of the master shape the joint 
			//is anchored to.
			ptContEdge[0] = GetOrigin();
			ptContEdge[0].Offset( m_CanvasOrigin );
			
			if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_TOP, TRUE ) )
				ptContEdge[1] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.bottom );

			else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_BOTTOM, TRUE ) )
				ptContEdge[1] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.top );
			
			else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_RIGHT, TRUE ) )
			{
				ptContEdge[1] = CPoint( JSRect.left, JSRect.top + JSRect.Height() / 2 );
				ptContEdge[0].x++; //tweak for 1 pixel gap I saw during testing.
			}
			
			else
			{
				ptContEdge[1] = CPoint( JSRect.right, JSRect.top + JSRect.Height() / 2 );
				ptContEdge[0].x--; //tweak for 1 pixel gap I saw during testing.
			}

			pDC->SelectObject( &EdgePen );
			pDC->Polyline( ptContEdge, 2 );
			
			break;

		case jointPlus:
			JSRect.DeflateRect( 0, 0 );
			pDC->SelectObject( &JSEdgePen );
			pDC->SelectObject( &BkgndBrush );
			pDC->Rectangle( &JSRect );

			JSRect.DeflateRect( 3, 3 );
			ptPlus[0] = CPoint( JSRect.left, JSRect.top + JSRect.Height() / 2 );
			ptPlus[1] = CPoint( JSRect.right, JSRect.top + JSRect.Height() / 2 );
			ptPlus[2] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.top );
			ptPlus[3] = CPoint( JSRect.left + JSRect.Width() / 2, JSRect.bottom );
			pDC->Polyline( ptPlus, 2 );
			pDC->Polyline( &ptPlus[2], 2 );
			break;

		case jointMinus:
			JSRect.DeflateRect( 0, 0 );
			pDC->SelectObject( &JSEdgePen );
			pDC->SelectObject( &BkgndBrush );
			pDC->Rectangle( &JSRect );
			
			JSRect.DeflateRect( 1, 1 );
			ptMinus[0] = CPoint( JSRect.left, JSRect.top + JSRect.Height() / 2 );
			ptMinus[1] = CPoint( JSRect.right, JSRect.top + JSRect.Height() / 2 );
			pDC->Polyline( ptMinus, 2 );		
			break;

		}
	}	

	pDC->RestoreDC( nSavedDC );

	NullPen.DeleteObject();
	JointBrush.DeleteObject();
	EdgePen.DeleteObject();
	BkgndBrush.DeleteObject();
	JSEdgePen.DeleteObject();
}

CPoint CILJoint::GetOrigin()
{
	return CPoint( m_BoundingRect.left + m_BoundingRect.Width() / 2,
				   m_BoundingRect.top + m_BoundingRect.Height() / 2 );
}

void CILJoint::SetControlJoint( CILJoint *pControl )
{	m_pControlJoint = pControl;  }

void CILJoint::AddSymbiantJoint( CILJoint *pSymbiant )
{
	//prevent symbiant from being added twice.
	if( m_SymbiantList.Find( pSymbiant ) )
		return;

	m_SymbiantList.AddTail( pSymbiant ); 
}

CILJoint* CILJoint::GetControlJoint()
{ return m_pControlJoint; }

BOOL CILJoint::GetSymbiantJoints( CILList<CILJoint*,CILJoint*> *pSymbiantList )
{
	ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
	while( Pos )
	{
		pSymbiantList->AddTail( m_SymbiantList.GetAt( Pos ) );
		m_SymbiantList.GetNext( Pos );
	}	
	return !pSymbiantList->IsEmpty();
}

BOOL CILJoint::RemoveSymbiantJoint( CILSymbol *pJoint )
{
	ILPOSITION Pos;

	if( !( Pos = m_SymbiantList.Find( (CILJoint*)pJoint ) ) )
		return FALSE;

	m_SymbiantList.RemoveAt( Pos );
	return TRUE;
}

void CILJoint::SetMasterShape( CILSymbol *pMasterShape )
{ m_pMasterShape = pMasterShape; }

CILSymbol* CILJoint::GetMasterShape()
{ return m_pMasterShape; }

void CILJoint::AddChild( CILSymbol *pSymbol )
{ /* YOU CAN'T ADD A CHILD TO A JOINT */ }

void CILJoint::AddJoint( CILSymbol *pJoint )
{ /*YOU CAN'T ADD A JOINT TO A JOINT*/ }

BOOL CILJoint::Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList, 
					  CILList<CILSymbol*,CILSymbol*> *pJointList )
{
	CPoint CanvasPoint(WindowPoint);
	CanvasPoint.Offset( -m_CanvasOrigin );
	
	//A Joint can only be tracked unilaterally. It cannot be resized.
	if( m_Tracker.HitTest( CanvasPoint ) == 8 )
		return CILShape::Track( pWnd, WindowPoint, pXYChildList, pJointList );

	return FALSE;
}

BOOL CILJoint::HasSymbiants()
{ return !m_SymbiantList.IsEmpty(); }

BOOL CILJoint::GetUpdateRect( CRect *pUpdateRect )
{
	//BRUTE METHOD.
	//I will later optimize this method, by recursively
	//halfing the edge, creating smaller rects for each half,
	//which should cut down on the flicker area.

	CRect NextRect;

	if( !m_SymbiantList.IsEmpty() )
	{
		NextRect.SetRect( GetOrigin(), m_SymbiantList.GetHead()->GetOrigin() );
		NextRect.NormalizeRect();  //normalize rect for GDI.	
		NextRect.InflateRect(2 + m_dwSelectionWidth , 2 + m_dwSelectionWidth ); //inflate rect incase it's linear
		*pUpdateRect = NextRect;
		return TRUE;
	}

	return FALSE;
}

void CILJoint::InvalidateSymbol( CWnd *pWnd )
{
	CILShape::InvalidateSymbol( pWnd );
	
	//Invaliation of this joint includes the updating of two rectangles.
	//1. The one formed from this joint and this joints symbiant joint.
	//2. The one formed from this joint and it's control joint.
	//   <See GetUpdateRect for rectangle formation>
	CRect rectUpdate;
	if( GetUpdateRect( &rectUpdate ) )
	{
		rectUpdate.OffsetRect( m_CanvasOrigin );
		pWnd->InvalidateRect( &rectUpdate );
	}

	if( m_pControlJoint )
	{
		m_pControlJoint->GetUpdateRect( &rectUpdate );
		rectUpdate.OffsetRect( m_CanvasOrigin );
		pWnd->InvalidateRect( &rectUpdate );
	}
}

BOOL CILJoint::IsSelected()
{ 
	if( !m_SymbiantList.IsEmpty() )
		return m_bEdgeSelection; 

	else 
		return CILShape::IsSelected();
}

void CILJoint::ShowSelection( BOOL bSelection, CWnd *pWnd )
{ 
	//if this is a control joint, then we should show edge selection.
	if( !m_SymbiantList.IsEmpty() )
		m_bEdgeSelection = bSelection;

	//else if this is a symbiant joint, we should show shape selection.
	else
		CILShape::ShowSelection( bSelection, pWnd );
}

BOOL CILJoint::HitTest( CPoint CanvasPoint )
{
	CPoint Edge[2];

	if( CILShape::HitTest( CanvasPoint ) )
		return TRUE;

	if( m_SymbiantList.IsEmpty() )
		return FALSE;

	Edge[0] = GetOrigin();
	Edge[1] = m_SymbiantList.GetHead()->GetOrigin();

	return _HitTestEdge( Edge, CanvasPoint );
}

void CILJoint::_RenderJointEdge( CDC *pDC )
{
	if( !m_SymbiantList.IsEmpty() )
	{
		CPoint Point[2];
		CPoint ChildCenter, ThisCenter;
	
		ThisCenter = GetOrigin();
		ChildCenter = m_SymbiantList.GetHead()->GetOrigin();
		ThisCenter.Offset( m_CanvasOrigin );
		ChildCenter.Offset( m_CanvasOrigin );
		Point[0] = ThisCenter;
		Point[1] = ChildCenter;
		pDC->Polyline( Point, 2 );
	}
}

void CILJoint::_RenderSelection( CDC *pDC )
{
	int nSavedDC = pDC->SaveDC();
	CPen SelectPen;

	SelectPen.CreatePen( PS_SOLID, m_dwSelectionWidth, m_dwSelectionColor );
	pDC->SelectObject( &SelectPen );
	_RenderJointEdge( pDC );
	pDC->RestoreDC( nSavedDC );
	SelectPen.DeleteObject();
}

BOOL CILJoint::_HitTestEdge( CPoint *pEdge, CPoint CanvasPoint )
{
	// P = P1 + u( P2 - P1 )
	//We need to calculate the closest point (P) on this edge (P1,P2), to
	//the hit-point (P3).
	CPoint P1, P2, P3, P, PDiff;
	float fu, fuNumerator, fuDenominator, fdistance;
	float fdistanceP1, fdistanceP2, fLengthSeg;

	//Given...
	P1 = *( pEdge );
	P2 = *( pEdge + 1 );
	P3 = CanvasPoint;
	
	//Calcuate u...
	fuNumerator = (float)((P3.x - P1.x) * ( P2.x - P1.x ) + ( P3.y - P1.y) * ( P2.y - P1.y ));
	PDiff = P2 - P1;
	fuDenominator = (float)(PDiff.x * PDiff.x + PDiff.y * PDiff.y);
	
	//<Prevent divide by zero>
	if( fuDenominator == 0 )
		return FALSE;

	fu = fuNumerator / fuDenominator;

	//Calculate P.
	P.x = (long)(P1.x + fu * ( P2.x - P1.x ));
	P.y = (long)(P1.y + fu * ( P2.y - P1.y ));

	//The above algorithm tests for hitting a line. we make the following
	//calculations for reducing the hit test to a segment (P1,P2) within the
	//line.
	
	fLengthSeg =  (float)sqrt( pow( P1.x - P2.x, 2 ) + pow( P1.y - P2.y, 2 ) );
	fdistanceP1 = (float)sqrt( pow( P1.x - P.x, 2 ) + pow( P1.y - P.y, 2 ) );
	fdistanceP2 = (float)sqrt( pow( P2.x - P.x, 2 ) + pow( P2.y - P.y, 2 ) );

	if( (int)(fdistanceP1 + fdistanceP2 ) != (int)fLengthSeg )
		return FALSE;
	
	//if the distance between the hit-point and the closest point on the line to the 
	//hit-point is within the hit-tolerance, then we can count it as a hit.
	fdistance = (float)sqrt( pow( P3.x - P.x, 2 ) + pow( P3.y - P.y, 2 ) );
	if( fdistance <= m_HitTolerance )
		return TRUE;
	
	return FALSE;	
}

UINT CILJoint::DoModalPropBox( CWnd *pWnd, CDialog *pExtdDlg )
{
	//Only control joints can manipulate properties of an edge.
	if( m_SymbiantList.IsEmpty() )
		return IDCANCEL;

	CEdgePropBox EdgePropBox;
	UINT retID;
	CString strBlock( TEXT("Block") );
	CString strDot( TEXT("Dot") );
	CString strArrow( TEXT("Arrow") );
	CString strAggregation( TEXT( "Aggregation" ) );
	CString strOne( TEXT( "One" ) );
	CString strMany( TEXT( "Many" ) );
	CString strNone( TEXT( "None" ) );
	CString strPlus( TEXT( "Plus" ) );
	CString strMinus( TEXT( "Minus" ) );
	CString strDep( TEXT( "Dependent" ) );
	
	EdgePropBox.m_strlstSymbols.AddTail( strBlock );
	EdgePropBox.m_strlstSymbols.AddTail( strArrow );
	EdgePropBox.m_strlstSymbols.AddTail( strAggregation );
	EdgePropBox.m_strlstSymbols.AddTail( strDot );
	EdgePropBox.m_strlstSymbols.AddTail( strOne );
	EdgePropBox.m_strlstSymbols.AddTail( strMany );
	EdgePropBox.m_strlstSymbols.AddTail( strNone );
	//EdgePropBox.m_strlstSymbols.AddTail( strPlus );
	//EdgePropBox.m_strlstSymbols.AddTail( strMinus );
	EdgePropBox.m_strlstSymbols.AddTail( strDep );
	

	EdgePropBox.m_strSelMasterSymbol = _JointSymbolToString( m_JointSymbol );
	EdgePropBox.m_strSelSlaveSymbol = _JointSymbolToString( m_SymbiantJointSymbol );
	EdgePropBox.m_strDefMasterSymbol = _JointSymbolToString( ILJOINT_DEFAULT_MASTERSYMBOL );
	EdgePropBox.m_strDefSlaveSymbol = _JointSymbolToString( ILJOINT_DEFAULT_SLAVESYMBOL );

	for( int i = 1; i <= 3; i++ )
		EdgePropBox.m_lstEdgeWidths.AddTail( i );
	
	EdgePropBox.m_nCurEdgeWidth = m_nJointEdgeWidth;
	EdgePropBox.m_btnEdgeClr.m_dwButtonColor =  m_dwJointEdgeColor;
	EdgePropBox.m_btnJointClr.m_dwButtonColor = m_dwJointBkgndColor;
	EdgePropBox.m_nEdgeType = ( m_uEdgeStyle == PS_SOLID ) ? 0 : 1;

	EdgePropBox.m_nDefEdgeWidth = ILJOINT_DEFAULT_EDGEWIDTH;
	EdgePropBox.m_dwDefJointBkgndClr = ILJOINT_DEFAULT_BKGNDCLR;
	EdgePropBox.m_dwDefJointEdgeClr = ILJOINT_DEFAULT_EDGECLR;
	EdgePropBox.m_uDefEdgeStyle = ILJOINT_DEFAULT_EDGESTYLE;

	if( IDOK == ( retID = EdgePropBox.DoModal() ) )
	{
		m_JointSymbol = _StringToJointSymbol( EdgePropBox.m_strSelMasterSymbol );
		m_SymbiantJointSymbol = _StringToJointSymbol( EdgePropBox.m_strSelSlaveSymbol );

		m_nJointEdgeWidth = EdgePropBox.m_nCurEdgeWidth;
		m_dwJointEdgeColor = EdgePropBox.m_btnEdgeClr.m_dwButtonColor;
		m_dwJointBkgndColor = EdgePropBox.m_btnJointClr.m_dwButtonColor;

		if( EdgePropBox.m_nCurEdgeWidth == 1 )
			m_uEdgeStyle = ( EdgePropBox.m_nEdgeType == 0 ) ? PS_SOLID : PS_DASH;

		if( EdgePropBox.m_bMakeCurrent )
		{
			m_nDefJointEdgeWidth = EdgePropBox.m_nCurEdgeWidth;
			m_dwDefJointBkgndColor = EdgePropBox.m_btnJointClr.m_dwButtonColor;
			m_dwDefJointEdgeColor = EdgePropBox.m_btnEdgeClr.m_dwButtonColor;
			m_DefJointSymbol = _StringToJointSymbol( EdgePropBox.m_strSelMasterSymbol );
			m_DefSymbiantJointSymbol = _StringToJointSymbol( EdgePropBox.m_strSelSlaveSymbol );

			if( EdgePropBox.m_nCurEdgeWidth == 1 )
				m_uDefEdgeStyle = ( EdgePropBox.m_nEdgeType == 0 ) ? PS_SOLID : PS_DASH;
		}
	}	
	
	return retID;
}

void CILJoint::AxisAlign( CILJoint *pJoint )
{
	//if this joint is anchored to the top or bottom of shape, we'll align the x coord,
	//and leave the y coord. 
	if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_TOP | JOINTDATA_BOTTOM, FALSE ) )
		SetOrigin( CPoint( pJoint->GetOrigin().x, GetOrigin().y ) ); 
	
	//else if this joint is anchored to the left or right of a shape, we'll align the y coord,
	//and leave the x coord.
	else if( CILBitOp::IsOn( m_dwExtraJointData, JOINTDATA_LEFT | JOINTDATA_RIGHT, FALSE ) )
		SetOrigin( CPoint(GetOrigin().x, pJoint->GetOrigin().y ) );
		
	//else, this joint is not aligned to any shape! So we do nothing.
}

void CILJoint::_CopyBaseData( CILSymbol **ppSymbol )
{	
	CILShape::_CopyBaseData( ppSymbol );

	((CILJoint*)(*ppSymbol))->m_BoundPadding = m_BoundPadding;
	((CILJoint*)(*ppSymbol))->m_nEdgeStyle = m_nEdgeStyle;
	((CILJoint*)(*ppSymbol))->m_uPointSize = m_uPointSize;
	((CILJoint*)(*ppSymbol))->m_dwExtraJointData = m_dwExtraJointData;
	((CILJoint*)(*ppSymbol))->m_pControlJoint = m_pControlJoint;
	((CILJoint*)(*ppSymbol))->m_bEdgeSelection = m_bEdgeSelection;
	((CILJoint*)(*ppSymbol))->m_dwSelectionWidth = m_dwSelectionWidth;
	((CILJoint*)(*ppSymbol))->m_dwSelectionColor = m_dwSelectionColor;
	((CILJoint*)(*ppSymbol))->m_HitTolerance = m_HitTolerance;
	((CILJoint*)(*ppSymbol))->m_JointSymbol = m_JointSymbol;
	((CILJoint*)(*ppSymbol))->m_nJointEdgeWidth = m_nJointEdgeWidth;
	((CILJoint*)(*ppSymbol))->m_dwJointEdgeColor = m_dwJointEdgeColor;
	((CILJoint*)(*ppSymbol))->m_dwJointBkgndColor = m_dwJointBkgndColor;
	((CILJoint*)(*ppSymbol))->m_SymbiantJointSymbol = m_SymbiantJointSymbol;
	((CILJoint*)(*ppSymbol))->m_uEdgeStyle = m_uEdgeStyle;

	((CILJoint*)(*ppSymbol))->m_SymbiantList.RemoveAll();
	ILPOSITION Pos = m_SymbiantList.GetHeadPosition();
	while( Pos )
	{
		((CILJoint*)(*ppSymbol))->m_SymbiantList.AddTail( m_SymbiantList.GetAt( Pos ) );
		m_SymbiantList.GetNext( Pos );
	}
}

void CILJoint::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILJoint; }

enum CILJoint::JOINTSYMBOL CILJoint::_StringToJointSymbol( CString& string )
{

	CString strBlock( TEXT("Block") );
	CString strDot( TEXT("Dot") );
	CString strArrow( TEXT("Arrow") );
	CString strAggregation( TEXT( "Aggregation" ) );
	CString strOne( TEXT( "One" ) );
	CString strMany( TEXT( "Many" ) );
	CString strNone( TEXT( "None" ) );
	CString strPlus( TEXT( "Plus" ) );
	CString strMinus( TEXT( "Minus" ) );
	CString strDep( TEXT( "Dependent" ) );

	if( string == strBlock )
		return jointBlock;

	else if( string == strDot )
		return jointDot;

	else if( string == strArrow )
		return jointArrow;

	else if( string == strAggregation )
		return jointAggregation;

	else if( string == strOne )
		return jointOne;

	else if( string == strMany )
		return jointMany;

	else if( string == strNone )
		return jointNone;

	else if( string == strPlus )
		return jointPlus;

	else if( string == strMinus )
		return jointMinus;

	else if( string == strDep )
		return jointDep;

	else
		return jointNIL;	
}

CString CILJoint::_JointSymbolToString( JOINTSYMBOL JointSymbol )
{
	CString strBlock( TEXT("Block") );
	CString strDot( TEXT("Dot") );
	CString strArrow( TEXT("Arrow") );
	CString strAggregation( TEXT( "Aggregation" ) );
	CString strOne( TEXT( "One" ) );
	CString strMany( TEXT( "Many" ) );
	CString strNone( TEXT( "None" ) );
	CString strPlus( TEXT( "Plus" ) );
	CString strMinus( TEXT( "Minus" ) );
	CString strDep( TEXT( "Dependent" ) );
	
	switch( JointSymbol )
	{
	case jointBlock:
		return strBlock;
	
	case jointDot:
		return strDot;
	
	case jointArrow:
		return strArrow;
	
	case jointAggregation:
		return strAggregation;
	
	case jointOne:
		return strOne;

	case jointMany:
		return strMany;

	case jointNone:
		return strNone;

	case jointPlus:
		return strPlus;

	case jointMinus:
		return strMinus;

	case jointDep:
		return strDep;

	default:
		return _T("");
	}
}

void CILJoint::Serialize( CArchive &ar )
{
	CILShape::Serialize( ar );

	if( ar.IsStoring() )
	{
		ar << m_nEdgeStyle;
		ar << m_BoundPadding;
		ar << m_uPointSize;
		ar << m_dwExtraJointData;
		ar << m_bEdgeSelection;
		ar << m_dwSelectionWidth;
		ar << m_dwSelectionColor;
		ar << m_HitTolerance;
		
		ar << m_nJointEdgeWidth;
		ar << m_dwJointEdgeColor;
		ar << m_dwJointBkgndColor;
		ar << (int)m_JointSymbol;
		ar << (int)m_SymbiantJointSymbol;
		ar << m_uEdgeStyle;
	}

	else
	{
		ar >> m_nEdgeStyle;
		ar >> m_BoundPadding;
		ar >> m_uPointSize;
		ar >> m_dwExtraJointData;
		ar >> m_bEdgeSelection;
		ar >> m_dwSelectionWidth;
		ar >> m_dwSelectionColor;
		ar >> m_HitTolerance;
		
		ar >> m_nJointEdgeWidth;
		ar >> m_dwJointEdgeColor;
		ar >> m_dwJointBkgndColor;
		ar >> (int&)m_JointSymbol;
		ar >> (int&)m_SymbiantJointSymbol;
		ar >> m_uEdgeStyle;			
	}
} 
