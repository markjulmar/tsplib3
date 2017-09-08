/*******************************************************************/
//
// CALL.H
//
// Single call object
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

#ifndef __JPBX_CALL_INC__
#define __JPBX_CALL_INC__

/*---------------------------------------------------------------*/
// PRE-DEFINITIONS
/*---------------------------------------------------------------*/
class CLineDevice;

/**************************************************************************
** CNetInfo
**
** This object represents the network information representing callerid
**
***************************************************************************/
class CNetInfo
{
// Class data
protected:
	CString m_strName;		// Name of the caller
	CString m_strNumber;	// DN of the caller

// Constructor
public:
	CNetInfo (const char* pszName, const char* pszDN);
	CNetInfo (const CNetInfo& cid);
	const CNetInfo& operator= (const CNetInfo& cid);

// Methods
public:
	const char* GetName() const;
	const char* GetNumber() const;
};

/**************************************************************************
** CCall
**
** This object represents one side of a call (source or destination)
**
***************************************************************************/
class CCall : public CObject
{
// Class data
public:
	enum CallState {
		Unknown=0,			// Unknown
		Dialing,			// Dialing
		Ringing,			// Call is offering to a station
		Alerting,			// Station is offering tone for offering call
		WaitForAnswer,		// Station made call, ringing on other side.
		Connected,			// Call is connected
		Busy,				// Call is busy
		Disconnected,		// Call is disconnected
		Holding,			// This side is on hold
		OtherSideHolding,	// The other side is on hold
		Queued				// Call is queued
	};

protected:
	DWORD m_dwCallID;				// Unique callid for this call
	CallState m_csState;			// Current call state
	CLineDevice* m_pLine;			// Line owning this object
	CNetInfo m_calleridInfo;		// Caller ID information
	CNetInfo m_calledidInfo;		// Called ID information
	CCall* m_pOther;				// Other side of the conversation (if available)
	SYSTEMTIME m_stState;			// Time this call entered this state

// Constructor
public:
	CCall(CLineDevice* pLine, const CNetInfo& calleridInfo, const CNetInfo& calledidInfo, bool fPlaceCall=false);
	CCall(CLineDevice* pLine, CCall& ec);
	~CCall();

// Methods
public:
	DWORD GetCallID() const;
	const CNetInfo& GetCallerIDInfo() const;
	const CNetInfo& GetCalledIDInfo() const;
	CString GetANI() const;
	CString GetDNIS() const;
	CallState GetCallState() const;
	CString GetCallStateString(int iType=-1) const;
	void SetCallState(CallState ncs, bool fNotifyOther=true);
	CLineDevice* GetLineOwner();
	CCall* GetAttachedCall();
	bool IsActiveState() const;
	SYSTEMTIME& GetStateTime();
	int SecondsInState() const;
	void Release();

// Internal methods
public:
	void AttachCall(CCall* pOther);
	void OnChangeCallState(CallState ncs);
	void MoveToLine(CLineDevice* pLine);
	void Merge(CCall* pCall);
};

/*****************************************************************************
** Procedure:  CNetInfo::CNetInfo
**
** Arguments: 'pszName' - Name
**            'pszNumber' - DN
**
** Returns:    void
**
** Description: Constructor for the callerid structure
**
*****************************************************************************/
inline CNetInfo::CNetInfo (const char* pszName, const char* pszDN) : 
	m_strName(pszName)
{
	while (*pszDN)
	{
		if (isdigit(*pszDN)) m_strNumber += *pszDN;
		pszDN++;
	}

}// CNetInfo::CNetInfo

/*****************************************************************************
** Procedure:  CNetInfo::CNetInfo
**
** Arguments: 'cid' - Reference to another CNetInfo
**
** Returns:    void
**
** Description: Copy constructor
**
*****************************************************************************/
inline CNetInfo::CNetInfo (const CNetInfo& cid)
{
	m_strName = cid.m_strName;
	m_strNumber = cid.m_strNumber;

}// CNetInfo::CNetInfo

/*****************************************************************************
** Procedure:  CNetInfo::operator=
**
** Arguments: 'cid' - Reference to another CNetInfo
**
** Returns:    void
**
** Description: Assignment operator
**
*****************************************************************************/
inline const CNetInfo& CNetInfo::operator= (const CNetInfo& cid)
{
	m_strName = cid.m_strName;
	m_strNumber = cid.m_strNumber;
	return *this;

}// CNetInfo::operator=

/*****************************************************************************
** Procedure:  CNetInfo::GetName
**
** Arguments: void
**
** Returns:    Caller name
**
** Description: Returns the caller name
**
*****************************************************************************/
inline const char* CNetInfo::GetName() const
{
	return m_strName;

}// CNetInfo::GetName

/*****************************************************************************
** Procedure:  CNetInfo::GetNumber
**
** Arguments: void
**
** Returns:    Caller number
**
** Description: Returns the caller number
**
*****************************************************************************/
inline const char* CNetInfo::GetNumber() const
{
	return m_strNumber;

}// CNetInfo::GetNumber

/*****************************************************************************
** Procedure:  CCall::GetCallerIDInfo
**
** Arguments: void
**
** Returns:    Reference to CallerID structure
**
** Description: Returns the callerid information
**
*****************************************************************************/
inline const CNetInfo& CCall::GetCallerIDInfo() const
{
	return m_calleridInfo;

}// CCall::GetCallerIDInfo

/*****************************************************************************
** Procedure:  CCall::GetCalledIDInfo
**
** Arguments: void
**
** Returns:    Reference to CallerID structure
**
** Description: Returns the callerid information
**
*****************************************************************************/
inline const CNetInfo& CCall::GetCalledIDInfo() const
{
	return m_calledidInfo;

}// CCall::GetCalledIDInfo

/*****************************************************************************
** Procedure:  CCall::GetCallState
**
** Arguments: void
**
** Returns:    CallState constant
**
** Description: Returns the call's callstate
**
*****************************************************************************/
inline CCall::CallState CCall::GetCallState() const
{
	return m_csState;

}// CCall::GetCallState

/*****************************************************************************
** Procedure:  CCall::GetCallID
**
** Arguments: void
**
** Returns:    CALLID
**
** Description: Returns the call's identifier
**
*****************************************************************************/
inline DWORD CCall::GetCallID() const
{
	return m_dwCallID;

}// CCall::GetCallID

/*****************************************************************************
** Procedure:  CCall::AttachCall
**
** Arguments: 'pOther' - Other side of the call
**
** Returns:    void
**
** Description: Attaches two calls together
**
*****************************************************************************/
inline void CCall::AttachCall(CCall* pOther)
{
	// Skip if already set
	if (m_pOther == pOther)
		return;

	// Disconnect from previous other call
	if (m_pOther != NULL &&
		m_pOther->m_pOther == this)
		m_pOther->m_pOther = NULL;
	
	// Connect to new other call
	m_pOther = pOther;
	if (m_pOther != NULL)
		m_pOther->AttachCall(this);

}// CCall::AttachCall

/*****************************************************************************
** Procedure:  CCall::GetLineOwner
**
** Arguments: void
**
** Returns:    Line pointer
**
** Description: Returns the line owner for a call object
**
*****************************************************************************/
inline CLineDevice* CCall::GetLineOwner()
{
	return m_pLine;

}// CCall::GetLineOwner

/*****************************************************************************
** Procedure:  CCall::GetAttachedCall
**
** Arguments: void
**
** Returns:    Attached call pointer
**
** Description: Returns the attached call (shadow)
**
*****************************************************************************/
inline CCall* CCall::GetAttachedCall()
{
	return m_pOther;

}// CCall::GetAttachedCall

/*****************************************************************************
** Procedure:  CCall::GetANI
**
** Arguments: void
**
** Returns:    String with ANI information
**
** Description: Returns the available ANI information (if any)
**
*****************************************************************************/
inline CString CCall::GetANI() const
{
	CString strBuff;
	strBuff.Format(_T("%s:%s"), m_calledidInfo.GetName(), m_calledidInfo.GetNumber());
	if (strBuff.GetLength() == 1)
		strBuff.Empty();
	return strBuff;

}// CCall::GetANI

/*****************************************************************************
** Procedure:  CCall::GetDNIS
**
** Arguments: void
**
** Returns:    String with DNIS information
**
** Description: Returns the available DNIS information (if any)
**
*****************************************************************************/
inline CString CCall::GetDNIS() const
{
	CString strBuff;
	strBuff.Format(_T("%s:%s"), m_calleridInfo.GetName(), m_calleridInfo.GetNumber());
	if (strBuff.GetLength() == 1)
		strBuff.Empty();
	return strBuff;

}// CCall::GetDNIS

/*****************************************************************************
** Procedure:  CCall::GetStateTime
**
** Arguments: void
**
** Returns:   SYSTEMTIME that this call entered the current state
**
** Description: Returns the time this call last changed.
**
*****************************************************************************/
inline SYSTEMTIME& CCall::GetStateTime()
{
	return m_stState;

}// CCall::GetStateTime

/*****************************************************************************
** Procedure:  CCall::SecondsInState
**
** Arguments: void
**
** Returns:   Number of seconds the call has been in the current state
**
** Description: Returns the second state counter
**
*****************************************************************************/
inline int CCall::SecondsInState() const
{
	SYSTEMTIME stTime;
	GetSystemTime(&stTime);
	CTime tmCurr(stTime), tmPrev(m_stState);
	CTimeSpan ts = tmCurr - tmPrev;
	return ts.GetSeconds();

}// CCall::SecondsInState

/*****************************************************************************
** Procedure:  CCall::IsActiveState
**
** Arguments: void
**
** Returns:   true/false if this call is taking up bandwidth
**
** Description: Returns whether this call is taking up bandwidth
**
*****************************************************************************/
inline bool CCall::IsActiveState() const
{
	if (m_csState == Dialing ||
		m_csState == WaitForAnswer ||
		m_csState == Connected ||
		m_csState == OtherSideHolding)
		return true;
	return false;

}// CCall::IsActiveState

#endif // __JPBX_CALL_INC__
