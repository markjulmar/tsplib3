/******************************************************************************/
//                                                                        
// GENERATETProps.cpp - Generate Tone properties
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
#include "GenerateTProps.h"
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
IMPLEMENT_DYNCREATE(CGenerateTProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CGenerateTProps, CDialog)
	//{{AFX_MSG_MAP(CGenerateTProps)
	ON_LBN_SELCHANGE(IDC_TONELIST, OnSelchangeTonelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_arrTones[] = {

	{ "Custom Tone", "LINETONEMODE_CUSTOM" },
	{ "Ringback tone", "LINETONEMODE_RINGBACK" },
	{ "Busy tone", "LINETONEMODE_BUSY" },
	{ "Generic beep tone", "LINETONEMODE_BEEP" },
	{ "Billing tone", "LINETONEMODE_BILLING" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CGenerateTProps::CGenerateTProps
//
// Constructor for the tone generation page
//
CGenerateTProps::CGenerateTProps() : CDialog(CGenerateTProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CGenerateTProps)
	m_strMaxTones = _T("3");
	//}}AFX_DATA_INIT

}// CGenerateTProps::CGenerateTProps

///////////////////////////////////////////////////////////////////////////
// CGenerateTProps::DoDataExchange
//
// Dialog Data Exchange for the tone generation page
//
void CGenerateTProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerateTProps)
	DDX_Control(pDX, IDC_TONELIST, m_lbTones);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_MAXTONES, m_edtMaxTones);
	DDX_Text(pDX, IDC_MAXTONES, m_strMaxTones);
	//}}AFX_DATA_MAP

}// CGenerateTProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CGenerateTProps::OnOK
//
// Dismiss the dialog
//
void CGenerateTProps::OnOK() 
{
	UpdateData(TRUE);

	bool fCustom = false;
	CString strModes = "";
	for (int i = 0; i < m_lbTones.GetCount(); i++)
	{
		if (m_lbTones.GetSel(i))
		{
			int nPos = static_cast<int>(m_lbTones.GetItemData(i));
			if (nPos == 0)
				fCustom = true;

			if (!strModes.IsEmpty())
				strModes += " | ";
			strModes += g_arrTones[nPos].pszTapi;
		}
	}

	if (strModes.IsEmpty())
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(&m_lbTones);
		return;
	}

	TSPWizardaw.m_Dictionary["GENERATETONE_STDMODES"] = (strModes == "LINETONEMODE_CUSTOM") ? "Yes" : "No";

	if (fCustom)
	{
		if (m_strMaxTones.IsEmpty() || atoi(m_strMaxTones) == 0)
		{
			AfxMessageBox(IDS_CANNOTBEBLANK);
			GotoDlgCtrl(&m_edtMaxTones);
			return;
		}
		TSPWizardaw.m_Dictionary["GENERATETONE_MAXTONES"] = m_strMaxTones;
		TSPWizardaw.m_Dictionary["GENERATETONE_CUSTOM"] = "Yes";
	}
	else
	{
		TSPWizardaw.m_Dictionary["GENERATETONE_MAXTONES"] = "0";
		TSPWizardaw.m_Dictionary["GENERATETONE_CUSTOM"] = "No";
	}

	TSPWizardaw.m_Dictionary["GENERATETONE_MODES"] = strModes;
	CDialog::OnOK();

}// CGenerateTProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CGenerateTProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CGenerateTProps::OnInitDialog() 
{
	// Connect the dialog elements
	CDialog::OnInitDialog();

	for (int i = 0; g_arrTones[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbTones.AddString(g_arrTones[i].pszDesc);
		m_lbTones.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	m_ctlSpin1.SetRange(1, 100);

	return TRUE;

}// CGenerateTProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CGenerateTProps::OnSelchangeTonelist
//
// Change the state of the edit
//
void CGenerateTProps::OnSelchangeTonelist() 
{
	for (int i = 0; i < m_lbTones.GetCount(); i++)
	{
		if (static_cast<int>(m_lbTones.GetItemData(i)) == 0)
		{
			m_edtMaxTones.EnableWindow(m_lbTones.GetSel(i) != 0);
			break;
		}
	}

}// CGenerateTProps::OnSelchangeTonelist
