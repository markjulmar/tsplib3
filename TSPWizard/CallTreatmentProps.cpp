/******************************************************************************/
//                                                                        
// CALLTREATMENTPROPS.CPP - Call treatment properties
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
#include "CallTreatmentProps.h"
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
IMPLEMENT_DYNCREATE(CCallTreatmentProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCallTreatmentProps, CDialog)
	//{{AFX_MSG_MAP(CCallTreatmentProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_arrList[] = {

	{ "Silence", "LINECALLTREATMENT_SILENCE" },
	{ "Ringback", "LINECALLTREATMENT_RINGBACK" },
	{ "Busy", "LINECALLTREATMENT_BUSY" },
	{ "Music", "LINECALLTREATMENT_MUSIC" },
	{ NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////
// CCallTreatmentProps::CCallTreatmentProps
//
// Constructor for the call treatments page
//
CCallTreatmentProps::CCallTreatmentProps() : CDialog(CCallTreatmentProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CCallTreatmentProps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}// CCallTreatmentProps::CCallTreatmentProps

///////////////////////////////////////////////////////////////////////////
// CCallTreatmentProps::DoDataExchange
//
// Dialog data exchange
//
void CCallTreatmentProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallTreatmentProps)
	DDX_Control(pDX, IDC_LIST, m_lbList);
	//}}AFX_DATA_MAP

}// CCallTreatmentProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CCallTreatmentProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CCallTreatmentProps::OnInitDialog() 
{
	// Connect the controls
	CDialog::OnInitDialog();

	// Add our elements
	for (int i = 0; g_arrList[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbList.AddString(g_arrList[i].pszDesc);
		m_lbList.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	return TRUE;

}// CCallTreatmentProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CCallTreatmentProps::OnOK
//
// Dismiss the dialog
//
void CCallTreatmentProps::OnOK() 
{
	int iCount = 0;
	char chBuff[20];
	for (int i = 0; i < m_lbList.GetCount(); i++)
	{
		if (m_lbList.GetSel(i))
		{
			CString strLine;
			int nPos = static_cast<int>(m_lbList.GetItemData(i));
			strLine = "{ ";
			strLine += g_arrList[nPos].pszTapi;
			strLine += ", _T(\"";
			strLine += g_arrList[nPos].pszDesc;
			strLine += "\") },";

			wsprintf(chBuff, "CALLTREATMENTMSG_%d", iCount++);
			TSPWizardaw.m_Dictionary[chBuff] = strLine;
		}
	}

	if (iCount == 0)
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(GetDlgItem(IDC_LIST));
		return;
	}

	wsprintf(chBuff, "%d", iCount);
	TSPWizardaw.m_Dictionary["CALLTREATMENT_COUNT"] = chBuff;
	CDialog::OnOK();

}// CCallTreatmentProps::OnOK
