/*******************************************************************/
//
// CALL.CPP
//
// Call object definitions
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
#include "jpbx.h"
#include "call.h"
#include "line.h"
#include "trunk.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*****************************************************************************
** Procedure:  GetUniqueCallID
**
** Arguments: void
**
** Returns:   unique DWORD for callid
**
** Description: Returns unique callid
**
*****************************************************************************/
static DWORD GetUniqueCallID()
{
	static DWORD g_dwLastCallID = 0;
	DWORD dwCallID;

	do {
		dwCallID = GetTickCount();
	} while (dwCallID == g_dwLastCallID);

	g_dwLastCallID = dwCallID;
	return dwCallID;

}// GetUniqueCallID

/*****************************************************************************
** Procedure:  CCall::CCall
**
** Arguments: 'pHandle' - Master call handle
**            'pLine' - Line owner for this object
**            'cidInfo' - Callerid information
**
** Returns:   void
**
** Description: Constructor for the call object
**
*****************************************************************************/
CCall::CCall(CLineDevice* pLine, const CNetInfo& calleridInfo, 
			 const CNetInfo& calledInfo, bool fPlaceCall) : 
		m_pLine(pLine), m_calleridInfo(calleridInfo), 
		m_calledidInfo(calledInfo), m_csState(Unknown), m_pOther(0)
{
	m_dwCallID = GetUniqueCallID();

	// Notify the line owner
	m_pLine->OnCallCreated(this, fPlaceCall);

	// Mark the current time
	GetSystemTime(&m_stState);

}// CCall::CCall

/*****************************************************************************
** Procedure:  CCall::CCall
**
** Arguments: 'pLine' - Line owner for this object
**            'pCall' - Call to attach to
**
** Returns:   void
**
** Description: Constructor for the call object
**
*****************************************************************************/
CCall::CCall(CLineDevice* pLine, CCall& eCall) : 
	m_pLine(pLine), m_dwCallID(eCall.m_dwCallID), m_csState(Unknown),
	m_calleridInfo(eCall.m_calleridInfo), m_calledidInfo(eCall.m_calledidInfo),
	m_pOther(0)
{
	// Attach to the other call
	eCall.AttachCall(this);

	// Notify the line owner
	m_pLine->OnCallCreated(this, false);

	// Determine the initial call state based on the other call.
	OnChangeCallState(eCall.m_csState);
	CopyMemory(&m_stState, &eCall.m_stState, sizeof(SYSTEMTIME));

}// CCall::CCall

/*****************************************************************************
** Procedure:  CCall::~CCall
**
** Arguments: void
**
** Returns:   void
**
** Description: Destructor for the call object
**
*****************************************************************************/
CCall::~CCall()
{
	m_pLine->OnCallDestroyed(this, (m_dwCallID != 0));
	if (m_csState != Disconnected && m_pOther != NULL)
	{
		CCall* pOther = m_pOther;
		m_pOther = NULL;
		if (pOther != NULL)
		{
			pOther->m_pOther = NULL;
			pOther->m_dwCallID = 0;
			delete pOther;
		}
	}

}// CCall::~CCall

/*****************************************************************************
** Procedure:  CCall::Release
**
** Arguments: void
**
** Returns:    void
**
** Description: Drops the call
**
*****************************************************************************/
void CCall::Release()
{
	// If we have an attached call (other side), then show that
	// this side disconnected from the conversation.  This will
	// cause this side to self-destruct.
	CCall* pCall = GetAttachedCall();
	if (pCall != NULL)
		pCall->SetCallState(Disconnected);

	// Otherwise, simply hangup
	else
		delete this;

}// CCall::Release

/*****************************************************************************
** Procedure:  CCall::SetCallState
**
** Arguments: 'ncs' - New call state
**
** Returns:    true/false if state was set
**
** Description: Sets the call state
**
*****************************************************************************/
void CCall::SetCallState(CallState ncs, bool fNotifyOther)
{
	if (ncs != m_csState)
	{
		// Set the new callstate
		m_csState = ncs;

		// Mark the current time
		GetSystemTime(&m_stState);

		// Notify the line owner
		m_pLine->OnCallStateChange(this);

		// Notify the other side of the call - if THIS call
		// is being disconnected, then it will be deleted
		// with this notification.
		if (m_pOther != NULL && fNotifyOther)
			m_pOther->OnChangeCallState(ncs);
	}

}// CCall::SetCallState

/*****************************************************************************
** Procedure:  CCall::OnChangeCallState
**
** Arguments: 'ncs' - New call state
**
** Returns:   void
**
** Description: Event which is called when the other side of a call
**              changes state
**
*****************************************************************************/
void CCall::OnChangeCallState(CallState ncs)
{
	// If the call is now disconnected, delete it.
	if (ncs == Disconnected)
		delete this;
	else if (dynamic_cast<CTrunk*>(GetLineOwner()) == NULL)
	{
		if (GetCallState() != Holding)
		{
			if (ncs == Alerting)
				SetCallState(WaitForAnswer, false);
			else if (ncs == Connected)
				SetCallState(Connected, false);
			else if (ncs == Holding)
			{
				if (GetCallState() == Connected)
					SetCallState(OtherSideHolding, false);
			}
		}
	}

}// CCall::OnChangeCallState

/*****************************************************************************
** Procedure:  CCall::MoveToLine
**
** Arguments: 'pLine' - New line owner
**
** Returns:   void
**
** Description: Moves a call from one line to another
**
*****************************************************************************/
void CCall::MoveToLine(CLineDevice* pLine)
{
	if (pLine == GetLineOwner())
		return;

	// Remove from the original line
	m_pLine->OnCallDestroyed(this, false);

	// Now mark the new line
	m_pLine = pLine;

	// Notify the line owner
	m_pLine->OnCallCreated(this, false);

}// CCall::MoveToLine

/*****************************************************************************
** Procedure:  CCall::Merge
**
** Arguments: 'pCall' - Call to copy into this one
**
** Returns:   void
**
** Description: Replaces our call information (callerid, calledid)
**
*****************************************************************************/
void CCall::Merge(CCall* pCall)
{
	// We are about to be attached to this call
	if (pCall->GetAttachedCall() != NULL)
	{
		CCall* pOther = pCall->GetAttachedCall();
		pOther->m_dwCallID = 0;
		pOther->m_pOther = NULL;
		pCall->m_pOther = NULL;
	}

	if (m_pOther != NULL)
	{
		m_pOther->m_pOther = NULL;
		m_pOther = NULL;
	}

	// We get the callid and caller-info
	m_dwCallID = pCall->m_dwCallID;
	m_calleridInfo = pCall->m_calleridInfo;
	m_calledidInfo = pCall->m_calledidInfo;

	// Attach the two calls
	AttachCall(pCall);

}// CCall::Copy

/*****************************************************************************
** Procedure:  CCall::GetCallStateString
**
** Arguments: void
**
** Returns:   String of call state
**
** Description: Returns the call state
**
*****************************************************************************/
CString CCall::GetCallStateString(int iType) const
{
	UINT uiState[] = {
		IDS_CS_UNKNOWN,
		IDS_CS_DIALING,
		IDS_CS_RINGING,
		IDS_CS_ALERTING,
		IDS_CS_ALERTING,
		IDS_CS_CONNECTED,
		IDS_CS_BUSY,
		IDS_CS_DISCONNECTED,
		IDS_CS_HOLDING,
		IDS_CS_CONNECTED2,
		IDS_CS_QUEUED
	};

	if (iType == -1)
		iType = static_cast<int>(m_csState);

	CString strBuff;
	if (!strBuff.LoadString(uiState[iType]))
		strBuff.LoadString(IDS_CS_UNKNOWN);
	return strBuff;

}// CCall::GetCallStateString

