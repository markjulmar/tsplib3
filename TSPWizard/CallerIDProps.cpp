/******************************************************************************/
//                                                                        
// CALLERIDPROPS.CPP - Callerid properties
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
#include "CallerIDProps.h"
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
IMPLEMENT_DYNCREATE(CCallerIDProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCallerIDProps, CDialog)
	//{{AFX_MSG_MAP(CCallerIDProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCTSTR pszFlag;
	LPCTSTR pszTapi;

} g_Flags[] = {

	{ "Information Blocked by caller", "LINECALLPARTYID_BLOCKED" },
	{ "Out of Area", "LINECALLPARTYID_OUTOFAREA" },
	{ "Name", "LINECALLPARTYID_NAME" },
	{ "Phone Number", "LINECALLPARTYID_ADDRESS" },
	{ "Partial information available", "LINECALLPARTYID_PARTIAL" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCallerIDProps::CCallerIDProps
//
// Constructor for the call completion dialog
//
CCallerIDProps::CCallerIDProps() : CDialog(CCallerIDProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CCallerIDProps)
	m_fBlocked = FALSE;
	m_fOverride = FALSE;
	m_fAppSupplied = FALSE;
	//}}AFX_DATA_INIT

}// CCallerIDProps::CCallerIDProps

///////////////////////////////////////////////////////////////////////////
// CCallerIDProps::DoDataExchange
//
// Dialog data exchange for the caller id
//
void CCallerIDProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallerIDProps)
	DDX_Control(pDX, IDC_CALLERID, m_lbCallerFlags);
	DDX_Check(pDX, IDC_BLOCKED, m_fBlocked);
	DDX_Check(pDX, IDC_BLOCKED2, m_fOverride);
	DDX_Check(pDX, IDC_BLOCKED3, m_fAppSupplied);
	//}}AFX_DATA_MAP

}// CCallerIDProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CCallerIDProps::OnInitDialog
//
// Handle the initialization of the dialog
//
BOOL CCallerIDProps::OnInitDialog() 
{
	// Connect the dialog
	CDialog::OnInitDialog();

	for (int i = 0; g_Flags[i].pszFlag != NULL; i++)
	{
		int nPos = m_lbCallerFlags.AddString(g_Flags[i].pszFlag);
		m_lbCallerFlags.SetSel(nPos, TRUE);
		m_lbCallerFlags.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	return TRUE;

}// CCallerIDProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CCallerIDProps::OnOK
//
// Dismiss the dialog
//
void CCallerIDProps::OnOK() 
{
	UpdateData(TRUE);

	CString strFlags = "LINECALLPARTYID_UNKNOWN | LINECALLPARTYID_UNAVAIL";
	for (int i = 0; i < m_lbCallerFlags.GetCount(); i++)
	{
		if (m_lbCallerFlags.GetSel(i))
		{
			int nPos = static_cast<int>(m_lbCallerFlags.GetItemData(i));
			strFlags += " | ";
			strFlags += g_Flags[nPos].pszTapi;
		}
	}

	TSPWizardaw.m_Dictionary["CALLERID_FLAGS"] = strFlags;
	TSPWizardaw.m_Dictionary["CALLERID_BLOCKED"] = (m_fBlocked) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["CALLERID_OVERRIDE"] = (m_fOverride) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["CALLERID_APPSUPPLIED"] = (m_fAppSupplied) ? "Yes" : "No";

	CDialog::OnOK();

}// CCallerIDProps::OnOK
