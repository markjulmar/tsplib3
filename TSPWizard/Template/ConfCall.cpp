$$IF(VERBOSE)
/***************************************************************************
//
// CONFCALL.CPP
//
// TAPI Service provider for TSP++ version 3.0
// CTSPIConferenceCall override
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
** Procedure:  $$CONFCALL_CLASS_NAME$$::$$CONFCALL_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the conference call object
**
*****************************************************************************/
$$ENDIF
$$CONFCALL_CLASS_NAME$$::$$CONFCALL_CLASS_NAME$$()
{
	// TODO: place any constructor code here

}// $$CONFCALL_CLASS_NAME$$::$$CONFCALL_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$CONFCALL_CLASS_NAME$$::~$$CONFCALL_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the conference call object
**
*****************************************************************************/
$$ENDIF
$$CONFCALL_CLASS_NAME$$::~$$CONFCALL_CLASS_NAME$$()
{
	// TODO: destroy any allocated memory here

}// $$CONFCALL_CLASS_NAME$$::~$$CONFCALL_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$CONFCALL_CLASS_NAME$$::Init
**
** Arguments: 'pAddr' - Address owning this call
**            'dwBearerMode' - Bearer mode for this call
**            'dwRate' - Data rate for this call (0 if not data)
**            'dwCallParamFlags' - Flags passed with LINECALLPARAMS for this call
**            'dwOrigin' - LINECALLORIGIN_xxx for this call
**            'dwReason' - LINECALLREASON_xxx for this call
**            'dwTrunk' - Trunk this call is on (0xffffffff = none or not known)
**            'dwCompletionID' - Completion ID if this is a lineCompleteCall response
**
** Returns:    void
**
** Description:  This function is called to initialize the call object after
**               the constructor is called.
**
*****************************************************************************/
$$ENDIF
void $$CONFCALL_CLASS_NAME$$::Init(CTSPIAddressInfo* pAddr, DWORD dwBearerMode, 
		DWORD dwRate, DWORD dwCallParamFlags, DWORD dwOrigin, DWORD dwReason, DWORD dwTrunk, DWORD dwCompletionID)
{
	// Pass through to base class
	CTSPIConferenceCall::Init(pAddr, dwBearerMode, dwRate, dwCallParamFlags,
			dwOrigin, dwReason, dwTrunk, dwCompletionID);

	// TODO: Initialize the call object

}// $$CONFCALL_CLASS_NAME$$::Init

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$CONFCALL_CLASS_NAME$$::OnConsultantCallIdle
**
** Arguments: void
**
** Returns:   void
**
** Description: This function is called when an attached consultant call
**              goes idle without being added to the conference. The conference
**              is still in the OnHoldPend state.
**
**              Some PBX environments issue a new dialtone call or retrieve
**              the existing conference.
**
*****************************************************************************/
$$ENDIF
void $$CONFCALL_CLASS_NAME$$::OnConsultantCallIdle()
{
	// TODO: Determine what the hardware does in this condition and
	// update the conference state to match.

}// $$CONFCALL_CLASS_NAME$$::OnConsultantCallIdle
