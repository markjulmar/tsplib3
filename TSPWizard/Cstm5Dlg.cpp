/******************************************************************************/
//                                                                        
// CSTM5DLG.CPP - Class name dialog
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
#include "cstm5dlg.h"
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
BEGIN_MESSAGE_MAP(CCustom5Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom5Dlg)
	ON_LBN_SELCHANGE(IDC_CLASSES, OnSelchangeClasses)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR strBaseName;
	LPCSTR strClassName;
	LPCSTR strCPP;
	LPCSTR strHeader;
	bool fAllowEditCPP;
	LPCSTR strKey;

} g_Classes[] = {

	{ "CServiceProvider" , "C@ServiceProvider", "@.cpp", "@.h", false, NULL },
	{ "CTSPIDevice", "C@Device", "device.cpp", "@.h", true, NULL },
	{ "CTSPILineConnection", "C@Line", "line.cpp", "@.h", true, "OVERRIDE_LINE" },
	{ "CTSPIAddressInfo", "C@Address", "address.cpp", "@.h", true, "OVERRIDE_ADDRESS" },
	{ "CTSPICallAppearance", "C@Call", "call.cpp", "@.h", true, "OVERRIDE_CALL" },
	{ "CTSPIConferenceCall", "C@ConfCall", "confcall.cpp", "@.h", true, "OVERRIDE_CONFCALL" },
	{ "CTSPIPhoneConnection", "C@Phone", "phone.cpp", "@.h", true, "OVERRIDE_PHONE" },
	{ NULL, NULL, NULL, NULL, false, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::CCustom5Dlg
//
// Constructor for the final page
//
CCustom5Dlg::CCustom5Dlg() : CAppWizStepDlg(CCustom5Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom5Dlg)
	m_strBaseClass = _T("");
	m_nCurrSel = -1;
	//}}AFX_DATA_INIT

}// CCustom5Dlg::CCustom5Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::DoDataExchange
//
// Dialog Data Exchange for the final page
//
void CCustom5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom5Dlg)
	DDX_Control(pDX, IDC_CLASSES, m_lbClasses);
	//}}AFX_DATA_MAP

	if (m_nCurrSel != -1)
	{
		DDX_Text(pDX, IDC_BASECLASS, m_strBaseClass);
		DDX_Text(pDX, IDC_CLASSNAME, m_strClassNames[m_nCurrSel]);
		DDX_Text(pDX, IDC_CPPFILE, m_strCPPFiles[m_nCurrSel]);
		DDX_Text(pDX, IDC_HEADER, m_strHFiles[m_nCurrSel]);
	}

}// CCustom5Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom5Dlg::OnKillActive()
{
	if (!UpdateData(TRUE))
		return FALSE;

	// Add the elements to the map.
	TSPWizardaw.m_Dictionary["SP_CLASS_NAME"] = m_strClassNames[0];
	TSPWizardaw.m_Dictionary["DEVICE_CLASS_NAME"] = m_strClassNames[1];
	TSPWizardaw.m_Dictionary["LINE_CLASS_NAME"] = m_strClassNames[2];
	TSPWizardaw.m_Dictionary["ADDRESS_CLASS_NAME"] = m_strClassNames[3];
	TSPWizardaw.m_Dictionary["CALL_CLASS_NAME"] = m_strClassNames[4];
	TSPWizardaw.m_Dictionary["CONFCALL_CLASS_NAME"] = m_strClassNames[5];
	TSPWizardaw.m_Dictionary["PHONE_CLASS_NAME"] = m_strClassNames[6];

	// Add the CPP files to the map
	TSPWizardaw.m_Dictionary["DEVICE_CPP_NAME"] = m_strCPPFiles[1];
	TSPWizardaw.m_Dictionary["LINE_CPP_NAME"] = m_strCPPFiles[2];
	TSPWizardaw.m_Dictionary["ADDRESS_CPP_NAME"] = m_strCPPFiles[3];
	TSPWizardaw.m_Dictionary["CALL_CPP_NAME"] = m_strCPPFiles[4];
	TSPWizardaw.m_Dictionary["CONFCALL_CPP_NAME"] = m_strCPPFiles[5];
	TSPWizardaw.m_Dictionary["PHONE_CPP_NAME"] = m_strCPPFiles[6];

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom5Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::Validate
//
// Validate the items selected
//
bool CCustom5Dlg::Validate()
{
	UpdateData(TRUE);

	if (m_strClassNames[m_nCurrSel].IsEmpty())
	{
		GotoDlgCtrl(GetDlgItem(IDC_CLASSNAME));
		return false;
	}

	if (m_strCPPFiles[m_nCurrSel].IsEmpty())
	{
		GotoDlgCtrl(GetDlgItem(IDC_CPPFILE));
		return false;
	}

	if (m_strHFiles[m_nCurrSel].IsEmpty())
	{
		GotoDlgCtrl(GetDlgItem(IDC_HEADER));
		return false;
	}

	return true;

}// CCustom5Dlg::Validate

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::OnSelchangeClasses
//
// This function moves through the various class types
//
void CCustom5Dlg::OnSelchangeClasses() 
{
	int nCurSel = m_lbClasses.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	int iSel = static_cast<int>(m_lbClasses.GetItemData(nCurSel));

	if (m_nCurrSel != -1)
	{
		if (!Validate())
			m_lbClasses.SetCurSel(m_nCurrSel);
	}

	m_nCurrSel = iSel;
	m_strBaseClass = g_Classes[iSel].strBaseName;
	UpdateData(FALSE);
	GetDlgItem(IDC_CPPFILE)->EnableWindow (g_Classes[iSel].fAllowEditCPP);

}// CCustom5Dlg::OnSelchangeClasses

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::OnInitDialog
//
// Initializes the dialog contents
//
BOOL CCustom5Dlg::OnInitDialog() 
{
	// Connect the controls
	CAppWizStepDlg::OnInitDialog();

	// Now determine the base filename.
	CString strBase;
	VERIFY (TSPWizardaw.m_Dictionary.Lookup("Safe_root", strBase) != 0);

	for (int i = 0; i < TOTAL_CLASSES; i++)
	{
		m_strClassNames[i] = g_Classes[i].strClassName;
		if (m_strClassNames[i].Find('@') != -1)
		{
			int iPos = m_strClassNames[i].Find('@');
			CString strClass = m_strClassNames[i];
			m_strClassNames[i] = strClass.Left(iPos) + strBase + strClass.Mid(iPos+1);
		}

		m_strCPPFiles[i] = g_Classes[i].strCPP;
		if (m_strCPPFiles[i].Find('@') != -1)
		{
			m_strCPPFiles[i] = strBase + ".cpp";
			m_strCPPFiles[i].MakeLower();
		}

		m_strHFiles[i] = g_Classes[i].strHeader;
		if (m_strHFiles[i].Find('@') != -1)
		{
			m_strHFiles[i] = strBase + ".h";
			m_strHFiles[i].MakeLower();
		}

		int nPos = m_lbClasses.AddString(m_strClassNames[i]);
		ASSERT (nPos != LB_ERR);
		m_lbClasses.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	m_lbClasses.SetCurSel(0);
	OnSelchangeClasses();

	return TRUE;

}// CCustom5Dlg::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CCustom5Dlg::OnSetActive
//
// This is called to show the window
//
BOOL CCustom5Dlg::OnSetActive()
{
	// Turn on the finish button
	TSPWizardaw.SetWizardButtons(PSWIZB_FINISH | PSWIZB_BACK);

	CString strValue;
	for (int i = 0; g_Classes[i].strBaseName != NULL; i++)
	{
		if (g_Classes[i].strKey != NULL && g_Classes[i].strKey[0] != '\0')
		{
			int nPos = m_lbClasses.FindStringExact(-1, m_strClassNames[i]);
			if (TSPWizardaw.m_Dictionary.Lookup(g_Classes[i].strKey, strValue) && strValue == "Yes")
			{
				if (nPos == LB_ERR)
				{
					int iPos = m_lbClasses.AddString(m_strClassNames[i]);
					m_lbClasses.SetItemData(iPos, static_cast<LPARAM>(i));
				}
			}
			else
			{
				if (nPos != LB_ERR)
					m_lbClasses.DeleteString(nPos);
			}
		}
	}
	m_lbClasses.SetCurSel(0);
	OnSelchangeClasses();

	return TRUE;

}// CCustom5Dlg::OnSetActive

