// Visual Notes.h : main header file for the VISUAL NOTES application
//

#if !defined(AFX_VISUALNOTES_H__BCBEA917_A500_4B56_BE99_D9F1D0E859E0__INCLUDED_)
#define AFX_VISUALNOTES_H__BCBEA917_A500_4B56_BE99_D9F1D0E859E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ILDBSet.h"

#define APPDEFAULT_REMOVECHILDREN		TRUE
#define APPDEFAULT_CANVASX				400
#define APPDEFAULT_CANVASY				APPDEFAULT_CANVASX
#define APPDEFAULT_CIRCULARDEPALERTOFF	FALSE
#define APPDEFAULT_ALLOWSTRECHINGBMP	FALSE

#define COMBO_TOOLSET_ID 1

typedef struct
{
	SYSTEMTIME	SysTime;
	wchar_t		szAuthCode[256];
	wchar_t		szUserName[256];
	SHORT		m_MajorVersion;
	SHORT		m_MinorVersion;
	
}IL_AUTHORIZATION_INFO;


/////////////////////////////////////////////////////////////////////////////
// CVisualNotesApp:
// See Visual Notes.cpp for the implementation of this class
//

class CVisualNotesApp : public CWinApp
{
private:
	CString m_strAppKey;
	CString	m_strUserName;
	CString	m_strAuthCode;
	CString	m_strFileFilter;
	CString	m_strFileFolder;

public:
	CVisualNotesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualNotesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVisualNotesApp)
	afx_msg void OnAppAbout();	
	afx_msg void OnToolsSettings();
	afx_msg void OnToolsDiagram();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	enum APPMODE
	{
		APPMODE_LINKSYMBOLS,
		APPMODE_CREATELABEL,
		APPMODE_CREATESYMBOL,
		APPMODE_TRACKEDIT,
		APPMODE_SELECTSYMBOL,
		APPMODE_TRACKSYMBOL,
		APPMODE_CREATEEDGE,
		APPMODE_NIL
	}m_AppMode;

	enum TREEDIR { treedirUp = 0, treedirDown = 1, treedirLeft = 2, treedirRight = 3 } m_TreeDir;

	enum ALIGNTYPE { AS_TOP, AS_BOTTOM, AS_RIGHT, AS_LEFT, AS_SIZE, AS_CENTER };
	
	//Application defined symbol flags.
	const DWORD m_fPrevShapeInBuild; //0x00000001
	const DWORD m_fLinkChild;		 //0x00000002
	const DWORD m_fPrevJointInBuild; //0x00000004
	const DWORD m_fLastCreatedShape;  //0x00000008
	const DWORD m_fControlShape;	 //0x0000000F

	//{{PERSISTING_SETTINGS
	BOOL m_bRemoveChildren; 
	BOOL m_bAllowStrechingBMP;
	BOOL m_bUnlinkOnCD;
	BOOL m_bLockDocument;
	BOOL m_bAutoRemoveLink;
	BOOL m_bDirectEdgeAA;
	CSize m_CanvasSize;

	BOOL m_bDefRemoveChildren; 
	BOOL m_bDefAllowStrechingBMP;
	BOOL m_bDefUnlinkOnCD;
	BOOL m_bDefLockDocument;
	BOOL m_bDefAutoRemoveLink;
	BOOL m_bDefDirectEdgeAA;
	CSize m_DefCanvasSize;
	//}}PERSISTING_SETTINGS	

public:	

	BOOL CreateShape( CRect CreateRect );
	BOOL CreateEdge( CPoint ptClick );
	void TrackSymbol( CPoint ptClick );
	void CreateLabel( CPoint ptClick );
	BOOL SelectSymbol( CPoint ptClick );
	void LinkSymbols( CPoint ptClick );
			
	void DisplayProperties();
	void Delete();
	void Undo();
	void Redo();

	void OnChangeUI( WPARAM	wParam );
	void DeleteSnapshotList( CILList<CILSymbol*,CILSymbol*> *pSnapshotList );
	void CreateSnapshotList( CILList<CILSymbol*,CILSymbol*> *pOriginalList, CILList<CILSymbol*,CILSymbol*> *pSnapshotList );
	void JointListToSymbolList( CILList<CILJoint*,CILJoint*> *pJointList, CILList<CILSymbol*,CILSymbol*> *pSymbolList );

	BOOL LoadUserSettings();
	void SaveUserSettings();
	BOOL VerifyLicense();
	BOOL VerifyAuthorizationCode( CString string );
	void RestoreDefaultSettings();
	
	UINT ConfirmedDeleteDBSet( UINT id );
	UINT ConfirmedRemoveLinks( CILList<CILSymbol*,CILSymbol*> *pListRL, int ConfirmMode );
	UINT ConfirmedRemoveLinks( CILSymbol *pSymbolRL, int ConfirmMode );
	
	void DoModalDiagramView( int DVMode, CILSymbol *pAttachSymbol );
	void SetLastCreatedShape( CILSymbol *pSymbol );

	void AlignSymbols( ALIGNTYPE AlignType );
	void OffsetList( CSize Offset, CILList<CILSymbol*,CILSymbol*> *pList );
	void MultiSelectMove( CSize Offset );
	BOOL MultiSelectSymbol( CRect *pRect );

	//convserve indirection to document data.
	//----------------------------------------
	
	CILDBSet* GetDBSet();
	CILSymbolDB* GetSymbolDatabase();
	CILTransactionCoord* GetSymbolTransactionCoord();

	//conserve indirection to doucument operations.
	//---------------------------------------------
	CILDBSet* FindDBSet( UINT id );

	void OnHelp();	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALNOTES_H__BCBEA917_A500_4B56_BE99_D9F1D0E859E0__INCLUDED_)
