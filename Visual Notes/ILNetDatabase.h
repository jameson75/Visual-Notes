

class CILNetDatabase : public CILBitmapShape
{
private:
	static int ms_BaseHeight;
	static UINT m_DefBmpID;

public:
	CILNetDatabase();
	CILNetDatabase( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_DATABASE; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetDatabase );
};
