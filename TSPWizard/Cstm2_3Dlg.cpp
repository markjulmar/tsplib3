/******************************************************************************/
//                                                                        
// CSTM2_3DLG.CPP - Device/Line/Phone parameters page for the TSP Wizard
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
#include "cstm2_3dlg.h"
#include "TSPWizardaw.h"
#include "chooser.h"

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
BEGIN_MESSAGE_MAP(CCustom2_3Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom2_3Dlg)
	ON_CLBN_CHKCHANGE(IDC_LBLIST, OnSelchangeFunctions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	bool fTapi3Only;
	bool fSelected;
	bool fDefault;
	LPCSTR pszDesc;
	LPCSTR pszMap;

} g_arrFunctions[] = {

	{ false, true, true, "Can perform call-control functions on the device", "OVERRIDE_LINE" },
#ifndef DEMO_VERSION
	{ false, false, false, "Can control display, button, lamp or hookswitch elements", "OVERRIDE_PHONE" },
	{ true, false, false, "Supports TAPI 3.x Media Service Provider", "HASTAPI3MSP" },
	{ true, true, true, "Supports TAPI 3.x Call Hubs", "HASTAPI3CALLHUBS" },
#endif
	{ false, true, true, "Supports Incoming Call traffic", "ALLOWINCOMINGCALLS" },
	{ false, true, true, "Supports Outgoing Call traffic", "ALLOWOUTGOINGCALLS" },
	{ false, false, false, "Dropping a consultation call reconnects to original", "DROPCALLRECONNECTS" },
	{ false, false, false, "Closing a line using TAPI will drop any active calls", "LINECLOSEDROP" },
	{ false, false, NULL, NULL }
};

static struct
{
	LPCTSTR pszMediaMode;
	LPCTSTR pszTapiFlag;
} g_MediaModes[] = {

	{ "Interactive Voice", "LINEMEDIAMODE_INTERACTIVEVOICE" },
#ifndef DEMO_VERSION
	{ "Automated Voice", "LINEMEDIAMODE_AUTOMATEDVOICE" },
	{ "Data Modem", "LINEMEDIAMODE_DATAMODEM" },
	{ "G3 Fax", "LINEMEDIAMODE_G3FAX" },
	{ "Telephony Device for the deaf", "LINEMEDIAMODE_TDD" },
	{ "G4 Fax", "LINEMEDIAMODE_G4FAX" },
	{ "Digital Data", "LINEMEDIAMODE_DIGITALDATA" },
	{ "Teletex", "LINEMEDIAMODE_TELETEX" },
	{ "Videotex", "LINEMEDIAMODE_VIDEOTEX" },
	{ "Telex", "LINEMEDIAMODE_TELEX" },
	{ "Mixed ISDN", "LINEMEDIAMODE_MIXED" },
	{ "Analog Display Services Interface (ASDI)", "LINEMEDIAMODE_ADSI" },
	{ "VoiceView", "LINEMEDIAMODE_VOICEVIEW" },
	{ "Video", "LINEMEDIAMODE_VIDEO" },
#endif
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg::CCustom2_3Dlg
//
// Constructor for the device/line page
//
CCustom2_3Dlg::CCustom2_3Dlg()
	: CAppWizStepDlg(CCustom2_3Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom2_3Dlg)
	m_fOverrideAddr = FALSE;
	m_fOverrideCall = FALSE;
	m_fOverrideConfCall = FALSE;
	//}}AFX_DATA_INIT

}// CCustom2_3Dlg::CCustom2_3Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg::DoDataExchange
//
// Dialog Data Exchange for the line page
//
void CCustom2_3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom2_3Dlg)
	DDX_Control(pDX, IDC_MEDIALIST, m_lbMediaList);
	DDX_Check(pDX, IDC_ADDRESS, m_fOverrideAddr);
	DDX_Check(pDX, IDC_CALL, m_fOverrideCall);
	DDX_Check(pDX, IDC_CONFCALL, m_fOverrideConfCall);
	//}}AFX_DATA_MAP

}// CCustom2_3Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom2_3Dlg::OnKillActive()
{
	if (!UpdateData(TRUE))
		return FALSE;

	// Walk through the list of selected entries
	for (int i = 0; g_arrFunctions[i].pszDesc != NULL; i++)
		TSPWizardaw.m_Dictionary[g_arrFunctions[i].pszMap] = (g_arrFunctions[i].fSelected) ? "Yes" : "No";

	if (g_arrFunctions[0].fSelected)
	{
		TSPWizardaw.m_Dictionary["OVERRIDE_ADDRESS"] = (m_fOverrideAddr) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["OVERRIDE_CALL"] = (m_fOverrideCall) ? "Yes" : "No";
		TSPWizardaw.m_Dictionary["OVERRIDE_CONFCALL"] = (m_fOverrideConfCall) ? "Yes" : "No";
	}
	else
	{
		TSPWizardaw.m_Dictionary["OVERRIDE_ADDRESS"] = "No";
		TSPWizardaw.m_Dictionary["OVERRIDE_CALL"] = "No";
		TSPWizardaw.m_Dictionary["OVERRIDE_CONFCALL"] = "No";
	}

#ifndef DEMO_VERSION
	if (!g_arrFunctions[1].fSelected && !g_arrFunctions[0].fSelected)
	{
		AfxMessageBox(IDS_NOLINEPHONE);
		GotoDlgCtrl(&m_lbOptions);
		return FALSE;
	}

	if (g_arrFunctions[0].fSelected &&
		!g_arrFunctions[2].fSelected && !g_arrFunctions[3].fSelected)
	{
		AfxMessageBox(IDS_NOIO);
		GotoDlgCtrl(&m_lbOptions);
		return FALSE;
	}

	// If there is no phone override then remove a step.
//	if (g_arrFunctions[1].fSelected && g_arrFunctions[0].fSelected)
//		SetNumberOfSteps(LAST_DLG);
//	else
//		SetNumberOfSteps(LAST_DLG-1);
#else
	if (!g_arrFunctions[0].fSelected)
	{
		AfxMessageBox(IDS_NOLINEPHONE);
		GotoDlgCtrl(&m_lbOptions);
		return FALSE;
	}

	if (g_arrFunctions[0].fSelected &&
		!g_arrFunctions[1].fSelected && !g_arrFunctions[2].fSelected)
	{
		AfxMessageBox(IDS_NOIO);
		GotoDlgCtrl(&m_lbOptions);
		return FALSE;
	}

	SetNumberOfSteps(LAST_DLG-1);
#endif

	// Create the media list
	CString strMediaMode = "LINEMEDIAMODE_UNKNOWN";
	for (i = 0; i < m_lbMediaList.GetCount(); i++)
	{
		if (m_lbMediaList.GetSel(i))
		{
			int nPos = static_cast<int>(m_lbMediaList.GetItemData(i));
			strMediaMode += " | ";
			strMediaMode += g_MediaModes[nPos].pszTapiFlag;
		}
	}
	TSPWizardaw.m_Dictionary["MEDIAMODES"] = strMediaMode;

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom2_3Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg::OnInitDialog
//
// This is called when the dialog is first created
//
BOOL CCustom2_3Dlg::OnInitDialog() 
{
	// Pass through to the normal setup
	CAppWizStepDlg::OnInitDialog();

	// Get the area for the listbox
	ASSERT (m_lbOptions.GetSafeHwnd() == NULL);

	CRect rcBox;
	CWnd* pwnd = GetDlgItem(IDC_LBLIST);
	pwnd->GetWindowRect(rcBox);
	ScreenToClient(rcBox);
	pwnd->SetDlgCtrlID(IDC_STATIC);

	// Create our listbox
	m_lbOptions.Create(
		LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_STANDARD | 
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP,
		rcBox, this, IDC_LBLIST);
	m_lbOptions.SetWindowPos(GetDlgItem(IDC_PICTURE), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// Set the font for the listbox
	CFont fntAnsi;
	fntAnsi.CreateStockObject(ANSI_VAR_FONT);
	m_lbOptions.SetFont(&fntAnsi);

	// Load our listbox
	for (int i = 0; g_arrFunctions[i].pszDesc != NULL; i++)
	{
		if (*g_arrFunctions[i].pszDesc != '\0')
		{
			int nPos = m_lbOptions.AddString(g_arrFunctions[i].pszDesc);
			m_lbOptions.SetItemData(nPos, static_cast<LPARAM>(i));
			if (g_arrFunctions[i].fDefault)
				m_lbOptions.SetCheck(nPos, TRUE);
		}
	}

	m_lbOptions.SetCurSel(0);

	for (i = 0; g_MediaModes[i].pszMediaMode != NULL; i++)
	{
		int nPos = m_lbMediaList.AddString(g_MediaModes[i].pszMediaMode);
		m_lbMediaList.SetItemData(nPos, static_cast<LPARAM>(i));
	}
	m_lbMediaList.SetSel(0);

	pwnd = GetParent()->GetDlgItem(FINISH_BUTTON);
	if (pwnd)
		pwnd->EnableWindow(FALSE);

	return TRUE;

}// CCustom2_3Dlg::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg::OnSelchangeFunctions
//
// The selection has changed - see if the item is checked and was not 
// checked before.
//
void CCustom2_3Dlg::OnSelchangeFunctions()
{
	int nCurSel = m_lbOptions.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	g_arrFunctions[static_cast<int>(m_lbOptions.GetItemData(nCurSel))].fSelected = (m_lbOptions.GetCheck(nCurSel) != 0);

	GetDlgItem(IDC_ADDRESS)->EnableWindow(g_arrFunctions[0].fSelected);
	GetDlgItem(IDC_CALL)->EnableWindow(g_arrFunctions[0].fSelected);
	GetDlgItem(IDC_CONFCALL)->EnableWindow(g_arrFunctions[0].fSelected);

}// CCustom2_3Dlg::OnSelchangeLineFuncs

///////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg::OnSetActive
//
// This is called to display the window
//
BOOL CCustom2_3Dlg::OnSetActive()
{
	TSPWizardaw.SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);

	CString strValue;
	bool fIsTapi3 = (TSPWizardaw.m_Dictionary.Lookup("ISTAPI3", strValue) && strValue == "Yes");

	// Enable/Disable the TAPI3 specific stuff
	for (int i = 0; i < m_lbOptions.GetCount(); i++)
	{
		int iPos = (int) m_lbOptions.GetItemData(i);
		if (g_arrFunctions[iPos].fTapi3Only)
			m_lbOptions.Enable(i, fIsTapi3);
	}

	return TRUE;
	
}// CCustom2_3Dlg::OnSetActive
