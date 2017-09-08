/******************************************************************************/
//                                                                        
// BLINDXFERPROPS.CPP
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
#include "BlindXferProps.h"
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
IMPLEMENT_DYNCREATE(CBlindXferProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CBlindXferProps, CDialog)
	//{{AFX_MSG_MAP(CBlindXferProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CBlindXferProps::CBlindXferProps
//
// Constructor for the blind transfer properties
//
CBlindXferProps::CBlindXferProps() : CDialog(CBlindXferProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CBlindXferProps)
	m_fNoInternal = FALSE;
	m_fNoExternal = FALSE;
	//}}AFX_DATA_INIT

}// CBlindXferProps::CBlindXferProps

///////////////////////////////////////////////////////////////////////////
// CBlindXferProps::DoDataExchange
//
// Dialog data exchange
//
void CBlindXferProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlindXferProps)
	DDX_Check(pDX, IDC_NOINTERNAL, m_fNoInternal);
	DDX_Check(pDX, IDC_NOEXTERNAL, m_fNoExternal);
	//}}AFX_DATA_MAP

}// CBlindXferProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CBlindXferProps::OnOK
//
// Dismiss the dialog
//
void CBlindXferProps::OnOK() 
{
	UpdateData(TRUE);
	TSPWizardaw.m_Dictionary["BLINDXFER_NOINT"] = (m_fNoInternal) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["BLINDXFER_NOEXT"] = (m_fNoExternal) ? "Yes" : "No";
	CDialog::OnOK();

}// CBlindXferProps::OnOK

