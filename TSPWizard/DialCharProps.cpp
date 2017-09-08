/******************************************************************************/
//                                                                        
// DIALCHARPROPS.CPP - dialing character properties
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
#include "DialCharProps.h"
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
IMPLEMENT_DYNCREATE(CDialCharProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CDialCharProps, CDialog)
	//{{AFX_MSG_MAP(CDialCharProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CDialCharProps::CDialCharProps
//
// Constructor for the line page
//
CDialCharProps::CDialCharProps() : CDialog(CDialCharProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CDialCharProps)
	m_fAnswer = FALSE;
	m_fBilling = FALSE;
	m_fDialtone = FALSE;
	//}}AFX_DATA_INIT

}// CDialCharProps::CDialCharProps

///////////////////////////////////////////////////////////////////////////
// CDialCharProps::DoDataExchange
//
// Dialog data exchange for the hold properties
//
void CDialCharProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialCharProps)
	DDX_Check(pDX, IDC_ANSWER, m_fAnswer);
	DDX_Check(pDX, IDC_BILLING, m_fBilling);
	DDX_Check(pDX, IDC_DIALTONE, m_fDialtone);
	//}}AFX_DATA_MAP

}// CDialCharProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CDialCharProps::OnOK
//
// Dismiss the dialog
//
void CDialCharProps::OnOK() 
{
	UpdateData(TRUE);

	if (!m_fAnswer && !m_fBilling && !m_fDialtone)
	{
		OnCancel();
		return;
	}

	CString strFlags = "";
	if (m_fAnswer)
		strFlags = "LINEDEVCAPFLAGS_DIALQUIET";
	if (m_fBilling)
	{
		if (!strFlags.IsEmpty())
			strFlags += " | ";
		strFlags += "LINEDEVCAPFLAGS_DIALBILLING";
	}
	if (m_fDialtone)
	{
		if (!strFlags.IsEmpty())
			strFlags += " | ";
		strFlags += "LINEDEVCAPFLAGS_DIALDIALTONE";
	}

	TSPWizardaw.m_Dictionary["DIALCHAR_ANSWER"] = (m_fAnswer) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["DIALCHAR_BILLING"] = (m_fBilling) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["DIALCHAR_DIALTONE"] = (m_fDialtone) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["DIALCHAR_FLAGS"] = strFlags;

	CDialog::OnOK();

}// CDialCharProps::OnOK
