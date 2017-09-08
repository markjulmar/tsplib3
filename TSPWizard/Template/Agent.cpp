$$IF(VERBOSE)
/***************************************************************************
//
// AGENT.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Agent management (from proxy) and event handling
//
// Copyright (C) $$CURR_YEAR$$ $$COMPANY_NAME$$
// All rights reserved
//
// $$JULMAR_INFO$$
// 
/***************************************************************************/

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
$$ENDIF
#include "stdafx.h"
#include "$$root$$.h"

$$IF(AGENT_SUPPORTSGROUPS)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnSetAgentGroup
**
** Arguments: 'pReq' - Request object representing this SetAgentGroup event 
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the lineSetAgentGroup processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnSetAgentGroup(RTSetAgentGroup* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);

$$ENDIF
$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
$$IF(AGENT_REQUIRESID)
$$IF(VERBOSE)
		// Collect the parameters. Since TAPI does not support agent-ids and 
		// passwords, the workaround taken by many commercial TSPs is to
		// place theagent-id in the first group entry position, 
		// the password is in the second and any groups
		// the agent wants access to are in subsequent entries.
$$ENDIF
		TString strName = (pRequest->GetCount() > 0) ? pRequest->GetGroup(0)->strName : _T("");
		TString strPassword = (pRequest->GetCount() > 1) ? pRequest->GetGroup(1)->strName : _T("");

$$ENDIF
$$IF(VERBOSE)
		// If this is a logoff request then handle it seperately from the
		// other requests since it is an agent state change to the switch.
$$ENDIF
		if (pRequest->GetCount() == 0)
		{
			// Fail if there is no agent logged on at this address.
			if (pRequest->GetAddressInfo()->GetCurrentAgentGroupCount() == 0)
			{
				CompleteRequest(pRequest, LINEERR_INVALAGENTGROUP);
				return false;
			}

			// TODO: Send command to log the agent off
		}
		else
		{
			// TODO: Log the agent into the address based on the passed group
			// identifiers or change the group association for this agent.
		}
	}

$$IF(VERBOSE)
	// If we are in the waiting stage (2) then see if we received an event from the
	// switch (vs. an interval timer) and if that event was an ACK/NAK in response
	// to the command we issued.
$$ENDIF
	else if (pRequest->GetState() == STATE_WAITING && lpBuff != NULL)
	{
		// TODO: Check the return code from the PBX and fail the request or
		// complete it with a zero return code
		bool fError = true;
		if (!fError)
		{
			CompleteRequest(pRequest, 0);
		}
		else
		{
			// TODO: Replace with a specific error.
			CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);
		}
	}

$$IF(VERBOSE)
	// Check to see if our request has exceeded the limit for processing.  If 
	// so, tell TAPI that the request failed and delete the request.
$$ENDIF
	if (pRequest->GetState() == STATE_WAITING && 
		(pRequest->GetStateTime()+REQUEST_TIMEOUT) < GetTickCount())
		CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);

	// Let the request fall through to the unsolicited handler.
	return false;

}// $$LINE_CLASS_NAME$$::OnSetAgentGroup
$$ENDIF

$$IF(AGENT_SUPPORTSSTATES)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnSetAgentState
**
** Arguments: 'pReq' - Request object representing this SetAgentState event 
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the lineSetAgentState processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnSetAgentState(RTSetAgentState* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);

$$ENDIF
$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
$$IF(VERBOSE)
		// Fail if there is not an agent logged on at this address.
$$ENDIF
		if (pRequest->GetAddressInfo()->GetCurrentAgentGroupCount() == 0)
		{
			CompleteRequest(pRequest, LINEERR_INVALAGENTID);
			return true;
		}

		// TODO: Validate the passed state based on the current state of the 
		// agent and the ACD capablities

		// TODO: Send an agent state change command to the ACD
	}

$$IF(VERBOSE)
	// If we are in the waiting stage (2) then see if we received an event from the
	// switch (vs. an interval timer) and if that event was an ACK/NAK in response
	// to the command we issued.
$$ENDIF
	else if (pRequest->GetState() == STATE_WAITING && lpBuff != NULL)
	{
		// TODO: Check the return code from the PBX and fail the request or
		// complete it with a zero return code
		bool fError = true;
		if (!fError)
		{
			CompleteRequest(pRequest, 0);
		}
		else
		{
			// TODO: Replace with a specific error.
			CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);
		}
	}

$$IF(VERBOSE)
	// Check to see if our request has exceeded the limit for processing.  If 
	// so, tell TAPI that the request failed and delete the request.
$$ENDIF
	if (pRequest->GetState() == STATE_WAITING && 
		(pRequest->GetStateTime()+REQUEST_TIMEOUT) < GetTickCount())
		CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);

	// Let the request fall through to the unsolicited handler.
	return false;

}// $$LINE_CLASS_NAME$$::OnSetAgentState
$$ENDIF

$$IF(AGENT_SUPPORTSACTIVITIES)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnSetAgentActivity
**
** Arguments: 'pReq' - Request object representing this SetAgentActivity event 
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the lineSetAgentActivity processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnSetAgentActivity(RTSetAgentActivity* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);

$$ENDIF
$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// Fail if there is not an agent logged on at this address.
		if (pRequest->GetAddressInfo()->GetCurrentAgentGroupCount() == 0)
		{
			CompleteRequest(pRequest, LINEERR_INVALAGENTID);
			return true;
		}

		// TODO: Validate the passed activity if necessary

		// TODO: Send an agent activity change to the ACD
		//
		// Note: many ACDs don't support activity codes - if the TSP is going to
		// internally support them, you can simply complete the request with a zero
		// return code and the activity code will be set into TAPI automatically.
	}

$$IF(VERBOSE)
	// If we are in the waiting stage (2) then see if we received an event from the
	// switch (vs. an interval timer) and if that event was an ACK/NAK in response
	// to the command we issued.
$$ENDIF
	else if (pRequest->GetState() == STATE_WAITING && lpBuff != NULL)
	{
		// TODO: Check the return code from the PBX and fail the request or
		// complete it with a zero return code
		bool fError = false;
		if (!fError)
		{
			CompleteRequest(pRequest, 0);
		}
		else
		{
			// TODO: Replace with a specific error.
			CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);
		}
	}

$$IF(VERBOSE)
	// Check to see if our request has exceeded the limit for processing.  If 
	// so, tell TAPI that the request failed and delete the request.
$$ENDIF
	if (pRequest->GetState() == STATE_WAITING && 
		(pRequest->GetStateTime()+REQUEST_TIMEOUT) < GetTickCount())
		CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);

	// Let the request fall through to the unsolicited handler.
	return false;

}// $$LINE_CLASS_NAME$$::OnSetAgentActivity
$$ENDIF
