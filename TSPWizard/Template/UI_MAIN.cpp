$$IF(VERBOSE)
/***************************************************************************
//
// $$ROOT$$UI.cpp
//
// TAPI Service provider for TSP++ version 3.0
// User-Interface main entrypoint
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
#include "$$root$$UI.h"
#include "config.h"
#include <spbstrm.h>

$$IF(VERBOSE)
/*-------------------------------------------------------------------------------*/
// RTTI Support
/*-------------------------------------------------------------------------------*/
$$ENDIF
IMPLEMENT_DYNCREATE(CUIDevice, CTSPUIDevice)
$$IF(OVERRIDE_LINE)
IMPLEMENT_DYNCREATE(CUILine, CTSPUILineConnection)
$$ENDIF
$$IF(OVERRIDE_PHONE)
IMPLEMENT_DYNCREATE(CUIPhone, CTSPUIPhoneConnection)
$$ENDIF
$$IF(OVERRIDE_ADDRESS)
IMPLEMENT_DYNCREATE(CUIAddress, CTSPUIAddressInfo)
$$ENDIF

/*----------------------------------------------------------------------------
	GLOBALS AND CONSTANTS
-----------------------------------------------------------------------------*/
CTspUIApp theApp;

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
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CTspUIApp::CTspUIApp
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Constructor for the UI application dll
**
*****************************************************************************/
$$ENDIF
CTspUIApp::CTspUIApp() : 
	CServiceProvider(_T("$$PROVIDER_INFO$$"))
{
$$IF(VERBOSE)
	// You must assign the overridden objects to the library 
	// so they are serialized properly when reloaded if you add additional
	// data to the iostream (read/write overridden).
$$ENDIF
	SetRuntimeObjects(
		RUNTIME_CLASS(CUIDevice),		// Device override
$$IF(OVERRIDE_LINE)
		RUNTIME_CLASS(CUILine),			// Line device
$$ELSE
		NULL, 
$$ENDIF
$$IF(OVERRIDE_ADDRESS)
		RUNTIME_CLASS(CUIAddress),		// Address object
$$ELSE
		NULL, 
$$ENDIF
$$IF(OVERRIDE_PHONE)
		RUNTIME_CLASS(CUIPhone));		// Phone device
$$ELSE
		NULL);
$$ENDIF

}// CTspUIApp::CTspUIApp

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CTspUIApp::providerInstall
**
** Arguments:  'dwPermanentProviderID' - Provider ID
**             'pwndOwner' - Owner window handle
**
** Returns:    TAPI 2.x result code
**
** Description:  This method is invoked when the TSP is to be installed via 
**               the TAPI install code.  It should insure that all the correct 
**               files are there, and write out the initial .INI settings.
**
*****************************************************************************/
$$ENDIF
LONG CTspUIApp::providerInstall(DWORD dwPermanentProviderID, CWnd* pwndOwner)
{
	// Verify that our TSP is not already installed; in most implementations
	// you should only allow for one device per TSP.
	DWORD dwMyPPid = 0;
	LONG lResult = IsProviderInstalled(_T("$$SAFE_ROOT$$.tsp"), &dwMyPPid);
	if (lResult == 0) // Returns LINEERR_NOMULTIPLEINSTANCE if already installed
	{
		// Create the device object which represents this provider. 
		AddDevice(dwPermanentProviderID);

		// Pass through to the TSP++ library to add registry keys and such.
		// If that is successful, then run the configuration for this device.
		lResult = CServiceProvider::providerInstall(dwPermanentProviderID, pwndOwner);
		if (lResult == 0)
		{
			// TODO: Show a user interface dialog if necessary and prompt the
			// user for information concerning line and phone devices present
			// on the hardware.
			CConfigDlg dlg(pwndOwner);
			if (dlg.DoModal() == IDOK)
			{
				// TODO: Remove the following code - it is here to create a
				// sample line and phone so the generated provider will have
				// devices. The first parameter is a unique identifier for the device
				// which is used to locate the device in the provider code.
$$IF(OVERRIDE_LINE)
				GetDeviceByIndex(0)->AddLine(new CUILine(100, CTSPUILineConnection::Station, _T("Line Device")));
$$ENDIF
$$IF(OVERRIDE_PHONE)
				GetDeviceByIndex(0)->AddPhone(new CUIPhone(100, _T("Phone Device")));
$$ENDIF
$$IF(OVERRIDE_LINE)
$$IF(OVERRIDE_PHONE)
				// Associate the two devices together
				GetDeviceByIndex(0)->GetPhoneConnectionInfo(0)->SetAssociatedLine(100);
$$ENDIF
$$ENDIF
				// Save off all our registry data. This is a built-in function of 
				// the TSP++ UI library and dumps all created objects into the registry.
				SaveObjects();
			}
			
			// Otherwise cancel the installation.
			else
				lResult = LINEERR_OPERATIONFAILED;
		}
	}
	return lResult;

}// CTspUIApp::providerInstall

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CTspUIApp::providerConfig
**
** Arguments:  'dwPPID' - Provider ID
**             'pwndOwner' - Owner window handle
**
** Returns:    TAPI 2.x result code
**
** Description:  This method is invoked when the user selects our ServiceProvider
**				 icon in the control panel.  It should invoke the configuration 
**               dialog which must be provided by the derived class.
**
*****************************************************************************/
$$ENDIF
LONG CTspUIApp::providerConfig(DWORD dwProviderID, CWnd* pwndOwner)
{
	// TODO: Show a user interface dialog of all the configuration
	// data for all lines and phones on the hardware.
	CConfigDlg dlg(pwndOwner);
	if (dlg.DoModal() == IDOK)
	{
		// Save off all our registry data. This is a built-in function of 
		// the TSP++ UI library and dumps all created objects into the registry.
		SaveObjects();
	}
	return 0;

}// CTspUIApp::providerConfig

$$IF(OVERRIDE_LINE)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CTspUIApp::lineConfigDialog
**
** Arguments:  'dwDeviceID' - Line Device ID
**             'pwndOwner' - Owner window handle
**             'strDeviceClass' - Device class we are working with
**
** Returns:    TAPI 2.x result code
**
** Description:  This method is called to display the line configuration dialog
**				 when the user requests it through either the TAPI api or the 
**               control panel applet.
**
*****************************************************************************/
$$ENDIF
LONG CTspUIApp::lineConfigDialog(DWORD dwDeviceID, CWnd* pwndOwner, CString& /*strDeviceClass*/) 
{
	// Convert the device id into a permanent device id to ensure that we
	// have the correct line device.  Normally the dwDeviceID is simply an
	// index into our line array but if there is more than one provider installed
	// the base might not be zero.
	DWORD dwpLID;
	if (GetUISP()->GetPermanentIDFromLineDeviceID(dwDeviceID, &dwpLID) == 0)
	{
		// NOTE: This assumes a single device object per provider - this is not a 
		// requirement of TSP++, but is typical of most providers.
		CTSPUILineConnection* pLine = GetUISP()->GetDeviceByIndex(0)->FindLineConnectionByPermanentID(dwpLID);
		if (pLine != NULL)
		{
			// TODO: Show user-interface for configuring this specific
			// line device.
			return FALSE;
		}
	}
	return LINEERR_OPERATIONFAILED;

}// CTspUIApp::lineConfigDialog

$$ENDIF
$$IF(OVERRIDE_PHONE)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CTspUIApp::phoneConfigDialog
**
** Arguments:  'dwDeviceID' - Phone Device ID
**             'pwndOwner' - Owner window handle
**             'strDeviceClass' - Device class we are working with
**
** Returns:    TAPI 2.x result code
**
** Description:  This method is called to display the phone configuration dialog
**				 when the user requests it through either the TAPI api or the 
**               control panel applet.
**
*****************************************************************************/
$$ENDIF
LONG CTspUIApp::phoneConfigDialog(DWORD dwDeviceID, CWnd* pwndOwner, CString& /*strDeviceClass*/) 
{
	// Convert the device id into a permanent device id to ensure that we
	// have the correct phone device.  Normally the dwDeviceID is simply an
	// index into our phone array but if there is more than one provider installed
	// the base might not be zero.
	DWORD dwpPID;
	if (GetUISP()->GetPermanentIDFromPhoneDeviceID(dwDeviceID, &dwpPID) == 0)
	{
		// NOTE: This assumes a single device object per provider - this is not a 
		// requirement of TSP++, but is typical of most providers.
		CTSPUIPhoneConnection* pPhone = GetUISP()->GetDeviceByIndex(0)->FindPhoneConnectionByPermanentID(dwpPID);
		if (pPhone != NULL)
		{
			// TODO: Show user-interface for configuring this specific
			// phone device.
			return FALSE;
		}
	}
	return PHONEERR_OPERATIONFAILED;

}// CTspUIApp::phoneConfigDialog

$$ENDIF
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIDevice::CUIDevice
**
** Arguments:  void
**
** Returns:    void
**
** Description: Default constructor for the device object
**
*****************************************************************************/
$$ENDIF
CUIDevice::CUIDevice()
{
	// TODO: Initialize any data

}// CUIDevice::CUIDevice

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIDevice::read
**
** Arguments:  'istm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUIDevice::read(TStream& istm)
{
	// Always call the base class!
	CTSPUIDevice::read(istm);

	// TODO: Read any addition information from the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return istm;

}// CUIDevice::read

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIDevice::write
**
** Arguments:  'ostm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUIDevice::write(TStream& ostm) const
{
	// Always call the base class!
	CTSPUIDevice::write(ostm);

	// TODO: Write any addition information into the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return ostm;

}// CUIDevice::write

$$IF(OVERRIDE_LINE)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUILine::CUILine
**
** Arguments:  void
**
** Returns:    void
**
** Description: Default constructor for the line object
**
*****************************************************************************/
$$ENDIF
CUILine::CUILine()
{
	// TODO: Initialize any data

}// CUILine::CUILine

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUILine::CUILine
**
** Arguments:  'dwDeviceID' - Device identifier
**             'iType' - Type of line device (Station, RoutePoint, etc.)
**             'pszName' - Name of line device
**
** Returns:    void
**
** Description:  Constructor for the line device
**
*****************************************************************************/
$$ENDIF
CUILine::CUILine(DWORD dwDeviceID, int iType, LPCTSTR pszName) : 
	CTSPUILineConnection(dwDeviceID, iType, pszName)
{
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Replace these entries with the name and dialable number
		// of each address created.
		TCHAR chBuff[20], chBuff2[20];
		wsprintf(chBuff, _T("Address %d"), iAddress);	
		wsprintf(chBuff2, _T("%d"), 100+iAddress);

		switch (GetLineType())
		{
			case CTSPUILineConnection::Station:
$$IF(LINEFEATURE_7)
				EnableAgentSupport(true);
$$ENDIF
$$IF(ALLOWINCOMINGCALLS)
$$IF(ALLOWOUTGOINGCALLS)
				CreateAddress(chBuff2, chBuff, true, true,
$$ELSE
				CreateAddress(chBuff2, chBuff, true, false,
$$ENDIF
$$ELSE
$$IF(ALLOWOUTGOINGCALLS)
				CreateAddress(chBuff2, chBuff, false, true,
$$ELSE
				CreateAddress(chBuff2, chBuff, false, false,
$$ENDIF
$$ENDIF
					$$MEDIAMODES$$,	LINEBEARERMODE_VOICE,
					0, 0, NULL, $$NUMACTIVECALLS$$, $$MAXONHOLD$$, $$TRANSF_MAXONHOLD$$, $$CONFMAXCOUNT$$, $$CONFTRANSFMAXCOUNT$$);
				break;
$$IF(LINEFEATURE_33)
			case CTSPUILineConnection::VRU:
				CreateAddress(chBuff2, chBuff, true, false, 
					LINEMEDIAMODE_AUTOMATEDVOICE, LINEBEARERMODE_VOICE,
					0, 0, NULL, $$NUMACTIVECALLS$$, $$MAXONHOLD$$, $$TRANSF_MAXONHOLD$$, 0, 0);
				break;
$$ENDIF
$$IF(LINEFEATURE_6)
			case CTSPUILineConnection::RoutePoint:
				CreateAddress(chBuff2, chBuff, true, false, 
						$$MEDIAMODES$$, LINEBEARERMODE_VOICE,
						0, 0, NULL, 0xffffffff, 0, 0, 0, 0);
				break;
$$ENDIF
$$IF(LINEFEATURE_31)
			case CTSPUILineConnection::Queue:
				CreateAddress(chBuff2, chBuff, true, false, 
						$$MEDIAMODES$$, LINEBEARERMODE_VOICE,
						0, 0, NULL, 0xffffffff, 0xffffffff, 0xffffffff, 0, 0);
				break;
$$ENDIF
$$IF(LINEFEATURE_32)
			case CTSPUILineConnection::PredictiveDialer:
				CreateAddress(chBuff2, chBuff, false, true,
						$$MEDIAMODES$$, LINEBEARERMODE_VOICE,
						0, 0, NULL, $$PDIAL_MAXCALLS$$, 0, 0, 0, 0);
				break;
$$ENDIF
$$IF(LINEFEATURE_37)
			case CTSPUILineConnection::Trunk:
				CreateAddress(chBuff2, chBuff, true, false, 
					$$MEDIAMODES$$, LINEBEARERMODE_VOICE,
					0, 0, NULL, 1, 1, 1, 0, 0);
				break;
$$ENDIF
			default: ASSERT(FALSE);
		}
	}
$$IF(HASTAPI3MSP)

	// TODO: Set the MSP guid for this line device. TAPI3 has two pre-defined MSPs
	// which come with Windows 2000 --  H323.msp and IPConf.msp
	// Additional MSPs may be written for a particular protocol or physical device.
	//
	// SetMSPGUID( CLSID_MyMSPDriver );
$$ENDIF
$$IF(ISTAPI3)

	// Set the protocol GUID for this line device -- this is by default set to
	// PSTN, you only need to do this if you want to use a different protocol or 
	// define your own.
	// 
	// SetProtocolCLSID(TAPIPROTOCOL_xxx);
$$ENDIF

}// CUILine::CUILine

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUILine::read
**
** Arguments:  'istm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUILine::read(TStream& istm)
{
	// Always call the base class!
	CTSPUILineConnection::read(istm);

	// TODO: Read any addition information from the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return istm;

}// CUILine::read

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUILine::write
**
** Arguments:  'ostm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUILine::write(TStream& ostm) const
{
	// Always call the base class!
	CTSPUILineConnection::write(ostm);

	// TODO: Write any addition information into the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return ostm;

}// CUILine::write

$$ENDIF
$$IF(OVERRIDE_ADDRESS)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIAddress::CUIAddress
**
** Arguments:  void
**
** Returns:    void
**
** Description: Default constructor for the address object
**
*****************************************************************************/
$$ENDIF
CUIAddress::CUIAddress()
{
	// TODO: Initialize any data

}// CUIAddress::CUIAddress

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIAddress::read
**
** Arguments:  'istm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUIAddress::read(TStream& istm)
{
	// Always call the base class!
	CTSPUIAddressInfo::read(istm);

	// TODO: Read any addition information from the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return istm;

}// CUIAddress::read

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIAddress::write
**
** Arguments:  'ostm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUIAddress::write(TStream& ostm) const
{
	// Always call the base class!
	CTSPUIAddressInfo::write(ostm);

	// TODO: Write any addition information into the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return ostm;

}// CUIAddress::write

$$ENDIF
$$IF(OVERRIDE_PHONE)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIPhone::CUIPhone
**
** Arguments:  void
**
** Returns:    void
**
** Description: Default constructor for the phone object
**
*****************************************************************************/
$$ENDIF
CUIPhone::CUIPhone()
{
	// TODO: Initialize any data

}// CUIPhone::CUIPhone

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIPhone::CUIPhone
**
** Arguments:  'dwPhoneID' - Phone identifier
**             'pszName' - Name of phone unit
**
** Returns:    void
**
** Description:  Constructor for the phone device
**
*****************************************************************************/
$$ENDIF
CUIPhone::CUIPhone(DWORD dwPhoneID, LPCTSTR pszName) : 
	CTSPUIPhoneConnection(dwPhoneID, pszName)
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
$$IF(PHONEFEATURE_3)
    // Setup the display buffer for the phone. The final parameter which is
	// not specified here is the character used to delimit rows. The default is
	// a line feed.
	SetupDisplay(DISPLAY_COLS, DISPLAY_ROWS);

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
                  g_ButtonLamps[iButton].dwLampMode, g_ButtonLamps[iButton].pszText);

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
}// CUIPhone::CUIPhone

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIPhone::read
**
** Arguments:  'istm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUIPhone::read(TStream& istm)
{
	// Always call the base class!
	CTSPUIPhoneConnection::read(istm);

	// TODO: Read any addition information from the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return istm;

}// CUIPhone::read

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CUIPhone::write
**
** Arguments:  'ostm' - Input iostream
**
** Returns:    iostream reference
**
** Description: This is called to read information in from the registry.
**
*****************************************************************************/
$$ENDIF
TStream& CUIPhone::write(TStream& ostm) const
{
	// Always call the base class!
	CTSPUIPhoneConnection::write(ostm);

	// TODO: Write any addition information into the registry stream
	// using the direct stream >> and << operators.
	//
	// WARNING: Make sure to coordinate the read/write functions

	return ostm;

}// CUIPhone::write

$$ENDIF

