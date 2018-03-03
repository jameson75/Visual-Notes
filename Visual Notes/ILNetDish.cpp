#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetDish, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetDish::m_DefBmpID = IDB_DISH1;

CILNetDish::CILNetDish()
{
	AssociateBitmapResource( IDB_DISH1, "Dish" );
}

CILNetDish::CILNetDish( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_DISH1, "Dish" );
}

void CILNetDish::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetDish::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetDish;
}