#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetModem, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetModem::m_DefBmpID = IDB_MODEM;

CILNetModem::CILNetModem()
{
	AssociateBitmapResource( IDB_MODEM, "Modem" );
}

CILNetModem::CILNetModem( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_MODEM, "Modem" );
}

void CILNetModem::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetModem::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetModem;
}