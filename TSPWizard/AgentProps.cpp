/******************************************************************************/
//                                                                        
// AGENTPROPS.CPP - Agent properties for a line
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
#include "AgentProps.h"
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
IMPLEMENT_DYNCREATE(CAgentProps,CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAgentProps, CDialog)
	//{{AFX_MSG_MAP(CAgentProps)
	ON_BN_CLICKED(IDC_GROUPS, OnGroups)
	ON_BN_CLICKED(IDC_REQUIRESID, OnRequiresid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCTSTR pszState;
	LPCTSTR pszCode;
} g_States[] = {

	{ "Logged Off", "LINEAGENTSTATE_LOGGEDOFF" },
	{ "Not Ready",  "LINEAGENTSTATE_NOTREADY" },
	{ "Ready", "LINEAGENTSTATE_READY" },
	{ "Busy Handling ACD call", "LINEAGENTSTATE_BUSYACD" },
	{ "Busy Handling Incoming call", "LINEAGENTSTATE_BUSYINCOMING" },
	{ "Busy Handling Outgoing call", "LINEAGENTSTATE_BUSYOUTBOUND" },
	{ "Busy", "LINEAGENTSTATE_BUSYOTHER" },
	{ "In Call Work", "LINEAGENTSTATE_WORKINGAFTERCALL" },
	{ NULL, NULL }
};

/////////////////////////////////////////////////////////////////////////////
// CAgentProps::CAgentProps
//
// Constructor for the agent properties dialog
//
CAgentProps::CAgentProps() : CDialog(CAgentProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CAgentProps)
	m_fActivities = FALSE;
	m_fGroups = FALSE;
	m_fStates = FALSE;
	m_strMaxAgentGroups = _T("3");
	m_fRequiresID = FALSE;
	//}}AFX_DATA_INIT

}// CAgentProps::CAgentProps

///////////////////////////////////////////////////////////////////////////
// CAgentProps::DoDataExchange
//
// Dialog Data Exchange for the agent properties
//
void CAgentProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentProps)
	DDX_Control(pDX, IDC_AGENTGROUPS, m_edtAgentGroups);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_LISTSTATES, m_lbStates);
	DDX_Check(pDX, IDC_ACTIVITIES, m_fActivities);
	DDX_Check(pDX, IDC_GROUPS, m_fGroups);
	DDX_Check(pDX, IDC_STATES, m_fStates);
	DDX_Text(pDX, IDC_AGENTGROUPS, m_strMaxAgentGroups);
	DDX_Check(pDX, IDC_REQUIRESID, m_fRequiresID);
	//}}AFX_DATA_MAP

}// CAgentProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CAgentProps::OnInitDialog
//
// Handle the initialization of the dialog
//
BOOL CAgentProps::OnInitDialog() 
{
	// Connect the data elements
	CDialog::OnInitDialog();

	// Fill the dialog
	for (int i = 0; g_States[i].pszState != NULL; i++)
	{
		int iPos = m_lbStates.AddString(g_States[i].pszState);
		ASSERT (iPos != LB_ERR);
		m_lbStates.SetItemData(iPos, static_cast<LPARAM>(i));
	}

	m_ctlSpin1.SetRange(1, 99);

	return TRUE;

}// CAgentProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CAgentProps::OnOK
//
// Handle the destruction of the dialog
//
void CAgentProps::OnOK() 
{
	if (UpdateData(TRUE))
	{
		if (atoi(m_strMaxAgentGroups) == 0 && m_fGroups)
		{
			AfxMessageBox(IDS_MUSTENTERGROUPS);
			GotoDlgCtrl(GetDlgItem(IDC_AGENTGROUPS));
			return;
		}

		TSPWizardaw.m_Dictionary["AGENT_SUPPORTSACTIVITIES"] = (m_fActivities) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["AGENT_SUPPORTSGROUPS"] = (m_fGroups) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["AGENT_SUPPORTSSTATES"] = (m_fStates) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["AGENT_REQUIRESID"] = (m_fRequiresID) ? "Yes" : "No";

		if (m_fGroups)
			TSPWizardaw.m_Dictionary["AGENT_MAXGROUPS"] = m_strMaxAgentGroups;
		else
			TSPWizardaw.m_Dictionary["AGENT_MAXGROUPS"] = "0";

		CString strStates = "LINEAGENTSTATE_UNKNOWN | LINEAGENTSTATE_UNAVAIL";
		for (int i = 0; i < m_lbStates.GetCount(); i++)
		{
			if (m_lbStates.GetSel(i) != 0)
			{
				int iPos = static_cast<int>(m_lbStates.GetItemData(i));
				strStates += " | ";
				strStates += g_States[iPos].pszCode;
			}
		}
		TSPWizardaw.m_Dictionary["AGENT_STATES"] = strStates;

		CString strFeatures = "";
		if (m_fGroups)
		{
			if (!strFeatures.IsEmpty())
				strFeatures += " | ";
			strFeatures += "LINEAGENTFEATURE_SETAGENTGROUP | LINEAGENTFEATURE_GETAGENTGROUP";
		}
		if (m_fStates)
		{
			if (!strFeatures.IsEmpty())
				strFeatures += " | ";
			strFeatures += "LINEAGENTFEATURE_SETAGENTSTATE";
		}
		if (m_fGroups)
		{
			if (!strFeatures.IsEmpty())
				strFeatures += " | ";
			strFeatures += "LINEAGENTFEATURE_SETAGENTGROUP";
		}
		if (m_fActivities)
		{
			if (!strFeatures.IsEmpty())
				strFeatures += " | ";
			strFeatures += "LINEAGENTFEATURE_SETAGENTACTIVITY | LINEAGENTFEATURE_GETAGENTACTIVITYLIST";
		}

		if (strFeatures.IsEmpty())
			strFeatures = "0";
		else
			strFeatures = "(" + strFeatures + ")";
		TSPWizardaw.m_Dictionary["AGENT_FEATURES"] = strFeatures;

		CDialog::OnOK();
	}

}// CAgentProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CAgentProps::OnGroups
//
// The group box has changed
//
void CAgentProps::OnGroups() 
{
	UpdateData(TRUE);
	m_edtAgentGroups.EnableWindow(m_fGroups);

}// CAgentProps::OnGroups

/////////////////////////////////////////////////////////////////////////////
// CAgentProps::OnRequiresid
//
// The "requires id" has changed
//
void CAgentProps::OnRequiresid() 
{
	UpdateData(TRUE);
	m_ctlSpin1.SetRange(2, 99);
	if (atoi(m_strMaxAgentGroups) < 2)
		m_strMaxAgentGroups = "2";
	UpdateData(FALSE);

}// CAgentProps::OnRequiresid
