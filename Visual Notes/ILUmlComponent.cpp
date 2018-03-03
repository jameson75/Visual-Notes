#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "UmlComponentPropBox.h"

IMPLEMENT_SERIAL( CILUmlComponent, CILShape, VERSIONABLE_SCHEMA | 1 );

int CILUmlComponent::ms_VerticleTagOffset = 3;
int CILUmlComponent::ms_LeftComponentOffset = 7;
int CILUmlComponent::ms_InterfaceBallPadding = 1;
CSize CILUmlComponent::ms_InterfaceSize = CSize( 15, 12 );
CSize CILUmlComponent::ms_TagSize = CSize( 15, 5 );

CILUmlComponent::CILUmlComponent() : m_RightBaseOffset(0) 
{}

CILUmlComponent::CILUmlComponent( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b), m_RightBaseOffset(0)
{}

void CILUmlComponent::DrawSymbol( CDC *pDC )
{
	CBrush BGBrush, SecondaryBrush;
	CPen EdgePen;
	CRect ComponentRect;
	CRect TagRect1, TagRect2;
	CFont InterfaceFont;
	CRect InterfaceRect;
	int nSavedDC = pDC->SaveDC();
	
	CILShape::DrawSymbol( pDC );
	
	BGBrush.CreateSolidBrush( m_dwBGColor );
	SecondaryBrush.CreateSolidBrush( m_dwSecondaryColor );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );
	pDC->SelectObject( &BGBrush );
	pDC->SelectObject( &EdgePen);

	//draw main rectangle of component.
	ComponentRect = m_BoundingRect;
	ComponentRect.left+= ms_LeftComponentOffset;
	ComponentRect.right-= m_RightBaseOffset;
	ComponentRect.OffsetRect( m_CanvasOrigin );
	pDC->Rectangle( &ComponentRect );

	pDC->SelectObject( &SecondaryBrush );

	//Draw component tags.
	TagRect1 = m_BoundingRect;
	TagRect1.top += ms_VerticleTagOffset;
	TagRect1.right = TagRect1.left + ms_TagSize.cx;
	TagRect1.bottom = TagRect1.top + ms_TagSize.cy;
	TagRect2 = TagRect1;
	TagRect2.OffsetRect( 0, ms_TagSize.cy + ms_VerticleTagOffset );
	TagRect1.OffsetRect( m_CanvasOrigin );
	TagRect2.OffsetRect( m_CanvasOrigin );
	pDC->Rectangle( &TagRect1 );
	pDC->Rectangle( &TagRect2 );	
	
	//Draw all interfaces.
	CString strInterface;
	CRect rectInterface;
	CRect InterfaceCircleBounds;
	CRect rectText;
	CPoint InterfaceLine[2];
	CSize TextExtent;
	CRect BaseRect;
	int nInterface = 0;
	
	pDC->SetBkMode( TRANSPARENT );

	BaseRect = m_BoundingRect;
	BaseRect.right-= m_RightBaseOffset;
	InterfaceFont.CreatePointFont( 80, TEXT("Arial") );
	pDC->SelectObject( &InterfaceFont );
	ILPOSITION Pos = m_InterfaceList.GetHeadPosition();
	while( Pos )
	{		
		strInterface = m_InterfaceList.GetAt( Pos );

		rectInterface.top = BaseRect.top;
		rectInterface.left = BaseRect.right - 1;
		rectInterface.right = rectInterface.left + ms_InterfaceSize.cx;
		rectInterface.bottom = rectInterface.top + ms_InterfaceSize.cy;
		
		//offset interface according to it's position.
		rectInterface.OffsetRect( 0, nInterface * ms_InterfaceSize.cy ); 
		//only draw visible interfaces.
		if( rectInterface.bottom > BaseRect.bottom )
			break;
		
		rectInterface.OffsetRect( m_CanvasOrigin );
		
		//Draw interface circle.
		InterfaceCircleBounds = rectInterface;
		InterfaceCircleBounds.left = InterfaceCircleBounds.right - InterfaceCircleBounds.Height();
		InterfaceCircleBounds.DeflateRect( ms_InterfaceBallPadding, ms_InterfaceBallPadding );
		pDC->Ellipse( &InterfaceCircleBounds );
		
		//Draw interface line.
		InterfaceLine[0].x = rectInterface.left;
		InterfaceLine[0].y = rectInterface.top + ( ms_InterfaceSize.cy / 2 );
		InterfaceLine[1].x = InterfaceCircleBounds.left;
		InterfaceLine[1].y = InterfaceLine[0].y;
		pDC->Polyline( InterfaceLine, 2 );
		
		TextExtent = pDC->GetTextExtent( strInterface );
		rectText.top = rectInterface.top;
		rectText.left = rectInterface.right;
		rectText.bottom = rectInterface.bottom;
		rectText.right = rectText.left + TextExtent.cx;

		//Draw interface text.
		pDC->ExtTextOut( rectText.left, rectText.top, ETO_CLIPPED, 
				        &rectText, strInterface, strInterface.GetLength(), NULL );

		nInterface++;
		m_InterfaceList.GetNext( Pos );
	}	
	
	_RenderCaption( pDC );

	BGBrush.DeleteObject();
	EdgePen.DeleteObject();
	SecondaryBrush.DeleteObject();
	InterfaceFont.DeleteObject();
	pDC->RestoreDC( nSavedDC );
}

void CILUmlComponent::AddInterface( CString strInterface, CWnd *pWnd )
{
	m_InterfaceList.AddTail( strInterface );
	_OnChangeInterface( pWnd );
}

BOOL CILUmlComponent::RemoveInterface( CString strInterface, CWnd *pWnd )
{ 
	ILPOSITION Pos = m_InterfaceList.GetHeadPosition();
	while( Pos )
	{
		if( strInterface == m_InterfaceList.GetAt( Pos ) )
			break;
		m_InterfaceList.GetNext( Pos );
	}
	
	//if string was not found return FALSE.
	if( !Pos )
		return FALSE;

	m_InterfaceList.RemoveAt( Pos );
	_OnChangeInterface( pWnd );
	return TRUE;
}

void CILUmlComponent::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILUmlComponent; }

void CILUmlComponent::_CopyBaseData( CILSymbol **ppSymbol )
{
	CILShape::_CopyBaseData( ppSymbol );

	((CILUmlComponent*)*ppSymbol)->m_InterfaceList.RemoveAll();
	ILPOSITION Pos = m_InterfaceList.GetHeadPosition();
	while( Pos )
	{
		((CILUmlComponent*)*ppSymbol)->m_InterfaceList.AddTail( m_InterfaceList.GetAt( Pos ) );
		m_InterfaceList.GetNext( Pos );
	}

	((CILUmlComponent*)*ppSymbol)->m_RightBaseOffset = m_RightBaseOffset;
}


void CILUmlComponent::_OnChangeInterface( CWnd *pWnd )
{
	CSize TextExtent;
	int CurrentOffset = 0;
	int OffsetDifference = 0;
	CString string;

	//Each time a new interface is added or subtracted (changed), we need to check 
	//for the smallest bounding rect which encloses this symbol.

	//if a change occured that resulted in the list being empty, it could only mean the
	//LAST interface was just removed. In this situation, we would like to reduce 
	// the bounding rect down to the size of the base rect.
	if( m_InterfaceList.IsEmpty() )
	{
		CRect NewBoundingRect;
		NewBoundingRect = m_BoundingRect;
		NewBoundingRect.right-= m_RightBaseOffset;
		m_RightBaseOffset = 0;
	}

	//else enumerate interfaces to deterimine smallest enclosing rectangle.
	else
	{
		CRect BaseRect = m_BoundingRect;
		BaseRect.right-= m_RightBaseOffset;

		ILPOSITION Pos = m_InterfaceList.GetHeadPosition();
		while( Pos )
		{
			string = m_InterfaceList.GetAt( Pos );
			TextExtent = pWnd->GetDC()->GetTextExtent( string );
			CurrentOffset = ms_InterfaceSize.cx + TextExtent.cx;
			if( CurrentOffset > m_RightBaseOffset )
				m_RightBaseOffset = CurrentOffset;
					
			m_InterfaceList.GetNext( Pos );
		}
 	
		CRect NewBoundingRect = BaseRect;
		NewBoundingRect.right+= m_RightBaseOffset;
		SetBoundingRect( &NewBoundingRect );
	}
}

UINT CILUmlComponent::DoModalPropBox( CWnd *pWnd, CDialog *pExtdDlg )
{
	
	UINT retVal;
	CUmlComponentPropBox PropBox;

	PropBox.m_strlstInterfaces.AddTail( &m_InterfaceList );
	retVal = CILShape::DoModalPropBox( pWnd, &PropBox );

	if( PropBox.m_retID == IDOK )
	{
		m_InterfaceList.RemoveAll();
		m_InterfaceList.AddTail( &PropBox.m_strlstInterfaces );
		_OnChangeInterface( pWnd ); 
	}

	if( PropBox.m_retID == IDOK || retVal == IDOK )
		return IDOK;

	else 
		return IDCANCEL;
}

void CILUmlComponent::_RenderCaption( CDC *pDC )
{
	CRect BaseRect;
	int nSavedDC = pDC->SaveDC();

	BaseRect = m_BoundingRect;
	BaseRect.right-= m_RightBaseOffset;
	BaseRect.left+= ms_LeftComponentOffset;
	BaseRect.OffsetRect( m_CanvasOrigin );


	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( m_strCaption, &BaseRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	pDC->RestoreDC( nSavedDC );
}

void CILUmlComponent::Serialize( CArchive &ar )
{
	CString strInterface;
	LONG nInterfaces;

	CILShape::Serialize( ar );
	
	if( ar.IsStoring() )
	{
		ar << m_RightBaseOffset;
		nInterfaces = m_InterfaceList.GetCount();
		ar << nInterfaces;
		ILPOSITION Pos = m_InterfaceList.GetHeadPosition();
		while( Pos )
		{
			ar << m_InterfaceList.GetAt( Pos );
			m_InterfaceList.GetNext( Pos );
		}
	}

	else
	{
		ar >> m_RightBaseOffset;
		ar >> nInterfaces;
		for( LONG i = 0; i < nInterfaces; i++ )
		{
			ar >> strInterface;
			m_InterfaceList.AddTail( strInterface );
		}
	}
}



