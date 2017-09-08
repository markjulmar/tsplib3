/******************************************************************************/
//                                                                        
// CSTM3DLG.CPP - Line parameters page for the TSP Wizard
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
#include "SecureProps.h"
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
IMPLEMENT_DYNCREATE(CSecureProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CSecureProps, CDialog)
	//{{AFX_MSG_MAP(CSecureProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CSecureProps::CSecureProps
//
// Constructor for the security page
//
CSecureProps::CSecureProps() : CDialog(CSecureProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CSecureProps)
	m_fSecure = FALSE;
	//}}AFX_DATA_INIT

}// CSecureProps::CSecureProps

///////////////////////////////////////////////////////////////////////////
// CSecureProps::DoDataExchange
//
// Dialog data exchange for the dialog
//
void CSecureProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecureProps)
	DDX_Check(pDX, IDC_SECURE, m_fSecure);
	//}}AFX_DATA_MAP

}// CSecureProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CSecureProps::OnOK
// 
// Dismiss the dialog
//
void CSecureProps::OnOK() 
{
	UpdateData(TRUE);
	TSPWizardaw.m_Dictionary["SECURE_ONCREATE"] = (m_fSecure) ? "Yes" : "No";
	CDialog::OnOK();

}// CSecureProps::OnOK
