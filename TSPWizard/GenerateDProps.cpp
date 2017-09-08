/******************************************************************************/
//                                                                        
// GENERATEDPROPS.CPP
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
#include "GenerateDProps.h"
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
IMPLEMENT_DYNCREATE(CGenerateDProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CGenerateDProps, CDialog)
	//{{AFX_MSG_MAP(CGenerateDProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CGenerateDProps::CGenerateDProps
//
// Constructor for the generate digits properties
//
CGenerateDProps::CGenerateDProps() : CDialog(CGenerateDProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CGenerateDProps)
	m_fPulse = FALSE;
	m_fTone = FALSE;
	//}}AFX_DATA_INIT

}// CGenerateDProps::CGenerateDProps

///////////////////////////////////////////////////////////////////////////
// CGenerateDProps::DoDataExchange
//
// Dialog Data Exchange for the generate digits page
//
void CGenerateDProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerateDProps)
	DDX_Check(pDX, IDC_PULSE, m_fPulse);
	DDX_Check(pDX, IDC_TONE, m_fTone);
	//}}AFX_DATA_MAP

}// CGenerateDProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CGenerateDProps::OnOK
// 
// Dismiss the dialog
//
void CGenerateDProps::OnOK() 
{
	UpdateData(TRUE);

	if (!m_fTone && !m_fPulse)
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(GetDlgItem(IDC_PULSE));
		return;
	}

	TSPWizardaw.m_Dictionary["GENERATEDIGIT_PULSE"] = (m_fPulse) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["GENERATEDIGIT_TONE"] = (m_fTone) ? "Yes" : "No";

	if (m_fTone && !m_fPulse)
		TSPWizardaw.m_Dictionary["GENERATEDIGIT_MODE"] = "LINEDIGITMODE_DTMF";
	else if (m_fTone && m_fPulse)
		TSPWizardaw.m_Dictionary["GENERATEDIGIT_MODE"] = "LINEDIGITMODE_DTMF | LINEDIGITMODE_PULSE";
	else
		TSPWizardaw.m_Dictionary["GENERATEDIGIT_MODE"] = "LINEDIGITMODE_PULSE";

	CDialog::OnOK();

}// CGenerateDProps::OnOK
