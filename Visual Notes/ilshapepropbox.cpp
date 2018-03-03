// SymbolPropertyDlg.cpp : implementation file
//
#include "stdafx.h"
#include "stdil.h"
#include "resource.h"
#include "ColorButton.h"
#include "ILColorPicker.h"
#include "ILShapePropBox.h"

//#include <typeinfo.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapePropBox dialog


CShapePropBox::CShapePropBox(CWnd* pParent /*=NULL*/  )
	: CDialog(CShapePropBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShapePropBox)
	m_uEdgeWidth = 0;
	m_uSymbolHeight = 0;
	m_uSymbolWidth = 0;
	m_bMakeCurrent = FALSE;
	m_nDashedEdge = -1;
	//}}AFX_DATA_INIT

	m_dwEdgeClr = RGB(0,0,0);
	m_dwBkgndClr = RGB(0,0,0);

	m_pExtdDlg = NULL;
}

void CShapePropBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapePropBox)
	DDX_Text(pDX, IDC_EDIT_SYMBOLHEIGHT, m_uSymbolHeight);
	DDX_Text(pDX, IDC_EDIT_SYMBOLWIDTH, m_uSymbolWidth);
	DDX_Check(pDX, IDC_CHECK_MAKECURRENT, m_bMakeCurrent);
	DDX_Radio(pDX, IDC_RADIO_DASHEDEDGE, m_nDashedEdge);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShapePropBox, CDialog)
	//{{AFX_MSG_MAP(CShapePropBox)
	ON_BN_CLICKED(ID_BUTTON_MORE, OnButtonMore)
	ON_BN_CLICKED(IDC_BUTTON_RESTOREDEFAULT, OnButtonRestoreDefault)
	ON_BN_CLICKED(IDC_CLRBUTTON_EDGE, OnClrbuttonEdge)
	ON_BN_CLICKED(IDC_CLRBUTTON_CAPTION, OnClrbuttonCaption)
	ON_BN_CLICKED(IDC_CLRBUTTON_BKGND, OnClrbuttonBkgnd)
	ON_BN_CLICKED(IDC_CLRBUTTON_SECONDARY, OnClrbuttonSecondary)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EXTDDLG, OnButtonExtddlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapePropBox message handlers

BOOL CShapePropBox::OnInitDialog() 
{
	// TODO: Add extra initialization here.	
	CDialog::OnInitDialog();
	CStatic *pstcSymbolColors = (CStatic*)GetDlgItem( IDC_STATIC_SYMBOLCOLORS );
	CButton *pbtnExtdDlg = (CButton*)GetDlgItem( IDC_BUTTON_EXTDDLG );

	m_btnEdgeClr.Create( this, IDC_CLRBUTTON_EDGE, m_dwEdgeClr );
	m_btnBkgndClr.Create( this, IDC_CLRBUTTON_BKGND, m_dwBkgndClr );
	m_btnSecondaryClr.Create( this, IDC_CLRBUTTON_SECONDARY, m_dwSecondaryClr );
	m_btnCaptionClr.Create( this, IDC_CLRBUTTON_CAPTION, m_dwCaptionClr );

	//Prevent the static window from hiding the newly created color buttons.
	pstcSymbolColors->SetWindowPos( &wndBottom, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE  );

	m_btnEdgeClr.ShowWindow( SW_SHOW );
	m_btnBkgndClr.ShowWindow( SW_SHOW );
	m_btnSecondaryClr.ShowWindow( SW_SHOW );
	m_btnCaptionClr.ShowWindow( SW_SHOW );

	if( m_uPenStyle == PS_SOLID )
		m_nDashedEdge = 1;

	else
		m_nDashedEdge = 0;

	if( m_pExtdDlg )
		pbtnExtdDlg->EnableWindow();

	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShapePropBox::OnButtonMore() 
{
	// TODO: Add your control notification handler code here
	m_ShapePropBox2.DoModal();	
}

void CShapePropBox::OnButtonRestoreDefault() 
{
	// TODO: Add your control notification handler code here

	m_dwEdgeClr = m_dwDefEdgeClr;
	m_dwBkgndClr = m_dwDefBkgndClr;
	m_dwSecondaryClr = m_dwDefSecondaryClr;
	m_dwCaptionClr = m_dwDefCaptionClr;
	m_bMakeCurrent = TRUE; //make sure the default settings become the current settings.

	m_btnEdgeClr.m_dwButtonColor =  m_dwEdgeClr;
	m_btnBkgndClr.m_dwButtonColor =  m_dwBkgndClr;
	m_btnSecondaryClr.m_dwButtonColor =  m_dwSecondaryClr;
	m_btnCaptionClr.m_dwButtonColor = m_dwCaptionClr;

	m_btnEdgeClr.RedrawWindow();
	m_btnBkgndClr.RedrawWindow();
	m_btnSecondaryClr.RedrawWindow();
	m_btnCaptionClr.RedrawWindow();
	
	UpdateData( FALSE );	
}

void CShapePropBox::OnClrbuttonEdge() 
{
	// TODO: Add your control notification handler code here
	CILColorPicker ColorPicker(this);
	
	ColorPicker.m_dwResultColor = m_dwEdgeClr;
	if( IDOK == ColorPicker.DoModal() && 
		ColorPicker.m_dwResultColor != m_dwEdgeClr )
	{
		m_dwEdgeClr = ColorPicker.m_dwResultColor;
		m_btnEdgeClr.m_dwButtonColor = m_dwEdgeClr;
		m_btnEdgeClr.RedrawWindow();
	}	
}

void CShapePropBox::OnClrbuttonCaption() 
{
	// TODO: Add your control notification handler code here.
	CILColorPicker ColorPicker;
	
	ColorPicker.m_dwResultColor = m_dwCaptionClr;
	if( IDOK == ColorPicker.DoModal() && 
		ColorPicker.m_dwResultColor != m_dwCaptionClr )
	{
		m_dwCaptionClr = ColorPicker.m_dwResultColor;
		m_btnCaptionClr.m_dwButtonColor = m_dwCaptionClr;
		m_btnCaptionClr.RedrawWindow();
	}	
}

void CShapePropBox::OnClrbuttonBkgnd() 
{
	// TODO: Add your control notification handler code here.
	CILColorPicker ColorPicker;
	
	ColorPicker.m_dwResultColor = m_dwBkgndClr;
	if( IDOK == ColorPicker.DoModal() && 
		ColorPicker.m_dwResultColor != m_dwBkgndClr )
	{
		m_dwBkgndClr = ColorPicker.m_dwResultColor;
		m_btnBkgndClr.m_dwButtonColor = m_dwBkgndClr;
		m_btnBkgndClr.RedrawWindow();
	}	
}

void CShapePropBox::OnClrbuttonSecondary() 
{
	// TODO: Add your control notification handler code here.
	CILColorPicker ColorPicker;
	
	ColorPicker.m_dwResultColor = m_dwSecondaryClr;
	if( IDOK == ColorPicker.DoModal() && 
		ColorPicker.m_dwResultColor != m_dwSecondaryClr )
	{
		m_dwSecondaryClr = ColorPicker.m_dwResultColor;
		m_btnSecondaryClr.m_dwButtonColor = m_dwSecondaryClr;
		m_btnSecondaryClr.RedrawWindow();
	}	
}

void CShapePropBox::OnButtonExtddlg() 
{
	// TODO: Add your control notification handler code here.	
	m_pExtdDlg->DoModal();
}
