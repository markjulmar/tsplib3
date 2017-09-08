$$IF(VERBOSE)
/***************************************************************************
//
// DEVSPECIFIC.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Device-specific extensions for the TSP
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
$$IF(LINEFEATURE_8)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::DevSpecificFeature
**
** Arguments: 'dwFeature' - Device-specific feature to invoke
**            'dwRequestId' - Asynchronous request identifier
**            'lpParams' - Parameter buffer passed from application
**            'dwSize' - Size of parameter buffer from application
**
** Returns:    TAPI 2.x result code
**
** Description: This function intercepts the lineDevSpecificFeature 
**              handler for the TSP. 
**
*****************************************************************************/
$$ENDIF
LONG $$LINE_CLASS_NAME$$::DevSpecificFeature(DWORD dwFeature, DRV_REQUESTID dwRequestId, LPVOID lpParams, DWORD dwSize)
{
$$IF(VERBOSE)
	// TODO: Implement the device specific features required.
	// If the request will be managed asynchronously, then a CTSPIRequest object must
	// be created for the request and inserted into the line management queue using the
	// AddAsynchRequest function.
	//
	// Example:
	//
	// MyRequest* pMyRequest = new MyRequest(dwFeature, ....);
	// if (AddAsynchRequest(pRequest))
	//		return (LONG) dwRequestID;
	// return LINEERR_OPERATIONFAILED;
	//
	// Otherwise simply perform the request on this thread and return FALSE
	// as a success indicator. TAPI will automatically complete the asynchronous
	// request.
$$ENDIF
	return FALSE;

}// $$LINE_CLASS_NAME$$::DevSpecificFeature

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::DevSpecific
**
** Arguments: 'pAddress' - Address object (may be NULL)
**            'pCall' - Call object (may be NULL)
**            'dwRequestId' - Asynchronous request identifier
**            'lpParams' - Parameter buffer passed from application
**            'dwSize' - Size of parameter buffer from application
**
** Returns:    TAPI 2.x result code
**
** Description: This function intercepts the lineDevSpecific handler for 
**              the TSP. 
**
*****************************************************************************/
$$ENDIF
LONG $$LINE_CLASS_NAME$$::DevSpecific(CTSPIAddressInfo* pAddress, CTSPICallAppearance* pCall, 
								DRV_REQUESTID dwRequestId, LPVOID lpParams, DWORD dwSize)
{
$$IF(VERBOSE)
	// TODO: Implement the device specific features required.
	// If the request will be managed asynchronously, then a CTSPIRequest object must
	// be created for the request and inserted into the line management queue using the
	// AddAsynchRequest function.
	//
	// Example:
	//
	// MyRequest* pMyRequest = new MyRequest(....);
	// if (AddAsynchRequest(pRequest))
	//		return (LONG) dwRequestID;
	// return LINEERR_OPERATIONFAILED;
	//
	// Otherwise simply perform the request on this thread and return FALSE
	// as a success indicator. TAPI will automatically complete the asynchronous
	// request.
$$ENDIF
	return FALSE;

}// $$LINE_CLASS_NAME$$::DevSpecific

$$ENDIF
$$ENDIF
$$IF(OVERRIDE_PHONE)
$$IF(PHONEFEATURE_0)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::DevSpecific
**
** Arguments: 'dwRequestId' - Asynchronous request identifier
**            'lpParams' - Parameter buffer passed from application
**            'dwSize' - Size of parameter buffer from application
**
** Returns:    TAPI 2.x result code
**
** Description: This function intercepts the phoneDevSpecific handler for 
**              the TSP. 
**
*****************************************************************************/
$$ENDIF
LONG $$PHONE_CLASS_NAME$$::DevSpecific(DRV_REQUESTID dwRequestId, LPVOID lpParams, DWORD dwSize)
{
$$IF(VERBOSE)
	// TODO: Implement the device specific features required.
	// If the request will be managed asynchronously, then a CTSPIRequest object must
	// be created for the request and inserted into the line management queue using the
	// AddAsynchRequest function.
	//
	// Example:
	//
	// MyRequest* pMyRequest = new MyRequest(....);
	// if (AddAsynchRequest(pRequest))
	//		return (LONG) dwRequestID;
	// return PHONEERR_OPERATIONFAILED;
	//
	// Otherwise simply perform the request on this thread and return FALSE
	// as a success indicator. TAPI will automatically complete the asynchronous
	// request.
$$ENDIF
	return FALSE;

}// $$PHONE_CLASS_NAME$$::DevSpecific
$$ENDIF
$$ENDIF
