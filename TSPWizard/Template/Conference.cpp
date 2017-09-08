$$IF(VERBOSE)
/***************************************************************************
//
// CONFERENCE.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Manage conference events
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
** Procedure:  $$LINE_CLASS_NAME$$::OnSetupConference
**
** Arguments: 'pReq' - Request object representing this SetupConf event
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineSetupConference processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnSetupConference(RTSetupConference* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPIConferenceCall* pConfCall = pRequest->GetConferenceCall();
	CTSPICallAppearance* pCall = pRequest->GetOriginalCall();
	CTSPICallAppearance* pConsult = pRequest->GetConsultationCall();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// TODO: Initiate the conference event
		// Most switches require that the original call is placed on hold

		// If the call we are going to transfer is not currently on hold then
		// place it on hold so it may be transferred by the switch
		if ((pCall->GetCallState() & 
				(LINECALLSTATE_ONHOLD | LINECALLSTATE_ONHOLDPENDTRANSFER | LINECALLSTATE_ONHOLDPENDCONF)) == 0)
		{
			// TODO: Send the command to the hardware to place this call on hold
		}

$$IF(VERBOSE)
		// If the call is already on hold then complete the request and transition the
		// consultation call to the DIALTONE state so that the application 
		// may use lineDial against it.
$$ENDIF
		else
		{
$$IF(VERBOSE)
			// Transition the existing call to the CONFERENCED state
			// and the conference call into the onHold pending conference state.
$$ENDIF
			pConfCall->SetCallState(LINECALLSTATE_ONHOLDPENDCONF);
			pCall->SetCallState(LINECALLSTATE_CONFERENCED);

$$IF(VERBOSE)
			// Place the consultation call into the DIALTONE state.
			// Most PBXs will have a dialtone on the handset now.
$$ENDIF
			pConsult->SetCallState(LINECALLSTATE_DIALTONE, 
					$$DIALTONE_DEFAULT$$, LINEMEDIAMODE_INTERACTIVEVOICE);
			
$$IF(VERBOSE)
			// Complete the request - the application should now use the lineDial()
			// TAPI command to dial on this consultation call and then call 
			// lineCompleteTransfer() to finish this transfer.
$$ENDIF
			CompleteRequest(pRequest, 0);
		}
	}

$$IF(VERBOSE)
	// If we are in the waiting stage (2) then see if we received an event from the
	// switch (vs. an interval timer) and if that event was an ACK/NAK in response
	// to the command we issued.
$$ENDIF
	else if (pRequest->GetState() == STATE_WAITING && lpBuff != NULL)
	{
		// TODO: If this is a response from the above command to place the
		// call on hold then perform the cleanup against this request if 
		// it was successful or fail the request if not.
		// Return TRUE to skip the unsolicited handler.
		bool fSuccess = false;
		if (fSuccess)
		{
$$IF(VERBOSE)
			// Transition the existing call to the CONFERENCED state
			// and the conference call into the onHold pending conference state.
$$ENDIF
			pConfCall->SetCallState(LINECALLSTATE_ONHOLDPENDCONF);
			pCall->SetCallState(LINECALLSTATE_CONFERENCED);

$$IF(VERBOSE)
			// Place the consultation call into the DIALTONE state.
			// Most PBXs will have a dialtone on the handset now.
$$ENDIF
			pConsult->SetCallState(LINECALLSTATE_DIALTONE, 
					$$DIALTONE_DEFAULT$$, LINEMEDIAMODE_INTERACTIVEVOICE);
			return true;
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

}// $$LINE_CLASS_NAME$$::OnSetupConf
$$IF(CONFADDEXISTING)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnPrepareAddToConference
**
** Arguments: 'pReq' - Request object representing this PrepareConf event
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_linePrepareAddToConference processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnPrepareAddToConference(RTPrepareAddToConference* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPIConferenceCall* pConfCall = pRequest->GetConferenceCall();
	CTSPICallAppearance* pConsult = pRequest->GetConsultationCall();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// TODO: Issue the commands necessary to hold the conference and
		// prepare it to add a new call.
	}

$$IF(VERBOSE)
	// If we are in the waiting stage (2) then see if we received an event from the
	// switch (vs. an interval timer) and if that event was an ACK/NAK in response
	// to the command we issued.
$$ENDIF
	else if (pRequest->GetState() == STATE_WAITING && lpBuff != NULL)
	{
		// TODO: Check the response to see if this is a completion of the above
		// command. If it is successful, perform the necessary call state changes
		// on the conference.  Otherwise, fail the request
		bool fSuccess = false;
		if (fSuccess)
		{
$$IF(VERBOSE)
			// Transition the consultation call to the DIALTONE state and the
			// and the conference call into the onHold pending conference state.
			// Each of the conference parties should stay in the CONFERENCED state.
$$ENDIF
			pConfCall->SetCallState(LINECALLSTATE_ONHOLDPENDCONF);
			pConsult->SetCallState(LINECALLSTATE_DIALTONE, 
					$$DIALTONE_DEFAULT$$, LINEMEDIAMODE_INTERACTIVEVOICE);
				
$$IF(VERBOSE)
			// Complete the request - the application should not use the lineDial()
			// TAPI command to dial on this consultation call and then call 
			// lineCompleteTransfer() to finish this transfer.
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

}// $$LINE_CLASS_NAME$$::OnPrepareAddToConference
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnAddToConference
**
** Arguments: 'pReq' - Request object representing this AddToConference event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineAddToConference processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnAddToConference(RTAddToConference* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPIConferenceCall* pConfCall = pRequest->GetConferenceCall();
	CTSPICallAppearance* pConsult = pRequest->GetConsultationCall();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// TODO: Add the specified consultation call to the conference
		// and re-establish the conferenced parties
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
			// TODO: If the hardware doesn't send an event which triggers the
			// unsolicited event to restore the conference:
			//
			// 1. Change the state of the consultation call to LINECALLSTATE_CONFERENCED
			// 2. Change the state of the conference owner to LINECALLSTATE_CONNECTED
			//
			// pConsult->SetCallState(LINECALLSTATE_CONFERENCED);
			// pConfCall->SetCallState(LINECALLSTATE_CONNECTED);
			//
			// It is better (if possible) to have this code in the unsolicited handler
			// since it presumably can occur independant of TAPI.
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

}// $$LINE_CLASS_NAME$$::OnAddToConference
$$IF(CONFREMOVE)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnRemoveFromConf
**
** Arguments: 'pReq' - Request object representing this RemoveFromConf event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineRemoveFromConference 
**               processing for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnRemoveFromConference(RTRemoveFromConference* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPIConferenceCall* pConfCall = pRequest->GetConferenceCall();
	CTSPICallAppearance* pCall = pRequest->GetCallToRemove();

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		// TODO: Drop the call from the conference
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
			// TODO: If the hardware does not send an explicit release call event
			// then you must set the call state of the removed call to Idle, this
			// will cause TSP++ to drop it from the conference
			//
			// pCall->SetCallState(LINECALLSTATE_IDLE);
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

}// $$LINE_CLASS_NAME$$::OnRemoveFromConference
$$ENDIF
