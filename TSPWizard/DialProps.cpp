/******************************************************************************/
//                                                                        
// DIALPROPS.CPP - Dialing properties
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
#include "DialProps.h"
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
IMPLEMENT_DYNCREATE(CDialProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CDialProps, CDialog)
	//{{AFX_MSG_MAP(CDialProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CDialProps::CDialProps
//
// Constructor for the dialing properties
//
CDialProps::CDialProps() : CDialog(CDialProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CDialProps)
	m_strDefaultDigit = _T("0");
	m_strMaxDigit = _T("0");
	m_strMinDigit = _T("0");
	m_strDefaultDuration = _T("0");
	m_strMaxDuration = _T("0");
	m_strMinDuration = _T("0");
	m_strDefaultPause = _T("0");
	m_strMaxPause = _T("0");
	m_strMinPause = _T("0");
	m_strDefaultDialtone = _T("0");
	m_strMaxDialtone = _T("0");
	m_strMinDialtone = _T("0");
	//}}AFX_DATA_INIT

}// CDialProps::CDialProps

///////////////////////////////////////////////////////////////////////////
// CDialProps::DoDataExchange
//
// Dialog data exchange
//
void CDialProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialProps)
	DDX_Control(pDX, IDC_SPIN9, m_ctlSpin12);
	DDX_Control(pDX, IDC_SPIN8, m_ctlSpin11);
	DDX_Control(pDX, IDC_SPIN7, m_ctlSpin10);
	DDX_Control(pDX, IDC_SPIN6, m_ctlSpin9);
	DDX_Control(pDX, IDC_SPIN5, m_ctlSpin8);
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin7);
	DDX_Control(pDX, IDC_SPIN2, m_ctlSpin6);
	DDX_Control(pDX, IDC_SPIN13, m_ctlSpin5);
	DDX_Control(pDX, IDC_SPIN12, m_ctlSpin4);
	DDX_Control(pDX, IDC_SPIN11, m_ctlSpin3);
	DDX_Control(pDX, IDC_SPIN10, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Text(pDX, IDC_DIGIT_DEF, m_strDefaultDigit);
	DDX_Text(pDX, IDC_DIGIT_MAX, m_strMaxDigit);
	DDX_Text(pDX, IDC_DIGIT_MIN, m_strMinDigit);
	DDX_Text(pDX, IDC_DUR_DEF, m_strDefaultDuration);
	DDX_Text(pDX, IDC_DUR_MAX, m_strMaxDuration);
	DDX_Text(pDX, IDC_DUR_MIN, m_strMinDuration);
	DDX_Text(pDX, IDC_PAUSE_DEF, m_strDefaultPause);
	DDX_Text(pDX, IDC_PAUSE_MAX, m_strMaxPause);
	DDX_Text(pDX, IDC_PAUSE_MIN, m_strMinPause);
	DDX_Text(pDX, IDC_TONE_DEF, m_strDefaultDialtone);
	DDX_Text(pDX, IDC_TONE_MAX, m_strMaxDialtone);
	DDX_Text(pDX, IDC_TONE_MIN, m_strMinDialtone);
	//}}AFX_DATA_MAP

}// CDialProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CDialProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CDialProps::OnInitDialog() 
{
	// Connect the variables up
	CDialog::OnInitDialog();

#if _MSC_VER < 1200
	m_ctlSpin1.SetRange(0, 0xffff);
	m_ctlSpin2.SetRange(0, 0xffff);
	m_ctlSpin3.SetRange(0, 0xffff);
	m_ctlSpin4.SetRange(0, 0xffff);
	m_ctlSpin5.SetRange(0, 0xffff);
	m_ctlSpin6.SetRange(0, 0xffff);
	m_ctlSpin7.SetRange(0, 0xffff);
	m_ctlSpin8.SetRange(0, 0xffff);
	m_ctlSpin9.SetRange(0, 0xffff);
	m_ctlSpin10.SetRange(0, 0xffff);
	m_ctlSpin11.SetRange(0, 0xffff);
	m_ctlSpin12.SetRange(0, 0xffff);

#else
	m_ctlSpin1.SetRange32(0, 0xffff);
	m_ctlSpin2.SetRange32(0, 0xffff);
	m_ctlSpin3.SetRange32(0, 0xffff);
	m_ctlSpin4.SetRange32(0, 0xffff);
	m_ctlSpin5.SetRange32(0, 0xffff);
	m_ctlSpin6.SetRange32(0, 0xffff);
	m_ctlSpin7.SetRange32(0, 0xffff);
	m_ctlSpin8.SetRange32(0, 0xffff);
	m_ctlSpin9.SetRange32(0, 0xffff);
	m_ctlSpin10.SetRange32(0, 0xffff);
	m_ctlSpin11.SetRange32(0, 0xffff);
	m_ctlSpin12.SetRange32(0, 0xffff);
#endif

	return TRUE;

}// CDialProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CDialProps::OnOK
//
// Dismiss the dialog
//
void CDialProps::OnOK() 
{
	UpdateData(TRUE);

	bool fMinMaxSupported = (
		atol(m_strMaxDigit) != 0 ||
		atol(m_strMinDigit) != 0 ||
		atol(m_strMaxDuration) != 0 ||
		atol(m_strMinDuration) != 0 ||
		atol(m_strMaxPause) != 0 ||
		atol(m_strMinPause) != 0 ||
		atol(m_strMaxDialtone) != 0 ||
		atol(m_strMinDialtone) != 0);

	bool fDefSupported = (
		atol(m_strDefaultDigit) != 0 ||
		atol(m_strDefaultDuration) != 0 ||
		atol(m_strDefaultPause) !=0 ||
		atol(m_strDefaultDialtone) != 0);

	if (fMinMaxSupported)
	{
		if (atol(m_strMaxDigit) < atol(m_strMinDigit))
		{
			AfxMessageBox(IDS_MINMAXERR);
			GotoDlgCtrl(GetDlgItem(IDC_DIGIT_MIN));
			return;
		}

		if (atol(m_strMaxDuration) < atol(m_strMinDuration))
		{
			AfxMessageBox(IDS_MINMAXERR);
			GotoDlgCtrl(GetDlgItem(IDC_DUR_MIN));
			return;
		}

		if (atol(m_strMaxPause) < atol(m_strMinPause) != 0)
		{
			AfxMessageBox(IDS_MINMAXERR);
			GotoDlgCtrl(GetDlgItem(IDC_PAUSE_MIN));
			return;
		}

		if (atol(m_strMaxDialtone) < atol(m_strMinDialtone))
		{
			AfxMessageBox(IDS_MINMAXERR);
			GotoDlgCtrl(GetDlgItem(IDC_TONE_MIN));
			return;
		}

		if (!fDefSupported)
		{
			if (AfxMessageBox(IDS_NEEDDEFAULTDP, MB_YESNO) == IDYES)
			{
				m_strDefaultDigit = m_strMinDigit;
				m_strDefaultDuration = m_strMinDuration;
				m_strDefaultPause = m_strMinPause;
				m_strDefaultDialtone = m_strMinDialtone;
			}
			else
			{
				GotoDlgCtrl(GetDlgItem(IDC_PAUSE_DEF));
				return;
			}
		}
	}
	else if (!fMinMaxSupported && !fDefSupported)
	{
		AfxMessageBox(IDS_YOUMUSTENTER);
		GotoDlgCtrl(GetDlgItem(IDC_PAUSE_MIN));
		return;
	}

	TSPWizardaw.m_Dictionary["DIALPARMS_MINMAX"] = (fMinMaxSupported) ? "Yes" : "No";
	if (fMinMaxSupported)
	{
		TSPWizardaw.m_Dictionary["DIALPARMS_MINDIGIT"] = m_strMinDigit;
		TSPWizardaw.m_Dictionary["DIALPARMS_MAXDIGIT"] = m_strMaxDigit;
		TSPWizardaw.m_Dictionary["DIALPARMS_MINDURATION"] = m_strMinDuration;
		TSPWizardaw.m_Dictionary["DIALPARMS_MAXDURATION"] = m_strMaxDuration;
		TSPWizardaw.m_Dictionary["DIALPARMS_MINPAUSE"] = m_strMinPause;
		TSPWizardaw.m_Dictionary["DIALPARMS_MAXPAUSE"] = m_strMaxPause;
		TSPWizardaw.m_Dictionary["DIALPARMS_MINDIALTONE"] = m_strMinDialtone;
		TSPWizardaw.m_Dictionary["DIALPARMS_MAXDIALTONE"] = m_strMaxDialtone;
	}

	TSPWizardaw.m_Dictionary["DIALPARMS_DEFDIGIT"] = m_strDefaultDigit;
	TSPWizardaw.m_Dictionary["DIALPARMS_DEFDURATION"] = m_strDefaultDuration;
	TSPWizardaw.m_Dictionary["DIALPARMS_DEFPAUSE"] = m_strDefaultPause;
	TSPWizardaw.m_Dictionary["DIALPARMS_DEFDIALTONE"] = m_strDefaultDialtone;

	CDialog::OnOK();

}// CDialProps::OnOK
