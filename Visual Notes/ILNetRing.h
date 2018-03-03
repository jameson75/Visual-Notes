
class CILNetRing : public CILShape
{
public:

	CILNetRing();
	CILNetRing( UINT l, UINT t, UINT r, UINT b );
	void DrawSymbol( CDC *pDC = NULL );

protected:

	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILNetRing );
};

	


