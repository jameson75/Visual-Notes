class CILBmpResItem
{
public:
	CString m_strBmpName;
	UINT	m_ResID;

public:
	CILBmpResItem() { Init(); }

	void Init() { m_strBmpName = TEXT(""); m_ResID = 0; } 
	CILBmpResItem& operator = ( CILBmpResItem &ResItem )
	{
		m_ResID = ResItem.m_ResID;
		m_strBmpName = ResItem.m_strBmpName;
		return *this;
	}

	BOOL operator == ( CILBmpResItem ResItem )
	{ return (m_ResID == ResItem.m_ResID); }
};

class CILBitmapShape : public CILShape
{
protected:
	static BOOL								m_bDefAllowStreching;
	
	BOOL									m_bAllowStretching;
	CILList<CILBmpResItem, CILBmpResItem&>	m_BitmapResourceList;

protected:
	CILBmpResItem							m_CurBmpResItem;

	
public:
	CILBitmapShape();
	CILBitmapShape( UINT l, UINT t, UINT r, UINT b );

	//override.
	UINT DoModalPropBox( CWnd *pWnd = NULL, CDialog *pExtdDlg = NULL );
	//override.
	BOOL Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList,
				CILList<CILSymbol*,CILSymbol*> *pJointList );
	//adds resource ids to the symbol's bitmap resource list.
	void AssociateBitmapResource( UINT resID, CString strBmpName );
	//sets the current bitmap resource that will be used during rendering.
	inline void SetCurrentBitmapRes( CILBmpResItem& ResItem ) { m_CurBmpResItem = ResItem; }
	//override to set USER default current bitmap.
	virtual void SetDefaultBmpID( UINT m_ID ) = 0;
	//override to get USER default current bitmap.
	virtual UINT GetDefaultBmpID() = 0;
	//override to get APPLICATION default current bitmap.	
	virtual UINT GetAppDefaultBmpID() = 0;
	//initializes object to it's application defualt bitmap.
	void Initialize();

	void Serialize( CArchive &ar );
	
protected:
	virtual void _RenderBitmap( CDC *pDC );
	void _CopyBaseData( CILSymbol **ppSymbol );
	DECLARE_DYNAMIC( CILBitmapShape );
};