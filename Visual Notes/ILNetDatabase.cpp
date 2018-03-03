#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetDatabase, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );


UINT CILNetDatabase::m_DefBmpID = IDB_DATABASE;

int CILNetDatabase::ms_BaseHeight = 10;

CILNetDatabase::CILNetDatabase() : CILBitmapShape()
{
	AssociateBitmapResource( IDB_DATABASE, "Dbase" );
}

CILNetDatabase::CILNetDatabase( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{	
	AssociateBitmapResource( IDB_DATABASE, "Dbase" ); 
}

void CILNetDatabase::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
	
}

void CILNetDatabase::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetDatabase; }

