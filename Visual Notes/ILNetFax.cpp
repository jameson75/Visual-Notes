#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetFax, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

UINT CILNetFax::m_DefBmpID = IDB_FAX1;

CILNetFax::CILNetFax()
{
	AssociateBitmapResource( IDB_FAX1, "Fax machine 1" );
	//AssociateBitmapResource( IDB_FAX2, "Fax machine 2" );
	AssociateBitmapResource( IDB_FAX3, "Fax machine 3" );
}

CILNetFax::CILNetFax( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_FAX1, "Fax machine 1" );
	//AssociateBitmapResource( IDB_FAX2, "Fax machine 2" );
	AssociateBitmapResource( IDB_FAX3, "Fax machine 3" );
}

void CILNetFax::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetFax::_New( CILSymbol **ppSymbol )
{
	*ppSymbol = new CILNetFax;
}