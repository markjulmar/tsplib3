$$IF(VERBOSE)
/***************************************************************************
//
// $$ROOT$$.H
//
// TAPI Service provider for TSP++ version 3.0
// Designed for $$SWITCH_INFO$$
//
// Copyright (C) $$CURR_YEAR$$ $$COMPANY_NAME$$
// All rights reserved
//
// $$JULMAR_INFO$$
// 
/***************************************************************************/
$$ENDIF

#ifndef _$$SAFE_ROOT$$_INC_
#define _$$SAFE_ROOT$$_INC_

$$IF(VERBOSE)
/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
$$ENDIF
#include "resource.h"
$$IF(USE_POOLMGR)
#include <poolmgr.h>
$$ENDIF

$$IF(VERBOSE)
/*----------------------------------------------------------------------------
	PRE-DECLARATIONS
-----------------------------------------------------------------------------*/
$$ENDIF
$$IF(OVERRIDE_LINE)
class $$LINE_CLASS_NAME$$;
$$ENDIF
$$IF(OVERRIDE_PHONE)
class $$PHONE_CLASS_NAME$$;
$$ENDIF
class $$DEVICE_CLASS_NAME$$;

$$IF(VERBOSE)
/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
$$ENDIF
#define STATE_WAITING		(STATE_INITIAL + 1)
#define REQUEST_TIMEOUT		(10000)				// Timeout for requests (mSec)

$$IF(PHONEFEATURE_4)
$$IF(BUTTONLAMP_COUNT)
$$IF(VERBOSE)
/**************************************************************************
** LAMPBUTTONENTRY
**
** Structure which is used to initialize the lamp/button entries.
**
***************************************************************************/
$$ENDIF
typedef struct
{
	DWORD dwFunction;		// PHONEBUTTONFUNCTION_xxxx
	DWORD dwButtonMode;		// PHONEBUTTONMODE_xxx
	DWORD dwLampMode;		// PHONELAMPMODE_xxx
	LPCTSTR pszText;		// Text for button/lamp
} LAMPBUTTONENTRY;

$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_5)
$$IF(VERBOSE)
/**************************************************************************
** HOOKSWITCHENTRY
**
** Structure which is used to initialize the hookswitch entries.
**
***************************************************************************/
$$ENDIF
typedef struct
{
	DWORD dwHookSwitchDevice;	// PHONEHOOKSWITCHDEV_xxx
	DWORD dwAvailModes;			// PHONEHOOKSWITCHMODE_xxx
	DWORD dwSettableModes;		// PHONEHOOKSWITCHMODE_xxx
	DWORD dwVolume;				// Default volume level
	DWORD dwGain;				// Default gain level
	bool fSupportsVolChange;	// TRUE/FALSE
	bool fSupportsGainChange;	// TRUE/FALSE
} HOOKSWITCHENTRY;

$$ENDIF
$$IF(EVENT_OBJECT_NAME)
$$IF(VERBOSE)
/**************************************************************************
** $$EVENT_OBJECT_NAME$$
**
** Structure/object which is used to encapsulate device event information
**
***************************************************************************/
$$ENDIF
typedef struct
{
	// TODO: Add h/w event elements here
	TCHAR chBuff[1];
} $$EVENT_OBJECT_NAME$$;

$$ENDIF
$$IF(VERBOSE)
/**************************************************************************
** $$SP_CLASS_NAME$$
**
** Main provider object which manages the lifetime of the TSP
**
***************************************************************************/
$$ENDIF
class $$SP_CLASS_NAME$$ : public CServiceProvider
{
// Constructor
public:
    $$SP_CLASS_NAME$$();
$$IF(WANT_TRACEMGR)
	// Overrides from CServiceProvider
public:
	virtual void TraceOut(TString& strBuff);
$$ENDIF
$$IF(!HAS_UI)
$$IF(!WANT_TRACEMGR)
	// Overrides from CServiceProvider
public:
$$ENDIF
    virtual LONG providerEnumDevices(DWORD dwProviderId, LPDWORD lpNumLines, LPDWORD lpNumPhones, HPROVIDER hProvider, LINEEVENT lpfnLineCreateProc, PHONEEVENT lpfnPhoneCreateProc);
$$ENDIF
};

$$IF(VERBOSE)
/**************************************************************************
** $$DEVICE_CLASS_NAME$$
**
** Device object which manages a connection to hardware device
**
***************************************************************************/
$$ENDIF
class $$DEVICE_CLASS_NAME$$ : public CTSPIDevice
{
// Unavailable functions (due to base class)
private:
	$$DEVICE_CLASS_NAME$$(const $$DEVICE_CLASS_NAME$$&);
	const $$DEVICE_CLASS_NAME$$& operator=(const $$DEVICE_CLASS_NAME$$&);
	
// Class Data
protected:
	HANDLE m_hevtStop;						// Stop event
	HANDLE m_hConnThread;					// Thread running hardware connection
	CRITICAL_SECTION m_csData;				// CS protecting queue
$$IF(USE_COMM)
	HANDLE m_hCommPort;						// Handle to communications device
$$ENDIF
$$IF(USE_POOLMGR)
$$IF(EVENT_OBJECT_NAME)
	CThreadPoolMgr<DWORD, $$EVENT_OBJECT_NAME$$*> m_mgrThreads;	// Thread pool manager class
$$ELSE
	CThreadPoolMgr<DWORD, LPCVOID> m_mgrThreads;	// Thread pool manager class
$$ENDIF
$$ENDIF
	
// Constructor/Destructor
public:
	$$DEVICE_CLASS_NAME$$();
	virtual ~$$DEVICE_CLASS_NAME$$();

// Input thread function
public:
	unsigned ConnectionThread();

// Overridden functions from CTSPIDevice
protected:
	friend class $$LINE_CLASS_NAME$$;
    virtual bool Init(DWORD dwProviderId, DWORD dwBaseLine, DWORD dwBasePhone, DWORD dwLines, DWORD dwPhones, HPROVIDER hProvider, ASYNC_COMPLETION lpfnCompletion);
$$IF(HAS_UI)
	virtual TStream& read(TStream& istm);
$$ENDIF

// Internal functions
protected:
$$IF(EVENT_OBJECT_NAME)
	CTSPIConnection* LocateOwnerFromEvent($$EVENT_OBJECT_NAME$$* pEvent);
$$ELSE
	CTSPIConnection* LocateOwnerFromEvent(LPCVOID pEvent);
$$ENDIF
};
$$IF(OVERRIDE_LINE)

$$IF(VERBOSE)
/**************************************************************************
** $$LINE_CLASS_NAME$$
**
** Line object which manages a single line on the hardware
**
***************************************************************************/
$$ENDIF
class $$LINE_CLASS_NAME$$ : public CTSPILineConnection
{
// Unavailable functions (due to base class)
private:
	$$LINE_CLASS_NAME$$(const $$LINE_CLASS_NAME$$&);
	const $$LINE_CLASS_NAME$$& operator=(const $$LINE_CLASS_NAME$$&);

// Constructor/Destructor
public:
	$$LINE_CLASS_NAME$$();
	virtual ~$$LINE_CLASS_NAME$$();

// Overrides from CTSPILineConnection
public:
	$$DEVICE_CLASS_NAME$$* GetDeviceInfo() const { return ($$DEVICE_CLASS_NAME$$*) CTSPIConnection::GetDeviceInfo(); }
$$IF(HAS_UI)
	virtual TStream& read(TStream& istm);
$$ENDIF
$$IF(LINEFEATURE_22)
    virtual void OnMediaControl(CTSPICallAppearance* pCall, DWORD dwMediaControl);
$$ENDIF
$$IF(LINEFEATURE_25)
    virtual LONG GetDevConfig(TString& strDeviceClass, LPVARSTRING lpDeviceConfig);
    virtual LONG SetDevConfig(TString& strDeviceClass, const LPVOID lpDevConfig, DWORD dwSize);
$$ENDIF
$$IF(LINEFEATURE_8)
    virtual LONG DevSpecificFeature(DWORD dwFeature, DRV_REQUESTID dwRequestId, LPVOID lpParams, DWORD dwSize);
	virtual LONG DevSpecific(CTSPIAddressInfo* pAddr, CTSPICallAppearance* pCall, DRV_REQUESTID dwRequestID, LPVOID lpParam, DWORD dwSize);
$$ENDIF
protected:
$$IF(!HAS_UI)
    virtual void Init(CTSPIDevice* pDev, DWORD dwLineDeviceID, DWORD dwPos, DWORD dwItemData=0);
$$ENDIF
	virtual DWORD OnAddressFeaturesChanged (CTSPIAddressInfo* pAddr, DWORD dwFeatures);
	virtual DWORD OnLineFeaturesChanged(DWORD dwLineFeatures);
	virtual DWORD OnCallFeaturesChanged(CTSPICallAppearance* pCall, DWORD dwCallFeatures);
	virtual bool OpenDevice();
	virtual bool UnsolicitedEvent(LPCVOID lpBuff);
	virtual void OnTimer();
$$IF(HASTAPI3MSP)
	virtual LONG ReceiveMSPData(CMSPDriver* pMSP, CTSPICallAppearance* pCall, LPVOID lpData, DWORD dwSize);
$$ENDIF

	// Create the event map
	DECLARE_TSPI_REQUESTS()

	// TSPI handlers
$$IF(LINEFEATURE_0)
	bool OnAccept(RTAccept* pRequest, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_2)
	bool OnAnswer(RTAnswer* pRequest, LPCVOID lpBuff);
$$ENDIF
	bool OnDropCall(RTDropCall* pReq, LPCVOID lpBuff);
	bool OnMakeCall(RTMakeCall* pReq, LPCVOID lpBuff);
$$IF(LINEFEATURE_14)
	bool OnHoldCall(RTHold* pReq, LPCVOID lpBuff);
	bool OnRetrieveCall(RTUnhold* pReq, LPCVOID lpBuff);
$$IF(HOLDCANSWAP)
	bool OnSwapHold(RTSwapHold* pReq, LPCVOID lpBuff);
$$ENDIF
$$ENDIF
	bool OnDial(RTDial* pReq, LPCVOID lpBuff);
$$IF(LINEFEATURE_32)
	bool OnPredictiveMakeCall(RTMakeCall* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_35)
	bool OnRedirect(RTRedirect* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_3)
	bool OnBlindTransfer(RTBlindTransfer* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_12)
	bool OnGenerateDigits(RTGenerateDigits* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_5)
	bool OnCompleteCall(RTCompleteCall* pReq, LPCVOID lpBuff);
	bool OnUncompleteCall(RTUncompleteCall* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_9)
	bool OnForward(RTForward* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_13)
	bool OnGenerateTone(RTGenerateTone* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_18)
	bool OnPark(RTPark* pReq, LPCVOID lpBuff);
	bool OnUnpark(RTUnpark* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_19)
	bool OnPickup(RTPickup* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_20)
	bool OnSecureCall(RTSecureCall* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_23)
	bool OnSetTerminal(RTSetTerminal* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_4)
	bool OnSetupTransfer(RTSetupTransfer* pReq, LPCVOID lpBuff);
	bool OnCompleteTransfer(RTCompleteTransfer* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_1)
	bool OnSetupConference(RTSetupConference* pReq, LPCVOID lpBuff);
	bool OnAddToConference(RTAddToConference* pReq, LPCVOID lpBuff);
$$IF(CONFADDEXISTING)
	bool OnPrepareAddToConference(RTPrepareAddToConference* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(CONFREMOVE)
	bool OnRemoveFromConference(RTRemoveFromConference* pReq, LPCVOID lpBuff);
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_26)
	bool OnSetQualityOfService(RTSetQualityOfService* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_27)
	bool OnSetCallTreatment(RTSetCallTreatment* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(LINEFEATURE_7)
$$IF(AGENT_SUPPORTSGROUPS)
	bool OnSetAgentGroup (RTSetAgentGroup* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(AGENT_SUPPORTSSTATES)	
	bool OnSetAgentState (RTSetAgentState* pReq, LPCVOID lpBuff);
$$ENDIF
$$IF(AGENT_SUPPORTSACTIVITIES)	
	bool OnSetAgentActivity (RTSetAgentActivity* pReq, LPCVOID lpBuff);
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_21)
$$IF(UUI_SEND)
	bool OnSendUUI(RTSendUserInfo* pReq, LPCVOID lpBuff);
$$ENDIF
$$ENDIF

// Internal methods
private:
	void InitializeStation();
	void InitAddress(CTSPIAddressInfo* pAddress);
$$IF(LINEFEATURE_33)
	void InitializeVRU();
$$ENDIF
$$IF(LINEFEATURE_31)
	void InitializeQueue();
$$ENDIF
$$IF(LINEFEATURE_6)
	void InitializeRoutePoint();
$$ENDIF
$$IF(LINEFEATURE_32)
	void InitializePredictiveDialer();
$$ENDIF
$$IF(LINEFEATURE_37)
	void InitializeTrunk();
$$ENDIF
};
$$ENDIF
$$IF(OVERRIDE_ADDRESS)

$$IF(VERBOSE)
/**************************************************************************
** $$ADDRESS_CLASS_NAME$$
**
** Address object which supersedes each created address on a line
**
***************************************************************************/
$$ENDIF
class $$ADDRESS_CLASS_NAME$$ : public CTSPIAddressInfo
{
// Unavailable functions (due to base class)
private:
	$$ADDRESS_CLASS_NAME$$(const $$ADDRESS_CLASS_NAME$$&);
	const $$ADDRESS_CLASS_NAME$$& operator=(const $$ADDRESS_CLASS_NAME$$&);

// Constructor/Destructor
public:
	$$ADDRESS_CLASS_NAME$$();
	virtual ~$$ADDRESS_CLASS_NAME$$();

// Initialization override from CTSPIAddressInfo
$$IF(HAS_UI)
public:
	virtual TStream& read(TStream& istm);
$$ELSE
protected:
    virtual void Init (CTSPILineConnection* pLine, DWORD dwAddressID, LPCTSTR lpszAddress, 
					   LPCTSTR lpszName, bool fIncoming, bool fOutgoing, DWORD dwAvailMediaModes, 
                       DWORD dwAvailBearerModes, DWORD dwMinRate, DWORD dwMaxRate,
                       DWORD dwMaxNumActiveCalls, DWORD dwMaxNumOnHoldCalls, 
                       DWORD dwMaxNumOnHoldPendCalls, DWORD dwMaxNumConference, 
                       DWORD dwMaxNumTransConf, DWORD dwAddressSharing, const LPLINEDIALPARAMS lpDialParams, DWORD dwAddressType);
$$ENDIF
};
$$ENDIF
$$IF(OVERRIDE_CALL)

$$IF(VERBOSE)
/**************************************************************************
** $$CALL_CLASS_NAME$$
**
** Call object which supersedes each created call on an address
**
***************************************************************************/
$$ENDIF
class $$CALL_CLASS_NAME$$ : public CTSPICallAppearance
{
// Unavailable functions (due to base class)
private:
	$$CALL_CLASS_NAME$$(const $$CALL_CLASS_NAME$$&);
	const $$CALL_CLASS_NAME$$& operator=($$CALL_CLASS_NAME$$&);

// Constructor/Destructor
public:
	$$CALL_CLASS_NAME$$();
	virtual ~$$CALL_CLASS_NAME$$();

// Initialization override from CTSPICallAppearance
protected:
    virtual void Init (CTSPIAddressInfo* pAddr, DWORD dwBearerMode, 
		DWORD dwRate, DWORD dwCallParamFlags, DWORD dwOrigin, DWORD dwReason, 
		DWORD dwTrunk, DWORD dwCompletionID);
$$IF(HASTAPI3MSP)
	virtual LONG ReceiveMSPData(CMSPDriver* pMSP, LPVOID lpData, DWORD dwSize);
$$ENDIF
};
$$ENDIF
$$IF(OVERRIDE_CONFCALL)

$$IF(VERBOSE)
/**************************************************************************
** $$CONFCALL_CLASS_NAME$$
**
** Conference call object which supersedes each created conference
**
***************************************************************************/
$$ENDIF
class $$CONFCALL_CLASS_NAME$$ : public CTSPIConferenceCall
{
// Unavailable functions (due to base class)
private:
	$$CONFCALL_CLASS_NAME$$(const $$CONFCALL_CLASS_NAME$$&);
	const $$CONFCALL_CLASS_NAME$$& operator=(const $$CONFCALL_CLASS_NAME$$&);

// Constructor/Destructor
public:
	$$CONFCALL_CLASS_NAME$$();
	virtual ~$$CONFCALL_CLASS_NAME$$();

// Initialization override from CTSPICallAppearance
protected:
    virtual void Init (CTSPIAddressInfo* pAddr, DWORD dwBearerMode, 
		DWORD dwRate, DWORD dwCallParamFlags, DWORD dwOrigin, DWORD dwReason, 
		DWORD dwTrunk, DWORD dwCompletionID);
    virtual void OnConsultantCallIdle();
};
$$ENDIF
$$IF(OVERRIDE_PHONE)

$$IF(VERBOSE)
/**************************************************************************
** $$PHONE_CLASS_NAME$$
**
** Phone object which manages a single phone device on the hardware
**
***************************************************************************/
$$ENDIF
class $$PHONE_CLASS_NAME$$ : public CTSPIPhoneConnection
{
// Unavailable functions (due to base class)
private:
	$$PHONE_CLASS_NAME$$(const $$PHONE_CLASS_NAME$$&);
	const $$PHONE_CLASS_NAME$$& operator=(const $$PHONE_CLASS_NAME$$&);

// Constructor/Destructor
public:
	$$PHONE_CLASS_NAME$$();
	virtual ~$$PHONE_CLASS_NAME$$();

// Overrides from CTSPIPhoneConnection
public:
	$$DEVICE_CLASS_NAME$$* GetDeviceInfo() const { return ($$DEVICE_CLASS_NAME$$*) CTSPIConnection::GetDeviceInfo(); }
$$IF(HAS_UI)
	virtual TStream& read(TStream& istm);
$$ENDIF
$$IF(PHONEFEATURE_0)
    virtual LONG DevSpecific(DRV_REQUESTID dwRequestId, LPVOID lpParams, DWORD dwSize);
$$ENDIF
protected:
$$IF(!HAS_UI)
    virtual void Init(CTSPIDevice* pDev, DWORD dwLineDeviceID, DWORD dwPos, DWORD dwItemData=0);
$$ENDIF
	virtual bool OpenDevice();
	virtual bool UnsolicitedEvent(LPCVOID lpBuff);
	virtual void OnTimer();

	// Create the event map
	DECLARE_TSPI_REQUESTS()

	// TSPI handlers
$$IF(PHONEFEATURE_1)
	bool OnGetData(RTGetPhoneData* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$IF(PHONEFEATURE_2)
	bool OnSetData(RTSetPhoneData* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$IF(PHONEFEATURE_3)
$$IF(DISPLAY_CANWRITE)
	bool OnSetDisplay(RTSetDisplay* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_4)
$$IF(BUTTON_CANWRITE)
	bool OnSetButtonInfo(RTSetButtonInfo* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$IF(LAMP_CANWRITE)
	bool OnSetLamp(RTSetLampInfo* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_5)
$$IF(GAIN_CANWRITE)
	bool OnSetGain(RTSetGain* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$IF(HOOKSWITCH_CANWRITE)
	bool OnSetHookswitch(RTSetHookswitch* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$IF(VOLUME_CANWRITE)
	bool OnSetVolume(RTSetVolume* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_6)
$$IF(RINGER_CANWRITE)
	bool OnSetRinger(RTSetRing* pRequest, LPCVOID lpvBuff);
$$ENDIF
$$ENDIF
};
$$ENDIF

#endif // _$$SAFE_ROOT$$_INC_
