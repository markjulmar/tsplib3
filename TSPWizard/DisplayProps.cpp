/******************************************************************************/
//                                                                        
// DISPLAYPROPS.CPP - Display properties
//                                                                        
// Copyright (C) 1998 Mark C. Smith, JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
// For internal use only
//                                                                        
/******************************************************************************/

/*---------------------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "tspwizard.h"
#include "DisplayProps.h"
#include "TSPWizardaw.h"

/*---------------------------------------------------------------------------*/
// DEBUG STATEMENTS
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------------------*/
// RTTI
/*---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CDisplayProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CDisplayProps, CDialog)
	//{{AFX_MSG_MAP(CDisplayProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CDisplayProps::CDisplayProps
//
// Constructor for the display properties
//
CDisplayProps::CDisplayProps() : CDialog(CDisplayProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CDisplayProps)
	m_fWrite = FALSE;
	m_strHeight = _T("2");
	m_strWidth = _T("40");
	//}}AFX_DATA_INIT

}// CDisplayProps::CDisplayProps

///////////////////////////////////////////////////////////////////////////
// CDisplayProps::DoDataExchange
//
// Dialog data exchange for the display properties
//
void CDisplayProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayProps)
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Check(pDX, IDC_CANUPDATEDISPLAY, m_fWrite);
	DDX_Text(pDX, IDC_HEIGHT, m_strHeight);
	DDX_Text(pDX, IDC_WIDTH, m_strWidth);
	//}}AFX_DATA_MAP

}// CDisplayProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CDisplayProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CDisplayProps::OnInitDialog() 
{
	// Connect the variables up
	CDialog::OnInitDialog();
	
	m_ctlSpin1.SetRange(0, 50);
	m_ctlSpin2.SetRange(0, 500);

	return TRUE;

}// CDisplayProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CDisplayProps::OnOK
//
// Dismiss the dialog
//
void CDisplayProps::OnOK() 
{
	UpdateData(TRUE);

	if (atoi(m_strHeight) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_HEIGHT));
		return;
	}
	if (atoi(m_strWidth) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_WIDTH));
		return;
	}

	TSPWizardaw.m_Dictionary["DISPLAY_HEIGHT"] = m_strHeight;
	TSPWizardaw.m_Dictionary["DISPLAY_WIDTH"] = m_strWidth;
	TSPWizardaw.m_Dictionary["DISPLAY_CANWRITE"] = (m_fWrite) ? "Yes" : "No";

	CDialog::OnOK();

}// CDisplayProps::OnOK
