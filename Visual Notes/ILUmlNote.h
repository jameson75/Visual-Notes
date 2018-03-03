
class CILUmlNote : public CILShape
{
private:
	static CSize ms_FoldSize;

public:
	CILUmlNote();
	CILUmlNote( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILUmlNote );
};