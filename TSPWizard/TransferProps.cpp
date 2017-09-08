/******************************************************************************/
//                                                                        
// TRANSFERPROPS.CPP - Consultation transfer properties
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
#include "TransferProps.h"
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
IMPLEMENT_DYNCREATE(CTransferProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CTransferProps, CDialog)
	//{{AFX_MSG_MAP(CTransferProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CTransferProps::CTransferProps
//
// Constructor for the transfer properties
//
CTransferProps::CTransferProps() : CDialog(CTransferProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CTransferProps)
	m_fHoldTransfer = FALSE;
	m_fMakeCallTransfer = FALSE;
	m_strMaxOnHoldPend = _T("1");
	//}}AFX_DATA_INIT

}// CTransferProps::CTransferProps

///////////////////////////////////////////////////////////////////////////
// CTransferProps::DoDataExhange
//
// Dialog data exchange for the transfer dialog
//
void CTransferProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferProps)
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Check(pDX, IDC_HOLDTRANSF, m_fHoldTransfer);
	DDX_Check(pDX, IDC_MAKETRANSF, m_fMakeCallTransfer);
	DDX_Text(pDX, IDC_MAXCALLSINCONFERENCE, m_strMaxOnHoldPend);
	//}}AFX_DATA_MAP

}// CTransferProps::DoDataExhange

/////////////////////////////////////////////////////////////////////////////
// CTransferProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CTransferProps::OnInitDialog() 
{
	// Connect up the controls
	CDialog::OnInitDialog();

	// Set the spinner
	m_ctlSpin1.SetRange(1, 99);
	
	return TRUE;

}// CTransferProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CTransferProps::OnOK
//
// Dismiss the dialog
//
void CTransferProps::OnOK() 
{
	UpdateData(TRUE);
	
	if (m_strMaxOnHoldPend.IsEmpty() || atoi(m_strMaxOnHoldPend) == 0)
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXCALLSINCONFERENCE));
		return;
	}

	TSPWizardaw.m_Dictionary["TRANSF_HELD"] = (m_fHoldTransfer) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["TRANSF_MAKECALL"] = (m_fMakeCallTransfer) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["TRANSF_MAXONHOLD"] = m_strMaxOnHoldPend;

	// Dismiss
	CDialog::OnOK();

}// CTransferProps::OnOK
