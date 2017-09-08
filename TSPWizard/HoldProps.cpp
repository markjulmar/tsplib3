/******************************************************************************/
//                                                                        
// HOLDPROPS.CPP - Hold properties
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
#include "HoldProps.h"
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
IMPLEMENT_DYNCREATE(CHoldProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CHoldProps, CDialog)
	//{{AFX_MSG_MAP(CHoldProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CHoldProps::CHoldProps
//
// Constructor for the line page
//
CHoldProps::CHoldProps() : CDialog(CHoldProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CHoldProps)
	m_strMaxOnHold = _T("1");
	m_fHoldMakesNew = FALSE;
	m_fSwapHold = FALSE;
	//}}AFX_DATA_INIT

}// CHoldProps::CHoldProps

///////////////////////////////////////////////////////////////////////////
// CHoldProps::DoDataExchange
//
// Dialog data exchange for the hold properties
//
void CHoldProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoldProps)
	DDX_Control(pDX, IDC_SPIN2, m_ctlSpin1);
	DDX_Text(pDX, IDC_MAXTRANSFINCONF, m_strMaxOnHold);
	DDX_Check(pDX, IDC_HOLDMAKESNEW, m_fHoldMakesNew);
	DDX_Check(pDX, IDC_SWAPHOLD, m_fSwapHold);
	//}}AFX_DATA_MAP

}// CHoldProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CHoldProps::OnOK
//
// Dismiss the dialog
//
void CHoldProps::OnOK() 
{
	UpdateData(TRUE);

	if (m_strMaxOnHold.IsEmpty() || atoi(m_strMaxOnHold) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXTRANSFINCONF));
		return;
	}

	TSPWizardaw.m_Dictionary["MAXONHOLD"] = m_strMaxOnHold;
	TSPWizardaw.m_Dictionary["HOLDMAKESNEW"] = (m_fHoldMakesNew) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["HOLDCANSWAP"] = (m_fSwapHold) ? "Yes" : "No";

	CDialog::OnOK();

}// CHoldProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CHoldProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CHoldProps::OnInitDialog() 
{
	// Connect the controls
	CDialog::OnInitDialog();
	
	// Max on hold calls
	m_ctlSpin1.SetRange(1, 1000);

	return TRUE;

}// CHoldProps::OnInitDialog
