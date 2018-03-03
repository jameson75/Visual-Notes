
class CILNetCloud : public CILShape
{
public:
	CILNetCloud();
	CILNetCloud( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILNetCloud );
};

