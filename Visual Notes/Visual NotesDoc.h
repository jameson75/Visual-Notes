// Visual NotesDoc.h : interface of the CVisualNotesDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUALNOTESDOC_H__D330321E_E800_456F_81D4_D30AF7B19A38__INCLUDED_)
#define AFX_VISUALNOTESDOC_H__D330321E_E800_456F_81D4_D30AF7B19A38__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CVisualNotesDoc : public CDocument
{
protected: // create from serialization only
	CVisualNotesDoc();
// Implementation
public:
	virtual ~CVisualNotesDoc();

// Attributes
public:
	CILList<CILDBSet*,CILDBSet*>		m_DBSetList;
	CILDBSet*	m_pDBSet;
	UINT	m_AppVersionMajor;
	UINT	m_AppVersionMinor;
	
// Operations
public:
	void SetCurrentDBSet( UINT id );
	CILDBSet* FindDBSet( UINT id );
	BOOL PeekOrphanage( CILDBSet *pExceptSet, CILList<CILSymbol*,CILSymbol*> *pListWIF, 
						CILList<CILDBSet*,CILDBSet*> *pOrphanList );
	void DeleteDBSet( UINT id );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualNotesDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

protected:
	void ReadDiagrams( CArchive& ar );
	void ReadSymbols( CILDBSet *pDBSet, CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );
	void ReadChildMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );
	void ReadSymbiantMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );
	void ReadJointMap( CILList<CILSymbol*, CILSymbol*> *pSymbolList, CArchive& ar );
	void ReadLinkedSymbolMap( CILList<CILSymbol*,CILSymbol*> *pProjectSymbols, CArchive& ar );

	CILSymbol* _FindSymbolByID( UINT id, CILList<CILSymbol*,CILSymbol*> *pSymbolList );

	CILSymbol* _SymbolFromName( CString& strClassName );
	CString _NameFromSymbol( CILSymbol *pSymbol );

	void WriteDiagrams( CArchive& ar );
	void WriteSymbols( CILDBSet *pDBSet, CArchive& ar );
	void WriteChildMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );
	void WriteJointMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );
	void WriteSymbiantMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );
	void WriteLinkedSymbolMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void DeleteContents();	

// Generated message map functions
protected:
	//{{AFX_MSG(CVisualNotesDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CVisualNotesDoc)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALNOTESDOC_H__D330321E_E800_456F_81D4_D30AF7B19A38__INCLUDED_)
