#include "stdafx.h"
#include "stdil.h"
#include "ildbset.h"

#include "Visual Notes.h"

extern CVisualNotesApp theApp;

UINT CILDBSet::m_HighID = ILDBSET_ID_FIRST;

CILDBSet::CILDBSet() : m_ID( m_HighID )
					   
{ 
	m_HighID++;
	
	if( m_ID == ILDBSET_ID_FIRST )
		m_strName = TEXT("_root");
	
	else
		m_strName.Format( TEXT("diagram_%ld"), m_ID ); //default diagram name.
}

void CILDBSet::AddLinkedSymbol( CILSymbol *pLinkSymbol )
{m_LinkSymbolList.AddTail( pLinkSymbol ); }

BOOL CILDBSet::RemoveLinkedSymbol( CILSymbol *pLinkSymbol )
{
	ILPOSITION Pos;
	if( !( Pos = m_LinkSymbolList.Find( pLinkSymbol ) ) )
		return FALSE;

	m_LinkSymbolList.RemoveAt( Pos );
	return TRUE;
}

BOOL CILDBSet::GetLinkedSymbols( CILList<CILSymbol*,CILSymbol*> *pSymbolList )
{
	ILPOSITION Pos = m_LinkSymbolList.GetHeadPosition();
	while( Pos )
	{
		pSymbolList->AddTail( m_LinkSymbolList.GetAt( Pos ) );
		m_LinkSymbolList.GetNext( Pos );
	}

	return !pSymbolList->IsEmpty();
}

BOOL CILDBSet::IsOrphan()
{
	CILList<UINT,UINT> VisitList;
	return IsOrphan( &VisitList );
}

BOOL CILDBSet::IsOrphan( CILList<UINT,UINT> *pVisitList )
{
	CILDBSet *pSet = NULL;
	
	//prevent searching in a loop
	if( pVisitList->Find( m_ID ) )
			return TRUE;
	
	//obviously, if there are no link symbols it must be orphaned.
	if( m_LinkSymbolList.IsEmpty() )
		return TRUE;

	ILPOSITION Pos = m_LinkSymbolList.GetHeadPosition();
	while( Pos )
	{
		//if the root diagram directly links to this diagram then this diagram is not an orphan.
		if( m_LinkSymbolList.GetAt( Pos )->GetDiagramID() == ILDBSET_ID_FIRST )
			return FALSE;

		//check to see if root diagram INDIRECTLY links to this diagram by searching each branch of linked symbols.
		//---------------------------------------------------------------------------------------------------------
		pSet = theApp.FindDBSet( m_LinkSymbolList.GetAt( Pos )->GetDiagramID() );
		
		ASSERT( pSet ); //if we are storing a link symbol whose diagram doesn't exist, 
						//something is wrong with the dbset deletion method.
	
		pVisitList->AddTail( m_ID );
		if( !pSet->IsOrphan( pVisitList ) ) //recursively search up the linked symbol branches.
			return FALSE;
		
		m_LinkSymbolList.GetNext( Pos );
	}

	return TRUE;
}




