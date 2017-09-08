/******************************************************************************/
//                                                                        
// ACCEPTPROPS.CPP - Accept properties
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
#include "AcceptProps.h"
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
IMPLEMENT_DYNCREATE(CAcceptProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAcceptProps, CDialog)
	//{{AFX_MSG_MAP(CAcceptProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CAcceptProps::CAcceptProps
//
// Constructor for the accept properties
//
CAcceptProps::CAcceptProps() : CDialog(CAcceptProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CAcceptProps)
	m_fAcceptToAlert = FALSE;
	//}}AFX_DATA_INIT

}// CAcceptProps::CAcceptProps

///////////////////////////////////////////////////////////////////////////
// CAcceptProps::DoDataExchange
//
// Dialog data exchange
//
void CAcceptProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcceptProps)
	DDX_Check(pDX, IDC_ACCEPTTOALERT, m_fAcceptToAlert);
	//}}AFX_DATA_MAP

}// CAcceptProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CAcceptProps::OnOK
//
// Dismiss the dialog
//
void CAcceptProps::OnOK() 
{
	UpdateData(TRUE);
	TSPWizardaw.m_Dictionary["ACCEPTTOALERT"] = (m_fAcceptToAlert) ? "Yes" : "No";
	CDialog::OnOK();

}// CAcceptProps::OnOK
