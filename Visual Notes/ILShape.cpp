//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/23/01
//*********************************************

#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "ColorButton.h"
#include "ILColorPicker.h"
#include "ILShapePropBox.h"

IMPLEMENT_DYNAMIC( CILShape, CILSymbol );

#define ILSHAPE_DEFAULT_EDGECLR			0
#define ILSHAPE_DEFAULT_BKGNDCLR		( RGB(16,188,156) )
#define ILSHAPE_DEFAULT_SECONDARYCLR	( RGB(72,193,203) )
#define ILSHAPE_DEFAULT_CAPTIONCLR		0
#define ILSHAPE_DEFAULT_PENSTYLE		PS_SOLID

UINT CILShape::m_uDefEdgeWidth = 1;
DWORD CILShape::m_dwDefEdgeColor = ILSHAPE_DEFAULT_EDGECLR;
DWORD CILShape::m_dwDefBGColor = ILSHAPE_DEFAULT_BKGNDCLR;
DWORD CILShape::m_dwDefSecondaryColor = ILSHAPE_DEFAULT_SECONDARYCLR;
DWORD CILShape::m_dwDefCaptionColor = ILSHAPE_DEFAULT_CAPTIONCLR;
UINT CILShape::m_uDefPenStyle =		ILSHAPE_DEFAULT_PENSTYLE;

CILShape::CILShape() : m_uEdgeWidth(m_uDefEdgeWidth), m_dwEdgeColor(m_dwDefEdgeColor),
					   m_dwBGColor( m_dwDefBGColor ), m_dwCaptionColor( m_dwDefCaptionColor),
					   m_dwSecondaryColor( m_dwDefSecondaryColor ),  m_SelectPadding(5),
					   m_uPenStyle(m_uDefPenStyle)
{}

CILShape::CILShape( UINT l, UINT t, UINT r, UINT b) : m_uEdgeWidth(m_uDefEdgeWidth), 
													  m_dwEdgeColor(m_dwDefEdgeColor), 
													  m_dwCaptionColor( m_dwDefCaptionColor ),
													  m_dwSecondaryColor( m_dwDefSecondaryColor ),
													  m_dwBGColor( m_dwDefBGColor ), 
													  m_SelectPadding(5), m_uPenStyle(m_uDefPenStyle)
													 
{ SetBoundingRect( CRect(l,t,r,b) ); }	

UINT CILShape::DoModalPropBox( CWnd *pParent, CDialog *pExtdDlg )
{
	UINT retVal = 0;
	CShapePropBox ShapePropBox( pParent );

	//{{READ ONLY DATA.
	ShapePropBox.m_uSymbolHeight = m_BoundingRect.Height();
	ShapePropBox.m_uSymbolWidth = m_BoundingRect.Width();
	ShapePropBox.m_ShapePropBox2.m_strNoteFile = m_strNoteLink;
	ShapePropBox.m_ShapePropBox2.m_nDiagramID = m_EmbeddedID;
	//}}READ ONLY DATA.

	ShapePropBox.m_dwEdgeClr = m_dwEdgeColor;
	ShapePropBox.m_dwBkgndClr = m_dwBGColor;
	ShapePropBox.m_dwCaptionClr = m_dwCaptionColor;
	ShapePropBox.m_dwSecondaryClr = m_dwSecondaryColor;
	ShapePropBox.m_uPenStyle = m_uPenStyle;

	ShapePropBox.m_dwDefEdgeClr = ILSHAPE_DEFAULT_EDGECLR;
	ShapePropBox.m_dwDefBkgndClr = ILSHAPE_DEFAULT_BKGNDCLR;
	ShapePropBox.m_dwDefSecondaryClr = ILSHAPE_DEFAULT_SECONDARYCLR;
	ShapePropBox.m_dwDefCaptionClr = ILSHAPE_DEFAULT_CAPTIONCLR;
	ShapePropBox.m_uDefPenStyle = ILSHAPE_DEFAULT_PENSTYLE;

	ShapePropBox.m_ShapePropBox2.m_strCaption = m_strCaption;

	ShapePropBox.m_pExtdDlg = pExtdDlg;
	
	if( (retVal = ShapePropBox.DoModal()) == IDOK )
	{
		m_dwEdgeColor=	ShapePropBox.m_dwEdgeClr;
		m_dwBGColor=	ShapePropBox.m_dwBkgndClr;
		m_dwCaptionColor = ShapePropBox.m_dwCaptionClr;
		m_dwSecondaryColor = ShapePropBox.m_dwSecondaryClr;
		m_strCaption = ShapePropBox.m_ShapePropBox2.m_strCaption;
		
		//1 is the group index of "solid" radio btn.
		m_uPenStyle = ( ShapePropBox.m_nDashedEdge == 1 ) ? PS_SOLID : PS_DASH;
		
		if( ShapePropBox.m_bMakeCurrent )
		{
			m_dwDefEdgeColor = ShapePropBox.m_dwEdgeClr;
			m_dwDefBGColor = ShapePropBox.m_dwBkgndClr;
			m_dwDefCaptionColor = ShapePropBox.m_dwCaptionClr;
			m_dwDefSecondaryColor = ShapePropBox.m_dwSecondaryClr;
			m_uDefPenStyle = ShapePropBox.m_uDefPenStyle;
		}
	}	

	return retVal;
}

BOOL CILShape::HitTest( CPoint CanvasPoint )
{
	if( m_BoundingRect.PtInRect( CanvasPoint ) )
		return TRUE;
	
	return FALSE;
}

BOOL CILShape::Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList, 
					  CILList<CILSymbol*,CILSymbol*> *pJointList )
{	
	BOOL retVal = FALSE;
	CRect PrevRect;
	
	PrevRect = m_BoundingRect;
	ShowTracker( TRUE );
	m_Tracker.Track( pWnd, WindowPoint );

	if( !PrevRect.EqualRect( m_BoundingRect ) )
	{ 
		//when tracking we need to adjust selection rectangle since
		//we may have altered the symbol's size.
		m_SelectRect = m_BoundingRect;
		m_SelectRect.InflateRect( m_SelectPadding, m_SelectPadding );

		//only move children if there was unilateral tracking ( no resizing ).
		if( PrevRect.Width() == m_BoundingRect.Width()  && PrevRect.Height() == m_BoundingRect.Height() && pXYChildList )
		{
			ILPOSITION Pos = pXYChildList->GetHeadPosition();
			while( Pos )
			{
				CRect NewRect = pXYChildList->GetAt( Pos )->m_BoundingRect;
				NewRect.OffsetRect( m_BoundingRect.left - PrevRect.left, m_BoundingRect.top - PrevRect.top  );
				pXYChildList->GetAt( Pos )->SetBoundingRect( NewRect );
				pXYChildList->GetNext( Pos );
			}			
		}

		if( pJointList )
		{
			CSize TLOffset( m_BoundingRect.left - PrevRect.left, m_BoundingRect.top - PrevRect.top  );
			CSize BROffset( m_BoundingRect.right - PrevRect.right, m_BoundingRect.bottom - PrevRect.bottom  );
			_UpdateJoints( pJointList, TLOffset, BROffset );
		}

		//let caller know if there has been a change in the rectangle's size.
		retVal = TRUE;
	}

	return retVal;
}

void CILShape::ShowTracker(  BOOL bShow, CWnd *pWnd )
{
	m_bShowTracker = bShow;

	if( !bShow && pWnd )
		m_Tracker.ClearTracker( pWnd );
}

BOOL CILShape::IsTracked()
{ return m_bShowTracker; }

BOOL CILShape::TrackerHitTest( CPoint CanvasPoint )
{   
	//** NOTE: **// 
	//We hit test with canvas coords since m_rect is defined in
	//canvas space.
	return m_Tracker.HitTest( CanvasPoint ); 
}

void CILShape::ShowSelection(  BOOL bShow, CWnd *pWnd )
{
	m_bShowSelection = bShow;
	
	if( !bShow && pWnd )
		m_SelectRect.Clear(pWnd);

	else if( bShow )
		m_SelectRect.Draw( pWnd->GetDC(), PS_DASH, RGB(50,50,50) );
}

void CILShape::Highlight( BOOL bHighlight, CWnd *pWnd, int PenStyle, DWORD dwColor )
{
	m_bHighlight = bHighlight;

	if( !bHighlight)
		m_SelectRect.Clear( pWnd );

	else 
		m_SelectRect.Draw( pWnd->GetDC(), PenStyle, dwColor );
}

BOOL CILShape::IsSelected()
{ return m_bShowSelection; }

void CILShape::InvalidateSymbol( CWnd *pWnd )
{
	CRect rectClear;

	if( m_bShowTracker )
	{
		m_Tracker.GetTrueRect( &rectClear );
		rectClear.OffsetRect( m_CanvasOrigin );
	}

	else if( m_bShowSelection || m_bHighlight )
	{
		rectClear = m_SelectRect;
		rectClear.OffsetRect( m_CanvasOrigin );
	}

	else
	{
		rectClear = m_BoundingRect;
		rectClear.OffsetRect( m_CanvasOrigin );
	}

	pWnd->InvalidateRect( &rectClear );
}

void CILShape::DrawSymbol( CDC *pDC )
{
	if( m_bShowTracker )
		m_Tracker.DrawTracker( pDC );	
	
	else if( m_bShowSelection )
		m_SelectRect.Draw( pDC, PS_DASH, RGB(50,50,50) );
}

void CILShape::SetBoundingRect( CRect rect )
{
	CILSymbol::SetBoundingRect( rect );
	m_Tracker.AssociateRect( &m_BoundingRect );
	m_SelectRect = m_BoundingRect;
	m_SelectRect.InflateRect( m_SelectPadding, m_SelectPadding );
}

void CILShape::SetCanvasOrigin( CPoint CanvasOrigin )
{
	CILSymbol::SetCanvasOrigin( CanvasOrigin );
	m_Tracker.m_DeviceOrigin = m_CanvasOrigin;
	m_SelectRect.m_DeviceOrigin = m_CanvasOrigin;
}

void CILShape::Resize( CRect NewRect, CILList<CILSymbol*,CILSymbol*> *pJointList )
{
	CRect OldRect = m_BoundingRect;
	SetBoundingRect( NewRect );

	if( pJointList )
	{
		CSize TLOffset( m_BoundingRect.left - OldRect.left, m_BoundingRect.top - OldRect.top  );
		CSize BROffset( m_BoundingRect.right - OldRect.right, m_BoundingRect.bottom - OldRect.bottom  );
		CILSymbol::_UpdateJoints( pJointList, TLOffset, BROffset );
	}
}

void CILShape::_RenderCaption( CDC *pDC )
{
	int nSavedDC = pDC->SaveDC();

	UINT nAlignFlag = ( m_bCenterCaption ) ? ( DT_CENTER | DT_VCENTER | DT_SINGLELINE ) : ( DT_LEFT | DT_WORDBREAK );
	CRect rectText = m_BoundingRect;
	rectText.DeflateRect( 2, 0 );
	
	rectText.OffsetRect( m_CanvasOrigin );
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( m_strCaption, &rectText, nAlignFlag );	

	pDC->RestoreDC( nSavedDC );
}

void CILShape::_CopyBaseData( CILSymbol **ppSymbol )
{
	CILSymbol::_CopyBaseData( ppSymbol );
	
	CILDragRect StaticSelectRect = m_BoundingRect;
	StaticSelectRect.InflateRect( m_SelectPadding, m_SelectPadding );
	
	((CILShape*)(*ppSymbol))->m_dwBGColor =			m_dwBGColor;
	((CILShape*)(*ppSymbol))->m_dwEdgeColor =		m_dwEdgeColor;
	((CILShape*)(*ppSymbol))->m_dwCaptionColor =	m_dwCaptionColor;
	((CILShape*)(*ppSymbol))->m_dwSecondaryColor =	m_dwSecondaryColor;
	((CILShape*)(*ppSymbol))->m_SelectPadding =		m_SelectPadding;
	((CILShape*)(*ppSymbol))->m_uPenStyle =			m_uPenStyle;
	
	//StaticSelectRect is the unaltered selection rect. Unlike the 
	//m_SelectRect which may currently be offset from this shape via DragMove()
	//the StaticSelectRect will always represents the selection rectangle which
	//encloses this shape.
	((CILShape*)(*ppSymbol))->m_SelectRect = StaticSelectRect;
	((CILShape*)(*ppSymbol))->m_Tracker = m_Tracker;
	//Since *ppSymbol's bounding rectangle has changed, we must reassociate it's tracker
	//with it.
	((CILShape*)(*ppSymbol))->m_Tracker.AssociateRect( &((CILShape*)(*ppSymbol))->m_BoundingRect ); //*2 see below
	((CILShape*)(*ppSymbol))->m_uEdgeWidth = m_uEdgeWidth;
}

void CILShape::Serialize( CArchive &ar )
{
	CILSymbol::Serialize( ar );

	if( ar.IsStoring() )
	{
		ar << m_SelectRect.left;
		ar << m_SelectRect.top;
		ar << m_SelectRect.bottom;
		ar << m_SelectRect.right;
		ar << m_uEdgeWidth;
		ar << m_dwEdgeColor;
		ar << m_dwBGColor;
		ar << m_dwSecondaryColor;
		ar << m_dwCaptionColor;
		ar << m_SelectPadding;
		ar << m_uPenStyle;
	}
	
	else
	{
		ar >> m_SelectRect.left;
		ar >> m_SelectRect.top;
		ar >> m_SelectRect.bottom;
		ar >> m_SelectRect.right;
		ar >> m_uEdgeWidth;
		ar >> m_dwEdgeColor;
		ar >> m_dwBGColor;
		ar >> m_dwSecondaryColor;
		ar >> m_dwCaptionColor;
		ar >> m_SelectPadding;
		ar >> m_uPenStyle;
	}
}


