/******************************************************************************/
//                                                                        
// CSTM3DLG.CPP - Line parameters page for the TSP Wizard
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
#include "TSPWizard.h"
#include "cstm3dlg.h"
#include "TSPWizardaw.h"
#include "confprop.h"
#include "agentprops.h"
#include "uuiprops.h"
#include "CallCompleteProps.h"
#include "CallerIDProps.h"
#include "AcceptProps.h"
#include "AnswerProps.h"
#include "TransferProps.h"
#include "HoldProps.h"
#include "ForwardProps.h"
#include "ParkProps.h"
#include "PickupProps.h"
#include "SecureProps.h"
#include "CallTreatmentProps.h"
#include "PDialerProps.h"
#include "GenerateDProps.h"
#include "GenerateTProps.h"
#include "MonitorTProps.h"
#include "MonitorDProps.h"
#include "BlindXferProps.h"
#include "MediaCtlProps.h"
#include "TerminalProps.h"
#include "DialProps.h"
#include "DialtoneProps.h"
#include "DialCharProps.h"

/*---------------------------------------------------------------------------*/
// DEBUG STATEMENTS
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCustom3Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom3Dlg)
	ON_CLBN_CHKCHANGE(IDC_LINEFUNCS, OnSelchangeLineFuncs)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	bool fSelected;
	bool fIncoming;
	bool fOutgoing;
	LPCSTR pszDesc;
	CRuntimeClass* ptrClass;

} g_arrLineFuncs[] = {

	{ false, true, false, "Stations can accept incoming calls before answer", RUNTIME_CLASS(CAcceptProps) },	// 0
	{ false, false, false, "Supports conferences", RUNTIME_CLASS(CConfProp) },			// 1
	{ false, true, false, "Can answer incoming calls", RUNTIME_CLASS(CAnswerProps) },	// 2
	{ false, false, false, "Supports blind transfers", RUNTIME_CLASS(CBlindXferProps) },	// 3
	{ false, false, false, "Supports consultation transfers", RUNTIME_CLASS(CTransferProps) }, // 4
	{ false, false, false, "Can complete unconnected calls automatically", RUNTIME_CLASS(CCallCompleteProps) }, // 5
#ifndef DEMO_VERSION
	{ false, true, false, "Supports incoming call route points", NULL },					// 6
	{ false, false, false, "Supports agents on stations", RUNTIME_CLASS(CAgentProps) },	// 7
	{ false, false, false, "Supports device specific features", NULL },					// 8
	{ false, false, false, "Can forward line/address to other destinations", RUNTIME_CLASS(CForwardProps) }, // 9
	{ false, false, false, "Can detect digits entered by far-end party", RUNTIME_CLASS(CMonitorDProps) },	// 10
	{ false, false, false, "Can detect DTMF tones entered by far-end party", RUNTIME_CLASS(CMonitorTProps) },	// 11
	{ false, false, false, "Can generate pulse/DTMF digits on connected calls", RUNTIME_CLASS(CGenerateDProps) }, // 12
	{ false, false, false, "Can generate tones on connected calls", RUNTIME_CLASS(CGenerateTProps) }, // 13
	{ false, false, false, "Supports Hold/Unhold ability", RUNTIME_CLASS(CHoldProps) },	// 14
	{ false, false, true, "Supports changing the dialing timeouts/delays", RUNTIME_CLASS(CDialProps) }, // 15
	{ false, false, true, "Calling party's phone goes offhook automatically", NULL },	// 16
	{ false, true, false, "Called party's phone can be forced off-hook automatically", NULL },	// 17
	{ false, false, false, "Supports park of calls to other extensions", RUNTIME_CLASS(CParkProps) },	// 18
	{ false, false, false, "Supports pickup of calls from other extensions", RUNTIME_CLASS(CPickupProps) },		// 19
	{ false, false, false, "Supports securing the media stream from interruption", RUNTIME_CLASS(CSecureProps) },	// 20
	{ false, false, false, "Supports user-to-user (UUI) information", RUNTIME_CLASS(CUUIProps) },	// 21
	{ false, false, false, "Supports manipulation/control of the media stream", RUNTIME_CLASS(CMediaCtlProps) },	// 22
	{ false, false, false, "Supports terminals (headset/handset/speaker)", RUNTIME_CLASS(CTerminalProps) },		// 23
	{ false, false, false, "Supports dynamic creation of line devices", NULL },			// 24
	{ false, false, false, "Want to support query/set of device configuration", NULL },	// 25
	{ false, false, false, "Supports Quality of Service", NULL },							// 26
	{ false, true, false, "Supports call treatments", RUNTIME_CLASS(CCallTreatmentProps) }, // 27
	{ false, false, true, "Supports partial dialing of numbers", NULL },					// 28
	{ false, false, true, "Supports multiple dialtones (internal/external)", RUNTIME_CLASS(CDialToneProps) },	// 29
	{ true, true, false, "Supports caller-id", RUNTIME_CLASS(CCallerIDProps) },			// 30
	{ false, true, false, "Device has queues for incoming calls", NULL },				// 31
	{ false, false, true, "Supports predictive dialing", RUNTIME_CLASS(CPDialerProps) }, // 32
	{ false, true, false, "Supports voice-response units (IVRs)", NULL },				// 33
	{ false, false, false, "Supports inverse multiplexing", NULL },						// 34
	{ false, true, false, "Supports redirecting of incoming calls w/o answer", NULL },	// 35
	{ false, false, true, "Supports special dialing characters", RUNTIME_CLASS(CDialCharProps) }, // 36
	{ false, false, false, "Device has incoming/outgoing trunk lines", NULL },			// 37
#endif
	{ false, false, false, NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCustom3Dlg::CCustom3Dlg
//
// Constructor for the line page
//
CCustom3Dlg::CCustom3Dlg()
	: CAppWizStepDlg(CCustom3Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom3Dlg)
	m_strAddressCount = _T("1");
	m_strMaxNumActiveCalls = _T("1");
	//}}AFX_DATA_INIT

}// CCustom3Dlg::CCustom3Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom3Dlg::DoDataExchange
//
// Dialog Data Exchange for the line page
//
void CCustom3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom3Dlg)
	DDX_Control(pDX, IDC_SPIN2, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Text(pDX, IDC_ADDRCOUNT, m_strAddressCount);
	DDV_MaxChars(pDX, m_strAddressCount, 3);
	DDX_Text(pDX, IDC_ADDRCOUNT2, m_strMaxNumActiveCalls);
	//}}AFX_DATA_MAP

}// CCustom3Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom3Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom3Dlg::OnKillActive()
{
	if (!UpdateData(TRUE))
		return FALSE;

	if (m_strAddressCount.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_ADDRCOUNT));
		return FALSE;
	}

	if (m_strMaxNumActiveCalls.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_ADDRCOUNT2));
		return FALSE;
	}

	// Add the dictionary elements based on what the user selected.
	TSPWizardaw.m_Dictionary["LINEADDRESS_COUNT"] = m_strAddressCount;
	TSPWizardaw.m_Dictionary["NUMACTIVECALLS"] = m_strMaxNumActiveCalls;

	// Walk through the list of line functions and add each entry to our list.
	for (int i = 0; g_arrLineFuncs[i].pszDesc != NULL; i++)
	{
		char szBuff[20];
		wsprintf(szBuff, "LINEFEATURE_%d", i);
		TSPWizardaw.m_Dictionary[szBuff] = (g_arrLineFuncs[i].fSelected) ? "Yes" : "No";
	}

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom3Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom3Dlg::OnInitDialog
//
// This is called when the dialog is first created
//
BOOL CCustom3Dlg::OnInitDialog() 
{
	// Pass through to the normal setup
	CAppWizStepDlg::OnInitDialog();

	// Get the area for the listbox
	ASSERT (m_lbLineFuncs.GetSafeHwnd() == NULL);

	CRect rcBox;
	CWnd* pwnd = GetDlgItem(IDC_LINEFUNCS);
	pwnd->GetWindowRect(rcBox);
	ScreenToClient(rcBox);
	pwnd->SetDlgCtrlID(IDC_STATIC);

	// Create our listbox
	m_lbLineFuncs.Create(
		LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_STANDARD | 
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP,
		rcBox, this, IDC_LINEFUNCS);
	m_lbLineFuncs.SetWindowPos(GetDlgItem(IDC_PICTURE), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// Set the font for the listbox
	CFont fntAnsi;
	fntAnsi.CreateStockObject(ANSI_VAR_FONT);
	m_lbLineFuncs.SetFont(&fntAnsi);

	// Load our listbox
	for (int i = 0; g_arrLineFuncs[i].pszDesc != NULL; i++)
	{
		if (*g_arrLineFuncs[i].pszDesc != '\0')
		{
			int nPos = m_lbLineFuncs.AddString(g_arrLineFuncs[i].pszDesc);
			m_lbLineFuncs.SetItemData(nPos, static_cast<LPARAM>(i));
			if (g_arrLineFuncs[i].fSelected)
				m_lbLineFuncs.SetCheck(nPos, TRUE);
		}
	}
	m_lbLineFuncs.SetCurSel(0);

	// Setup the spin buttons
	m_ctlSpin1.SetRange(0, 999);
	m_ctlSpin2.SetRange(0, 999);

	pwnd = GetParent()->GetDlgItem(FINISH_BUTTON);
	if (pwnd)
		pwnd->EnableWindow(FALSE);

	return TRUE;

}// CCustom3Dlg::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CCustom3Dlg::OnSelchangeLineFuncs
//
// The selection has changed - see if the item is checked and was not 
// checked before.
//
void CCustom3Dlg::OnSelchangeLineFuncs()
{
	int nCurSel = m_lbLineFuncs.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	int nPos = static_cast<int>(m_lbLineFuncs.GetItemData(nCurSel));
	bool fSelected = (m_lbLineFuncs.GetCheck(nCurSel) != 0);
	if (fSelected != g_arrLineFuncs[nPos].fSelected)
	{
		g_arrLineFuncs[nPos].fSelected = fSelected;
		if (fSelected && g_arrLineFuncs[nPos].ptrClass != NULL)
		{
			CDialog* pDialog = reinterpret_cast<CDialog*>(g_arrLineFuncs[nPos].ptrClass->CreateObject());
			if (pDialog->DoModal() == IDCANCEL)
			{
				g_arrLineFuncs[nPos].fSelected = false;
				m_lbLineFuncs.SetCheck(nCurSel, 0);
			}
			delete pDialog;
		}
	}

}// CCustom3Dlg::OnSelchangeLineFuncs

///////////////////////////////////////////////////////////////////////////
// CCustom3Dlg::OnSetActive
//
// This is called to display the window
//
BOOL CCustom3Dlg::OnSetActive()
{
	TSPWizardaw.SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);

	CString strValue;
	bool fHasIncoming = (TSPWizardaw.m_Dictionary.Lookup("ALLOWINCOMINGCALLS", strValue) && strValue == "Yes");
	bool fHasOutgoing = (TSPWizardaw.m_Dictionary.Lookup("ALLOWOUTGOINGCALLS", strValue) && strValue == "Yes");

	// Reset our listbox
	for (int i = 0; i < m_lbLineFuncs.GetCount(); i++)
	{
		int nPos = static_cast<int>(m_lbLineFuncs.GetItemData(i));
		ASSERT (*g_arrLineFuncs[nPos].pszDesc);
		if ((g_arrLineFuncs[nPos].fIncoming && !fHasIncoming) ||
			(g_arrLineFuncs[nPos].fOutgoing && !fHasOutgoing))
		{
			m_lbLineFuncs.SetSel(i, FALSE);
			m_lbLineFuncs.Enable(i, FALSE);
		}
		else if ((g_arrLineFuncs[nPos].fIncoming && fHasIncoming) ||
			(g_arrLineFuncs[nPos].fOutgoing && fHasOutgoing))
		{
			m_lbLineFuncs.Enable(i, TRUE);
		}
	}

	return TRUE;

}// CCustom3Dlg::OnSetActive
