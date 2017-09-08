$$IF(VERBOSE)
/***************************************************************************
//
// UNSOLICITED.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Unsolicited event handler
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

$$IF(OVERRIDE_LINE)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::UnsolicitedEvent
**
** Arguments:  'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function is called when no request processed a given
**               response from the device and it was directed at this line.
**
*****************************************************************************/
$$ENDIF
bool $$LINE_CLASS_NAME$$::UnsolicitedEvent(LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);
$$ENDIF

	// TODO: process the event block which was received.

	// This might include agent state changes, call creation, call destruction, 
	// call state changes, transfers, conferences, etc.

	// Use the relevant objects to change information - for example,
	// use the CTSPICallAppearance and change the call state using the "SetCallState"
	// method.

	// For an example of this, see the JTSP sample supplied with TSP++ 3.0

	return true;

}// $$LINE_CLASS_NAME$$::UnsolicitedEvent
$$ENDIF
$$IF(OVERRIDE_PHONE)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::UnsolicitedEvent
**
** Arguments:  'lpBuff' - The received event block
**
** Returns:    void
**
** Description:  This function is called when no request processed a given
**               response from the device and it was directed at this phone.
**
*****************************************************************************/
$$ENDIF
bool $$PHONE_CLASS_NAME$$::UnsolicitedEvent(LPCVOID lpBuff)
{
$$IF(EVENT_OBJECT_NAME)
	// Cast the input opaque pointer back to an event block
	const $$EVENT_OBJECT_NAME$$* pEvent = static_cast<const $$EVENT_OBJECT_NAME$$*>(lpBuff);

$$ENDIF
	// TODO: process the event block which was received.

	// This might include display changes, lamp changes, button presses, etc.

	// Use the relevant objects to change information - for example,
	// use the CTSPIPhoneConnection and change the display using the "SetDisplay"
	// method.

	// For an example of this, see the JTSP sample supplied with TSP++ 3.0

	return true;

}// $$PHONE_CLASS_NAME$$::UnsolicitedEvent
$$ENDIF
