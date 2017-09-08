$$IF(VERBOSE)
/***************************************************************************
//
// FORWARD.CPP
//
// TAPI Service provider for TSP++ version 3.0
// lineForward handler
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
** Procedure:  $$LINE_CLASS_NAME$$::OnForward
**
** Arguments: 'pReq' - Request object representing this forward request
**            'lpBuff' - The passed opaque block of data
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineForward processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnForward(RTForward* pRequest, LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF
	CTSPIAddressInfo* pAddress = pRequest->GetAddressInfo();
$$IF(FWD_REQUIRESCONSULT)
	CTSPICallAppearance* pConsult = pRequest->GetConsultationCall();
$$ENDIF

$$IF(VERBOSE)
	// If we are in the initial state (i.e. this request has not been processed
	// before by any other thread). Then move the packet to the waiting state so 
	// other threads will not interfere with other events or timers.  This is 
	// guaranteed to be thread-safe and atomic.
$$ENDIF
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
$$IF(FWD_SPECIFYRINGS)
		DWORD dwNumRingsNoAnswer = pRequest->GetNoAnswerRingCount();

$$ENDIF
$$IF(VERBOSE)
		// Walk through the forwarding request array and send each set of 
		// commands to the hardware. It will not exceed $$FWD_MAXENTRIES$$ entries.
$$ENDIF
		for (unsigned int i = 0; i < pRequest->GetForwardingAddressCount(); i++)
		{
			TSPIFORWARDINFO* pForwardInfo = pRequest->GetForwardingInfo(i);
			if (pAddress != NULL)
			{
				// TODO: Send the forwarding information block using the pForwardInfo block
				// and forward just this address.
			}
			else
			{
				// TODO: Send the forwarding information block using the pForwardInfo block
				// and forward ALL addresses on this line.
			}
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
			// TSP++ will automatically update the proper status structures indicating
			// that the line/address is forwarded.
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

}// $$LINE_CLASS_NAME$$::OnForward

