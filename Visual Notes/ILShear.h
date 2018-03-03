//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/20/01
//*********************************************

class CILShear : public CILPolygon
{
protected:
	int m_ShearFactor;

public:
	CILShear();
	CILShear::CILShear( UINT l, UINT t, UINT r, UINT b );

	//Defines the shears points.
	void CreatePoints( POINT **ppPoints, UINT *pNPoints );

protected:
	void _CopyBaseData( CILSymbol **ppSymbol );
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILShear );
};
