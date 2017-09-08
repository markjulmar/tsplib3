/*******************************************************************/
//
// ACDQUEUE.CPP
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "jpbx.h"
#include "acdqueue.h"
#include "document.h"
#include "station.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// RTTI INFORMATION
/*---------------------------------------------------------------*/
IMPLEMENT_SERIAL(CACDQueue, CLineDevice, 1)

/*****************************************************************************
** Procedure:  CACDQueue::CACDQueue
**
** Arguments: 'pGroup' - Agent group to work with
**
** Returns:    void
**
** Description:  Constructor for the queue device
**
*****************************************************************************/
CACDQueue::CACDQueue(CAgentGroup* pGroup, DWORD dwQueueID) : 
	CLineDevice(dwQueueID, pGroup->GetGroupName()), m_pGroup(pGroup)
{
	m_pGroup->AssignQueue(this);

}// CACDQueue::CACDQueue

/*****************************************************************************
** Procedure:  CACDQueue::CACDQueue
**
** Arguments: void
**
** Returns:    void
**
** Description:  No parameter constructor for the queue device
**
*****************************************************************************/
CACDQueue::CACDQueue() : CLineDevice(), m_pGroup(NULL)
{
}// CACDQueue::CACDQueue

/*****************************************************************************
** Procedure:  CACDQueue::~CACDQueue
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line device
**
*****************************************************************************/
CACDQueue::~CACDQueue()
{
}// CACDQueue::~CACDQueue

/*****************************************************************************
** Procedure:  CACDQueue::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CACDQueue::Serialize( CArchive& archive )
{
	CLineDevice::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << static_cast<DWORD>(m_pGroup->GetGroupID());
	}
	else
	{
		// Agent groups MUST be serialized first!
		DWORD dwGroupID;
		archive >> dwGroupID;
		m_pGroup = GetDocument()->FindAgentGroup(dwGroupID);

		ASSERT (m_pGroup != NULL);
		m_pGroup->AssignQueue(this);
	}

}// CACDQueue::Serialize

/*****************************************************************************
** Procedure:  CACDQueue::OnAgentStateChanged
**
** Arguments: 'pAgent' - Agent which has changed
**            'nas' - New Agent state
**
** Returns:    void
**
** Description: This member function is called when an agent changes state
**
*****************************************************************************/
void CACDQueue::OnAgentStateChanged(CAgent* pAgent, CAgent::AgentState nas)
{
	// No calls or agent not logged into this queue?
	if (!IsAgentInQueue(pAgent))
		return;

	// If the agent has gone READY, grab the next call and throw it at him.
	if (GetQueuedCallCount() > 0 && nas == CAgent::Ready)
	{
		CCall* pCall = GetHeadCall();
		if (pCall != NULL)
		{
			CStation* pStation = pAgent->GetStation();
			ASSERT(pStation != NULL);
			pStation->TransferCall(NULL, pCall);
		}
	}

	// If the agent has signed out, remove him from our list.
	else if (nas == CAgent::SignedOut)
	{
		for (int i = 0; i < m_arrAgents.GetSize(); i++)
		{
			CAgent* pTest = m_arrAgents[i];
			if (pTest == pAgent)
			{
				m_arrAgents.RemoveAt(i);
				GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
				break;
			}
		}
	}

}// CACDQueue::OnAgentStateChanged

/*****************************************************************************
** Procedure:  CACDQueue::AddAgent
**
** Arguments: 'pAgent' - Agent to add to our array
**
** Returns:    void
**
** Description: This function adds an agent to this queue
**
*****************************************************************************/
void CACDQueue::AddAgent(CAgent* pAgent)
{
	// Verify it isn't in the array already
	for (int i = 0; i < m_arrAgents.GetSize(); i++)
	{
		CAgent* pTest = m_arrAgents[i];
		if (pTest == pAgent)
			return;
	}

	// Add it and update the UI.
	m_arrAgents.Add(pAgent);
	GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);

	// If the agent is READY, see if we have a call
	OnAgentStateChanged(pAgent, pAgent->GetState());

}// CACDQueue::AddAgent

/*****************************************************************************
** Procedure:  CACDQueue::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an incoming call
**
*****************************************************************************/
void CACDQueue::OnReceiveCall(CCall* /*pCall_Source*/)
{
	// Cannot direct-dial queue
	ASSERT (FALSE);

}// CLineDevice::OnReceiveCall

/*****************************************************************************
** Procedure:  CACDQueue::TransferCall
**
** Arguments: 'pCall_Consult' - Consultation call which is being dissolved
**            'pCall_Source' - Call being transferred
**
** Returns:    void
**
** Description: This function is a notification for an incoming transfer/route
**
*****************************************************************************/
bool CACDQueue::TransferCall(CCall* pCall_Consult, CCall* pCall_Source)
{
	// Must always blind transfer calls
	pCall_Consult;
	ASSERT (pCall_Consult == NULL);

	// Save off the previous owner and move the call to the new line (us)
	CLineDevice* pOwner = pCall_Source->GetLineOwner();
	pCall_Source->MoveToLine(this);
	pCall_Source->SetCallState(CCall::Queued);

	// Send the appropriate event concerning this movement.
	SendEvent(true, pCall_Source, PBXEVT_CQ, pOwner->GetExtension(), pCall_Source->GetCallID(), GetExtension());

	// Now find an agent to transfer this call to.
	CAgent* pAgent = GetDocument()->GetNextAvailableAgent(m_pGroup);
	if (pAgent != NULL)
	{
		CStation* pStation = pAgent->GetStation();
		ASSERT(pStation != NULL);
		VERIFY (pStation->TransferCall(NULL, pCall_Source));
	}

	return true;

}// CACDQueue::OnTransferCall

/*****************************************************************************
** Procedure:  CACDQueue::ContextMenu
**
** Arguments: 'pwnd' - Owner for pop-up menu
**
** Returns:    true/false if the popup menu was shown
**
** Description:  This function is called to show a popup menu for an object
**
*****************************************************************************/
bool CACDQueue::ContextMenu(CWnd* pwndOwner)
{
	return RunPopup( IDR_QUEUEMENU, pwndOwner );

}// CACDQueue::ContextMenu
