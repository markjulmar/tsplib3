$$IF(VERBOSE)
/***************************************************************************
//
// TERMINAL.CPP
//
// TAPI Service provider for TSP++ version 3.0
// lineSetTerminal handler
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
** Procedure:  $$LINE_CLASS_NAME$$::OnSetTerminal
**
** Arguments: 'pReq' - Request object representing this request
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the lineSetTerminal processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnSetTerminal(RTSetTerminal* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPIAddressInfo* pAddress = pRequest->GetAddress();
	CTSPICallAppearance* pCall = pRequest->GetCall();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		DWORD dwTerminalID = pRequest->GetTerminalID();	// Terminal identifier (0-NumTerminals)
		DWORD dwEvents = pRequest->GetTerminalModes();	// LINETERMMODE_xxx
		bool fEnable = pRequest->Enable();
		if (pCall != NULL)
		{
			// TODO: Send the command to enable/disable the given terminal events 
			// for this call only.
		}
		else // (pAddress != NULL)
		{
			// TODO: Send the command to enable/disable the given terminal events
			// for all calls on this address.
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

}// $$LINE_CLASS_NAME$$::OnSetTerminal

