/*******************************************************************/
//
// INTERFACE.H
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_INTF_INC__
#define __JPBX_INTF_INC__

/*----------------------------------------------------------------------------
	COMMAND VERSIONS
-----------------------------------------------------------------------------*/
#define PBXVER _T("V3.00.00")

/*----------------------------------------------------------------------------
	ERROR CODES
-----------------------------------------------------------------------------*/
typedef unsigned long PBXERROR;
#define PBXERR_SUCCESS		(0x00000000)	// Success
#define PBXERR_INVALDEV		(0x80000001)	// Invalid device identifier
#define PBXERR_INVALDN		(0x80000002)	// Invalid extension
#define PBXERR_INVALPARAM   (0x80000003)	// Invalid parameter
#define PBXERR_INVALCALLID	(0x80000004)	// Invalid callid
#define PBXERR_RESUNAVAIL	(0x80000005)	// Resource unavailable
#define PBXERR_BADCRC       (0x80000006)	// Bad CRC
#define PBXERR_BADCOMMAND   (0x80000007)	// Bad command for this device
#define PBXERR_BADSTATE     (0x80000008)	// Device is in the improper state for command
#define PBXERR_INVALLOGON   (0x80000009)	// Not logged on
#define PBXERR_INVALAGENTID (0x8000000A)	// Bad agent id
#define PBXERR_INVALAGENTGROUP (0x8000000B) // Bad agent group
#define PBXERR_INVALSTATE   (0x8000000C)	// Incorrect agent state
#define PBXERR_FAILED		(0x8000FFFF)	// Unknown failure

/*----------------------------------------------------------------------------
	LINEIDS
-----------------------------------------------------------------------------*/
#define LID_ALL (0xffffffff)

/*----------------------------------------------------------------------------
	COMMANDS
-----------------------------------------------------------------------------*/
#define PBX_COMMAND_LOGON _T("LO")

/*----------------------------------------------------------------------------
	EVENTS
-----------------------------------------------------------------------------*/
typedef LPCSTR PBXEVTCODE;
// Call Detected:  CD, Extension, CallID, DNIS, ANI, CRC
#define PBXEVT_CD _T("CD;%s;%08X;%s;%s")
// Call Placed:    CP, Extension, CallID, DNIS, ANI, CRC
#define PBXEVT_CP _T("CP;%s;%08X;%s;%s")
// Call State:     CS, Extension, CallID, State, CRC
#define PBXEVT_CS _T("CS;%s;%08X;%c")
// Call Released:  CR, CallID, CRC
#define PBXEVT_CR _T("CR;%08X")
// Call Conference CC, Extension, CallID1, CallID2, CallID3, CallID4, CRC
#define PBXEVT_CC _T("CC;%s;%s")
// Call Transfer   CT, Extension, NCallID, OCallID, TargetExtension, DNIS, ANI
#define PBXEVT_CT _T("CT;%s;%08X;%08X;%s;%s;%s")
// Call Queued     CQ, Extension, CallID, QueueExtension
#define PBXEVT_CQ _T("CQ;%s;%08X;%s")
// Digit detected  DD, CallID, Digit
#define PBXEVT_DD _T("DD;%08X;%c")
// Call Media detected  CMD, CallID, New Media
#define PBXEVT_CMD _T("CMD;%08X;%c")
// Display changed      PDC, Extension, Display contents
#define PBXEVT_PDC _T("PDC;%s;%s")
// Volume changed       PVC, Extension, New Volume
#define PBXEVT_PVC _T("PVC;%s;%d")
// Gain changed         PGC, Extension, New Gain
#define PBXEVT_PGC _T("PGC;%s;%d")
// Hookswitch changed   PHC, Extension, New Value
#define PBXEVT_PHC _T("PHC;%s;%c")
// Lamp changed         PLC, Extension, LampID, 'S/D'
#define PBXEVT_PLC _T("PLC;%s;%d;%c")
// Agent State Changed  ASC, Extension, AgentID, Agent State
#define PBXEVT_ASC _T("ASC;%s;%s;%c")
// Agent Group Changed  AGC, Extension, AgentID, Group#1, Group#2
#define PBXEVT_AGC _T("AGC;%s;%s;%08lX;%08lX")

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CConnection;
class CConnectionMgr;
class CCall;

/**************************************************************************
** COMMANDBLOCK
**
** Command block passed between interface layer and processor
**
***************************************************************************/
class COMMANDBLOCK : public CObject
{
// Class data
protected:
	CString m_strCommand;		// Original command
	CString m_strReturn;		// Return value
	DWORD m_dwExtension;		// Extension
	LPVOID m_lpvData;			// Item data

// Destructor
public:
	virtual ~COMMANDBLOCK() { }

// Constructor
protected:
	COMMANDBLOCK(CString& strCommand, DWORD dwExtension, LPVOID lpvData) {
		m_strCommand = strCommand; m_lpvData = lpvData; m_dwExtension = dwExtension;
	}

// Factory
public:
	static COMMANDBLOCK* Create(CConnection* pConn, CString& strCommand, LPVOID lpvData=0);
	static bool StripCRC(CString& strData);
	static CString GetNextElem(CString& strData);

// Public methods
public:
	LPVOID GetItemData() const { return m_lpvData; }
	LPCTSTR GetCommand() const  { return m_strCommand; }
	DWORD GetExtension() const { return m_dwExtension; }
	void SetItemData(LPVOID lpv) { m_lpvData = lpv; }
	CString GetReturnString() const { return m_strReturn; }

// Overridable Methods
public:
	virtual PBXERROR Process(CJTDocument* pDoc) = 0;
};

/**************************************************************************
** CConnection
**
** This object represents a single connection to the PBX
**
***************************************************************************/
class CConnection : public CObject
{
// Class data
protected:
	SOCKET m_hSocket;			// Socket this connection is working with
	DWORD m_dwExtension;		// Item data for socket
	HANDLE m_hThread;			// Thread running input from socke
	CEvent m_evtStop;			// Stop evnt
	CConnectionMgr* m_pMgr;		// Owner connection manager
	CString m_strLast;			// Last string sent to socket

// Constructor
public:
	CConnection(CConnectionMgr* pMgr);
	~CConnection();

// Socket Methods
public:
	bool Create();
	bool Connect(LPCSTR pszHostAddress, int nPort);
	bool OnConnect(SOCKET sockParent);
	bool Send(LPCSTR pszBuf, int nBuf);
	void Close();
	int  Receive(LPSTR pszBuf, int nBuf);
	bool HasData();
	bool IsValid() const;

// Other methods
public:
	void SetExtension(DWORD dwExtension);
	DWORD GetExtension() const;
	void ThreadEntry();
};

/**************************************************************************
** CConnectionMgr
**
** This object manages all our connections, there should only be one
** of these objects in the project
**
***************************************************************************/
class CConnectionMgr
{
// Class data
protected:
	HANDLE m_hThread;			// Runner thread
	HANDLE m_hListenThread;		// Listening thread
	SOCKET m_hSocket;			// Listen socket
	CStringArray m_arrInput;	// Input from various sockets
	CEvent m_evtReady;			// Data read event
	CEvent m_evtShutdown;		// Event to shutdown
	CCriticalSection m_csLock;	// Critical section for our array
	CJTDocument* m_pDocument;	// Document pointer
	CCriticalSection m_csConn;	// Connection lock
	CTypedPtrArray<CObArray, CConnection*> m_arrConnections;

// Constructor
public:
	CConnectionMgr(CJTDocument* pDoc);
	~CConnectionMgr();

// Methods
public:
	bool SendData(DWORD dwExtension, CString& strBuff);
	bool SendData(bool fAll, CCall* pCall, CString& strBuff);
	void OnReceiveData(CConnection* pConn, CString& strBuff);
	void OnConnClose(CConnection* pConn);
	int GetConnectionCount() const;
	void ThreadEntry();
	void ListenEntry();
	void Ack(COMMANDBLOCK* pCB, DWORD dwError);
	bool IsValid(CConnection* pConn);

// Internal methods
protected:
	void Ack(CConnection* pConn, DWORD dwExtension, CString& strBuff, DWORD dwError, CString& strReturn);
};

/**************************************************************************
** SendEvent
**
** This allows access to the event stream
**
***************************************************************************/
bool _cdecl SendEvent (DWORD dwExtension, LPCTSTR pszEvent, ...);
bool _cdecl SendEvent (bool fAll, CCall* pCall, LPCTSTR pszEvent, ...);

/*****************************************************************************
** Procedure:  CConnection::IsValid
**
** Arguments: void
**
** Returns:   void
**
** Description:  TRUE/FALSE whether socket is valid
**
*****************************************************************************/
inline bool CConnection::IsValid() const
{
	return (m_hSocket != INVALID_SOCKET);

}// CConnection::IsValid

/*****************************************************************************
** Procedure:  CConnection::SetExtension
**
** Arguments: 'dwExtension' - Valid extension this socket is monitoring
**
** Returns:    void
**
** Description:  Sets the extension
**
*****************************************************************************/
inline void CConnection::SetExtension(DWORD dwExtension)
{
	m_dwExtension = dwExtension;

}// CConnection::SetExtension

/*****************************************************************************
** Procedure:  CConnection::GetExtension
**
** Arguments: void
**
** Returns:    DWORD extension stored by constructor
**
** Description:  Returns the itemdata which is the connection extension
**
*****************************************************************************/
inline DWORD CConnection::GetExtension() const
{
	return m_dwExtension;

}// CConnection::GetExtension

/*****************************************************************************
** Procedure:  CConnectionMgr::GetConnectionCount
**
** Arguments: void
**
** Returns:    Integer
**
** Description:  This returns the number of connections we have
**
*****************************************************************************/
inline int CConnectionMgr::GetConnectionCount() const
{
	return m_arrConnections.GetSize();

}// CConnectionMgr::GetConnectionCount

#endif // __JPBX_INTF_INC__
