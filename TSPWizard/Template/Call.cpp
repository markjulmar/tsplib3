$$IF(VERBOSE)
/***************************************************************************
//
// CALL.CPP
//
// TAPI Service provider for TSP++ version 3.0
// CTSPICallAppearance override
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
** Procedure:  $$CALL_CLASS_NAME$$::$$CALL_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the call object
**
*****************************************************************************/
$$ENDIF
$$CALL_CLASS_NAME$$::$$CALL_CLASS_NAME$$()
{
	// TODO: place any constructor code here

}// $$CALL_CLASS_NAME$$::$$CALL_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$CALL_CLASS_NAME$$::~$$CALL_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the call object
**
*****************************************************************************/
$$ENDIF
$$CALL_CLASS_NAME$$::~$$CALL_CLASS_NAME$$()
{
	// TODO: destroy any allocated memory here

}// $$CALL_CLASS_NAME$$::~$$CALL_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$CALL_CLASS_NAME$$::Init
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
void $$CALL_CLASS_NAME$$::Init(CTSPIAddressInfo* pAddr, DWORD dwBearerMode, DWORD dwRate, 
				DWORD dwCallParamFlags, DWORD dwOrigin, DWORD dwReason, DWORD dwTrunk, DWORD dwCompletionID)
{
	// Pass through to base class
	CTSPICallAppearance::Init(pAddr, dwBearerMode, dwRate, dwCallParamFlags,
			dwOrigin, dwReason, dwTrunk, dwCompletionID);

	// TODO: Initialize the call object

}// $$CALL_CLASS_NAME$$::Init

$$IF(HASTAPI3MSP)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$CALL_CLASS_NAME$$::ReceiveMSPData
**
** Arguments:  'pMSP' - MSP driver object
**             'lpData' - Data being sent from the MSP
**             'dwSize' - Size of the data block
**
** Returns:    TAPI result code
**
** Description: This method is called on behalf of the TSPI_ReceiveMSPData
**              entrypoint. It is called if the passed Call handle is not NULL.
**
*****************************************************************************/
$$ENDIF
LONG $$CALL_CLASS_NAME$$::ReceiveMSPData(CMSPDriver* pMSP, LPVOID lpData, DWORD dwSize)
{
	// TODO: Handle the incoming data - or pass through to the default which
	// will cause the CTSPILineConnection::ReceiveMSPData to be called for this event.
	return CTSPICallAppearance::ReceiveMSPData(pMSP, lpData, dwSize);

}// $$CALL_CLASS_NAME$$::ReceiveMSPData
$$ENDIF
