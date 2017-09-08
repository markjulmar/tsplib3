/*******************************************************************/
//
// INTERFACE.CPP
//
// WinSock interface definitions
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
#include "document.h"
#include "Interface.h"
#include "base.h"

/*****************************************************************************
** Procedure:  _ConnThreadEntry
**
** Arguments: 'pConn' - Pointer to CConnection object
**
** Returns:    int
**
** Description:  Thread entry function
**
*****************************************************************************/
static DWORD WINAPI _ConnThreadEntry(LPVOID lpvParam)
{
	CConnection* pConn = reinterpret_cast<CConnection*>(lpvParam);
	pConn->ThreadEntry();
	return 0;

}// _ConnThreadEntry

/*****************************************************************************
** Procedure:  _CMListenEntry
**
** Arguments: 'pConn' - Pointer to CConnectionMgr object
**
** Returns:    int
**
** Description:  Thread entry function
**
*****************************************************************************/
static DWORD WINAPI _CMListenEntry(LPVOID lpvParam)
{
	CConnectionMgr* pMgr = reinterpret_cast<CConnectionMgr*>(lpvParam);
	pMgr->ListenEntry();
	return 0;

}// _CMListenEntry

/*****************************************************************************
** Procedure:  _CMThreadEntry
**
** Arguments: 'pConn' - Pointer to CConnectionMgr object
**
** Returns:    int
**
** Description:  Thread entry function
**
*****************************************************************************/
static DWORD WINAPI _CMThreadEntry(LPVOID lpvParam)
{
	CConnectionMgr* pMgr = reinterpret_cast<CConnectionMgr*>(lpvParam);
	pMgr->ThreadEntry();
	return 0;

}// _CMThreadEntry

/*****************************************************************************
** Procedure:  CConnection::CConnection
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the connection object
**
*****************************************************************************/
CConnection::CConnection(CConnectionMgr* pMgr) : 
	m_pMgr(pMgr), m_dwExtension(0), 
	m_hSocket(INVALID_SOCKET), m_hThread(0)
{
}// CConnection::CConnection

/*****************************************************************************
** Procedure:  CConnection::Close
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the connection object
**
*****************************************************************************/
CConnection::~CConnection()
{
	// Close the socket
	Close();

	// Ask the thread to terminate
	m_evtStop.SetEvent();

	// Wait for our thread to terminate
	if (m_hThread)
		if (WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hThread, 0);

}// CConnection::~CConnection

/*****************************************************************************
** Procedure:  CConnection::Close
**
** Arguments: void
**
** Returns:   void
**
** Description:  Receieve data from the socket
**
*****************************************************************************/
void CConnection::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

}// CConnection::Close

/*****************************************************************************
** Procedure:  CConnection::Create
**
** Arguments: void
**
** Returns:   bool result
**
** Description:  Creates our socket for sending/receiving data
**
*****************************************************************************/
bool CConnection::Create()
{
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	return (m_hSocket != INVALID_SOCKET);

}// CConnection::Create

/*****************************************************************************
** Procedure:  CConnection::Connect
**
** Arguments: 'pszHostAddress' - Address to connect to
**            'nPort' - Port to use
**
** Returns:   bool result
**
** Description:  Connects to a socket destination
**
*****************************************************************************/
bool CConnection::Connect(LPCSTR pszHostAddress, int nPort)
{
	// Create the socket if necessary
	if (m_hSocket == INVALID_SOCKET)
		Create();

	// Determine if the address is in dotted notation
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)nPort);
	sockAddr.sin_addr.s_addr = inet_addr(pszHostAddress);

	// If the address is not dotted notation, then do a DNS 
	// lookup of it.
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(pszHostAddress);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL); 
			return false;
		}
	}

	// Connect the socket
	if (connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != SOCKET_ERROR)
	{
		DWORD dwThreadID;
		m_hThread = CreateThread(NULL, 0, _ConnThreadEntry, static_cast<void*>(this), 0, &dwThreadID);
		if (m_hThread != NULL)
			return true;
		Close();
	}
	
	return false;

}// CConnection::Connect

/*****************************************************************************
** Procedure:  CConnection::OnConnect
**
** Arguments: 'sockParent' - Parent which is doing the listen
**
** Returns:   bool result
**
** Description:  Connects to a socket destination
**
*****************************************************************************/
bool CConnection::OnConnect(SOCKET sockParent)
{
	// This will block
	m_hSocket = accept(sockParent, NULL, NULL);
	if (m_hSocket != INVALID_SOCKET)
	{
		DWORD dwThreadID;
		m_hThread = CreateThread(NULL, 0, _ConnThreadEntry, static_cast<void*>(this), 0, &dwThreadID);
		if (m_hThread != NULL)
			return true;
		Close();
	}
	return false;

}// CConnection::OnConnect

/*****************************************************************************
** Procedure:  CConnection::Send
**
** Arguments: 'pszBuff' - Buffer to send
**            'nBufSize' - Buffer size
**
** Returns:   bool result
**
** Description:  Send buffer through the socket
**
*****************************************************************************/
bool CConnection::Send(LPCSTR pszBuff, int nBufSize)
{
	if (m_hSocket != INVALID_SOCKET)
	{
		// Ignore if this was already sent on this connection
		if (lstrcmpi(m_strLast,pszBuff))
		{
			m_strLast = pszBuff;
			if (nBufSize == 0)
				nBufSize = lstrlen(pszBuff);
			return (send(m_hSocket, pszBuff, nBufSize, 0) != SOCKET_ERROR);
		}
	}
	return false;

}// CConnection::Send

/*****************************************************************************
** Procedure:  CConnection::Recieve
**
** Arguments: 'pszBuff' - Buffer to recieve into
**            'nBufSize' - Buffer size
**
** Returns:   count of bytes read
**
** Description:  Receieve data from the socket
**
*****************************************************************************/
int CConnection::Receive(LPSTR pszBuff, int nBufSize)
{
	if (m_hSocket != INVALID_SOCKET)
	{
		int iCount = recv(m_hSocket, pszBuff, nBufSize, 0); 
		m_strLast = _T("");
		return iCount;
	}
	return 0;

}// CConnection::Recieve

/*****************************************************************************
** Procedure:  CConnection::HasData
**
** Arguments: void
**
** Returns:   true/false whether the socket has data
**
** Description:  Determine wether there is data on the socket
**
*****************************************************************************/
bool CConnection::HasData()
{
	timeval timeout = { 0, 0 };

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_hSocket, &fds);

	// Get the status of the socket
	int nStatus = select(0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
		return FALSE;
	return (!(nStatus == 0));

}// CConnection::HasData

/*****************************************************************************
** Procedure:  CConnection::ThreadEntry
**
** Arguments: void
**
** Returns:    void
**
** Description:  "Runner" for the thread
**
*****************************************************************************/
void CConnection::ThreadEntry()
{
	CSingleLock lck(&m_evtStop);
	CString strBuff;
	char c;

	while (!lck.Lock(0))
	{
		// Block on the read event
		int iRecv = Receive(&c, 1);
		if (iRecv == 1)
		{
			if (c == '\r')
			{
				if (!strBuff.IsEmpty())
				{
					CString strLine;
					strLine.Format(_T("%8x%s"), reinterpret_cast<DWORD>(this), strBuff);
					m_pMgr->OnReceiveData(this, strLine);
					strBuff = _T("");
				}
			}
			else if (isprint(c))
				strBuff += c;
		}
		else if (iRecv == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
				m_pMgr->OnConnClose(this);
		}
		else if (iRecv == 0)
		{
			m_pMgr->OnConnClose(this);
		}
	}

}// CConnection::ThreadEntry

/*****************************************************************************
** Procedure:  CConnectionMgr::CConnectionMgr
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the connection manager
**
*****************************************************************************/
CConnectionMgr::CConnectionMgr(CJTDocument* pDoc) : 
	m_pDocument(pDoc), m_hThread(0), m_hListenThread(0), 
	m_hSocket(INVALID_SOCKET)
{
	// Create our listening thread
	DWORD dwThreadID;
	m_hListenThread = CreateThread(NULL, 0, _CMListenEntry, static_cast<void*>(this), 0, &dwThreadID);

	// Create our runner thread
	m_hThread = CreateThread(NULL, 0, _CMThreadEntry, static_cast<void*>(this), 0, &dwThreadID);

	// Clear the list and set our initial log.
	m_pDocument->ClearLog();
	m_pDocument->AppendToLog(LT_INFO, IDS_LOG_STARTING);

}// CConnectionMgr::CConnectionMgr
	
/*****************************************************************************
** Procedure:  CConnectionMgr::~CConnectionMgr
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the connection manager
**
*****************************************************************************/
CConnectionMgr::~CConnectionMgr()
{
	// Set our shutdown event
	m_evtShutdown.SetEvent();

	// Wait for all the threads to stop
	if (WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
		TerminateThread(m_hThread, 0);
	if (WaitForSingleObject(m_hListenThread, 5000) == WAIT_TIMEOUT)
		TerminateThread(m_hListenThread, 0);

}// CConnectionMgr::~CConnectionMgr

/*****************************************************************************
** Procedure:  CConnectionMgr::SendData
**
** Arguments: 'dwExtension' - Extension to send the data from
**            'strBuff' - Data we are sending
**
** Returns:    Success result
**
** Description:  Called to send data through the sockets
**
*****************************************************************************/
bool CConnectionMgr::SendData(DWORD dwExtension, CString& strBuff)
{
	// Put it into out buffer
	m_pDocument->AppendToLog(LT_SENT, strBuff);

	// Add a CR to the line
	strBuff += "\r\n";

	// Walk through our connections and send the data
	CSingleLock lock (&m_csConn);
	for (int i = 0 ; i < m_arrConnections.GetSize(); i++)
	{
		CConnection* pConn = m_arrConnections[i];
		if (pConn->GetExtension() == dwExtension ||
			pConn->GetExtension() == 0xffff ||
			dwExtension == LID_ALL)
		{
			pConn->Send(strBuff, strBuff.GetLength());
		}
	}
	return true;

}// CConnectionMgr::SendData

/*****************************************************************************
** Procedure:  CConnectionMgr::SendData
**
** Arguments: 'pCall' - Call object which has changed
**            'strBuff' - Data we are sending
**
** Returns:    Success result
**
** Description:  Called to send data through the sockets
**
*****************************************************************************/
bool CConnectionMgr::SendData(bool fAll, CCall* pCall, CString& strBuff)
{
	DWORD dwLineID1 = pCall->GetLineOwner()->GetLineID();
	DWORD dwLineID2 = 0;
	if (fAll && pCall->GetAttachedCall() != NULL)
	{
		CLineDevice* pLine = pCall->GetAttachedCall()->GetLineOwner();
		if (pLine != NULL)
			dwLineID2 = pLine->GetLineID();
	}

	// Put it into out buffer
	m_pDocument->AppendToLog(LT_SENT, strBuff);

	// Add a CR to the line
	strBuff += _T("\r\n");

	// Walk through our connections and send the data
	CSingleLock lock (&m_csConn);
	for (int i = 0 ; i < m_arrConnections.GetSize(); i++)
	{
		CConnection* pConn = m_arrConnections[i];
		if (pConn->GetExtension() == 0xffff ||
			pConn->GetExtension() == dwLineID1 ||
			(dwLineID2 != 0 && pConn->GetExtension() == dwLineID2))
		{
			pConn->Send(strBuff, strBuff.GetLength());
		}
	}
	return true;

}// CConnectionMgr::SendData

/*****************************************************************************
** Procedure:  CConnectionMgr::OnReceiveData
**
** Arguments: 'pConn' - Connection data was received from
**            'strBuff' - Data we are receving
**
** Returns:    void
**
** Description:  Called by connections to buffer up received data
**
*****************************************************************************/
void CConnectionMgr::OnReceiveData(CConnection* pConn, CString& strBuff)
{
	// Treat the logon specially
	if (strBuff.Left(2) == PBX_COMMAND_LOGON)
	{
		DWORD dwExtension = atol(strBuff.Mid(3,4));
		if (dwExtension > 0 && dwExtension < 10000)
			pConn->SetExtension(dwExtension * -1);
	}

	// Add the buffer to our queue
	CSingleLock lck(&m_csLock, TRUE);
	m_arrInput.Add(strBuff);
	lck.Unlock();
	m_evtReady.SetEvent();

}// CConnectionMgr::OnReceiveData

/*****************************************************************************
** Procedure:  CConnectionMgr::OnConnClose
**
** Arguments: 'pConn' - Connection which is closing
**
** Returns:    void
**
** Description:  Called by connections to close itself
**
*****************************************************************************/
void CConnectionMgr::OnConnClose(CConnection* pConn)
{
	// Walk through our connections and delete the now-invalid one.
	CSingleLock lock (&m_csConn);
	for (int i = 0 ; i < m_arrConnections.GetSize(); i++)
	{
		CConnection* pConn2 = m_arrConnections[i];
		if (pConn == pConn2)
		{
			m_arrConnections.RemoveAt(i);
			break;
		}
	}
	delete pConn;
	m_pDocument->UpdateAllViews(NULL, HINT_CONNCHANGE);

}// CConnectionMgr::OnConnClose

/*****************************************************************************
** Procedure:  CConnectionMgr::ThreadEntry
**
** Arguments: void
**
** Returns:    void
**
** Description:  Runner thread
**
*****************************************************************************/
void CConnectionMgr::ThreadEntry()
{
	HANDLE arrEvents[] = { (HANDLE)m_evtShutdown, (HANDLE)m_evtReady };
	CSingleLock lck(&m_csLock);
	CString strBuff;
	
	while (TRUE)
	{
		DWORD dwUnlock = WaitForMultipleObjects(2, arrEvents, FALSE, INFINITE);
		if (dwUnlock == WAIT_OBJECT_0 || dwUnlock == WAIT_ABANDONED_0)
			return;

		// Get a single string and process it.
		lck.Lock();
		if (m_arrInput.GetSize() > 0)
		{
			strBuff = m_arrInput[0];
			m_arrInput.RemoveAt(0);
			if (m_arrInput.GetSize() > 0)
				m_evtReady.SetEvent();
		}
		lck.Unlock();

		// If we have data, process it on this thread.
		if (!strBuff.IsEmpty())
		{
			try
			{
				// Locate the connection
				CConnection* pConn = NULL;
				sscanf(strBuff.Left(8), _T("%x"), &pConn);
				strBuff = strBuff.Mid(8);
				if (pConn != NULL && pConn->IsValid())
				{
					// Create a parser block
					COMMANDBLOCK* pCB = COMMANDBLOCK::Create(pConn, strBuff, reinterpret_cast<LPVOID>(pConn));
					AfxGetMainWnd()->SendMessage(UM_PROCDATA, 0, reinterpret_cast<LPARAM>(pCB));
				}
			}
			catch(...)
			{
			}
		}
	}

}// CConnectionMgr::ThreadEntry

/*****************************************************************************
** Procedure:  CConnectionMgr::Ack
**
** Arguments: 'pCB' - Command block
**            'dwError' - Error code (0 success)
**
** Returns:    void
**
** Description:  Responds with ACK/NAK for given command
**
*****************************************************************************/
void CConnectionMgr::Ack(COMMANDBLOCK* pCB, DWORD dwError)
{
	CConnection* pConn = reinterpret_cast<CConnection*>(pCB->GetItemData());
	if (!IsValid(pConn))
		return;

	// If this is a LOGON response, mark the extension which this
	// connection is now logged onto.
	CString strBuff = pCB->GetCommand();
	if (!strBuff.Left(2).CompareNoCase(_T("LO")) && dwError == 0)
		pConn->SetExtension(pCB->GetExtension());

	CString strReturn = pCB->GetReturnString();
	Ack(pConn, pCB->GetExtension(), strBuff, dwError, strReturn);

}// CConnectionMgr::Ack

/*****************************************************************************
** Procedure:  CConnectionMgr::Ack
**
** Arguments: 'pConn' - Connection
**            'strBuff' - String buffer
**            'dwError' - Error code (0 success)
**
** Returns:    void
**
** Description:  Responds with ACK/NAK for given command
**
*****************************************************************************/
void CConnectionMgr::Ack(CConnection* pConn, DWORD dwExtension, CString& strBuff, DWORD dwError, CString& strReturn)
{
	// Strip off the command if more was given
	int iPos = strBuff.Find(_T(';'));
	if (iPos > 0)
		strBuff = strBuff.Left(iPos);

	if (dwExtension == 0)
		dwExtension = pConn->GetExtension();

	// Send the proper response
	CString strResponse;
	if (dwError == 0)
	{
		strResponse.Format(_T("ACK;%s;%04ld;0;%s"), 
			strBuff, dwExtension, strReturn);
	}
	else
	{
		dwError &= 0xffff;
		strResponse.Format(_T("NAK;%s;%04ld;%04lx"), 
			strBuff, dwExtension, dwError);
	}

	// Add the response to our buffer
	m_pDocument->AppendToLog(LT_SENT, strResponse);

	strResponse.MakeUpper();
	strResponse += _T("\r\n");

	// Send the response to the waiting connection
	if (IsValid(pConn))
		pConn->Send(strResponse, strResponse.GetLength());

	// If this is a failed logon, close the socket.
	if (!strBuff.Left(2).CompareNoCase(_T("LO")) && dwError != 0)
		OnConnClose(pConn);

}// CConnectionMgr::Ack

/*****************************************************************************
** Procedure:  CConnectionMgr::IsValid
**
** Arguments: Pointer to CConnection
**
** Returns:    true/false
**
** Description:  Validates the Connection pointer
**
*****************************************************************************/
bool CConnectionMgr::IsValid(CConnection* pConn)
{
	CSingleLock lock (&m_csConn);
	for (int i = 0 ; i < m_arrConnections.GetSize(); i++)
	{
		CConnection* pConn2 = m_arrConnections[i];
		if (pConn == pConn2)
			return true;
	}
	return false;

}// CConnectionMgr::IsValid

/*****************************************************************************
** Procedure:  CConnectionMgr::ListenEntry
**
** Arguments: void
**
** Returns:    void
**
** Description:  Listen thread
**
*****************************************************************************/
void CConnectionMgr::ListenEntry()
{
	struct sockaddr_in local;
	local.sin_family = AF_INET; 
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(m_pDocument->GetIPPort()); 

	// Create our listening socket
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// Bind the socket
	if (bind(m_hSocket,(struct sockaddr*)&local,sizeof(local)) == SOCKET_ERROR)
	{
		m_pDocument->AppendToLog(LT_INFO, IDS_ERRORBIND);
		return;
	}

	// Now perform a listen
	listen(m_hSocket, 4);

	// Wait for an accept
	CSingleLock lck(&m_evtShutdown);
	while (!lck.Lock(0))
	{
		CConnection* pConn = new CConnection(this);
		if (pConn->OnConnect(m_hSocket))
		{
			CSingleLock lock (&m_csConn);
			m_arrConnections.Add(pConn);
			if (AfxGetMainWnd() != NULL)
				AfxGetMainWnd()->PostMessage(UM_UPDATESTATS);
		}
		else
			delete pConn;
	}

	// Delete all our connections
	CSingleLock lock (&m_csConn);
	for (int i = 0 ; i < m_arrConnections.GetSize(); i++)
	{
		CConnection* pConn = m_arrConnections[i];
		delete pConn;
	}
	m_arrConnections.RemoveAll();

	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;

}// CConnectionMgr::ListenEntry

/*****************************************************************************
** Procedure:  SendEvent
**
** Arguments: 'dwLineID' - Extension id
**            'pszEvent' - Event wsprintf string
**            '...' - Arguments
**
** Returns:    void
**
** Description:  Sends a block of data to one or more connections
**
*****************************************************************************/
bool _cdecl SendEvent(DWORD dwLineID, LPCTSTR pszEvent, ...)
{
	va_list args;
	CString strEvent;
	char* pszBuffer = strEvent.GetBuffer(512);

	// Formate the string
	va_start(args, pszEvent);
	int rc = vsprintf(pszBuffer, pszEvent, args);
	va_end(args);
	strEvent.ReleaseBuffer();

	// Pass it through the connection manager
	CUIObject::GetDocument()->GetConnectionMgr().SendData(dwLineID, strEvent);
	return true;

}// CConnectionIntf::SendEvent

/*****************************************************************************
** Procedure:  SendEvent
**
** Arguments: 'pCall' - Call object
**            'pszEvent' - Event wsprintf string
**            '...' - Arguments
**
** Returns:    void
**
** Description:  Sends a block of data to one or more connections
**
*****************************************************************************/
bool _cdecl SendEvent(bool fAll, CCall* pCall, LPCTSTR pszEvent, ...)
{
	va_list args;
	CString strEvent;
	char* pszBuffer = strEvent.GetBuffer(512);

	// Formate the string
	va_start(args, pszEvent);
	int rc = vsprintf(pszBuffer, pszEvent, args);
	va_end(args);
	strEvent.ReleaseBuffer();

	// Pass it through the connection manager
	CUIObject::GetDocument()->GetConnectionMgr().SendData(fAll, pCall, strEvent);
	return true;

}// CConnectionIntf::SendEvent
