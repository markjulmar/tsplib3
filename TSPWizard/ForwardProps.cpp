/******************************************************************************/
//                                                                        
// FORWARDPROPS.CPP - Forward properties
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
#include "ForwardProps.h"
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
IMPLEMENT_DYNCREATE(CForwardProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CForwardProps, CDialog)
	//{{AFX_MSG_MAP(CForwardProps)
	ON_BN_CLICKED(IDC_CALLERID, OnCallerid)
	ON_BN_CLICKED(IDC_SPECIFYRINGS, OnSpecifyrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static LPCSTR g_arrModes[] = { "Unconditional forward", "Forward on busy or no answer", NULL };

///////////////////////////////////////////////////////////////////////////
// CForwardProps::CForwardProps
//
// Constructor for the forward properties page
//
CForwardProps::CForwardProps() : CDialog(CForwardProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CForwardProps)
	m_fBusyNA = FALSE;
	m_fRequiresConsult = FALSE;
	m_fIntExtSeperate = FALSE;
	m_fStatusValid = FALSE;
	m_strMaxFwdCallid = _T("1");
	m_strMaxFwdEntries = _T("1");
	m_strMaxRings = _T("5");
	m_strMinRings = _T("1");
	m_fSupportsCallerid = FALSE;
	m_fSpecifyRings = FALSE;
	//}}AFX_DATA_INIT

}// CForwardProps::CForwardProps

///////////////////////////////////////////////////////////////////////////
// CForwardProps::DoDataExchange
//
// Dialog data exchange for the forward properties
//
void CForwardProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CForwardProps)
	DDX_Control(pDX, IDC_MAXRINGS, m_edtMaxRings);
	DDX_Control(pDX, IDC_MINRINGS, m_edtMinRings);
	DDX_Control(pDX, IDC_MAXFWDCALLID, m_edtCallid);
	DDX_Control(pDX, IDC_SPIN5, m_ctlSpin4);
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin3);
	DDX_Control(pDX, IDC_SPIN2, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_MODELIST, m_lbModes);
	DDX_Check(pDX, IDC_BUSYNA, m_fBusyNA);
	DDX_Check(pDX, IDC_CONSULTATION, m_fRequiresConsult);
	DDX_Check(pDX, IDC_INTEXT, m_fIntExtSeperate);
	DDX_Check(pDX, IDC_STATUSVALID, m_fStatusValid);
	DDX_Text(pDX, IDC_MAXFWDCALLID, m_strMaxFwdCallid);
	DDX_Text(pDX, IDC_MAXFWDENTRIES, m_strMaxFwdEntries);
	DDX_Text(pDX, IDC_MAXRINGS, m_strMaxRings);
	DDX_Text(pDX, IDC_MINRINGS, m_strMinRings);
	DDX_Check(pDX, IDC_CALLERID, m_fSupportsCallerid);
	DDX_Check(pDX, IDC_SPECIFYRINGS, m_fSpecifyRings);
	//}}AFX_DATA_MAP

}// CForwardProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CForwardProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CForwardProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (int i = 0; g_arrModes[i] != NULL; i++)
		m_lbModes.AddString(g_arrModes[i]);

	m_ctlSpin1.SetRange(0, 99);
	m_ctlSpin2.SetRange(0, 99);
	m_ctlSpin3.SetRange(0, 99);
	m_ctlSpin4.SetRange(0, 99);

	return TRUE;

}// CForwardProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CForwardProps::OnOK
//
// Dismiss the dialog
//
void CForwardProps::OnOK() 
{
	UpdateData(TRUE);

	if (m_lbModes.GetSel(0) == 0 &&
		m_lbModes.GetSel(1) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MODELIST));
		return;
	}

	if (m_strMaxFwdEntries.IsEmpty() || atoi(m_strMaxFwdEntries) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXFWDENTRIES));
		return;
	}
	
	TSPWizardaw.m_Dictionary["FWD_MAXENTRIES"] = m_strMaxFwdEntries;
	TSPWizardaw.m_Dictionary["FWD_REQUIRESCONSULT"] = (m_fRequiresConsult) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["FWD_STATUSVALID"] = (m_fStatusValid) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["FWD_INTEXTSEPERATE"] = (m_fIntExtSeperate) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["FWD_BUSYNASEPERATE"] = (m_fBusyNA) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["FWD_SPECIFYRINGS"] = (m_fSpecifyRings) ? "Yes" : "No";

	if (m_fSpecifyRings)
	{
		if (m_strMaxRings.IsEmpty() || m_strMinRings.IsEmpty())
		{
			AfxMessageBox(IDS_CANNOTBEBLANK);
			GotoDlgCtrl(GetDlgItem(IDC_MINRINGS));
			return;
		}

		if (atol(m_strMaxRings) < atol(m_strMinRings))
		{
			AfxMessageBox(IDS_MINMAXERR);
			GotoDlgCtrl(GetDlgItem(IDC_MINRINGS));
			return;
		}

		TSPWizardaw.m_Dictionary["FWD_MINRINGS"] = m_strMinRings;
		TSPWizardaw.m_Dictionary["FWD_MAXRINGS"] = m_strMaxRings;
	}
	else
	{
		TSPWizardaw.m_Dictionary["FWD_MINRINGS"] = "0";
		TSPWizardaw.m_Dictionary["FWD_MAXRINGS"] = "0";
	}

	if (m_fSupportsCallerid)
	{
		if (m_strMaxFwdCallid.IsEmpty())
		{
			AfxMessageBox(IDS_CANNOTBEBLANK);
			GotoDlgCtrl(GetDlgItem(IDC_MAXFWDCALLID));
			return;
		}
		TSPWizardaw.m_Dictionary["FWD_MAXSPECIFIC"] = m_strMaxFwdCallid;
	}
	else
	{
		TSPWizardaw.m_Dictionary["FWD_MAXSPECIFIC"] = "0";
	}

	CString strModes = "LINEFORWARDMODE_UNKNOWN | LINEFORWARDMODE_UNAVAIL";
	if (m_lbModes.GetSel(0))
	{
		strModes += " | LINEFORWARDMODE_UNCOND";
		if (m_fIntExtSeperate)
			strModes += " | LINEFORWARDMODE_UNCONDINTERNAL | LINEFORWARDMODE_UNCONDEXTERNAL";
		if (m_fSupportsCallerid)
			strModes += " | LINEFORWARDMODE_UNCONDSPECIFIC";
	}

	if (m_lbModes.GetSel(1))
	{
		strModes += " | LINEFORWARDMODE_BUSYNA";
		if (m_fIntExtSeperate)
			strModes += " | LINEFORWARDMODE_BUSYNAINTERNAL | LINEFORWARDMODE_BUSYNAEXTERNAL";
		if (m_fSupportsCallerid)
			strModes += " | LINEFORWARDMODE_BUSYNASPECIFIC";

		if (m_fBusyNA)
		{
			strModes += " | LINEFORWARDMODE_BUSY | LINEFORWARDMODE_NOANSW";
			if (m_fIntExtSeperate)
				strModes += " | LINEFORWARDMODE_BUSYINTERNAL | LINEFORWARDMODE_BUSYEXTERNAL | LINEFORWARDMODE_NOANSWINTERNAL | LINEFORWARDMODE_NOANSWEXTERNAL";
			if (m_fSupportsCallerid)
				strModes += " | LINEFORWARDMODE_BUSYSPECIFIC | LINEFORWARDMODE_NOANSWSPECIFIC";
		}
	}

	TSPWizardaw.m_Dictionary["FWDMODES"] = strModes;

	CDialog::OnOK();

}// CForwardProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CForwardProps::OnCallerid
//
// Enable/Disable the entries/callerid
//
void CForwardProps::OnCallerid() 
{
	UpdateData(TRUE);
	m_edtCallid.EnableWindow(m_fSupportsCallerid);

}// CForwardProps::OnCallerid

/////////////////////////////////////////////////////////////////////////////
// CForwardProps::OnSpecifyrings
//
// Enable/Disable the rings boxes
//
void CForwardProps::OnSpecifyrings() 
{
	UpdateData(TRUE);
	m_edtMinRings.EnableWindow(m_fSpecifyRings);
	m_edtMaxRings.EnableWindow(m_fSpecifyRings);

}// CForwardProps::OnSpecifyrings
