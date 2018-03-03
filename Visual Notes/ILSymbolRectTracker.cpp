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


 CILSymbolRectTracker::CILSymbolRectTracker() : m_pAssociatedRect(NULL), m_UniformRectPadding(3)
{
	m_nHandleSize++;
	m_nStyle = resizeOutside | hatchedBorder;
	m_DeviceOrigin = CPoint(0,0);
}

CILSymbolRectTracker::CILSymbolRectTracker( CRect *pRect ) : m_UniformRectPadding(3)
{ 
	m_nHandleSize++;
	m_nStyle = resizeOutside | hatchedBorder;
	m_DeviceOrigin = CPoint(0,0);
	AssociateRect( pRect );
}

void CILSymbolRectTracker::OnChangedRect( const CRect &rOldRect )
{	
	ASSERT( m_pAssociatedRect );

	//NOTE: OnChangedRect is called with "m_rect" in WINDOW coords (Due to the call to Track() method).
	//      The symbol's bounding rect must be set in CANVAS coords. This tranformation is done below.
	m_rect.OffsetRect( -m_DeviceOrigin ); 
	*m_pAssociatedRect =  m_rect; 
	m_rect.OffsetRect( m_DeviceOrigin ); 
}

void CILSymbolRectTracker::AssociateRect( CRect *pRect )
{
	m_pAssociatedRect = pRect;
	m_rect = *pRect;
}	

CRect* CILSymbolRectTracker::GetAssociatedRect()
{ return m_pAssociatedRect; }

void CILSymbolRectTracker::UnassociateRect()
{	m_pAssociatedRect = NULL; }

void CILSymbolRectTracker::DrawTracker( CDC *pDC )
{
	//We only draw the tracker if it is associated with some symbol.
	if( m_pAssociatedRect )
	{
		//The tracker must be drawn in WINDOW coords.
		m_rect.OffsetRect( m_DeviceOrigin );
		Draw( pDC );
		m_rect.OffsetRect( -m_DeviceOrigin ); 
	}
}

void CILSymbolRectTracker::Track( CWnd *pWnd, CPoint WindowPoint )
{
	CRect OldRect, NewRect, MRect, OldMRect;

	//CRectTracker::Track() expects "m_rect" in WINDOW coords.
	m_rect.OffsetRect( m_DeviceOrigin );
	GetTrueRect( &OldRect );
	OldMRect = m_rect;
	CRectTracker::Track( pWnd, WindowPoint );
	GetTrueRect( &NewRect );
	m_rect.OffsetRect( -m_DeviceOrigin );

	pWnd->InvalidateRect( &OldRect );
 	pWnd->InvalidateRect( &NewRect );
}

void CILSymbolRectTracker::ClearTracker( CWnd *pWnd )
{
	if( m_pAssociatedRect )
	{
		CRect TrueTrackerRect, DeflatedMRect;
		
		m_rect.OffsetRect( m_DeviceOrigin ); //we require window coordinates of rect tracker for updating.
		
		//Invalidate ONLY the border and handles, leave the interior ( m_rect ) alone.
		GetTrueRect( &TrueTrackerRect );
		pWnd->InvalidateRect( &TrueTrackerRect );
		DeflatedMRect = m_rect;
		DeflatedMRect.DeflateRect(1,1);
		pWnd->ValidateRect( &DeflatedMRect );
		
		m_rect.OffsetRect( -m_DeviceOrigin ); //set rect tracker back to canvas coordinates.
	}
}

CILSymbolRectTracker& CILSymbolRectTracker::operator = ( CILSymbolRectTracker& rTracker )
{
	*((CRectTracker*)this) = *((CRectTracker*)&rTracker);
	m_UniformRectPadding = rTracker.m_UniformRectPadding;
	m_DeviceOrigin = rTracker.m_DeviceOrigin;
	return *this;
}
