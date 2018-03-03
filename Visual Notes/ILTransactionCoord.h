class CILTransactionStacks
{
private:
	CILList<CILTransaction*,CILTransaction*>	m_RollBackStack;
	CILList<CILTransaction*,CILTransaction*>	m_RollFowardStack;
protected:
	UINT m_MaxStackSize;
	
public:
	enum STACKTYPE{ stackRollBack = 0x01, stackRollFoward = 0x02 };

public:
	void ClearStack( STACKTYPE ClearStack );

protected:
	CILTransactionStacks();
	~CILTransactionStacks();

	void _PushRollBack( CILTransaction *pTopAction );
	void _PushRollFoward( CILTransaction *pTopAction );
	BOOL _PopRollBack( CILTransaction **ppTopAciton );
	BOOL _PopRollFoward( CILTransaction **ppTopAction );
};

class CILTransactionCoord : CILTransactionStacks
{
	
public:

	//Registers new undoable transaction.
	void Commit( CILTransaction *pTransaction );
	//rolls foward the last transactions.
	void RollFoward( int nTransactions = 1 );
	//rolls back the last transactions.
	void RollBack( int nTransactions = 1 );
	//combines the last nTransctions on the rollback stack.
	void Collapse( int nTransactions );
	//deletes the las nTransacitions from the rollback stack.
	void Disreguard( int nTransactions );
};
