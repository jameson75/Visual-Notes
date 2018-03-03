
class CILUmlClass : public CILShape
{
private:
	static int ms_TitleBarHeight;
	static int ms_MethodOffset;

protected:
	CString m_Title;
	CILList<CString,CString&> m_MethodList;

public:
	CILUmlClass();
	CILUmlClass( UINT l, UINT t, UINT r, UINT b );
	
	void DrawSymbol( CDC *pDC );
	UINT DoModalPropBox( CWnd *pWnd = NULL, CDialog *pExtdDlg = NULL );
	
	void AddMethod( CString strMethod );
	BOOL RemoveMethod( CString strMethod );
	void SetTitle( CString strMethod );

	void Serialize( CArchive &ar );

protected:
	void _New( CILSymbol **ppSymbol );
	void _CopyBaseData( CILSymbol **ppSymbol );
	DECLARE_SERIAL( CILUmlClass );
};


