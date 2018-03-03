// ILColorPicker.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ColorButton.h"
#include "ILColorPicker.h"

#include "stdil.h"
#include "Visual Notes.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILColorPicker dialog

CILColorPicker::CILColorPicker(CWnd* pParent /*=NULL*/)
	: CDialog(CILColorPicker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CILColorPicker)
	m_uGreen = 0;
	m_uBlue = 0;
	m_uRed = 0;
	//}}AFX_DATA_INIT
	m_dwResultColor = 0;

}

void CILColorPicker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILColorPicker)
	DDX_Text(pDX, IDC_EDIT_GREEN, m_uGreen);
	DDV_MinMaxUInt(pDX, m_uGreen, 0, 255);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_uBlue);
	DDV_MinMaxUInt(pDX, m_uBlue, 0, 255);
	DDX_Text(pDX, IDC_EDIT_RED, m_uRed);
	DDV_MinMaxUInt(pDX, m_uRed, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CILColorPicker, CDialog)
	//{{AFX_MSG_MAP(CILColorPicker)
	ON_BN_CLICKED(IDC_CLRBUTTON01, OnClrbutton01)
	ON_BN_CLICKED(IDC_CLRBUTTON02, OnClrbutton02)
	ON_BN_CLICKED(IDC_CLRBUTTON03, OnClrbutton03)
	ON_BN_CLICKED(IDC_CLRBUTTON04, OnClrbutton04)
	ON_BN_CLICKED(IDC_CLRBUTTON05, OnClrbutton05)
	ON_BN_CLICKED(IDC_CLRBUTTON06, OnClrbutton06)
	ON_BN_CLICKED(IDC_CLRBUTTON07, OnClrbutton07)
	ON_BN_CLICKED(IDC_CLRBUTTON08, OnClrbutton08)
	ON_BN_CLICKED(IDC_CLRBUTTON09, OnClrbutton09)
	ON_BN_CLICKED(IDC_CLRBUTTON10, OnClrbutton10)
	ON_BN_CLICKED(IDC_CLRBUTTON11, OnClrbutton11)
	ON_BN_CLICKED(IDC_CLRBUTTON12, OnClrbutton12)
	ON_BN_CLICKED(IDC_CLRBUTTON13, OnClrbutton13)
	ON_BN_CLICKED(IDC_CLRBUTTON14, OnClrbutton14)
	ON_BN_CLICKED(IDC_CLRBUTTON15, OnClrbutton15)
	ON_BN_CLICKED(IDC_CLRBUTTON16, OnClrbutton16)
	ON_BN_CLICKED(IDC_CLRBUTTON17, OnClrbutton17)
	ON_BN_CLICKED(IDC_CLRBUTTON18, OnClrbutton18)
	ON_BN_CLICKED(IDC_CLRBUTTON19, OnClrbutton19)
	ON_BN_CLICKED(IDC_CLRBUTTON20, OnClrbutton20)
	ON_BN_CLICKED(IDC_CLRBUTTON21, OnClrbutton21)
	ON_BN_CLICKED(IDC_CLRBUTTON22, OnClrbutton22)
	ON_BN_CLICKED(IDC_CLRBUTTON23, OnClrbutton23)
	ON_BN_CLICKED(IDC_CLRBUTTON24, OnClrbutton24)
	ON_BN_CLICKED(IDC_CLRBUTTON25, OnClrbutton25)
	ON_BN_CLICKED(IDC_CLRBUTTON26, OnClrbutton26)
	ON_BN_CLICKED(IDC_CLRBUTTON27, OnClrbutton27)
	ON_BN_CLICKED(IDC_CLRBUTTON28, OnClrbutton28)
	ON_BN_CLICKED(IDC_CLRBUTTON29, OnClrbutton29)
	ON_BN_CLICKED(IDC_CLRBUTTON30, OnClrbutton30)
	ON_BN_CLICKED(IDC_CLRBUTTON31, OnClrbutton31)
	ON_BN_CLICKED(IDC_CLRBUTTON32, OnClrbutton32)
	ON_BN_CLICKED(IDC_CLRBUTTON33, OnClrbutton33)
	ON_BN_CLICKED(IDC_CLRBUTTON34, OnClrbutton34)
	ON_BN_CLICKED(IDC_CLRBUTTON35, OnClrbutton35)
	ON_BN_CLICKED(IDC_CLRBUTTON36, OnClrbutton36)
	ON_BN_CLICKED(IDC_CLRBUTTON37, OnClrbutton37)
	ON_BN_CLICKED(IDC_CLRBUTTON38, OnClrbutton38)
	ON_BN_CLICKED(IDC_CLRBUTTON39, OnClrbutton39)
	ON_BN_CLICKED(IDC_CLRBUTTON40, OnClrbutton40)
	ON_BN_CLICKED(IDC_CLRBUTTON41, OnClrbutton41)
	ON_BN_CLICKED(IDC_CLRBUTTON42, OnClrbutton42)
	ON_BN_CLICKED(IDC_CLRBUTTON43, OnClrbutton43)
	ON_BN_CLICKED(IDC_CLRBUTTON44, OnClrbutton44)
	ON_BN_CLICKED(IDC_CLRBUTTON45, OnClrbutton45)
	ON_BN_CLICKED(IDC_CLRBUTTON46, OnClrbutton46)
	ON_BN_CLICKED(IDC_CLRBUTTON47, OnClrbutton47)
	ON_BN_CLICKED(IDC_CLRBUTTON48, OnClrbutton48)
	ON_BN_CLICKED(IDC_CLRBUTTON49, OnClrbutton49)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILColorPicker message handlers

extern CVisualNotesApp theApp;

BOOL CILColorPicker::OnInitDialog() 
{
	// TODO: Add extra initialization here.
	CDialog::OnInitDialog();
	
	//16 million colors, NCOLORBTNS choices... 
	DWORD dwScale = (DWORD)pow(256,3) / (NCOLORBTNS );
	
	for( int i = 0, j = IDC_CLRBUTTON01; i < NCOLORBTNS; i++, j++ )
	{
		m_ClrBtn[i].Create( this, j );
		m_ClrBtn[i].m_dwButtonColor = (i * dwScale);
	}

	m_ClrBtn[NCOLORBTNS - 1].m_dwButtonColor = RGB( 255, 255, 255 );

	m_CurrentClrBtn.Create( this, IDC_BUTTON_CLRPICK );
	m_CurrentClrBtn.m_dwButtonColor = m_dwResultColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CILColorPicker::OnChooseColor()
{
	BYTE bColor;
	bColor = (BYTE)( m_CurrentClrBtn.m_dwButtonColor >> 16 );
	m_uBlue = bColor;
	bColor = (BYTE)( m_CurrentClrBtn.m_dwButtonColor >> 8 );
	m_uGreen = bColor;
	m_uRed = (BYTE)( m_CurrentClrBtn.m_dwButtonColor );
	
	m_CurrentClrBtn.RedrawWindow();
	UpdateData( FALSE );
}

void CILColorPicker::OnClrbutton01() 
{
	// TODO: Add your control notification handler code here
	
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[0].m_dwButtonColor;
	OnChooseColor();
	
}
void CILColorPicker::OnClrbutton02() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[1].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton03() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[2].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton04() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[3].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton05() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[4].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton06() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[5].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton07() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[6].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton08() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[7].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton09() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[8].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton10() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[9].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton11() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[10].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton12() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[11].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton13() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[12].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton14() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[13].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton15() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[14].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton16() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[15].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton17()
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[16].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton18() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[17].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton19() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[18].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton20() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[19].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton21() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[20].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton22() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[21].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton23() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[22].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton24() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[23].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton25() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[24].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton26() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[25].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton27() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[26].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton28() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[27].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton29() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[28].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton30() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[29].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton31() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[30].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton32() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[31].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton33() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[32].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton34() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[33].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton35() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[34].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton36() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[35].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton37() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[36].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton38() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[37].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton39() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[38].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton40() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[39].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton41() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[40].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton42() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[41].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton43() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[42].m_dwButtonColor;
	OnChooseColor();
	
}

void CILColorPicker::OnClrbutton44() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[43].m_dwButtonColor;
	OnChooseColor();
}

void CILColorPicker::OnClrbutton45() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[44].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton46() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[45].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton47() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[46].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton48() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[47].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnClrbutton49() 
{
	// TODO: Add your control notification handler code here
	m_CurrentClrBtn.m_dwButtonColor = m_ClrBtn[48].m_dwButtonColor;
	OnChooseColor();	
}

void CILColorPicker::OnOK()
{
	m_dwResultColor = m_CurrentClrBtn.m_dwButtonColor;
	CDialog::OnOK();
}
