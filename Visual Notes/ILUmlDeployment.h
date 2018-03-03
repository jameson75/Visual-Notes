
class CILUmlDeployment : public CILShape
{
public:
	CILUmlDeployment();
	CILUmlDeployment( UINT l, UINT t, UINT r, UINT b );

protected:
	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );
	void _RenderCaption( CDC *pDC );

	DECLARE_SERIAL( CILUmlDeployment );
};