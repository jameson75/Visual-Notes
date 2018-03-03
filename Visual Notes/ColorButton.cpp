// ColorButton.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton() : m_dwButtonColor(0), m_bUseBitmap( FALSE ), m_BitmapResID(0) 
{}

CColorButton::~CColorButton()
{}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

BOOL CColorButton::Create(CWnd *pParent, UINT resID, DWORD dwColor )
{
	CRect ButtonRect;
	ASSERT( pParent->IsKindOf( RUNTIME_CLASS( CDialog )));
	CButton *pResButton = (CButton*)((CDialog*)pParent)->GetDlgItem( resID );
	pResButton->GetWindowRect( &ButtonRect );
	pParent->ScreenToClient( &ButtonRect );
	
	CButton::Create( NULL, WS_CHILD | BS_OWNERDRAW | WS_VISIBLE | BS_PUSHBUTTON, ButtonRect,
					pParent, resID );

	m_dwButtonColor = dwColor;
	pResButton->DestroyWindow();
	return TRUE;
}

void CColorButton::SetBitmap( UINT ResID )
{ 
	m_BitmapResID = ResID;
	m_bUseBitmap = TRUE;
}

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	if( !m_bUseBitmap )
	{
		CRect ButtonRect;
		GetClientRect( &ButtonRect );
		GetDC()->FillSolidRect( &ButtonRect, m_dwButtonColor );
	}

	else
	{
		CDC TempDC;
		CBitmap bitmap;
		BITMAP BMProp;
		CRect ClientRect;
				
		bitmap.LoadBitmap( m_BitmapResID );
		bitmap.GetBitmap( &BMProp );
		TempDC.CreateCompatibleDC( GetDC() );
		TempDC.SelectObject( &bitmap );		
		GetClientRect( &ClientRect );
		GetDC()->StretchBlt( 0, 0, ClientRect.Width(), ClientRect.Height(),
							&TempDC, 0, 0, BMProp.bmWidth, BMProp.bmHeight, SRCCOPY );
		TempDC.DeleteDC();
		bitmap.DeleteObject();
	}		
}

