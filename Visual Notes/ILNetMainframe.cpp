#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetMainframe, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetMainframe::m_DefBmpID = IDB_IBMMAINFRAME;

CILNetMainframe::CILNetMainframe()
{
	AssociateBitmapResource( IDB_IBMMAINFRAME, "Mainframe" );
}

CILNetMainframe::CILNetMainframe( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_IBMMAINFRAME, "Mainframe" );
}

void CILNetMainframe::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetMainframe::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetMainframe;
}