/******************************************************************************/
//                                                                        
// PICKUPPROPS.CPP - Pickup properties
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
#include "PickupProps.h"
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
IMPLEMENT_DYNCREATE(CPickupProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CPickupProps, CDialog)
	//{{AFX_MSG_MAP(CPickupProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CPickupProps::CPickupProps
//
// Constructor for the pickup properties
//
CPickupProps::CPickupProps() : CDialog(CPickupProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CPickupProps)
	m_fCallWaiting = FALSE;
	m_fGroupId = FALSE;
	//}}AFX_DATA_INIT

}// CPickupProps::CPickupProps

///////////////////////////////////////////////////////////////////////////
// CPickupProps::DoDataExchange
//
// Dialog data exchange
//
void CPickupProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickupProps)
	DDX_Check(pDX, IDC_CALLWAITING, m_fCallWaiting);
	DDX_Check(pDX, IDC_GROUPID, m_fGroupId);
	//}}AFX_DATA_MAP

}// CPickupProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CPickupProps::OnOK
//
// Dismiss the dialog
//
void CPickupProps::OnOK() 
{
	UpdateData(TRUE);

	TSPWizardaw.m_Dictionary["PICKUP_GROUPREQ"] = (m_fGroupId) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["PICKUP_CALLWAIT"] = (m_fCallWaiting) ? "Yes" : "No";
	CDialog::OnOK();

}// CPickupProps::OnOK
