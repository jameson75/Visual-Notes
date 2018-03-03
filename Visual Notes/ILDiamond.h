//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/20/01
//*********************************************

class CILDiamond : public CILPolygon
{
public:
	CILDiamond();
	CILDiamond( UINT l, UINT t, UINT r, UINT b );
	void CreatePoints( POINT **ppPoints, UINT *pNPoints );

	protected:
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILDiamond );
};
