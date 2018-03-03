
class CILUmlPackage : public CILShape
{
private:
	static CSize ms_TagSize;

public:
	CILUmlPackage();
	CILUmlPackage( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILUmlPackage );
};
