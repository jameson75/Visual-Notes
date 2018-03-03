//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/20/01
//*********************************************

class CILPolygon: public CILShape
{
public:
	CILPolygon();
	CILPolygon( UINT l, UINT t, UINT r, UINT b );
	
	//Draws Symbol to specified device.
	void DrawSymbol( CDC *pDC );
	//override to define the polygon's vertices.
	virtual void CreatePoints( POINT **ppPoints, UINT *pNPoints ) = 0;

	DECLARE_DYNAMIC( CILPolygon );
};
	
