/***************************************************************************
//
// LINE.CPP
//
// TAPI Service provider for TSP++ version 3.00
// Line management functions
//
// Copyright (C) 1999 JulMar Technology, Inc.
// All rights reserved
//
// Generated by the TSPWizard (C) JulMar Technology, Inc.
// 
/***************************************************************************/

/*-------------------------------------------------------------------------------*/
// INCLUDE FILES
/*-------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "TestTsp.h"

/*----------------------------------------------------------------------------
	DEBUG SUPPORT
-----------------------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------
	GLOBALS AND CONSTANTS
-----------------------------------------------------------------------------*/
const UINT MAXCALLDATA_SIZE = 4096;

/*-------------------------------------------------------------------------------*/
// TSPI Request map
/*-------------------------------------------------------------------------------*/
BEGIN_TSPI_REQUEST(CTestTspLine)
	ON_AUTO_TSPI_REQUEST(REQUEST_SETCALLDATA)
	ON_TSPI_REQUEST_MAKECALL(OnMakeCall)
	ON_TSPI_REQUEST_DIAL(OnDial)
	ON_TSPI_REQUEST_DROPCALL(OnDropCall)
END_TSPI_REQUEST()

/*****************************************************************************
** Procedure:  CTestTspLine::CTestTspLine
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the line object
**
*****************************************************************************/
CTestTspLine::CTestTspLine()
{
	// TODO: place any constructor code here

}// CTestTspLine::CTestTspLine

/*****************************************************************************
** Procedure:  CTestTspLine::~CTestTspLine
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line object
**
*****************************************************************************/
CTestTspLine::~CTestTspLine()
{
	// TODO: destroy any allocated memory here

}// CTestTspLine::~CTestTspLine

/*****************************************************************************
** Procedure:  CTestTspLine::Init
**
** Arguments:  'pDev'			-	Device object this line belongs to
**             'dwLineDeviceID'	-	Unique line identifier within the TSP
**             'dwPos'			-	Index position of line within device array
**             'dwItemData'		-   Used when line was dynamically created (P&P).
**
** Returns:    void
**
** Description:  This function is called by the device owner to initialize
**               the line object.
**
*****************************************************************************/
void CTestTspLine::Init(CTSPIDevice* pDev, DWORD dwLineDeviceID, DWORD dwPos, DWORD dwItemData)
{
    LPLINEDEVCAPS lpLineCaps = GetLineDevCaps();

	// Let the base class initialize first.
	CTSPILineConnection::Init (pDev, dwLineDeviceID, dwPos, dwItemData);

    // Set the name associated with this line.  This is optional, it gives
    // the user a displayable name which is associated with the line.  Most
	// applications use this name in their UI.
	TCHAR chBuff[50];
	wsprintf(chBuff, _T("Line Device %ld"), dwLineDeviceID);
    SetName (chBuff);

	// TODO: Determine the type of line device this entry represents.
	// This would be done either by querying the device or through static configuration
	// stored in the registry. You can use the 'dwPos' variable passed to index information
	// to identify particular lines.
	//
	// An example might be to divide the lines into groups - 20 total lines, the first
	// 10 are stations, 6 queues, 3 VRUs and 1 dialer.
	//
	// You can access unique areas in the registry using the CServiceProvider functions
	// ReadProfileDWord and ReadProfileString.
	//
	// SetLineType( Station );

	// TODO: Set the permanent line identifier for this line. This value should
	// be something that the hardware uses to identify devices that events
	// have occurred on. Some vendors refer to this as a "workstation id".
	//
	// SetPermanentLineID( dwValue );

	// TODO: Add any WAV devices which will be used for I/O - locate your WAV drive
	// and use the device identifier given by the multimedia system.
	// AddDeviceClass(_T("wave/in"), dwWaveInDeviceID);
	// AddDeviceClass(_T("wave/out"), dwWaveOutDeviceID);

	// Configure the line based on the type - this requires that it was set above
	// using the SetLineType function.
	switch (GetLineType())
	{
		case Station:			InitializeStation(); break;
		default:				break;
	}

}// CTestTspLine::Init

/*****************************************************************************
** Procedure:  CTestTspLine::InitializeStation
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function is called to initialize a station line object.
**
*****************************************************************************/
void CTestTspLine::InitializeStation()
{
	LPLINEDEVCAPS lpCaps = GetLineDevCaps();


    // Create each dialable number (address) which exists on each line.
	for (int iAddress = 0; iAddress < 1; iAddress++)
	{
		// TODO: Read the address configuration in from some persistant storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[50];
		wsprintf(chBuff, _T("Address %d"), iAddress);	
		CreateAddress(	
					_T("100"),				// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
					false,					// Do not allow incoming call traffic
					true,					// Allow outgoing calls
					LINEMEDIAMODE_UNKNOWN | LINEMEDIAMODE_INTERACTIVEVOICE,
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					1,		// Max number of active calls on address
					0,			// Max number of OnHold calls on address
					0,	// Max number of OnHoldPending calls on address
					0,		// Max number of calls in a conference on address
					0);	// Max number of calls transferred into a conference on address
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a station capabilities.
		// Look at the CTestTspLine::InitAddress function to see how the
		// address is configured for all lines.
	}

}// CTestTspLine::InitializeStation

/*****************************************************************************
** Procedure:  CTestTspLine::InitAddress
**
** Arguments:  'pAddr' - Address object to initialize
**
** Returns:    void
**
** Description:  This function is called to initialize a single address
**
*****************************************************************************/
void CTestTspLine::InitAddress(CTSPIAddressInfo* pAddress)
{
	_TSP_ASSERTE (pAddress != NULL);
	LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();
	lpAddrCaps->dwMaxCallDataSize = MAXCALLDATA_SIZE;

	// Set the dialtone modes
	lpAddrCaps->dwDialToneModes &= ~(LINEDIALTONEMODE_INTERNAL | LINEDIALTONEMODE_EXTERNAL | LINEDIALTONEMODE_SPECIAL);

	// TODO: Adjust the address capabilities to reflect the device
	// features and abilities. The wizard has set the following flags
	// for you based on your requested feature set:

	// The phone is not automatically taken off hook
	lpAddrCaps->dwAddrCapFlags &= ~LINEADDRCAPFLAGS_ORIGOFFHOOK;

}// CTestTspLine::InitAddress

/*****************************************************************************
** Procedure:  CTestTspLine::OnAddressFeaturesChanged
**
** Arguments:  'pAddr' - Address features are changing on
**             'dwFeatures' - New features for address
**
** Returns:    New features for address
**
** Description: This method is called when the features of the address change
**              It gives the derived code an opportunity to adjust the reflected
**              features before TAPI is notified.
**
*****************************************************************************/
DWORD CTestTspLine::OnAddressFeaturesChanged (CTSPIAddressInfo* pAddr, DWORD dwFeatures)
{
	// TODO: Adjust any features for the address based on the current state of the
	// hardware. This is where you can restrict the features shown in the provider for
	// special cases.
	return CTSPILineConnection::OnAddressFeaturesChanged(pAddr, dwFeatures);

}// CTestTspLine::OnAddressFeaturesChanged

/*****************************************************************************
** Procedure:  CTestTspLine::OnLineFeaturesChanged
**
** Arguments:  'dwFeatures' - New features for line
**
** Returns:    New features for address
**
** Description: This method is called when the features of the line change.
**              It gives the derived code an opportunity to adjust the reflected
**              features before TAPI is notified.
**
*****************************************************************************/
DWORD CTestTspLine::OnLineFeaturesChanged (DWORD dwFeatures)
{
	// TODO: Adjust any features for the line based on the current state of the
	// hardware. This is where you can restrict the features shown in the provider for
	// special cases.
	return CTSPILineConnection::OnLineFeaturesChanged(dwFeatures);

}// CTestTspLine::OnLineFeaturesChanged

/*****************************************************************************
** Procedure:  CTestTspLine::OnCallFeaturesChanged
**
** Arguments:  'pCall' - Call that changed
**             'dwCallFeatures' - new feature list
**
** Returns:    true/false success indicator
**
** Description: This method is called whenever the call features have changed due to
**              state changes.  The address/line have also been adjusted by the time
**              this function is called.
**
*****************************************************************************/
DWORD CTestTspLine::OnCallFeaturesChanged(CTSPICallAppearance* pCall, DWORD dwCallFeatures)
{      
	// TODO: Adjust the call features for the given call based on information
	// associated with the call and hardware capabilities.
	return CTSPILineConnection::OnCallFeaturesChanged(pCall, dwCallFeatures);

}// CTestTspLine::OnCallFeaturesChanged

/*****************************************************************************
** Procedure:  CTestTspLine::OpenDevice
**
** Arguments:  void
**
** Returns:    void
**
** Description: This method is called when the phone is opened by TAPI.
**              It may be used to check the link to the switch and fail the
**              lineOpen() call to TAPI so that no line operations can
**              occur.
**
*****************************************************************************/
bool CTestTspLine::OpenDevice()
{
	// TODO: Validate the connection to the hardware and return FALSE if it
	// is not connected or pass through to the default implementation if it is.
	return CTSPILineConnection::OpenDevice();

}// CTestTspLine::OpenDevice

/*****************************************************************************
** Procedure:  CTestTspLine::OnTimer
**
** Arguments:  void
**
** Returns:    void
**
** Description: This method is called periodically by the interval timer
**
*****************************************************************************/
void CTestTspLine::OnTimer()
{
	// Poll the active request for timeout. This is not necessary if you
	// do not implement time-out conditions for the switch. Note that the
	// interval is controlled by the "SetIntervalTimer" in device.cpp (Init)
	ReceiveData();

}// CTestTspLine::OnTimer

LONG CTestTspLine::GenericDialogData(LPVOID lpvItemData, LPVOID lpParams, DWORD dwSize)
{
	LPCSTR lpByte = (LPCSTR) lpParams;

	OutputDebugString("TSPI_providerGenericDialogData\r\n");

	if (dwSize == 1 && *lpByte == 'A')
		ReceiveData(lpParams);
	
	if (lpByte && dwSize > 0)
	{
		DWORD dwCount = 0, dwLine = 0;
		while (dwCount < dwSize)
		{
			int i;
			char b[17], szBuff[20];
			wsprintf(szBuff, "%0.8lX   ", dwLine);
			OutputDebugString(szBuff);

			// Add the digits
			for (i = 0; i < 16; i++)
			{
				if (dwSize-dwCount > 0)
				{
					b[i] = *lpByte++;
					dwCount++;
					wsprintf(szBuff, "%0.2X ", (int)b[i]&0xff);
					OutputDebugString(szBuff);
				}
				else
				{
					OutputDebugString("   ");
					b[i] = 0;
				}

				// Change to a '.' if it is not printable
			    if (!isprint(b[i]))
			        b[i] = '.';
			}

			// Add the ascii portion
			b[16] = '\0';
			OutputDebugString("   ");
			OutputDebugString(b);
			OutputDebugString("\r\n");
			dwLine = dwCount;
		}
	}
	
    return 0;
}
