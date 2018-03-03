#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetCamera, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetCamera::m_DefBmpID = IDB_CAMERA1;

CILNetCamera::CILNetCamera()
{
	AssociateBitmapResource( IDB_CAMERA1, "Handheld Camera" );
	AssociateBitmapResource( IDB_CAMERA2, "Studio Camera" );
}

CILNetCamera::CILNetCamera( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_CAMERA1, "Handheld Camera" );
	AssociateBitmapResource( IDB_CAMERA2, "Studio Camera" );
}

void CILNetCamera::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetCamera::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetCamera;
}