$$IF(VERBOSE)
/***************************************************************************
//
// ADDRESS.CPP
//
// TAPI Service provider for TSP++ version 3.0
// CTSPIAddressInfo override
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
$$IF(HAS_UI)
#include <spbstrm.h>
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$ADDRESS_CLASS_NAME$$::$$ADDRESS_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the address object
**
*****************************************************************************/
$$ENDIF
$$ADDRESS_CLASS_NAME$$::$$ADDRESS_CLASS_NAME$$()
{
	// TODO: place any constructor code here

}// $$ADDRESS_CLASS_NAME$$::$$ADDRESS_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$ADDRESS_CLASS_NAME$$::~$$ADDRESS_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the address object
**
*****************************************************************************/
$$ENDIF
$$ADDRESS_CLASS_NAME$$::~$$ADDRESS_CLASS_NAME$$()
{
	// TODO: destroy any allocated memory here

}// $$ADDRESS_CLASS_NAME$$::~$$ADDRESS_CLASS_NAME$$

$$IF(HAS_UI)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$ADDRESS_CLASS_NAME$$::read
**
** Arguments: 'istm' - I/O stream to read information from
**
** Returns:    I/O stream buffer
**
** Description:  This function is called to read the address information
**               from a registry stream buffer.
**
*****************************************************************************/
$$ENDIF
TStream& $$ADDRESS_CLASS_NAME$$::read(TStream& istm)
{
	// Pass through to the base class to read address information
	CTSPIAddressInfo::read(istm);

	// TODO: read information from the stream to initialize the address object

	return istm;

}// $$ADDRESS_CLASS_NAME$$::read
$$ELSE
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$ADDRESS_CLASS_NAME$$::Init
**
** Arguments: 'pLine' - Line owner
**            'dwAddressID' - Sequential identifier for this address object
**            'lpszAddress' - Dialable number for the address
**            'lpszName' - Name of the address
**            'fIncoming' - TRUE if supports incoming call traffic
**            'fOutgoing' - TRUE if supports outgoing call traffic
**            'dwAvailMediaModes' - Available media modes on this address
**            'dwAvailBearerMode' - Bearer mode for this address
**			  'dwMinRate' - Minimum data rate
**            'dwMaxRate' - Maximum data rate
**            'dwMaxNumActiveCalls - Maximum number of calls taking bandwidth (non-held)
**            'dwMaxNumOnHoldCalls - Maximum number of calls on hold
**            'dwMaxNumOnHoldPendCalls - Maximum number of calls pending transfer/conf
**            'dwMaxNumConference - Maximum number of calls in a conference
**            'dwMaxNumTransConf - Maximum number of calls transferred into a conference
**            'dwAddressSharing' - Share type for this address
**            'dwAddressType' - Type of address (TAPI3)
**
** Returns:    void
**
** Description:  This function is called to initialize the address object after
**               the constructor is called.
**
*****************************************************************************/
$$ENDIF
void $$ADDRESS_CLASS_NAME$$::Init(CTSPILineConnection* pLine, DWORD dwAddressID, 
		LPCTSTR lpszAddress, LPCTSTR lpszName, bool fIncoming, bool fOutgoing, 
		DWORD dwAvailMediaModes, DWORD dwAvailBearerMode, DWORD dwMinRate, DWORD dwMaxRate,
        DWORD dwMaxNumActiveCalls, DWORD dwMaxNumOnHoldCalls, DWORD dwMaxNumOnHoldPendCalls, 
		DWORD dwMaxNumConference, DWORD dwMaxNumTransConf, DWORD dwAddressSharing, const LPLINEDIALPARAMS lpDialParams, DWORD dwAddressType)
{
	// Pass through to base class
	CTSPIAddressInfo::Init(pLine, dwAddressID, lpszAddress, lpszName, fIncoming, fOutgoing,
		dwAvailMediaModes, dwAvailBearerMode, dwMinRate, dwMaxRate, dwMaxNumActiveCalls,
		dwMaxNumOnHoldCalls, dwMaxNumOnHoldPendCalls, dwMaxNumConference, 
		dwMaxNumTransConf, dwAddressSharing, lpDialParams, dwAddressType);

	// TODO: Initialize the address object

}// $$ADDRESS_CLASS_NAME$$::Init
$$ENDIF
