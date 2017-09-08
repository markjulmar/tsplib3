/******************************************************************************/
//                                                                        
// PARKPROPS.CPP - Park properties
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
#include "ParkProps.h"
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
IMPLEMENT_DYNCREATE(CParkProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CParkProps, CDialog)
	//{{AFX_MSG_MAP(CParkProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CParkProps::CParkProps
//
// Constructor for the park properties
//
CParkProps::CParkProps() : CDialog(CParkProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CParkProps)
	m_fDirected = FALSE;
	m_fNonDirected = FALSE;
	//}}AFX_DATA_INIT

}// CParkProps::CParkProps

///////////////////////////////////////////////////////////////////////////
// CParkProps::DoDataExchange
//
// Dialog data exchange
//
void CParkProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParkProps)
	DDX_Check(pDX, IDC_DIRECTED, m_fDirected);
	DDX_Check(pDX, IDC_NONDIRECTED, m_fNonDirected);
	//}}AFX_DATA_MAP

}// CParkProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CParkProps::OnOK
//
// Dismiss the dialog
//
void CParkProps::OnOK() 
{
	UpdateData(TRUE);

	if (!m_fDirected && !m_fNonDirected)
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(GetDlgItem(IDC_DIRECTED));
		return;
	}

	TSPWizardaw.m_Dictionary["PARKMODE_DIRECTED"] = (m_fDirected) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["PARKMODE_NONDIRECTED"] = (m_fNonDirected) ? "Yes" : "No";

	if (m_fDirected && !m_fNonDirected)
		TSPWizardaw.m_Dictionary["PARK_MODE"] = "LINEPARKMODE_DIRECTED";
	else if (m_fDirected && m_fNonDirected)
		TSPWizardaw.m_Dictionary["PARK_MODE"] = "LINEPARKMODE_DIRECTED | LINEPARKMODE_NONDIRECTED";
	else
		TSPWizardaw.m_Dictionary["PARK_MODE"] = "LINEPARKMODE_NONDIRECTED";

	CDialog::OnOK();

}// CParkProps::OnOK
