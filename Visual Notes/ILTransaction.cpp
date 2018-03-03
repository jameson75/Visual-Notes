#include "stdafx.h"
#include "stdil.h"
#include "Visual Notes.h"

extern CVisualNotesApp theApp;

UINT CILTransaction::m_InstanceCount = 0;

void CILTransaction::AppendTransaction( CILTransaction *pTransaction )
{
	ILPOSITION Pos;

	Pos = pTransaction->m_InsertList.GetHeadPosition();
	while( Pos )
	{
		m_InsertList.AddTail( pTransaction->m_InsertList.GetAt( Pos ) );
		pTransaction->m_InsertList.GetNext( Pos );
	}

	Pos = pTransaction->m_RemoveList.GetHeadPosition();
	while( Pos )
	{
		m_RemoveList.AddTail( pTransaction->m_RemoveList.GetAt( Pos ) );
		pTransaction->m_RemoveList.GetNext( Pos );
	}

	Pos = pTransaction->m_OriginalSymbList.GetHeadPosition();
	while( Pos )
	{
		m_OriginalSymbList.AddTail( pTransaction->m_OriginalSymbList.GetAt( Pos ) );
		pTransaction->m_OriginalSymbList.GetNext( Pos );
	}

	Pos = pTransaction->m_OriginalImgList.GetHeadPosition();
	while( Pos )
	{
		m_OriginalImgList.AddTail( pTransaction->m_OriginalImgList.GetAt( Pos ) );
		pTransaction->m_OriginalImgList.GetNext( Pos );
	}

	Pos = pTransaction->m_ReplaceImgList.GetHeadPosition();
	while( Pos )
	{
		m_ReplaceImgList.AddTail( pTransaction->m_ReplaceImgList.GetAt( Pos ) );
		pTransaction->m_ReplaceImgList.GetNext( Pos );
	}
}

void CILTransaction::OnRollBack()
{
	CILSymbolDB *pDB = theApp.GetSymbolDatabase();
	CILList<CILSymbol*,CILSymbol*> ReverseRemoveList, ReverseInsertList,
								 ReverseOrgSymbList, ReverseOrgImgList;

	//All Transaction participants are un-manipulated in reverse order
	///** We do this to gaurantee that children are put back before their 
	///**original parents are put back.
	//=================================================================

	ILPOSITION Pos = m_RemoveList.GetHeadPosition();
	while( Pos )
	{
		ReverseRemoveList.AddHead( m_RemoveList.GetAt( Pos ) );
		m_RemoveList.GetNext( Pos );
	}

	Pos = m_InsertList.GetHeadPosition();
	while( Pos )
	{
		ReverseInsertList.AddHead( m_InsertList.GetAt( Pos ) );
		m_InsertList.GetNext( Pos );
	}

	Pos = m_OriginalSymbList.GetHeadPosition();
	while( Pos )
	{
		ReverseOrgSymbList.AddHead( m_OriginalSymbList.GetAt( Pos ) );
		m_OriginalSymbList.GetNext( Pos );
	}

	Pos = m_OriginalImgList.GetHeadPosition();
	while( Pos )
	{
		ReverseOrgImgList.AddHead( m_OriginalImgList.GetAt( Pos ) );
		m_OriginalImgList.GetNext( Pos );
	}

		
	pDB->Insert( &ReverseRemoveList, NULL );
	pDB->Remove( &ReverseInsertList, theApp.m_bRemoveChildren, NULL );
	pDB->Update( &ReverseOrgSymbList, &ReverseOrgImgList, NULL );
	
}

void CILTransaction::OnRollFoward()
{
	CILSymbolDB *pDB = theApp.GetSymbolDatabase();

	pDB->Insert( &m_InsertList, NULL );
	pDB->Remove( &m_RemoveList, theApp.m_bRemoveChildren, NULL );
	pDB->Update( &m_OriginalSymbList, &m_ReplaceImgList, NULL );
}

void CILTransaction::OnRollBackFinal()
{
	ILPOSITION Pos;
	
	Pos = m_RemoveList.GetHeadPosition();
	while( Pos )
	{
		delete ( m_RemoveList.GetAt( Pos ) );
		m_RemoveList.GetNext( Pos );
	}

	Pos = m_OriginalImgList.GetHeadPosition();
	while( Pos )
	{
		delete ( m_OriginalImgList.GetAt( Pos ) );
		m_OriginalImgList.GetNext( Pos );
	}

	Pos = m_ReplaceImgList.GetHeadPosition();
	while( Pos )
	{
		delete ( m_ReplaceImgList.GetAt( Pos ) );
		m_ReplaceImgList.GetNext( Pos );
	}
}

void CILTransaction::OnRollFowardFinal()
{
	ILPOSITION Pos;
	
	Pos = m_InsertList.GetHeadPosition();
	while( Pos )
	{
		delete ( m_InsertList.GetAt( Pos ) );
		m_InsertList.GetNext( Pos );
	}

	Pos = m_OriginalImgList.GetHeadPosition();
	while( Pos )
	{
		delete ( m_OriginalImgList.GetAt( Pos ) );
		m_OriginalImgList.GetNext( Pos );
	}

	Pos = m_ReplaceImgList.GetHeadPosition();
	while( Pos )
	{
		delete ( m_ReplaceImgList.GetAt( Pos ) );
		m_ReplaceImgList.GetNext( Pos );
	}
}
