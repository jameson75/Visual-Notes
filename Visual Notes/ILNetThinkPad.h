class CILNetThinkPad : public CILBitmapShape
{
protected:
	static UINT m_DefBmpID;

public:
	CILNetThinkPad();
	CILNetThinkPad( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );
	void SetDefaultBmpID( UINT id ) { m_DefBmpID = id; }
	UINT GetDefaultBmpID() { return m_DefBmpID; }
	UINT GetAppDefaultBmpID() { return IDB_THINKPAD1; }

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetThinkPad );
};