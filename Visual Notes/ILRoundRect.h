
class CILRoundRect :  public CILShape
{
public:
	CILRoundRect();
	CILRoundRect( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILRoundRect );
};