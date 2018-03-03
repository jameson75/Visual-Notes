
class CILGeneralize : public CILPolygon
{
public:
	CILGeneralize();
	CILGeneralize( UINT l, UINT t, UINT r, UINT b );

	void CreatePoints( POINT **ppPoints, UINT *pNPoints );

protected:
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILGeneralize )
};