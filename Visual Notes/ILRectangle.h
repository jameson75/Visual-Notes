
//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/16/01
//*********************************************

//CLASS DESCRIPTION:
//*********************************************
//This class represents the rectangular symbol
//objects used by this app.
//*********************************************

class CILRectangle : public CILPolygon
{
public:
	CILRectangle();
	CILRectangle( UINT t, UINT b, UINT r, UINT l );
	
//OVERRIDES

	//Defines the vertices of the rectangle.
	void CreatePoints( POINT **ppPoints, UINT *pNPoints );

protected:
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILRectangle );
};
