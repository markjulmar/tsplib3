$$IF(VERBOSE)
/***************************************************************************
//
// PARK.CPP
//
// TAPI Service provider for TSP++ version 3.0
// linePark and lineUnpark support
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
** Procedure:  $$LINE_CLASS_NAME$$::OnPark
**
** Arguments: 'pReq' - Request object representing this PARK event 
**            'lpBuff' - The event block object
**
** Returns:    void
**
** Description:  This function manages the TSPI_linePark processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnPark(RTPark* pRequest, LPCVOID lpBuff)
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
$$IF(PARKMODE_DIRECTED)
$$IF(!PARKMODE_NONDIRECTED)
		TString strAddress = pRequest->GetDialableNumber();
		// TODO: Send the command to the hardware to park the call to the given address
$$ELSE
		if (pRequest->GetParkMode() == LINEPARKMODE_DIRECTED)
		{
			TString strAddress = pRequest->GetDialableNumber();
			// TODO: Send the command to the hardware to park the call to the given address
		}
		else // (if pRequest->GetParkMode() == LINEPARKMODE_NONDIRECTED)
		{
			// TODO: Send the command to park the call
		}
$$ENDIF
$$ENDIF
$$IF(PARKMODE_NONDIRECTED)
$$IF(!PARKMODE_DIRECTED)
		// TODO: Send the command to park the call
$$ENDIF
$$ENDIF
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
$$IF(PARKMODE_NONDIRECTED)
$$IF(PARKMODE_DIRECTED)
			if (pRequest->GetParkMode() == LINEPARKMODE_NONDIRECTED)
			{
				// TODO: Set the address where the call was parked
				pRequest->SetParkedAddress(_T("555-1212"));
			}

$$ELSE
			// TODO: Set the address where the call was parked
			pRequest->SetParkedAddress(_T("555-1212"));

$$ENDIF
$$ENDIF
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

}// $$LINE_CLASS_NAME$$::OnPark

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnUnpark
**
** Arguments: 'pReq' - Request object representing this UNPARK event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineUnpark processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnUnpark(RTUnpark* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPICallAppearance* pCall = pRequest->GetCallInfo();
	DIALINFO* pAddress = (pRequest->GetCount() > 0) ? pRequest->GetDialableNumber(0) : NULL;

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// TODO: Send a command to retrieve the parked call
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

}// $$LINE_CLASS_NAME$$::OnUnpark
