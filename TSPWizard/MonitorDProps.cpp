/******************************************************************************/
//                                                                        
// MONITORDPROPS.CPP - Monitor Digit properties
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
#include "MonitorDProps.h"
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
IMPLEMENT_DYNCREATE(CMonitorDProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CMonitorDProps, CDialog)
	//{{AFX_MSG_MAP(CMonitorDProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_arrModes[] = {

	{ "Pulse", "LINEDIGITMODE_PULSE" },
	{ "DTMF", "LINEDIGITMODE_DTMF" },
	{ "Down edges of DTMF digits (end)", "LINEDIGITMODE_DTMFEND" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CMonitorDProps::CMonitorDProps
//
// Constructor for the monitor digits properties
//
CMonitorDProps::CMonitorDProps() : CDialog(CMonitorDProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMonitorDProps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}// CMonitorDProps::CMonitorDProps

///////////////////////////////////////////////////////////////////////////
// CMonitorDProps::DoDataExchange
//
// Dialog data exchange for the monitor digits page
//
void CMonitorDProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorDProps)
	DDX_Control(pDX, IDC_LIST, m_lbList);
	//}}AFX_DATA_MAP

}// CMonitorDProps::DoDataExchange

////////////////////////////////////////////////////////////////////////////
// CMonitorDProps::OnInitDialog
//
// Init dialog handling
//
BOOL CMonitorDProps::OnInitDialog() 
{
	// Attach the controls
	CDialog::OnInitDialog();

	for (int i = 0; g_arrModes[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbList.AddString(g_arrModes[i].pszDesc);
		m_lbList.SetItemData(nPos, static_cast<LPARAM>(i));
	}
	return TRUE;

}// CMonitorDProps::OnInitDialog

////////////////////////////////////////////////////////////////////////////
// CMonitorDProps::OnOK
//
// Dismiss the dialog
//
void CMonitorDProps::OnOK() 
{
	CString strModes = "";
	for (int i = 0; i < m_lbList.GetCount(); i++)
	{
		if (m_lbList.GetSel(i))
		{
			if (!strModes.IsEmpty())
				strModes += " | ";
			int nPos = static_cast<int>(m_lbList.GetItemData(i));
			strModes += g_arrModes[nPos].pszTapi;
		}
	}

	if (strModes.IsEmpty())
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(&m_lbList);
		return;
	}

	TSPWizardaw.m_Dictionary["MONITORDIGIT_MODES"] = strModes;
	CDialog::OnOK();

}// CMonitorDProps::OnOK
