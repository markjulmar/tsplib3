$$IF(VERBOSE)
/***************************************************************************
//
// DEVICE.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Handles raw device events and the physical connection to the hardware.
//
// Copyright (C) $$CURR_YEAR$$ $$COMPANY_NAME$$
// All rights reserved
//
// $$JULMAR_INFO$$
// 
/***************************************************************************/

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
$$ENDIF
#include "stdafx.h"
#include <process.h>
#include "$$root$$.h"
$$IF(HAS_UI)
#include <spbstrm.h>
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  MainConnThread
**
** Arguments:  'pDevice'	- Device object
**
** Returns:    void
**
** Description:  Main connection thread
**
*****************************************************************************/
$$ENDIF
static unsigned __stdcall MainConnThread(void* pParam)
{
	reinterpret_cast<$$DEVICE_CLASS_NAME$$*>(pParam)->ConnectionThread();
	_endthreadex(0);
	return 0;

}// ConnThread

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$DEVICE_CLASS_NAME$$::$$DEVICE_CLASS_NAME$$
**
** Arguments:  void
**
** Returns:    void
**
** Description: Constructor for the device object
**
*****************************************************************************/
$$ENDIF
$$IF(USE_COMM)
$$DEVICE_CLASS_NAME$$::$$DEVICE_CLASS_NAME$$() : m_hConnThread(0), m_hCommPort(INVALID_HANDLE_VALUE)
$$ELSE
$$DEVICE_CLASS_NAME$$::$$DEVICE_CLASS_NAME$$() : m_hConnThread(0)
$$ENDIF
{
	// Create the stop event.
	m_hevtStop = CreateEvent (NULL, true, false, NULL);
	InitializeCriticalSection(&m_csData);

	// TODO: place any constructor code here

}// $$DEVICE_CLASS_NAME$$::$$DEVICE_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$DEVICE_CLASS_NAME$$::~$$DEVICE_CLASS_NAME$$
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Destructor for the device object
**
*****************************************************************************/
$$ENDIF
$$DEVICE_CLASS_NAME$$::~$$DEVICE_CLASS_NAME$$()
{
	// Stop the input thread and wait for it to terminate.
	SetEvent (m_hevtStop);
	WaitForSingleObject(m_hConnThread, 5000);
	CloseHandle(m_hConnThread);
$$IF(USE_WINSOCK)

	// Shutdown Winsock
	while (WSACleanup() == WSAEINPROGRESS)
		Sleep(100);
$$ENDIF

	// Close all our synchronization object handles
	CloseHandle(m_hevtStop);
	DeleteCriticalSection(&m_csData);

	// TODO: destroy any allocated memory here

}// $$DEVICE_CLASS_NAME$$::~$$DEVICE_CLASS_NAME$$

$$IF(HAS_UI)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$DEVICE_CLASS_NAME$$::read
**
** Arguments:  'istm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& $$DEVICE_CLASS_NAME$$::read(TStream& istm)
{
	// Always call the base class!
	CTSPIDevice::read(istm);

	// TODO: Add any additional information which was stored by the 
	// user-interface component of the TSP.

	return istm;

}// $$DEVICE_CLASS_NAME$$::read

$$ENDIF
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$DEVICE_CLASS_NAME$$::Init
**
** Arguments:  'dwProviderId'		-	TAPI provider ID
**             'dwBaseLine'			-	Starting line index from TAPI system
**             'dwBasePhone'		-	Starting phone index from TAPI system
**             'dwLines'			-	Number of lines owned by this device
**             'dwPhones'			-	Number of phones owned by this device
**             'hProvider'			-	Opaque Provider handle from TAPI
**			   'lpfnCompletion'		-	Completion PROC for any ASYNCH requests.
**
** Returns:    true/false whether TAPI should continue loading the device.
**
** Description: This function is called during providerInit to initialize
**              each device identified by TAPI (group of lines/phones).
**              It is overridden to create threads and other init-time work
**              that might fail (and therefore shouldn't be done in the 
**              constructor).
**
*****************************************************************************/
$$ENDIF
bool $$DEVICE_CLASS_NAME$$::Init(DWORD dwProviderId, DWORD dwBaseLine, DWORD dwBasePhone, 
					 DWORD dwLines, DWORD dwPhones, HPROVIDER hProvider, 
					 ASYNC_COMPLETION lpfnCompletion)
{
$$IF(USE_WINSOCK)
	// Initialize WinSock
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (nResult != 0)
	{
		_TSP_DTRACE(_T("Winsock failed to initialize, rc=0x%lx\n"), WSAGetLastError());
		return false;
	}

$$ENDIF
	// Add the switch information so others can identify this provider.
	SetSwitchInfo(_T("$$SWITCH_INFO$$"));

	// Turn on the interval timer for this device for once every second
	SetIntervalTimer(1000);

$$IF(VERBOSE)
	// Pass through to the base class and let it initialize the line and phone 
	// devices.  After this call, each of the objects will be available.
	// In addition, the providerID information will be filled in.
$$ENDIF
	if (CTSPIDevice::Init (dwProviderId, dwBaseLine, dwBasePhone, dwLines, dwPhones, hProvider, lpfnCompletion))
	{
$$IF(VERBOSE)
		// Create our connection thread which manages the connection to the hardware.
$$ENDIF
		UINT uiThread;
		m_hConnThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, MainConnThread, static_cast<void*>(this), 0, &uiThread));

$$IF(VERBOSE)
		// If the thread failed to create, output an error and tell TAPI to stop
		// loading us (fatal initialization error).
$$ENDIF
		if (m_hConnThread == NULL)
		{
			_TSP_DTRACE(_T("Failed to create input thread\n"), GetLastError());
			return false;
		}

		// Tell TAPI to continue loading our provider.
		return true;
	}
	return false;

}// $$DEVICE_CLASS_NAME$$::Init

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$DEVICE_CLASS_NAME$$::ConnectionThread
**
** Arguments:  void
**
** Returns:    Thread return code
**
** Description:  This thread manages the communication connection to the hardware.
**
*****************************************************************************/
$$ENDIF
unsigned $$DEVICE_CLASS_NAME$$::ConnectionThread()
{
$$IF(USE_COMM)
	// TODO: Read the port information from persistent storage
	TString strPortToOpen = _T("COM1");

$$ENDIF
	// Loop around trying to connect and then receiving data
	bool fConnected = false;
	while (WaitForSingleObject(m_hevtStop, 0) == WAIT_TIMEOUT)
	{
		// TODO: Remove this sleep command once the thread pauses waiting
		// for events from the hardware. This was inserted so that the generated
		// provider doesn't soak up CPU time when tested.
		Sleep(1000);

		// Keep trying to connect
		while (!fConnected)
		{
$$IF(USE_COMM)
			m_hCommPort = CreateFile (strPortToOpen.c_str(), 
			                   GENERIC_READ|GENERIC_WRITE, 0, NULL, 
		                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | 
							   FILE_FLAG_OVERLAPPED, NULL);
			if (m_hCommPort == INVALID_HANDLE_VALUE)
				_TSP_DTRACE(_T("Failed to open port <%s>, rc=0x%lx\n"), strPortToOpen.c_str(), GetLastError());
			else
			{ 
				// TODO: Initialize the comm device to the required settings
				DCB dcb;
				SetupComm (m_hCommPort, 4096, 4096);
				GetCommState(m_hCommPort, &dcb);
				dcb.BaudRate = 9600;
				dcb.Parity = NOPARITY;
				dcb.ByteSize = 8;
				dcb.StopBits = ONESTOPBIT;
				SetCommState(m_hCommPort, &dcb);
				EscapeCommFunction (m_hCommPort, SETDTR);

				// TODO: Perform some logon to the hardware
				fConnected = true;
			}
$$ELSE
$$IF(USE_WINSOCK)
			// TODO: Connect to the hardware device using Winsock
			fConnected = true;
$$ELSE
			// TODO: Connect to the hardware device
			fConnected = true;
$$ENDIF
$$ENDIF
		}

		// TODO: Receive a packet/event from the switch

$$IF(EVENT_OBJECT_NAME)
		// TODO: Parse the packet into a $$EVENT_OBJECT_NAME$$
		$$EVENT_OBJECT_NAME$$* pEvent = NULL;
$$ELSE
		// TODO: Parse the packet
		LPCTSTR pEvent = NULL;
$$ENDIF

		// Determine the owner of the event
		CTSPIConnection* pConnOwner = LocateOwnerFromEvent(pEvent);
$$IF(USE_POOLMGR)

		// Add it to the pool manager
		bool fDispatched = (pConnOwner != NULL && 
				m_mgrThreads.Add(pConnOwner, pConnOwner->GetPermanentDeviceID(), pEvent));
$$IF(VERBOSE)

		// Delete it if we could not find an owner. This could happen if the TSP
		// never saw required preceding events (such as call creations) because it 
		// wasn't running at the time or an event loss occurred due to link failure.
$$ENDIF
		if (!fDispatched)
			TPM_DelEvent(pEvent);
$$ELSE
$$IF(VERBOSE)

		// RECOMMENDATION:
		// Since the worker thread class is not being used, the wizard has assumed
		// that you intend to develop your own thread implementation. It is recommended
		// that the dispatch code be performed on a different thread than the one
		// receiving the input events from the device.
$$ENDIF

		// Dispatch the request to the owner
		if (pConnOwner != NULL)
			pConnOwner->ReceiveData(static_cast<LPCVOID>(pEvent));		
$$ENDIF
	}

$$IF(USE_COMM)
	// Reset and close the comm device
	PurgeComm (m_hCommPort, PURGE_TXABORT | PURGE_TXCLEAR);
	PurgeComm (m_hCommPort, PURGE_RXABORT | PURGE_RXCLEAR);
	EscapeCommFunction (m_hCommPort, CLRDTR);
	CloseHandle(m_hCommPort);
	m_hCommPort = INVALID_HANDLE_VALUE;

$$ENDIF
	return 0;

}// $$DEVICE_CLASS_NAME$$::ConnectionThread

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$DEVICE_CLASS_NAME$$::LocateOwnerFromEvent
**
** Arguments:  'pEvent' - Event received from the hardware
**
** Returns:    CTSPIConnection which "owns" the event.
**
** Description: This function examines the received event and determines
**              which connection object the event belongs to.
**
*****************************************************************************/
$$ENDIF
$$IF(EVENT_OBJECT_NAME)
CTSPIConnection* $$DEVICE_CLASS_NAME$$::LocateOwnerFromEvent($$EVENT_OBJECT_NAME$$* pEvent)
$$ELSE
CTSPIConnection* $$DEVICE_CLASS_NAME$$::LocateOwnerFromEvent(LPCVOID pEvent)
$$ENDIF
{
	CTSPIConnection* pConnOwner = NULL;

	// TODO: Examine the event and determine which line or phone object it 
	// refers to.
	//
	// Many telephony systems send a "station identifier" or owner with
	// each event so that link monitoring software can properly track things.
	//
	// You can associate an identifier with lines and phones by calling the
	// CTSPILineConnection::SetPermanentLineID and CTSPIPhoneConnection::SetPermanentPhoneID
	// functions, then they can be found as follows:
	
	DWORD dwIdentifier = 0;

$$IF(OVERRIDE_LINE)
	bool fIsLineEvent = false;
	if (pConnOwner == NULL && fIsLineEvent)
	{
		pConnOwner = FindLineConnectionByPermanentID(dwIdentifier);
	}

$$ENDIF
$$IF(OVERRIDE_PHONE)
	bool fIsPhoneEvent = false;
	if (pConnOwner == NULL && fIsPhoneEvent)	
	{
		pConnOwner = FindPhoneConnectionByPermanentID(dwIdentifier);
	}

$$ENDIF
	// Another item that comes with many events is the call-id. This uniquely
	// identifies the call on the hardware.

	DWORD dwCallID = 0;

	// If we received a call-id, check for in-switch calls (station-to-station)
	// and see if another call object is sharing this call-id. If this is the
	// case, determine which call the event refers to.
	if (pConnOwner == NULL && dwCallID != 0)
	{
		const CTSPICallHub* pHub = FindCallHub(dwCallID);
		if (pHub != NULL)
		{
			if (pHub->GetHubCount() > 1)
			{
				// TODO: Determine which call it refers to
			}
			else
			{
				CTSPICallAppearance* pCall = pHub->GetCall(0);
				pConnOwner = pCall->GetLineOwner();
			}
		}
	}

	return pConnOwner;
	
}// $$DEVICE_CLASS_NAME$$::LocateOwnerFromEvent
