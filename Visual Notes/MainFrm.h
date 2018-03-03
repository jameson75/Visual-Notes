// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C36EE185_92FE_430A_9C1E_E33B581964A7__INCLUDED_)
#define AFX_MAINFRM_H__C36EE185_92FE_430A_9C1E_E33B581964A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Array for the toolbar buttons

#if (_WIN32_WCE < 201)
static TBBUTTON g_arCBButtons[] = {
	{ 0,	ID_FILE_NEW,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  0},
	{ 1,    ID_FILE_OPEN,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  1},
	{ 2,	ID_FILE_SAVE,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  2},
	{ 0,	0,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0, 0, -1},
	{ 3,    ID_EDIT_CUT,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  3},
	{ 4,	ID_EDIT_COPY,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  4},
	{ 5,	ID_EDIT_PASTE,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  5}
};
#endif

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 212)
#define NUM_TOOL_TIPS 8
#endif

#define IDUSER_TOOLBAR_GENERAL	50010
#define IDUSER_TOOLBAR_CREATE	50011


const int SelIndexFlow = 0;
const int SelIndexUml = 1;
const int SelIndexNet1 = 2;
const int SelIndexNet2 = 3;
const int SelIndexLogic = 4;

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	CComboBox	*m_pSTSSelection;

	enum FLOWCHARTSYMBOL{ 
		FLOWCHARTSYMBOL_CIRCLE, FLOWCHARTSYMBOL_DIAMOND, FLOWCHARTSYMBOL_RECTANGLE, FLOWCHARTSYMBOL_SHEAR,
		FLOWCHARTSYMBOL_GENERALIZE, FLOWCHARTSYMBOL_TAGA, FLOWCHARTSYMBOL_TAGB, FLOWCHARTSYMBOL_NIL 
	}m_FlowChartSymbol;

	enum UMLSYMBOL{
		UMLSYMBOL_ACTOR, UMLSYMBOL_PACKAGE, UMLSYMBOL_CASE, UMLSYMBOL_CLASS, UMLSYMBOL_COMPONENT,
		UMLSYMBOL_NOTE, UMLSYMBOL_DEPLOYMENT, UMLSYMBOL_CAPSULE, UMLSYMBOL_ROUNDRECT, 
		UMLSYMBOL_NIL
	}m_UmlSymbol;

	enum NET1SYMBOL{
		NET1SYMBOL_CLIENT, NET1SYMBOL_SERVER, NET1SYMBOL_DATABASE, NET1SYMBOL_CLOUD, NET1SYMBOL_PRINTER,
		NET1SYMBOL_HUB, NET1SYMBOL_GATEWAY, NET1SYMBOL_FIREWALL, NET1SYMBOL_MAINFRAME, NET1SYMBOL_RING,
		NET1SYMBOL_NIL
	}m_Net1Symbol;

	enum NET2SYMBOL{
		NET2SYMBOL_SATELLITE, NET2SYMBOL_DISH, NET2SYMBOL_CELLPHONE, NET2SYMBOL_PDA, NET2SYMBOL_THINKPAD,
		NET2SYMBOL_CAMERA, NET2SYMBOL_FAX, NET2SYMBOL_WIREPHONE, NET2SYMBOL_NIL
	}m_Net2Symbol;

	enum EDGESET1{
		EDGESET1_BASICEDGE, EDGESET1_COMPLEXEDGE, EDGESET1_DEPCOMPLEXEDGE, EDGESET1_DEPBASICEDGE, EDGESET1_NIL 
	}m_EdgeSet1;

	enum GENERALTOOL{
		GENERALTOOL_TEXT, GENERALTOOL_CREATE, GENERALTOOL_SELECT, GENERALTOOL_TRACKER, GENERALTOOL_PROPERTIES,
		GENERALTOOL_LINK, GENERALTOOL_EDGE, GENERALTOOL_TRACK, GENERALTOOL_NIL
	}m_GeneralTool;

	enum ALIGNSYMBOL{
		ALIGNSYMBOL_MOVE, ALIGNSYMBOL_NIL 
	}m_AlignSymbol;

	enum SYMBOLTOOLSET{
		SYMBOLTOOLSET_FLOWCHART, SYMBOLTOOLSET_UML, SYMBOLTOOLSET_NET1, SYMBOLTOOLSET_NET2, SYMBOLTOOLSET_ALIGN
	}m_CurrentSTS; 
	
	enum EDGETOOLSET{
		EDGETOOLSET_EDGESET1
	}m_CurrentETS;

	enum SELTOOLSET{ SELTOOLSET_ALIGN, SELTOOLSET_SHOWHIDE
	}m_CurrentSelTS;

	enum CREATEBARTYPE { 
		CBT_EDGE, CBT_SYMBOL, CBT_SELECT
	}m_CreateBarType;

	int			m_CurrentSTSSel;
	int			m_CurrentETSSel;
	int			m_CurrentSelTSSel;
	CToolBar	m_CreateToolBar;
	CToolBar	m_GeneralToolBar;
	BOOL		m_bShowTools;
		
	void LoadNewToolSet( SYMBOLTOOLSET ToolSet );	
	void LoadNewToolSet( EDGETOOLSET ToolSet );
	void LoadNewToolSet( SELTOOLSET SelToolSet );
	void UseCreateBarType( CREATEBARTYPE cst );
	BOOL AdjustViewFromTools( CRect *pAdjustRect );
	void ResetToolUI();

	afx_msg void OnSelectToolSet();
	void OnHighlightToolbtn( NMHDR *pNMHDR, LRESULT *pLResult );
	BOOL OnHelpInfo( HELPINFO* lpHelpInfo );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CCeCommandBar	m_wndCommandBar;
	
	

// Generated message map functions
protected:

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd); 	
	afx_msg void OnToolbtnCircle();
	afx_msg void OnToolbtnDiamond();
	afx_msg void OnToolbtnRectangle();
	afx_msg void OnToolbtnShear();
	afx_msg void OnUpdateToolbtnCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbtnDiamond(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbtnRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbtnShear(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnText();
	afx_msg void OnUpdateToolbtnText(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnSelect();
	afx_msg void OnUpdateToolbtnSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbtnCreate(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnCreate();
	afx_msg void OnToolbtnProperties();
	afx_msg void OnToolbtnDelete();
	afx_msg void OnToolbtnLink();
	afx_msg void OnUpdateToolbtnLink(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnEdge();
	afx_msg void OnUpdateToolbtnEdge(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnTrack();
	afx_msg void OnUpdateToolbtnTrack(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnShowhidetools();
	afx_msg void OnToolbtnBasicedge();
	afx_msg void OnUpdateToolbtnBasicedge(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnCmplxedge();
	afx_msg void OnUpdateToolbtnCmplxedge(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnRedo();
	afx_msg void OnToolbtnUndo();
	afx_msg void OnToolbtnUmlactor();
	afx_msg void OnUpdateToolbtnUmlactor(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnUmlcase();
	afx_msg void OnUpdateToolbtnUmlcase(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnUmlclass();
	afx_msg void OnUpdateToolbtnUmlclass(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnUmlcomponent();
	afx_msg void OnUpdateToolbtnUmlcomponent(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnUmlnote();
	afx_msg void OnUpdateToolbtnUmlnote(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnUmlpackage();
	afx_msg void OnUpdateToolbtnUmlpackage(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2camera();
	afx_msg void OnUpdateToolbtnNet2camera(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2cellphone();
	afx_msg void OnUpdateToolbtnNet2cellphone(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2dish();
	afx_msg void OnUpdateToolbtnNet2dish(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2fax();
	afx_msg void OnUpdateToolbtnNet2fax(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2pda();
	afx_msg void OnUpdateToolbtnNet2pda(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2satellite();
	afx_msg void OnUpdateToolbtnNet2satellite(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2thinkpad();
	afx_msg void OnUpdateToolbtnNet2thinkpad(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNet2wirephone();
	afx_msg void OnUpdateToolbtnNet2wirephone(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNetclient();
	afx_msg void OnUpdateToolbtnNetclient(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNetcloud();
	afx_msg void OnUpdateToolbtnNetcloud(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNetdatabase();
	afx_msg void OnUpdateToolbtnNetdatabase(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNethub();
	afx_msg void OnUpdateToolbtnNethub(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNetmainframe();
	afx_msg void OnUpdateToolbtnNetmainframe(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNetprinter();
	afx_msg void OnUpdateToolbtnNetprinter(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnNetserver();
	afx_msg void OnUpdateToolbtnNetserver(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnDepbasicedge();
	afx_msg void OnUpdateToolbtnDepbasicedge(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnDepcomplexedge();
	afx_msg void OnUpdateToolbtnDepcomplexedge(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnAlignBottom();
	afx_msg void OnToolbtnAlignLeft();
	afx_msg void OnToolbtnAlignSize();
	afx_msg void OnToolbtnAlignTop();
	afx_msg void OnToobtnAlignRight();
	afx_msg void OnToolbtnGeneralize();
	afx_msg void OnUpdateToolbtnGeneralize(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnTaga();
	afx_msg void OnUpdateToolbtnTaga(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnTagb();
	afx_msg void OnUpdateToolbtnTagb(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnDeployment();
	afx_msg void OnUpdateToolbtnDeployment(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnRoundrect();
	afx_msg void OnUpdateToolbtnRoundrect(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnCapsule();
	afx_msg void OnUpdateToolbtnCapsule(CCmdUI* pCmdUI);
	afx_msg void OnToolbtnAligncenter();
	afx_msg void OnToolbtnShowHidden();
	afx_msg void OnToolbtnHideSelected();	
	afx_msg void OnToolbtnNetring();
	afx_msg void OnUpdateToolbtnNetring(CCmdUI* pCmdUI);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C36EE185_92FE_430A_9C1E_E33B581964A7__INCLUDED_)
