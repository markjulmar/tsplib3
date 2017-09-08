$$IF(VERBOSE)
/***************************************************************************
//
// BTNLAMP.CPP
//
// TAPI Service provider for TSP++ version 3.0
// phoneSetButtonInfo and phoneSetLampInfo handlers
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

/*----------------------------------------------------------------------------
	EXTERNALS
-----------------------------------------------------------------------------*/
extern LAMPBUTTONENTRY g_ButtonLamps[];
$$IF(BUTTON_CANWRITE)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::OnSetButtonInfo
**
** Arguments: 'pReq' - Request object representing this request
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the phoneSetButtonInfo processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$PHONE_CLASS_NAME$$::OnSetButtonInfo(RTSetButtonInfo* pRequest, LPCVOID lpBuff)
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
		// TODO: Validate the button being changed - on some PBXs, 
		// only certain buttons defined on the station are "soft". 
		DWORD dwButtonIndex = pRequest->GetButtonLampID();
		if (dwButtonIndex < TOTAL_STD_BUTTONS) // Within 0-9,#,*?
		{
			CompleteRequest(pRequest, PHONEERR_INVALBUTTONLAMPID);
			return false;
		}
		
		// TODO: Validate the button mode and function
		DWORD dwMode = pRequest->GetButtonMode();
		DWORD dwFunction = pRequest->GetButtonFunction();

		// TODO: Send the change request to the switch to update the
		// button on the station.
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

}// $$PHONE_CLASS_NAME$$::OnSetButtonInfo
$$ENDIF
$$IF(LAMP_CANWRITE)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::OnSetLamp
**
** Arguments: 'pReq' - Request object representing this request
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the phoneSetLampInfo processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$PHONE_CLASS_NAME$$::OnSetLamp(RTSetLampInfo* pRequest, LPCVOID lpBuff)
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
		// TODO: Validate the lamp being changed - TSP++ verified that
		// it was (or has) a lamp associated with the index.
		DWORD dwLampIndex = pRequest->GetButtonLampID();
		
		// TODO: Validate the lamp state
		DWORD dwMode = pRequest->GetLampMode();

		// TODO: Send the change request to the switch to update the
		// lamp on the station.
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

}// $$PHONE_CLASS_NAME$$::OnSetLamp
$$ENDIF

