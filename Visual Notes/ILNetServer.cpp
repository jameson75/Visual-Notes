#include "stdafx.h"
#include "resource.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetServer, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

CSize CILNetServer::ms_Drive1Offset = CSize(5,5);
int CILNetServer::ms_DriveHeight = 10;

UINT CILNetServer::m_DefBmpID = IDB_SERVER1;

CILNetServer::CILNetServer()
{
	AssociateBitmapResource( IDB_SERVER1, "Corp. Server" );
	AssociateBitmapResource( IDB_SERVER2, "Mainframe Server" );
}

CILNetServer::CILNetServer( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_SERVER1, "Corp. Server" );
	AssociateBitmapResource( IDB_SERVER2, "Mainframe Server" );
}

void CILNetServer::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetServer::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetServer; }


