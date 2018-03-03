#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetHub, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetHub::m_DefBmpID = IDB_HUB1;

CILNetHub::CILNetHub()
{
	AssociateBitmapResource( IDB_HUB1, "Ethernet Hub 1" );
	AssociateBitmapResource( IDB_HUB2, "Ethernet Hub 2" );
}

CILNetHub::CILNetHub( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l, t, r, b )
{
	AssociateBitmapResource( IDB_HUB1, "Ethernet Hub 1" );
	AssociateBitmapResource( IDB_HUB2, "Ethernet Hub 2" );
}

void CILNetHub::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );

	return;
}

void CILNetHub::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetHub; }
