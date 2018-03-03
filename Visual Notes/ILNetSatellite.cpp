#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetSatellite, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetSatellite::m_DefBmpID = IDB_SATELLITE;

CILNetSatellite::CILNetSatellite()
{
	AssociateBitmapResource( IDB_SATELLITE, TEXT("Satellite") );
}

CILNetSatellite::CILNetSatellite( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_SATELLITE, TEXT("Satellite") );
}

void CILNetSatellite::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetSatellite::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetSatellite;
}