/*******************************************************************/
//
// ACDQUEUE.H
//
// ACD Queue emulation support
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

#ifndef __JPBX_ACDQUEUE_INC__
#define __JPBX_ACDQUEUE_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "agent.h"
#include "agentgroup.h"
#include "line.h"
#include "call.h"

/**************************************************************************
** CACDQueue
**
** This object represents a single ACD queue to the simulator
**
***************************************************************************/
class CACDQueue : public CLineDevice
{
// Class data
protected:
	CAgentGroup* m_pGroup;	// Agent group which embodies this queue
	CTypedPtrArray<CObArray,CAgent*> m_arrAgents; // Logged on agents

// Public Constructor
public:
	CACDQueue( CAgentGroup* pGroup, DWORD dwQueueID );
	virtual ~CACDQueue();

// Serializable constructor
protected:
	DECLARE_SERIAL(CACDQueue)
	CACDQueue();

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Access functions
public:
	// Queue count
	int GetQueuedCallCount() const;
	CCall* GetHeadCall();

	// Agent management
	int GetAgentCount() const;
	bool IsAgentInQueue(CAgent* pAgent);
	void OnAgentStateChanged(CAgent* pAgent, CAgent::AgentState nas);
	CAgent* FindAgentInState(enum AgentState as) const;
	void AddAgent(CAgent* pAgent);

// Methods from CLineDevice
public:
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual bool TransferCall(CCall* pCall_Consult, CCall* pCall_Source);

// UI Functions
	virtual bool ContextMenu( CWnd* pwndOwner );
};	

/*****************************************************************************
** Procedure:  CACDQueue::GetQueuedCallCount
**
** Arguments: void
**
** Returns:    Total count of queued calls
**
** Description: This member function is called to determine the count of calls
**              currently queued.
**
*****************************************************************************/
inline int CACDQueue::GetQueuedCallCount() const
{
	return m_lstCalls.GetCount();

}// CACDQueue::GetQueuedCallCount

/*****************************************************************************
** Procedure:  CACDQueue::GetHeadCall
**
** Arguments: void
**
** Returns:    The top call in our queue
**
** Description: This member function is called to retrieve the next
**              call to work with.
**
*****************************************************************************/
inline CCall* CACDQueue::GetHeadCall()
{
	if (GetQueuedCallCount() > 0)
		return m_lstCalls.GetHead();
	return NULL;

}// CACDQueue::GetHeadCall

/*****************************************************************************
** Procedure:  CACDQueue::GetAgentCount
**
** Arguments: void
**
** Returns:    The total number of logged on agents
**
** Description: This member function counts the logged on agents
**
*****************************************************************************/
inline int CACDQueue::GetAgentCount() const
{
	return m_arrAgents.GetSize();

}// CACDQueue::GetAgentCount

/*****************************************************************************
** Procedure:  CACDQueue::IsAgentInQueue
**
** Arguments: 'pAgent' - Agent to locate
**
** Returns:    true/false
**
** Description: This member function determines if the agent is in our queue
**
*****************************************************************************/
inline bool CACDQueue::IsAgentInQueue(CAgent* pAgent)
{
	for (int i = 0; i < GetAgentCount(); i++)
		if (pAgent == m_arrAgents[i])
			return true;
	return false;

}// CACDQueue::IsAgentInQueue

/*****************************************************************************
** Procedure:  CACDQueueGroup::FindAgentInState
**
** Arguments: 'as' - Agent state to search for
**
** Returns: Pointer to the next agent in the given state
**
** Description:  This internal functions searches the internal group
**               array to locate an agent in the given state
**
*****************************************************************************/
inline CAgent* CACDQueue::FindAgentInState(enum AgentState as) const
{
	for (int i=0; i<m_arrAgents.GetSize(); i++)
	{
		if (m_arrAgents[i]->GetState() == as)
			return (m_arrAgents[i]);
	}
	return NULL;

}// CACDQueue::FindAgentInState

#endif // __JPBX_ACDQUEUE_INC__
