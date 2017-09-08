$$IF(VERBOSE)
/***************************************************************************
//
// PHONE.CPP
//
// TAPI Service provider for TSP++ version 3.0
// CTSPIPhoneConnection override
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
$$IF(HAS_UI)
#include <spbstrm.h>
$$ENDIF

$$IF(!HAS_UI)
/*------------------------------------------------------------------------------*/
//	GLOBALS AND CONSTANTS
/*------------------------------------------------------------------------------*/
$$IF(PHONEFEATURE_3)
const UINT DISPLAY_COLS = $$DISPLAY_WIDTH$$;
const UINT DISPLAY_ROWS = $$DISPLAY_HEIGHT$$;

$$ENDIF
$$IF(PHONEFEATURE_4)
$$IF(BUTTONLAMP_COUNT)
// Button/Lamp information - note since buttons/lamps are inserted into the
// list in-order, you can rely on the positions of these buttons/lamps in your
// event handler. Note: do not alter the first 12 buttons - they are required
// by TAPI specification to be in-order.
LAMPBUTTONENTRY g_ButtonLamps[] = {
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("0") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("1") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("2") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("3") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("4") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("5") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("6") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("7") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("8") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("9") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("*") },
	{ PHONEBUTTONFUNCTION_NONE, PHONEBUTTONMODE_KEYPAD, PHONELAMPMODE_DUMMY, _T("#") },
$$BEGINLOOP(BUTTONLAMP_COUNT)
	$$BUTTONLAMP_ENTRY$$
$$ENDLOOP
};
#define BUTTON_COUNT (sizeof(g_ButtonLamps) / sizeof(g_ButtonLamps[0]))

$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_5)
// Hookswitch devices - use the PHONEHOOKSWITCH_DEV constants to identify
// and reference the different hookswitches
static HOOKSWITCHENTRY g_HSDevice[] = {
$$BEGINLOOP(HOOKSWITCH_COUNT)
	$$HOOKSWITCH_ENTRY$$
$$ENDLOOP
};
#define HOOKSWITCH_COUNT (sizeof(g_HSDevice) / sizeof(g_HSDevice[0]))

$$ENDIF
$$ENDIF
$$IF(VERBOSE)
/*-------------------------------------------------------------------------------*/
// TSPI Request map
/*-------------------------------------------------------------------------------*/
$$ENDIF
BEGIN_TSPI_REQUEST($$PHONE_CLASS_NAME$$)
$$IF(PHONEFEATURE_1)
	ON_TSPI_REQUEST_GETPHONEDATA(OnGetData)
$$ENDIF
$$IF(PHONEFEATURE_2)
	ON_TSPI_REQUEST_SETPHONEDATA(OnSetData)
$$ENDIF
$$IF(PHONEFEATURE_3)
$$IF(DISPLAY_CANWRITE)
	ON_TSPI_REQUEST_SETDISPLAY(OnSetDisplay)
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_4)
$$IF(BUTTON_CANWRITE)
	ON_TSPI_REQUEST_SETBUTTONINFO(OnSetButtonInfo)
$$ENDIF
$$IF(LAMP_CANWRITE)
	ON_TSPI_REQUEST_SETLAMP(OnSetLamp)
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_5)
$$IF(GAIN_CANWRITE)
	ON_TSPI_REQUEST_SETHOOKSWITCHGAIN(OnSetGain)
$$ENDIF
$$IF(HOOKSWITCH_CANWRITE)
	ON_TSPI_REQUEST_SETHOOKSWITCH(OnSetHookswitch)
$$ENDIF
$$IF(VOLUME_CANWRITE)
	ON_TSPI_REQUEST_SETHOOKSWITCHVOL(OnSetVolume)
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_6)
$$IF(RINGER_CANWRITE)
	ON_TSPI_REQUEST_SETRING(OnSetRinger)
$$ENDIF
$$ENDIF
END_TSPI_REQUEST()

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::$$PHONE_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the address object
**
*****************************************************************************/
$$ENDIF
$$PHONE_CLASS_NAME$$::$$PHONE_CLASS_NAME$$()
{
	// TODO: place any constructor code here

}// $$PHONE_CLASS_NAME$$::$$PHONE_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::~$$PHONE_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the address object
**
*****************************************************************************/
$$ENDIF
$$PHONE_CLASS_NAME$$::~$$PHONE_CLASS_NAME$$()
{
	// TODO: destroy any allocated memory here

}// $$PHONE_CLASS_NAME$$::~$$PHONE_CLASS_NAME$$

$$IF(HAS_UI)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::read
**
** Arguments:  'istm' - Input stream
**
** Returns:    pointer to istm
**
** Description:  This function is called to serialize data in from the
**               registry.  The phone object has already been completely
**               initialized by the TSP++ library
**
*****************************************************************************/
$$ENDIF
TStream& $$PHONE_CLASS_NAME$$::read(TStream& istm)
{
	// Always call the base class to read the basic phone information.
	CTSPIPhoneConnection::read(istm);

	// TODO: Read any information stored in the phone stream

$$IF(PHONEFEATURE_3)
	// TODO: Set the default display value for the phone device
	// if the display has something when it is not in use.
	// SetDisplay(_T(""));

$$ENDIF
	// TODO: Add any WAV devices which will be used for I/O - locate your WAV drive
	// and use the device identifier given by the multimedia system.
	// AddDeviceClass(_T("wave/in"), dwWaveInDeviceID);
	// AddDeviceClass(_T("wave/out"), dwWaveOutDeviceID);

	return istm;

}// $$PHONE_CLASS_NAME$$::read
$$ELSE
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::Init
**
** Arguments:  'pDevice'		-	Device object this phone belongs to
**             'dwPhoneDeviceID'-	Unique phone identifier within the TSP
**             'dwPos'			-	Index position of phone within device array
**             'dwItemData'		-   Used when phone was dynamically created (P&P).
**
** Returns:    void
**
** Description:  This function is called by the device owner to initialize
**               the phone object.
**
*****************************************************************************/
$$ENDIF
void $$PHONE_CLASS_NAME$$::Init(CTSPIDevice* pDevice, DWORD dwPhoneDeviceID, DWORD dwPos, DWORD dwItemData)
{
$$IF(PHONEFEATURE_1)
	static DWORD g_dwUploadBuffers[] = {
$$BEGINLOOP(UPLOADBUFFER_COUNT)
		$$UPLOADBUFFER_SIZE$$,
$$ENDLOOP
	};
	
$$ENDIF
$$IF(PHONEFEATURE_2)
	static DWORD g_dwDownloadBuffers[] = {
$$BEGINLOOP(DOWNLOADBUFFER_COUNT)
		$$DOWNLOADBUFFER_SIZE$$,
$$ENDLOOP
	};
	
$$ENDIF
	// Let the base class initialize first.
	CTSPIPhoneConnection::Init(pDevice, dwPhoneDeviceID, dwPos, dwItemData);

$$IF(PHONEFEATURE_3)
    // Setup the display buffer for the phone. The final parameter which is
	// not specified here is the character used to delimit rows. The default is
	// a line feed.
    SetupDisplay(DISPLAY_COLS, DISPLAY_ROWS);

	// TODO: Set the default display value for the phone device if the display 
	// has something when it is not in use. The default is blank.
	// SetDisplay(_T(""));

$$ENDIF
$$IF(PHONEFEATURE_5)
	// Add the hookswitch devices
	for (int iHookswitch = 0; iHookswitch < HOOKSWITCH_COUNT; iHookswitch++)
		AddHookSwitchDevice(g_HSDevice[iHookswitch].dwHookSwitchDevice,
				g_HSDevice[iHookswitch].dwAvailModes, g_HSDevice[iHookswitch].dwVolume,
				g_HSDevice[iHookswitch].dwGain, g_HSDevice[iHookswitch].dwSettableModes,
				g_HSDevice[iHookswitch].fSupportsVolChange, g_HSDevice[iHookswitch].fSupportsGainChange);


$$ENDIF
$$IF(PHONEFEATURE_4)
$$IF(BUTTONLAMP_COUNT)
	// Setup the buttons and lamps
    for (int iButton = 0; iButton < BUTTON_COUNT; iButton++)
		AddButton(g_ButtonLamps[iButton].dwFunction, g_ButtonLamps[iButton].dwButtonMode,  
                  g_ButtonLamps[iButton].dwLampMode, (g_ButtonLamps[iButton].dwLampMode == PHONELAMPMODE_DUMMY) ?
				  0 : PHONELAMPMODE_UNKNOWN,
                  g_ButtonLamps[iButton].pszText);

$$ELSE
	// TODO: Setup the buttons and lamps. For each button/lamp identified on the
	// phone unit, call the AddButton() function. The first 12 buttons should always
	// be PHONEBUTTONMODE_KEYPAD with the text 0-9, *, #.

$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_1)
	// Add the upload buffers
	for (int iUpBuffer = 0; iUpBuffer < $$UPLOADBUFFER_COUNT$$; iUpBuffer++)
		AddUploadBuffer(g_dwUploadBuffers[iUpBuffer]);

$$ENDIF
$$IF(PHONEFEATURE_2)
	// Add the download buffers
	for (int iDnBuffer = 0; iDnBuffer < $$DOWNLOADBUFFER_COUNT$$; iDnBuffer++)
		AddDownloadBuffer(g_dwDownloadBuffers[iDnBuffer]);

$$ENDIF
$$IF(PHONEFEATURE_6)
	// Adjust the ringer styles
    GetPhoneCaps()->dwNumRingModes = $$RINGER_STYLE_COUNT$$;
    GetPhoneStatus()->dwRingMode = $$RINGER_STYLE_DEFAULT$$;

$$ENDIF
	// TODO: Add any WAV devices which will be used for I/O - locate your WAV drive
	// and use the device identifier given by the multimedia system.
	// AddDeviceClass(_T("wave/in"), dwWaveInDeviceID);
	// AddDeviceClass(_T("wave/out"), dwWaveOutDeviceID);

	// TODO: Associate this phone unit with a line device if there is
	// one that performs call-control for this station. You must do this here
	// and not in the line since lines are initialized first.
	int iPhone = dwPos;
	for (int iLines = 0; iLines < pDevice->GetLineCount(); iLines++)
	{
		// TODO: Replace code - this finds the station in the same position.
		CTSPILineConnection* pLine = pDevice->GetLineConnectionInfo(iLines);
		if (pLine->GetLineType() == CTSPILineConnection::Station)
		{
			if (iPhone-- == 0)
			{
				pDevice->AssociateLineWithPhone(iLines, dwPos);
				break;
			}
		}
	}

}// $$PHONE_CLASS_NAME$$::Init
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::OpenDevice
**
** Arguments:  void
**
** Returns:    void
**
** Description: This method is called when the phone is opened by TAPI.
**              It may be used to check the link to the switch and fail the
**              phoneOpen() call to TAPI so that no phone operations can
**              occur.
**
*****************************************************************************/
$$ENDIF
bool $$PHONE_CLASS_NAME$$::OpenDevice()
{
	// TODO: Validate the connection to the hardware and return FALSE if it
	// is not connected or pass through to the default implementation if it is.
	return CTSPIPhoneConnection::OpenDevice();

}// $$PHONE_CLASS_NAME$$::OpenDevice

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$PHONE_CLASS_NAME$$::OnTimer
**
** Arguments:  void
**
** Returns:    void
**
** Description: This method is called periodically by the interval timer
**
*****************************************************************************/
$$ENDIF
void $$PHONE_CLASS_NAME$$::OnTimer()
{
	// Poll the active request for timeout
	ReceiveData();

}// $$PHONE_CLASS_NAME$$::OnTimer
