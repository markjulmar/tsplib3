/*****************************************************************************/
//
// TAPIFUN.CPP - Source code for TAPI portion of the JTAProxy application
//                                                                        
// Copyright (C) 1998-2002 JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
//                                                                        
// This file contains the TAPI initialization functions for the JTAProxy.
//
// This source code is intended only as a supplement to the
// TSP++ Class Library product documentation.  This source code cannot 
// be used in part or whole in any form outside the TSP++ library.
// 
/*****************************************************************************/

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "service.h"
#include <stdio.h>
#include <iostream>
#include <tspi.h>

/*----------------------------------------------------------------------------
	CONSTANTS AND GLOBALS
-----------------------------------------------------------------------------*/
#define ISTR(s,p) ((LPTSTR)((LPBYTE)s + s->p))

static struct  
{
	DWORD dwAgentBit;
	DWORD dwAgentFeature;
} gAgentFeatures[] = 
{
	{ AGENTFEATURE_SETAGENTGROUP, LINEPROXYREQUEST_SETAGENTGROUP  },
	{ AGENTFEATURE_SETAGENTSTATE, LINEPROXYREQUEST_SETAGENTSTATE },				
	{ AGENTFEATURE_SETAGENTACTIVITY, LINEPROXYREQUEST_SETAGENTACTIVITY },			
	{ AGENTFEATURE_GETAGENTCAPS, LINEPROXYREQUEST_GETAGENTCAPS },				
	{ AGENTFEATURE_GETAGENTSTATUS, LINEPROXYREQUEST_GETAGENTSTATUS },				
	{ AGENTFEATURE_AGENTSPECIFIC, LINEPROXYREQUEST_AGENTSPECIFIC },				
	{ AGENTFEATURE_GETAGENTACTIVITYLIST, LINEPROXYREQUEST_GETAGENTACTIVITYLIST },		
	{ AGENTFEATURE_GETAGENTGROUPLIST, LINEPROXYREQUEST_GETAGENTGROUPLIST },			
	{ AGENTFEATURE_CREATEAGENT, LINEPROXYREQUEST_CREATEAGENT },				
	{ AGENTFEATURE_SETAGENTMEASUREMENTPERIOD, LINEPROXYREQUEST_SETAGENTMEASUREMENTPERIOD },	
	{ AGENTFEATURE_GETAGENTINFO, LINEPROXYREQUEST_GETAGENTINFO },				
	{ AGENTFEATURE_CREATEAGENTSESSION, LINEPROXYREQUEST_CREATEAGENTSESSION },			
	{ AGENTFEATURE_GETAGENTSESSIONLIST, LINEPROXYREQUEST_GETAGENTSESSIONLIST },		
	{ AGENTFEATURE_SETAGENTSESSIONSTATE, LINEPROXYREQUEST_SETAGENTSESSIONSTATE },		
	{ AGENTFEATURE_GETAGENTSESSIONINFO, LINEPROXYREQUEST_GETAGENTSESSIONINFO },		
	{ AGENTFEATURE_GETQUEUELIST, LINEPROXYREQUEST_GETQUEUELIST },				
	{ AGENTFEATURE_SETQUEUEMEASUREMENTPERIOD, LINEPROXYREQUEST_SETQUEUEMEASUREMENTPERIOD },	
	{ AGENTFEATURE_GETQUEUEINFO, LINEPROXYREQUEST_GETQUEUEINFO },				
	{ AGENTFEATURE_GETGROUPLIST, LINEPROXYREQUEST_GETGROUPLIST },				
	{ AGENTFEATURE_SETAGENTSTATEEX, LINEPROXYREQUEST_SETAGENTSTATEEX }			
};

static LPCTSTR g_pszFunctions[] = {
	// TAPI 2.1
	{ _T("SetAgentGroup") },					// 1
	{ _T("SetAgentState") },					// 2
	{ _T("SetAgentActivity") },					// 3
	{ _T("GetAgentCaps") },						// 4
	{ _T("GetAgentStatus") },					// 5
	{ _T("AgentSpecific") },					// 6
	{ _T("GetAgentActivityList") },				// 7
	{ _T("GetAgentGroupList") },				// 8
	// TAPI 2.2
	{ _T("CreateAgent") },						// 9
	{ _T("SetAgentMeasurementPeriod") },		// 10
	{ _T("GetAgentInfo") },						// 11
	{ _T("CreateAgentSession") },				// 12
	{ _T("GetAgentSessionList") },				// 13
	{ _T("SetAgentSessionState") },				// 14
	{ _T("GetAgentSessionInfo") },				// 15
	{ _T("GetQueueList") },						// 16
	{ _T("SetQueueMeasurementPeriod") },		// 17
	{ _T("GetQueueInfo") },						// 18
	{ _T("GetGroupList") },						// 19
	{ _T("SetAgentStateEx") },					// 20
};

/*****************************************************************************
** Procedure:  _CompletionThread
**
** Arguments:  'lParam' - Thread parameter
**
** Returns:    TRUE/FALSE return code
**
** Description:  This function initializes TAPI for the program.
**
*****************************************************************************/
DWORD WINAPI _CompletionThread(LPVOID lpParam)
{
	CTapiInfo* pInfo = (CTapiInfo*) lpParam;
	pInfo->PullAgentRequests();
	return 0;

}// _CompletionThread

/*****************************************************************************
** Procedure:  CTapiInfo::~CTapiInfo
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Destructor for our TAPI object
**
*****************************************************************************/
CTapiInfo::~CTapiInfo()
{
	if (m_hIOCPort != NULL)
		PostQueuedCompletionStatus(m_hIOCPort, 0, 0, NULL);

	// Wait for the thread to end
	if (WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
		TerminateThread(m_hThread, 0);
	CloseHandle(m_hThread);

	if (m_hLineApp != NULL)
	{
		if (!_Module.IsService())
			_Module.LogEvent(0, _T("Calling lineShutdown..."));
		lineShutdown(m_hLineApp);
	}

	if (m_hIOCPort != NULL)
		CloseHandle (m_hIOCPort);

}// CTapiInfo::~CTapiInfo

/*****************************************************************************
** Procedure:  CTapiInfo::Initialize
**
** Arguments:  'hInstance' - Instance handle for application
**
** Returns:    TRUE/FALSE return code
**
** Description:  This function initializes TAPI for the program.
**
*****************************************************************************/
BOOL CTapiInfo::Initialize (HINSTANCE hInstance, LPCTSTR pszAppName)
{
	// Create our I/O completion port for receiving TAPI notifications.
	m_hIOCPort = CreateIoCompletionPort (INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hIOCPort == NULL)
	{
		_Module.LogEvent(EVENTLOG_ERROR_TYPE, 
				_T("CreateIoCompletionPort failed, rc=0x%lx\r\n%s"), 
				GetLastError(), GetLastErrorText());
		return FALSE;
	}

	// Create our shared-memory informational block.
	if (!m_smMgr.Init(ACDPROXY_INPUT, MAX_ACD_MAX_SIZE, ACDPROXY_OUTPUT, MAX_ACD_MAX_SIZE))
	{
		_Module.LogEvent(EVENTLOG_ERROR_TYPE, 
				_T("Failed to create shared memory block with TSP, rc=0x%lx\r\n%s"),
				GetLastError(), GetLastErrorText());
		return FALSE;
	}

	// Fill in the LINEINITEX parameters
	LINEINITIALIZEEXPARAMS liParams;
	ZeroMemory(&liParams, sizeof(LINEINITIALIZEEXPARAMS));
	liParams.dwTotalSize = sizeof(LINEINITIALIZEEXPARAMS);
	liParams.dwOptions = LINEINITIALIZEEXOPTION_USECOMPLETIONPORT;
	liParams.Handles.hCompletionPort = m_hIOCPort;

	if (!_Module.IsService())
		_Module.LogEvent(0, _T("Connecting to TAPISRV..."));

	// Initialize the TAPI line system for this process.
	LONG lResult = LINEERR_REINIT;
	while (lResult == LINEERR_REINIT)
	{
		lResult = lineInitializeEx(&m_hLineApp, hInstance, NULL, 
									pszAppName, &m_dwNumLines,
									&m_dwApiVersion, &liParams);
		if (lResult == LINEERR_REINIT)
			Sleep(250);
	}

	if (lResult != 0)
	{
		_Module.LogEvent(EVENTLOG_ERROR_TYPE, 
			_T("TAPI failed to initialize, rc=0x%lx"), lResult);
		return FALSE;
	}

	// If there are no line devices, shutdown TAPI and return an error.
	if (m_dwNumLines == 0)
	{
		_Module.LogEvent(EVENTLOG_INFORMATION_TYPE,
			_T("No available line devices associated with this machine."));
		return FALSE;
	}
	else if (!_Module.IsService())
		_Module.LogEvent(0, _T("TAPI initialized, found %ld devices."), m_dwNumLines);

	// Start our I/O completion thread which will receive each of the agent requests.
	m_hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) _CompletionThread, 
						this, 0, &m_dwThreadID);
	if (m_hThread == NULL)
	{
		_Module.LogEvent(EVENTLOG_ERROR_TYPE, 
			_T("Unable to create processing thread, rc=0x%lx\r\n%s"), 
			GetLastError(), GetLastErrorText());
		return FALSE;
	}

	return TRUE;

}// Initialize_TAPI

/*****************************************************************************
** Procedure:  CTapiInfo::Run
**
** Arguments:  'hevtClose' - Handle which will cause us to close
**
** Returns:    void
**
** Description:  This function runs the request thread which is connected
**               to TAPI.
**
*****************************************************************************/
void CTapiInfo::Run(HANDLE hevtClose)
{
    static LPCTSTR g_pszMsgs[] = {
            {_T("Line_AddressState")},               // 0
            {_T("Line_CallInfo")},                   // 1
            {_T("Line_CallState")},                  // 2
            {_T("Line_Close")},                      // 3
            {_T("Line_DevSpecific")},                // 4
            {_T("Line_DevSpecificFeature")},         // 5
            {_T("Line_GatherDigits")},               // 6
            {_T("Line_Generate")},                   // 7
            {_T("Line_LineDevState")},               // 8
            {_T("Line_MonitorDigits")},              // 9
            {_T("Line_MonitorMedia")},               // 10
            {_T("Line_MonitorTone")},                // 11
            {_T("Line_Reply")},                      // 12
            {_T("Line_Request")},                    // 13
            {_T("Phone_Button")},                    // 14
            {_T("Phone_Close")},                     // 15
            {_T("Phone_DevSpecific")},               // 16
            {_T("Phone_Reply")},                     // 17
            {_T("Phone_State")},                     // 18
            {_T("Line_Create")},                     // 19
            {_T("Phone_Create")},                    // 20
			{_T("Line_AgentSpecific")},				 // 21
			{_T("Line_AgentStatus")},				 // 22
			{_T("Line_AppNewCall")},				 // 23
			{_T("Line_ProxyRequest")},				 // 24
			{_T("Line_Remove")},					 // 25
			{_T("Phone_Remove")}					 // 26
        };        
    static LPCTSTR g_pszTSPIMsgs[] = {
			{_T("Line_NewCall")},					 // 0
			{_T("Line_CallDevSpecific")},            // 1
			{_T("Line_CallDevSpecificFeature")},     // 2
			{_T("Line_CreateDialogInstance")},       // 3
			{_T("Line_SendDialogInstanceData")}      // 4
		};

	// Tell the service provider(s) that the proxy server is 
	// now running!
	if (!_Module.IsService())
		_Module.LogEvent(0, _T("Starting ACD Proxy: connecting to all running service providers"));

	{ACDREQUEST ACD;
	ACD.dwSize = sizeof(ACDREQUEST);
	ACD.dwType = ACDTYPE_OPEN;
	ACD.dwProviderID = _UI32_MAX;
	m_smMgr.Write(&ACD);}

	// Wait for a received event from the service provider.
	// -OR our close event
	for(;;)
	{
		ACDREQUEST* lpACD;
		if (!m_smMgr.ReadAny(&lpACD, hevtClose))
			break;

		// No block? spin back around
		if (lpACD == NULL)
			continue;

		// Process the received event.
		switch (lpACD->dwType)
		{
			// Response from the service provider concerning a sent request from 
			// some agent application somewhere in the world.
			case ACDTYPE_PROXYRESPONSE:
			{
				LPLINEPROXYREQUEST lpProxy = reinterpret_cast<LPLINEPROXYREQUEST>(lpACD->dwReqKey);
				LPLINEPROXYREQUEST lpMyProxy = reinterpret_cast<LPLINEPROXYREQUEST>(lpACD->vPart);
				MoveMemory(lpProxy, lpMyProxy, lpMyProxy->dwSize);
				if (!_Module.IsService())
				{
					_Module.LogEvent(0, _T("Rcv Response for %s [0x%lx] rc=0x%lx"), 
						(lpProxy->dwRequestType >= 1 && lpProxy->dwRequestType <= 8) ?
							g_pszFunctions[lpProxy->dwRequestType-1] : _T("Unknown"),
							lpProxy, lpACD->lResult);
				}
				lineProxyResponse(DeviceIDToLine(lpACD->dwLineDeviceID), lpProxy, lpACD->lResult);
			}
			break;

			// The state of an agent on some address has changed, forward the
			// information on to TAPI.
			case ACDTYPE_NOTIFY:
			{
				ACDNOTIFY* lpNotify = (ACDNOTIFY*) lpACD->vPart;
				HLINE hLine = DeviceIDToLine(lpACD->dwLineDeviceID);
				LONG lResult = lineProxyMessage(hLine, NULL, lpNotify->dwMsg,
								lpNotify->dwParam1, lpNotify->dwParam2, lpNotify->dwParam3);
				if (!_Module.IsService())
				{
					LPCTSTR pszMsg = _T("???");			
					if (lpNotify->dwMsg <= 26)
						pszMsg = g_pszMsgs[lpNotify->dwMsg];
					else if (lpNotify->dwMsg >= TSPI_MESSAGE_BASE && 
						lpNotify->dwMsg < TSPI_MESSAGE_BASE+5)
						pszMsg = g_pszTSPIMsgs[lpNotify->dwMsg-TSPI_MESSAGE_BASE];
					_Module.LogEvent(0, _T("%s (0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx), rc=0x%lx"),
									pszMsg, lpACD->dwLineDeviceID, lpNotify->dwMsg,
									lpNotify->dwParam1, lpNotify->dwParam2, 
									lpNotify->dwParam3, lResult);
				}
			}
			break;

			// This should be the first packet we see from the service provider.
			case ACDTYPE_OPEN:
			{
				ACDOPEN* lpInit = (ACDOPEN*) lpACD->vPart;
				OpenDevice(lpACD->dwProviderID, lpACD->dwLineDeviceID, lpInit->dwAgentFeatures);
			}
			break;

			// This is a notification from the provider to SHUTDOWN all our
			// connections to that provider.
			case ACDTYPE_CLOSE:
				CloseDevices(lpACD->dwProviderID);
				break;

			// Unknown
			default:
				_Module.LogEvent(EVENTLOG_ERROR_TYPE, _T("Unknown TSP event 0x%lx received, discarded."), lpACD->dwType);
				break;
		}

		// Delete the ACD request and swing around for the next response.
		delete [] (char*) lpACD;
	}

	// Close any open TAPI devices.
	CloseDevices();

	if (!_Module.IsService())
		_Module.LogEvent(0, _T("Stopping ACD Proxy: disconnecting from all running service providers"));

	// Tell the service provider(s) that the proxy server is now closed.
	{ACDREQUEST ACD;
	ACD.dwSize = sizeof(ACDREQUEST);
	ACD.dwType = ACDTYPE_CLOSE;
	ACD.dwProviderID = _UI32_MAX;
	m_smMgr.Write(&ACD);}

}// CTapiInfo::Run

/*****************************************************************************
** Procedure:  CTapiInfo::PullAgentRequests
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function pulls requests from TAPI and queues them
**               for the runner thread.
**
*****************************************************************************/
void CTapiInfo::PullAgentRequests()
{
	// Wait for data on the completion port
	LPLINEMESSAGE pMsg;
	DWORD dwNumBytes, dwKey;
	while (GetQueuedCompletionStatus(m_hIOCPort, &dwNumBytes, &dwKey,
									 (LPOVERLAPPED*)&pMsg, INFINITE))
	{
		// If we didn't get a message, our completion port was destroyed
		// which indicates that this thread needs to terminate.
		if (pMsg == NULL)
			break;

		// Otherwise, process the message.
		if (pMsg->dwMessageID == LINE_PROXYREQUEST)
			QueueAgentMessage((HLINE)pMsg->hDevice, (LPLINEPROXYREQUEST) pMsg->dwParam1);

		// If this is a notification of a call on this line, delete
		// our representation of the call so it can go away in the provider
		// when the call is IDLE'd.
		else if (pMsg->dwMessageID == LINE_APPNEWCALL)
		{
			if (!_Module.IsService())
			{
				_Module.LogEvent(0, _T("LINE_APPNEWCALL: hLine=0x%lx, AddressID=%ld, hCall=0x%lx, Privs=%s"),
					pMsg->hDevice, pMsg->dwParam1, pMsg->dwParam2,
					(pMsg->dwParam3 == LINECALLPRIVILEGE_NONE) ? _T("None") :
					(pMsg->dwParam3 == LINECALLPRIVILEGE_MONITOR) ? _T("Monitor") :
					(pMsg->dwParam3 == LINECALLPRIVILEGE_OWNER) ? _T("Owner") : 
					_T("Owner/Monitor"));
			}

			// Deallocate the call handle.
			lineDeallocateCall((HCALL)pMsg->dwParam2);
		}

		// If the provider is forcing the line to close, then close the hLine.
		else if (pMsg->dwMessageID == LINE_CLOSE)
		{
			DWORD dwDevice = LineToDeviceID((HLINE)pMsg->hDevice);
			if (dwDevice != -1L)
			{
				if (!_Module.IsService())
					_Module.LogEvent(0, _T("LINE_CLOSE: hLine=0x%lx, Line Device %ld"), 
										pMsg->hDevice, dwDevice);
				CloseLineDevice(dwDevice);
			}
		}

		// If a line gets created, show it in the log.
		else if (pMsg->dwMessageID == LINE_CREATE)
		{
			if (!_Module.IsService())
				_Module.LogEvent(0, _T("LINE_CREATE: hLine=0x%lx"), pMsg->hDevice);
		}

		// Free the buffer.
		LocalFree(pMsg);
	}

}// CTapiInfo::PullAgentRequests

/*****************************************************************************
** Procedure:  CTapiInfo::QueueAgentMessage
**
** Arguments:  'hLine'		- Line which is causing this event
**             'lpProxy'	- PROXYREQUEST structure for the event
**
** Returns:    void
**
** Description:  This function passes the request to the service provider
**               using the shared memory buffer.
**
*****************************************************************************/
void CTapiInfo::QueueAgentMessage(HLINE hLine, LPLINEPROXYREQUEST lpProxy)
{
	// If the proxy request is invalid, kill it.
	if (lpProxy == NULL)
		return;

	DWORD dwLineID = LineToDeviceID(hLine);
	DWORD dwProviderID = DeviceIDToProviderID(dwLineID);

	if (!_Module.IsService())
	{
		_Module.LogEvent(0, _T("Snd %s [0x%lx] for 0x%lx:0x%lx from %s\\%s"), 
			(lpProxy->dwRequestType >= LINEPROXYREQUEST_SETAGENTGROUP && 
			 lpProxy->dwRequestType <= LINEPROXYREQUEST_SETAGENTSTATEEX) ?
				g_pszFunctions[lpProxy->dwRequestType-1] : _T("Unknown"),
			lpProxy, dwProviderID, dwLineID, 
			(lpProxy->dwClientMachineNameSize > 0) ?
			ISTR(lpProxy, dwClientMachineNameOffset) : _T(""),
			(lpProxy->dwClientUserNameSize > 0) ?
			ISTR(lpProxy, dwClientUserNameOffset) : _T(""));
	}

	// Ask the IPC to queue it.
	DWORD dwSize = sizeof(ACDREQUEST) + lpProxy->dwSize;
	
	ACDREQUEST* pACD = (ACDREQUEST*) new char[dwSize];
	pACD->dwSize = dwSize;
	pACD->dwType = ACDTYPE_PROXYREQUEST;
	pACD->dwLineDeviceID = dwLineID;
	pACD->dwProviderID = dwProviderID;
	pACD->dwReqKey = reinterpret_cast<DWORD_PTR>(lpProxy);
	lstrcpy(pACD->chMachine, ISTR(lpProxy,dwClientMachineNameOffset));
	lstrcpy(pACD->chUserName, ISTR(lpProxy,dwClientUserNameOffset));
	MoveMemory(pACD->vPart, lpProxy, lpProxy->dwSize);
	m_smMgr.Write(pACD);

	delete [] (char*) pACD;

}// CTapiInfo::ProcessAgentMessage

/*****************************************************************************
** Procedure:  CTapiInfo::OpenDevice
**
** Arguments:  'dwDevice'
**
** Returns:    HLINE representing open TAPI device
**
** Description:  This function opens the set of devices for agent activity.
**
*****************************************************************************/
BOOL CTapiInfo::OpenDevice(DWORD dwProviderID, DWORD dwDeviceID, DWORD dwFeatures)
{
	// If this device is ALREADY open, return OK.
	if (DeviceIDToLine(dwDeviceID) != NULL)
		return TRUE;

	// Negotiate a version of TAPI for this line device; if this fails we cannot
	// open the device since it doesn't even support TAPI 2.0.
	DWORD dwVersion = 0;
	LINEEXTENSIONID lineExtID;
	ZeroMemory(&lineExtID, sizeof(lineExtID));
	LONG lResult = lineNegotiateAPIVersion(m_hLineApp, dwDeviceID, 0x00020000, m_dwApiVersion, &dwVersion, &lineExtID);
	if (lResult != 0 || dwVersion < 0x00020000)
		return FALSE;

	// Allocate a LINECALLPARAMS structure.
	LPLINECALLPARAMS pLCP = (LPLINECALLPARAMS) malloc(sizeof(LINECALLPARAMS) + sizeof(DWORD)*8);
	if (pLCP == NULL)
		return FALSE;

	ZeroMemory(pLCP, sizeof(LINECALLPARAMS));
	pLCP->dwTotalSize = sizeof(LINECALLPARAMS) + sizeof(DWORD)*8;
	pLCP->dwDevSpecificOffset = sizeof(LINECALLPARAMS);
	pLCP->dwDevSpecificSize = 0;

	// Go through the capabilities and mark it.
	LPDWORD lpdwPR = (LPDWORD)((LPBYTE)pLCP + pLCP->dwDevSpecificOffset);
	TString strFeatures = _T("");
	for (int i = 0; i < sizeof(gAgentFeatures) / sizeof(gAgentFeatures[0]); ++i)
	{
		if (dwFeatures & gAgentFeatures[i].dwAgentBit)
		{
			*lpdwPR++ = gAgentFeatures[i].dwAgentFeature;
			if (!strFeatures.empty())
				strFeatures += _T(", ");
			strFeatures += g_pszFunctions[gAgentFeatures[i].dwAgentFeature-1];
			pLCP->dwDevSpecificSize++;
		}
	}

	// Add the total size.
	pLCP->dwDevSpecificSize *= sizeof(DWORD);

	// If we are not a service, output some debug information.
	if (!_Module.IsService())
	{
		_Module.LogEvent(0, _T("Opening line device #%ld, Provider=0x%lx, Features=0x%lx\r\n%s"), 
					dwDeviceID, dwProviderID, dwFeatures, strFeatures.c_str());
	}

	// Open the device
	HLINE hLine = NULL;
	lResult = lineOpen(m_hLineApp, dwDeviceID, &hLine, 
			dwVersion, 0, 0, LINEOPENOPTION_PROXY | 
			LINECALLPRIVILEGE_NONE, LINEMEDIAMODE_INTERACTIVEVOICE, pLCP);

	free(pLCP);

	if (lResult != 0)
	{
		_Module.LogEvent(EVENTLOG_ERROR_TYPE, 
			_T("Failed to open device %ld, rc=0x%lx"), dwDeviceID, lResult);
		return FALSE;
	}

	// Map the open device.
	m_mapLineToId[hLine] = dwDeviceID;
	m_mapIdToLine[dwDeviceID] = hLine;
	m_mapIdToProviderID[dwDeviceID] = dwProviderID;
	return TRUE;

}// CTapiInfo::OpenTapiDevice

/*****************************************************************************
** Procedure:  CTapiInfo::CloseDevices
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function closes all the devices we are monitoring.
**
*****************************************************************************/
void CTapiInfo::CloseDevices(DWORD dwProviderID)
{
	if (!_Module.IsService())
	{
		if (dwProviderID == -1L)
			_Module.LogEvent(0, _T("Closing all devices"));
		else
			_Module.LogEvent(0, _T("Closing devices under provider 0x%lx"), dwProviderID);
	}

	if (dwProviderID == -1L)
	{
		std::map<DWORD,HLINE>::iterator iPos;
		for (iPos = m_mapIdToLine.begin(); iPos != m_mapIdToLine.end(); iPos++)
			lineClose((*iPos).second);

		m_mapLineToId.erase(m_mapLineToId.begin(), m_mapLineToId.end());
		m_mapIdToLine.erase(m_mapIdToLine.begin(), m_mapIdToLine.end());
		m_mapIdToProviderID.erase(m_mapIdToProviderID.begin(), m_mapIdToProviderID.end());
	}
	else
	{
		for (std::map<DWORD,DWORD>::iterator hPos = m_mapIdToProviderID.begin();
			 hPos != m_mapIdToProviderID.end(); hPos++)
		{
			DWORD dwDeviceID = (*hPos).first;
			CloseLineDevice(dwDeviceID);
		}
	}

}// CTapiInfo::CloseDevices

/*****************************************************************************
** Procedure:  CTapiInfo::CloseLineDevice
**
** Arguments:  'dwDevice' - Device ID to close
**
** Returns:    void
**
** Description:  This function closes all the devices we are monitoring.
**
*****************************************************************************/
void CTapiInfo::CloseLineDevice(DWORD dwDeviceID)
{
	if (!_Module.IsService())
		_Module.LogEvent(0, _T("Closing line device #%ld, Provider=0x%lx"), 
					dwDeviceID, DeviceIDToProviderID(dwDeviceID));

	// Close the device
	HLINE hLine = DeviceIDToLine(dwDeviceID);
	if (hLine != NULL)
		lineClose(hLine);

	// Remove it from our map.
	std::map<HLINE,DWORD>::iterator iPos1 = m_mapLineToId.find(hLine);
	if (iPos1 != m_mapLineToId.end())
		m_mapLineToId.erase(iPos1);
	std::map<DWORD,HLINE>::iterator iPos2 = m_mapIdToLine.find(dwDeviceID);
	if (iPos2 != m_mapIdToLine.end())
		m_mapIdToLine.erase(iPos2);
	std::map<DWORD,DWORD>::iterator iPos3 = m_mapIdToProviderID.find(dwDeviceID);
	if (iPos3 != m_mapIdToProviderID.end())
		m_mapIdToProviderID.erase(iPos3);

}// CTapiInfo::CloseLineDevice
