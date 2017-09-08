/******************************************************************************/
//                                                                        
// CSTM2DLG.CPP - Device parameters page for the TSP Wizard
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
#include "cstm2dlg.h"
#include "TSPWizardaw.h"

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
BEGIN_MESSAGE_MAP(CCustom2Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom2Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CCustom2Dlg::CCustom2Dlg
//
// Constructor for the device page
//
CCustom2Dlg::CCustom2Dlg() : CAppWizStepDlg(CCustom2Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom2Dlg)
	m_strEventObjectName = _T("");
	m_strProviderInfo = _T("");
	m_strSwitchInfo = _T("");
	m_iCommParams = 2;
	m_fUsePoolMgr = FALSE;
	//}}AFX_DATA_INIT
}// CCustom2Dlg::CCustom2Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom2Dlg::DoDataExchange
//
// Dialog Data Exchange for the device page
//
void CCustom2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom2Dlg)
	DDX_Text(pDX, IDC_EVENTOBJNAME, m_strEventObjectName);
	DDX_Text(pDX, IDC_PROVINFO, m_strProviderInfo);
	DDX_Text(pDX, IDC_SWITCHINFO, m_strSwitchInfo);
	DDX_Radio(pDX, IDC_USESWINSOCK, m_iCommParams);
	DDX_Check(pDX, IDC_USEPOOLMGR, m_fUsePoolMgr);
	//}}AFX_DATA_MAP

}// CCustom2Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom2Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom2Dlg::OnKillActive()
{
	if (!UpdateData(TRUE))
		return FALSE;

	// Validate the parameters
	if (m_strProviderInfo.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_PROVINFO));
		return FALSE;
	}

	if (m_strSwitchInfo.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_SWITCHINFO));
		return FALSE;
	}

	TSPWizardaw.m_Dictionary["PROVIDER_INFO"] = m_strProviderInfo;
	TSPWizardaw.m_Dictionary["SWITCH_INFO"] = m_strSwitchInfo;

	if (m_strEventObjectName.IsEmpty() == false)
		TSPWizardaw.m_Dictionary["EVENT_OBJECT_NAME"] = m_strEventObjectName;
	else
		TSPWizardaw.m_Dictionary.RemoveKey("EVENT_OBJECT_NAME");

	TSPWizardaw.m_Dictionary["USE_WINSOCK"] = (m_iCommParams == 0) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["USE_COMM"] = (m_iCommParams == 1) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["USE_POOLMGR"] = (m_fUsePoolMgr) ? "Yes" : "No";

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom2Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom2Dlg::OnInitDialog
//
// Initialize the dialog
//
BOOL CCustom2Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();
	
#ifdef DEMO_VERSION
	GetDlgItem(IDC_USESWINSOCK)->EnableWindow(FALSE);
	GetDlgItem(IDC_USECOMM)->EnableWindow(FALSE);
	GetDlgItem(IDC_USEOTHER)->EnableWindow(FALSE);
	GetDlgItem(IDC_USEPOOLMGR)->EnableWindow(FALSE);
#endif

	CWnd* pwnd = GetParent()->GetDlgItem(FINISH_BUTTON);
	if (pwnd)
		pwnd->EnableWindow(FALSE);
	
	return TRUE;

}// CCustom2Dlg::OnInitDialog
