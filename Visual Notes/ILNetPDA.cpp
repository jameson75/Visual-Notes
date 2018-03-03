#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetPDA, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetPDA::m_DefBmpID = IDB_PALM1;

CILNetPDA::CILNetPDA()
{
	AssociateBitmapResource( IDB_PALM1, "Palm" );
	AssociateBitmapResource( IDB_PALM2, "Palm III" );
	AssociateBitmapResource( IDB_PALM3, "Palm V" );
	AssociateBitmapResource( IDB_POCKETPC1, "Casio" );
	AssociateBitmapResource( IDB_POCKETPC2, "Jornada" );
}

CILNetPDA::CILNetPDA( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_PALM1, "Palm");
	AssociateBitmapResource( IDB_PALM2, "Palm III" );
	AssociateBitmapResource( IDB_PALM3, "Palm V" );
	AssociateBitmapResource( IDB_POCKETPC1, "Casio" );
	AssociateBitmapResource( IDB_POCKETPC2, "Jornada" );
}

void CILNetPDA::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetPDA::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetPDA;
}