/******************************************************************************/
//                                                                        
// CSTM1DLG.CPP - Introduction page for the TSP Wizard
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
#include <atlcomcli.h>
#include "TSPWizard.h"
#include "cstm1dlg.h"
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
BEGIN_MESSAGE_MAP(CCustom1Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom1Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR strKey;
	LPCSTR strClassName;

} g_Classes[] = {

	{ "SP_CLASS_NAME", "C@ServiceProvider"  },
	{ "DEVICE_CLASS_NAME", "C@Device" },
	{ "LINE_CLASS_NAME", "C@Line" },
	{ "ADDRESS_CLASS_NAME", "C@Address" },
	{ "CALL_CLASS_NAME", "C@Call" },
	{ "CONFCALL_CLASS_NAME", "C@ConfCall" },
	{ "PHONE_CLASS_NAME", "C@Phone" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCustom1Dlg::CCustom1Dlg
//
// Constructor for the introduction page
//
CCustom1Dlg::CCustom1Dlg() : CAppWizStepDlg(CCustom1Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom1Dlg)
	m_strCompanyName = _T("");
	m_iHasComments = 0;
	m_fGenerateUI = FALSE;
	m_iTapiVersion = 0;
	m_fTraceEvents = FALSE;
	m_fWinnt = FALSE;
	//}}AFX_DATA_INIT
}// CCustom1Dlg::CCustom1Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom1Dlg::DoDataExchange
//
// Dialog Data Exchange for the introduction page
//
void CCustom1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom1Dlg)
	DDX_Control(pDX, IDC_PICTURE, m_ctlPicture);
	DDX_Control(pDX, IDC_TAPIVER, m_cbTapiVersion);
	DDX_Text(pDX, IDC_COMPANYNAME, m_strCompanyName);
	DDX_Radio(pDX, IDC_HASCOMMENTS, m_iHasComments);
	DDX_Check(pDX, IDC_HASUI, m_fGenerateUI);
	DDX_CBIndex(pDX, IDC_TAPIVER, m_iTapiVersion);
	DDX_Check(pDX, IDC_WANTTRACER, m_fTraceEvents);
	DDX_Check(pDX, IDC_WINNT, m_fWinnt);
	//}}AFX_DATA_MAP

}// CCustom1Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom1Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom1Dlg::OnKillActive()
{
	static LPCSTR pszTapiVer[] = { "TAPIVER_20", "TAPIVER_21", "TAPIVER_30" };
	static LPCSTR pszTapiVer2[] = { "2.0", "2.1", "3.0" };

	// Validate the data on the dialog
	if (!UpdateData(TRUE))
		return FALSE;

	if (m_strCompanyName.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_COMPANYNAME));
		return FALSE;
	}

#if 0
	// If the user is using TAPI 3.0, make sure they realize it won't
	// work under any other platform other than NT5
	if (m_iTapiVersion == 2)
	{
		if (AfxMessageBox(IDS_MUSTUSENT5, MB_YESNO) == IDNO)
		{
			GotoDlgCtrl(&m_cbTapiVersion);
			return FALSE;
		}
	}
#endif

	// Add the dictionary elements from this dialog pane.
	TSPWizardaw.m_Dictionary["COMPANY_NAME"] = m_strCompanyName;
	TSPWizardaw.m_Dictionary["HAS_UI"] = (m_fGenerateUI) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["VERBOSE"] = (m_iHasComments == 0) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["TAPIVERSION"] = pszTapiVer[m_iTapiVersion];
	TSPWizardaw.m_Dictionary["TAPIVERSION2"] = pszTapiVer2[m_iTapiVersion];
	TSPWizardaw.m_Dictionary["ISTAPI3"] = (m_iTapiVersion == 2) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["WANT_TRACEMGR"] = (m_fTraceEvents) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["WINNTONLY"] = (m_fWinnt) ? "Yes" : "No";

	// Set the filenames so the user can press Finish before the last page
	CString strBase;
	VERIFY (TSPWizardaw.m_Dictionary.Lookup("Safe_root", strBase) != 0);

	for (int i = 0; g_Classes[i].strClassName != NULL; i++)
	{
		CString strClassName = g_Classes[i].strClassName;
		int iPos = strClassName.Find('@');
		CString strClass = strClassName;
		strClassName = strClass.Left(iPos) + strBase + strClass.Mid(iPos+1);
		TSPWizardaw.m_Dictionary[g_Classes[i].strKey] = strClassName;
	}

	TSPWizardaw.m_Dictionary["DEVICE_CPP_NAME"] = "device.cpp";
	TSPWizardaw.m_Dictionary["LINE_CPP_NAME"] = "line.cpp";
	TSPWizardaw.m_Dictionary["ADDRESS_CPP_NAME"] = "address.cpp";
	TSPWizardaw.m_Dictionary["CALL_CPP_NAME"] = "call.cpp";
	TSPWizardaw.m_Dictionary["CONFCALL_CPP_NAME"] = "confcall.cpp";
	TSPWizardaw.m_Dictionary["PHONE_CPP_NAME"] = "phone.cpp";

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom1Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom1Dlg::OnSetActive
//
// This is called when the user presses "Prev" to move back to this pane.
//
BOOL CCustom1Dlg::OnSetActive()
{
	// Turn on the finish button
	TSPWizardaw.SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);
	return TRUE;

}// CCustom1Dlg::OnSetActive

///////////////////////////////////////////////////////////////////////////
// CCustom1Dlg::OnInitDialog
//
// This is called to initialize the dialog
//
BOOL CCustom1Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();

#ifdef DEMO_VERSION
	GetDlgItem(IDC_HASUI)->EnableWindow(FALSE);
	GetDlgItem(IDC_WINNT)->EnableWindow(FALSE);
#endif

	return TRUE;

}// CCustom1Dlg::OnInitDialog

