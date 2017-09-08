/******************************************************************************/
//                                                                        
// MEDIACTLPROPS.CPP - Media control properties
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
#include "MediaCtlProps.h"
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
IMPLEMENT_DYNCREATE(CMediaCtlProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CMediaCtlProps, CDialog)
	//{{AFX_MSG_MAP(CMediaCtlProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CMediaCtlProps::CMediaCtlProps
//
// Constructor for the media control dialog
//
CMediaCtlProps::CMediaCtlProps() : CDialog(CMediaCtlProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMediaCtlProps)
	m_strMaxCallStates = _T("0");
	m_strMaxDigits = _T("0");
	m_strMaxMediaModes = _T("0");
	m_strMaxTones = _T("0");
	//}}AFX_DATA_INIT

}// CMediaCtlProps::CMediaCtlProps

///////////////////////////////////////////////////////////////////////////
// CMediaCtlProps::DoDataExchange
//
// Dialog data exchange for the dialog
//
void CMediaCtlProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaCtlProps)
	DDX_Control(pDX, IDC_SPIN5, m_ctlSpin4);
	DDX_Control(pDX, IDC_SPIN4, m_ctlSpin3);
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Text(pDX, IDC_MAXCS, m_strMaxCallStates);
	DDX_Text(pDX, IDC_MAXDIGITS, m_strMaxDigits);
	DDX_Text(pDX, IDC_MAXMEDIAMODES, m_strMaxMediaModes);
	DDX_Text(pDX, IDC_MAXTONES, m_strMaxTones);
	//}}AFX_DATA_MAP

}// CMediaCtlProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CMediaCtlProps::OnOK
//
// Dismiss the dialog
//
void CMediaCtlProps::OnOK() 
{
	UpdateData(TRUE);

	if (m_strMaxCallStates.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXCS));
		return;
	}

	if (m_strMaxDigits.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXDIGITS));
		return;
	}

	if (m_strMaxMediaModes.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXMEDIAMODES));
		return;
	}

	if (m_strMaxTones.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXTONES));
		return;
	}

	if (atoi(m_strMaxCallStates) == 0 && atoi(m_strMaxDigits) == 0 &&
		atoi(m_strMaxMediaModes) == 0 && atoi(m_strMaxTones) == 0)
	{
		AfxMessageBox(IDS_YOUMUSTENTER);
		GotoDlgCtrl(GetDlgItem(IDC_MAXDIGITS));
		return;
	}

	TSPWizardaw.m_Dictionary["MEDIACTL_MAXCALLSTATES"] = m_strMaxCallStates;
	TSPWizardaw.m_Dictionary["MEDIACTL_MAXDIGITS"] = m_strMaxDigits;
	TSPWizardaw.m_Dictionary["MEDIACTL_MAXMEDIAMODES"] = m_strMaxMediaModes;
	TSPWizardaw.m_Dictionary["MEDIACTL_MAXTONES"] = m_strMaxTones;

	CDialog::OnOK();

}// CMediaCtlProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CMediaCtlProps::OnInitDialog
//
// Initial dialog processing
//
BOOL CMediaCtlProps::OnInitDialog() 
{
	// Connect the variables
	CDialog::OnInitDialog();

	m_ctlSpin1.SetRange(0, 500);
	m_ctlSpin2.SetRange(0, 500);
	m_ctlSpin3.SetRange(0, 500);
	m_ctlSpin4.SetRange(0, 500);
	
	return TRUE;

}// CMediaCtlProps::OnInitDialog
