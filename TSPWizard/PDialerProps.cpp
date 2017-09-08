/******************************************************************************/
//                                                                        
// PDIALERPROPS.CPP - Predictive dialer properties
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
#include "PDialerProps.h"
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
IMPLEMENT_DYNCREATE(CPDialerProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CPDialerProps, CDialog)
	//{{AFX_MSG_MAP(CPDialerProps)
	ON_BN_CLICKED(IDC_TIMEOUT, OnTimeout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_arrStates[] = {

	{ "Idle", "LINECALLSTATE_IDLE" },
	{ "Offering", "LINECALLSTATE_OFFERING" },
	{ "Accepted", "LINECALLSTATE_ACCEPTED" },
	{ "Dialtone", "LINECALLSTATE_DIALTONE" },
	{ "Dialing", "LINECALLSTATE_DIALING" },
	{ "Ringback", "LINECALLSTATE_RINGBACK" },
	{ "Busy", "LINECALLSTATE_BUSY" }, 
	{ "Connected", "LINECALLSTATE_CONNECTED" },
	{ "Waiting for Ringback", "LINECALLSTATE_PROCEEDING" }, 
	{ "OnHold", "LINECALLSTATE_ONHOLD" },
	{ "OnHold pending conference", "LINECALLSTATE_ONHOLDPENDCONF" },
	{ "OnHold pending transfer", "LINECALLSTATE_ONHOLDPENDTRANSFER" },
	{ "Disconnected", "LINECALLSTATE_DISCONNECTED" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CPDialerProps::CPDialerProps
//
// Constructor for predictive dialer property page
//
CPDialerProps::CPDialerProps() : CDialog(CPDialerProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CPDialerProps)
	m_strMaxCalls = _T("500");
	m_strMaxTimeout = _T("120");
	m_fTimeout = FALSE;
	//}}AFX_DATA_INIT

}// CPDialerProps::CPDialerProps

///////////////////////////////////////////////////////////////////////////
// CPDialerProps::DoDataExchange
//
// Dialog Data Exchange for the predictive dialer page
//
void CPDialerProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPDialerProps)
	DDX_Control(pDX, IDC_MAXTIMEOUT, m_edtMaxTimeout);
	DDX_Control(pDX, IDC_STATES, m_lbStates);
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Text(pDX, IDC_MAXCALLS, m_strMaxCalls);
	DDX_Text(pDX, IDC_MAXTIMEOUT, m_strMaxTimeout);
	DDX_Check(pDX, IDC_TIMEOUT, m_fTimeout);
	//}}AFX_DATA_MAP

}// CPDialerProps::DoDataExchange

////////////////////////////////////////////////////////////////////////////
// CPDialerProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CPDialerProps::OnInitDialog() 
{
	// Connect the data elements
	CDialog::OnInitDialog();
	
	// Load our listbox
	for (int i = 0; g_arrStates[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbStates.AddString(g_arrStates[i].pszDesc);
		m_lbStates.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	m_ctlSpin1.SetRange(1, 1000);
	m_ctlSpin2.SetRange(1, 1000);

	return TRUE;

}// CPDialerProps::OnInitDialog

////////////////////////////////////////////////////////////////////////////
// CPDialerProps::OnOK
//
// Dismiss the dialog
//
void CPDialerProps::OnOK() 
{
	UpdateData(TRUE);
	
	if (m_strMaxCalls.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXCALLS));
		return;
	}

	TSPWizardaw.m_Dictionary["PDIAL_MAXCALLS"] = m_strMaxCalls;

	if (m_fTimeout)
	{
		if (m_strMaxTimeout.IsEmpty())
		{
			AfxMessageBox(IDS_CANNOTBEBLANK);
			GotoDlgCtrl(GetDlgItem(IDC_MAXTIMEOUT));
			return;
		}
		TSPWizardaw.m_Dictionary["PDIAL_MAXTIMEOUT"] = m_strMaxTimeout;
	}
	else
		TSPWizardaw.m_Dictionary["PDIAL_MAXTIMEOUT"] = "0";

	CString strStates;
	for (int i = 0; i < m_lbStates.GetCount(); i++)
	{
		if (m_lbStates.GetSel(i))
		{
			int nPos = static_cast<int>(m_lbStates.GetItemData(i));
			if (!strStates.IsEmpty())
				strStates += " | ";
			strStates += g_arrStates[nPos].pszTapi;
		}
	}
	if (strStates.IsEmpty())
		strStates = "0";

	TSPWizardaw.m_Dictionary["PDIAL_STATES"] = strStates;

	CDialog::OnOK();

}// CPDialerProps::OnOK

////////////////////////////////////////////////////////////////////////////
// CPDialerProps::OnTimeout
//
// Enable/Disable the timeout value
//
void CPDialerProps::OnTimeout() 
{
	UpdateData(TRUE);
	m_edtMaxTimeout.EnableWindow(m_fTimeout);

}// CPDialerProps::OnTimeout
