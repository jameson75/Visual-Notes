typedef void* ILPOSITION;

template<class Type, class ArgType>
class CILList
{

///////////////////////////////////////////////////////////
// CILListNode : relationship containter of each item.
protected:
	class CILListNode
	{
	public:
		Type	m_Data;
		CILListNode* m_pNext;
		CILListNode* m_pPrev;
	
		CILListNode() : m_pNext(NULL), m_pPrev(NULL)
		{}
	};

public:
	CILListNode *m_pHead;
	CILListNode *m_pTail;
	
public:

//Construction:

	CILList(); 
	CILList( CILList& rILList );
	~CILList();

//Insertion:

	//adds new item to head of the list.
	void AddHead( ArgType Data );
	//adds an entire list to the head of the list.
	void AddHead( CILList *plstAppend );
	//adds new item to back of the list.
	void AddTail( ArgType Data );
	//adds an entire list to the back of the list.
	void AddTail( CILList *plstAppend );
	//inserts item before specified postion in the list.
	ILPOSITION InsertBefore( Type ArgType, ILPOSITION Pos ); 
	//inserts item after specified position in the list.
	ILPOSITION InsertAfter( Type ArgType, ILPOSITION Pos );
	
//Retrieval:

	//gets the first node in the list.
	ILPOSITION GetHeadPosition();
	//gets the last node in the list.
	ILPOSITION GetTailPosition();
	//gets item at specified node in the list.
	Type GetAt( ILPOSITION Pos );	
	//gets tbe next node which follows the specifed node.
	void GetNext( ILPOSITION& Pos );
	//gets the previous node which precedes the specified node.
	void GetPrev( ILPOSITION& Pos );
	//gets the first item in the list.
	Type GetHead();
	//gets the last item in the list.
	Type GetTail();
	//finds a specified item in the list.	
	ILPOSITION Find( ArgType Data, ILPOSITION SearchAfter = NULL );
	
//Deletion:

	//removes item at specified position in the list.
	void RemoveAt( ILPOSITION Pos );
	//removes all items from the list.
	void RemoveAll();
	//removes item from front of the list.
	void RemoveHead();
	//removes item from back of the list.
	void RemoveTail();	

//Updating
	
	//sets the data at a specified position in the list.
	void SetAt( ArgType Data, ILPOSITION Pos );
	
//Status:

	//determines whether the list is empty.
	BOOL IsEmpty();
	//returns the number of items in the list.
	UINT GetCount();
};

template<class Type, class ArgType>
CILList<Type,ArgType>::CILList() : m_pHead(NULL), m_pTail( NULL )
{}

template<class Type, class ArgType>
CILList<Type,ArgType>::CILList( CILList<Type,ArgType>& ILList )
{ AddTail( &IIList ); }

template<class Type, class ArgType>
CILList<Type,ArgType>::~CILList()
{ RemoveAll(); }
	
template<class Type, class ArgType>
void CILList<Type,ArgType>::AddHead( ArgType Data )
{
	CILListNode *pNewNode = NULL;	
	
	//set node's data.
	pNewNode = new CILListNode;
	pNewNode->m_Data = Data;

	// if list is empty, the first node also becomes the tail.
	if( IsEmpty() )
		m_pTail = pNewNode;

	else //else the list has at least one node, so make the new node precede the first
	{
		//link new node in front of first node.
		pNewNode->m_pNext = m_pHead;
		m_pHead->m_pPrev = pNewNode;
	}
	
	//The new node is always the begining of the list.
	m_pHead = pNewNode;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::AddHead( CILList *plstAppend )
{
	CILListNode *pEnumNode = plstAppend->m_pHead;
	
	while( pEnumNode )
	{
		AddHead( pEnumNode->m_Data );
		pEnumNode = pEnumNode->m_pNext;
	}
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::AddTail( ArgType Data )
{
	CILListNode *pNewNode = NULL, *pEnumNode = NULL;

	//set node's data.
	pNewNode = new CILListNode;
	pNewNode->m_Data = Data;

	//if list is empty, the last node also becomes the first.
	if( IsEmpty() )
		m_pHead = pNewNode;


	//else there is at least one node in the list, so make the new node follow the last.
	else
	{
		//link new node in back of last node.
		pNewNode->m_pPrev = m_pTail;
		m_pTail->m_pNext = pNewNode;
	}

	//new node always becomes the tail.
	m_pTail = pNewNode;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::AddTail( CILList *plstAppend )
{
	CILListNode *pEnumNode = plstAppend->m_pHead;
	
	while( pEnumNode )
	{
		AddTail( pEnumNode->m_Data );
		pEnumNode = pEnumNode->m_pNext;
	}	
}

template<class Type, class ArgType>
ILPOSITION CILList<Type,ArgType>::GetHeadPosition()
{
	ILPOSITION HeadPos;

	HeadPos = m_pHead;
	return HeadPos;
}

template<class Type, class ArgType>
ILPOSITION CILList<Type, ArgType>::GetTailPosition()
{
	ILPOSITION TailPos;

	TailPos = m_pTail;
	return TailPos;
}

template<class Type, class ArgType>
Type CILList<Type,ArgType>::GetAt( ILPOSITION Pos )
{
	//simply extract data from node argument.
	return ((CILListNode*)Pos)->m_Data; 
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::GetNext( ILPOSITION& Pos )
{
	CILListNode *pCurNode = (CILListNode*)Pos;
	Pos = pCurNode->m_pNext;
}	

template<class Type, class ArgType>
void CILList<Type,ArgType>::GetPrev( ILPOSITION& Pos )
{
	CILListNode *pCurNode = (CILListNode*)Pos;
	Pos = pCurNode->m_pPrev;	
}

template<class Type, class ArgType>
Type CILList<Type,ArgType>::GetHead()
{
	ILPOSITION HeadPos;
	HeadPos = GetHeadPosition();
	return ((CILListNode*)HeadPos)->m_Data;
}

template<class Type, class ArgType>
Type CILList<Type,ArgType>::GetTail()
{
	ILPOSITION TailPos;
	TailPos = GetTailPosition();
	return ((CILListNode*)TailPos)->m_Data;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::RemoveAt( ILPOSITION Pos )
{
	CILListNode *pCurNode = (CILListNode*)Pos;

	if( pCurNode->m_pNext )
		pCurNode->m_pNext->m_pPrev = pCurNode->m_pPrev;

	if( pCurNode->m_pPrev )
		pCurNode->m_pPrev->m_pNext = pCurNode->m_pNext;

	if( pCurNode == m_pHead )
		m_pHead = pCurNode->m_pNext;

	if( pCurNode == m_pTail )
		m_pTail = pCurNode->m_pPrev;

	//we delete the current node.
	//**NOTE: we are not responsible for deleting the node's data.**//
	//This leaves Pos Invalid!
	delete pCurNode;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::RemoveAll()
{
	CILListNode *pEnumNode = m_pHead;

	while( pEnumNode )
	{
		m_pHead = pEnumNode->m_pNext;
		delete pEnumNode;
		pEnumNode = m_pHead;
	}
	
	m_pTail = NULL;
}

template<class Type, class ArgType>
UINT CILList<Type,ArgType>::GetCount()
{
	UINT count = 0;
	CILListNode *pEnumNode = m_pHead;

	while( pEnumNode )
	{
		count++;
		pEnumNode = pEnumNode->m_pNext;
	}
	
	return count;
}

template<class Type, class ArgType>
BOOL CILList<Type,ArgType>::IsEmpty()
{ return !GetCount(); }

template<class Type, class ArgType>
ILPOSITION CILList<Type,ArgType>::InsertAfter( Type ArgType, ILPOSITION Pos )
{
	CILListNode *pNewNode = new CILListNode;
	CILListNode *pCurNode = (CILListNode*)Pos;

	pNewNode->m_Data = ArgType;
	pNewNode->m_pNext = pCurNode->m_pNext;
	
	if( pCurNode->m_pNext )
		pCurNode->m_pNext->m_pPrev = pNewNode;

	pNewNode->m_pPrev = pCurNode;
	pCurNode->m_pNext = pNewNode;

	if( m_pTail == pCurNode )
		m_pTail = pNewNode;

	Pos = pNewNode;
	return Pos;
}

template<class Type, class ArgType>
ILPOSITION CILList<Type,ArgType>::InsertBefore( Type ArgType, ILPOSITION Pos )
{
	CILListNode *pNewNode = new CILListNode;
	CILListNode *pCurNode = (CILListNode*)Pos;
	
	pNewNode->m_Data = ArgType;
	
	pNewNode->m_pPrev = pCurNode->m_pPrev;
	if( pCurNode->m_pPrev )
		pCurNode->m_pPrev->m_pNext = pNewNode;

	pNewNode->m_pNext = pCurNode;
	pCurNode->m_pPrev = pNewNode;

	if( m_pHead == pCurNode )
		m_pHead = pNewNode;

	Pos = pNewNode;
	return Pos;
}

template<class Type, class ArgType>
ILPOSITION CILList<Type,ArgType>::Find( ArgType Data, ILPOSITION SearchAfter )
{
	CILListNode *pEnumNode = m_pHead;
	ILPOSITION Pos = NULL;

	//if user specified a position to searchafter, the first item to be searched will be the
	//node whose position follows SearchAfter.
	if( SearchAfter )
		pEnumNode = ((CILListNode*)SearchAfter)->m_pNext;

	while( pEnumNode )
	{
		if( pEnumNode->m_Data == Data )
		{
			Pos = pEnumNode;
			break;
		}
		
		pEnumNode = pEnumNode->m_pNext;
	}

	return Pos;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::RemoveHead()
{
	CILListNode *pRemoveNode = m_pHead;

	m_pHead = m_pHead->m_pNext;
	
	if( m_pHead )
		m_pHead->m_pPrev = NULL;
	
	delete pRemoveNode;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::RemoveTail()
{
	CILListNode *pRemoveNode = m_pTail;

	m_pTail = m_pTail->m_pPrev;

	if( m_pTail )
		m_pTail->m_pNext = NULL;

	delete pRemoveNode;
}

template<class Type, class ArgType>
void CILList<Type,ArgType>::SetAt( ArgType Data, ILPOSITION Pos )
{
	((CILListNode*)Pos)->m_Data = Data;
}