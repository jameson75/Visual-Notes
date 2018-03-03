class CILNetDish : public CILBitmapShape
{
protected:
	static UINT m_DefBmpID;

public:
	CILNetDish();
	CILNetDish( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );	
	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_DISH1; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetDish );
};