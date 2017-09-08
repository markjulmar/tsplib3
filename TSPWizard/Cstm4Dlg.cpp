/******************************************************************************/
//                                                                        
// CSTM4DLG.CPP - Phone parameters page for the TSP Wizard
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
#include "cstm4dlg.h"
#include "TSPWizardaw.h"
#include "UploadBuffProps.h"
#include "DownloadBuffProps.h"
#include "DisplayProps.h"
#include "ButtonLampProps.h"
#include "HSwitchProps.h"
#include "RingerProps.h"

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
BEGIN_MESSAGE_MAP(CCustom4Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom4Dlg)
	ON_CLBN_CHKCHANGE(IDC_PHONEFUNCS, OnSelchangePhoneFuncs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
struct
{
	bool fSelected;
	LPCSTR pszDesc;
	CRuntimeClass* ptrClass;

} g_arrPhoneFuncs[] = {

	{ false, "Supports device specific features", NULL }, // 0
	{ false, "Has one or more upload buffers", RUNTIME_CLASS(CUploadBuffProps) }, // 1
	{ false, "Has one or more download buffers", RUNTIME_CLASS(CDownloadBuffProps) }, // 2
	{ false, "Has a display device", RUNTIME_CLASS(CDisplayProps) }, // 3
	{ false, "Has buttons and/or lamps", RUNTIME_CLASS(CButtonLampProps) }, // 4
	{ false, "Supports a handset, headset, or speaker", RUNTIME_CLASS(CHSwitchProps) }, // 5
	{ false, "Supports multiple ringer styles", RUNTIME_CLASS(CRingerProps) }, // 6
	{ false, "Supports dynamic creation of phone devices", NULL }, // 7
	{ false, NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCustom4Dlg::CCustom4Dlg
//
// Constructor for the Phone page
//
CCustom4Dlg::CCustom4Dlg()
	: CAppWizStepDlg(CCustom4Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom4Dlg)
	//}}AFX_DATA_INIT

}// CCustom4Dlg::CCustom4Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom4Dlg::DoDataExchange
//
// Dialog Data Exchange for the Phone page
//
void CCustom4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom4Dlg)
	//}}AFX_DATA_MAP

}// CCustom4Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom4Dlg::OnSetActive
//
// This is called when the user presses "Prev" to move back to this pane.
//
BOOL CCustom4Dlg::OnSetActive()
{
	TSPWizardaw.SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	return TRUE;

}// CCustom4Dlg::OnSetActive

///////////////////////////////////////////////////////////////////////////
// CCustom4Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom4Dlg::OnKillActive()
{
	if (!UpdateData(TRUE))
		return FALSE;

	// Validate the parameters

	// Walk through the list of phone functions and add each entry to our list.
	for (int i = 0; g_arrPhoneFuncs[i].pszDesc != NULL; i++)
	{
		char szBuff[20];
		wsprintf(szBuff, "PHONEFEATURE_%d", i);
		TSPWizardaw.m_Dictionary[szBuff] = (g_arrPhoneFuncs[i].fSelected) ? "Yes" : "No";
	}

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom4Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom4Dlg::OnInitDialog
//
// This is called when the dialog is first created
//
BOOL CCustom4Dlg::OnInitDialog() 
{
	// Pass through to the normal setup
	CAppWizStepDlg::OnInitDialog();

	// Get the area for the listbox
	CRect rcBox;
	CWnd* pwnd = GetDlgItem(IDC_PHONEFUNCS);
	pwnd->GetWindowRect(rcBox);
	ScreenToClient(rcBox);
	pwnd->SetDlgCtrlID(IDC_STATIC);

	// Create our listbox
	m_lbPhoneFuncs.Create(
		LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_STANDARD | 
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP,
		rcBox, this, IDC_PHONEFUNCS);

	// Set the font for the listbox
	CFont fntAnsi;
	fntAnsi.CreateStockObject(ANSI_VAR_FONT);
	m_lbPhoneFuncs.SetFont(&fntAnsi);

	// Load our listbox
	for (int i = 0; g_arrPhoneFuncs[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbPhoneFuncs.AddString(g_arrPhoneFuncs[i].pszDesc);
		m_lbPhoneFuncs.SetItemData(nPos, static_cast<LPARAM>(i));
	}
	m_lbPhoneFuncs.SetCurSel(0);

	pwnd = GetParent()->GetDlgItem(FINISH_BUTTON);
	if (pwnd)
		pwnd->EnableWindow(FALSE);

	return TRUE;

}// CCustom4Dlg::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CCustom4Dlg::OnSelchangePhoneFuncs
//
// The selection has changed - see if the item is checked and was not 
// checked before.
//
void CCustom4Dlg::OnSelchangePhoneFuncs()
{
	int nCurSel = m_lbPhoneFuncs.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	int nPos = static_cast<int>(m_lbPhoneFuncs.GetItemData(nCurSel));
	bool fSelected = (m_lbPhoneFuncs.GetCheck(nCurSel) != 0);
	if (fSelected != g_arrPhoneFuncs[nPos].fSelected)
	{
		g_arrPhoneFuncs[nPos].fSelected = fSelected;
		if (fSelected && g_arrPhoneFuncs[nPos].ptrClass != NULL)
		{
			CDialog* pDialog = reinterpret_cast<CDialog*>(g_arrPhoneFuncs[nPos].ptrClass->CreateObject());
			if (pDialog->DoModal() == IDCANCEL)
			{
				g_arrPhoneFuncs[nPos].fSelected = false;
				m_lbPhoneFuncs.SetCheck(nCurSel, 0);
			}
			delete pDialog;
		}
	}

}// CCustom4Dlg::OnSelchangePhoneFuncs
