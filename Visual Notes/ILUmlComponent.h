

class CILUmlComponent : public CILShape
{
private:
	static int ms_VerticleTagOffset;
	static CSize ms_TagSize;	
	static int ms_LeftComponentOffset;
	static int ms_InterfaceBallPadding;
	static CSize ms_InterfaceSize;
	
protected:
	CILList<CString,CString&> m_InterfaceList;
	int m_RightBaseOffset;

public:
	CILUmlComponent();
	CILUmlComponent( UINT l, UINT t, UINT r, UINT b );

	void DrawSymbol( CDC *pDC );
	void AddInterface( CString strInterface, CWnd *pWnd );
	BOOL RemoveInterface( CString strInterface, CWnd *pWnd );
	UINT DoModalPropBox( CWnd *pWnd = NULL, CDialog *pExtdDlg = NULL );

	void Serialize( CArchive &ar );

protected:
	void _New( CILSymbol **ppSymbol );
	void _CopyBaseData( CILSymbol **ppSymbol );
	void _OnChangeInterface( CWnd *pWnd );
	void _RenderCaption( CDC *pDC );

	DECLARE_SERIAL( CILUmlComponent );
};
