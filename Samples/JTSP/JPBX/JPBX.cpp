/*******************************************************************/
//
// JPBX.CPP
//
// JulMar PBX/ACD simulator program application shell
//
// Copyright (C) 1998 JulMar Technology, Inc.
// All rights reserved
//
// TSP++ Version 3.00 PBX/ACD Emulator Projects
// Internal Source Code - Do Not Release
//
// Modification History
// --------------------
// 1998/09/05 MCS@JulMar	Initial revision
//
/*******************************************************************/

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "JPBX.h"
#include "MainFrm.h"
#include "document.h"
#include "viewtree.h"
#include "viewstats.h"
#include "viewlist.h"
#include "about.h"
#include "winplace.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// CJPBXApp message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CJPBXApp, CWinApp)
	//{{AFX_MSG_MAP(CJPBXApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------*/
// GLOBAL VARIABLES
/*---------------------------------------------------------------*/
CJPBXApp theApp;

/*****************************************************************************
** Procedure:  CJPBXApp::CJPBXApp
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Constructor for the UI application dll
**
*****************************************************************************/
CJPBXApp::CJPBXApp()
{
}// CJPBXApp::CJPBXApp

/*****************************************************************************
** Procedure:  CJPBXApp::InitInstance
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Initial entry point for our application
**
*****************************************************************************/
BOOL CJPBXApp::InitInstance()
{
	// Initialize the WinSock layer
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Enable common controls.
	InitCommonControls();

	// Set our registry key section
	SetRegistryKey(_T("JulMar Technology\\JPBX3"));

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CJTDocument),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CJTView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Restore our proper position.
	CWindowPlacement wp;
	wp.RestorePosition(m_pMainWnd, _T("Position"));

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;

}// CJPBXApp::InitInstance

/*****************************************************************************
** Procedure:  CJPBXApp::OnAppAbout
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Runs the about box dialog
**
*****************************************************************************/
void CJPBXApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

}// CJPBXApp::OnAppAbout

/*****************************************************************************
** Procedure:  CJPBXApp::OnIdle
**
** Arguments:  'lCount' - Type of idle operation to work on
**
** Returns:    Boolean result
**
** Description:  Runs the idle operations for the PBX
**
*****************************************************************************/
BOOL CJPBXApp::OnIdle(LONG lCount) 
{
	static DWORD g_dwLastTime = 0;
	CWinApp::OnIdle(lCount);

	// Check last time we sent idle timer
	if (GetTickCount() > g_dwLastTime + 1000)
	{
		CJTDocument* pDoc = CUIObject::GetDocument();
		if (pDoc != NULL)
			pDoc->OnTimer();
		g_dwLastTime = GetTickCount();
	}

	return TRUE;

}// CJPBXApp::OnIdle
