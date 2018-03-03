
class CILTransaction
{
private:
	static UINT m_InstanceCount;

public:
	CILList<CILSymbol*,CILSymbol*> m_InsertList;
	CILList<CILSymbol*,CILSymbol*> m_RemoveList;
	CILList<CILSymbol*,CILSymbol*> m_OriginalSymbList;
	CILList<CILSymbol*,CILSymbol*> m_ReplaceImgList;
	CILList<CILSymbol*,CILSymbol*> m_OriginalImgList;
	
	void AppendTransaction( CILTransaction *pTransaction );

	CILTransaction() { m_InstanceCount++; }
	~CILTransaction() { m_InstanceCount--; }

	void OnRollBack();
	void OnRollFoward();
	void OnRollBackFinal();
	void OnRollFowardFinal();
};
