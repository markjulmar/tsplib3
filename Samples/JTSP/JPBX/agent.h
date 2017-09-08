/*******************************************************************/
//
// AGENT.H
//
// Agent management objects and dialogs
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

#ifndef __JPBX_AGENT_INC__
#define __JPBX_AGENT_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "base.h"

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
const UINT AGENTID_SIZE = 25;
const UINT AGENTPW_SIZE = 9;

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CStation;

/**************************************************************************
** CAgent
**
** This object represents a single agent to the PBX/ACD simulator.
**
***************************************************************************/
class CAgent : public CUIObject
{
// Class data
public:
	enum AgentState { 
			Unknown = 0,		// Agent state not known
			SignedOut,			// Agent is not signed in to any address
			Ready,				// Agent is ready to receive a call	
			NotReady,			// Agent is not ready
			InCallWork,			// Agent is busy performing after call work
			BusyACD,			// Agent is handling an ACD routed call
			Busy,				// Agent is handled a non-ACD routed call
		};
	enum AgentFlags {
			AutoReady = 0x000000001	// Agent goes to auto-ready when call ends
		};
protected:
	CString m_strID;			// Agent id
	CString m_strPassword;		// Password for the agent
	AgentState m_aState;		// Current agent state
	AgentState m_anState;		// Next agent state
	CStation* m_pStation;		// Station this agent is logged into
	DWORD m_afFlags;			// Agent flags

// Public Constructor
public:
	CAgent( const char* pszId, const char* pszPassword = NULL );
	virtual ~CAgent();

// Internal constructor for serialization
protected:
	DECLARE_SERIAL(CAgent)
	CAgent();

// Public methods
public:
	const char* GetAgentId() const;
	void NotifyState() const;
	bool ValidatePassword(const char* pszPassword) const;
	AgentState GetState() const;
	DWORD GetFlags() const;
	CString GetStateText() const;
	void SetState(AgentState as);
	void SetNextState(AgentState as);
	void GotoNextState();
	AgentState GetNextState() const;

	// Station to agent association
	void AssociateStation(CStation* pStation);
	CStation* GetStation();

// Serialization support from CObject
public:
	void Serialize( CArchive& archive );

// Creation dialog
public:
	static CAgent* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
	virtual bool ContextMenu( CWnd* pwndOwner );
	virtual void OnCommand( UINT nCmd );
};

/*****************************************************************************
** Procedure:  CAgent::CAgent
**
** Arguments: void
**
** Returns:    void
**
** Description:  Default no-parameter constructor for the agent object
**
*****************************************************************************/
inline CAgent::CAgent() : m_strID(_T("")), m_strPassword(_T("")), 
	m_aState(SignedOut), m_afFlags(AutoReady), m_pStation(0),
	m_anState(NotReady)
{
}// CAgent::CAgent

/*****************************************************************************
** Procedure:  CAgent::CAgent
**
** Arguments: void
**
** Returns:    void
**
** Description:  Parameterized constructor for the agent obec
**
*****************************************************************************/
inline CAgent::CAgent(const char* pszID, const char* pszPassword/*=NULL*/) : 
	m_strID(pszID), m_strPassword(pszPassword), m_aState(SignedOut), 
	m_afFlags(AutoReady), m_pStation(0), m_anState(NotReady)
{
}// CAgent::CAgent

/*****************************************************************************
** Procedure:  CAgent::GetAgentId
**
** Arguments: void
**
** Returns:    Pointer to string with agent id
**
** Description:  This function allows public access to the agent id
**
*****************************************************************************/
inline const char* CAgent::GetAgentId() const
{
	return m_strID;

}// CAgent::GetAgentId

/*****************************************************************************
** Procedure:  CAgent::ValidatePassword
**
** Arguments: 'pszPassword' - Password to check against agent password
**
** Returns:    true/false result
**
** Description:  This function compares the given password against the
**               agent's password and returns whether they are the same.
**
*****************************************************************************/
inline bool CAgent::ValidatePassword(const char* pszPassword) const
{
	if (!m_strPassword.IsEmpty())
		return (m_strPassword.CompareNoCase(pszPassword) == 0);
	return true;

}// CAgent::ValidatePassword

/*****************************************************************************
** Procedure:  CAgent::GetState
**
** Arguments: void
**
** Returns:    Current AgentState constant which represents the agent state.
**
** Description:  This function retrieves the dynamic agent state
**
*****************************************************************************/
inline CAgent::AgentState CAgent::GetState() const
{
	return m_aState;

}// CAgent::GetState

/*****************************************************************************
** Procedure:  CAgent::GetNextState
**
** Arguments: void
**
** Returns:    Next AgentState constant which represents the agent state.
**
** Description:  This function retrieves the dynamic agent state
**
*****************************************************************************/
inline CAgent::AgentState CAgent::GetNextState() const
{
	return m_anState;

}// CAgent::GetNextState

/*****************************************************************************
** Procedure:  CAgent::AssociateStation
**
** Arguments: 'pStation' - Station to associate to agent
**
** Returns:    void
**
** Description:  This function associates a station (line) and agent
**
*****************************************************************************/
inline void CAgent::AssociateStation(CStation* pStation)
{
	if (m_pStation != pStation)
	{
		CStation* pOld = m_pStation;
		m_pStation = pStation;

		if (pOld != NULL)
			SetState(SignedOut);
		if (m_pStation != NULL)
			SetState(NotReady);
	}

}// CAgent::AssociateStation

/*****************************************************************************
** Procedure:  CAgent::GetStation
**
** Arguments: void
**
** Returns:    Pointer to station object
**
** Description:  This function returns our associated station
**
*****************************************************************************/
inline CStation* CAgent::GetStation()
{
	return m_pStation;

}// CAgent::GetStation

/*****************************************************************************
** Procedure:  CAgent::GetFlags
**
** Arguments: void
**
** Returns:    Agent flags
**
** Description:  This function returns the agent flags
**
*****************************************************************************/
inline DWORD CAgent::GetFlags() const
{
	return m_afFlags;

}// CAgent::GetFlags

#endif // __JPBX_AGENT_INC__

