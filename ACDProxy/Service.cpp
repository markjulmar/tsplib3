/******************************************************************************/
//                                                                        
// SERVICE.CPP - Source code for the NT service portion of an ACD proxy.
//                                                                        
// Copyright (C) 1998-2002 JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
//                                                                        
// This file contains the main service file for the ACD proxy.
//
// This source code is intended only as a supplement to the
// TSP++ Class Library product documentation.  This source code cannot 
// be used in part or whole in any form outside the TSP++ library.
//                                                                        
/******************************************************************************/

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "service.h"
#include "acdmsg.h"

/*----------------------------------------------------------------------------
    GLOBAL VARIABLES
----------------------------------------------------------------------------*/  
CServiceModule _Module;				// Main service object
BOOL g_fQuiet = FALSE;				// TRUE - no messages
LPCTSTR g_pszEventLog = _T("System\\CurrentControlSet\\Services\\EventLog\\Application");

/**************************************************************************************
** Procedure: DoMessageBox
**
** Arguments: 'uiMsg' - Resource ID of string to show
**
** Returns: void
**
** Description: Displays an error.
**
**************************************************************************************/
void DoMessageBox (UINT uiResID)
{
	if (!g_fQuiet)
		_putts(GetString(uiResID).c_str());

}// DoMessageBox

/**************************************************************************************
** Procedure: DoMessageBoxE
**
** Arguments: 'uiMsg' - Resource ID of string to show
**
** Returns: void
**
** Description: Displays an error.
**
**************************************************************************************/
void DoMessageBoxE (UINT uiResID)
{
	if (g_fQuiet)
		return;

	DWORD dwErr = GetLastError();
	if (dwErr > 0)
	{
		TString strErr = GetLastErrorText();
		TString strMsg = GetString(uiResID);

		TCHAR chBuff[255];
		wsprintf(chBuff, strMsg.c_str(), dwErr, strErr.c_str());
		_putts(chBuff);
	}
	else
		DoMessageBox(uiResID);

}// DoMessageBox

/**************************************************************************************
** Procedure: main
**
** Arguments: 'argc' - Arguments
**            'argv' - Arguments
**
** Returns: void
**
** Description: This function is the main entrypoint for the module.
**
**************************************************************************************/
void _cdecl main(int /*argc*/, char ** /*argv*/, char ** /*envp*/)
{
	OSVERSIONINFO verInfo = {0};     

	// Get the full UNICODE command line
	TCHAR szTokens[] = _T("-/");
	TCHAR szUserId[50], szPassword[50];
	BOOL fAutoStart = FALSE;
	BOOL fIsService = TRUE;
	TString strCmdLine = GetCommandLine();
	TString strCmdLineU = GetCommandLine();
	CharUpperBuff(&strCmdLineU[0], static_cast<DWORD>(strCmdLineU.length()));

	// Initialize the service object information
	_Module.Init(GetModuleHandle(NULL), IDS_SERVICENAME);
	ZeroMemory(szUserId, sizeof(szUserId));
	ZeroMemory(szPassword, sizeof(szPassword));

	// See if we should auto-start.
	fAutoStart = (strCmdLineU.find(_T("AUTOSTART")) != TString::npos);

	// Quiet?
	if (strCmdLineU.find(_T("QUIET")) != TString::npos)
		g_fQuiet = TRUE;
	else
	{
		puts("\r\nJTAProxy (C) 1999-2002 JulMar Technology, Inc.");
		puts("All rights reserved.\r\n\r\n");
	}

	// Start as a console application
	if (strCmdLineU.find(_T("CONSOLE")) != TString::npos)
		fIsService = FALSE;

	// See if we have a password/userid
	TString::size_type iPos = strCmdLineU.find(_T("USER="));
	if (iPos != TString::npos)
	{
		iPos+=5;
		TString::size_type iEnd = strCmdLine.substr(iPos).find(_T(" "));
		if (iEnd == TString::npos)
			iEnd = (strCmdLine.length()+1) - iPos;
		lstrcpyn(szUserId, strCmdLine.substr(iPos).c_str(), static_cast<int>(iEnd));
	}

	// See if we have a password
	iPos = strCmdLineU.find(_T("PASSWORD="));
	if (iPos != TString::npos)
	{
		iPos += 9;
		TString::size_type iEnd = strCmdLine.substr(iPos).find(_T(" "));
		if (iEnd == TString::npos)
			iEnd = (strCmdLine.length()+1) - iPos;
		lstrcpyn(szPassword, strCmdLine.substr(iPos).c_str(), static_cast<int>(iEnd));
	}

	// If we have a userid but no password or vice-versa, give an error.
	if (szPassword[0] != '\0' && szUserId[0] == '\0')
	{
		DoMessageBox(IDS_INVALUSERPASS);
		_Module.SetExitCode(ERROR_INVALID_PARAMETER);
		goto myExit;
	}

	// If this is Windows 95 or Windows 98 then give an error message
	// and exit since we don't support that configuration.
	verInfo.dwOSVersionInfoSize = sizeof(verInfo);     
	GetVersionEx(&verInfo);      
	if (verInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
	{
		if (!g_fQuiet)
			puts("This program requires Windows NT/2K/XP.\r\n");
		_Module.SetExitCode(ERROR_INVALID_ORDINAL);
		goto myExit;
	}

	// Process our command line.
	iPos = strCmdLine.find_first_of(szTokens);
	while (iPos != TString::npos)
	{
		strCmdLine = strCmdLine.substr(iPos+1);

		// Quiet?
		if (!_tcsnicmp(strCmdLine.c_str(), _T("quiet"), 1))
			;

		// Unregister server
		else if (!_tcsnicmp (strCmdLine.c_str(), _T("unregister"), 10))
		{
			if (_Module.Uninstall())
				DoMessageBox(IDS_UNREGISTER);
			else
				DoMessageBoxE(IDS_ERR_UNREGISTER);
			goto myExit;
		}

  		// Register as Service
		else if (!_tcsnicmp(strCmdLine.c_str(), _T("register"), 8))
		{
			if (_Module.Install(szUserId, szPassword, fAutoStart))
				DoMessageBox(IDS_REGISTER);
			else
				DoMessageBoxE(IDS_ERR_REGISTER);
			goto myExit;
		}

		else if (!_tcsnicmp(strCmdLine.c_str(), _T("start"), 5))
		{
			if (!_Module.ServiceStart())
				DoMessageBoxE(IDS_ERR_START);
			goto myExit;
		}

		else if (!_tcsnicmp(strCmdLine.c_str(), _T("stop"), 4))
		{
			if (!_Module.ServiceStop())
				DoMessageBoxE(IDS_ERR_STOP);
			goto myExit;
		}

		// Start as a console application
		else if (!_tcsnicmp(strCmdLine.c_str(), _T("console"), 7))
			;

		// Help
		else
		{
			if (g_fQuiet)
				_Module.SetExitCode(ERROR_INVALID_PARAMETER);
			else
			{
				puts("This program is a TAPI agent proxy for installed TAPI "\
					 "service providers on this machine.\r\n");
				puts("Valid options are:");
				puts("   register     Registers the proxy as a NT service.");
				puts("                In addition to this, you can specify a");
				puts("                userid and password to run the service under");
				puts("                by using the parameters:");
				puts("                If user/password is supplied, it should match the");
				puts("                user/password that TAPI server is running under.");
				puts("       user=domain\\userid  (use \".\\userid\" for local domain");
				puts("       password=user's password");
				puts("\r\n");
				puts("   autostart    Causes the server to automatically start with NT.");
				puts("   unregister   Un-registers the proxy from the NT service table.");
				puts("   quiet        Does not output errors to the console.");
				puts("   console      Runs the proxy as a console application.");
				puts("   start        Starts the service running.");
				puts("   stop         Stops the running service.");
			}
			goto myExit;
		}
		iPos = strCmdLine.find_first_of(szTokens);
	}

	// If we are still ok, then start the service
	if (_Module.GetExitCode() == 0)
	{
		// Start the service/server running
		_Module.Start(fIsService);
	}

myExit:
	// Shutdown the process.
	ExitProcess(_Module.GetExitCode());
	
}// main

/**************************************************************************************
** Procedure: GetLastErrorText
**
** Arguments: void
**
** Returns: Pointer to string buffer
**
** Description: Returns the text for the last system error
**
**************************************************************************************/
LPTSTR GetLastErrorText()
{
	#define MAX_MSG_SIZE 256
    static TCHAR szMsgBuf[MAX_MSG_SIZE];
    DWORD dwError, dwRes;

    dwError = GetLastError ();

    dwRes = FormatMessage (
				FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                dwError,
                MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US),
				szMsgBuf,
				MAX_MSG_SIZE,
				NULL);
    if (0 == dwRes) {
		szMsgBuf[0] = _T('\0');
    }
    return szMsgBuf;

}// GetLastErrorText

/**************************************************************************************
** Procedure: CServiceModule::CServiceModule
**
** Arguments: void
**
** Returns: void
**
** Description: Constructor for the service module object
**
**************************************************************************************/
CServiceModule::CServiceModule() :
	m_hService(NULL), m_fIsService(TRUE), m_ptiRunner(NULL)
{
}// CServiceModule::CServiceModule

/**************************************************************************************
** Procedure: CServiceModule::Init
**
** Arguments: 'hInstance' - HINSTANCE of this EXE
**            'nServiceNameID' - Service name identifier
**
** Returns: void
**
** Description: Initialize the ATL library
**
**************************************************************************************/
void CServiceModule::Init(HINSTANCE hInstance, UINT nServiceNameID)
{
	// Save off the hInstance.
	m_hInstance = hInstance;

	// Load the service name from the resource table.
	m_strServiceName = GetString(nServiceNameID);

    // Set up the initial service status 
    m_sStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_sStatus.dwCurrentState = SERVICE_STOPPED;
    m_sStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_sStatus.dwWin32ExitCode = 0;
    m_sStatus.dwCheckPoint = 0;
    m_sStatus.dwWaitHint = 0;
    m_sStatus.dwServiceSpecificExitCode = 0;

	// Create our "stop" event.
	m_hevtStop = CreateEvent(NULL, TRUE, FALSE, NULL);

}// CServiceModule::Init

/**************************************************************************************
** Procedure: CServiceModule::ServiceStart
**
** Arguments: void
**
** Returns: void
**
** Description: Asks the service manager to start the service.
**
**************************************************************************************/
BOOL CServiceModule::ServiceStart()
{
	// Mark us as NOT a service.
	m_fIsService = FALSE;

	// Ask the SCM to start us.
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
	{
		_Module.SetExitCode(GetLastError());
        return FALSE;
	}

    SC_HANDLE hService = ::OpenService(hSCM, GetString(IDS_SHORTNAME).c_str(), SERVICE_ALL_ACCESS);
    if (hService == NULL)
	{
		_Module.SetExitCode(GetLastError());
		::CloseServiceHandle(hSCM);
		return FALSE;
    }

	// Try to start the service
	if (!g_fQuiet)
		_putts(_T("Starting JTAProxy..."));

	BOOL fStarted = (StartService(hService, 0, NULL));
	if (fStarted)
	{
		if (!g_fQuiet)
			_putts(_T("Waiting for service to start..."));

		Sleep(1000);
		while (QueryServiceStatus(hService, &m_sStatus))
		{
			if (m_sStatus.dwCurrentState != SERVICE_START_PENDING)
				break;
		}

		if (m_sStatus.dwCurrentState == SERVICE_RUNNING)
		{
			if (!g_fQuiet)
				_putts(_T("Service started."));
		}
		else
			fStarted = FALSE;
	}
	else
		_Module.SetExitCode(GetLastError());

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

	return fStarted;
		
}// CServiceModule::ServiceStart

/**************************************************************************************
** Procedure: CServiceModule::ServiceStop
**
** Arguments: void
**
** Returns: void
**
** Description: Asks the service manager to start the service.
**
**************************************************************************************/
BOOL CServiceModule::ServiceStop()
{
	// Mark us as NOT a service.
	m_fIsService = FALSE;

	// Ask the SCM to start us.
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
	{
		_Module.SetExitCode(GetLastError());
        return FALSE;
	}

    SC_HANDLE hService = ::OpenService(hSCM, GetString(IDS_SHORTNAME).c_str(), SERVICE_ALL_ACCESS);
    if (hService == NULL)
	{
		_Module.SetExitCode(GetLastError());
		::CloseServiceHandle(hSCM);
		return FALSE;
    }

	// Try to stop the service
	if (!g_fQuiet)
		_putts(_T("Stopping JTAProxy..."));

	BOOL fStarted = ControlService(hService, SERVICE_CONTROL_STOP, &m_sStatus);
	if (fStarted)
	{
		if (!g_fQuiet)
			_putts(_T("Waiting for service to stop..."));
		Sleep(1000);
		while (QueryServiceStatus(hService, &m_sStatus))
		{
			if (m_sStatus.dwCurrentState != SERVICE_STOP_PENDING)
				break;
		}

		if (m_sStatus.dwCurrentState == SERVICE_STOPPED)
		{
			if (!g_fQuiet)
				_putts(_T("Service stopped."));
		}
		else
			fStarted = FALSE;
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_SERVICE_NOT_ACTIVE)
		{
			if (!g_fQuiet)
				_putts(_T("The JTAProxy service is not currently running."));
		}
		else
			_Module.SetExitCode(dwErrorCode);
	}

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

	return fStarted;
		
}// CServiceModule::ServiceStop

/**************************************************************************************
** Procedure: CServiceModule::Start
**
** Arguments: void
**
** Returns: void
**
** Description: Starts the service
**
**************************************************************************************/
void CServiceModule::Start(BOOL fIsService)
{
    SERVICE_TABLE_ENTRY st[] = {
      { (LPTSTR) m_strServiceName.c_str(), _ServiceMain },
      { NULL, NULL } };

	// Save off our "service" status
	m_fIsService = fIsService;

	if (!m_fIsService)
	{
		LogEvent(0, _T("JTAProxy (C) 1998 JulMar Entertainment Technology, Inc."));
		LogEvent(0, _T("All rights reserved."));
	}

	// Add our service to the running SCM list.
	if (m_fIsService && !::StartServiceCtrlDispatcher(st))
	{
		_Module.SetExitCode(GetLastError());
		LogEvent(EVENTLOG_ERROR_TYPE, _T("Service failed to start 0x%lx\r\n%s"), GetLastError(), GetLastErrorText());
		return;
	}

	// If we are simply an EXE, then run.
	else if (m_fIsService == FALSE)
	{
	    SetConsoleCtrlHandler(_ConsoleHandler, TRUE);
		Run();
	}

}// CServiceModule::Start

/**************************************************************************************
** Procedure: CServiceModule::_ServiceMain
**
** Arguments: arguments
**
** Returns: void
**
** Description: Service manager processor loop.
**
**************************************************************************************/
void WINAPI CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	_Module.ServiceMain(dwArgc, lpszArgv);

}// CServiceModule::_ServiceMain

/**************************************************************************************
** Procedure: CServiceModule::_Handler
**
** Arguments: 'dwOpCode'
**
** Returns: void
**
** Description: Static entrypoint for SCM notifications
**
**************************************************************************************/
void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
	_Module.Handler(dwOpcode); 

}// CServiceModule::_Handler

/**************************************************************************************
** Procedure: CServiceModule::_ConsoleHandler
**
** Arguments: 'dwCtrlType' - What type of console event occurred
**
** Returns: TRUE/FALSE
**
** Description: Static entrypoint for keyboard interrupts during debug.
**
**************************************************************************************/
BOOL WINAPI CServiceModule::_ConsoleHandler(DWORD dwCtrlType)
{
    switch( dwCtrlType )
    {
		case CTRL_SHUTDOWN_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
		case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
			_Module.LogEvent(0, _T("Stopping console application."));
            _Module.Handler(SERVICE_CONTROL_STOP);
            return TRUE;
            break;
    }
    return FALSE;

}// CServiceModule::_ConsoleHandler

/**************************************************************************************
** Procedure: CServiceModule::ServiceMain
**
** Arguments: 'dwArgc' - Count of arguments from SCM
**            'lpszArgv' - List of args.
**
** Returns: void
**
** Description: Main processing code
**
**************************************************************************************/
void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_sStatus.dwCurrentState = SERVICE_START_PENDING;
    m_hService = RegisterServiceCtrlHandler(m_strServiceName.c_str(), _Handler);
    if (m_hService == NULL)
	{
        LogEvent(EVENTLOG_ERROR_TYPE, _T("Control Handler not installed"));
        return;
    }

	// Mark us as "pending" start.
    SetServiceStatus(SERVICE_START_PENDING);
    m_sStatus.dwWin32ExitCode = S_OK;
    m_sStatus.dwCheckPoint = 0;
    m_sStatus.dwWaitHint = 0;

    // When the Run function returns, the service has stopped.
    Run();

    SetServiceStatus(SERVICE_STOPPED);

}// CServiceMain::ServiceMain

/**************************************************************************************
** Procedure: CServiceModule::Handler
**
** Arguments: 'dwOpCode'
**
** Returns: void
**
** Description: Processes asynch commands from the SCM.
**
**************************************************************************************/
void CServiceModule::Handler(DWORD dwOpcode)
{
	// Switch on the handler code
	switch (dwOpcode)
	{
		case SERVICE_CONTROL_STOP:
			SetServiceStatus(SERVICE_STOP_PENDING);
			SetEvent(m_hevtStop);
			break;
		case SERVICE_CONTROL_PAUSE:
		case SERVICE_CONTROL_CONTINUE:
		case SERVICE_CONTROL_INTERROGATE:
		case SERVICE_CONTROL_SHUTDOWN:
			break;
		default:
			LogEvent(EVENTLOG_ERROR_TYPE, _T("Bad service request: 0x%lx"), dwOpcode);
			break;
	}

}// CServiceModule::Handler

/**************************************************************************************
** Procedure: CServiceModule::Run
**
** Arguments: void
**
** Returns: void
**
** Description: Main run loop for the service.
**
**************************************************************************************/
void CServiceModule::Run()
{
	// Set our thread ID
	_Module.m_dwThreadID = GetCurrentThreadId();

	LogEvent(EVENTLOG_INFORMATION_TYPE, _T("JulMar Agent PROXY starting..."));

	// Mark the service as running.
	if (m_fIsService)
		SetServiceStatus(SERVICE_RUNNING);

	m_ptiRunner = new CTapiInfo;
	if (m_ptiRunner == NULL)
	{
		LogEvent(EVENTLOG_ERROR_TYPE, 
				_T("Error creating TAPI information object, rc=0x%lx\r\n%s"),
				GetLastError(), GetLastErrorText());
	}
	else
	{
		// Initialize our TAPI informational object.
		if (m_ptiRunner->Initialize(GetResourceInstance(), m_strServiceName.c_str()))
		{
			// Pass control to the ACD agent proxy runner.  It will return
			// when the give event is set.
			m_ptiRunner->Run(m_hevtStop);
		}

		// Delete the TAPI object - this will shutdown the thread.
		delete m_ptiRunner;
	}

	// By returning, we will exit the service.
	LogEvent(EVENTLOG_INFORMATION_TYPE, _T("JulMar Agent PROXY shutting down..."));

}// CServiceModule::Run

/**************************************************************************************
** Procedure: CServiceModule::IsInstalled
**
** Arguments: void
**
** Returns: TRUE/FALSE whether service is installed in the SCM.
**
** Description: This checks with the NT service control manager to determine if the
**              service is currently installed.
**
**************************************************************************************/
BOOL CServiceModule::IsInstalled()
{
    BOOL bResult = FALSE;
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM != NULL)
	{
		// Get the shortname for the service.
        SC_HANDLE hService = ::OpenService(hSCM, GetString(IDS_SHORTNAME).c_str(), SERVICE_QUERY_CONFIG);
        if (hService != NULL)
		{
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;

}// CServiceModule::IsInstalled

/**************************************************************************************
** Procedure: CServiceModule::Install
**
** Arguments: void
**
** Returns: TRUE/FALSE whether service is installed in the SCM.
**
** Description: This installs the service into the SCM.
**
**************************************************************************************/
BOOL CServiceModule::Install(LPCTSTR pszUserId, LPCTSTR pszPassword, BOOL fAutoStart)
{
	// Mark us as NOT a service.
	m_fIsService = FALSE;

	// If the service is alread installed, then we are done.
	if (IsInstalled())
		return TRUE;

	// Make sure it's all or nothing.
	if (pszUserId == NULL || pszPassword == NULL ||
		*pszUserId == '\0' || *pszPassword == '\0')
	{
		pszUserId = NULL;
		pszPassword = NULL;
	}

	// Otherwise, ask the SCM to install us.
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
	{
		_Module.SetExitCode(GetLastError());
        return FALSE;
	}

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    SC_HANDLE hService = ::CreateService(
			hSCM, 
			GetString(IDS_SHORTNAME).c_str(), 
			m_strServiceName.c_str(),
			SERVICE_ALL_ACCESS, 
			SERVICE_WIN32_OWN_PROCESS,
			(fAutoStart) ? 
			SERVICE_AUTO_START :
			SERVICE_DEMAND_START, 
			SERVICE_ERROR_NORMAL,
			szFilePath, 
			NULL,				// Load ordering group
			NULL,				// Tag identifier
			_T("TAPISRV\0\0"),	// Requires TAPISRV be running
			pszUserId,			// Local account
			pszPassword);		// Password	

    if (hService == NULL)
	{
		_Module.SetExitCode(GetLastError());
		::CloseServiceHandle(hSCM);
		return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

	// Add keys to the registry allowing us to write events into the logfile.
	if (!AddEventLogApp())
	{
		LogEvent(0, _T("Error adding event log information, rc=0x%lx\r\n%s"), 
				GetLastError(), GetLastErrorText());
	}

	return TRUE;

}// CServiceModule::Install

/**************************************************************************************
** Procedure: CServiceModule::Uninstall
**
** Arguments: void
**
** Returns: TRUE/FALSE whether service is uninstalled from the SCM.
**
** Description: This removes the service from the SCM list.
**
**************************************************************************************/
BOOL CServiceModule::Uninstall()
{
	// Mark us as NOT a service.
	m_fIsService = FALSE;

	// If it is not currently installed, then we are done.
	if (!IsInstalled())
		return TRUE;

	// Open the service and ask it to shutdown if it is running.
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		_Module.SetExitCode(GetLastError());
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, GetString(IDS_SHORTNAME).c_str(), SERVICE_STOP | DELETE);
	if (hService == NULL)
	{
		_Module.SetExitCode(GetLastError());
		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	// Ask the service to stop running.
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	// Now delete the service.
	BOOL bDelete = ::DeleteService(hService);
	if (!bDelete)
		_Module.SetExitCode(GetLastError());

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	// Remove our keys from the registry
	RemoveEventLogApp();

	return (bDelete);

}// CServiceModule::Uninstall

/**************************************************************************************
** Procedure: CServiceModule::AddEventLogApp
**
** Arguments: void
**
** Returns: void
**
** Description: Writes the key into the registry to allow this application
**              to log entries into the system event log.
**
**************************************************************************************/
BOOL CServiceModule::AddEventLogApp()
{
    DWORD dwDisposition;
	DWORD dwValue = EVENTLOG_ERROR_TYPE | EVENTLOG_INFORMATION_TYPE | EVENTLOG_WARNING_TYPE;
	TString strModule = GetString(IDS_SHORTNAME);

    // Open the APPLICATION key for services.
    HKEY hKeyEvents;
    if (RegCreateKeyEx (HKEY_LOCAL_MACHINE, g_pszEventLog, 0, _T(""), REG_OPTION_NON_VOLATILE,
                        KEY_ALL_ACCESS, NULL, &hKeyEvents, &dwDisposition) != ERROR_SUCCESS)
        return FALSE;

    // Now create our application entry
    HKEY hKeyApplication;
    if (RegCreateKeyEx (hKeyEvents, strModule.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, 
						KEY_ALL_ACCESS, NULL, &hKeyApplication, &dwDisposition) != ERROR_SUCCESS)
	{
		RegCloseKey (hKeyEvents);
        return FALSE;
	}

	// Get the full path to our executable.
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);
	
	// Now write the entries into the key.
	RegSetValueEx (hKeyApplication, _T("EventMessageFile"), 0, REG_SZ, (LPBYTE)szFilePath, (lstrlen(szFilePath)+1) * sizeof(TCHAR));
	RegSetValueEx (hKeyApplication, _T("TypesSupported"), 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));

	// Close the registry keys
	RegCloseKey (hKeyApplication);
	RegCloseKey (hKeyEvents);

	return TRUE;

}// CServiceModule::AddEventLogApp

/**************************************************************************************
** Procedure: CServiceModule::RemoveEventLogApp
**
** Arguments: void
**
** Returns: void
**
** Description: Removes all the keys from the registry related to our event log.
**
**************************************************************************************/
void CServiceModule::RemoveEventLogApp()
{
    // Open the APPLICATION key for services.
    HKEY hKeyEvents;
    if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, g_pszEventLog, 0, KEY_ALL_ACCESS, &hKeyEvents) == ERROR_SUCCESS)
	{
		// Walk through all entries in this and delete them.
		TString strModule = GetString(IDS_SHORTNAME);
		IntRegDeleteKey (hKeyEvents, strModule.c_str());
		RegCloseKey (hKeyEvents);
	}

}// CServiceModule::RemoveEventLogApp

/**************************************************************************************
** Procedure: CServiceModule::IntRegDeleteKey
**
** Arguments: 'hDeleteKey' - Owner key
**            'pszMainDir' - Directory to delete
**
** Returns: TRUE/FALSE success code
**
** Description: Removes all the keys from the registry related to our event log.
**
**************************************************************************************/
BOOL CServiceModule::IntRegDeleteKey (HKEY hDeleteKey, LPCTSTR pszMainDir)
{
	// Attempt to delete the key directly.  Under Win95, this will also delete
	// any branches under it.
    if (RegDeleteKey (hDeleteKey, pszMainDir) != ERROR_SUCCESS)
	{
		// Open the top-level key.
		HKEY hKey;
		DWORD dwRC = RegOpenKeyEx(hDeleteKey, pszMainDir, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey);
		if (dwRC == ERROR_SUCCESS)
		{
			DWORD dwReqSize = 1024;
			LPTSTR pszName = (LPTSTR) malloc (dwReqSize+1);
			if (pszName == NULL)
			{
				RegCloseKey (hKey);
				return FALSE;
			}
		
			do
			{
				dwRC = RegEnumKeyEx(hKey, 0, pszName, &dwReqSize, NULL, NULL, NULL, NULL);
				if (dwRC == ERROR_NO_MORE_ITEMS)
				{
				   dwRC = RegDeleteKey(hDeleteKey, pszMainDir);
				   break;
				}
				else if (dwRC == ERROR_SUCCESS)
				   IntRegDeleteKey (hKey, pszName);
			}
			while (dwRC == ERROR_SUCCESS);
			
			RegCloseKey(hKey);
			free (pszName);
		}
	}
	return TRUE;

}// CServiceModule::IntRegDeleteKey

/**************************************************************************************
** Procedure: CServiceModule::LogEvent
**
** Arguments: 'pFormat' - Variable format list
**
** Returns: void
**
** Description: Writes a line to the event log for this service.
**
**************************************************************************************/
void CServiceModule::LogEvent(WORD wType, LPCTSTR pFormat, ...)
{
    TCHAR chMsg[256];
    HANDLE hEventSource;
    LPTSTR lpszStrings[2];
	TString strModule = GetString(IDS_SHORTNAME);

	va_list	pArg;
	va_start(pArg, pFormat);
	wvsprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = (LPTSTR) strModule.c_str();
    lpszStrings[1] = chMsg;

	if (m_fIsService)
	{
	    // Get a handle to use with ReportEvent.
		hEventSource = RegisterEventSource(NULL, strModule.c_str());
	    if (hEventSource != NULL)
	    {
	        // Write to event log.
            ReportEvent(hEventSource, // handle of event source
                wType,				  // event type
                0,                    // event category
                MSG_PROXY_ERR,        // event ID
                NULL,                 // current user's SID
                2,                    // strings in lpszStrings
                0,                    // no bytes of raw data
                (LPCTSTR*)lpszStrings,// array of error strings
                NULL);                // no raw data
	        DeregisterEventSource(hEventSource);
	    }
	}
	else if (!g_fQuiet)
		_putts(chMsg);

}// CServiceModule::LogEvent
