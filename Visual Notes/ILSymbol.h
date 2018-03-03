//*********************************************
//Developer: Eugene Adams
//Invisible Labs
//Copyright 2001 - 2002
//*********************************************

//*********************************************
//Start Date: 11/16/01
//*********************************************

class CILSymbol : public CObject
{
private:
	static UINT						m_InstanceCount; //for testing purposes.
	UINT   m_ID;
	

protected:
	CILList<CILSymbol*,CILSymbol*>	m_ChildList;
	CILList<CILSymbol*,CILSymbol*>	m_JointList;
	CILSymbol*						m_pParent;
	BOOL							m_bShowTracker;
	BOOL							m_bShowSelection;
	CPoint							m_CanvasOrigin;
	BOOL							m_bHighlight;

	CString							m_strNoteLink;
	UINT							m_EmbeddedID;
	UINT							m_DiagramID;
	CString							m_strCaption;
	BOOL							m_bCenterCaption;
	BOOL							m_bHidden;
	
public:
	static CSize					ms_MinSize;
	static CSize					ms_MaxSize;
	
public:
	DWORD							m_dwExtraData;
	CRect							m_BoundingRect;
	static UINT						m_HighID;	
	
public:
	CILSymbol();
	~CILSymbol();
		
	//override to draw symbol to the device.
	virtual void DrawSymbol( CDC *pDC ) = 0;
	//override to display symbol's property box.
	virtual UINT DoModalPropBox( CWnd *pWnd = NULL, CDialog *pExtdDlg = NULL ) = 0;
	//override to perform hit test on symbol.
	virtual BOOL HitTest( CPoint CanvasPoint ) = 0;
	//override to track the symbol.
	virtual BOOL Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList,
						CILList<CILSymbol*,CILSymbol*> *pJointList ) = 0;
	//override to enable and disable tracker.
	virtual void ShowTracker( BOOL bShow, CWnd *pWnd = NULL ) = 0;
	//override to hit test tracker handles
	virtual BOOL TrackerHitTest( CPoint CanvasPoint ) = 0;
	//override to enable and disable selection highlight.
	virtual void ShowSelection( BOOL bShow, CWnd *pWnd ) = 0;
	//override to determine symbols selection state;
	virtual BOOL IsSelected() = 0;
	//override to determine symbols tracked state.
	virtual BOOL IsTracked() = 0;
	//override to highlight symbol with user specified parameters
	virtual void Highlight( BOOL bHighlight, CWnd *pWnd, int PenStyle, DWORD dwColor ) = 0;	
		
	//adds child to symbol
	virtual void AddChild( CILSymbol *pSymbol );
	//removes child from symbol.
	virtual BOOL RemoveChild( CILSymbol *pSymbol );
	//returns the parent symbol of this object.
	virtual CILSymbol* GetParent();
	//returns a copied list of immediate children or all children of the symbol.
	virtual BOOL GetChildren( CILList<CILSymbol*,CILSymbol*> *pChildList, BOOL bAllChildren = FALSE );
	//Determines if specified symbol is a child of this symbol.
	virtual BOOL IsChild( CILSymbol *pChild, BOOL bCheckAllLevels = TRUE );
	//sets the parent of this symbol
	virtual void SetParent( CILSymbol *pParent );
	//determines if the symbol has a parent.

	//adds a new joint to this symbol.
	virtual void AddJoint( CILSymbol *pJoint );
	//returns a copy of this symbols joint list.
	virtual BOOL GetJoints( CILList<CILSymbol*,CILSymbol*> *pJointList, BOOL bAllJoints = FALSE );
	//removes a joint from the this symbol
	virtual BOOL RemoveJoint( CILSymbol *pJoint );

	//override to invalidate the window coordinates of the symbol.
	virtual void InvalidateSymbol( CWnd *pWnd ) = 0;
	//sets the visible state of this symbol
	inline virtual void HideSymbol( BOOL bHide = TRUE ) { m_bHidden = bHide; }
	//indicates the visible state of this symbol
	inline virtual BOOL IsHidden() { return m_bHidden; }
	//gets the unique id of this symbol
	inline virtual UINT GetID() { return m_ID; }
	//determines if this symbol has any children.
	virtual BOOL HasChildren();
	//determinses if this symbols owns any joints.
	virtual BOOL HasJoints();
		
	//records the canvas' current origin in window coordinates.
	virtual void SetCanvasOrigin( CPoint CanvasOrigin );
	//sets the bounding rectangle.
	virtual void SetBoundingRect( CRect rect );
	//creates snapshot of this object.
	virtual void GetSnapshot( CILSymbol **ppSnapshot );
	//creates original object which created snap shot, and reintegrates original in it's environment.
	virtual void SetSnapshot( CILSymbol *pSnapshot );

	//retrieves a referenced (embedded) diagram's id.
	inline virtual UINT GetEmbeddedID() { return m_EmbeddedID; }
	//sets the id to a referenced (embedded) diagram.
	inline virtual void SetEmbeddedID( UINT id ) { m_EmbeddedID = id; }
	//sets the id of the diagram this symbol is apart of.
	inline virtual void SetDiagramID( UINT id ) { m_DiagramID = id; }
	//gets the id of the diagram this symbol is apart of.
	inline virtual UINT GetDiagramID() { return m_DiagramID; }
	//retrieves the name of an external note file and stores it in a buffer.
	virtual void GetNoteLink( CString *pNoteLink );
	//sets the name of an external note file.
	virtual void SetNoteLink( CString& NoteLink );
	//override to resize this symbol and adjust the symbols joints.
	virtual void Resize( CRect NewRect, CILList<CILSymbol*,CILSymbol*> *pJointList ) = 0;

	void Serialize( CArchive &ar );

protected:
	//copies data common between snaphot and original objects.
	virtual void _CopyBaseData( CILSymbol **ppSymbol );
	//override to create new instance of object.
	virtual void _New( CILSymbol **ppSymbol ) = 0;
	//raligns joints with this symbol
	virtual void _UpdateJoints( CILSymbol *pJoint, CSize TLOffset, CSize BROffset );
	//raligns joints with this symbol
	virtual void _UpdateJoints( CILList<CILSymbol*,CILSymbol*> *pUpdateJoints, CSize TLOffset, CSize BROffset );
	//override to render the symbols caption to the specified device.
	virtual void _RenderCaption( CDC *pDC ) = 0;

	DECLARE_DYNAMIC( CILSymbol );
};




