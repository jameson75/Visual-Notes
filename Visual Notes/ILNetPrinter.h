class CILNetPrinter : public CILBitmapShape
{
private:
	static int ms_LightOffset;
	static int ms_PaperOffset;
	static int ms_FeedOffset;
	static CSize ms_LightSize;

protected:
	static UINT m_DefBmpID;


public:
	CILNetPrinter();
	CILNetPrinter( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_PRINTER1; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetPrinter );
};