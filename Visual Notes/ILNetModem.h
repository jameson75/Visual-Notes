class CILNetModem : public CILBitmapShape
{
protected:
	static UINT m_DefBmpID;

public:
	CILNetModem();
	CILNetModem( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_MODEM; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetModem );
};