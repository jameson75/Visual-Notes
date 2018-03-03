#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "ColorButton.h"
#include "BitmapSymbolPropBox.h"

#define ILBITMAP_DEFAULT_ALLOWSTRECHING FALSE

IMPLEMENT_DYNAMIC( CILBitmapShape, CILShape );

BOOL CILBitmapShape::m_bDefAllowStreching = ILBITMAP_DEFAULT_ALLOWSTRECHING;

CILBitmapShape::CILBitmapShape() : m_bAllowStretching(m_bDefAllowStreching)
{}

CILBitmapShape::CILBitmapShape( UINT l, UINT t, UINT r, UINT b ) : CILShape( l,t,r,b ), 
																   m_bAllowStretching(m_bDefAllowStreching)
{}

void CILBitmapShape::AssociateBitmapResource( UINT resID, CString strBmpName )
{ 
	CILBmpResItem ResItem;

	ResItem.m_ResID = resID;
	ResItem.m_strBmpName = strBmpName;
	m_BitmapResourceList.AddTail( ResItem ); 
}

UINT CILBitmapShape::DoModalPropBox( CWnd *pWnd, CDialog *pExtdDlg )
{
	UINT retVal;
	CBitmapSymbolPropBox BmpSymbolPropBox( pWnd );
	CILBmpResItem DefResItem;

	//Add the names of the bitmap images to this bitmap-symbol's property box.
	ILPOSITION Pos = m_BitmapResourceList.GetHeadPosition();
	while( Pos )
	{
		BmpSymbolPropBox.m_BitmapResourceList.AddTail( m_BitmapResourceList.GetAt( Pos ) );
		
		if( m_BitmapResourceList.GetAt( Pos ).m_ResID == GetAppDefaultBmpID() )
			BmpSymbolPropBox.m_DefBmpResItem = m_BitmapResourceList.GetAt( Pos );
		
		m_BitmapResourceList.GetNext( Pos );
	}

	BmpSymbolPropBox.m_CurBmpResItem = m_CurBmpResItem;
	BmpSymbolPropBox.m_bAllowStreching = m_bAllowStretching;
	BmpSymbolPropBox.m_bDefAllowStreching = ILBITMAP_DEFAULT_ALLOWSTRECHING;
		
	//set any changes the user may have specified.
	if( ( retVal = BmpSymbolPropBox.DoModal() ) == IDOK )
	{
		m_bAllowStretching = BmpSymbolPropBox.m_bAllowStreching;
	 	m_CurBmpResItem =  BmpSymbolPropBox.m_CurBmpResItem;
		
		if( BmpSymbolPropBox.m_bMakeCurrent )
		{
			m_bDefAllowStreching = BmpSymbolPropBox.m_bAllowStreching;
			SetDefaultBmpID( BmpSymbolPropBox.m_CurBmpResItem.m_ResID );
		}
	}
			
	return retVal;
}

void CILBitmapShape::_RenderBitmap( CDC *pDC )
{
	if( m_CurBmpResItem.m_ResID )
	{
		CDC SourceDC;
		CBitmap Bitmap;
		BITMAP BMProp;
		CPoint ptOrigin;

		SourceDC.CreateCompatibleDC( pDC );
		Bitmap.LoadBitmap( m_CurBmpResItem.m_ResID );
		SourceDC.SelectObject( &Bitmap );
		Bitmap.GetBitmap( &BMProp );

		ptOrigin = m_BoundingRect.TopLeft();
		ptOrigin.Offset( m_CanvasOrigin );
		
		if( m_bAllowStretching )
		{
			pDC->StretchBlt( ptOrigin.x, ptOrigin.y, m_BoundingRect.Width(), m_BoundingRect.Height(), 
							 &SourceDC, 0, 0, BMProp.bmWidth, BMProp.bmHeight, SRCCOPY );
		}

		else
		{
			pDC->BitBlt( ptOrigin.x, ptOrigin.y, BMProp.bmWidth, BMProp.bmHeight, &SourceDC, 0, 0,
						 SRCCOPY );
			
			if( BMProp.bmHeight != m_BoundingRect.Height() || BMProp.bmWidth != m_BoundingRect.Width() )
			{
				CRect NewBoundingRect = m_BoundingRect;
				NewBoundingRect.right = NewBoundingRect.left + BMProp.bmWidth;
				NewBoundingRect.bottom = NewBoundingRect.top + BMProp.bmHeight;
				SetBoundingRect( &NewBoundingRect );
			}
		}

		Bitmap.DeleteObject();
		SourceDC.DeleteDC();
	}
}

BOOL CILBitmapShape::Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList,
						    CILList<CILSymbol*,CILSymbol*> *pJointList )
{
	//if this bitmap is not in Stretching mode, we only uniform movement of the symbol
	//no shape changing.
	CPoint CanvasPoint(WindowPoint);

	CanvasPoint.Offset( -m_CanvasOrigin );
	if( m_bAllowStretching || m_Tracker.HitTest( CanvasPoint ) == 8  )
		return CILShape::Track( pWnd, WindowPoint, pXYChildList, pJointList );

	return TRUE;
}

void CILBitmapShape::Initialize()
{
	ILPOSITION Pos = m_BitmapResourceList.GetHeadPosition();
	while( Pos )
	{
		if( m_BitmapResourceList.GetAt( Pos ).m_ResID == GetDefaultBmpID() )
		{
			m_CurBmpResItem = m_BitmapResourceList.GetAt( Pos );
			break;
		}

		m_BitmapResourceList.GetNext( Pos );
	}
}

void CILBitmapShape::_CopyBaseData( CILSymbol **ppSymbol )
{
	CILShape::_CopyBaseData( ppSymbol );

	((CILBitmapShape*)*ppSymbol)->m_bAllowStretching = m_bAllowStretching;
	((CILBitmapShape*)*ppSymbol)->m_CurBmpResItem = m_CurBmpResItem;
	
	((CILBitmapShape*)*ppSymbol)->m_BitmapResourceList.RemoveAll();
	ILPOSITION Pos = m_BitmapResourceList.GetHeadPosition();
	while( Pos )
	{
		((CILBitmapShape*)*ppSymbol)->m_BitmapResourceList.AddTail( m_BitmapResourceList.GetAt( Pos ) );
		m_BitmapResourceList.GetNext( Pos );
	}
}

void CILBitmapShape::Serialize( CArchive &ar )
{
	CILShape::Serialize( ar );

	if( ar.IsStoring() )
	{
		ar << m_CurBmpResItem.m_strBmpName;
		ar << m_CurBmpResItem.m_ResID;
		ar << m_bAllowStretching;
	}

	else
	{
		ar >> m_CurBmpResItem.m_strBmpName;
		ar >> m_CurBmpResItem.m_ResID;
		ar >> m_bAllowStretching;
	}
}