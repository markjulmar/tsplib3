/******************************************************************************/
//                                                                        
// CHOOSER.CPP - Implements the CDialogChooser object
//                                                                        
// Copyright (C) 1998 Mark C. Smith, JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
// For internal use only
//                                                                        
// This file contains the source code for the CDialogChooser which moves
// the user between different dialogs in the wizard.
//
/******************************************************************************/

/*---------------------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "TSPWizard.h"
#include "chooser.h"
#include "TSPWizardaw.h"

/*---------------------------------------------------------------------------*/
// DEBUG STATEMENTS
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------------------*/
// GLOBALS AND CONSTANTS
/*---------------------------------------------------------------------------*/
const UINT FIRST_CUSTOM_STEP  = 1;
const UINT LAST_CUSTOM_STEP	  = LAST_DLG;

///////////////////////////////////////////////////////////////////////////
// CDialogChooser::CDialogChooser
//
// Constructor for the dialog chooser object
//
CDialogChooser::CDialogChooser()
{
	m_pDlgs[0] = NULL;
	m_pDlgs[1] = new CCustom1Dlg;
	m_pDlgs[2] = new CCustom2Dlg;
	m_pDlgs[3] = new CCustom2_3Dlg;
	m_pDlgs[4] = new CCustom3Dlg;
	m_pDlgs[5] = new CCustom4Dlg;
	m_pDlgs[6] = new CCustom5Dlg;
	m_nCurrDlg = 0;

}// CDialogChooser::CDialogChooser

///////////////////////////////////////////////////////////////////////////
// CDialogChooser::~CDialogChooser
//
// Destructor for the dialog chooser object
//
CDialogChooser::~CDialogChooser()
{
	// Delete each of our dialogs
	for (int i = FIRST_CUSTOM_STEP; i <= LAST_CUSTOM_STEP; i++)
	{
		ASSERT(m_pDlgs[i] != NULL);
		delete m_pDlgs[i];
	}

}// CDialogChooser::CDialogChooser

///////////////////////////////////////////////////////////////////////////
// CDialogChooser::Next
//
// Returns the next step for the app wizard
//
CAppWizStepDlg* CDialogChooser::Next(CAppWizStepDlg* /*pDlg*/)
{
	ASSERT(0 <= m_nCurrDlg && m_nCurrDlg < LAST_DLG);
	ASSERT(pDlg == m_pDlgs[m_nCurrDlg]);

	m_nCurrDlg++;
	CString strValue;

	if (m_nCurrDlg == 4 &&
		(!TSPWizardaw.m_Dictionary.Lookup("OVERRIDE_LINE", strValue) ||
		 strValue == "No"))
		m_nCurrDlg++;

	if (m_nCurrDlg == 5 &&
		(!TSPWizardaw.m_Dictionary.Lookup("OVERRIDE_PHONE", strValue) ||
		 strValue == "No"))
		m_nCurrDlg++;

	return m_pDlgs[m_nCurrDlg];

}// CDialogChooser::Next

///////////////////////////////////////////////////////////////////////////
// CDialogChooser::Back
//
// Returns the previous step for the app wizard
//
CAppWizStepDlg* CDialogChooser::Back(CAppWizStepDlg* /*pDlg*/)
{
	ASSERT(1 <= m_nCurrDlg && m_nCurrDlg <= LAST_DLG);
	ASSERT(pDlg == m_pDlgs[m_nCurrDlg]);

	m_nCurrDlg--;
	CString strValue;

	if (m_nCurrDlg == 5 &&
		(!TSPWizardaw.m_Dictionary.Lookup("OVERRIDE_PHONE", strValue) ||
		 strValue == "No"))
		m_nCurrDlg--;

	if (m_nCurrDlg == 4 &&
		(!TSPWizardaw.m_Dictionary.Lookup("OVERRIDE_LINE", strValue) ||
		 strValue == "No"))
		m_nCurrDlg--;

	return m_pDlgs[m_nCurrDlg];

}// CDialogChooser::Back
