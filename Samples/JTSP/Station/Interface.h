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
	ERRORS
-----------------------------------------------------------------------------*/
#define PBXERR_INVALDEV		(0x0001)	// Invalid device identifier
#define PBXERR_INVALDN		(0x0002)	// Invalid extension
#define PBXERR_INVALPARAM   (0x0003)	// Invalid parameter
#define PBXERR_INVALCALLID	(0x0004)	// Invalid callid
#define PBXERR_RESUNAVAIL	(0x0005)	// Resource unavailable
#define PBXERR_BADCRC       (0x0006)	// Bad CRC
#define PBXERR_BADCOMMAND   (0x0007)	// Bad command for this device
#define PBXERR_BADSTATE     (0x0008)	// Device is in the improper state for command
#define PBXERR_INVALLOGON   (0x0009)	// Not logged on
#define PBXERR_INVALAGENTID (0x000A)	// Bad agent id
#define PBXERR_INVALAGENTGROUP (0x000B) // Bad agent group
#define PBXERR_INVALSTATE   (0x000C)	// Incorrect agent state
#define PBXERR_FAILED		(0xFFFF)	// Unknown failure

/*----------------------------------------------------------------------------
	COMMANDS
-----------------------------------------------------------------------------*/
// LOGON - LO;EXTENSION
#define PBXCMD_LOGON _T("LO;%d")
// AGENT LOGON - ALO;EXTENSION;AGENTID;PASSWORD;GROUP1;GROUP2
#define PBXCMD_ALO _T("ALO;%d;%s;%s")
// AGENT STATE - CAS;EXTENSION;AGENTID;STATE
#define PBXCMD_CAS _T("CAS;%d;%s;%c")
// HOLD CALL - HC;EXTENSION;CALLID
#define PBXCMD_HC  _T("HC;%d;%x")
// RETRIEVE CALL - RC;EXTENSION;CALLID
#define PBXCMD_RTC  _T("RTC;%d;%x")
// GENERATE TONE/DIGIT - GTD;EXTENSION;CALLID;DIGIT
#define PBXCMD_GTD _T("GTD;%d;%x;%c")
// PLACE CALL - PC;EXTENSION;NUMBER
#define PBXCMD_PC  _T("PC;%d;%s")
// RELEASE CALL - RC;EXTENSION;CALLID
#define PBXCMD_RC  _T("RC;%d;%x")
// ANSWER CALL -  AN;EXTENSION;CALLID
#define PBXCMD_AN  _T("AN;%d;%x")
// HOLD CALL - HC;EXTENSION;CALLID
#define PBXCMD_HC  _T("HC;%d;%x")
// TRANSFER CALL - TC;EXTENSION;HCALLID;TCALLID
#define PBXCMD_TC  _T("TC;%d;%x;%x")

#define UM_GOTDATA   (WM_USER+1)
#define UM_CLOSECONN (WM_USER+2)

/**************************************************************************
** CMessage
**
** This object represents a message from the PBX
**
***************************************************************************/
class CMessage : public CString
{
// Methods
public:
	CMessage(const CString& str) : CString(str) {}

	void Skip() {
		int iPos = Find(_T(';'));
		if (iPos <= 0)
			Empty();
		else
			CString::operator=(Mid(iPos+1));
	}

	CString GetNextElem() {
		int iPos = Find(_T(';'));
		if (iPos == 0)
		{
			CString::operator=(Mid(iPos+1));
			return _T("");			
		}
		else if (iPos == -1)
			iPos = GetLength();
		
		CString strBuff = Left(iPos);
		if (iPos < GetLength())
			CString::operator=(Mid(iPos+1));
		else
			Empty();
		return strBuff;
	}
};

/**************************************************************************
** CConnection
**
** This object represents a single connection to the PBX
**
***************************************************************************/
class CConnection
{
// Class data
protected:
	SOCKET m_hSocket;			// Socket this connection is working with
	HANDLE m_hThread;			// Thread running input from socke
	CEvent m_evtStop;			// Stop evnt

// Constructor
public:
	CConnection(CString strIPAddress, int nPort);
	~CConnection();

// Socket Methods
public:
	bool Connect(LPCSTR pszHostAddress, int nPort);
	bool Send(LPCSTR pszBuf, int nBuf);
	void Close();
	int  Receive(LPSTR pszBuf, int nBuf);
	bool IsValid() const;
	bool HasData();
	bool _cdecl SendEvent(LPCTSTR pszEvent, ...);

// Other methods
public:
	void ThreadEntry();
};

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

#endif // __JPBX_INTF_INC__
