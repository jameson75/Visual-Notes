//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/24/01
//*********************************************
#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "ColorButton.h"
#include "ILColorPicker.h"
#include "SymbolTextDlg.h"

IMPLEMENT_SERIAL( CILLabel, CILShape, VERSIONABLE_SCHEMA | 1 );

CString CILLabel::ms_strDefaultText = CString("Label");

CILLabel::CILLabel()
{
	m_LabelText = ms_strDefaultText;
	m_FontSize = 9;
	m_dwFontColor = RGB( 255,0,0);
	m_FontName = "Tahoma";
	m_bItalic = m_bBold = m_bCentered = m_bUnderline = FALSE;

}

CILLabel::CILLabel( UINT l, UINT t, UINT r, UINT b ) : CILShape(l,t,r,b)
{
	m_LabelText = "Label";
	m_FontSize = 9;
	m_dwFontColor = RGB( 0,0,0 );
	m_FontName = "Tahoma";
	m_bItalic = m_bBold = m_bCentered = m_bUnderline = FALSE;
}

void CILLabel::DrawSymbol( CDC *pDC )
{
	CFont LabelFont;
	CRect BoundingRectWS; 
	CSize LogicalStringSize;
	CString SubString;
	
	UINT fCenter = ( m_bCentered ) ? DT_CENTER : DT_LEFT;
	
	int nSavedDC = pDC->SaveDC();
	int LogicalCharWidth = 0; 
	int MaxCharsPerLine = 0;

	CILShape::DrawSymbol(pDC);
	
	//Get text rect in window space.
	BoundingRectWS = m_BoundingRect;
	BoundingRectWS.OffsetRect( m_CanvasOrigin );
		
//	LabelFont.CreatePointFont( m_FontSize * 10, m_FontName );
	_MakeLabelFont( &LabelFont, pDC );
		
	//Set new device state.	
	pDC->SelectObject( &LabelFont );
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( m_dwFontColor );

	
	CRect rectText = m_BoundingRect;
	rectText.OffsetRect( m_CanvasOrigin );
	pDC->DrawText( m_LabelText, rectText, fCenter | DT_WORDBREAK );
	
	//restore previous device state.
	pDC->RestoreDC( nSavedDC );
	LabelFont.DeleteObject();
}

UINT CILLabel::DoModalPropBox( CWnd *pParent, CDialog *pExtdDlg )
{
	CSymbolTextDlg TextDlg(pParent);
	UINT retVal = 0;
	
	TextDlg.m_MinFontSize = 9;
	TextDlg.m_MaxFontSize = 24;

	TextDlg.m_SymbolText = m_LabelText;
	TextDlg.m_strFontSize.Format( TEXT("%d"), m_FontSize );
	TextDlg.m_dwFontColor = m_dwFontColor;
	TextDlg.m_strFontStyle = m_FontName;
	TextDlg.m_bCentered = m_bCentered;
	TextDlg.m_bBold = m_bBold;
	TextDlg.m_bItalic = m_bItalic;
	TextDlg.m_bUnderline = m_bUnderline;

	if( (retVal = TextDlg.DoModal()) == IDOK )
	{
		//A Label's text should NEVER be empty.
		if( TextDlg.m_SymbolText.IsEmpty() )
			m_LabelText = ms_strDefaultText;
		else
			m_LabelText = TextDlg.m_SymbolText;
		
		m_dwFontColor = TextDlg.m_dwFontColor;
		m_FontName = TextDlg.m_strFontStyle;
		m_bCentered = TextDlg.m_bCentered;
		m_bBold = TextDlg.m_bBold;
		m_bItalic = TextDlg.m_bItalic;
		m_bUnderline = TextDlg.m_bUnderline;
		
		//determine which font size the user has chosen.
		for( int i = TextDlg.m_MinFontSize; i <= TextDlg.m_MaxFontSize; i++ )
		{
			//we need to convert the string value to integer value.
			CString NumString;
			NumString.Format( TEXT("%d"), i );
			if( NumString == TextDlg.m_strFontSize )
			{
				m_FontSize = i;
				break;
			}
		}

		//determine the new bounding rect of the text.
		//we'll have to set the label's font as the current font to get 
		//he appropiate size of the bounding rectangle.
		//--------------------------------------------------------------
		CRect rectNewText = m_BoundingRect;
		CFont Font;
		CDC* pDC = pParent->GetDC();
		int nSavedDC = pDC->SaveDC();
		UINT fCenter = ( m_bCentered ) ? DT_CENTER : DT_LEFT;
		
		_MakeLabelFont( &Font, pDC );
		pDC->SelectObject( &Font );	
		
		rectNewText.OffsetRect( m_CanvasOrigin );
		pDC->DrawText( m_LabelText, &rectNewText, DT_CALCRECT /*| DT_SINGLELINE*/ | fCenter | DT_WORDBREAK );
		SetBoundingRect( rectNewText );
		Font.DeleteObject();
		pDC->RestoreDC( nSavedDC );
	}

	return retVal;
}

void CILLabel::_CopyBaseData( CILSymbol **ppSymbol )
{
	CILShape::_CopyBaseData( ppSymbol );
	
	((CILLabel*)(*ppSymbol))->m_dwFontColor = m_dwFontColor;
	((CILLabel*)(*ppSymbol))->m_FontName = m_FontName;
	((CILLabel*)(*ppSymbol))->m_FontSize = m_FontSize;
	((CILLabel*)(*ppSymbol))->m_LabelText = m_LabelText;
	((CILLabel*)(*ppSymbol))->m_bCentered = m_bCentered;
	((CILLabel*)(*ppSymbol))->m_bBold =		m_bBold;
	((CILLabel*)(*ppSymbol))->m_bItalic =	m_bItalic;
	((CILLabel*)(*ppSymbol))->m_bUnderline = m_bUnderline;
}

void CILLabel::_New( CILSymbol **ppSymbol )
{ *ppSymbol = new CILLabel; }

void CILLabel::Serialize( CArchive &ar )
{
	CILShape::Serialize( ar );

	if( ar.IsStoring() )
	{
		ar << m_LabelText;
		ar << m_FontName;
		ar << m_FontSize;
		ar << m_dwFontColor;
		ar << m_bCentered;
		ar << m_bBold;
		ar << m_bItalic;
		ar << m_bUnderline;
	}

	else
	{
		ar >> m_LabelText;
		ar >> m_FontName;
		ar >> m_FontSize;
		ar >> m_dwFontColor;
		ar >> m_bCentered;
		ar >> m_bBold;
		ar >> m_bItalic;
		ar >> m_bUnderline;
	}
}

void CILLabel::_MakeLabelFont( CFont *pFont, CDC *pDC )
{
	CFont TempFont;
	TEXTMETRIC tm;
	int nSavedDC = pDC->SaveDC();
	UINT fWeight = ( m_bBold ) ? FW_BOLD : FW_NORMAL;
	
	//Use the DC to help convert our point font sized specified font to a
	//logical unit size specified font.
	//--------------------------------------------------------------------

	TempFont.CreatePointFont( m_FontSize * 10, m_FontName );
	pDC->SelectObject( &TempFont );
	GetTextMetrics( pDC->m_hDC, &tm ); //now we can use the tmHieght member as our logical size!
	
	pFont->CreateFont( tm.tmHeight, 0, 0, 0, fWeight, m_bItalic, m_bUnderline, 
						  0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
						  CLIP_DEFAULT_PRECIS, DEFAULT_PITCH | FF_SWISS, m_FontName );

	pDC->RestoreDC( nSavedDC );
	TempFont.DeleteObject();
}











