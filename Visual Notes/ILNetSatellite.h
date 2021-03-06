class CILNetSatellite : public CILBitmapShape
{
protected:
	static UINT m_DefBmpID;

public:
	CILNetSatellite();
	CILNetSatellite( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );
	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_SATELLITE; }
	
	DECLARE_SERIAL( CILNetSatellite );
};