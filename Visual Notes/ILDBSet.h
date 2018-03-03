#ifndef __ILDBSET_H
#define __ILDBSET_H

#define ILDBSET_ID_FIRST 1
#define ILDBSET_ID_NULL 0

class CILDBSet
{
protected:
	UINT							m_ID;
	CILList<CILSymbol*,CILSymbol*>	m_LinkSymbolList;
	CString							m_strName;

public:
	static UINT			m_HighID;
	CILSymbolDB			m_DB;
	CILTransactionCoord m_TC;

public:
	CILDBSet();
	
	//sets this diagram's assigned id.
	inline void SetID( UINT id ) { m_ID = id; }
	//returns the id assigned to this database set object.
	inline UINT GetID() { return m_ID; }
	//returns the name of the database set.
	inline void GetName( CString& strName ) { strName = m_strName; }
	//sets the name of the database set.
	void SetName( CString strName ){ m_strName = strName; }

protected:
	//determines whether this diagram is an orphan within a project.
	BOOL IsOrphan( CILList<UINT,UINT> *pVisitList );

public:
	//determines whether this diagram is an orphan within a project.
	BOOL IsOrphan();
	//sets the containing symbol
	void AddLinkedSymbol( CILSymbol *pLinkSymbol ); 
	//returns the containing symbol.
	BOOL GetLinkedSymbols( CILList<CILSymbol*,CILSymbol*> *pSymbolList ); 
	//remvoes the specified link symbol from this objects list if it exists.
	BOOL RemoveLinkedSymbol( CILSymbol *pLinkSymbol );
	
};

#endif