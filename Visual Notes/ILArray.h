
template<class Type, class ArgType>
class CILArray
{
protected:
	Type*	m_pArray;
	Type	m_Initializer;
	UINT	m_nGrow; 
	UINT	m_nSize;
	UINT	m_nValidSize;
	BOOL	m_bInitializeOnGrow;

public:

//Construction:

	CILArray();
	CILArray( UINT nStart, UINT nGrow );
	CILArray( CILArray& Array );
	~CILArray();

//Status:

	//sets the initial and growth size of the array.
	void SetSize( UINT nStart, UINT nGrow );
	//gets the current total size of the array.
	UINT GetSize();
	//gets the size of the array up to and including the highest vaild index.
	UINT GetValidSize();
	//grows the array by it's set growth size.
	void Grow();
	//determines if array should intiailize data on growth.
	void InitializeOnGrow( BOOL bInitialize );
	//removes all invalid elements after last valid index.
	void Reduce();
		
//Insertion:

	//copies elements into this array, grows array if neccessary.
	void Copy( CILArray& Array );
	//appends the elements of the array to this array.
	void Append( CILArray& Array, BOOL bAfterValid );
	//adds element to end of array, grows if neccessary.
	void Add( ArgType Data );

//Update

	//initializes all elements in the array.
	void Initialize( ArgType Data );
	//sets the element at the current index.
	void SetAt( UINT nIndex, ArgType Data );
	//sets the element at the current index, grows the array if neccessary.
	void SetAtGrow( UINT nIndex, ArgType Data );
	//gets the element at the specified index.
	Type GetAt( UINT nIndex );
	//access array elements using subscript operator.
	Type& operator [] ( UINT nIndex );
	
//Deletion:
	//removes all elements from this array.
	void RemoveAll();
	//removes an element from end of array.
	void Remove();

//helpers
protected:
	void _CopyArray( Type* pDest, Type *pSource, UINT nArraySize );
	void _InitializeArray( Type* pArray, UINT nBegin, UINT nElements );
};

template<class Type, class ArgType>
CILArray<Type,ArgType>::CILArray() : m_pArray( NULL ), m_nGrow(1), m_nSize(0),
									 m_bInitializeOnGrow(0), m_nValidSize(0)
{}

template<class Type, class ArgType>
CILArray<Type,ArgType>::CILArray( UINT nStart, UINT nGrow ) : m_pArray( NULL ), m_nGrow(1), m_nSize(0),
															m_bInitializeOnGrow(0), m_nValidSize(0)
{ SetSize( nStart, nGrow ); }

template<class Type, class ArgType>
CILArray<Type,ArgType>::~CILArray()
{ RemoveAll(); }

template<class Type, class ArgType>
CILArray<Type,ArgType>::CILArray( CILArray& Array ) : m_pArray( NULL ), nGrow(1), m_nSize(0),										
													  m_bInitializeOnGrow(0), m_nValidSize(0)
{ 
	Copy( Array );
	Initializer = Array.Initializer;
	m_bInitializeOnGrow = Array.m_bInitializeOnGrow;
	nGrow = Array.m_nGrow;
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::SetSize( UINT nStart, UINT nGrow )
{
	UINT nCopySize = 0;
	Type *pNewArray = NULL;

	//if user specified a size of zero for the array, just delete the array.
	if( nStart == 0 )
		RemoveAll();
	
	pNewArray = new Type[nStart];	
	
	//if the user is setting the size of an already intialized array, copy the old 
	//into the new then delete the old array.
	if( m_pArray )
	{
		//if the old array is smaller or equal to the size of the new array, 
		//we can copy ALL the old array's elements, into the new array and it will have the SAME valid
		//size and we might have to initialize the new invalid elements. Otherwise, we must
		//limit the number of copies to the size of the new array, and the
		//new valid size will be the last element in the new array and no initialization is needed.
		if( m_nSize <= nStart ) 
		{
			nCopySize = m_nSize;
			if( m_bInitializeOnGrow )
				_InitializeArray( pNewArray, m_nSize, nStart - m_nSize );
		}
		
		else
		{
			nCopySize = nStart; 
			m_nValidSize = nStart; //valid size changes if not all elements could be copied.
		}

		_CopyArray( pNewArray, m_pArray, nCopySize );
		delete[] m_pArray;
	}

	else // else this is a brand new array and we initialize the entire thing.
		_InitializeArray( pNewArray, 0, nStart );
	
	m_pArray = pNewArray;
	m_nSize = nStart; 
	m_nGrow = nGrow;
}

template<class Type, class ArgType>
UINT CILArray<Type,ArgType>::GetSize()
{ return m_nSize; }

template<class Type, class ArgType>
UINT CILArray<Type,ArgType>::GetValidSize()
{ return m_nValidSize; }

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Grow()
{	
	//Grow() is just a special case of set SetSize()
	SetSize( m_nSize + m_nGrow, m_nGrow );
} 

template<class Type, class ArgType>
void CILArray<Type,ArgType>::InitializeOnGrow( BOOL bInitialize )
{ m_bInitializeOnGrow = bInitialize; }

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Reduce()
{	
	//Reduce() is just a special case of SetSize()
	SetSize( m_nValidSize, m_nGrow ); 
} 

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Copy( CILArray& Array )
{
	//copy will only increase the size of the array if needed.
	//it will not reduce it.	
	if( m_nSize < Array.m_nSize )
	{
		delete[] m_pArray;
		m_pArray = new Type[Array.m_nSize];
	}

	_CopyArray( m_pArray, Array.m_pArray, Array.m_nSize );
	m_nSize = Array.m_nSize;
	
	//We also copied invalid elements, so we need to retieve the valid size of original.
	m_nValidSize = Array.m_nValidSize;
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Append( CILArray &Array, BOOL bAfterValid )
{
	//if specified, the appending array will be appended
	//after the last valid element.
	UINT nThisSize = ( bAfterValid ) ? m_nValidSize : m_nSize;

	//create a new array EXACTLY large enough for both arrays.
	//and copy both arrays into it.

	Type *pNewArray = new Type[ nThisSize + Array.m_nSize ];
	_CopyArray( pNewArray, m_pArray, nThisSize );
	
	for( UINT i = nThisSize; i < nThisSize + Array.m_nSize; i++ )
		*( pNewArray + i ) = *(Array.m_pArray + ( i - nThisSize ) );

	m_pArray = pNewArray;
	m_nSize = nThisSize + Array.m_nSize;
	m_nValidSize = nThisSize + Array.m_nValidSize;
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Add( ArgType Data )
{
	//the valid size should NEVER be greater than the real size.
	//so we only need to the grow array in the case where they're equal.
	if( m_nValidSize == m_nSize )
		Grow();
	
	*( m_pArray + m_nValidSize ) = Data;
	m_nValidSize++;
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Initialize( ArgType Data )
{ 
	m_Initializer = Data;
	m_bInitializeOnGrow = 1;
	_InitializeArray( m_pArray, 0, m_nSize  ); 
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::SetAtGrow( UINT nIndex, ArgType Data )
{
	UINT nRequiredSize = nIndex + 1;

	if( nRequiredSize > m_nSize )
	{
		//if we can accomadate the required size using Grow() then we use it.
		if( m_nSize + m_nGrow > nRequiredSize )
			Grow();
		
		//else we expand the array EXACTLY to the required size for the index.
		else 
			SetSize( nRequiredSize, m_nGrow );
	}

	m_nValidSize = nRequiredSize;
	*( m_pArray + nIndex ) = Data;
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::SetAt( UINT nIndex, ArgType Data )
{	
	if( ( nIndex + 1 ) > m_nValidSize )
		m_nValidSize = (nIndex + 1);

	*( m_pArray + nIndex ) = Data; 
}

template<class Type, class ArgType>
Type CILArray<Type,ArgType>::GetAt( UINT nIndex )
{ return *( m_pArray + nIndex ); }

template<class Type, class ArgType>
Type& CILArray<Type,ArgType>::operator [] ( UINT nIndex )
{	
	if( (nIndex + 1) > m_nValidSize )
		m_nValidSize = nIndex + 1;

	return *( m_pArray + nIndex ); 
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::Remove()
{
	//we don't physically remove the last valid element, 
	//we simply discard it.
	if( m_nValidSize )
		m_nValidSize--;
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::RemoveAll()
{
	if( m_pArray )
	{
		delete[] m_pArray;
		m_pArray = NULL;
		m_nSize = 0;
		m_nValidSize = 0;
	}
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::_CopyArray( Type *pDest, Type *pSource, UINT nCopySize )
{
	for( UINT i = 0; i < nCopySize; i++ )
		*( pDest + i ) = *( pSource + i );
}

template<class Type, class ArgType>
void CILArray<Type,ArgType>::_InitializeArray( Type *pArray, UINT nBegin, UINT nElements )
{
	for( UINT i = nBegin; i < nBegin + nElements; i++ )
			*(pArray + i) = m_Initializer;
}


	






