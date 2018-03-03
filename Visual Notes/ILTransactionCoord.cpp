#include "stdafx.h"
#include "stdil.h"

CILTransactionStacks::CILTransactionStacks() : m_MaxStackSize(10)
{}

CILTransactionStacks::~CILTransactionStacks()
{ 
	ClearStack( stackRollBack );
	ClearStack( stackRollFoward );
}

void CILTransactionStacks::_PushRollBack( CILTransaction *pTopAction )
{
	m_RollBackStack.AddHead( pTopAction );

	//We're implementing a bottom-out stack (when stack reaches max size, the bottom
	//item falls out).
	if( m_RollBackStack.GetCount() > m_MaxStackSize )
	{
		//if we are permanently removing an action, we will need to free the 
		//resources allocated to this action.
		CILTransaction *pBottomAction = m_RollBackStack.GetTail();
		pBottomAction->OnRollBackFinal();
		delete pBottomAction;
		m_RollBackStack.RemoveTail();
	}
}

void CILTransactionStacks::_PushRollFoward( CILTransaction *pTopAction )
{
	m_RollFowardStack.AddHead( pTopAction );

	//We're implementing a bottom-out stack (when stack reaches max size, the bottom
	//item falls out).

	if( m_RollFowardStack.GetCount() > m_MaxStackSize )
	{
		//if we are permanently removing an action, we will need to free the 
		//resources allocated to this action.
		CILTransaction *pBottomAction = m_RollFowardStack.GetTail();
		pBottomAction->OnRollFowardFinal();
		delete pBottomAction;
		m_RollFowardStack.RemoveTail();
	}
}

BOOL CILTransactionStacks::_PopRollBack( CILTransaction **ppTopAction )
{
	if( m_RollBackStack.IsEmpty() )
	{
		*ppTopAction = NULL;
		return FALSE;
	}
	
	*ppTopAction = m_RollBackStack.GetHead();
	m_RollBackStack.RemoveHead();

	return TRUE;
}

BOOL CILTransactionStacks::_PopRollFoward( CILTransaction **ppTopAction )
{
	if( m_RollFowardStack.IsEmpty() )
	{
		*ppTopAction = NULL;
		return FALSE;
	}
	
	*ppTopAction = m_RollFowardStack.GetHead();
	m_RollFowardStack.RemoveHead();

	return TRUE;
}

void CILTransactionStacks::ClearStack( STACKTYPE ClearStack )
{
	CILTransaction *pDelAction = NULL;
	
	if( ClearStack == stackRollFoward )
	{
		while( _PopRollFoward( &pDelAction ) )
		{
			pDelAction->OnRollFowardFinal();
			delete pDelAction;
		}
	}

	else if( ClearStack == stackRollBack )
	{
		while( _PopRollBack( &pDelAction ) )
		{
			pDelAction->OnRollBackFinal();
			delete pDelAction;
		}
	}
}

//-----------------------------------------------------------------------------------------

void CILTransactionCoord::Commit( CILTransaction *pAction )
{
	CILTransaction *pRollFowardAction = NULL;
	
	//Every new registered action, clears the redo stack.
	//NOTE: Because of this response to new actions, the RollFoward stack should NEVER bottom out.
	ClearStack( stackRollFoward );
	_PushRollBack( pAction );
}

void CILTransactionCoord::RollBack( int nTransactions )
{
	CILTransaction *pRollBackAction = NULL;
	
	while( nTransactions )
	{
		if( _PopRollBack( &pRollBackAction ) )
		{
			pRollBackAction->OnRollBack();
			_PushRollFoward( pRollBackAction );
		}

		nTransactions--;
	}
}

void CILTransactionCoord::RollFoward( int nTransactions )
{
	CILTransaction *pRollFowardAction = NULL;

	while( nTransactions )
	{
		if( _PopRollFoward( &pRollFowardAction ) )
		{
			pRollFowardAction->OnRollFoward();
			_PushRollBack( pRollFowardAction );
		}

		nTransactions--;
	}
}

void CILTransactionCoord::Collapse( int nTransactions )
{
	CILTransaction *pRollBackAction = NULL;
	CILTransaction *pCollapsedTransaction;
	CILList<CILTransaction*,CILTransaction*> CollapseList;

	while( nTransactions )
	{
		//if the user is trying to undo more actions than is on the RollBack
		//stack, make no more attempts.
		if( !_PopRollBack( &pRollBackAction )  )
			break;

		//record popped transactions in reverse order.
		CollapseList.AddHead( pRollBackAction );
		
		pRollBackAction->OnRollBackFinal();
		delete pRollBackAction; pRollBackAction = NULL;
		nTransactions--;
	}

	//combine any collapsed transactions.
	if( CollapseList.GetCount() )
	{
		pCollapsedTransaction = new CILTransaction;
		ILPOSITION CollapsePos = CollapseList.GetHeadPosition();
		while( CollapsePos )
		{
			pCollapsedTransaction->AppendTransaction( CollapseList.GetAt( CollapsePos ) );
			CollapseList.GetNext( CollapsePos );
		}
	}
}

void CILTransactionCoord::Disreguard( int nTransactions )
{
	CILTransaction *pRollBackAction = NULL;

	for( int i = 0; i < nTransactions; i++ )
	{
		if( _PopRollBack( &pRollBackAction ) )
		{
			pRollBackAction->OnRollBackFinal();
			delete pRollBackAction;
		}

		else
			break;
	}

	ClearStack( stackRollFoward );
}
//------------------------------------------------------------------------------------------








