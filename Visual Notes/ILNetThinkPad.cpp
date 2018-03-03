#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetThinkPad, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetThinkPad::m_DefBmpID = IDB_THINKPAD1;

CILNetThinkPad::CILNetThinkPad()
{
	AssociateBitmapResource( IDB_THINKPAD1, "Note Book" );
	AssociateBitmapResource( IDB_THINKPAD2, "E-Series" );
	AssociateBitmapResource( IDB_THINKPAD3, "I-35" );
}

CILNetThinkPad::CILNetThinkPad( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_THINKPAD1, "Note Book" );
	AssociateBitmapResource( IDB_THINKPAD2, "E-Series" );
	AssociateBitmapResource( IDB_THINKPAD3, "I-35" );
}

void CILNetThinkPad::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetThinkPad::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetThinkPad;
}