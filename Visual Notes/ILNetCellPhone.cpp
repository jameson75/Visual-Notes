#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetCellPhone, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetCellPhone::m_DefBmpID = IDB_CELLPHONE;

CILNetCellPhone::CILNetCellPhone()
{
	AssociateBitmapResource( IDB_CELLPHONE, "Cell" );
}

CILNetCellPhone::CILNetCellPhone( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_CELLPHONE, "Cell" );
}

void CILNetCellPhone::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetCellPhone::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetCellPhone;
}