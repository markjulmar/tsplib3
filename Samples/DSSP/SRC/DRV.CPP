/*****************************************************************************/
//
// DRV.CPP - Digital Switch Service Provider Sample
//                                                                        
// This file contains the "device" code for the emulated TSP.  The functions
// in here would be device specific in order to drive the switch.  In the
// case of our emulated switch, the emulator program does most of the work.
//
// Copyright (C) 1997-1999 JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
//                                                                        
// This service provider talks to a simulated digital switch emulator.  It
// is designed as a sample of how to implement a service provider using
// the TAPI C++ SP class library.  
//
// This source code is only intended as a supplement to the
// documentation provided with the TAPI C++ SP class library.
//
/*****************************************************************************/

#include "stdafx.h"
#include "dssp.h"

/*****************************************************************************
** CClientSocket
**
** This is our client socket object
**
*****************************************************************************/
class CSocketMgr;
class CClientSocket : public CSocket
{
// Construction
public:
	CClientSocket(CSocketMgr* pMgr);

// Operations
public:
	CSocketMgr* m_pMgr;

// Implementation
protected:
	virtual void OnReceive(int nErrorCode);
};

#define UM_SENDMESSAGE (WM_USER+100)

/*****************************************************************************
** CSocketMgr
**
** This thread manages our socket connection to the emulator server.
**
*****************************************************************************/
class CSocketMgr : public CWinThread
{
	DECLARE_DYNCREATE(CSocketMgr)

// Data members
public:
	CEvent* m_pevtConnect;		// Event
	CDSDevice* m_pDevice;		// Device owner
	CClientSocket* m_pSocket;	// Socket connected to emulator
	CSocketFile* m_pFile;		// File used with archives
	CArchive* m_pArchiveIn;		// Input archive queue
	CArchive* m_pArchiveOut;	// Output archive queue
	struct
	{
		LPVOID lpBuff;
		DWORD dwSize;
		CEvent* pevtDone;
	} WaitBlock;

// Overrides
public:
	CSocketMgr();
	BOOL InitInstance();
	int ExitInstance();

// New methods
public:
	bool SendMsg(DWORD dwCommand, LPVOID lpBuff, DWORD dwSize);
	void ProcessPendingRead();
	bool ReceiveMsg(LPDWORD pdwResponse, LPVOID* ppBuff, LPDWORD pdwSize);

// Message map entries
protected:
	afx_msg void OnSendMessage(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

IMPLEMENT_DYNCREATE(CSocketMgr, CWinThread)
BEGIN_MESSAGE_MAP (CSocketMgr, CWinThread)
	ON_THREAD_MESSAGE(UM_SENDMESSAGE, OnSendMessage)
END_MESSAGE_MAP()

/*****************************************************************************
** DATABLOCK
**
** Passed data block between two worker threads
**
*****************************************************************************/
typedef struct
{
	DWORD dwCommand;
	LPVOID lpvData;
	DWORD dwSize;
	CEvent* pevtWait;
	LPVOID lpvReturn;
	DWORD dwReturnSize;
} DATABLOCK;

/*****************************************************************************
** Procedure:  CClientSocket::CClientSocket
**
** Arguments:  'pMgr' - Socket manager
**
** Returns:    void
**
** Description:  Constructor
**
*****************************************************************************/
CClientSocket::CClientSocket(CSocketMgr* pMgr) : m_pMgr(pMgr)
{
}// CClientSocket::CClientSocket

/*****************************************************************************
** Procedure:  CClientSocket::OnReceive
**
** Arguments:  'nErrorCode'
**
** Returns:    void
**
** Description:  This tells us we have data waiting on the socket.
**
*****************************************************************************/
void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	m_pMgr->ProcessPendingRead();

}// CClientSocket::OnReceive

/*****************************************************************************
** Procedure:  CSocketMgr::CSocketMgr
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Constructor
**
*****************************************************************************/
CSocketMgr::CSocketMgr() : m_pDevice(0), m_pevtConnect(0), m_pSocket(0),
	m_pFile(0), m_pArchiveIn(0), m_pArchiveOut(0)
{
	ZeroMemory(&WaitBlock, sizeof(WaitBlock));

}// CSocketMgr::CSocketMgr

/*****************************************************************************
** Procedure:  CSocketMgr::InitInstance
**
** Arguments:  void
**
** Returns:    TRUE/FALSE to run a message loop
**
** Description:  This is the starting function for our socket manager.
**
*****************************************************************************/
BOOL CSocketMgr::InitInstance()
{
	// Initialize the sockets interface
	AfxSocketInit();

	// Get the destination address
	CString strAddress = GetSP()->ReadProfileString (m_pDevice->GetProviderID(), _T("IPServer"), _T("127.0.0.1")).c_str();
	_TSP_DTRACE(TRC_MIN, _T("Opening socket and connecting to %s\r\n"), strAddress);

	// Mark us as "not connected"
	m_pDevice->m_fConnected = FALSE;

	// Create a socket to manage notifications for.
	try
	{
		// Create the socket
		m_pSocket = new CClientSocket(this);
		if (!m_pSocket->Create())
			throw 1;

		// Connect to our destination.
		if (!m_pSocket->Connect(strAddress, SOCKET_PORT))
			throw 2;
	}
	catch (...)
	{
		if (m_pevtConnect)
			m_pevtConnect->SetEvent();
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;
		return FALSE;
	}

	// The device is now connected to the emulator, tell our device thread to 
	// proceed with initialization.
	m_pDevice->m_fConnected = TRUE;
	if (m_pevtConnect != NULL)
		m_pevtConnect->SetEvent();

	// Create our stream support for the socket.
	m_pFile = new CSocketFile(m_pSocket);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);

	return TRUE;

}// CSocketMgr::InitInstance

/*****************************************************************************
** Procedure:  CSocketMgr::ExitInstance
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Destructor for the socket manager
**
*****************************************************************************/
int CSocketMgr::ExitInstance()
{
	if (m_pDevice)
		m_pDevice->m_pThreadInput = NULL;

	// Send the disconnect to the emulator.
	if ((m_pSocket != NULL) && (m_pFile != NULL) &&	(m_pArchiveOut != NULL))
	{
		*m_pArchiveOut << (DWORD) EMCOMMAND_CLOSE;
		*m_pArchiveOut << (DWORD) 0;
		m_pArchiveOut->Flush();
	}

	// Delete our archives
	delete m_pArchiveOut;
	delete m_pArchiveIn;
	delete m_pFile;

	// Delete our socket
	if (m_pSocket != NULL)
	{
		BYTE Buffer[50];
		m_pSocket->ShutDown();
		while(m_pSocket->Receive(Buffer,50) > 0);
	}
	delete m_pSocket;

	return 0;

}// CSocketMgr::ExitInstance

/*****************************************************************************
** Procedure:  CSocketMgr::SendMsg
**
** Arguments:  'dwCommand'	- Command to send
**             'lpBuff'		- Optional buffer
**             'dwSize'		- Size of buffer
**
** Returns:    TRUE/FALSE result code
**
** Description:  This function sends the specified message to the server.
**
*****************************************************************************/
bool CSocketMgr::SendMsg(DWORD dwCommand, LPVOID lpBuff, DWORD dwSize)
{
#ifdef _DEBUG
	LPCTSTR g_pszCommands[] = {
		_T(""), _T("Init"), _T("QueryCaps"), _T("GetVersion"), _T("GetAddressInfo"), _T(""), 
		_T("PrepareForCall"), _T("Dial"), _T("DropCall"), _T("TSPExit"), _T("Answer"), 
		_T("Forward"), _T("Transfer"), _T("Hold"), _T("Unhold"), _T("Flash"), _T("SetLevel"), 
		_T("SetHookswitch"), _T("Conference"), _T("Redirect"), _T("SetRingMode"), 
		_T("CompleteCall"), _T("Park"), _T("Unpark"), _T("Close")
	};
#endif

	if (m_pArchiveOut != NULL)
	{
#ifdef _DEBUG
		_TSP_DTRACE(_T("Snd> 0x%lx %s\n"), dwCommand, g_pszCommands[dwCommand]);
#endif

		TRY
		{
			*m_pArchiveOut << dwCommand;
			*m_pArchiveOut << dwSize;
			if (dwSize > 0)
				m_pArchiveOut->Write (lpBuff, dwSize);
			m_pArchiveOut->Flush();
		}
		CATCH(CFileException, e)
		{
			m_pArchiveOut->Abort();
			delete m_pArchiveOut;
			m_pArchiveOut = NULL;
			return FALSE;
		}
		END_CATCH
	}

	return TRUE;

}// CSocketMgr::SendMsg

/*****************************************************************************
** Procedure:  CSocketMgr::ProcessPendingRead
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This is called by the socket when there is data waiting.
**
*****************************************************************************/
void CSocketMgr::ProcessPendingRead()
{
	EVENTBUFF evBuff;
#ifdef _DEBUG
	static LPCTSTR g_pszResults[] = {
		_T("Timer"), _T("Init"), _T("AppExit"), _T("AddressChange"), _T("Ring"), 
		_T("CallerID"), _T("LampChange"), _T("HookswitchChange"), _T(""), _T(""), 
		_T(""), _T("DisplayChange"), _T("ButtonChange"), _T("DigitDetected"), 
		_T("LevelChange"), _T("RedirectID"), _T("ToneDetected"), _T("RingChange"), 
		_T("CompletionResult"), _T("CommandResponse"), _T("Error")
	};
#endif

	do
	{
		// Receive the message
		ZeroMemory(&evBuff, sizeof(EVENTBUFF));
		if (!ReceiveMsg(&evBuff.dwResponse, &evBuff.lpBuff, &evBuff.dwSize))
			return;

#ifdef _DEBUG
		_TSP_DTRACE(_T("Rcv> 0x%lx %s\n"), evBuff.dwResponse, g_pszResults[evBuff.dwResponse]);
#endif

		// If this is a response from a query-style command that the TSP
		// issued to the emulator, then check to see if we are waiting for
		// this command to complete - if not, then ignore the response.
		if (evBuff.dwResponse == EMRESULT_RECEIVED)
		{
			// If we have a pending waitblock, and this satisfies it, then
			// terminate the wait.  This is to handle places where synchronous
			// requests are easier to manage.
			if (WaitBlock.lpBuff != NULL && WaitBlock.dwSize == evBuff.dwSize)
			{
				CopyMemory(WaitBlock.lpBuff, evBuff.lpBuff, evBuff.dwSize);
				CEvent* pevtDone = WaitBlock.pevtDone;
				ZeroMemory(&WaitBlock, sizeof(WaitBlock));
				pevtDone->SetEvent();
			}
		}
		else
		{
			// If there was an error, save off the address that the error
			// occurred on so that the owner of this event can determine if
			// the error is a problem.
			if (evBuff.dwResponse == EMRESULT_ERROR)
			{
				_TSP_ASSERTE(evBuff.dwSize == sizeof(DWORD));
				evBuff.dwAddress = LOWORD(*((LPDWORD)evBuff.lpBuff));
				evBuff.dwError = HIWORD(*((LPDWORD)evBuff.lpBuff));
				FreeMem(evBuff.lpBuff);
				evBuff.lpBuff = NULL;
				evBuff.dwSize = 0;
			}

			// Hand it to the device - it will spin through all the lines and
			// phones in this provider and let them see it.
			m_pDevice->ReceiveData(0, &evBuff);
		}

		// Delete the buffer if one was allocated for this request.
		if (evBuff.lpBuff != NULL && evBuff.dwSize > 0)
			FreeMem (evBuff.lpBuff);
	}
	while(!m_pArchiveIn->IsBufferEmpty());

}// CSocketMgr::ProcessPendingRead

/*****************************************************************************
** Procedure:  CSocketMgr::ReceiveMsg
**
** Arguments:  'pdwResponse' - Returning response
**             'ppBuff'      - Returning buffer
**             'pdwSize'     - Returning size
**
** Returns:    TRUE/FALSE if we returned a message
**
** Description:  This reads data off the socket.
**
*****************************************************************************/
bool CSocketMgr::ReceiveMsg(LPDWORD pdwResponse, LPVOID* ppBuff, LPDWORD pdwSize)
{
	TRY
	{
		*m_pArchiveIn >> *pdwResponse;
		*m_pArchiveIn >> *pdwSize;
		if (*pdwSize > 0)
		{
			char* pBuff = (char*) AllocMem(*pdwSize + 1);
			m_pArchiveIn->Read(pBuff, *pdwSize);
			*ppBuff = pBuff;
		}
		else
			*ppBuff = NULL;
	}
	CATCH(CFileException, e)
	{
		m_pArchiveOut->Abort();
	}
	END_CATCH
	return TRUE;

}// CSocketMgr::ReceiveMsg

/*****************************************************************************
** Procedure:  CDSDevice::CDSDevice
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Constructor for the phone object
**
*****************************************************************************/
CDSDevice::CDSDevice() : m_fConnected(0), m_pThreadInput(0)
{
}// CDSDevice::CDSDevice

/*****************************************************************************
** Procedure:  CDSDevice::~CDSDevice
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Destructor for the phone object
**
*****************************************************************************/
CDSDevice::~CDSDevice()
{
	// Tell the input thread to exit.
	if (m_pThreadInput)
	{
		HANDLE hThread = m_pThreadInput->m_hThread;
		m_pThreadInput->PostThreadMessage(WM_QUIT, 0, 0);
		if (WaitForSingleObject(hThread, 10000) == WAIT_TIMEOUT)
		{
			TerminateThread(hThread, 0);
			delete m_pThreadInput;
		}
	}

}// CDSDevice::~CDSDevice

/*****************************************************************************
** Procedure:  CDSDevice::Init
**
** Arguments:  'dwProviderId'		-	TAPI provider ID
**             'dwBaseLine'			-	Starting line index from TAPI system
**             'dwBasePhone'		-	Starting phone index from TAPI system
**             'dwLines'			-	Number of lines owned by this device
**             'dwPhones'			-	Number of phones owned by this device
**             'hProvider'			-	Opaque Provider handle from TAPI
**			   'lpfnCompletion'		-	Completion PROC for any ASYNCH requests.
**
** Returns:    void
**
** Description:  This function is called during providerInit to initialize
**               each device identified by TAPI.  In our sample provider, there
**               is only one device (the emulator).
**
*****************************************************************************/
bool CDSDevice::Init(DWORD dwProviderId, DWORD dwBaseLine, DWORD dwBasePhone, 
					 DWORD dwLines, DWORD dwPhones, HPROVIDER hProvider, ASYNC_COMPLETION lpfnCompletion)
{
	CEvent evtConnect;
	evtConnect.ResetEvent();

	// Set our provider ID since the thread reads it before we pass on the INIT command.
	// We don't call INIT before we start the thread since it will create line and phone
	// objects which will need to talk to the emulator to initialize.
	m_dwProviderId = dwProviderId;

	// Create a thread to send/receive the data to/from the emulator
	m_pThreadInput = AfxBeginThread(RUNTIME_CLASS(CSocketMgr), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (m_pThreadInput != NULL)
	{
		// Now associate our device with the thread and start it.
		((CSocketMgr*)m_pThreadInput)->m_pDevice = this;
		((CSocketMgr*)m_pThreadInput)->m_pevtConnect = &evtConnect;
		m_pThreadInput->ResumeThread();
	}

	// Wait 1 1/2 second for the socket to connect
	CSingleLock sLock (&evtConnect, FALSE);
	if (sLock.Lock(1500) == 0)
	{
		if (m_pThreadInput && ((CSocketMgr*)m_pThreadInput)->m_pSocket != NULL)
			((CSocketMgr*)m_pThreadInput)->m_pSocket->CancelBlockingCall();
	}

	// If we are NOT connected, then do not add lines.
	if (m_fConnected == FALSE)
		dwLines = dwPhones = 0;

	// Pass through to the default setup.
	CTSPIDevice::Init (dwProviderId, dwBaseLine, dwBasePhone, dwLines, dwPhones, hProvider, lpfnCompletion);

	// If we are connected, associate the line/phone objects together.  The library doesn't do this
	// in case there are multiple lines/phones so that the developer can decide which should be associated.
	if (m_fConnected)
		AssociateLineWithPhone(0, 0);

	return (dwLines > 0);

}// CDSDevice::Init

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Dial
**
** Arguments:  'dwAddressID' - Address ID to dial on
**             'pDialInfo'   - Structure with dialing information
**             'dwCountryCode' - Country code information
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function dials on a prepared address call appearance.
**
*****************************************************************************/
bool CDSDevice::DRV_Dial (DWORD dwAddressID, DIALINFO* pDialInfo, DWORD /*dwCountryCode*/)
{                          
	USES_CONVERSION;

    EMADDRESSINFO AddressInfo;
    AddressInfo.wAddressID = (WORD) dwAddressID;
    lstrcpynA((LPSTR)&AddressInfo.szAddress, T2A((LPTSTR)pDialInfo->strNumber.c_str()), ADDRESS_SIZE);
    AddressInfo.wAddressState = 0;
    AddressInfo.dwMediaMode = 0;
    return SendThreadRequest (EMCOMMAND_DIAL, (LPVOID)&AddressInfo, sizeof(EMADDRESSINFO));

}// CDSDevice::DRV_Dial

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Transfer
**
** Arguments:  'dwAddressID' - Address ID to transfer (FROM)
**             'lpszAddr'    - Dialable portion of transfer address
**             'dwAddressToID' - Possible address to transfer to
**             'fMoveToConf' - Whether this is to transition to a conference call.
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function transfers a call, possibly transferring to conference.
**
*****************************************************************************/
bool CDSDevice::DRV_Transfer (DWORD dwAddressID, LPCSTR lpszAddr, DWORD dwAddressToID, bool fMoveToConf)
{                            
    EMTRANSFERINFO TranInfo;
    TranInfo.wAddressID = (WORD) dwAddressID;
    TranInfo.wTransferAddress = (WORD) dwAddressToID;
    TranInfo.fMoveToConference = (WORD) fMoveToConf;
    
    if (lpszAddr != NULL)
        lstrcpynA(TranInfo.szAddress, lpszAddr, ADDRESS_SIZE);
    else
        TranInfo.szAddress[0] = '\0';
    return SendThreadRequest (EMCOMMAND_TRANSFER, (LPVOID)&TranInfo, sizeof(EMTRANSFERINFO));

}// CDSDevice::DRV_Transfer

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Forward
**
** Arguments:  'dwAddressID' - Address ID to forward
**             'lpszAddr'    - Dialable portion of transfer address
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function sets an address to forward.
**
*****************************************************************************/
bool CDSDevice::DRV_Forward (DWORD dwAddressID, LPCSTR lpszAddr)
{                            
    EMFORWARDINFO ForwInfo;
    ForwInfo.wAddressID = (WORD) dwAddressID;
    if (lpszAddr != NULL) // Unforward request?
        lstrcpynA(ForwInfo.szAddress, lpszAddr, ADDRESS_SIZE);
    else
        ForwInfo.szAddress[0] = '\0';
    return SendThreadRequest (EMCOMMAND_FORWARD, (LPVOID)&ForwInfo, sizeof(EMFORWARDINFO));

}// CDSDevice::DRV_Transfer

/*****************************************************************************
** Procedure:  CDSDevice::DRV_SetGain
**
** Arguments:  'dwHookSwitch' - Hookswitch to set the gain for.
**             'dwLevel' - Level to set gain to
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function adjusts the gain of the hookswitch.
**
*****************************************************************************/
bool CDSDevice::DRV_SetGain (DWORD /*dwHookSwitch*/, DWORD dwLevel)
{                           
    EMLEVELCHANGE emChange;
    emChange.wLevelType = LEVELTYPE_MIC;
    emChange.wLevel = (WORD) dwLevel;
    return SendThreadRequest (EMCOMMAND_SETLEVEL, (LPVOID)&emChange, sizeof(EMLEVELCHANGE));
    
}// CDSDevice::DRV_SetGain

/*****************************************************************************
** Procedure:  CDSDevice::DRV_SetVolume
**
** Arguments:  'dwHookSwitch' - Hookswitch to set the volume for.
**             'dwLevel' - Level to set volume to
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function sets the volume of a hookswitch device
**
*****************************************************************************/
bool CDSDevice::DRV_SetVolume (DWORD /*dwHookSwitch*/, DWORD dwLevel)
{                           
    EMLEVELCHANGE emChange;
    emChange.wLevelType = LEVELTYPE_SPEAKER;
    emChange.wLevel = (WORD) dwLevel;
    return SendThreadRequest (EMCOMMAND_SETLEVEL, (LPVOID)&emChange, sizeof(EMLEVELCHANGE));
    
}// CDSDevice::DRV_SetVolume

/*****************************************************************************
** Procedure:  CDSDevice::DRV_SetHookswitch
**
** Arguments:  'dwHookSwitch' - Hookswitch to set the volume for.
**             'dwType' - State to set the hookswitch to
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function sets the state of the hookswitch device (on/off hook)
**
*****************************************************************************/
bool CDSDevice::DRV_SetHookswitch (DWORD /*dwHookSwitch*/, DWORD dwType)
{   
    EMHOOKSWITCHCHANGE emChange;
    emChange.wHookswitchID = HSDEVICE_HANDSET;
    switch (dwType)
    {
        case PHONEHOOKSWITCHMODE_ONHOOK: 
            emChange.wHookswitchState = HSSTATE_ONHOOK; 
            break;
        case PHONEHOOKSWITCHMODE_MICSPEAKER:
            emChange.wHookswitchState = HSSTATE_OFFHOOKMICSPEAKER;
            break;
        case PHONEHOOKSWITCHMODE_MIC:
            emChange.wHookswitchState = HSSTATE_OFFHOOKMIC;
            break;
        case PHONEHOOKSWITCHMODE_SPEAKER:
            emChange.wHookswitchState = HSSTATE_OFFHOOKSPEAKER;
            break;
        default:
            _TSP_ASSERT(FALSE);
            emChange.wHookswitchState = HSSTATE_OFFHOOKMICSPEAKER;
            break;
    }            
    return SendThreadRequest (EMCOMMAND_SETHOOKSWITCH, (LPVOID)&emChange, sizeof(EMHOOKSWITCHCHANGE));
    
}// CDSDevice::DRV_SetGain

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Conference
**
** Arguments:  'dwAddressID' - Starting address
**             'dwAddressID2' - Consult call
**             'wCommand' - Conference command
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function runs a conference call on the switch
**
*****************************************************************************/
bool CDSDevice::DRV_Conference(DWORD dwAddressID, DWORD dwAddressID2, WORD wCommand)
{
    EMCONFERENCEINFO ConfInfo;
    ConfInfo.wAddressID = (WORD) dwAddressID;
    ConfInfo.wConfAddress = (WORD) dwAddressID2;
    ConfInfo.wCommand = wCommand;
    return SendThreadRequest (EMCOMMAND_CONFERENCE, (LPVOID)&ConfInfo, sizeof(EMCONFERENCEINFO));

}// CDSDevice::DRV_Conference

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Redirect
**
** Arguments:  'dwAddressID' - Starting address
**             'lpszAddress' - Address to redirect to
**             'dwCountryCode' - Country code information
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function redirects the phone to another address
**
*****************************************************************************/
bool CDSDevice::DRV_Redirect (DWORD dwAddressID, LPCSTR lpszAddress, DWORD /*dwCountryCode*/)
{
    EMFORWARDINFO RedirectInfo;
    RedirectInfo.wAddressID = (WORD) dwAddressID;
    _TSP_ASSERTE(lpszAddress != NULL);
    lstrcpynA(RedirectInfo.szAddress, lpszAddress, ADDRESS_SIZE);
    return SendThreadRequest (EMCOMMAND_REDIRECT, (LPVOID)&RedirectInfo, sizeof(EMFORWARDINFO));

}// CDSDevice::DRV_Redirect

/*****************************************************************************
** Procedure:  CDSDevice::DRV_CompleteCall
**
** Arguments:  'dwAddressID' - Starting address
**             'dwComplType' - Completion type
**             'pszMsg' - Message to send
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function completes a waiting call on the switch
**
*****************************************************************************/
bool CDSDevice::DRV_CompleteCall (DWORD dwAddressID, DWORD dwComplType, LPCSTR pszMsg)
{                           
    EMCOMPLETECALL CCall;
    CCall.wAddressID = (WORD) dwAddressID;
    switch (dwComplType)
    {
        case LINECALLCOMPLMODE_CAMPON:     
            CCall.wCompletionType = CALLCOMPLTYPE_CAMP; 
            break;
        case LINECALLCOMPLMODE_CALLBACK:   
            CCall.wCompletionType = CALLCOMPLTYPE_CALLBACK; 
            break;
        case LINECALLCOMPLMODE_INTRUDE:    
            CCall.wCompletionType = CALLCOMPLTYPE_INTRUDE; 
            break;
        case LINECALLCOMPLMODE_MESSAGE:    
            CCall.wCompletionType = CALLCOMPLTYPE_MESSAGE; 
            break;
        default: 
            _TSP_ASSERT(FALSE);
            break;          
    }
    
    // Copy the message into the structure.
    if (pszMsg != NULL)
        lstrcpynA(CCall.szMessage, pszMsg, COMPLMESSAGE_SIZE);
    else
        CCall.szMessage[0] = '\0';
    return SendThreadRequest (EMCOMMAND_COMPLETECALL, (LPVOID)&CCall, sizeof(EMCOMPLETECALL));
    
}// CDSDevice::DRV_CompleteCall

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Park
**
** Arguments:  'dwAddressID' - Starting address
**             'pszAddr' - Address to park call to
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function parks the specified line to an address.
**
*****************************************************************************/
bool CDSDevice::DRV_Park (DWORD dwAddressID, LPCSTR pszAddr)
{                                                  
    EMPARKINFO ParkInfo;
    ParkInfo.wAddressID = (WORD) dwAddressID;
    _TSP_ASSERTE(pszAddr != NULL);
    lstrcpynA(ParkInfo.szAddress, pszAddr, ADDRESS_SIZE);
    return SendThreadRequest (EMCOMMAND_PARKCALL, (LPVOID)&ParkInfo, sizeof(EMPARKINFO));
    
}// CDSDevice::DRV_Park

/*****************************************************************************
** Procedure:  CDSDevice::DRV_Unpark
**
** Arguments:  'dwAddressID' - Starting address
**             'pszAddr' - Address to park call to
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function un-parks the specified line to an address.
**
*****************************************************************************/
bool CDSDevice::DRV_Unpark (DWORD dwAddressID, LPCSTR pszAddr)
{                                                  
    EMPARKINFO ParkInfo;
    ParkInfo.wAddressID = (WORD) dwAddressID;
    _TSP_ASSERTE(pszAddr != NULL);
    lstrcpynA(ParkInfo.szAddress, pszAddr, ADDRESS_SIZE);
    return SendThreadRequest (EMCOMMAND_UNPARKCALL, (LPVOID)&ParkInfo, sizeof(EMPARKINFO));
    
}// CDSDevice::DRV_Unpark

/*****************************************************************************
** Procedure:  CDSDevice::OpenDevice
**
** Arguments:  'pConn' - Connection object which is requesting open
**
** Returns:    TRUE/FALSE whether device opened
**
** Description:  This function is called by the CTSPIConnection object to
**               open the device when lineOpen or phoneOpen is invoked.
**
*****************************************************************************/
bool CDSDevice::OpenDevice (CTSPIConnection* /*pConn*/)
{
	return m_fConnected;

}// CDSDevice::OpenDevice

/*****************************************************************************
** Procedure:  CDSDevice::SendThreadRequest
**
** Arguments:  'uCommand' - Message to send (EMCOMMAND_xxx)
**             'pData'	  - Data buffer to send
**             'dwSize'	  - Size of data buffer
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function sends the emulator program a block of memory
**               using the inter-process WM_COPYDATA message.
**
*****************************************************************************/
bool CDSDevice::SendThreadRequest (UINT uCommand, LPVOID pData, DWORD dwSize)
{
	if (GetCurrentThreadId() == m_pThreadInput->m_nThreadID)
		((CSocketMgr*)m_pThreadInput)->SendMsg((DWORD)uCommand, pData, dwSize);
	else
	{
		CEvent evtComplete;
		evtComplete.ResetEvent();
		
		DATABLOCK db;
		db.dwCommand = (DWORD) uCommand;
		db.lpvData = pData;
		db.dwSize = dwSize;
		db.pevtWait = &evtComplete;
		db.lpvReturn = NULL;
		db.dwReturnSize = 0;

		if (!m_pThreadInput->PostThreadMessage(UM_SENDMESSAGE, 0, (LPARAM)&db))
			return FALSE;

		// Wait on our event to signal
		if (WaitForSingleObject(evtComplete, 5000) == WAIT_TIMEOUT)
			_TSP_ASSERT(FALSE);
	}

	return TRUE;

}// CDSDevice::SendThreadRequest

/*****************************************************************************
** Procedure:  CDSDevice::SendThreadRequest
**
** Arguments:  'uCommand' - Message to send (EMCOMMAND_xxx)
**             'dwData'   - inline data to send.
**
** Returns:    TRUE/FALSE success code
**
** Description:  This function sends the emulator program a request with a
**               single DWORD of data.  The command CANNOT have an immediate
**               inline response.
**
*****************************************************************************/
bool CDSDevice::SendThreadRequest (UINT uCommand, DWORD dwData)
{
	WORD wData = (WORD) dwData;
	return SendThreadRequest(uCommand, &wData, sizeof(WORD));

}// CDSDevice::SendThreadRequest

/*****************************************************************************
** Procedure:  CDSDevice::WaitForResponse
**
** Arguments:  'dwRequest' - Request to send
**             'lpBuff' - Buffer to return data in
**             'dwSize' - Size of buffer to fill
**
** Returns:    TRUE/FALSE whether received data
**
** Description:  This function waits on a EMRESPONSE packet from the
**               emulator when synchronous requests are needed.
**
*****************************************************************************/
bool CDSDevice::WaitForResponse(DWORD dwRequest, LPVOID lpSend, DWORD dwSendSize, LPVOID lpBuff, DWORD dwSize)
{
	_TSP_ASSERTE(GetCurrentThreadId() != m_pThreadInput->m_nThreadID);

	CEvent evtComplete;
	evtComplete.ResetEvent();
	
	DATABLOCK db;
	db.dwCommand = dwRequest;
	db.lpvData = lpSend;
	db.dwSize = dwSendSize;
	db.pevtWait = &evtComplete;
	db.lpvReturn = lpBuff;
	db.dwReturnSize = dwSize;

	// Send the request to the socket thread which will forward it onto the
	// emulator for action.
	if (!m_pThreadInput->PostThreadMessage(UM_SENDMESSAGE, 1, (LPARAM)&db))
		return FALSE;

	// Wait on our event to signal
	DWORD dwTimeout = (GetSP()->ReadProfileDWord(GetProviderID(), _T("IPTimeout"), 5) * 1000);
	if (WaitForSingleObject(evtComplete, dwTimeout) == WAIT_TIMEOUT)
		return FALSE;

	return TRUE;

}// CDSDevice::WaitForResponse

/*****************************************************************************
** Procedure:  CSocketMgr::OnSendMessage
**
** Arguments:  'wParam' - 0
**             'lParam' - Block to send
**
** Returns:    0
**
** Description:  This is used to send a message using the socket thread
**
*****************************************************************************/
void CSocketMgr::OnSendMessage(WPARAM wParam, LPARAM lParam)
{
	DATABLOCK* pDataBlock = (DATABLOCK*) lParam;
	if (wParam == 0)
	{
		SendMsg(pDataBlock->dwCommand, pDataBlock->lpvData, pDataBlock->dwSize);
		pDataBlock->pevtWait->SetEvent();
	}
	else
	{
		_TSP_ASSERTE(WaitBlock.lpBuff == NULL);
		WaitBlock.lpBuff = pDataBlock->lpvReturn;
		WaitBlock.dwSize = pDataBlock->dwReturnSize;
		WaitBlock.pevtDone = pDataBlock->pevtWait;
		SendMsg(pDataBlock->dwCommand, pDataBlock->lpvData, pDataBlock->dwSize);
	}

}// CSocketMgr::OnSendMessage
