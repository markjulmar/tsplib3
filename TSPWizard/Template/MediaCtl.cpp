$$IF(VERBOSE)
/***************************************************************************
//
// MEDIACTL.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Media control management
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
** Procedure:  $$LINE_CLASS_NAME$$::OnMediaControl
**
** Arguments: 'pCall' - Call object which has triggered the media control event
**            'dwMediaControl' - Media action to perform on the call
**
** Returns:    void
**
** Description:  This function manages the media control (lineSetMediaControl)
**               for this service provider.
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::OnMediaControl(CTSPICallAppearance* pCall, DWORD dwMediaControl)
{
	// TODO:
	// The event (media change, tone, digit or call state) has happened,
	// send the command to execute the specified media control on this call
	//
	// The dwMediaControl value is a LINEMEDIACONTROL_xxxx constant.

}// $$LINE_CLASS_NAME$$::OnMediaControl

