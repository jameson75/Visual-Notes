//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright (C) 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/20/01
//*********************************************

class CILCircle : public CILShape
{
public:
	CILCircle();
	CILCircle( UINT r, UINT t, UINT b, UINT l );
	void DrawSymbol( CDC *pDC );

protected:
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILCircle );
};
