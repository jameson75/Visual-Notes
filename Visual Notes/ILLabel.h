//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/24/01
//*********************************************

class CILLabel : public CILShape
{
private:
	static CString ms_strDefaultText;

public:
	CString m_LabelText;
	CString m_FontName;
	int		m_FontSize;
	DWORD	m_dwFontColor;
	BOOL	m_bCentered;
	BOOL	m_bBold;
	BOOL	m_bItalic;
	BOOL	m_bUnderline;

	
public:
	CILLabel();
	CILLabel( UINT l, UINT t, UINT r, UINT b);

	void DrawSymbol( CDC *pDC );
	UINT DoModalPropBox( CWnd *pParent = NULL, CDialog *pExtdDlg = NULL );

	void Serialize( CArchive &ar );

protected:
	void _CopyBaseData( CILSymbol **ppSymbol );
	void _New( CILSymbol **ppSymbol );
	void _MakeLabelFont( CFont *pFont, CDC *pDC );

	DECLARE_SERIAL( CILLabel );
};
	
