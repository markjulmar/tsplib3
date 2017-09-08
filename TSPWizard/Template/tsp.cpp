$$IF(VERBOSE)
/***************************************************************************
//
// $$ROOT$$.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Main TAPI Service Provider shell
//
// Copyright (C) $$CURR_YEAR$$ $$COMPANY_NAME$$
// All rights reserved
//
// $$JULMAR_INFO$$
// 
/***************************************************************************/

/*-------------------------------------------------------------------------------*/
// INCLUDE FILES
/*-------------------------------------------------------------------------------*/
$$ENDIF
#include "stdafx.h"
#include "$$root$$.h"

$$IF(VERBOSE)
/*-------------------------------------------------------------------------------*/
// CONSTANTS AND GLOBALS
/*-------------------------------------------------------------------------------*/
$$ENDIF
$$SP_CLASS_NAME$$ theSP;

$$IF(VERBOSE)
/*-------------------------------------------------------------------------------*/
// RTTI overrides of TSP++ objects
/*-------------------------------------------------------------------------------*/
$$ENDIF
DECLARE_TSPI_OVERRIDE($$DEVICE_CLASS_NAME$$);
$$IF(OVERRIDE_LINE)
DECLARE_TSPI_OVERRIDE($$LINE_CLASS_NAME$$);
$$ENDIF
$$IF(OVERRIDE_ADDRESS)
DECLARE_TSPI_OVERRIDE($$ADDRESS_CLASS_NAME$$);
$$ENDIF
$$IF(OVERRIDE_CALL)
DECLARE_TSPI_OVERRIDE($$CALL_CLASS_NAME$$);
$$ENDIF
$$IF(OVERRIDE_CONFCALL)
DECLARE_TSPI_OVERRIDE($$CONFCALL_CLASS_NAME$$);
$$ENDIF
$$IF(OVERRIDE_PHONE)
DECLARE_TSPI_OVERRIDE($$PHONE_CLASS_NAME$$);
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$SP_CLASS_NAME$$::$$SP_CLASS_NAME$$
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This is the constructor for the provider object.  It
**               is responsible for initializing the UI.DLL name and the
**               provider manufacturer names.  In additional, any overrides
**               of class objects should be performed here.
**
*****************************************************************************/
$$ENDIF
$$SP_CLASS_NAME$$::$$SP_CLASS_NAME$$() : 
$$IF(HAS_UI)
	CServiceProvider(_T("$$safe_root$$UI.DLL"),		// Name of UI DLL
$$ELSE
	CServiceProvider(_T("$$safe_root$$.TSP"),		// Name of UI DLL
$$ENDIF
					 _T("$$PROVIDER_INFO$$"),
					 $$TAPIVERSION$$)
{   
	// Setup our line device and phone device object override.
	SetRuntimeObjects (
		"$$DEVICE_CLASS_NAME$$",	// Device override 
$$IF(OVERRIDE_LINE)
		"$$LINE_CLASS_NAME$$",		// Line override
$$ELSE
		NULL,						// Line override
$$ENDIF
$$IF(OVERRIDE_ADDRESS)
		"$$ADDRESS_CLASS_NAME$$",		// Address override
$$ELSE
		NULL,						// Address override
$$ENDIF
$$IF(OVERRIDE_CALL)
		"$$CALL_CLASS_NAME$$",		// Call override
$$ELSE
		NULL,						// Call override
$$ENDIF
$$IF(OVERRIDE_CONFCALL)
		"$$CONFCALL_CLASS_NAME$$",	// Conference call override
$$ELSE
		NULL,						// Conference call override
$$ENDIF
$$IF(OVERRIDE_PHONE)
		"$$PHONE_CLASS_NAME$$");	// Phone override
$$ELSE
		NULL);						// Phone override
$$ENDIF

#ifdef _DEBUG
	// Turn on tracing level for debug builds -- see SPLIB.H for
	// additional tracing flags.
	SetTraceLevel(TRC_API |			// TSPI_xxx entrypoints (requires JTTSPTRC.dll)
				  TRC_DUMP |		// TSPI buffers (requires JTTSPTRC.dll)
				  TRC_MIN |			// Basic tracing inside library
				  TRC_WARNINGS |	// Warnings from TSP++
				  TRC_STATS |		// Statistics on calls/addresses/lines
				  TRC_OBJECTS |		// Object creation/destruction
				  TRC_THREADS);		// Thread creation/destruction
#endif

}// $$SP_CLASS_NAME$$::$$SP_CLASS_NAME$$
$$IF(!HAS_UI)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$SP_CLASS_NAME$$::providerEnumDevices
**
** Arguments:  'dwProviderId'	-	Passed provider ID from TAPI
**             'lpNumLines'		-	Returning count of lines
**             'lpNumPhones'	-	Returning count of phones
**			   'hProvider'		-	Handle to the provider from TAPI
**			   'lpfnLineC..'	-	Line creation PROC for LINE_CREATE
**			   'lpfnPhoneC..'	-	Phone creation PROC for PHONE_CREATE	
**
** Returns:    TAPI result code
**
** Description:  This is called after the constructor to determine how many
**				 lines and phones the provider is managing.  This is called
**				 BEFORE providerInit.
**
**				 It is a required function under TAPI 2.x
**
*****************************************************************************/
$$ENDIF
LONG $$SP_CLASS_NAME$$::providerEnumDevices(DWORD dwProviderId, LPDWORD lpNumLines,
	LPDWORD lpNumPhones, HPROVIDER hProvider, 
	LINEEVENT lpfnLineCreateProc, PHONEEVENT lpfnPhoneCreateProc)
{   
	// Pass it onto the base class and let the library know about the provider handle.  
	// This is required for dynamic UI dialog support.
	LONG lResult = CServiceProvider::providerEnumDevices(dwProviderId, lpNumLines, lpNumPhones, hProvider, lpfnLineCreateProc, lpfnPhoneCreateProc);
	if (lResult == 0)
	{
		// TODO: Read the count of lines and phones from some static persistent
		// storage and supply them here. 
$$IF(LINEFEATURE_24)
		// Since this device supports plug and play, another option is to return 
		// zero for both of these and then dynamically add each line/phone device 
		// as you determine what is available.
$$ENDIF
$$IF(OVERRIDE_LINE)
		*lpNumLines	 = 1;
$$ELSE
		*lpNumLines	 = 0;
$$ENDIF
$$IF(OVERRIDE_PHONE)
		*lpNumPhones = 1;
$$ELSE
		*lpNumPhones = 0;
$$ENDIF
	}
    return lResult;

}// $$SP_CLASS_NAME$$::providerEnumDevices
$$ENDIF
$$IF(WANT_TRACEMGR)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$SP_CLASS_NAME$$::TraceOut
**
** Arguments:  'strBuff' - Buffer to output
**
** Returns:    void
**
** Description:  This is called on an idle thread to save off the debug
**               contents to persistent storage.
**
*****************************************************************************/
$$ENDIF
void $$SP_CLASS_NAME$$::TraceOut(TString& /*strBuff*/)
{
	// TODO:
	// Add code to process the given string and output to some storage
	// device such as a window, file, etc.

}// $$SP_CLASS_NAME$$::TraceOut
$$ENDIF
