#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "UmlClassPropBox.h"

IMPLEMENT_SERIAL( CILUmlClass, CILShape, VERSIONABLE_SCHEMA | 1 );

int CILUmlClass::ms_TitleBarHeight = 16;
int CILUmlClass::ms_MethodOffset = 10;

CILUmlClass::CILUmlClass() : CILShape()
{
	m_Title.LoadString( IDS_UMLCLASS_EMPTYTITLEBAR );
}

CILUmlClass::CILUmlClass( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{
	m_Title.LoadString( IDS_UMLCLASS_EMPTYTITLEBAR );
}

void CILUmlClass::DrawSymbol( CDC *pDC )
{	
	CRect rectTitleBar;
	CRect rectMethodView;
	CRect rectUp;
	CRect rectDown;
	CPoint TriUp[3];
	CPoint TriDown[3];
	CFont TitleFont;
	CSize TextExtent;
	
	CBrush BGBrush;
	CBrush SecondaryBrush;
	CPen EdgePen;
	int nSavedDC;

	CILShape::DrawSymbol( pDC );
	
	rectTitleBar = m_BoundingRect;
	rectMethodView = m_BoundingRect;
	rectTitleBar.OffsetRect( m_CanvasOrigin );
	rectMethodView.OffsetRect( m_CanvasOrigin );

	rectTitleBar.bottom = rectTitleBar.top + ms_TitleBarHeight;
	rectMethodView.top = rectTitleBar.bottom - 1;

	BGBrush.CreateSolidBrush( m_dwBGColor );
	EdgePen.CreatePen( PS_SOLID, m_uEdgeWidth, m_dwEdgeColor );
	SecondaryBrush.CreateSolidBrush( m_dwSecondaryColor );

	nSavedDC = pDC->SaveDC();
	
	pDC->SelectObject( &EdgePen );

	pDC->SelectObject( &SecondaryBrush );
	pDC->Rectangle( &rectTitleBar );
	pDC->SelectObject( &BGBrush );
	pDC->Rectangle( &rectMethodView );
	pDC->SetBkMode( TRANSPARENT );

	TitleFont.CreatePointFont( 83, TEXT("Tahoma") );
	pDC->SelectObject( &TitleFont );
	pDC->ExtTextOut( rectTitleBar.left + m_uEdgeWidth, 
					 rectTitleBar.top + m_uEdgeWidth,
					 ( ETO_CLIPPED ), &rectTitleBar, m_Title, NULL );
	
	CString strMethod;
	int nMethod = 0;
	ILPOSITION Pos = m_MethodList.GetHeadPosition();
	while( Pos )
	{ 
		strMethod = m_MethodList.GetAt( Pos );
		TextExtent = pDC->GetTextExtent( strMethod );
		pDC->ExtTextOut( rectMethodView.left + m_uEdgeWidth + ms_MethodOffset, 
						 rectMethodView.top + m_uEdgeWidth + (TextExtent.cy * nMethod ),
						 ( ETO_CLIPPED ), &rectMethodView, strMethod, NULL );
		nMethod++;
		m_MethodList.GetNext( Pos );
	}

		
	BGBrush.DeleteObject();
	EdgePen.DeleteObject();
	SecondaryBrush.DeleteObject();
	TitleFont.DeleteObject();
	pDC->RestoreDC( nSavedDC );	
}

void CILUmlClass::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILUmlClass; }

void CILUmlClass::_CopyBaseData( CILSymbol **ppSymbol )
{ 
	CILShape::_CopyBaseData( ppSymbol );

	((CILUmlClass*)*ppSymbol)->m_Title = m_Title;
	
	((CILUmlClass*)*ppSymbol)->m_MethodList.RemoveAll();
	ILPOSITION Pos = m_MethodList.GetHeadPosition();
	while( Pos )
	{
		((CILUmlClass*)*ppSymbol)->m_MethodList.AddTail( m_MethodList.GetAt( Pos ) );
		m_MethodList.GetNext( Pos );
	}
}

void CILUmlClass::AddMethod( CString strMethod )
{ m_MethodList.AddTail( strMethod ); }

BOOL CILUmlClass::RemoveMethod( CString strMethod )
{
	ILPOSITION Pos = m_MethodList.GetHeadPosition();
	while( Pos )
	{
		if( strMethod == m_MethodList.GetAt( Pos ) )
			break;
		m_MethodList.GetNext( Pos );
	}

	if( !Pos )
		return FALSE;

	m_MethodList.RemoveAt( Pos );
	return TRUE;
}

void CILUmlClass::SetTitle( CString strMethod )
{ m_Title = strMethod; }

UINT CILUmlClass::DoModalPropBox( CWnd *pWnd, CDialog *pExtdDlg )
{
	CUmlClassPropBox PropBox;
	UINT retVal;
	
	PropBox.m_strClassTitle = m_Title;
	PropBox.m_strlstMethods.AddTail( &m_MethodList );
	retVal = CILShape::DoModalPropBox( pWnd, &PropBox );
	
	if( PropBox.m_retID == IDOK )
	{
		if( PropBox.m_strClassTitle == _T("") )
			m_Title = TEXT("No Title");
		else
			m_Title = PropBox.m_strClassTitle;
		
		m_MethodList.RemoveAll();
		m_MethodList.AddTail( &PropBox.m_strlstMethods );
	}

	//if any dialog in the chain returned with idOK, we need to let the caller know,
	//that a change to this object was made.
	if( retVal == IDOK || PropBox.m_retID == IDOK )
		return IDOK;

	return IDCANCEL;
}

void CILUmlClass::Serialize( CArchive &ar )
{
	LONG nMethods;
	CString strMethod;

	CILShape::Serialize( ar );
	
	if( ar.IsStoring() )
	{
		ar << m_Title;
		nMethods = m_MethodList.GetCount();
		ar << nMethods;
		ILPOSITION Pos = m_MethodList.GetHeadPosition();
		while( Pos )
		{
			ar << m_MethodList.GetAt( Pos );
			m_MethodList.GetNext( Pos );
		}
	}

	else
	{
		ar >> m_Title;
		ar >> nMethods;
		for( LONG i = 0; i < nMethods; i++ )
		{
			ar >> strMethod;
			m_MethodList.AddTail( strMethod );
		}
	}
}
		
