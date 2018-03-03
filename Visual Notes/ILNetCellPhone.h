
class CILNetCellPhone : public CILBitmapShape
{
protected:
	static UINT m_DefBmpID;

public:
	CILNetCellPhone();
	CILNetCellPhone( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_CELLPHONE; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetCellPhone );
};