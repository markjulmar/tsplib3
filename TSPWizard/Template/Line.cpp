$$IF(VERBOSE)
/***************************************************************************
//
// LINE.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Line management functions
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

/*----------------------------------------------------------------------------
	GLOBALS AND CONSTANTS
-----------------------------------------------------------------------------*/
const UINT MAXCALLDATA_SIZE = 4096;

$$IF(VERBOSE)
/*-------------------------------------------------------------------------------*/
// TSPI Request map
/*-------------------------------------------------------------------------------*/
$$ENDIF
BEGIN_TSPI_REQUEST($$LINE_CLASS_NAME$$)
	ON_AUTO_TSPI_REQUEST(REQUEST_SETCALLDATA)
$$IF(LINEFEATURE_21)
	ON_AUTO_TSPI_REQUEST(REQUEST_RELEASEUSERINFO)
$$ENDIF
$$IF(LINEFEATURE_7)
$$IF(AGENT_SUPPORTSGROUPS)
	ON_TSPI_REQUEST_SETAGENTGROUP(OnSetAgentGroup)
$$ENDIF
$$IF(AGENT_SUPPORTSSTATES)	
	ON_TSPI_REQUEST_SETAGENTSTATE(OnSetAgentState)
$$ENDIF
$$IF(AGENT_SUPPORTSACTIVITIES)	
	ON_TSPI_REQUEST_SETAGENTACTIVITY(OnSetAgentActivity)
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_0)
	ON_TSPI_REQUEST_ACCEPT(OnAccept)
$$ENDIF
$$IF(LINEFEATURE_35)
	ON_TSPI_REQUEST_REDIRECT(OnRedirect)
$$ENDIF
$$IF(LINEFEATURE_3)
	ON_TSPI_REQUEST_BLINDXFER(OnBlindTransfer)
$$ENDIF
$$IF(LINEFEATURE_4)
	ON_TSPI_REQUEST_SETUPXFER(OnSetupTransfer)
	ON_TSPI_REQUEST_COMPLETEXFER(OnCompleteTransfer)
$$ENDIF
$$// Conferencing support
$$IF(LINEFEATURE_1)
	ON_TSPI_REQUEST_SETUPCONF(OnSetupConference)
	ON_TSPI_REQUEST_ADDCONF(OnAddToConference)
$$IF(CONFADDEXISTING)
	ON_TSPI_REQUEST_PREPAREADDCONF(OnPrepareAddToConference)
$$ENDIF
$$IF(CONFREMOVE)
	ON_TSPI_REQUEST_REMOVEFROMCONF(OnRemoveFromConference)
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_2)
	ON_TSPI_REQUEST_ANSWER(OnAnswer)
$$ENDIF
	ON_TSPI_REQUEST_MAKECALL(OnMakeCall)
	ON_TSPI_REQUEST_DIAL(OnDial)
	ON_TSPI_REQUEST_DROPCALL(OnDropCall)
$$IF(LINEFEATURE_14)
	ON_TSPI_REQUEST_HOLD(OnHoldCall)
	ON_TSPI_REQUEST_UNHOLD(OnRetrieveCall)
$$IF(HOLDCANSWAP)
	ON_TSPI_REQUEST_SWAPHOLD(OnSwapHold)
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_21)
$$IF(UUI_SEND)
	ON_TSPI_REQUEST_SENDUSERINFO(OnSendUUI)
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_12)
	ON_TSPI_REQUEST_GENERATEDIGITS(OnGenerateDigits)
$$ENDIF
$$IF(LINEFEATURE_5)
	ON_TSPI_REQUEST_COMPLETECALL(OnCompleteCall)
	ON_TSPI_REQUEST_UNCOMPLETECALL(OnUncompleteCall)
$$ENDIF
$$IF(LINEFEATURE_9)
	ON_TSPI_REQUEST_FORWARD(OnForward)
$$ENDIF
$$IF(LINEFEATURE_13)
	ON_TSPI_REQUEST_GENERATETONE(OnGenerateTone)
$$ENDIF
$$IF(LINEFEATURE_18)
	ON_TSPI_REQUEST_PARK(OnPark)
	ON_TSPI_REQUEST_UNPARK(OnUnpark)
$$ENDIF
$$IF(LINEFEATURE_19)
	ON_TSPI_REQUEST_PICKUP(OnPickup)
$$ENDIF
$$IF(LINEFEATURE_20)
	ON_TSPI_REQUEST_SECURECALL(OnSecureCall)
$$ENDIF
$$IF(LINEFEATURE_23)
	ON_TSPI_REQUEST_SETTERMINAL(OnSetTerminal)
$$ENDIF
$$IF(LINEFEATURE_26)
	ON_TSPI_REQUEST_SETQOS(OnSetQualityOfService)
$$ENDIF
$$IF(LINEFEATURE_27)
	ON_TSPI_REQUEST_SETCALLTREATMENT(OnSetCallTreatment)
$$ENDIF
END_TSPI_REQUEST()

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::$$LINE_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the line object
**
*****************************************************************************/
$$ENDIF
$$LINE_CLASS_NAME$$::$$LINE_CLASS_NAME$$()
{
	// TODO: place any constructor code here

}// $$LINE_CLASS_NAME$$::$$LINE_CLASS_NAME$$

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::~$$LINE_CLASS_NAME$$
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line object
**
*****************************************************************************/
$$ENDIF
$$LINE_CLASS_NAME$$::~$$LINE_CLASS_NAME$$()
{
	// TODO: destroy any allocated memory here

}// $$LINE_CLASS_NAME$$::~$$LINE_CLASS_NAME$$

$$IF(HAS_UI)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::read
**
** Arguments:  'istm' - Input stream
**
** Returns:    pointer to istm
**
** Description:  This function is called to serialize data in from the
**               registry.  The line object has already been completely
**               initialized by the TSP++ library
**
*****************************************************************************/
$$ENDIF
TStream& $$LINE_CLASS_NAME$$::read(TStream& istm)
{
	// Always call the base class to read in default line information
	CTSPILineConnection::read(istm);

	// TODO: Read any information stored in the line stream

    LPLINEDEVCAPS lpLineCaps = GetLineDevCaps();
	// TODO: Adjust the device capabilities for this line
$$// User user information
$$IF(LINEFEATURE_21)

    // Setup the USER->USER information sizes.
$$IF(UUI_ACCEPT)
    lpLineCaps->dwUUIAcceptSize	= $$UUI_ACCEPT$$;
$$ENDIF
$$IF(UUI_ANSWER)
	lpLineCaps->dwUUIAnswerSize	= $$UUI_ANSWER$$;
$$ENDIF
$$IF(UUI_MAKECALL)
	lpLineCaps->dwUUIMakeCallSize = $$UUI_MAKECALL$$;
$$ENDIF
$$IF(UUI_DROP)
    lpLineCaps->dwUUIDropSize = $$UUI_DROP$$;
$$ENDIF
$$IF(UUI_SEND)
	lpLineCaps->dwUUISendUserUserInfoSize = $$UUI_SEND$$;
$$ENDIF
	lpLineCaps->dwUUICallInfoSize = $$UUI_MAXSIZE$$;
$$ENDIF
$$IF(!HASTAPI3MSP)

	// TODO: Add any WAV devices which will be used for I/O - locate your WAV drive
	// and use the device identifier given by the multimedia system.
	// AddDeviceClass(_T("wave/in"), dwWaveInDeviceID);
	// AddDeviceClass(_T("wave/out"), dwWaveOutDeviceID);
$$ENDIF

	// Configure the line based on the type
	switch (GetLineType())
	{
		case Station:			InitializeStation(); break;
$$IF(LINEFEATURE_6)
		case RoutePoint:		InitializeRoutePoint(); break;
$$ENDIF
$$IF(LINEFEATURE_31)
		case Queue:				InitializeQueue(); break;
$$ENDIF
$$IF(LINEFEATURE_32)
		case PredictiveDialer:	InitializePredictiveDialer(); break;
$$ENDIF
$$IF(LINEFEATURE_33)
		case VRU:				InitializeVRU(); break;
$$ENDIF
$$IF(LINEFEATURE_37)
		case Trunk:				InitializeTrunk(); break;
$$ENDIF
		default:				break;
	}

	return istm;

}// $$LINE_CLASS_NAME$$::read
$$ELSE
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::Init
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
$$ENDIF
void $$LINE_CLASS_NAME$$::Init(CTSPIDevice* pDev, DWORD dwLineDeviceID, DWORD dwPos, DWORD dwItemData)
{
    LPLINEDEVCAPS lpLineCaps = GetLineDevCaps();

	// Let the base class initialize first.
	CTSPILineConnection::Init (pDev, dwLineDeviceID, dwPos, dwItemData);

$$IF(VERBOSE)
    // Set the name associated with this line.  This is optional, it gives
    // the user a displayable name which is associated with the line.  Most
	// applications use this name in their UI.
$$ENDIF
	TCHAR chBuff[50];
	wsprintf(chBuff, _T("Line Device %ld"), dwLineDeviceID);
    SetName (chBuff);

$$// User user information
$$IF(LINEFEATURE_21)
    // Setup the USER->USER information sizes.
$$IF(UUI_ACCEPT)
    lpLineCaps->dwUUIAcceptSize	= $$UUI_ACCEPT$$;
$$ENDIF
$$IF(UUI_ANSWER)
	lpLineCaps->dwUUIAnswerSize	= $$UUI_ANSWER$$;
$$ENDIF
$$IF(UUI_MAKECALL)
	lpLineCaps->dwUUIMakeCallSize = $$UUI_MAKECALL$$;
$$ENDIF
$$IF(UUI_DROP)
    lpLineCaps->dwUUIDropSize = $$UUI_DROP$$;
$$ENDIF
$$IF(UUI_SEND)
	lpLineCaps->dwUUISendUserUserInfoSize = $$UUI_SEND$$;
$$ENDIF
	lpLineCaps->dwUUICallInfoSize = $$UUI_MAXSIZE$$;

$$ENDIF
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
$$IF(HASTAPI3MSP)

	// TODO: Set the MSP guid for this line device. TAPI3 has two pre-defined MSPs
	// which come with Windows 2000 --  H323.msp and IPConf.msp
	// Additional MSPs may be written for a particular protocol or physical device.
	//
	// The following line implements the TSPI_MSPIdentify functionallity for this provider.
	// SetMSPGUID( CLSID_MyMSPDriver );
$$ELSE

	// TODO: Add any WAV devices which will be used for I/O - locate your WAV drive
	// and use the device identifier given by the multimedia system.
	// AddDeviceClass(_T("wave/in"), dwWaveInDeviceID);
	// AddDeviceClass(_T("wave/out"), dwWaveOutDeviceID);
$$ENDIF
$$IF(ISTAPI3)

	// Set the protocol GUID for this line device -- this is by default set to
	// PSTN, you only need to do this if you want to use a different protocol or 
	// define your own.
	// 
	// SetProtocolCLSID(TAPIPROTOCOL_xxx);
$$ENDIF

	// Configure the line based on the type - this requires that it was set above
	// using the SetLineType function.
	switch (GetLineType())
	{
		case Station:			InitializeStation(); break;
$$IF(LINEFEATURE_6)
		case RoutePoint:		InitializeRoutePoint(); break;
$$ENDIF
$$IF(LINEFEATURE_31)
		case Queue:				InitializeQueue(); break;
$$ENDIF
$$IF(LINEFEATURE_32)
		case PredictiveDialer:	InitializePredictiveDialer(); break;
$$ENDIF
$$IF(LINEFEATURE_33)
		case VRU:				InitializeVRU(); break;
$$ENDIF
$$IF(LINEFEATURE_37)
		case Trunk:				InitializeTrunk(); break;
$$ENDIF
		default:				break;
	}

}// $$LINE_CLASS_NAME$$::Init
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitializeStation
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function is called to initialize a station line object.
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitializeStation()
{
	LPLINEDEVCAPS lpCaps = GetLineDevCaps();

$$IF(LINEFEATURE_2)
	// Answer mode information
	lpCaps->dwAnswerMode = $$ANSWERMODE$$;
$$ENDIF
$$IF(LINEFEATURE_12)
	// Digit generation parameters
	lpCaps->dwGenerateDigitModes = $$GENERATEDIGIT_MODE$$;
$$ENDIF
$$IF(LINEFEATURE_22)
	// Media control capabilities are supported through lineSetMediaControl
	lpCaps->dwDevCapFlags |= LINEDEVCAPFLAGS_MEDIACONTROL;
$$ENDIF
$$IF(LINEFEATURE_34)
	// Line supports inverse multiplexing and can dial multiple addresses on the
	// same call
	lpCaps->dwDevCapFlags |= LINEDEVCAPFLAGS_MULTIPLEADDR;
$$ENDIF
$$IF(LINEFEATURE_36)
	// Line supports special dialing characters
	lpCaps->dwDevCapFlags |= ($$DIALCHAR_FLAGS$$);
$$ENDIF
$$IF(LINEFEATURE_13)
	// Tone generation parameters
    lpCaps->dwGenerateToneModes = $$GENERATETONE_MODES$$;
    lpCaps->dwGenerateToneMaxNumFreq = $$GENERATETONE_MAXTONES$$;
$$ENDIF
$$IF(LINEFEATURE_11)
	// Tone monitoring support
    lpCaps->dwMonitorToneMaxNumFreq = $$MONITORTONE_MAXFREQ$$;
    lpCaps->dwMonitorToneMaxNumEntries = $$MONITORTONE_MAXTONES$$;
$$ENDIF
$$IF(LINEFEATURE_10)
	// Digit monitoring support
    lpCaps->dwMonitorDigitModes = $$MONITORDIGIT_MODES$$;
	// Digit gathering support
    lpCaps->dwGatherDigitsMinTimeout = 500;		// Minimum supported by TSP++
    lpCaps->dwGatherDigitsMaxTimeout = 32000;   
$$ENDIF
$$IF(LINEFEATURE_22)
	// Media control support
	lpCaps->dwMedCtlCallStateMaxListSize = $$MEDIACTL_MAXCALLSTATES$$;
	lpCaps->dwMedCtlDigitMaxListSize = $$MEDIACTL_MAXDIGITS$$;
	lpCaps->dwMedCtlMediaMaxListSize = $$MEDIACTL_MAXMEDIAMODES$$;
	lpCaps->dwMedCtlToneMaxListSize = $$MEDIACTL_MAXTONES$$;
$$ENDIF
$$IF(LINEFEATURE_15)
$$IF(DIALPARMS_MINMAX)
	// Minimum dialing parameters
	lpCaps->MinDialParams.dwDialPause = $$DIALPARMS_MINPAUSE$$;
	lpCaps->MinDialParams.dwDialSpeed = $$DIALPARMS_MINDIGIT$$;
	lpCaps->MinDialParams.dwDigitDuration = $$DIALPARMS_MINDURATION$$;
	lpCaps->MinDialParams.dwWaitForDialtone = $$DIALPARMS_MINDIALTONE$$;
	// Maximum dialing parameters
	lpCaps->MinDialParams.dwDialPause = $$DIALPARMS_MAXPAUSE$$;
	lpCaps->MinDialParams.dwDialSpeed = $$DIALPARMS_MAXDIGIT$$;
	lpCaps->MinDialParams.dwDigitDuration = $$DIALPARMS_MAXDURATION$$;
	lpCaps->MinDialParams.dwWaitForDialtone = $$DIALPARMS_MAXDIALTONE$$;
$$ENDIF
	// Default dialing parameters
	lpCaps->DefaultDialParams.dwDialPause = $$DIALPARMS_DEFPAUSE$$;
	lpCaps->DefaultDialParams.dwDialSpeed = $$DIALPARMS_DEFDIGIT$$;
	lpCaps->DefaultDialParams.dwDigitDuration = $$DIALPARMS_DEFDURATION$$;
	lpCaps->DefaultDialParams.dwWaitForDialtone = $$DIALPARMS_DEFDIALTONE$$;
$$ENDIF
$$IF(LINEFEATURE_1)
$$IF(CONFCROSSADDR)
	// Conferences can be established between two addresses on this line
	lpCaps->dwDevCapFlags |= LINEDEVCAPFLAGS_CROSSADDRCONF;
$$ENDIF
$$ENDIF
$$IF(LINECLOSEDROP)
	// Closing a line using TAPI will drop active calls on that line.
    lpCaps->dwDevCapFlags |= LINEDEVCAPFLAGS_CLOSEDROP;
$$ENDIF
$$IF(LINEFEATURE_8)

	// TODO: Set the valid extension versions for the device-specific extensions
	// This information is used during the lineSelectExtVersion function.
	SetExtVersionInfo(
			0,					// Minimum version
			1,					// Maximum version
			0,					// Extension ID 0
			0,					// Extension ID 1
			0,					// Extension ID 2
			1);					// Extension ID 3
$$ENDIF
$$IF(LINEFEATURE_23)
    // Add the terminal devices supported on this hardware.
    LINETERMCAPS tCaps;
$$BEGINLOOP(TERMINAL_COUNT)
    tCaps.dwTermDev		= $$TERMINAL_DEVTYPE$$;
    tCaps.dwTermModes	= $$TERMINAL_MODES$$;
    tCaps.dwTermSharing = $$TERMINAL_SHARING$$;
    AddTerminal (_T("$$TERMINAL_NAME$$"), tCaps, $$TERMINAL_DEFMODES$$);
$$ENDLOOP
$$ENDIF

$$IF(!HAS_UI)
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Read the address configuration in from some persistant storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[50];
		wsprintf(chBuff, _T("Address %d"), iAddress);	
		CreateAddress(	
					_T("100"),				// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
$$IF(ALLOWINCOMINGCALLS)
					true,					// Allow incoming calls
$$ELSE
					false,					// Do not allow incoming call traffic
$$ENDIF
$$IF(ALLOWOUTGOINGCALLS)
					true,					// Allow outgoing calls
$$ELSE
					false,					// Do not allow outgoing calls
$$ENDIF
					$$MEDIAMODES$$,
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					$$NUMACTIVECALLS$$,		// Max number of active calls on address
					$$MAXONHOLD$$,			// Max number of OnHold calls on address
					$$TRANSF_MAXONHOLD$$,	// Max number of OnHoldPending calls on address
					$$CONFMAXCOUNT$$,		// Max number of calls in a conference on address
					$$CONFTRANSFMAXCOUNT$$);	// Max number of calls transferred into a conference on address
$$ELSE
	// TODO: Adjust the ADDRESSCAPS based on this being a station
	for (int iAddress = 0; iAddress < GetAddressCount(); iAddress++)
	{
$$ENDIF
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a station capabilities.
		// Look at the $$LINE_CLASS_NAME$$::InitAddress function to see how the
		// address is configured for all lines.
$$IF(LINEFEATURE_7)

		// Adjust the agent features for this station.
		TAgentCaps* pAgentCaps = pAddress->GetAgentCaps();
		pAgentCaps->dwFeatures = $$AGENT_FEATURES$$;
		pAgentCaps->dwStates = $$AGENT_STATES$$;
		pAgentCaps->dwNextStates = $$AGENT_STATES$$;
		pAgentCaps->dwAgentStatusMessages = (LINEAGENTSTATUS_STATE | LINEAGENTSTATUS_NEXTSTATE |
					LINEAGENTSTATUS_VALIDSTATES | LINEAGENTSTATUS_VALIDNEXTSTATES);
		pAgentCaps->dwMaxNumGroupEntries = $$AGENT_MAXGROUPS$$;

		// Adjust the AGENT status for this address.  Further adjustments
		// will be made when we see whether an agent is logged on or not.
		TAgentStatus* pAgentStatus = pAddress->GetAgentStatus();
		pAgentStatus->dwState = LINEAGENTSTATE_UNKNOWN;
		pAgentStatus->dwNextState = LINEAGENTSTATE_UNKNOWN;

		// Initialize our current agent state to unknown until we know what the device
		// is really set to. Note: if this cannot be determined, the initial state should
		// be set to LINEAGENTSTATE_LOGGEDOFF.
		pAddress->GetAgentStatus()->dwState = LINEAGENTSTATE_UNKNOWN;
$$ENDIF
	}
$$IF(LINEFEATURE_7)

	// TODO: Uncomment the below line if desired:
	// If the line is marked as "out of service" until we see an agent logon, it will
	// stop all activity from TAPI against the line until an agent logs on.
	// SetDeviceStatusFlags(GetLineDevStatus()->dwDevStatusFlags & ~LINEDEVSTATUSFLAGS_INSERVICE);

	// Enable the agent support on this line
	EnableAgentProxy();
$$ENDIF

}// $$LINE_CLASS_NAME$$::InitializeStation
$$IF(LINEFEATURE_6)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitializeRoutePoint
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function is called to initialize a route point object
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitializeRoutePoint()
{
$$IF(LINEFEATURE_2)
	// Set what happens to an active call when a call is answered.
	GetLineDevCaps()->dwAnswerMode = $$ANSWERMODE$$;
$$ENDIF

$$IF(!HAS_UI)
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Read the address configuration in from some persistant storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[50];
		wsprintf(chBuff, _T("Route Point %d"), iAddress);	
		CreateAddress(	
					_T("100"),				// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
					true,					// Allow incoming calls
					false,					// Do not allow outgoing calls
					$$MEDIAMODES$$,
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					0xffffffff,				// Max number of active calls on address
					0,						// Max number of OnHold calls on address
					0,						// Max number of OnHoldPending calls on address
					0,						// Max number of calls in a conference on address
					0);						// Max number of calls transferred into a conference on address
$$ELSE
	// TODO: Adjust the ADDRESSCAPS based on this being a route point
	for (int iAddress = 0; iAddress < GetAddressCount(); iAddress++)
	{
$$ENDIF
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a route point's capabilities.
		// Look at the $$LINE_CLASS_NAME$$::InitAddress function to see how the
		// address is configured for all lines.
		LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();

		// TODO: Adjust the call features for what the route point is capable of.
		lpAddrCaps->dwCallFeatures = (
			LINECALLFEATURE_DROP |					// Allow drop calls
$$IF(LINEFEATURE_35)
			LINECALLFEATURE_REDIRECT |				// Allow redirect calls ringing here 
$$ENDIF
$$IF(LINEFEATURE_21)
			LINECALLFEATURE_RELEASEUSERUSERINFO |	// Allow release useruser information
$$ENDIF
			LINECALLFEATURE_SETCALLDATA);			// Allow set call data on calls
	}

}// $$LINE_CLASS_NAME$$::InitializeRoutePoint
$$ENDIF
$$IF(LINEFEATURE_31)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitializeQueue
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function is called to initialize an ACD queue
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitializeQueue()
{
$$IF(VERBOSE)
	// Queue's shouldn't support answering calls
$$ENDIF
	GetLineDevCaps()->dwAnswerMode = LINEANSWERMODE_NONE;

$$IF(!HAS_UI)
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Read the address configuration in from some persistent storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[50];
		wsprintf(chBuff, _T("ACD Queue %d"), iAddress);	
		CreateAddress(	
					_T("100"),				// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
					true,					// Allow incoming calls
					false,					// Do not allow outgoing calls
					$$MEDIAMODES$$,
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					0xffffffff,				// Max number of active calls on address
					0xffffffff,				// Max number of OnHold calls on address
					0xffffffff,				// Max number of OnHoldPending calls on address
					0,						// Max number of calls in a conference on address
					0);						// Max number of calls transferred into a conference on address
$$ELSE
	// TODO: Adjust the ADDRESSCAPS based on this being a route point
	for (int iAddress = 0; iAddress < GetAddressCount(); iAddress++)
	{
$$ENDIF
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a queue's capabilities.
		// Look at the $$LINE_CLASS_NAME$$::InitAddress function to see how the
		// address is configured for all lines.
		LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();

		// TODO: Adjust the call features for what the queue is capable of.
		lpAddrCaps->dwCallFeatures = (
			LINECALLFEATURE_DROP |					// Allow drop calls
$$IF(LINEFEATURE_35)
			LINECALLFEATURE_REDIRECT |				// Allow redirect calls from the queue
$$ENDIF
$$IF(LINEFEATURE_21)
			LINECALLFEATURE_RELEASEUSERUSERINFO |	// Allow release useruser information
$$ENDIF
			LINECALLFEATURE_SETCALLDATA);			// Allow set call data on calls
	}

}// $$LINE_CLASS_NAME$$::InitializeQueue
$$ENDIF
$$IF(LINEFEATURE_32)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitializePredictiveDialer
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function is called to initialize a predictive dialer
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitializePredictiveDialer()
{
$$IF(VERBOSE)
	// Dialers don't generally support incoming calls.
$$ENDIF
	GetLineDevCaps()->dwAnswerMode = LINEANSWERMODE_NONE;

$$IF(!HAS_UI)
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Read the address configuration in from some persistent storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[50];
		wsprintf(chBuff, _T("Dialer %d"), iAddress);	
		CreateAddress(	
					_T("100"),				// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
					false,					// Do not allow incoming calls
					true,					// Allow outgoing calls
					$$MEDIAMODES$$,
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					$$PDIAL_MAXCALLS$$,		// Max number of active calls on address
					0,						// Max number of OnHold calls on address
					0,						// Max number of OnHoldPending calls on address
					0,						// Max number of calls in a conference on address
					0);						// Max number of calls transferred into a conference on address
$$ELSE
	// TODO: Adjust the ADDRESSCAPS based on this being a route point
	for (int iAddress = 0; iAddress < GetAddressCount(); iAddress++)
	{
$$ENDIF
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a dialer's capabilities.
		// Look at the $$LINE_CLASS_NAME$$::InitAddress function to see how the
		// address is configured for all lines.
		LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();

$$IF(VERBOSE)
		// Set the predictive dialer properties
$$ENDIF
		lpAddrCaps->dwPredictiveAutoTransferStates = $$PDIAL_STATES$$;
		lpAddrCaps->dwMaxNoAnswerTimeout = $$PDIAL_MAXTIMEOUT$$;

		// TODO: Adjust the call features for what the device's dialer is capable of.
		lpAddrCaps->dwCallFeatures = (
			LINECALLFEATURE_DROP |					// Allow drop calls
$$IF(LINEFEATURE_21)
			LINECALLFEATURE_RELEASEUSERUSERINFO |	// Allow release useruser information
$$ENDIF
			LINECALLFEATURE_SETCALLDATA);			// Allow set call data on calls

		// TODO: Adjust the address features
		pAddress->SetAddressFeatures(LINEADDRFEATURE_MAKECALL);
	}

	// Recalculate our line features based on the address features
	RecalcLineFeatures();

}// $$LINE_CLASS_NAME$$::InitializePredictiveDialer
$$ENDIF
$$IF(LINEFEATURE_33)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitializeVRU
**
** Arguments:  void
**
** Returns:    void
**
** Description:  This function is called to initialize a IVR device
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitializeVRU()
{
$$IF(VERBOSE)
	// VRU's don't generally support answering calls programmatically - 
	// the answer is completed by the ACD.
$$ENDIF
	GetLineDevCaps()->dwAnswerMode = LINEANSWERMODE_NONE;

$$IF(LINEFEATURE_12)
	// Do not allow digit generation
	GetLineDevCaps()->dwGenerateDigitModes = 0;
$$ENDIF
$$IF(LINEFEATURE_10)
	// Allow digit monitoring on the line
	GetLineDevCaps()->dwMonitorDigitModes = $$MONITORDIGIT_MODES$$;
$$ENDIF

$$IF(!HAS_UI)
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Read the address configuration in from some persistant storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[50];
		wsprintf(chBuff, _T("VRU %d"), iAddress);	
		CreateAddress(	
					_T("100"),				// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
					true,					// Allow incoming calls
					false,					// Do not allow outgoing calls
					LINEMEDIAMODE_AUTOMATEDVOICE, // Support automated voice only
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					$$NUMACTIVECALLS$$,		// Max number of active calls on address
					$$MAXONHOLD$$,			// Max number of OnHold calls on address
					$$TRANSF_MAXONHOLD$$,	// Max number of OnHoldPending calls on address
					0,						// Max number of calls in a conference on address
					0);						// Max number of calls transferred into a conference on address
$$ELSE
	// TODO: Adjust the ADDRESSCAPS based on this being a route point
	for (int iAddress = 0; iAddress < GetAddressCount(); iAddress++)
	{
$$ENDIF
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a VRUs capabilities.
		// Look at the $$LINE_CLASS_NAME$$::InitAddress function to see how the
		// address is configured for all lines.
		LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();

		// TODO: Adjust the call features for what the device's VRU is capable of.
		lpAddrCaps->dwCallFeatures = (
			LINECALLFEATURE_DROP |					// Allow drop calls
$$IF(LINEFEATURE_35)
			LINECALLFEATURE_REDIRECT |				// Allow redirect calls ringing here 
$$ENDIF
$$IF(LINEFEATURE_3)
			LINECALLFEATURE_BLINDTRANSFER |			// Allow blind transfers on answered calls
$$ENDIF
$$IF(LINEFEATURE_21)
			LINECALLFEATURE_RELEASEUSERUSERINFO |	// Allow release useruser information
$$ENDIF
			LINECALLFEATURE_SETCALLDATA);			// Allow set call data on calls

		// Turn off all the address features - most VRUs don't support
		// the creation of calls by the link software.
		pAddress->SetAddressFeatures(0);
	}

	// Turn off all the line features since VRUs cannot do anything
	// listed in the LINEFEATURE_xxx constants.
	SetLineFeatures(0);

}// $$LINE_CLASS_NAME$$::InitializeVRU
$$ENDIF
$$IF(LINEFEATURE_37)

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitializeTrunk
**
** Arguments:  void
**
** Returns:    void
**
** Description: This function is called to initialize a trunk device.
**              The trunk device generally represents the end-point to a
**              call which is off the switch. 
**
**              The assumption for this code is that the trunk device
**              is used to record an end-point for a conversation on the
**              switch. It doesn't require any features or abilities since
**              the call on the trunk presumably has another side on another
**              line (station, VRU, queue, etc.) where it can be controlled.
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitializeTrunk()
{
$$IF(VERBOSE)
	// Trunks don't generally support answering calls programmatically - 
	// the call would be associated to a route point, queue or station
	// first (where it would be answered).
$$ENDIF
	GetLineDevCaps()->dwAnswerMode = LINEANSWERMODE_NONE;

$$IF(LINEFEATURE_12)
	// Do not allow digit generation
	GetLineDevCaps()->dwGenerateDigitModes = 0;
$$ENDIF
$$IF(LINEFEATURE_10)
	// Allow digit monitoring on the line
	GetLineDevCaps()->dwMonitorDigitModes = $$MONITORDIGIT_MODES$$;
$$ENDIF

$$IF(!HAS_UI)
$$IF(VERBOSE)
    // Create each dialable number (address) which exists on each line.
$$ENDIF
	for (int iAddress = 0; iAddress < $$LINEADDRESS_COUNT$$; iAddress++)
	{
		// TODO: Read the address configuration in from some persistent storage.
		// Specifically, read the dialable address (DN) so it can be stored into 
		// the address.
		TCHAR chBuff[20];
		wsprintf(chBuff, _T("Trunk %d"), iAddress);	
		CreateAddress(	
					_T(""),					// Dialable address (extension or phone#)
					chBuff,					// Address name (used for identification)
					true,					// Allow incoming calls
					false,					// Do not allow outgoing calls
					$$MEDIAMODES$$,
					LINEBEARERMODE_VOICE,	// Single bearer mode for this address
                    0,						// Minimum data rate on address
					0,						// Maximum data rate on address
					NULL,					// Dialing parameters (LINEDIALPARAMS)
					1,						// Max number of active calls on address
					1,						// Max number of OnHold calls on address
					1,						// Max number of OnHoldPending calls on address
					0,						// Max number of calls in a conference on address
					0);						// Max number of calls transferred into a conference on address
$$ELSE
	// TODO: Adjust the ADDRESSCAPS based on this being a trunk device
	for (int iAddress = 0; iAddress < GetAddressCount(); iAddress++)
	{
$$ENDIF
		CTSPIAddressInfo* pAddress = GetAddress(iAddress);
		_TSP_ASSERTE (pAddress != NULL);

		// Initialize the address information to be the basic set of information
		InitAddress(pAddress);

		// TODO: Adjust the address capabilities based on a trunks capabilities.
		// Look at the $$LINE_CLASS_NAME$$::InitAddress function to see how the
		// address is configured for all lines.

		LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();

		// TODO: Adjust the call features for what the device's trunk 
		// link software is capable of.
		lpAddrCaps->dwCallFeatures = (
			LINECALLFEATURE_DROP |					// Allow drop calls
$$IF(LINEFEATURE_35)
			LINECALLFEATURE_REDIRECT |				// Allow redirect calls ringing here 
$$ENDIF
$$IF(LINEFEATURE_3)
			LINECALLFEATURE_BLINDTRANSFER |			// Allow blind transfers on answered calls
$$ENDIF
$$IF(LINEFEATURE_21)
			LINECALLFEATURE_RELEASEUSERUSERINFO |	// Allow release useruser information
$$ENDIF
			LINECALLFEATURE_SETCALLDATA);			// Allow set call data on calls

		// Turn off all the address features - outgoing calls would be created
		// on stations or some other type of line and then associated to the
		// trunk line when it is seized.
		pAddress->SetAddressFeatures(0);
	}

	// Turn off all the line features - assume the trunk cannot do anything
	// listed in the LINEFEATURE_xxx constants.
	SetLineFeatures(0);

}// $$LINE_CLASS_NAME$$::InitializeTrunk
$$ENDIF

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::InitAddress
**
** Arguments:  'pAddr' - Address object to initialize
**
** Returns:    void
**
** Description:  This function is called to initialize a single address
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::InitAddress(CTSPIAddressInfo* pAddress)
{
$$IF(LINEFEATURE_5)
$$IF(CALLCOMPLMSG)
	static LPCTSTR pszComplMessage[] = {
$$BEGINLOOP(CALLCOMPLMSG_COUNT)
		$$CALLCOMPLMSG$$
$$ENDLOOP
	};

$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_27)
	static struct {
		DWORD dwIdentifier;
		LPCTSTR pszText;
	} g_arrCallTreatments[] = {
$$BEGINLOOP(CALLTREATMENT_COUNT)
		$$CALLTREATMENTMSG$$
$$ENDLOOP
	};

$$ENDIF
	_TSP_ASSERTE (pAddress != NULL);
	LPLINEADDRESSCAPS lpAddrCaps = pAddress->GetAddressCaps();
	lpAddrCaps->dwMaxCallDataSize = MAXCALLDATA_SIZE;
$$IF(LINEFEATURE_5)

	// Add all of the call completion information to the line.
$$IF(CALLCOMPLMSG)
	for (int iComplMsg = 0; iComplMsg < $$CALLCOMPLMSG_COUNT$$; iComplMsg++)
		pAddress->AddCompletionMessage(pszComplMessage[iComplMsg]);
$$ENDIF
	lpAddrCaps->dwMaxCallCompletions = $$CALLCOMPL_MAX$$;
	lpAddrCaps->dwCallCompletionModes = $$CALLCOMPL_MODES$$;
	lpAddrCaps->dwCallCompletionConds = $$CALLCOMPL_COND$$;
$$ENDIF
$$IF(LINEFEATURE_27)

	// Add all the call treatments
	for (int iCallTreatments = 0; iCallTreatments < $$CALLTREATMENT_COUNT$$; iCallTreatments++)
		pAddress->AddCallTreatment(g_arrCallTreatments[iCallTreatments].dwIdentifier, g_arrCallTreatments[iCallTreatments].pszText);
$$ENDIF
$$IF(LINEFEATURE_4)

	// Change the transfer parameters
$$IF(CONFADDTRANSFER)
	lpAddrCaps->dwTransferModes = LINETRANSFERMODE_TRANSFER | LINETRANSFERMODE_CONFERENCE;
$$ELSE
	lpAddrCaps->dwTransferModes = LINETRANSFERMODE_TRANSFER;
$$ENDIF
$$IF(TRANSF_HELD)
	// Any hard held call may be transferred (vs. a consultation hold)
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_TRANSFERHELD;
$$ENDIF
$$IF(TRANSF_MAKECALL)
	// Any call may be used as a consultation call for transfer
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_TRANSFERMAKE;
$$ENDIF
$$ENDIF

$$IF(LINEFEATURE_30)
$$IF(CALLERID_FLAGS)
	// Update the caller-id flags
	lpAddrCaps->dwCallerIDFlags =
	lpAddrCaps->dwCalledIDFlags =
$$IF(LINEFEATURE_3 || LINEFEATURE_4 || LINEFEATURE_9 || LINEFEATURE_35 || LINEFEATURE_31)
	lpAddrCaps->dwConnectedIDFlags = 
	lpAddrCaps->dwRedirectionIDFlags =
	lpAddrCaps->dwRedirectingIDFlags = $$CALLERID_FLAGS$$;
$$ELSE
	lpAddrCaps->dwConnectedIDFlags = $$CALLERID_FLAGS$$;
	lpAddrCaps->dwRedirectionIDFlags =
	lpAddrCaps->dwRedirectingIDFlags = LINECALLPARTYID_UNAVAIL;
$$ENDIF
$$ENDIF
$$ELSE
	// Update the caller-id flags
	lpAddrCaps->dwCallerIDFlags =
	lpAddrCaps->dwCalledIDFlags =
	lpAddrCaps->dwConnectedIDFlags = 
	lpAddrCaps->dwRedirectionIDFlags =
	lpAddrCaps->dwRedirectingIDFlags = LINECALLPARTYID_UNAVAIL;
$$ENDIF
$$IF(LINEFEATURE_9)

	// Change the forwarding parameters
	lpAddrCaps->dwForwardModes = $$FWDMODES$$;
	lpAddrCaps->dwMaxForwardEntries = $$FWD_MAXENTRIES$$;
	lpAddrCaps->dwMaxSpecificEntries = $$FWD_MAXSPECIFIC$$;
	lpAddrCaps->dwMinFwdNumRings = $$FWD_MINRINGS$$;
	lpAddrCaps->dwMaxFwdNumRings = $$FWD_MAXRINGS$$;
$$IF(FWD_REQUIRESCONSULT)
	// Forward request creates a consultation call
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_FWDCONSULT;
$$ENDIF
$$IF(FWD_STATUSVALID)
	// The status reported in the LINEADDRESSSTATUS structure is based on hardware reporting
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_FWDSTATUSVALID;
$$ENDIF
$$IF(FWD_INTEXTSEPERATE)
	// Internal and external calls may be forwarded seperately
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_FWDINTEXTADDR;
$$ENDIF
$$IF(FWD_BUSYNASEPERATE)
	// Busy and no answer responses may be forwarded seperately
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_FWDBUSYNAADDR;
$$ENDIF
$$ENDIF
	// Set the dialtone modes
$$// Dialtone modes
$$IF(LINEFEATURE_29)
	lpAddrCaps->dwDialToneModes = $$DIALTONE_MODES$$;
$$ELSE
	lpAddrCaps->dwDialToneModes &= ~(LINEDIALTONEMODE_INTERNAL | LINEDIALTONEMODE_EXTERNAL | LINEDIALTONEMODE_SPECIAL);
$$ENDIF
$$IF(LINEFEATURE_18)

	// Change the park features
	lpAddrCaps->dwParkModes = $$PARK_MODE$$;
$$ENDIF
$$IF(LINEFEATURE_1)

	// Change the conference parameters
	lpAddrCaps->dwRemoveFromConfCaps = $$CONFREMOVE_TYPE$$;
	lpAddrCaps->dwRemoveFromConfState = $$CONFREMOVE_STATE$$;
$$IF(CONFCANDROP)
	// A conference call can be dropped
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_CONFDROP;
$$ENDIF
$$IF(CONFADDEXISTING)
	// Existing calls (non-consultation) can be added to a conference
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_CONFERENCEMAKE;
$$ENDIF
$$IF(CONFADDHELD)
	// Hard hold call may be conferenced in
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_CONFERENCEHELD;
$$ENDIF
$$IF(CONFCREATENOCALL)
	// Can create a conference w/o an existing call
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_SETUPCONFNULL;
$$ENDIF
$$ENDIF

$$IF(VERBOSE)
	// TODO: Adjust the address capabilities to reflect the device
	// features and abilities. The wizard has set the following flags
	// for you based on your requested feature set:

$$ENDIF
$$IF(LINEFEATURE_28)
	// A partial number is accepted within a dial string.
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_PARTIALDIAL;
$$ENDIF
$$IF(!ALLOWOUTGOINGCALLS)
	// Destination is fixed for this address - cannot dial to other numbers
	lpAddrCaps->dwAddrCapFlags &= ~LINEADDRCAPFLAGS_DIALED;
$$ENDIF
$$IF(LINEFEATURE_29)
$$IF(!DIALTONE_HASINTERNAL)
	// Line cannot place internal calls
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_NOINTERNALCALLS;
$$ELSE
$$IF(!DIALTONE_HASEXTERNAL)
	// Line cannot place external calls
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_NOEXTERNALCALLS;
$$ENDIF
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_0)
$$IF(ACCEPTTOALERT)
	// Calls must be accepted before they begin to ring
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_ACCEPTTOALERT;
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_14)
$$IF(HOLDMAKESNEW)
	// Placing a call on hold results in a new call
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_HOLDMAKESNEW;
$$ENDIF
$$ENDIF
$$IF(!LINEFEATURE_16)
	// The phone is not automatically taken off hook
	lpAddrCaps->dwAddrCapFlags &= ~LINEADDRCAPFLAGS_ORIGOFFHOOK;
$$ENDIF
$$IF(LINEFEATURE_17)
	// The phone is automatically taken off hook on reception
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_DESTOFFHOOK;
$$ENDIF
$$IF(LINEFEATURE_30)
$$IF(CALLERID_BLOCKED)
	// Callerid information is automatically blocked on outgoing calls
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_BLOCKIDDEFAULT;
$$ENDIF
$$IF(CALLERID_OVERRIDE)
	// Callerid block can be overridden on a call-by-call basis
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_BLOCKIDOVERRIDE;
$$ENDIF
$$IF(CALLERID_APPSUPPLIED)
	// Callerid can be supplied by application in LINECALLPARAMS
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_SETCALLINGID;
$$ENDIF
$$ENDIF
$$IF(DROPCALLRECONNECTS)
	// Dropping a consultation call reconnects to the original party
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_AUTORECONNECT;
$$ENDIF
$$IF(LINEFEATURE_19)
$$IF(PICKUP_GROUPREQ)
	// Call pickup requires a group identifier
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_PICKUPGROUPID;
$$ENDIF
$$IF(PICKUP_CALLWAIT)
	// Call pickup may be used to retrieve a call waiting call
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_PICKUPCALLWAIT;
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_20)
$$IF(SECURE_ONCREATE)
	// Calls may be secured when they are created
	lpAddrCaps->dwAddrCapFlags |= LINEADDRCAPFLAGS_SECURE;
$$ENDIF
$$ENDIF

}// $$LINE_CLASS_NAME$$::InitAddress

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnAddressFeaturesChanged
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
$$ENDIF
DWORD $$LINE_CLASS_NAME$$::OnAddressFeaturesChanged (CTSPIAddressInfo* pAddr, DWORD dwFeatures)
{
	// TODO: Adjust any features for the address based on the current state of the
	// hardware. This is where you can restrict the features shown in the provider for
	// special cases.
	return CTSPILineConnection::OnAddressFeaturesChanged(pAddr, dwFeatures);

}// $$LINE_CLASS_NAME$$::OnAddressFeaturesChanged

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnLineFeaturesChanged
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
$$ENDIF
DWORD $$LINE_CLASS_NAME$$::OnLineFeaturesChanged (DWORD dwFeatures)
{
	// TODO: Adjust any features for the line based on the current state of the
	// hardware. This is where you can restrict the features shown in the provider for
	// special cases.
	return CTSPILineConnection::OnLineFeaturesChanged(dwFeatures);

}// $$LINE_CLASS_NAME$$::OnLineFeaturesChanged

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnCallFeaturesChanged
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
$$ENDIF
DWORD $$LINE_CLASS_NAME$$::OnCallFeaturesChanged(CTSPICallAppearance* pCall, DWORD dwCallFeatures)
{      
$$IF(LINEFEATURE_3)
$$IF(BLINDXFER_NOINT)
	// If the call is not on a trunk, then disallow any blind transfer events.
	if (pCall->GetCallInfo()->dwTrunk == 0xffffffff)
		dwCallFeatures &= ~LINECALLFEATURE_BLINDTRANSFER;
$$ENDIF
$$IF(BLINDXFER_NOEXT)
	// If the call is on a trunk, then disallow any blind transfer events.
	if (pCall->GetCallInfo()->dwTrunk != 0xffffffff)
		dwCallFeatures &= ~LINECALLFEATURE_BLINDTRANSFER;
$$ENDIF
$$ENDIF
	// TODO: Adjust the call features for the given call based on information
	// associated with the call and hardware capabilities.
	return CTSPILineConnection::OnCallFeaturesChanged(pCall, dwCallFeatures);

}// $$LINE_CLASS_NAME$$::OnCallFeaturesChanged

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OpenDevice
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
$$ENDIF
bool $$LINE_CLASS_NAME$$::OpenDevice()
{
	// TODO: Validate the connection to the hardware and return FALSE if it
	// is not connected or pass through to the default implementation if it is.
	return CTSPILineConnection::OpenDevice();

}// $$LINE_CLASS_NAME$$::OpenDevice

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::OnTimer
**
** Arguments:  void
**
** Returns:    void
**
** Description: This method is called periodically by the interval timer
**
*****************************************************************************/
$$ENDIF
void $$LINE_CLASS_NAME$$::OnTimer()
{
	// Poll the active request for timeout. This is not necessary if you
	// do not implement time-out conditions for the switch. Note that the
	// interval is controlled by the "SetIntervalTimer" in device.cpp (Init)
	ReceiveData();

}// $$LINE_CLASS_NAME$$::OnTimer

$$IF(HASTAPI3MSP)
$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  $$LINE_CLASS_NAME$$::ReceiveMSPData
**
** Arguments:  'pMSP' - MSP driver object
**             'pCall' - Call this data is for
**             'lpData' - Data being sent from the MSP
**             'dwSize' - Size of the data block
**
** Returns:    TAPI result code
**
** Description: This method is called on behalf of the TSPI_ReceiveMSPData
**              entrypoint. It is called if the passed Call handle is NULL
**              or if the CTSPICallAppearance::ReceiveMSPData is not
**              overridden.
**
*****************************************************************************/
$$ENDIF
LONG $$LINE_CLASS_NAME$$::ReceiveMSPData(CMSPDriver* pMSP, CTSPICallAppearance* pCall, LPVOID lpData, DWORD dwSize)
{
	// TODO: Handle the incoming data
	return 0;

}// $$LINE_CLASS_NAME$$::ReceiveMSPData
$$ENDIF
