// Visual NotesDoc.cpp : implementation of the CVisualNotesDoc class
//

#include "stdafx.h"
#include "stdil.h"
#include "Visual Notes.h"

#include "Visual NotesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CVisualNotesDoc

IMPLEMENT_DYNCREATE(CVisualNotesDoc, CDocument)

BEGIN_MESSAGE_MAP(CVisualNotesDoc, CDocument)
	//{{AFX_MSG_MAP(CVisualNotesDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesDoc construction/destruction


CVisualNotesDoc::CVisualNotesDoc() : m_AppVersionMajor(1), m_AppVersionMinor(0)
{
	// TODO: add one-time construction code here
	m_pDBSet = NULL;
}

CVisualNotesDoc::~CVisualNotesDoc()
{}

BOOL CVisualNotesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	//each new document created will have it's own set of id's for each 
	//diagram page, within the docuemnt and each set will start with id 0.
	
	CILDBSet::m_HighID = ILDBSET_ID_FIRST;
	CILSymbol::m_HighID = 0;
	m_DBSetList.AddTail( m_pDBSet = new CILDBSet );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesDoc serialization

void CVisualNotesDoc::Serialize(CArchive& ar)
{
	CString strFileSig, strCheckSig;


	if (ar.IsStoring())
	{
		// TODO: add storing code here
		strFileSig.LoadString( IDS_APPSIG );
		
		//write header.
		ar << strFileSig;
		ar << m_AppVersionMajor;
		ar << m_AppVersionMinor;
		ar << CILDBSet::m_HighID;
		ar << CILSymbol::m_HighID;
		
		WriteDiagrams( ar );
	}

	else
	{	
		//TODO: add loading code here
		//We will need to first initialize the root database.
		CILDBSet::m_HighID = ILDBSET_ID_FIRST; //make sure root diagram get's this id.
		m_pDBSet = new CILDBSet;
		m_DBSetList.AddTail( m_pDBSet  );		
		
		//read header:
	
		//check file signature.
		ar >> strFileSig;
		strCheckSig.LoadString( IDS_APPSIG );
		if( strFileSig != strCheckSig )
		{			
			::MessageBox( NULL, _T( "Illegal file format" ), _T( "Error" ), MB_ICONERROR );
			ar.Abort();
			return;
		}				
		
		ar >> m_AppVersionMajor;
		ar >> m_AppVersionMinor;
		ar >> CILDBSet::m_HighID;
		ar >> CILSymbol::m_HighID;
	
	
		//read all diagrams.
		ReadDiagrams( ar );
	}
}

void CVisualNotesDoc::ReadDiagrams( CArchive &ar )
{
	LONG nDiagrams;
	CILDBSet *pDBSet = NULL;
	UINT DiagramID;
	CString strDiagramName;	
	CILList <CILSymbol*,CILSymbol*> AllProjectSymbols;
	CILList <CILSymbol*,CILSymbol*> DiagramSymbols;

	ar >> nDiagrams;	
	for( LONG i = 0; i < nDiagrams; i++ )
	{
		ar >> DiagramID;
		ar >> strDiagramName;

		//We only create and initialize embedded diagrams.
		if( DiagramID != ILDBSET_ID_FIRST )
		{
			pDBSet = new CILDBSet;
			pDBSet->SetID( DiagramID );
			pDBSet->SetName( strDiagramName );
			ReadSymbols( pDBSet, &DiagramSymbols, ar );
			m_DBSetList.AddTail( pDBSet );
		}

		//*********************************************************************
		//The root diagram is expected to be the current diagram at this time.*
		//It should already be instanciated and initiailized!				  *
		//*********************************************************************
		
		else 
			ReadSymbols( m_pDBSet, &DiagramSymbols, ar );

		AllProjectSymbols.AddTail( &DiagramSymbols );
		DiagramSymbols.RemoveAll();
	}

	ReadLinkedSymbolMap( &AllProjectSymbols, ar );
	AllProjectSymbols.RemoveAll();
}

void CVisualNotesDoc::ReadSymbols( CILDBSet *pDBSet, CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive &ar )
{
	CString strClassName;
	CILSymbol *pSymbol = NULL, *pRuntimeSymbol = NULL;
	LONG nSymbols;

	ar >> nSymbols;

	for( int i = 0; i < nSymbols; i++ )
	{
		ar >> strClassName;
		pRuntimeSymbol = _SymbolFromName( strClassName );
		ASSERT( pRuntimeSymbol );
		pSymbol = (CILSymbol*)ar.ReadObject( pRuntimeSymbol->GetRuntimeClass() );
		pSymbolList->AddTail( pSymbol );
		delete pRuntimeSymbol; pRuntimeSymbol = NULL;
	}

	ReadChildMap( pSymbolList, ar );
	ReadJointMap( pSymbolList, ar );
	ReadSymbiantMap( pSymbolList, ar );

	
	ILPOSITION Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		pSymbol = pSymbolList->GetAt( Pos );

		if( pSymbol->IsKindOf( RUNTIME_CLASS( CILJoint ) ) )
			pDBSet->m_DB.m_JointData.AddTail( pSymbol );
		else
			pDBSet->m_DB.m_SymbolData.AddTail( pSymbol );

		pSymbolList->GetNext( Pos );
	}
}

void CVisualNotesDoc::ReadChildMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, 
									 CArchive& ar )
{
	UINT ParentID, ChildID;
	LONG nParents, nChildren;
	CILSymbol *pParent = NULL;
	CILSymbol *pChild = NULL;

	ar >> nParents;
	for( LONG i = 0; i < nParents; i++ )
	{
		ar >> ParentID;
		pParent = _FindSymbolByID( ParentID, pSymbolList );
		ASSERT( pParent );

		ar >> nChildren;
		for( LONG j = 0; j < nChildren; j++ )
		{
			ar >> ChildID;
			pChild = _FindSymbolByID( ChildID, pSymbolList );
			ASSERT( pChild );

			pParent->AddChild( pChild );
			pChild->SetParent( pParent );
		}
	}
}

void CVisualNotesDoc::ReadJointMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, 
									 CArchive& ar )
{
	UINT MasterID, JointID;
	LONG nJoints, nMasters;
	CILSymbol *pMaster = NULL;
	CILSymbol *pJoint= NULL;

	ar >> nMasters;
	for( LONG i = 0; i < nMasters; i++ )
	{
		ar >> MasterID;
		pMaster = _FindSymbolByID( MasterID, pSymbolList );
		ASSERT( pMaster );

		ar >> nJoints;
		for( LONG j = 0; j < nJoints; j++ )
		{
			ar >> JointID;
			pJoint = _FindSymbolByID( JointID, pSymbolList );
			ASSERT( pJoint );
			ASSERT( pJoint->IsKindOf( RUNTIME_CLASS( CILJoint ) ) );
			((CILJoint*)pJoint)->SetMasterShape( pMaster );
			pMaster->AddJoint( pJoint );
		}
	}
}

void CVisualNotesDoc::ReadSymbiantMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList,
									    CArchive& ar )
{
	UINT ControlID, SymbiantID;
	LONG nControls, nSymbiants;
	CILSymbol *pControl = NULL;
	CILSymbol *pSymbiant = NULL;

	ar >> nControls;
	for( LONG i = 0; i < nControls; i++ )
	{
		ar >> ControlID;
		pControl = _FindSymbolByID( ControlID, pSymbolList );
		ASSERT( pControl );
		ASSERT( pControl->IsKindOf( RUNTIME_CLASS( CILJoint ) ) );

		ar >> nSymbiants;
		for( LONG j = 0; j < nSymbiants; j++ )
		{
			ar >> SymbiantID;
			pSymbiant = _FindSymbolByID( SymbiantID, pSymbolList );
			ASSERT( pSymbiant );
			ASSERT( pSymbiant->IsKindOf( RUNTIME_CLASS( CILJoint ) ) );

			((CILJoint*)pControl)->AddSymbiantJoint( (CILJoint*)pSymbiant );
			((CILJoint*)pSymbiant)->SetControlJoint( (CILJoint*)pControl );
		}
	}
}

void CVisualNotesDoc::ReadLinkedSymbolMap( CILList<CILSymbol*,CILSymbol*> *pProjectSymbols,
										    CArchive& ar )
{
	UINT DiagramID, LinkedSymbolID;
	LONG nRefDiagrams, nLinkedSymbols;
	CILDBSet *pRefDiagram = NULL;
	CILSymbol *pLinkedSymbol = NULL;

	ar >> nRefDiagrams;
	for( LONG i = 0; i < nRefDiagrams; i++ )
	{
		ar >> DiagramID;
		pRefDiagram = FindDBSet( DiagramID );
		ASSERT( pRefDiagram );

		ar >> nLinkedSymbols;
		for( LONG j = 0; j < nLinkedSymbols; j++ )
		{
			ar >> LinkedSymbolID;
			pLinkedSymbol = _FindSymbolByID( LinkedSymbolID, pProjectSymbols );
			ASSERT( pLinkedSymbol );

			pRefDiagram->AddLinkedSymbol( pLinkedSymbol );
		}
	}
}

void CVisualNotesDoc::WriteDiagrams( CArchive &ar )
{
	LONG nDiagrams;
	CILDBSet *pDBSet = NULL;
	UINT DiagramID;
	CILList<CILSymbol*,CILSymbol*> AllProjectSymbols;
	CILList<CILSymbol*,CILSymbol*> DiagramSymbols;
	CString strDiagramName;

	nDiagrams = m_DBSetList.GetCount();

	ar << nDiagrams;
	ILPOSITION Pos = m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		pDBSet = m_DBSetList.GetAt( Pos );
		pDBSet->GetName( strDiagramName );
		DiagramID = pDBSet->GetID();

		ar << DiagramID;
		ar << strDiagramName;

		WriteSymbols( pDBSet, ar );		
		
		pDBSet->m_DB.QueryAll( &AllProjectSymbols ); //<appends list>
		m_DBSetList.GetNext( Pos );
	}

	WriteLinkedSymbolMap( &AllProjectSymbols, ar );
}

void CVisualNotesDoc::WriteSymbols( CILDBSet *pDBSet, CArchive& ar )
{
	CString strClassName;
	CILSymbol *pSymbol = NULL;
	LONG nSymbols;
	CILList<CILSymbol*,CILSymbol*> SymbolList;

	pDBSet->m_DB.QueryAll( &SymbolList );
	nSymbols = SymbolList.GetCount();

	ar << nSymbols;

	ILPOSITION Pos = SymbolList.GetHeadPosition();
	while( Pos )
	{
		pSymbol = SymbolList.GetAt( Pos );
		
		strClassName = _NameFromSymbol( pSymbol );
		ASSERT( strClassName != _T("") );
		ar << strClassName;
		ar.WriteObject(  pSymbol );

		SymbolList.GetNext( Pos );
	}

	WriteChildMap( &SymbolList, ar );
	WriteJointMap( &SymbolList, ar );
	WriteSymbiantMap( &SymbolList, ar );
}

void CVisualNotesDoc::WriteChildMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive &ar )
{
	UINT ParentID, ChildID;
	LONG nParents, nChildren;
	CILSymbol *pParent = NULL;
	CILSymbol *pChild = NULL;
	ILPOSITION Pos, ChildPos;
	CILList<CILSymbol*,CILSymbol*> ChildList;
	
	nParents = 0;
	Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{									
		if( pSymbolList->GetAt( Pos )->HasChildren() )
			nParents++;
		 
		pSymbolList->GetNext( Pos );
	}

	ar << nParents;
	Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		pParent = pSymbolList->GetAt( Pos );
		if( pParent->GetChildren( &ChildList ) )
		{
			ParentID = pParent->GetID();
			ar << ParentID;
			
			nChildren = ChildList.GetCount();
			ar << nChildren;
			ChildPos = ChildList.GetHeadPosition();
			while( ChildPos )
			{
				pChild = ChildList.GetAt( ChildPos );
				ChildID = pChild->GetID();
				ar << ChildID;
				
				ChildList.GetNext( ChildPos );
			}
		}

		ChildList.RemoveAll();
		pSymbolList->GetNext( Pos );
	}
}

void CVisualNotesDoc::WriteJointMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar )
{
	UINT JointID, MasterID;
	LONG nJoints, nMasters;
	CILSymbol *pMaster = NULL;
	CILSymbol *pJoint = NULL;
	CILList<CILSymbol*,CILSymbol*> JointList;
	ILPOSITION Pos, JointPos;

	nMasters = 0;
	Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		if( pSymbolList->GetAt( Pos )->HasJoints() )
			nMasters++;
		pSymbolList->GetNext( Pos );
	}

	ar << nMasters;
	Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		pMaster = pSymbolList->GetAt( Pos );
		if( pMaster->GetJoints( &JointList ) )
		{
			MasterID = pMaster->GetID();
			ar << MasterID;

			nJoints = JointList.GetCount();
			ar << nJoints;
			JointPos = JointList.GetHeadPosition();
			while( JointPos )
			{
				pJoint = JointList.GetAt( JointPos );
				JointID = pJoint->GetID();
				ar << JointID;

				JointList.GetNext( JointPos );
			}
		}
		
		JointList.RemoveAll();
		pSymbolList->GetNext( Pos );
	}
}

void CVisualNotesDoc::WriteSymbiantMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar )
{
	UINT ControlID, SymbiantID;
	LONG nControls, nSymbiants;
	CILSymbol *pControl = NULL;
	CILSymbol *pSymbiant = NULL;
	CILList<CILJoint*,CILJoint*> SymbiantList;
	ILPOSITION Pos, SymbiantPos;

	nControls = 0;
	Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		if( pSymbolList->GetAt( Pos )->IsKindOf( RUNTIME_CLASS( CILJoint ) ) &&
			((CILJoint*)pSymbolList->GetAt( Pos ))->HasSymbiants() )
			nControls++;

		pSymbolList->GetNext( Pos );
	}

	ar << nControls;
	Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		pControl = pSymbolList->GetAt( Pos );
		if( pControl->IsKindOf( RUNTIME_CLASS( CILJoint ) ) &&
			((CILJoint*)pControl)->GetSymbiantJoints( &SymbiantList ) )
		{
			ControlID = pControl->GetID();
			ar << ControlID;

			nSymbiants = SymbiantList.GetCount();
			ar << nSymbiants;
			SymbiantPos = SymbiantList.GetHeadPosition();
			while( SymbiantPos )
			{
				pSymbiant = SymbiantList.GetAt( SymbiantPos );
				SymbiantID = pSymbiant->GetID();
				ar << SymbiantID;

				SymbiantList.GetNext( SymbiantPos );
			}
		}

		SymbiantList.RemoveAll();
		pSymbolList->GetNext( Pos );
	}
}

void CVisualNotesDoc::WriteLinkedSymbolMap( CILList<CILSymbol*,CILSymbol*> *pSymbolList, CArchive& ar )
{
	UINT DiagramID, LinkedSymbolID;
	LONG nRefDiagrams, nLinkedSymbols;
	CILDBSet *pRefDiagram = NULL;
	CILSymbol *pLinkedSymbol = NULL;
	CILList<CILSymbol*,CILSymbol*> LinkedSymbolList;
	ILPOSITION Pos, LinkPos;

	nRefDiagrams = 0;
	Pos = m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		if( !m_DBSetList.GetAt( Pos )->IsOrphan() )
			nRefDiagrams++;
		m_DBSetList.GetNext( Pos );	
	}

	ar << nRefDiagrams;
	Pos = m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		pRefDiagram = m_DBSetList.GetAt( Pos );
		if( !pRefDiagram->GetLinkedSymbols( &LinkedSymbolList ) )
		{
			DiagramID = pRefDiagram->GetID();
			ar << DiagramID;

			nLinkedSymbols = LinkedSymbolList.GetCount();
			ar << nLinkedSymbols;
			LinkPos = LinkedSymbolList.GetHeadPosition();
			while( LinkPos )
			{
				pLinkedSymbol = LinkedSymbolList.GetAt( LinkPos );
				LinkedSymbolID = pLinkedSymbol->GetID();
				ar << LinkedSymbolID;
				
				LinkedSymbolList.GetNext( LinkPos );
			}
		}

		LinkedSymbolList.RemoveAll();
		m_DBSetList.GetNext( Pos );
	}
}

CILSymbol* CVisualNotesDoc::_FindSymbolByID( UINT id, CILList<CILSymbol*,CILSymbol*> *pSymbolList )
{
	CILSymbol *pFindSymbol = NULL;

	ILPOSITION Pos = pSymbolList->GetHeadPosition();
	while( Pos )
	{
		if( pSymbolList->GetAt( Pos )->GetID() == id )
		{
			pFindSymbol = pSymbolList->GetAt( Pos );
			break;
		}
		
		pSymbolList->GetNext( Pos );
	}

	return pFindSymbol;
}

CString CVisualNotesDoc::_NameFromSymbol( CILSymbol *pSymbol )
{	return pSymbol->GetRuntimeClass()->m_lpszClassName;	 }

CILSymbol* CVisualNotesDoc::_SymbolFromName( CString& strClassName )
{	
	//flow chart symbols.
	//-------------------
	if( strClassName.CompareNoCase( TEXT( "CILRectangle" ) ) == 0 )
		return new CILRectangle;

	else if( strClassName.CompareNoCase( TEXT( "CILCircle" ) ) == 0 )
		return new CILCircle;

	else if( strClassName.CompareNoCase( TEXT( "CILDiamond" ) ) == 0 )
		return new CILDiamond;

	else if( strClassName.CompareNoCase( TEXT( "CILShear" ) ) == 0 )
		return new CILShear;

	else if( strClassName.CompareNoCase( TEXT( "CILGeneralize" ) ) == 0 )
		return new CILGeneralize;

	else if( strClassName.CompareNoCase( TEXT( "CILTagA" ) ) == 0 )
		return new CILTagA;

	else if( strClassName.CompareNoCase( TEXT( "CILTagB" ) ) == 0 )
		return new CILTagB;


	//uml symbols.
	//-----------
	else if( strClassName.CompareNoCase( TEXT( "CILUmlActor" ) ) == 0 )
		return new CILUmlActor;

	else if( strClassName.CompareNoCase( TEXT( "CILUmlNote" ) ) == 0 )
		return new CILUmlNote;

	else if( strClassName.CompareNoCase( TEXT( "CILUmlComponent" ) ) == 0 )
		return new CILUmlComponent;

	else if( strClassName.CompareNoCase( TEXT( "CILUmlPackage" ) ) == 0 )
		return new CILUmlPackage;

	else if( strClassName.CompareNoCase( TEXT( "CILUmlClass" ) ) == 0 )
		return new CILUmlClass;

	else if( strClassName.CompareNoCase( TEXT( "CILUmlDeployment" ) ) == 0 )
		return new CILUmlDeployment;

	else if( strClassName.CompareNoCase( TEXT( "CILRoundRect" ) ) == 0 )
		return new CILRoundRect;

	else if( strClassName.CompareNoCase( TEXT( "CILCapsule" ) ) == 0 )
		return new CILCapsule;


	//net1 symbols
	//-------------
	else if( strClassName.CompareNoCase( TEXT( "CILNetClient" ) ) == 0 )
		return new CILNetClient;

	else if( strClassName.CompareNoCase( TEXT( "CILNetServer" ) ) == 0 )
		return new CILNetServer;

	else if( strClassName.CompareNoCase( TEXT( "CILNetDatabase" ) ) == 0 )
		return new CILNetDatabase;

	else if( strClassName.CompareNoCase( TEXT( "CILNetCloud" ) ) == 0 )
		return new CILNetCloud;

	else if( strClassName.CompareNoCase( TEXT( "CILNetPrinter" ) ) == 0 )
		return new CILNetPrinter;

	else if( strClassName.CompareNoCase( TEXT( "CILNetHub" ) ) == 0 )
		return new CILNetHub;

	else if( strClassName.CompareNoCase( TEXT( "CILNetMainframe" ) ) == 0 )
		return new CILNetMainframe;

	else if( strClassName.CompareNoCase( TEXT( "CILNetRing" ) ) == 0 )
		return new CILNetRing;

	//net2 symbols
	//-------------
	else if( strClassName.CompareNoCase( TEXT( "CILNetSatellite" ) ) == 0 )
		return new CILNetSatellite;

	else if( strClassName.CompareNoCase( TEXT( "CILNetDish" ) ) == 0 )
		return new CILNetDish;

	else if( strClassName.CompareNoCase( TEXT( "CILNetCellPhone" ) ) == 0 )
		return new CILNetCellPhone;

	else if( strClassName.CompareNoCase( TEXT( "CILNetPDA" ) ) == 0 )
		return new CILNetPDA;

	else if( strClassName.CompareNoCase( TEXT( "CILNetThinkPad" ) ) == 0 )
		return new CILNetThinkPad;

	else if( strClassName.CompareNoCase( TEXT( "CILNetCamera" ) ) == 0 )
		return new CILNetCamera;

	else if( strClassName.CompareNoCase( TEXT( "CILNetFax" ) ) == 0 )
		return new CILNetFax;

	else if( strClassName.CompareNoCase( TEXT( "CILNetModem" ) ) == 0 )
		return new CILNetModem;

	//joint symbols.
	//--------------
	else if( strClassName.CompareNoCase( TEXT( "CILJoint" ) ) == 0 )
		return new CILJoint;

	else if( strClassName.CompareNoCase( TEXT( "CILJointTree" ) ) == 0 )
		return new CILJointTree;

	else if( strClassName.CompareNoCase( TEXT( "CILJointCorner" ) ) == 0 )
		return new CILJointCorner;


	//label
	//-----
	else if( strClassName.CompareNoCase( TEXT( "CILLabel" ) ) == 0 )
		return new CILLabel;

	else 
		return NULL;


}

BOOL CVisualNotesDoc::PeekOrphanage( CILDBSet *pExceptSet, CILList<CILSymbol*,CILSymbol*> *pListWIF, 
									  CILList<CILDBSet*,CILDBSet*> *pOrphanList )
{
	CILSymbol *pSymbolWIF = NULL;
	CILDBSet *pSet = NULL, *pTestSet = NULL;

	//simulate "what if" the following symbols were remvoed by removing symbols ( we'll put them back latter ).
	ILPOSITION Pos = pListWIF->GetHeadPosition();
	while( Pos )
	{
		pSymbolWIF = pListWIF->GetAt( Pos );
		pSet = FindDBSet( pSymbolWIF->GetEmbeddedID() );
		ASSERT( pSet );
		pSet->RemoveLinkedSymbol( pSymbolWIF );
		pListWIF->GetNext( Pos );
	}

	//After simulation, test each set in the project to determine if it would be orphaned.
	Pos = m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		pTestSet = m_DBSetList.GetAt( Pos );

		//do not test root for orpahange. Orphanage does not apply to root diagram.
		if( pTestSet->GetID() == ILDBSET_ID_FIRST )
		{
			m_DBSetList.GetNext( Pos );
			continue;
		}
		
		//if user specified a set to exclude from testing, don't test it, else test all sets for orphanage.
		if( pExceptSet )
		{
			if( pExceptSet != pTestSet && pTestSet->IsOrphan() )
				pOrphanList->AddTail( pTestSet );
		}
		
		else if( pTestSet->IsOrphan() )
			pOrphanList->AddTail( pTestSet );

		m_DBSetList.GetNext( Pos );
	}

	//To end simualtion, place back all symbols in there respective sets.
	Pos = pListWIF->GetHeadPosition();
	while( Pos )
	{
		pSymbolWIF = pListWIF->GetAt( Pos );
		pSet = FindDBSet( pSymbolWIF->GetEmbeddedID() );
		ASSERT( pSet );
		pSet->AddLinkedSymbol( pSymbolWIF );
		pListWIF->GetNext( Pos );
	}
	
	return !pOrphanList->IsEmpty();
}

void CVisualNotesDoc::SetCurrentDBSet( UINT id )
{
	CILDBSet *pSet = NULL;
	
	if( ( pSet = FindDBSet( id ) ) )
		m_pDBSet = pSet;
}

CILDBSet* CVisualNotesDoc::FindDBSet( UINT id )
{
	CILDBSet *pSet = NULL;

	ILPOSITION Pos = m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		pSet = m_DBSetList.GetAt( Pos );
		if( pSet->GetID() == id )
		{
			return pSet;
			break;
		}

		m_DBSetList.GetNext( Pos );
	}

	return NULL;
}

void CVisualNotesDoc::DeleteDBSet( UINT id )
{
	CILDBSet *pSet = NULL, *pRefSet = NULL;
	CILList<CILSymbol*,CILSymbol*> LinkedSymbolList, *pSymbolTable;
	CILSymbol *pDBSymbol = NULL, *pLinkedSymbol = NULL;

	//root diagram view cannot be deleted by the user, however it's link symbols need to be
	//updated, and since we are not deleting the diagram, we must explicitly remove the link
	//symbols.
	if( id == ILDBSET_ID_FIRST )
	{
		pSet = FindDBSet( id ); //root diagram always exists.
		pSet->GetLinkedSymbols( &LinkedSymbolList );
		ILPOSITION Pos = LinkedSymbolList.GetHeadPosition();
		while( Pos )
		{
			pLinkedSymbol = LinkedSymbolList.GetAt( Pos );
			pLinkedSymbol->SetEmbeddedID( ILDBSET_ID_NULL );
			pSet->RemoveLinkedSymbol( pLinkedSymbol );
			LinkedSymbolList.GetNext( Pos );
		}
	}

	else if( ( pSet = FindDBSet( id ) ) )
	{
		//We need to let linking symbols know that this diagram is being deleted.
		pSet->GetLinkedSymbols( &LinkedSymbolList );
		ILPOSITION Pos = LinkedSymbolList.GetHeadPosition();
		while( Pos )
		{
			LinkedSymbolList.GetAt( Pos )->SetEmbeddedID( ILDBSET_ID_NULL );
			LinkedSymbolList.GetNext( Pos );
		}

		//We also need to let referenced diagrams that this diagram is being deleted.
		pSymbolTable = pSet->m_DB.GetSymbolTable();
		Pos = pSymbolTable->GetHeadPosition();
		while( Pos )
		{
			pDBSymbol = pSymbolTable->GetAt( Pos );

			if( ( pRefSet = FindDBSet( pDBSymbol->GetEmbeddedID() ) ) )
				pRefSet->RemoveLinkedSymbol( pDBSymbol );
			
			pSymbolTable->GetNext( Pos );
		}

		m_DBSetList.RemoveAt( m_DBSetList.Find( pSet ) );
		delete pSet;
	}

	LinkedSymbolList.RemoveAll();
}

void CVisualNotesDoc::DeleteContents()
{
	CILDBSet *pDBSet = NULL;
	ILPOSITION Pos = m_DBSetList.GetHeadPosition();
	while( Pos )
	{
		pDBSet = m_DBSetList.GetAt( Pos );
		ASSERT( pDBSet );
		delete pDBSet;
		m_DBSetList.GetNext( Pos );
	}
	
	m_DBSetList.RemoveAll();
	m_pDBSet = NULL;

	CDocument::DeleteContents();
}

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesDoc diagnostics

#ifdef _DEBUG
void CVisualNotesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVisualNotesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisualNotesDoc commands


BOOL CVisualNotesDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// CDocData implementation



