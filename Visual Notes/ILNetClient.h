class CILNetClient : public CILBitmapShape
{
private: 
	static CSize ms_ScreenOffset;
	static CSize ms_MonitorOffset;
	static CSize ms_CDRomOffset;
	static UINT	m_DefBmpID;

public:
	CILNetClient();
	CILNetClient( UINT l, UINT t, UINT r, UINT b );
	void DrawSymbol( CDC *pDC );

	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_CLIENT1; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetClient );
};