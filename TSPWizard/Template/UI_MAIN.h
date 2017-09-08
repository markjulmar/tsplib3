$$IF(VERBOSE)
/***************************************************************************
//
// $$ROOT$$UI.h
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
$$ENDIF

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __$$SAFE_ROOT$$UI_INC__
#define __$$SAFE_ROOT$$UI_INC__

#include "resource.h"       // main symbols

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
$$IF(VERBOSE)
/**************************************************************************
** CTspUIApp
**
** This object is our connection to the service provider
**
***************************************************************************/
$$ENDIF
class CTspUIApp : public CServiceProvider
{
// Class data
protected:
	DWORD m_dwProviderID;

// Constructor
public:
	CTspUIApp();

// Publics
public:
	DWORD GetProviderID() const { return m_dwProviderID; }

// TSPI overrides.
public:
	// This method is invoked when the user selects our ServiceProvider
	// icon in the control panel.  It should invoke the configuration dialog
	// which must be provided by the derived class.
	virtual LONG providerConfig(DWORD dwPPID, CWnd* pwndOwner);

	// This method is invoked when the TSP is to be installed via the
	// TAPI install code.  It should insure that all the correct files
	// are there, and write out the initial registry settings.
	virtual LONG providerInstall(DWORD dwPermanentProviderID, CWnd* pwndOwner);

$$IF(OVERRIDE_LINE)
	// This method is called to display the line configuration dialog
	// when the user requests it through either the TAPI api or the control
	// panel applet.
	virtual LONG lineConfigDialog(DWORD dwDeviceID, CWnd* pwndOwner, CString& strDeviceClass);

$$ENDIF
$$IF(OVERRIDE_PHONE)
	// This method invokes the parameter configuration dialog for the
	// phone device.
	virtual LONG phoneConfigDialog(DWORD dwDeviceID, CWnd* pwndOwner, CString& strDevClass);

$$ENDIF
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTspUIApp)
	//}}AFX_VIRTUAL
};

$$IF(VERBOSE)
/**************************************************************************
** CUIDevice
**
** This object is an override for the device object. It stores off 
** TCP/IP information into the registry stream.
**
***************************************************************************/
$$ENDIF
class CUIDevice : public CTSPUIDevice
{
// Constructor
protected:
	CUIDevice();
	DECLARE_DYNCREATE(CUIDevice)

// Serialization support
protected:
	virtual TStream& write(TStream& ostm) const;
	virtual TStream& read(TStream& istm);
};

$$IF(OVERRIDE_LINE)
$$IF(VERBOSE)
/**************************************************************************
** CUILine
**
** This object is an override for the line object to add additional
** data to the line.
**
***************************************************************************/
$$ENDIF
class CUILine : public CTSPUILineConnection
{
// Constructor which adds data to the line.
public:
	CUILine();
	CUILine(DWORD dwDeviceID, int iType, LPCTSTR pszName);
	DECLARE_DYNCREATE(CUILine)

// Serialization support
protected:
	virtual TStream& write(TStream& ostm) const;
	virtual TStream& read(TStream& istm);
};

$$ENDIF
$$IF(OVERRIDE_ADDRESS)
$$IF(VERBOSE)
/**************************************************************************
** CUIAddress
**
** This object is an override for the address object to add additional
** data to the address.
**
***************************************************************************/
$$ENDIF
class CUIAddress : public CTSPUIAddressInfo
{
// Make it dynamically creatable.
public:
	CUIAddress();
	DECLARE_DYNCREATE(CUIAddress)

// Serialization support
protected:
	virtual TStream& write(TStream& ostm) const;
	virtual TStream& read(TStream& istm);
};

$$ENDIF
$$IF(OVERRIDE_PHONE)
$$IF(VERBOSE)
/**************************************************************************
** CUIPhone
**
** This object is an override for the phone object to add additional
** data to the phone.
**
***************************************************************************/
$$ENDIF
class CUIPhone : public CTSPUIPhoneConnection
{
// Constructor which adds data to the phone.
public:
	CUIPhone();
	CUIPhone(DWORD dwPhoneID, LPCTSTR pszName);
	DECLARE_DYNCREATE(CUIPhone)

// Serialization support
protected:
	virtual TStream& write(TStream& ostm) const;
	virtual TStream& read(TStream& istm);
};
$$ENDIF

#endif // __$$SAFE_ROOT$$UI_INC__


