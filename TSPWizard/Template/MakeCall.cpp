$$IF(VERBOSE)
/***************************************************************************
//
// MAKECALL.CPP
//
// TAPI Service provider for TSP++ version 3.0
// lineMakeCall and lineDial management
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
** Procedure:  $$LINE_CLASS_NAME$$::OnMakeCall
**
** Arguments: 'pReq' - Request object representing this MAKECALL event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineMakeCall processing
**               for this service provider. 
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnMakeCall(RTMakeCall* pRequest, LPCVOID lpBuff)
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
	if (pRequest->EnterState(STATE_INITIAL, STATE_IGNORE))
	{
$$IF(LINEFEATURE_32)
		if (GetLineType() == PredictiveDialer)
		{
			// TODO:
			// Handle predictive dialing if this is a predictive dialer line
		}

$$ENDIF
		// TODO: Examine the dialing string and convert TAPI special indicator
		// characters to the appropriate information to send to the device. The
		// supported characters are:
		//
		// ! - Indicates that a hookflash (one-half second onhook, followed by one-half 
		// second offhook before continuing) is to be inserted in the dial string.
		//
		// t/T - Indicates that tone (DTMF) dialing is to be used for the digits 
		// following it
		//
		// p/P - Indicates that pulse dialing is to be used for the digits following it.
		// , - Indicates that dialing is to be paused. The duration of a pause is 
		// specified by GetLineDevCaps()->DefaultDialParams.dwDialPause. 
		// Multiple commas can be used to provide longer pauses
$$IF(LINEFEATURE_36)
$$IF(DIALCHAR_ANSWER)
		//
		// @ - Indicates that dialing is to "wait for quiet answer" before dialing 
		// the remainder of the dialable address. This means to wait for at least one 
		// ringback tone followed by several seconds of silence.
$$ENDIF
$$IF(DIALCHAR_BILLING)
		//
		// $ - Indicates that it is dialing the billing information and must wait for a 
		// "billing signal" (such as a credit card prompt tone).
$$ENDIF
$$IF(DIALCHAR_DIALTONE)
		//
		// w/W  - Indicates that dialing should proceed only after a dial tone has 
		// been detected
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_28)
$$IF(VERBOSE)

		// If we have no dialing information (i.e. NULL specified in lineMakeCall)
		// or the number given is incomplete (i.e. it is terminated by a semicolon
		// as per the TAPI specification) then the dialing string is incomplete or
		// partial. It may need to be handled differently.
$$ENDIF
		if (pAddress == NULL || pAddress->strNumber.empty() || pAddress->fIsPartialAddress)
		{
			// TODO: Send the command to issue partial digits.
		}
		else
		{
			// TODO: Send a command to place a call on this line
		}
$$ELSE
		// TODO: Send a command to place a call on this line
$$ENDIF

		// Transition the waiting state - waiting for a hardware response
		pRequest->SetState(STATE_WAITING);
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
		if (fError == false)
		{
$$IF(VERBOSE)
			// Complete this request so that TAPI will forward on callstate information
			// to applications. Make sure to tell TSP++ to not delete the request yet
			// by using the last parameter
$$ENDIF
			CompleteRequest(pRequest, 0, true, false);

$$IF(VERBOSE)
			// Because the request is "complete" in TAPI's eyes, we can adjust the
			// call state and change call information (assigned trunk, caller-id, etc.)
			// Change the call state to dialtone.
$$ENDIF
			pCall->SetCallState(LINECALLSTATE_DIALTONE, 
						$$DIALTONE_DEFAULT$$, LINEMEDIAMODE_INTERACTIVEVOICE);
$$IF(LINEFEATURE_29)
$$IF(DIALTONE_HASEXTERNAL)
$$IF(VERBOSE)

			// TODO: If the hardware siezes trunks on a specific digit, that
			// can be checked here.
			if (pAddress && pAddress->strNumber[0] == _T('9') && pAddress->strNumber.size() >= 1)
				pCall->SetCallState(LINECALLSTATE_DIALTONE, 
					LINEDIALTONEMODE_EXTERNAL, LINEMEDIAMODE_INTERACTIVEVOICE);
$$ENDIF
$$ENDIF
$$ENDIF

			// TODO: Associate the call-id from the hadrware with the call object so we
			// can find it later.
			// pCall->SetCallID(dwCallID);

			// TODO: Add any additional information to the call which was received from
			// the hardware. Another option is to do this in the unsolicitied handler
			// found in UNSOLICITED.CPP - this is especially helpful if the event can
			// be recieved independant of a call being placed by the TSP (i.e. external
			// stimulus
			//
			// pCall->SetTrunk (...) etc.

$$IF(LINEFEATURE_28)
			// Once we are past the dialtone stage, transition to the DIALING state
			if (pAddress && pAddress->fIsPartialAddress && !pAddress->strNumber.empty())
				pCall->SetCallState(LINECALLSTATE_DIALING);
			else if (!pAddress->fIsPartialAddress)
			{
				// Transition to the dialing and then the proceeding (waiting for ringback) state
				pCall->SetCallState(LINECALLSTATE_DIALING);
				pCall->SetCallState(LINECALLSTATE_PROCEEDING);
			}
$$ELSE
			// Transition to the dialing stage and the the proceeding (waiting for ringback) state
			pCall->SetCallState(LINECALLSTATE_DIALING);
			pCall->SetCallState(LINECALLSTATE_PROCEEDING);
$$ENDIF

			// Finally, destroy the request from our list now that we have
			// setup the call appearance. This releases the address information.
			CompleteRequest(pRequest, 0);
		}
		return true;
	}

$$IF(VERBOSE)
	// Check to see if our request has exceeded the limit for processing.  If 
	// so, tell TAPI that the request failed and delete the request.
$$ENDIF
	if (pRequest->GetState() == STATE_WAITING &&
		(pRequest->GetStateTime()+REQUEST_TIMEOUT) < GetTickCount())
		CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);

	// Let the request fall through to the unsolicited handler
	return false;

}// $$LINE_CLASS_NAME$$::OnMakeCall

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnDial
**
** Arguments: 'pReq' - Request object representing this DIAL event 
**            'lpBuff' - Our CEventBlock* pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineDial processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnDial(RTDial* pRequest, LPCVOID lpBuff)
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
	if (pRequest->EnterState(STATE_INITIAL, STATE_IGNORE))
	{
		// TODO: Examine the dialing string and convert TAPI special indicator
		// characters to the appropriate information to send to the device. The
		// supported characters are:
		//
		// ! - Indicates that a hookflash (one-half second onhook, followed by one-half 
		// second offhook before continuing) is to be inserted in the dial string.
		//
		// t/T - Indicates that tone (DTMF) dialing is to be used for the digits 
		// following it
		//
		// p/P - Indicates that pulse dialing is to be used for the digits following it.
		// , - Indicates that dialing is to be paused. The duration of a pause is 
		// specified by GetLineDevCaps()->DefaultDialParams.dwDialPause. 
		// Multiple commas can be used to provide longer pauses
$$IF(LINEFEATURE_36)
$$IF(DIALCHAR_ANSWER)
		//
		// @ - Indicates that dialing is to "wait for quiet answer" before dialing 
		// the remainder of the dialable address. This means to wait for at least one 
		// ringback tone followed by several seconds of silence.
$$ENDIF
$$IF(DIALCHAR_BILLING)
		//
		// $ - Indicates that it is dialing the billing information and must wait for a 
		// "billing signal" (such as a credit card prompt tone).
$$ENDIF
$$IF(DIALCHAR_DIALTONE)
		//
		// w/W  - Indicates that dialing should proceed only after a dial tone has 
		// been detected
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_28)
$$IF(VERBOSE)

		// If the number given is incomplete (i.e. it is terminated by a semicolon
		// as per the TAPI specification) then the dialing string is incomplete or
		// partial. It may need to be handled differently.
$$ENDIF
		if (pAddress->strNumber.empty() || pAddress->fIsPartialAddress)
		{
			// TODO: Send the command to issue partial digits.
		}
		else
		{
			// TODO: Send a command to place a call on this line
		}
$$ELSE
		// TODO: Send a command to place a call on this line
$$ENDIF

		// Transition the waiting state - waiting for a hardware response
		pRequest->SetState(STATE_WAITING);
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
		if (fError == false)
		{
$$IF(VERBOSE)
			// Complete this request so that TAPI will forward on callstate information
			// to applications. Make sure to tell TSP++ to not delete the request yet
			// by using the last parameter
$$ENDIF
			CompleteRequest(pRequest, 0, true, false);

$$IF(VERBOSE)
			// Because the request is "complete" in TAPI's eyes, we can adjust the
			// call state and change call information (assigned trunk, caller-id, etc.)
			// Change the call state to dialtone.
$$ENDIF
			pCall->SetCallState(LINECALLSTATE_DIALTONE, 
						$$DIALTONE_DEFAULT$$, LINEMEDIAMODE_INTERACTIVEVOICE);
$$IF(LINEFEATURE_29)
$$IF(DIALTONE_HASEXTERNAL)
$$IF(VERBOSE)

			// TODO: If the hardware siezes trunks on a specific digit, that
			// can be checked here.
			if (pAddress && pAddress->strNumber[0] == _T('9') && pAddress->strNumber.size() >= 1)
				pCall->SetCallState(LINECALLSTATE_DIALTONE, 
					LINEDIALTONEMODE_EXTERNAL, LINEMEDIAMODE_INTERACTIVEVOICE);
$$ENDIF
$$ENDIF
$$ENDIF

			// TODO: Associate the call-id from the hardware with the call object so we
			// can find it later.
			// pCall->SetCallID(dwCallID);

			// TODO: Add any additional information to the call which was received from
			// the hardware. Another option is to do this in the unsolicited handler
			// found in UNSOLICITED.CPP - this is especially helpful if the event can
			// be received independent of a call being placed by the TSP (i.e. external
			// stimulus
			//
			// pCall->SetTrunk (...) etc.

$$IF(LINEFEATURE_28)
			// Once we are past the dialtone stage, transition to the DIALING state
			if (pAddress && pAddress->fIsPartialAddress && !pAddress->strNumber.empty())
				pCall->SetCallState(LINECALLSTATE_DIALING);
			else if (!pAddress->fIsPartialAddress)
			{
				// Transition to the dialing and then the proceeding (waiting for ringback) state
				pCall->SetCallState(LINECALLSTATE_DIALING);
				pCall->SetCallState(LINECALLSTATE_PROCEEDING);
			}
$$ELSE
			// Transition to the dialing stage and the the proceeding (waiting for ringback) state
			pCall->SetCallState(LINECALLSTATE_DIALING);
			pCall->SetCallState(LINECALLSTATE_PROCEEDING);
$$ENDIF

			// Finally, destroy the request from our list now that we have
			// setup the call appearance. This releases the address information.
			CompleteRequest(pRequest, 0);
		}
		return true;
	}

$$IF(VERBOSE)
	// Check to see if our request has exceeded the limit for processing.  If 
	// so, tell TAPI that the request failed and delete the request.
$$ENDIF
	if (pRequest->GetState() == STATE_WAITING && 
		(pRequest->GetStateTime()+REQUEST_TIMEOUT) < GetTickCount())
		CompleteRequest(pRequest, LINEERR_OPERATIONFAILED);

	// Let the request fall through to the unsolicited handler
	return false;

}// $$LINE_CLASS_NAME$$::OnDial
