$$IF(VERBOSE)
/***************************************************************************
//
// DEVCONFIG.CPP
//
// TAPI Service provider for TSP++ version 3.0
// lineGetDevConfig and lineSetDevConfig handlers
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
** Procedure:  $$LINE_CLASS_NAME$$::GetDevConfig
**
** Arguments: 'strDeviceClass' - Device class type being asked for
**            'lpDeviceConfig' - Returning device configuration
**
** Returns:    void
**
** Description: This function returns an "opaque" data structure object, 
**              the contents of which are specific to this service provider 
**              and the passed device class. The data structure object should
**              contain the current configuration of a media-stream device 
**              associated with the line device.
**
*****************************************************************************/
$$ENDIF
LONG $$LINE_CLASS_NAME$$::GetDevConfig(TString& strDeviceClass, LPVARSTRING lpDeviceConfig)
{
	// TODO: store any provider configuration information related to the 
	// specified device class. For example, the contents of this structure 
	// could specify data rate, character format, modulation schemes, and 
	// error control protocol settings for a "datamodem" media device associated 
	// with the line. 

	return LINEERR_INVALDEVICECLASS;

}// $$LINE_CLASS_NAME$$::GetDevConfig

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::SetDevConfig
**
** Arguments: 'strDeviceClass' - Device class type being asked for
**            'lpDevConfig' - Input buffer from the application
**            'dwSize' - Size of the buffer
**
** Returns:    void
**
** Description: This function allows the application to restore the configuration 
**              of a media stream device on a line device to a setup previously 
**              obtained using lineGetDevConfig. 
**
*****************************************************************************/
$$ENDIF
LONG $$LINE_CLASS_NAME$$::SetDevConfig(TString& strDeviceClass, const LPVOID lpDevConfig, DWORD dwSize)
{
	// TODO: restore provider configuration information related to the
	// specified device class from the opaque data structure. Make sure
	// to verify the contents of the data block before assuming it is valid
	// since neither TAPI nor TSP++ can validate it.

	return LINEERR_INVALDEVICECLASS;

}// $$LINE_CLASS_NAME$$::SetDevConfig

