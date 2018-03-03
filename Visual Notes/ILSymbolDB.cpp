#include "stdafx.h"
#include "stdil.h"
#include "Visual Notes.h"
#include "Visual NotesDoc.h"
#include "Visual NotesView.h"
#include "MainFrm.h"

extern CVisualNotesApp theApp;

void CILSymbolDB::Insert( CILSymbol *pInsertSymbol, CILTransaction **ppTransaction )
{ 
	CILList<CILSymbol*,CILSymbol*> List;
	List.AddTail( pInsertSymbol );
	Insert( &List, ppTransaction );
	List.RemoveAll();
}

void CILSymbolDB::Remove( CILSymbol *pDeleteSymbol, BOOL bRemoveChildren, CILTransaction **ppTransaction )
{
	CILList<CILSymbol*,CILSymbol*> List;
	List.AddTail( pDeleteSymbol );
	Remove( &List, bRemoveChildren, ppTransaction );
	List.RemoveAll();
}

void CILSymbolDB::Update( CILSymbol *pOriginalSymbol, CILSymbol *pReplaceSymbol, CILTransaction **ppTransaction )
{
	CILList<CILSymbol*, CILSymbol*> OriginalList, ReplaceList;
	OriginalList.AddTail( pOriginalSymbol );
	ReplaceList.AddTail( pReplaceSymbol );
	Update( &OriginalList, &ReplaceList, ppTransaction );
	OriginalList.RemoveAll();
	ReplaceList.RemoveAll();
}

void CILSymbolDB::Insert( CILList<CILSymbol*,CILSymbol*> *pList, CILTransaction **ppTransaction )
{
	//VERY IMPORTANT: The following call ASSUMES that all items being inserted during this call,
	//				  correspoind to the active view. Therefore, it would be wise to have a database
	//				  object have a one to one relationship with all views.				  
	CVisualNotesView *pView= (CVisualNotesView*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveView();
	CPoint CanvasOrigin(0,0);
	
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );
	
	CILSymbol *pSymbol = NULL, *pParent = NULL, *pChild = NULL, *pOldParent = NULL;
	CILList<CILSymbol*,CILSymbol*> ChildList, JointList;

	ILPOSITION Pos = pList->GetHeadPosition();
	while( Pos )
	{
		pSymbol = pList->GetAt( Pos );
		
		if( pSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
		{
			JointList.AddTail( pSymbol );
			pList->GetNext( Pos );
			continue;
		}

		//if the inserting symbol had a parent it was orphaned from, reparent it.
		if( (pParent = pSymbol->GetParent()) && !ppTransaction )
			pParent->AddChild( pSymbol );
	
		//if the inserting symbol had children, get them back too.
		if( (pSymbol->GetChildren( &ChildList )) && !ppTransaction )
		{			
			ILPOSITION ChildPos = ChildList.GetHeadPosition();
			while( ChildPos )
			{	
				pChild = ChildList.GetAt( ChildPos );
				
				//if any of the inserting symbol's previous children has new parents, 
				//remove the children from their parents.
				if( (pOldParent = pChild->GetParent()) )
					pOldParent->RemoveChild( pChild );
				
				//reunite child and inserting symbol.
				pSymbol->AddChild( pChild );
				pChild->SetParent( pSymbol );

				ChildList.GetNext( ChildPos );
			}

			ChildList.RemoveAll();
		}

		//Now symbol has been reparented and gained back it's children. I can invalidate it now.
		pSymbol->SetCanvasOrigin( CanvasOrigin );
		pSymbol->InvalidateSymbol( pView );
		m_SymbolData.AddTail( pSymbol );

		//if user wishes, save the current symbol as a transaction participant.
		if( ppTransaction )
			(*ppTransaction)->m_InsertList.AddTail( pSymbol );
		else
			ZOrderSymbol( pSymbol ); //This is Okay since a symbols child is garaunteed to be in DB if symbol is in DB.
		
		pOldParent = NULL;
		pParent = NULL; 
		pChild = NULL;		
		pList->GetNext( Pos );
	}	

	_InsertJoints( &JointList, ppTransaction );
	((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument()->SetModifiedFlag();
}

void CILSymbolDB::Remove( CILList<CILSymbol*,CILSymbol*> *pList, BOOL bRemoveChildren, CILTransaction **ppTransaction )
{
	//VERY IMPORTANT: The following call ASSUMES that all items being removed during this call,
	//				  correspoind to the active view. Therefore, it would be wise to have a database
	//				  object have a one to one relationship with all views.				  
	CVisualNotesView *pView= (CVisualNotesView*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveView();
	CPoint CanvasOrigin(0,0);
	
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );

	CILSymbol *pSymbol = NULL;
	CILList<CILSymbol*,CILSymbol*> JointList;
	CILList<CILSymbol*,CILSymbol*> ChildList;
	CILSymbol *pGrandParent = NULL;
	CILSymbol *pChild = NULL;
	CILSymbol *pParent = NULL;

	ILPOSITION Pos = pList->GetHeadPosition();
	while( Pos )
	{
		pSymbol = pList->GetAt( Pos );

		if( pSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
		{
			//if the symbol we are removing has joints, set them for removal only if the
			//user did not already select them for removal.
			if( !JointList.Find( pSymbol ) )
				JointList.AddTail( pSymbol );
			pList->GetNext( Pos );
			continue;
		}
		
		//find symbol we wish to remove.
		ILPOSITION ShapePos = NULL;
		if( ( ShapePos = m_SymbolData.Find( pSymbol ) ) )
		{//BEGIN REMOVE SYMBOL
		
			m_SymbolData.RemoveAt( ShapePos ); //remove symbol from database.
				
			//user wishes to remove the children of this symbol with it, do so if the symbol
			//has children.
			if( bRemoveChildren && pSymbol->GetChildren( &ChildList, TRUE ) )
			{
				ILPOSITION ChildPos = ChildList.GetHeadPosition();
				while( ChildPos )
				{
					//only add child to remove list if user did not already select child for removal.
					if( !pList->Find( ChildList.GetAt( ChildPos ) ) )
						pList->AddTail( ChildList.GetAt( ChildPos ) ); 
					ChildList.GetNext( ChildPos );
				}
			}
				
			//else if the symbol we are removing has children, we need to reparent them.
			else if( pSymbol->GetChildren( &ChildList ) )
			{
				pGrandParent = pSymbol->GetParent();
				ILPOSITION ChildPos = ChildList.GetHeadPosition();
				while( ChildPos )				
				{
					pChild = ChildList.GetAt( ChildPos );
					
					pChild->SetParent( pGrandParent );
					if( pGrandParent )
						pGrandParent->AddChild( pChild );								
					
					ChildList.GetNext( ChildPos );
				}
				
				ChildList.RemoveAll();
			}

			//if the symbol we are removing has a parent, we need to remove symbol from it's parent.
			if( ( pParent = pSymbol->GetParent() ) )
				pParent->RemoveChild( pSymbol );

			pSymbol->GetJoints( pList );
							
			pSymbol->SetCanvasOrigin( CanvasOrigin );
			pSymbol->InvalidateSymbol( pView );
			
			if( ppTransaction )
				(*ppTransaction)->m_RemoveList.AddTail( pSymbol );
		
		}//END REMOVE SYMBOL
		
		pGrandParent = NULL;
		pChild = NULL;
		pList->GetNext( Pos );
	}

	_RemoveJoints( &JointList, ppTransaction );
	((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument()->SetModifiedFlag();
}

void CILSymbolDB::Update( CILList<CILSymbol*,CILSymbol*> *pOriginalList, 
						  CILList<CILSymbol*,CILSymbol*> *pReplaceList, 
						  CILTransaction **ppTransaction )
{
	//VERY IMPORTANT: The following call ASSUMES that all items being inserted during this call,
	//				  correspoind to the active view. Therefore, it would be wise to have a database
	//				  object have a one to one relationship with all views.				  
	CVisualNotesView *pView= (CVisualNotesView*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveView();
	CPoint CanvasOrigin(0,0);
	
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );
	
	CILSymbol *pOriginalSnapshot = NULL;
	CILSymbol *pReplaceSnapshot = NULL;
	CILSymbol *pOriginalSymbol = NULL;
	CILSymbol *pReplaceSymbol = NULL;
	
	ILPOSITION OriginalPos = pOriginalList->GetHeadPosition();
	ILPOSITION ReplacePos = pReplaceList->GetHeadPosition();
	while( OriginalPos )
	{
		pOriginalSymbol = pOriginalList->GetAt( OriginalPos ); //get orginal symbol.
		pReplaceSymbol = pReplaceList->GetAt( ReplacePos );    //get symbol that will replace original.
		
		pOriginalSymbol->SetCanvasOrigin( CanvasOrigin );
		pOriginalSymbol->GetSnapshot( &pOriginalSnapshot ); //copy original symbol.
		pReplaceSymbol->GetSnapshot( &pReplaceSnapshot );    //copy symbol that will replace original.
		pOriginalSymbol->InvalidateSymbol( pView );			 //invalidate original's old area.
		pOriginalSymbol->SetSnapshot( pReplaceSnapshot );	 //change original.
		pOriginalSymbol->SetCanvasOrigin( CanvasOrigin );
		pOriginalSymbol->InvalidateSymbol( pView );			 //invalidate original's new area.
		
		if( ppTransaction )
		{			
			//the following items are saved for the current transaction.
			(*ppTransaction)->m_OriginalSymbList.AddTail( pOriginalSymbol ); //save ptr to original's ADDRESS.
			(*ppTransaction)->m_ReplaceImgList.AddTail( pReplaceSnapshot );  //save copy of replacement's DATA.
			(*ppTransaction)->m_OriginalImgList.AddTail( pOriginalSnapshot );//save copy of original's DATA.
		}

		else  
		{
			delete pOriginalSnapshot; pOriginalSnapshot = NULL;
			delete pReplaceSnapshot; pReplaceSnapshot = NULL;
		}
		
		pOriginalList->GetNext( OriginalPos );
		pReplaceList->GetNext( ReplacePos );
	}

	((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument()->SetModifiedFlag();
}

void CILSymbolDB::DeleteDB()
{
	ILPOSITION Pos = m_SymbolData.GetHeadPosition();
	while( Pos )
	{
		delete m_SymbolData.GetAt( Pos );
		m_SymbolData.GetNext( Pos );
	}

	Pos = m_JointData.GetHeadPosition();
	while( Pos )
	{
		delete m_JointData.GetAt( Pos );
		m_JointData.GetNext( Pos );
	}

	m_JointData.RemoveAll();
	m_SymbolData.RemoveAll(); 
}

CILList<CILSymbol*,CILSymbol*>* CILSymbolDB::GetSymbolTable()
{ return &m_SymbolData; }

CILList<CILSymbol*,CILSymbol*>* CILSymbolDB::GetJointTable()
{ return &m_JointData; }

BOOL CILSymbolDB::QueryAll( CILList<CILSymbol*,CILSymbol*> *pSymbolList )
{

	ILPOSITION ShapePos = m_SymbolData.GetHeadPosition();
	while( ShapePos )
	{
		pSymbolList->AddTail( m_SymbolData.GetAt( ShapePos ) );
		m_SymbolData.GetNext( ShapePos );
	}

	ILPOSITION JointPos = m_JointData.GetHeadPosition();
	while( JointPos )
	{
		pSymbolList->AddTail( m_JointData.GetAt( JointPos ) );
		m_JointData.GetNext( JointPos );
	}

	return !pSymbolList->IsEmpty();
}

BOOL CILSymbolDB::QuerySelected( CILList<CILSymbol*,CILSymbol*> *pSymbolList )
{
	ILPOSITION ShapePos = m_SymbolData.GetHeadPosition();
	while( ShapePos )
	{
		if( m_SymbolData.GetAt( ShapePos )->IsSelected() )
			pSymbolList->AddTail( m_SymbolData.GetAt( ShapePos ) );

		m_SymbolData.GetNext( ShapePos );
	}

	ILPOSITION JointPos = m_JointData.GetHeadPosition();
	while( JointPos )
	{
		if( m_JointData.GetAt( JointPos )->IsSelected() )
			pSymbolList->AddTail( m_SymbolData.GetAt( JointPos ) );
		m_JointData.GetNext( JointPos );
	}

	return !pSymbolList->IsEmpty();
}

BOOL CILSymbolDB::QueryTracked( CILSymbol **ppTrackedSymbol )
{	
	*ppTrackedSymbol = NULL;
	ILPOSITION ShapePos = m_SymbolData.GetHeadPosition();

	while( ShapePos )
	{
		if( m_SymbolData.GetAt( ShapePos )->IsTracked() )
		{
			*ppTrackedSymbol = m_SymbolData.GetAt( ShapePos );
			return TRUE;
		}

		m_SymbolData.GetNext( ShapePos );
	}

	
	ILPOSITION JointPos = m_JointData.GetHeadPosition();
	while( JointPos )
	{
		if( m_JointData.GetAt( JointPos )->IsTracked() )
		{
			*ppTrackedSymbol = m_JointData.GetAt( JointPos );
			return TRUE;
		}

		m_JointData.GetNext( JointPos );
	}

	return FALSE;
}

BOOL CILSymbolDB::QueryFlag( DWORD dwFlag,  CILList<CILSymbol*,CILSymbol*> *pResultList )
{
	CILSymbol *pCurSymbol = NULL;
	
	ILPOSITION ShapePos = m_SymbolData.GetHeadPosition();
	while( ShapePos )
	{
		pCurSymbol = m_SymbolData.GetAt( ShapePos );
		
		if( CILBitOp::IsOn( pCurSymbol->m_dwExtraData, dwFlag, TRUE ) )
			pResultList->AddTail( pCurSymbol );

		m_SymbolData.GetNext( ShapePos );
	}

	ILPOSITION JointPos = m_JointData.GetHeadPosition();
	while( JointPos )
	{
		pCurSymbol = m_JointData.GetAt( JointPos );

		if( CILBitOp::IsOn( pCurSymbol->m_dwExtraData, dwFlag, TRUE ) )
			pResultList->AddTail( pCurSymbol );

		m_JointData.GetNext( JointPos );
	}

	return !pResultList->IsEmpty();
}

BOOL CILSymbolDB::QueryFlag( DWORD dwFlag, CILSymbol **ppSymbol )
{
	CILList<CILSymbol*,CILSymbol*> ResultList;

	if( QueryFlag( dwFlag, &ResultList ) )
	{
		*ppSymbol = ResultList.GetHead();
		ResultList.RemoveAll();
		return TRUE;
	}

	else
	{
		*ppSymbol = NULL;
		return FALSE;
	}
}

BOOL CILSymbolDB::QueryHit( CPoint ptHit, CILSymbol **ppHitSymbol )
{
	CILSymbol *pTestSymbol = NULL;

	//Start hit testing from back to front so that the foremost symbols
	//in the z-order are tested first.
	ILPOSITION ShapePos = m_SymbolData.GetTailPosition();
	while( ShapePos )
	{
		pTestSymbol = m_SymbolData.GetAt( ShapePos );
		if( !pTestSymbol->IsHidden() && pTestSymbol->HitTest( ptHit ) )
		{
			*ppHitSymbol = pTestSymbol;
			return TRUE;
		}

		m_SymbolData.GetPrev( ShapePos );
	}

	ILPOSITION JointPos = m_JointData.GetTailPosition();
	while( JointPos )
	{
		pTestSymbol = m_JointData.GetAt( JointPos );
		if( !pTestSymbol->IsHidden() && pTestSymbol->HitTest( ptHit ) )
		{
			*ppHitSymbol = pTestSymbol;
			return TRUE;
		}

		m_JointData.GetPrev( JointPos );
	}
	
	*ppHitSymbol = NULL;
	return FALSE;
}

BOOL CILSymbolDB::QueryHidden( CILList<CILSymbol*,CILSymbol*> *pSymbolList )
{

	
	ILPOSITION ShapePos = m_SymbolData.GetHeadPosition();
	while( ShapePos )
	{
		if( m_SymbolData.GetAt( ShapePos )->IsHidden() )
			pSymbolList->AddTail( m_SymbolData.GetAt( ShapePos ) );

		m_SymbolData.GetNext( ShapePos );
	}

	ILPOSITION JointPos = m_JointData.GetHeadPosition();
	while( JointPos )
	{
		if( m_JointData.GetAt( JointPos )->IsHidden() )
			pSymbolList->AddTail( m_SymbolData.GetAt( JointPos ) );
		m_JointData.GetNext( JointPos );
	}

	return !pSymbolList->IsEmpty();
}

void CILSymbolDB::ZOrderSymbol( CILSymbol *pSymbol )
{	
	//CILSymbol *pChildSymbol;
	CILList<CILSymbol*,CILSymbol*> ChildList;
	CILList<CILJoint*,CILJoint*> SymbiantList;
	ILPOSITION SymbolPos;	

	if( pSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
	{
		SymbolPos = m_JointData.Find( pSymbol );
		m_JointData.RemoveAt( SymbolPos );
		m_JointData.AddTail( pSymbol );

		((CILJoint*)pSymbol)->GetSymbiantJoints( &SymbiantList );

		if( !SymbiantList.IsEmpty() )
		{
			_ZOrderSymbiants( &SymbiantList );
			SymbiantList.RemoveAll();
		}
	}

	else
	{
		SymbolPos = m_SymbolData.Find( pSymbol );
		m_SymbolData.RemoveAt( SymbolPos );
		m_SymbolData.AddTail( pSymbol ); 
	
		/*
		pSymbol->InitEnumerator();
		while( pSymbol->EnumerateChildren( &pChildSymbol ) )
			ChildList.AddTail( pChildSymbol );	
		*/

		pSymbol->GetChildren( &ChildList );
	

		if( !ChildList.IsEmpty() )
		{
			_ZOrderChildLevels( &ChildList );
			ChildList.RemoveAll();
		}
	}
}

void CILSymbolDB::_ZOrderChildLevels( CILList<CILSymbol*,CILSymbol*> *pRootList )
{
	CILSymbol *pChildSymbol = NULL;
	CILSymbol *pRootSymbol = NULL;
	CILList<CILSymbol*,CILSymbol*> ChildList; 
	BOOL bChildListAvailable = FALSE;
	ILPOSITION ChildPos;

	ILPOSITION ListEnumerator = pRootList->GetHeadPosition();
	while( ListEnumerator ) 
	{
		pRootSymbol = pRootList->GetAt( ListEnumerator );
		ChildPos = m_SymbolData.Find( pRootSymbol );
		m_SymbolData.RemoveAt( ChildPos );
		m_SymbolData.AddTail( pRootSymbol );
		pRootList->GetNext( ListEnumerator );
	}

	ListEnumerator = pRootList->GetHeadPosition();
	while(  ListEnumerator )
	{
		pRootSymbol = pRootList->GetAt( ListEnumerator );
		pRootSymbol->GetChildren( &ChildList );

		/*
		pRootSymbol->InitEnumerator();
		while( pRootSymbol->EnumerateChildren( &pChildSymbol ) )
			ChildList.AddTail( pChildSymbol );
		*/
	
		pRootList->GetNext( ListEnumerator );
	}

	if( !ChildList.IsEmpty() )
	{
		_ZOrderChildLevels( &ChildList );
		ChildList.RemoveAll();
	}
}

void CILSymbolDB::_ZOrderSymbiants( CILList<CILJoint*,CILJoint*> *pSymbiantList )
{
	CILJoint *pSymbiant = NULL;
	CILList<CILJoint*,CILJoint*> NewSymbiantList;
	ILPOSITION JointPos;

	ILPOSITION Pos = pSymbiantList->GetHeadPosition();
	while( Pos )
	{
		pSymbiant = (CILJoint*)pSymbiantList->GetAt( Pos );
		JointPos = m_JointData.Find( pSymbiant );
		m_JointData.RemoveAt( JointPos );
		m_JointData.AddTail( pSymbiant );
		pSymbiantList->GetHeadPosition();

		pSymbiantList->GetNext( Pos );
	}

	Pos = pSymbiantList->GetHeadPosition();
	while( Pos )
	{
		pSymbiant = pSymbiantList->GetAt( Pos );
		pSymbiant->GetSymbiantJoints( &NewSymbiantList );
		pSymbiantList->GetNext( Pos );
	}

	if( !NewSymbiantList.IsEmpty() )
	{
		_ZOrderSymbiants( &NewSymbiantList );
		NewSymbiantList.RemoveAll();
	}
}

void CILSymbolDB::_InsertJoints( CILList<CILSymbol*,CILSymbol*> *pInsertList, CILTransaction **ppTransaction )
{
	CILJoint *pJoint = NULL;
	CILJoint *pNextJoint = NULL, *pPrevJoint = NULL;

	CVisualNotesView *pView= (CVisualNotesView*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveView();
	CPoint CanvasOrigin(0,0);
	
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CVisualNotesView ) ) );
	pView->CanvasToClient( &CanvasOrigin );
	
	ILPOSITION Pos = pInsertList->GetHeadPosition();
	while( Pos )
	{
		pJoint = (CILJoint*)pInsertList->GetAt( Pos );

		//add joint back to its owner shape. ( joints always have master shapes )
		if( !ppTransaction )
			pJoint->GetMasterShape()->AddJoint( pJoint );

		//add joint back to its control joint.
		if( pJoint->GetControlJoint() && !ppTransaction )
			pJoint->GetControlJoint()->AddSymbiantJoint( pJoint );
		
		pJoint->SetCanvasOrigin( CanvasOrigin );
		pJoint->InvalidateSymbol( pView );
		m_JointData.AddTail( pJoint );

		if( ppTransaction )
			(*ppTransaction)->m_InsertList.AddTail( pJoint );
		else 
			ZOrderSymbol( pJoint ); 
		
		pInsertList->GetNext( Pos );
	}
}

void CILSymbolDB::_RemoveJoints( CILList<CILSymbol*,CILSymbol*> *pRemoveList, CILTransaction **ppTransaction )
{
	CILJoint *pJoint = NULL, *pControl = NULL;

	CVisualNotesView *pView= (CVisualNotesView*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveView();
	CPoint CanvasOrigin(0,0);
	
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CView ) ) );
	pView->CanvasToClient( &CanvasOrigin );
	CILList<CILJoint*,CILJoint*> SymbiantJoints;

	ILPOSITION Pos = pRemoveList->GetHeadPosition();
	while( Pos )
	{
		pJoint = (CILJoint*)pRemoveList->GetAt( Pos );

		//Remove the joint from it's owner shape.
		pJoint->GetMasterShape()->RemoveJoint( pJoint );
		
		//prepare joints symbiants for removal from database only if they are not already set for removal.
		if( pJoint->GetSymbiantJoints( &SymbiantJoints ) )
		{
			ILPOSITION SymbiantPos = SymbiantJoints.GetHeadPosition();
			while( SymbiantPos )
			{
				//make sure symbiant is not already set for removal.
				if( !pRemoveList->Find( SymbiantJoints.GetAt( SymbiantPos ) ) )
					pRemoveList->AddTail( SymbiantJoints.GetAt( SymbiantPos ) );
				SymbiantJoints.GetNext( SymbiantPos );
			}
			
			SymbiantJoints.RemoveAll();
		}

		//OKay, this one's a little complicated.
		//If  1. the joint has a control joint, 
		//AND 2. the control joint will have NO symbiants after this call,
		//AND 3. control joint is not set for removal, then prepare control joint for removal.
		if( (pControl = pJoint->GetControlJoint() ) )
		{
			BOOL bDeleteControl = TRUE;
			pControl->GetSymbiantJoints( &SymbiantJoints );
			ILPOSITION SymbiantPos = SymbiantJoints.GetHeadPosition();
			//Test condition 2 stated above. ( control will have no symbiants )
			while( SymbiantPos )
			{
				if( !pRemoveList->Find( SymbiantJoints.GetAt( SymbiantPos ) ) )
				{
					bDeleteControl = FALSE;
					break;
				}

				SymbiantJoints.GetNext( SymbiantPos );
			}

			if( bDeleteControl && !pRemoveList->Find( pControl ) )
				pRemoveList->AddTail( pControl );
		}
		
		pJoint->SetCanvasOrigin( CanvasOrigin );
		pJoint->InvalidateSymbol( pView );

		//Now we can remove symbiant joint from it's control
		if( ( pControl = pJoint->GetControlJoint() ) )
			pControl->RemoveSymbiantJoint( pJoint );
		
		m_JointData.RemoveAt( m_JointData.Find( pJoint ) );
		
		if( ppTransaction )
			(*ppTransaction)->m_RemoveList.AddTail( pJoint );

		pRemoveList->GetNext( Pos );
	}
}

