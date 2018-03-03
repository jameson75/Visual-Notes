class CILSymbolDB 
{
protected:
	CILList<CILSymbol*,CILSymbol*> m_SymbolData;
	CILList<CILSymbol*,CILSymbol*> m_JointData;
	
public:
	~CILSymbolDB() { DeleteDB(); }

public:
	void Insert( CILList<CILSymbol*,CILSymbol*> *pInsertList, CILTransaction **ppTransaction );
	void Remove( CILList<CILSymbol*,CILSymbol*> *pDeleteList, BOOL bRemoveChildren, CILTransaction **ppTransaction );
	void Update( CILList<CILSymbol*,CILSymbol*> *pOriginalList, CILList<CILSymbol*,CILSymbol*> *pReplaceList, 
		         CILTransaction **ppTransaction );
	void Insert( CILSymbol *pInsertSymbol, CILTransaction **ppTransaction );
	void Remove( CILSymbol *pDeleteSymbol, BOOL bRemoveChildren, CILTransaction **ppTransaction );
	void Update( CILSymbol *pOriginalSymbol, CILSymbol *pReplaceSymbol, CILTransaction **ppTransaction ); 

	void DeleteDB();
	CILList<CILSymbol*,CILSymbol*>* GetSymbolTable();
	CILList<CILSymbol*,CILSymbol*>* GetJointTable();

	BOOL QueryAll( CILList<CILSymbol*,CILSymbol*> *pSymbolList );
	BOOL QueryTracked( CILSymbol **ppTrackedSymbol );
	BOOL QuerySelected( CILList<CILSymbol*,CILSymbol*> *pSelectList );
	BOOL QueryFlag( DWORD dwFlag, CILList<CILSymbol*,CILSymbol*> *pResultList );
	BOOL QueryFlag( DWORD dwFlag, CILSymbol **ppSymbol );
	BOOL QueryHit( CPoint ptCanvasHit, CILSymbol **ppHitSymbol );
	BOOL QueryHidden( CILList<CILSymbol*, CILSymbol*> *pSymbolList );
	void ZOrderSymbol( CILSymbol *pSymbol );
	
protected:
	
	void _ZOrderChildLevels( CILList<CILSymbol*,CILSymbol*> *pChildList );
	void _ZOrderSymbiants( CILList<CILJoint*, CILJoint*> *pSymbiantList );
	void _InsertJoints( CILList<CILSymbol*,CILSymbol*> *pInsertList, CILTransaction **ppTransaction );
	void _RemoveJoints( CILList<CILSymbol*,CILSymbol*> *pRemoveList, CILTransaction **ppTransaction );

	friend class CVisualNotesDoc;
};
