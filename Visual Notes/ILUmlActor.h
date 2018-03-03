
class CILUmlActor : public CILShape
{
protected:
	static	CSize	ms_FixedSize;

public:
	CILUmlActor();
	CILUmlActor( UINT x, UINT y );
	CILUmlActor( UINT l, UINT t, UINT r, UINT b );
	
	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILUmlActor );
};
