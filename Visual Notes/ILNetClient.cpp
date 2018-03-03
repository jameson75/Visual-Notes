#include "stdafx.h"
#include "stdil.h"

IMPLEMENT_SERIAL( CILNetClient, CILBitmapShape, VERSIONABLE_SCHEMA | 1 );

CSize CILNetClient::ms_MonitorOffset(5,5);
CSize CILNetClient::ms_ScreenOffset(5,5);
CSize CILNetClient::ms_CDRomOffset(5,5);

UINT CILNetClient::m_DefBmpID = IDB_CLIENT1;

CILNetClient::CILNetClient() 
{
	AssociateBitmapResource( IDB_CLIENT1, "Terminal" );
	AssociateBitmapResource( IDB_CLIENT2, "Desktop" );
	AssociateBitmapResource( IDB_CLIENT3, "Tower" );
	AssociateBitmapResource( IDB_CLIENT4,  "Remember 486");
	AssociateBitmapResource( IDB_CLIENTMAC, "Mac");
}

CILNetClient::CILNetClient( UINT l, UINT t, UINT r, UINT b ) : CILBitmapShape( l,t,r,b )
{
	AssociateBitmapResource( IDB_CLIENT1, "Terminal" );
	AssociateBitmapResource( IDB_CLIENT2, "Desktop" );
	AssociateBitmapResource( IDB_CLIENT3, "Tower" );
	AssociateBitmapResource( IDB_CLIENT4,  "Remember 486");
	AssociateBitmapResource( IDB_CLIENTMAC, "Mac");
}


void CILNetClient::DrawSymbol( CDC *pDC )
{
	CILBitmapShape::DrawSymbol( pDC );
	_RenderBitmap( pDC );
}

void CILNetClient::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILNetClient; }

