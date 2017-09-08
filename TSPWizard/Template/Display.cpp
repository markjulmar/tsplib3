$$IF(VERBOSE)
/***************************************************************************
//
// DISPLAY.CPP
//
// TAPI Service provider for TSP++ version 3.0
// phoneSetDisplay support
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

$$IF(PHONEFEATURE_3)
$$IF(DISPLAY_CANWRITE)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::OnSetDisplay
**
** Arguments: 'pReq' - Request object representing this request
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the phoneSetDisplay processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$PHONE_CLASS_NAME$$::OnSetDisplay(RTSetDisplay* pRequest, LPCVOID lpBuff)
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
		int iColumn = static_cast<int>(pRequest->GetColumn());
		int iRow = static_cast<int>(pRequest->GetRow());
		LPCTSTR pszDisplay = pRequest->GetBufferPtr();
		DWORD dwSize = pRequest->GetBufferSize();
		// TODO: Update the display, starting at the given column/row.
		// The display buffer is a single string, with no character separating
		// the rows from each other.
	}

$$IF(VERBOSE)
	// If we are in the waiting stage (2) then see if we received an event from the
	// switch (vs. an interval timer) and if that event was an ACK/NAK in response
	// to the command we issued.
$$ENDIF
	else if (pRequest->GetState() == STATE_WAITING && lpBuff != NULL)
	{
		// TODO: Check the return code from the PBX and fail the request 
		// if it was unable to set the button.
		bool fError = false;
		if (!fError)
		{
			// TSP++ will automatically change the stored button information
			// and reflect the new button structure to TAPI.
			CompleteRequest(pRequest, 0);
		}
		else
		{
			// TODO: Replace with a specific error.
			CompleteRequest(pRequest,PHONEERR_OPERATIONFAILED);
		}
	}

$$IF(VERBOSE)
	// Check to see if our request has exceeded the limit for processing.  If 
	// so, tell TAPI that the request failed and delete the request.
$$ENDIF
	if (pRequest->GetState() == STATE_WAITING && 
		(pRequest->GetStateTime()+REQUEST_TIMEOUT) < GetTickCount())
		CompleteRequest(pRequest, PHONEERR_OPERATIONFAILED);

	// Let the request fall through to the unsolicited handler.
	return false;

}// $$PHONE_CLASS_NAME$$::OnSetDisplay
$$ENDIF
$$ENDIF
