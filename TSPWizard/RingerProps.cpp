/******************************************************************************/
//                                                                        
// RINGERPROPS.CPP
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
#include "RingerProps.h"
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
IMPLEMENT_DYNCREATE(CRingerProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CRingerProps, CDialog)
	//{{AFX_MSG_MAP(CRingerProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CRingerProps::CRingerProps
//
// Constructor for the ringer properties
//
CRingerProps::CRingerProps() : CDialog(CRingerProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CRingerProps)
	m_strRingStyles = _T("1");
	m_fWrite = FALSE;
	m_strDefRing = _T("1");
	//}}AFX_DATA_INIT

}// CRingerProps::CRingerProps

///////////////////////////////////////////////////////////////////////////
// CRingerProps::DoDataExchange
//
// Dialog data exchange function
//
void CRingerProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRingerProps)
	DDX_Control(pDX, IDC_SPIN5, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN2, m_ctlSpin1);
	DDX_Text(pDX, IDC_MAXRINGSTYLES, m_strRingStyles);
	DDX_Check(pDX, IDC_WRITE, m_fWrite);
	DDX_Text(pDX, IDC_DEFRINGSTYLE, m_strDefRing);
	//}}AFX_DATA_MAP

}// CRingerProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CRingerProps::OnOK
//
// Dismiss the dialog
//
void CRingerProps::OnOK() 
{
	UpdateData(TRUE);

	int iDefRing = atoi(m_strDefRing);
	int iMaxRing = atoi(m_strRingStyles);

	if (iDefRing == 0)
	{
		AfxMessageBox(IDS_CANNOTBEZERO);
		GotoDlgCtrl(GetDlgItem(IDC_DEFRINGSTYLE));
		return;
	}

	if (iMaxRing == 0)
	{
		AfxMessageBox(IDS_CANNOTBEZERO);
		GotoDlgCtrl(GetDlgItem(IDC_MAXRINGSTYLES));
		return;
	}

	if (iDefRing > iMaxRing)
	{
		AfxMessageBox(IDS_MAXPROB);
		GotoDlgCtrl(GetDlgItem(IDC_MAXRINGSTYLES));
		return;
	}

	TSPWizardaw.m_Dictionary["RINGER_CANWRITE"] = (m_fWrite) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["RINGER_STYLE_COUNT"] = m_strRingStyles;
	char chBuff[20];
	wsprintf(chBuff, "%d", iDefRing-1);
	TSPWizardaw.m_Dictionary["RINGER_STYLE_DEFAULT"] = chBuff;

	CDialog::OnOK();

}// CRingerProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CRingerProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CRingerProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CEdit*)GetDlgItem(IDC_MAXRINGSTYLES))->LimitText(3);
	((CEdit*)GetDlgItem(IDC_DEFRINGSTYLE))->LimitText(3);
	m_ctlSpin1.SetRange(1, 999);
	m_ctlSpin2.SetRange(1, 999);

	return TRUE;

}// CRingerProps::OnInitDialog
