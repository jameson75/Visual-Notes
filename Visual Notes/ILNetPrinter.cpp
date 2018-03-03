#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetPrinter, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

int CILNetPrinter::ms_LightOffset = 5;
int CILNetPrinter::ms_PaperOffset = 5;
int CILNetPrinter::ms_FeedOffset = 5;
CSize CILNetPrinter::ms_LightSize = CSize( 10, 5 );

UINT CILNetPrinter::m_DefBmpID = IDB_PRINTER1;

CILNetPrinter::CILNetPrinter()
{
	AssociateBitmapResource( IDB_PRINTER1, "Xerox 1" );
	AssociateBitmapResource( IDB_PRINTER2, "Xerox 2" );
	AssociateBitmapResource( IDB_PRINTER3, "InkJet 1" );
	AssociateBitmapResource( IDB_PRINTER4, "InkJet 2" );
	AssociateBitmapResource( IDB_PRINTER5, "DotMatrix" );
}

CILNetPrinter::CILNetPrinter( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_PRINTER1, "Xerox 1" );
	AssociateBitmapResource( IDB_PRINTER2, "Xerox 2" );
	AssociateBitmapResource( IDB_PRINTER3, "InkJet 1" );
	AssociateBitmapResource( IDB_PRINTER4, "InkJet 2" );
	AssociateBitmapResource( IDB_PRINTER5, "DotMatrix" );
}

void CILNetPrinter::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetPrinter::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetPrinter; }

