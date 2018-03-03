#define JOINTDATA_TOP		1
#define JOINTDATA_BOTTOM	2
#define JOINTDATA_RIGHT		4
#define JOINTDATA_LEFT		8
#define JOINTDATA_HCENTER	16
#define JOINTDATA_VCENTER	32
#define JOINTDATA_ANCHORED	64


#define ILJOINT_DEFAULT_EDGEWIDTH	1
#define ILJOINT_DEFAULT_BKGNDCLR	( RGB( 150, 150, 150) )
#define ILJOINT_DEFAULT_EDGECLR		0
#define ILJOINT_DEFAULT_MASTERSYMBOL jointNone
#define ILJOINT_DEFAULT_SLAVESYMBOL  jointArrow
#define ILJOINT_DEFAULT_EDGESTYLE	PS_SOLID

class CILJoint : public CILShape
{
public:
	enum JOINTSYMBOL 
	{ jointBlock,	jointDot, 
	  jointArrow,	jointAggregation, 
	  jointOne,		jointMany,
	  jointNone,	jointPlus,
	  jointMinus,	jointDep,
	  jointNIL
	};

protected:
	CILJoint*					m_pControlJoint;
	CILSymbol*					m_pMasterShape;
	CILList<CILJoint*,CILJoint*>	m_SymbiantList;
	static int					m_nDefJointEdgeWidth;
	static DWORD				m_dwDefJointBkgndColor;
	static DWORD				m_dwDefJointEdgeColor;
	static UINT					m_uDefEdgeStyle;
	static CSize				m_sizeJointRect;
	static JOINTSYMBOL			m_DefJointSymbol;
	static JOINTSYMBOL			m_DefSymbiantJointSymbol;
	
public:
	int			m_nEdgeStyle;
	UINT		m_BoundPadding;
	UINT		m_uPointSize;
	DWORD		m_dwExtraJointData;
	BOOL		m_bEdgeSelection;
	DWORD		m_dwSelectionWidth;
	DWORD		m_dwSelectionColor;
	int			m_HitTolerance;

	int			m_nJointEdgeWidth;
	DWORD		m_dwJointEdgeColor;
	DWORD		m_dwJointBkgndColor;
	JOINTSYMBOL m_JointSymbol;
	JOINTSYMBOL m_SymbiantJointSymbol;
	UINT		m_uEdgeStyle;

public:
	CILJoint();
	CILJoint( CPoint pos );
	
	void DrawSymbol( CDC *pDC );
	BOOL Track( CWnd *pWnd, CPoint WindowPoint, CILList<CILSymbol*,CILSymbol*> *pXYChildList,
		        CILList<CILSymbol*,CILSymbol*> *pJointList );
	void InvalidateSymbol( CWnd *pWnd );
	virtual BOOL GetUpdateRect( CRect *pRect );
	UINT DoModalPropBox( CWnd *pWnd, CDialog *pExtdDlg = NULL );
	void SetOrigin( CPoint Origin );
	CPoint GetOrigin();
	void AddSymbiantJoint( CILJoint *pSymbiant );
	void SetControlJoint( CILJoint *pControl );
	BOOL GetSymbiantJoints( CILList<CILJoint*,CILJoint*> *pSymbiantList );
	CILJoint* GetControlJoint();
	void SetMasterShape( CILSymbol* pMasterShape );
	CILSymbol* GetMasterShape();
	BOOL RemoveSymbiantJoint( CILSymbol *pJoint );
	BOOL HasSymbiants();
	void AxisAlign( CILJoint *pJoint );
	
	void AddChild( CILSymbol *pSymbol );
	void AddJoint( CILSymbol *pJoint );
	void ShowSelection( BOOL bSelection, CWnd *pWnd );
	BOOL IsSelected();
	BOOL HitTest( CPoint CanvasPoint );

	BOOL GetTempUpdateList( CILList<CILJoint*,CILJoint*> *pUpdateList, BOOL bClearList = TRUE );
	void ClearUpdateList();

	void Serialize( CArchive &ar );
	
protected:
	void _CopyBaseData( CILSymbol **ppSymbol );
	void _CopyReserveData( CILSymbol **ppSymbol );
	void _New( CILSymbol **ppSymbol );
	virtual void _RenderJointEdge( CDC *pDC );
	virtual void _RenderSelection( CDC *pDC );
	virtual BOOL _HitTestEdge( CPoint *pEdge, CPoint CanvasPoint );
	enum JOINTSYMBOL _StringToJointSymbol( CString& string );
	CString _JointSymbolToString( JOINTSYMBOL JointSymbol );

	DECLARE_SERIAL( CILJoint );
};
	
