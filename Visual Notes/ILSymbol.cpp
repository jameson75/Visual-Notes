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

IMPLEMENT_DYNAMIC( CILSymbol, CObject );

CSize CILSymbol::ms_MinSize = CSize(35,35);
CSize CILSymbol::ms_MaxSize = CSize(100,100);

UINT CILSymbol::m_InstanceCount = 0;
UINT CILSymbol::m_HighID = 0;

CILSymbol::CILSymbol() : m_pParent(NULL), m_bShowTracker(FALSE), m_bShowSelection(FALSE),	
						 m_dwExtraData(0), m_bHighlight(FALSE), m_EmbeddedID(0), m_bCenterCaption( TRUE ),
						 m_bHidden( FALSE ), m_ID( m_HighID )
{ 
	m_CanvasOrigin = CPoint(0,0);
	m_InstanceCount++;
	m_strNoteLink = _T("");
	m_strCaption = _T("");
	m_HighID++;
}

CILSymbol::~CILSymbol()
{
	m_ChildList.RemoveAll();
	m_InstanceCount--;
}


void CILSymbol::AddChild( CILSymbol *pSymbol )
{
	if( !m_ChildList.Find( pSymbol ) )
		m_ChildList.AddTail( pSymbol ); 
}

BOOL CILSymbol::RemoveChild( CILSymbol *pSymbolRemove )
{ 
	ILPOSITION SymbolPos;
	
	SymbolPos = m_ChildList.Find( pSymbolRemove );
	if( SymbolPos )
		m_ChildList.RemoveAt( SymbolPos );

	return (BOOL)SymbolPos;
}

BOOL CILSymbol::IsChild( CILSymbol *pChild, BOOL bCheckAllLevels )
{
	if( m_ChildList.Find( pChild ) )
		return TRUE;

	else if( bCheckAllLevels )
	{
		ILPOSITION ChildPos = m_ChildList.GetHeadPosition();
		while( ChildPos )
		{
			if( m_ChildList.GetAt( ChildPos )->IsChild( pChild, bCheckAllLevels ) )
				return TRUE;
			
			m_ChildList.GetNext( ChildPos );
		}
	}

	return FALSE;
}

CILSymbol* CILSymbol::GetParent()
{ return m_pParent; }

void CILSymbol::SetParent( CILSymbol *pParent )
{ m_pParent = pParent; }

BOOL CILSymbol::GetChildren( CILList<CILSymbol*,CILSymbol*> *pChildList, BOOL bAllChildren )
{
	pChildList->AddTail( &m_ChildList );
	return !m_ChildList.IsEmpty();
}

BOOL CILSymbol::HasChildren()
{ return !m_ChildList.IsEmpty(); }

BOOL CILSymbol::HasJoints()
{ return !m_JointList.IsEmpty(); }

void CILSymbol::SetCanvasOrigin( CPoint ptOrigin )
{ m_CanvasOrigin = ptOrigin; }

void CILSymbol::SetBoundingRect( CRect rect )
{ m_BoundingRect = rect; }

void CILSymbol::GetSnapshot( CILSymbol **ppSnapshot )
{
	CILSymbol *pChild = NULL;
	
	// It's VERY important that these are the first two methods called (in this order).
	// Before anything, the symbol must be instantiated, and then the base data must be
	// copied.
	_New( ppSnapshot );
	_CopyBaseData( ppSnapshot );
	m_HighID--; //we DO NOT want snapshots to use up unique id's.
}

void CILSymbol::SetSnapshot( CILSymbol *pSnapshot )
{
	CILSymbol *pThisSymbol = this;
	pSnapshot->_CopyBaseData( &pThisSymbol );
}	

void CILSymbol::AddJoint( CILSymbol *pJoint )
{
	int dt, dl, dr, db, min;

	ASSERT( pJoint->IsKindOf( RUNTIME_CLASS( CILJoint ) ) );

	if( m_JointList.Find( pJoint ) )
		return;

	//if this joint was already anchored to a shape, no need to anchor it again.
	if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_ANCHORED, FALSE ) )
	{
		m_JointList.AddTail( pJoint );
		return;
	}

	dt = (int)abs( ((CILJoint*)pJoint)->GetOrigin().y - m_BoundingRect.top );
	db = (int)abs( ((CILJoint*)pJoint)->GetOrigin().y - m_BoundingRect.bottom );
	dr = (int)abs( ((CILJoint*)pJoint)->GetOrigin().x - m_BoundingRect.right );
	dl = (int)abs( ((CILJoint*)pJoint)->GetOrigin().x - m_BoundingRect.left );
	
	//if joint does not have a pre determined anchored side. determine which side of the master shape to 
	//anchor the joint to by calculating the side closest to the joint's origin.
	if( CILBitOp::IsOff( ((CILJoint*)pJoint)->m_dwExtraJointData,
						 JOINTDATA_TOP | JOINTDATA_BOTTOM | JOINTDATA_LEFT | JOINTDATA_RIGHT,
						 TRUE ) )
	{
		min = dt;
		if( dl < min ) min = dl;
		if( dr < min ) min = dr;
		if( db < min ) min = db;
	}

	//else make sure that joint get's anchored to the predetermined side of this symbol
	else
	{
		if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_TOP, TRUE ) )
			min = dt;
		else if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_BOTTOM, TRUE ) )
			min = db;
		else if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_LEFT, TRUE ) )
			min = dl;
		else if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_RIGHT, TRUE ) )
			min = dr;
	}

	CPoint ptOrigin( ((CILJoint*)pJoint)->GetOrigin() );
	CRect rectJoint( pJoint->m_BoundingRect );

	//if joint is to be centered relative to this shapes verticle side...
	if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_VCENTER, TRUE ) )
		ptOrigin.y = m_BoundingRect.top + m_BoundingRect.Height() / 2;

	//if joint is to be centered relative to this shapes horizontal side...
	if( CILBitOp::IsOn( ((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_HCENTER, TRUE ) )
		ptOrigin.x = m_BoundingRect.left + m_BoundingRect.Width() / 2;

	//Set new origin of the joint, according to which side it's anchored to.
	if( min == dt )
	{
		CILBitOp::TurnOn( &((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_TOP );
		((CILJoint*)pJoint)->SetOrigin( CPoint( ptOrigin.x, m_BoundingRect.top - rectJoint.Height() / 2 ) );
	}
				
	else if( min == dl )
	{
		CILBitOp::TurnOn( &((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_LEFT );
		((CILJoint*)pJoint)->SetOrigin( CPoint( m_BoundingRect.left - rectJoint.Width() / 2, ptOrigin.y ) );
	}

	else if( min == dr )
	{
		CILBitOp::TurnOn( &((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_RIGHT );
		((CILJoint*)pJoint)->SetOrigin( CPoint( m_BoundingRect.right + rectJoint.Width() / 2, ptOrigin.y ) );
	}
		
	else if( min == db )
	{
		CILBitOp::TurnOn( &((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_BOTTOM );
		((CILJoint*)pJoint)->SetOrigin( CPoint( ptOrigin.x, m_BoundingRect.bottom + rectJoint.Height() / 2 ) );
	}
		
	CILBitOp::TurnOn( &((CILJoint*)pJoint)->m_dwExtraJointData, JOINTDATA_ANCHORED );
	m_JointList.AddTail( pJoint );
}

BOOL CILSymbol::RemoveJoint( CILSymbol *pSymbol )
{
	ILPOSITION JointPos;

	if( !( JointPos = m_JointList.Find( pSymbol ) ) )
		return FALSE;
	
	m_JointList.RemoveAt( JointPos );
	return TRUE;
}

BOOL CILSymbol::GetJoints( CILList<CILSymbol*,CILSymbol*> *pJointList, BOOL bAllJoints )
{	
	CILList<CILSymbol*,CILSymbol*> ChildList;

	ILPOSITION Pos = m_JointList.GetHeadPosition();
	while( Pos )
	{
		pJointList->AddTail( m_JointList.GetAt( Pos ) );
		m_JointList.GetNext( Pos );
	}

	//if user specifies, Enumerate ALL children and for each child, collect ALL
	//their joints.
	if( bAllJoints )
	{
		GetChildren( &ChildList, TRUE );
		Pos = ChildList.GetHeadPosition();
		while( Pos )
		{
			ChildList.GetAt( Pos )->GetJoints( pJointList );
			ChildList.GetNext( Pos );
		}
	}

	return !pJointList->IsEmpty();
}

void CILSymbol::GetNoteLink( CString *pNoteLink )
{	*pNoteLink = m_strNoteLink; }

void CILSymbol::SetNoteLink( CString& NoteLink )
{ m_strNoteLink = NoteLink; }

void CILSymbol::_CopyBaseData( CILSymbol **ppSymbol )
{
	(*ppSymbol)->m_ID			=	m_ID;
	(*ppSymbol)->m_dwExtraData =	m_dwExtraData;
	(*ppSymbol)->m_BoundingRect =	m_BoundingRect;
	(*ppSymbol)->m_bShowSelection = m_bShowSelection;
	(*ppSymbol)->m_bShowTracker =   m_bShowTracker;
	(*ppSymbol)->m_pParent		=   m_pParent;
	(*ppSymbol)->m_bHighlight   =   m_bHighlight;
	(*ppSymbol)->m_DiagramID =		m_DiagramID;
	(*ppSymbol)->m_bCenterCaption = m_bCenterCaption;
	(*ppSymbol)->m_strCaption =		m_strCaption;
		
	//We EXPLICITLY DONOT make thes attributes copiable so that embedded link (or note)
	//removal is a permanent action and its value doesn't change upon redoing and
	//undoing this objects state.
	/*
		(*ppSymbol)->m_EmbeddedID =		m_EmbeddedID;
		(*ppSymbol)->m_strNoteLink =	m_strNoteLink;
	*/

	//Symbol visibility changes are NOT reversable.
	/*
		(*ppSymbol)->m_bHidden =		m_bHidden;
	*/
	
	(*ppSymbol)->m_ChildList.RemoveAll(); //we want the targets list to REPLACED, not appended to.
	(*ppSymbol)->m_ChildList.AddTail( &m_ChildList );
	/* OLD WAY OF COPYING...
	ILPOSITION Pos = m_ChildList.GetHeadPosition();
	while( Pos )
	{
		(*ppSymbol)->m_ChildList.AddTail( m_ChildList.GetAt( Pos ) );
		m_ChildList.GetNext( Pos );
	}
	*/
	
	(*ppSymbol)->m_JointList.RemoveAll();	//we want the targets list to REPLACED, not appended to.
	(*ppSymbol)->m_JointList.AddTail( &m_JointList );
	/* OLD WAY OF COPYING...
	Pos = m_JointList.GetHeadPosition();
	while( Pos )
	{
		(*ppSymbol)->m_JointList.AddTail( m_JointList.GetAt( Pos ) );
		m_JointList.GetNext( Pos );
	}
	*/
}

void CILSymbol::_UpdateJoints( CILSymbol *pJoint, CSize TLOffset, CSize BROffset )
{
	CILList<CILSymbol*,CILSymbol*> JointList;
	JointList.AddTail( pJoint );
	_UpdateJoints( &JointList, TLOffset, BROffset );
	JointList.RemoveAll();
}

void CILSymbol::_UpdateJoints( CILList<CILSymbol*,CILSymbol*> *pUpdateJoints, CSize TLOffset, CSize BROffset )
{
	CILJoint *pJoint = NULL;
	ILPOSITION Pos = pUpdateJoints->GetHeadPosition();
	CPoint ptOrigin;
	BOOL bUniLateral = ( TLOffset == BROffset );
		
	while( Pos )
	{
		pJoint = (CILJoint*)pUpdateJoints->GetAt( Pos );

		//if only one of the sides of the boundingrect was tracked, offset each anchored joint ONLY if it's side was adjusted.
		if( !bUniLateral )
		{
			ptOrigin = pJoint->GetOrigin();

			if( CILBitOp::IsOn( pJoint->m_dwExtraJointData, JOINTDATA_TOP, TRUE ) )
				pJoint->SetOrigin( CPoint( ptOrigin.x, ptOrigin.y + TLOffset.cy ) );
								
			if( CILBitOp::IsOn( pJoint->m_dwExtraJointData, JOINTDATA_LEFT, TRUE ) )
				pJoint->SetOrigin( CPoint( ptOrigin.x + TLOffset.cx, ptOrigin.y ) );
		
			if( CILBitOp::IsOn( pJoint->m_dwExtraJointData, JOINTDATA_RIGHT, TRUE ) )
				pJoint->SetOrigin( CPoint( ptOrigin.x + BROffset.cx, ptOrigin.y ) );
							
			if( CILBitOp::IsOn( pJoint->m_dwExtraJointData, JOINTDATA_BOTTOM,TRUE ) )
				pJoint->SetOrigin( CPoint( ptOrigin.x, ptOrigin.y + BROffset.cy) );	
		}

		//else if this shape was tracked unilaterally, we can just offset the joint by the offset of any one of the
		//corners of this symbols bounding rect. ( we'll arbitrally choose the top,left corner ).
		else
		{
			CRect NewRect = pJoint->m_BoundingRect;
			NewRect.OffsetRect( TLOffset );
			pJoint->SetBoundingRect( NewRect );			
		}
		
		pUpdateJoints->GetNext( Pos );
	}
}

void CILSymbol::Serialize( CArchive &ar )
{
	if( ar.IsStoring() )
	{	
		ar << m_ID;
		ar << m_bShowTracker;
		ar << m_bShowSelection;
		ar << m_bHighlight;
		ar << m_strNoteLink;
		ar << m_EmbeddedID;
		ar << m_DiagramID;
		ar << m_strCaption;
		ar << m_bCenterCaption;
		ar << m_bHidden;
		ar << m_dwExtraData;
		ar << m_BoundingRect.left;
		ar << m_BoundingRect.top;
		ar << m_BoundingRect.right;
		ar << m_BoundingRect.bottom;
	}

	else
	{
		ar >> m_ID;
		ar >> m_bShowTracker;
		ar >> m_bShowSelection;
		ar >> m_bHighlight;
		ar >> m_strNoteLink;
		ar >> m_EmbeddedID;
		ar >> m_DiagramID;
		ar >> m_strCaption;
		ar >> m_bCenterCaption;
		ar >> m_bHidden;
		ar >> m_dwExtraData;
		ar >> m_BoundingRect.left;
		ar >> m_BoundingRect.top;
		ar >> m_BoundingRect.right;
		ar >> m_BoundingRect.bottom;

		m_dwExtraData = 0; //we currently ignore tempdata for this version.
	}
}






	

	


