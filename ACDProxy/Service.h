/******************************************************************************/
//                                                                        
// SERVICE.H - CService object definitions
//                                                                        
// Copyright (C) 1998-2002 JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
//                                                                        
// This file contains the declarations for our CService object
//
// This source code is intended only as a supplement to the
// TSP++ Class Library product documentation.  This source code cannot 
// be used in part or whole in any form outside the TSP++ library.
//                                                                        
/******************************************************************************/

#ifndef __SERVICE_INC__
#define __SERVICE_INC__

/*----------------------------------------------------------------------------
    EXTERNAL REFERENCES
----------------------------------------------------------------------------*/  
class CServiceModule;
extern CServiceModule _Module;
extern LPTSTR GetLastErrorText();

#include "acdproxy.h"

/**************************************************************************************
** CServiceModule
**
** Class to wrap Windows NT service capabilities
** 
**************************************************************************************/
class CServiceModule
{
// Class data
protected:
	SERVICE_STATUS_HANDLE m_hService;	// Handle to running service
	SERVICE_STATUS m_sStatus;			// Current status of the service
	HANDLE m_hevtStop;					// Stop event
	BOOL m_fIsService;					// Whether this is a true service or console app.
	TString m_strServiceName;			// Service name
	HINSTANCE m_hInstance;				// Instance handle for executable.
	DWORD m_dwThreadID;					// Thread id for the running service.
	CTapiInfo* m_ptiRunner;				// TAPI info object

// Constructor/Destructor
public:
	CServiceModule();

// Static methods for callback
public:
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	static void WINAPI _Handler(DWORD dwOpcode);
	static BOOL WINAPI _ConsoleHandler(DWORD dwCtrlType);

// Function methods
public:
	void SetExitCode(DWORD dwExitCode);
	void Init(HINSTANCE hInstance, UINT nServiceNameID);
	void Start(BOOL fIsService=TRUE);
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	void Handler(DWORD dwOpcode);
	void SetServiceStatus(DWORD dwState);
	void Run();
	BOOL ServiceStart();
	BOOL ServiceStop();
	BOOL IsInstalled();
	BOOL Install(LPCTSTR pszUserId, LPCTSTR pszPassword, BOOL fAutoStart);
	BOOL Uninstall();
	void LogEvent(WORD wEvent, LPCTSTR pFormat, ...);
	HINSTANCE GetResourceInstance() const;
	DWORD GetExitCode() const;
	BOOL IsService() const;

// Internal functions
protected:
	BOOL AddEventLogApp();
	void RemoveEventLogApp();
	BOOL IntRegDeleteKey (HKEY hDeleteKey, LPCTSTR pszMainDir);
};

/**************************************************************************************
** Procedure: CServiceModule::GetExitCode
**
** Arguments: void
**
** Returns: Final return code
**
** Description: This returns the exit code
**
**************************************************************************************/
inline DWORD CServiceModule::GetExitCode() const
{
	return m_sStatus.dwWin32ExitCode;

}// CServiceModule::GetExitCode

/**************************************************************************************
** Procedure: CServiceModule::SetExitCode
**
** Arguments: 'dwExitCode' - Exit code to report
**
** Returns: void
**
** Description: Sets the final exit code for the process.
**
**************************************************************************************/
inline void CServiceModule::SetExitCode(DWORD dwExitCode)
{
	m_sStatus.dwWin32ExitCode = dwExitCode;

}// CServiceModule::SetExitCode

/**************************************************************************************
** Procedure: CServiceModule::IsService
**
** Arguments: void
**
** Returns: TRUE/FALSE whether this is a service or not.
**
** Description: This returns the "service" flag
**
**************************************************************************************/
inline BOOL CServiceModule::IsService() const
{
	return m_fIsService;

}// CServiceModule::IsService

/**************************************************************************************
** Procedure: CServiceModule::GetResourceInstance
**
** Arguments: void
**
** Returns: HINSTANCE for executable image
**
** Description: This returns the hInstance global for this executable.
**
**************************************************************************************/
inline HINSTANCE CServiceModule::GetResourceInstance() const
{
	return m_hInstance;

}// CServiceModule::GetResourceInstance

/**************************************************************************************
** Procedure: CServiceModule::SetServiceStatus
**
** Arguments: 'dwState'
**
** Returns: void
**
** Description: Changes the state of the service and notifies the SCM.
**
**************************************************************************************/
inline void CServiceModule::SetServiceStatus(DWORD dwState)
{
	if (m_fIsService)
	{
		m_sStatus.dwCurrentState = dwState;
		::SetServiceStatus(m_hService, &m_sStatus);
	}

}// CServiceModule::SetServiceStatus

/**************************************************************************************
** Procedure: GetString
**
** Arguments: 'uiResID' - Resource id
**
** Returns: void
**
** Description: Load the string and return it.
**
**************************************************************************************/
inline TString GetString (UINT uiResID)
{
	TCHAR chBuff[_MAX_PATH];
	LoadString (_Module.GetResourceInstance(), uiResID, chBuff, sizeof(chBuff));
	return chBuff;

}// GetString

#endif // __SERVICE_INC__
