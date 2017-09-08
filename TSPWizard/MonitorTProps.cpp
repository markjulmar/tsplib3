/******************************************************************************/
//                                                                        
// MONITORTPROPS.CPP - Monitor tones properties
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
#include "MonitorTProps.h"
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
IMPLEMENT_DYNCREATE(CMonitorTProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CMonitorTProps, CDialog)
	//{{AFX_MSG_MAP(CMonitorTProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CMonitorTProps::CMonitorTProps
//
// Constructor for the tone monitor page
//
CMonitorTProps::CMonitorTProps() : CDialog(CMonitorTProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMonitorTProps)
	m_strMaxFreq = _T("3");
	m_strMaxTones = _T("5");
	//}}AFX_DATA_INIT

}// CMonitorTProps::CMonitorTProps

///////////////////////////////////////////////////////////////////////////
// CMonitorTProps::DoDataExchange
//
// Dialog data exchange
//
void CMonitorTProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorTProps)
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Text(pDX, IDC_MAXFREQ, m_strMaxFreq);
	DDX_Text(pDX, IDC_MAXTONES, m_strMaxTones);
	//}}AFX_DATA_MAP

}// CMonitorTProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CMonitorTProps::OnInitDialog
// 
// Handle the initialization of the dialog
//
BOOL CMonitorTProps::OnInitDialog() 
{
	// Connect up the controls
	CDialog::OnInitDialog();

	m_ctlSpin1.SetRange(1, 10);
	m_ctlSpin2.SetRange(1, 30);
	
	return TRUE;

}// CMonitorTProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CMonitorTProps::OnOK
// 
// Dismiss the dialog
//
void CMonitorTProps::OnOK() 
{
	UpdateData(TRUE);

	if (m_strMaxFreq.IsEmpty() || atoi(m_strMaxFreq) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXFREQ));
		return;
	}

	if (m_strMaxTones.IsEmpty() || atoi(m_strMaxTones) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXTONES));
		return;
	}

	TSPWizardaw.m_Dictionary["MONITORTONE_MAXFREQ"] = m_strMaxFreq;
	TSPWizardaw.m_Dictionary["MONITORTONE_MAXTONES"] = m_strMaxTones;

	CDialog::OnOK();

}// CMonitorTProps::OnOK
