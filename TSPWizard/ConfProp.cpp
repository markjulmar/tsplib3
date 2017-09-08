/******************************************************************************/
//                                                                        
// CONFPROP.CPP - Conference properties for a line
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
#include "ConfProp.h"
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
IMPLEMENT_DYNCREATE(CConfProp, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CConfProp, CDialog)
	//{{AFX_MSG_MAP(CConfProp)
	ON_LBN_SELCHANGE(IDC_REMOVEWHEN, OnSelchangeRemovewhen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CConfProp::CConfProp
//
// Constructor for the conference property page
//
CConfProp::CConfProp() : CDialog(CConfProp::IDD, NULL)
{
	//{{AFX_DATA_INIT(CConfProp)
	m_fAddHeld = FALSE;
	m_fDrop = FALSE;
	m_strMaxInConference = _T("3");
	m_strMaxTransfer = _T("0");
	m_fCreateNoCall = FALSE;
	m_fCrossAddrConf = FALSE;
	//}}AFX_DATA_INIT
}// CConfProp::CConfProp

///////////////////////////////////////////////////////////////////////////
// CConfProp::DoDataExchange
//
// Dialog Data Exchange for the conference property page
//
void CConfProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfProp)
	DDX_Control(pDX, IDC_SPIN2, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_REMOVEWHEN, m_lbRemoveType);
	DDX_Control(pDX, IDC_REMOVESTATE, m_cbRemoveState);
	DDX_Check(pDX, IDC_ADDHELD, m_fAddHeld);
	DDX_Check(pDX, IDC_DROP, m_fDrop);
	DDX_Text(pDX, IDC_MAXCALLSINCONFERENCE, m_strMaxInConference);
	DDX_Text(pDX, IDC_MAXTRANSFINCONF, m_strMaxTransfer);
	DDX_Check(pDX, IDC_CREATEWOCALL, m_fCreateNoCall);
	DDX_Check(pDX, IDC_CROSSADDR, m_fCrossAddrConf);
	//}}AFX_DATA_MAP

}// CConfProp::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CConfProp::OnInitDialog
//
// Handle the initialization of the dialog
//
BOOL CConfProp::OnInitDialog() 
{
	static LPCTSTR g_pszRemovalTypes[] = { "None", "Last", "Any", NULL };
	static LPCTSTR g_pszCallStates[] = { "Idle", "Disconnected", "Connected", "OnHold", "OnHoldPendConf", "OnHoldPendTransfer", NULL };

	// Attach controls
	CDialog::OnInitDialog();

	// Fill in our removal types
	for (int i = 0; g_pszRemovalTypes[i] != NULL; i++)
	{
		int iPos = m_lbRemoveType.AddString(g_pszRemovalTypes[i]);

		if (i == 0)
			m_lbRemoveType.SetCurSel(0);


		ASSERT(iPos != LB_ERR);
		m_lbRemoveType.SetItemData(iPos, static_cast<LPARAM>(i));
	}

	// .. and our call states
	for (i = 0; g_pszCallStates[i] != NULL; i++)
	{
		int iPos = m_cbRemoveState.AddString(g_pszCallStates[i]);
		
		if (i == 0)
			m_cbRemoveState.SetCurSel(0);

		ASSERT(iPos != LB_ERR);
		m_cbRemoveState.SetItemData(iPos, static_cast<LPARAM>(i));
	}

	// Limit the spinbox.
	m_ctlSpin1.SetRange(3, 99);
	m_ctlSpin2.SetRange(0, 99);

	OnSelchangeRemovewhen();

	return TRUE;

}// CConfProp::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CConfProp::OnOK
//
// Dismiss the dialog and write our map information out
//
void CConfProp::OnOK() 
{
	static LPCTSTR g_pszRemovalTypes[] = { 
		"LINEREMOVEFROMCONF_NONE",
		"LINEREMOVEFROMCONF_LAST",
		"LINEREMOVEFROMCONF_ANY"
	};

	static LPCTSTR g_pszCallStates[] = { 
		"LINECALLSTATE_IDLE", 
		"LINECALLSTATE_DISCONNECTED", 
		"LINECALLSTATE_CONNECTED", 
		"LINECALLSTATE_ONHOLD", 
		"LINECALLSTATE_ONHOLDPENDCONF", 
		"LINECALLSTATE_ONHOLDPENDTRANSFER"
	};

	// Validate the data
	if (UpdateData(TRUE))
	{
		if (m_strMaxInConference.IsEmpty())
		{
			AfxMessageBox(IDS_CANNOTBEBLANK);
			GotoDlgCtrl(GetDlgItem(IDC_MAXCALLSINCONFERENCE));
			return;
		}

		if (m_strMaxTransfer.IsEmpty())
		{
			AfxMessageBox(IDS_CANNOTBEBLANK);
			GotoDlgCtrl(GetDlgItem(IDC_MAXTRANSFINCONF));
			return;
		}

		int nCurSel = m_lbRemoveType.GetCurSel();
		ASSERT(nCurSel != LB_ERR);
		nCurSel = static_cast<int>(m_lbRemoveType.GetItemData(nCurSel));
		TSPWizardaw.m_Dictionary["CONFREMOVE_TYPE"] = g_pszRemovalTypes[nCurSel];

		if (m_cbRemoveState.IsWindowEnabled())
		{
			TSPWizardaw.m_Dictionary["CONFREMOVE"] = "Yes";

			nCurSel = m_cbRemoveState.GetCurSel();
			nCurSel = static_cast<int>(m_cbRemoveState.GetItemData(nCurSel));
		}
		else
		{
			nCurSel = 0;
			TSPWizardaw.m_Dictionary["CONFREMOVE"] = "No";
		}

		TSPWizardaw.m_Dictionary["CONFREMOVE_STATE"] = g_pszCallStates[nCurSel];
		TSPWizardaw.m_Dictionary["CONFCROSSADDR"] = (m_fCrossAddrConf) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["CONFADDHELD"] = (m_fAddHeld) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["CONFADDHELD"] = (m_fAddHeld) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["CONFCANDROP"] = (m_fDrop) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["CONFCREATENOCALL"] = (m_fCreateNoCall) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["CONFMAXCOUNT"] = m_strMaxInConference;
		TSPWizardaw.m_Dictionary["CONFTRANSFMAXCOUNT"] = m_strMaxTransfer;
		TSPWizardaw.m_Dictionary["CONFADDEXISTING"] = (atoi(m_strMaxInConference) > 3) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["CONFADDTRANSFER"] = (atoi(m_strMaxTransfer) > 0) ? "Yes" : "No";

		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CConfProp::OnSelchangeRemovewhen
//
// Change the state of the remove conference capablity
//
void CConfProp::OnSelchangeRemovewhen() 
{
	if (m_lbRemoveType.GetItemData(m_lbRemoveType.GetCurSel()) == 0)
		m_cbRemoveState.EnableWindow(FALSE);
	else
		m_cbRemoveState.EnableWindow(TRUE);
}
