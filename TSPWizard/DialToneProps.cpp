/******************************************************************************/
//                                                                        
// DIALTONEPROPS.CPP - Dialtone properties
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
#include "DialtoneProps.h"
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
IMPLEMENT_DYNCREATE(CDialToneProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CDialToneProps, CDialog)
	//{{AFX_MSG_MAP(CDialToneProps)
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

	{ "Normal", "LINEDIALTONEMODE_NORMAL" },
	{ "Special condition", "LINEDIALTONEMODE_SPECIAL" },
	{ "Internal", "LINEDIALTONEMODE_INTERNAL" },
	{ "External", "LINEDIALTONEMODE_EXTERNAL" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CDialToneProps::CDialToneProps
//
// Constructor for the dialtone page
//
CDialToneProps::CDialToneProps() : CDialog(CDialToneProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CDialToneProps)
	//}}AFX_DATA_INIT

}// CDialToneProps::CDialToneProps

///////////////////////////////////////////////////////////////////////////
// CDialToneProps::DoDataExchange
//
// Dialog Data Exchange for the dialtone page
//
void CDialToneProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialToneProps)
	DDX_Control(pDX, IDC_DIALTONES, m_lbDialtones);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDialToneProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CDialToneProps::OnInitDialog() 
{
	// Connect the dialog elements
	CDialog::OnInitDialog();

	for (int i = 0; g_arrTones[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbDialtones.AddString(g_arrTones[i].pszDesc);
		m_lbDialtones.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	return TRUE;

}// CDialToneProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CDialToneProps::OnOK
//
// Dismiss the dialog
//
void CDialToneProps::OnOK() 
{
	bool fInternal = false;
	bool fExternal = false;

	CString strModes = "";
	for (int i = 0; i < m_lbDialtones.GetCount(); i++)
	{
		if (m_lbDialtones.GetSel(i))
		{
			int nPos = static_cast<int>(m_lbDialtones.GetItemData(i));
			if (nPos == 2)
				fInternal = true;
			else if (nPos == 3)
				fExternal = true;

			if (!strModes.IsEmpty())
				strModes += " | ";
			strModes += g_arrTones[nPos].pszTapi;
		}
	}

	if (strModes.IsEmpty())
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(&m_lbDialtones);
		return;
	}

	strModes += " | LINEDIALTONEMODE_UNKNOWN | LINEDIALTONEMODE_UNAVAIL";

	TSPWizardaw.m_Dictionary["DIALTONE_MODES"] = strModes;
	TSPWizardaw.m_Dictionary["DIALTONE_HASINTERNAL"] = (fInternal) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["DIALTONE_HASEXTERNAL"] = (fExternal) ? "Yes" : "No";

	TSPWizardaw.m_Dictionary["DIALTONE_DEFAULT"] = 
		(fInternal) ? "LINEDIALTONEMODE_INTERNAL" :
		(strModes.Find("LINEDIALTONEMODE_NORMAL") != -1) ? "LINEDIALTONEMODE_NORMAL" : 
		(fExternal) ? "LINEDIALTONEMODE_EXTERNAL" : 
		"LINEDIALTONEMODE_UNKNOWN";

	CDialog::OnOK();

}// CDialToneProps::OnOK
