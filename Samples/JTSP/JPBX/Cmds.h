/*******************************************************************/
//
// CMDS.H
//
// COMMANDBLOCK derivatives
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

#ifndef __JPBX_CMDS_INC__
#define __JPBX_CMDS_INC__

/*----------------------------------------------------------------------------
	INCLUDES
-----------------------------------------------------------------------------*/
#include "interface.h"

/**************************************************************************
** COMMANDERROR
**
** Command block which is passed back when a bad CRC or command is found
**
***************************************************************************/
class COMMANDERROR : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwError;

// Constructor
public:
	COMMANDERROR(CString& strCommand, DWORD dwExtension, LPVOID lpvData, DWORD dwError) : 
	  COMMANDBLOCK(strCommand, dwExtension, lpvData) { m_dwError = dwError; }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument*) { return m_dwError; }
};

/**************************************************************************
** LOGON
**
** Command block which represents a logon command
**
***************************************************************************/
class LOGON : public COMMANDBLOCK
{
// Constructor
public:
	LOGON(CString& strCommand, DWORD dwExtension, LPVOID lpvData) : 
		COMMANDBLOCK(strCommand, dwExtension, lpvData) {}

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** VERSION
**
** Command block which represents a version command
**
***************************************************************************/
class VERSION : public COMMANDBLOCK
{
// Constructor
public:
	VERSION(CString& strCommand, DWORD dwExtension, LPVOID lpvData) : 
		COMMANDBLOCK(strCommand, dwExtension, lpvData) {}

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* /*pDoc*/) {
		m_strReturn = PBXVER;
		return 0;
	}
};

/**************************************************************************
** QUERYAGENTSTATE
**
** Command block which represents a query-agent-state command
**
***************************************************************************/
class QUERYAGENTSTATE : public COMMANDBLOCK
{
// Constructor
public:
	QUERYAGENTSTATE(CString& strCommand, DWORD dwExtension, LPVOID lpvData) : 
		COMMANDBLOCK(strCommand, dwExtension, lpvData) {}

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* /*pDoc*/);
};

/**************************************************************************
** PLACECALL
**
** Command block which represents a place call command
**
***************************************************************************/
class PLACECALL : public COMMANDBLOCK
{
// Class data
protected:
	CString m_strDestination;

// Constructor
public:
	PLACECALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
			m_strDestination = GetNextElem(strData);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** RELEASECALL
**
** Command block which represents a release call command
**
***************************************************************************/
class RELEASECALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;

// Constructor
public:
	RELEASECALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** ANSWERCALL
**
** Command block which represents an answer call command
**
***************************************************************************/
class ANSWERCALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;

// Constructor
public:
	ANSWERCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** ACCEPTCALL
**
** Command block which represents an accept call command
**
***************************************************************************/
class ACCEPTCALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;

// Constructor
public:
	ACCEPTCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** HOLDCALL
**
** Command block which represents a hold call command
**
***************************************************************************/
class HOLDCALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;

// Constructor
public:
	HOLDCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** RETRIEVECALL
**
** Command block which represents a retrieve call command
**
***************************************************************************/
class RETRIEVECALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;

// Constructor
public:
	RETRIEVECALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** AGENTLOGON
**
** Command block which represents an agent logon command
**
***************************************************************************/
class AGENTLOGON : public COMMANDBLOCK
{
// Class data
protected:
	CString m_strAgentID;
	CString m_strPassword;
	DWORD m_dwGroup1;
	DWORD m_dwGroup2;
	bool m_fGroupIsBlank;

// Constructor
public:
	AGENTLOGON(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  m_dwGroup1 = m_dwGroup2 = 0;
		  m_strAgentID = GetNextElem(strData);
		  m_strPassword = GetNextElem(strData);

		  CString strElem = GetNextElem(strData);
		  m_fGroupIsBlank = (strElem.IsEmpty() == TRUE);
		  if (!m_fGroupIsBlank)
		  {
			  sscanf(strElem, _T("%lx"), &m_dwGroup1);
			  sscanf(GetNextElem(strData), _T("%lx"), &m_dwGroup2);
		  }
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** AGENTSTATE
**
** Command block which represents an agent state change command
**
***************************************************************************/
class AGENTSTATE : public COMMANDBLOCK
{
// Class data
protected:
	CString m_strAgentID;
	CString m_strState;

// Constructor
public:
	AGENTSTATE(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  m_strAgentID = GetNextElem(strData);
		  m_strState = GetNextElem(strData);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** SETHOOKSWITCH
**
** Command block which represents a set hookswitch command
**
***************************************************************************/
class SETHOOKSWITCH : public COMMANDBLOCK
{
// Class data
protected:
	CString m_strState;

// Constructor
public:
	SETHOOKSWITCH(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  m_strState = GetNextElem(strData);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** SETVOLUME
**
** Command block which represents a set volume command
**
***************************************************************************/
class SETVOLUME : public COMMANDBLOCK
{
// Class data
protected:
	int m_iVolume;

// Constructor
public:
	SETVOLUME(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%x"), &m_iVolume);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** SETGAIN
**
** Command block which represents a set gain command
**
***************************************************************************/
class SETGAIN : public COMMANDBLOCK
{
// Class data
protected:
	int m_iGain;

// Constructor
public:
	SETGAIN(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%x"), &m_iGain);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** GENERATETD
**
** Command block which represents a generate tone/digit command
**
***************************************************************************/
class GENERATETD : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;
	CString m_strDigit;

// Constructor
public:
	GENERATETD(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
		  m_strDigit = GetNextElem(strData);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** PLACETCALL
**
** Command block which represents a place trunk call command
**
***************************************************************************/
class PLACETCALL : public COMMANDBLOCK
{
// Class data
protected:
	CString m_strDestination;
	CString m_strCallerName;
	CString m_strCallerNumber;

// Constructor
public:
	PLACETCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
			m_strDestination = GetNextElem(strData);
			m_strCallerName = GetNextElem(strData);
			m_strCallerNumber = GetNextElem(strData);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** RELEASETCALL
**
** Command block which represents a place trunk call command
**
***************************************************************************/
class RELEASETCALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwTrunkID;
	DWORD m_dwCallID;

// Constructor
public:
	RELEASETCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
  		  sscanf(GetNextElem(strData), _T("%ld"), &m_dwTrunkID);
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** BLINDTRANSFERCALL
**
** Command block which represents a transfer request
**
***************************************************************************/
class BLINDTRANSFERCALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;
	DWORD m_dwTarget;

// Constructor
public:
	BLINDTRANSFERCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
  		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
  		  sscanf(GetNextElem(strData), _T("%ld"), &m_dwTarget);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** TRANSFERCALL
**
** Command block which represents a transfer request
**
***************************************************************************/
class TRANSFERCALL : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID1;
	DWORD m_dwCallID2;

// Constructor
public:
	TRANSFERCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
  		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID1);
		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID2);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** GETCALLSTATS
**
** Command block which represents a GCS command
**
***************************************************************************/
class GETCALLSTATS : public COMMANDBLOCK
{
// Class data
protected:
	DWORD m_dwCallID;

// Constructor
public:
	GETCALLSTATS(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
  		  sscanf(GetNextElem(strData), _T("%lx"), &m_dwCallID);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** GETQUEUESTATS
**
** Command block which represents a GQS command
**
***************************************************************************/
class GETQUEUESTATS : public COMMANDBLOCK
{
// Constructor
public:
	GETQUEUESTATS(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& /*strData*/) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

/**************************************************************************
** PLACEPDCALL
**
** Command block which represents a place predictive dialer call
**
***************************************************************************/
class PLACEPDCALL : public COMMANDBLOCK
{
// Class data
protected:
	CString m_strNumber;
	DWORD m_dwTransferTo;
	int m_iTimeout;

// Constructor
public:
	PLACEPDCALL(CString& strInput, DWORD dwExtension, LPVOID lpvData, CString& strData) : 
	  COMMANDBLOCK(strInput, dwExtension, lpvData) {
			m_strNumber = GetNextElem(strData);
			m_iTimeout = atoi(GetNextElem(strData));
			sscanf(GetNextElem(strData), _T("%ld"), &m_dwTransferTo);
	  }

// Overriden functions
public:
	virtual PBXERROR Process(CJTDocument* pDoc);
};

#endif // __JPBX_CMDS_INC__
