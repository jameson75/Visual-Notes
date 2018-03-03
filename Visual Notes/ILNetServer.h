
class CILNetServer : public CILBitmapShape
{
private:
	static CSize	ms_Drive1Offset;
	static int		ms_DriveHeight;

	static UINT		m_DefBmpID;

public:
	CILNetServer();
	CILNetServer( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_SERVER1; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetServer );
};