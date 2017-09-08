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
#define PBXERR_INVALDN		(0x0002)	// Invalid extension
#define PBXERR_RESUNAVAIL	(0x0005)	// Resource unavailable

/*----------------------------------------------------------------------------
	COMMANDS
-----------------------------------------------------------------------------*/
// TPC - Trunk Place Call; 0; Number to dial; CallerID
#define PBXCMD_TPC _T("TPC;0;%s;%s;%s")
// TRC - Trunk Release Call; 0; TrunkID; CallID;
#define PBXCMD_TRC _T("TRC;0;%s")

#define UM_GOTDATA (WM_USER+1)

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CConnectionMgr;

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
