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
#include "Interface.h"

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
** Procedure:  CConnection::CConnection
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the connection object
**
*****************************************************************************/
CConnection::CConnection(CString strIPAddress, int nPort) :
	m_hSocket(INVALID_SOCKET), m_hThread(0)
{
	// Create our socket to use
	Connect(strIPAddress, nPort);
			
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
	if (m_hThread && m_hThread != GetCurrentThread())
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
		m_hSocket = socket(AF_INET, SOCK_STREAM, 0);

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
	}

	Close();
	return false;

}// CConnection::Connect

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
	ASSERT(m_hSocket != INVALID_SOCKET);
	if (nBufSize == 0)
		nBufSize = lstrlen(pszBuff);
	return (send(m_hSocket, pszBuff, nBufSize, 0) != SOCKET_ERROR);

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
	if (m_hSocket == INVALID_SOCKET)
		return SOCKET_ERROR;
	return recv(m_hSocket, pszBuff, nBufSize, 0); 

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
					if (AfxGetMainWnd() != NULL)
						AfxGetMainWnd()->SendMessage(UM_GOTDATA, 0, reinterpret_cast<LPARAM>(&strBuff));
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
			{
				Close();
				if (AfxGetMainWnd())
					AfxGetMainWnd()->SendMessage(UM_CLOSECONN);
				return;
			}
		}
		else if (iRecv == 0)
		{
			Close();
			if (AfxGetMainWnd())
				AfxGetMainWnd()->SendMessage(UM_CLOSECONN);
			return;
		}
	}

}// CConnection::ThreadEntry

/*****************************************************************************
** Procedure:  SendEvent
**
** Arguments: 'pszEvent' - Event wsprintf string
**            '...' - Arguments
**
** Returns:    void
**
** Description:  Sends a block of data to one or more connections
**
*****************************************************************************/
bool _cdecl CConnection::SendEvent(LPCTSTR pszEvent, ...)
{
	va_list args;
	CString strEvent;
	char* pszBuffer = strEvent.GetBuffer(512);

	// Formate the string
	va_start(args, pszEvent);
	int rc = vsprintf(pszBuffer, pszEvent, args);
	va_end(args);
	strEvent.ReleaseBuffer();

	strEvent += _T("\r\n");

	// Pass it through the connection manager
	Send(strEvent, strEvent.GetLength());
	return true;

}// CConnection::SendEvent
