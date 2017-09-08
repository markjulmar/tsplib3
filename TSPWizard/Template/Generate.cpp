$$IF(VERBOSE)
/***************************************************************************
//
// GENERATE.CPP
//
// TAPI Service provider for TSP++ version 3.0
// lineGenerateDigit and lineGenerateTone handlers
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

$$IF(LINEFEATURE_12)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnGenerateDigits
**
** Arguments: 'pReq' - Request object representing this GENERATEDIGITS event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineGenerateDigits processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnGenerateDigits(RTGenerateDigits* pRequest, LPCVOID lpBuff)
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
		TString strDigits = pRequest->GetDigits();
		DWORD dwDuration = pRequest->GetDuration();
		for (unsigned int i = 0; i < strDigits.length(); i++)
		{
			TCHAR chDigit = strDigits[i];
			if (chDigit == _T('!'))
			{
				// TODO: Generate a hookflash (one-half second onhook, 
				// followed by one-half second offhook before continuing) event.
			}
			else if (chDigit == _T(','))
			{
				// Pause in the dialing string
				Sleep(GetLineDevCaps()->DefaultDialParams.dwDialPause);
			}
			else
			{
				// TODO: Generate the requested digit, ignore invalid digits
				// for the device rather than reporting an error.
				// Use the "dwDuration" as the tone duration
$$IF(GENERATEDIGIT_PULSE)
$$IF(GENERATEDIGIT_TONE)
				if (pRequest->GetDigitMode() == LINEDIGITMODE_PULSE)
				{
					// TODO: Generate the pulse-style digit
				}
				else // if (pRequest->GetDigitMode() == LINEDIGITMODE_TONE)
				{
					// TODO: Generate a DTMF digit
				}
$$ENDIF
$$ENDIF
			}
			// Pause for inter-digit timeout
			Sleep(dwDuration);
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

}// $$LINE_CLASS_NAME$$::OnGenerateDigits
$$ENDIF
$$IF(LINEFEATURE_13)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnGenerateTone
**
** Arguments: 'pReq' - Request object representing this GENERATETONE event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineGenerateTone processing
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::OnGenerateTone(RTGenerateTone* pRequest, LPCVOID lpBuff)
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
		DWORD dwDuration = pRequest->GetDuration();
$$IF(GENERATETONE_CUSTOM)
		if (pRequest->GetToneMode() == LINETONEMODE_CUSTOM)
		{
$$IF(VERBOSE)
			// Tones come in an array (1-Max number you specified in the wizard)
			// which are actually frequencies to generate simultaneously to equal
			// the expected tone.
$$ENDIF
			unsigned int iCount = pRequest->GetToneCount();
			// TODO: Send a command to generate the requested tone based on the
			// passed frequencies in pRequest for the requested duration (mSecs).
		}
$$IF(GENERATETONE_STDMODES)
		else
		{
			DWORD dwToneMode = pRequest->GetToneMode();
			// TODO: Generate the requested tone for the requested duration (mSecs)
		}
$$ENDIF
$$ELSE
		DWORD dwToneMode = pRequest->GetToneMode();
		// TODO: Generate the requested tone for the requested duration (mSecs)
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

}// $$LINE_CLASS_NAME$$::OnGenerateTone
$$ENDIF
