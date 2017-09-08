// TSPWizard.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TSPWizard.h"
#include "TSPWizardAw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTSPWizardApp

BEGIN_MESSAGE_MAP(CTSPWizardApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// The one and only CTSPWizardApp object
CTSPWizardApp theApp;

// CTSPWizardApp initialization
BOOL CTSPWizardApp::InitInstance()
{
	InitCommonControls();
	CWinApp::InitInstance();

	// Create our property sheet.
	TSPWizardaw.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
