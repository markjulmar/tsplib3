$$IF(VERBOSE)
/***************************************************************************
//
// COMPLETE.CPP
//
// TAPI Service provider for TSP++ version 3.0
// lineCompleteCall and lineUncompleteCall handlers
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

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnCompleteCall
**
** Arguments: 'pReq' - Request object representing this CompleteCall event 
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the lineCompleteCall processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnCompleteCall(RTCompleteCall* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPICallAppearance* pCall = pRequest->GetCallInfo();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		DWORD dwCompletionMode = pRequest->GetCompletionMode();

		// TODO: Send a command to ask the switch to complete the call
		// using the passed LINECOMPLETECALL_xxx type
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
			// TODO: Use some returning identifier from the switch to associate
			// the completion request to a switch request. You must do this BEFORE
			// completing the request.
			//
			// pRequest->SetIdentifier( string or DWORD );

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

}// $$LINE_CLASS_NAME$$::OnCompleteCall

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnUncompleteCall
**
** Arguments: 'pReq' - Request object representing this UncompleteCall event 
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the lineUncompleteCall processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnUncompleteCall(RTUncompleteCall* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPICallAppearance* pCall = pRequest->GetCallInfo();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// DWORD dwSwitchID = pRequest->GetNumericIdentifier();
		//     .... or ....
		// TString strSwitchID = pRequest->GetStringIdentifier();

		// TODO: Send a command to ask the switch to throw away the previous
		// completion request asked for. The switch identifier can be located
		// using the above functions based on what was associated to the
		// request by the original CompleteCall request.
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

}// $$LINE_CLASS_NAME$$::OnUncompleteCall

