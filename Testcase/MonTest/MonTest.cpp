// MONTEST.CPP

/*-------------------------------------------------------------------------------*/
// INCLUDE FILES
/*-------------------------------------------------------------------------------*/
#include <windows.h>				// Windows includes
#include <tchar.h>					// Unicode support
#include <tspi.h>                   // TAPI support for service providers.
#include <splib.h>                  // JulMar TSP++ Class library

/*-------------------------------------------------------------------------------*/
// CONSTANTS
/*-------------------------------------------------------------------------------*/
const UINT STATE_WAITING = (STATE_INITIAL+1);
static LPCTSTR gszTelephonyKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Telephony");

/**************************************************************************
** CMonTestServiceProvider
**
** Main provider object which manages the lifetime of the TSP
**
***************************************************************************/
class CMonTestServiceProvider : public CServiceProvider
{
// Constructor
public:
    CMonTestServiceProvider() : 
	  CServiceProvider(_T("MonTest.TSP"), _T("Test Sample Provider"), TAPIVER_21) {
		SetRuntimeObjects("CMonTestDevice", "CMonTestLine");
		SetTraceLevel(TRC_API |			// TSPI_xxx entrypoints (requires JTTSPTRC.dll)
					  TRC_DUMP |		// TSPI buffers (requires JTTSPTRC.dll)
					  TRC_STRUCT |		// TSPI structures
					  TRC_MIN |			// Basic tracing inside library
					  TRC_WARNINGS |	// Warnings from TSP++
					  TRC_STATS |		// Statistics on calls/addresses/lines
					  TRC_OBJECTS |		// Object creation/destruction
					  TRC_THREADS);		// Thread creation/destruction
	}
public:
    virtual LONG providerEnumDevices(DWORD dwProviderId, LPDWORD lpNumLines, LPDWORD lpNumPhones, HPROVIDER hProvider, LINEEVENT lpfnLineCreateProc, PHONEEVENT lpfnPhoneCreateProc) {
		LONG lResult = CServiceProvider::providerEnumDevices(dwProviderId, lpNumLines, lpNumPhones, hProvider, lpfnLineCreateProc, lpfnPhoneCreateProc);
		if (lResult == 0) {
			*lpNumLines	 = 2;
			*lpNumPhones = 0;
		}
	    return lResult;
    }
};

/**************************************************************************
** CMonTestDevice
**
** Device object which manages a connection to hardware device
**
***************************************************************************/
class CMonTestDevice : public CTSPIDevice
{
protected:
    virtual bool Init(DWORD dwProviderId, DWORD dwBaseLine, DWORD dwBasePhone, DWORD dwLines, DWORD dwPhones, HPROVIDER hProvider, ASYNC_COMPLETION lpfnCompletion) {
		SetSwitchInfo(_T("Test NE Switch"));
		return CTSPIDevice::Init (dwProviderId, dwBaseLine, dwBasePhone, dwLines, dwPhones, hProvider, lpfnCompletion);
    }
};

/**************************************************************************
** CMonTestLine
**
** Line object which manages a single line on the hardware
**
***************************************************************************/
class CMonTestLine : public CTSPILineConnection
{
protected:
    virtual void Init(CTSPIDevice* pDev, DWORD dwLineDeviceID, DWORD dwPos, DWORD dwItemData=0) {
		CTSPILineConnection::Init (pDev, dwLineDeviceID, dwPos, dwItemData);
		TCHAR chBuff[50]; wsprintf(chBuff, _T("Line Device %ld"), dwLineDeviceID);
		SetName (chBuff);
		if (dwPos == 0)
		{
			srand((unsigned)GetTickCount());
			CreateAddress(_T("100"), NULL, true, true, 
					LINEMEDIAMODE_UNKNOWN | LINEMEDIAMODE_INTERACTIVEVOICE, LINEBEARERMODE_VOICE,
	                0, 0, NULL, 1, 3);
		}
		else
		{
			CreateMonitoredAddress(_T("101"), NULL, 
					LINEMEDIAMODE_UNKNOWN | LINEMEDIAMODE_INTERACTIVEVOICE, LINEBEARERMODE_VOICE,
					0, 0, 1, 3);
		}
	}
    virtual void OnCallStateChange (CTSPIAddressInfo* pAddr, CTSPICallAppearance* pCall, DWORD dwNewState, DWORD dwOldState) {
		CTSPILineConnection::OnCallStateChange(pAddr, pCall, dwNewState, dwOldState);
		// Simple test - track a different address on same line.
		if (this == GetDeviceInfo()->GetLineConnectionInfo(0))
		{
			CTSPIAddressInfo* pAddr2 = GetDeviceInfo()->GetLineConnectionInfo(1)->GetAddress(0);
			CTSPICallAppearance* pCall2 = pAddr2->FindCallByCallID(pCall->GetCallID()*-1);
			if (pCall2 == NULL)	{
				if (dwNewState == LINECALLSTATE_IDLE) return;
				pCall2 = pAddr2->CreateCallAppearance(); // Don't care about anything related to call
				pCall2->SetCallID(-1*pCall->GetCallID());
			}
			pCall2->SetCallState(dwNewState, 0, LINEMEDIAMODE_INTERACTIVEVOICE);
		}
    }

	// Create the event map
	DECLARE_TSPI_REQUESTS()

	// TSPI handlers
	bool OnDropCall(RTDropCall* pRequest, LPCVOID lpBuff) {
		if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
		{
			pRequest->GetCallInfo()->SetCallState(LINECALLSTATE_IDLE);
			CompleteRequest(pRequest, 0);
		}
		return true;
	}

	bool OnMakeCall(RTMakeCall* pRequest, LPCVOID lpBuff) {
		CTSPICallAppearance* pCall = pRequest->GetCallInfo();
		DIALINFO* pAddress = (pRequest->GetCount() > 0) ? pRequest->GetDialableNumber(0) : NULL;

		if (pRequest->EnterState(STATE_INITIAL, STATE_IGNORE))
		{
			pCall->SetCallID(static_cast<DWORD>(rand()));
			pCall->SetCallState(LINECALLSTATE_DIALTONE, LINEDIALTONEMODE_NORMAL, LINEMEDIAMODE_INTERACTIVEVOICE);
			pCall->SetCallState(LINECALLSTATE_DIALING);
			pCall->SetCallState(LINECALLSTATE_PROCEEDING);

			CompleteRequest(pRequest, 0);
		}
		return true;
	}

	bool OnHoldCall(RTHold* pReq, LPCVOID lpBuff) {
		CTSPICallAppearance* pCall = pReq->GetCallInfo();
		if (pReq->EnterState(STATE_INITIAL, STATE_WAITING))
		{
			CompleteRequest(pReq, 0);
			pCall->SetCallState(LINECALLSTATE_ONHOLD);
		}
		return true;
	}

	bool OnRetrieveCall(RTUnhold* pRequest, LPCVOID lpBuff) {
		CTSPICallAppearance* pCall = pRequest->GetCallInfo();
		if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
		{
			CompleteRequest(pRequest, 0);
			pCall->SetCallState(LINECALLSTATE_CONNECTED);
		}
		return true;
	}
};

/*-------------------------------------------------------------------------------*/
// CONSTANTS AND GLOBALS
/*-------------------------------------------------------------------------------*/
CMonTestServiceProvider theSP;

/*-------------------------------------------------------------------------------*/
// RTTI overrides of TSP++ objects
/*-------------------------------------------------------------------------------*/
DECLARE_TSPI_OVERRIDE(CMonTestDevice);
DECLARE_TSPI_OVERRIDE(CMonTestLine);

/*-------------------------------------------------------------------------------*/
// TSPI Request map
/*-------------------------------------------------------------------------------*/
BEGIN_TSPI_REQUEST(CMonTestLine)
	ON_TSPI_REQUEST_MAKECALL(OnMakeCall)
	ON_TSPI_REQUEST_DROPCALL(OnDropCall)
	ON_TSPI_REQUEST_HOLD(OnHoldCall)
	ON_TSPI_REQUEST_UNHOLD(OnRetrieveCall)
END_TSPI_REQUEST()

/*****************************************************************************
** Procedure:  _IntRegDeleteKey
**
** Arguments: 'hKeyTelephony' - Key to start from
**            'pszMainDir' - Directory to delete
**
** Returns:    true/false result code
**
** Description: This internal function deletes a directory tree in the 
**              registry under both Windows 9x and Windows NT.
**
*****************************************************************************/
static bool _IntRegDeleteKey(HKEY hKeyTelephony, LPCTSTR pszMainDir)
{
	// Attempt to delete the key directly. Under Win95, this will also delete
	// any branches under it.
    if (RegDeleteKey(hKeyTelephony, pszMainDir) != ERROR_SUCCESS)
	{
		// Open the top-level key.
		HKEY hKey;
		DWORD dwRC = RegOpenKeyEx(hKeyTelephony, pszMainDir, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey);
		if (dwRC == ERROR_SUCCESS)
		{
			DWORD dwReqSize = _MAX_PATH;
			LPTSTR pszName = new TCHAR[_MAX_PATH];
			if (pszName == NULL)
			{
				RegCloseKey(hKey);
				dwRC = ERROR_NOT_ENOUGH_MEMORY;
			}

			while (dwRC == ERROR_SUCCESS)
			{
				dwReqSize = _MAX_PATH;
				dwRC = RegEnumKeyEx(hKey, 0, pszName, &dwReqSize, NULL, NULL, NULL, NULL);
				if (dwRC == ERROR_NO_MORE_ITEMS)
				{
				   dwRC = RegDeleteKey(hKeyTelephony, pszMainDir);
				   break;
				}
				else if (dwRC == ERROR_SUCCESS)
				   _IntRegDeleteKey(hKey, pszName);
			}

			RegCloseKey(hKey);
			delete [] pszName;
		}
	}
	return true;

}// _IntRegDeleteKey

/*****************************************************************************
** Procedure:  TUISPI_providerInstall
**
** Arguments: 'lpfnDLLCallback' - Callback function for TAPI
**            'hwndOwner' - Owner for any UI
**            'dwPermanentProviderID' - Assigned provider id
**
** Returns:    TAPI 2.x result code
**
** Description: This function is called by TAPI when the provider is installed.
**
*****************************************************************************/
extern "C"
LONG TSPIAPI TUISPI_providerInstall(TUISPIDLLCALLBACK /*lpfnDLLCallback*/,
			HWND hwndOwner, DWORD dwPermanentProviderID)
{
	// Open the TELEPHONY key
    HKEY hKeyTelephony;
    if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, gszTelephonyKey, 0, KEY_ALL_ACCESS, &hKeyTelephony)
		!= ERROR_SUCCESS)
		return LINEERR_OPERATIONFAILED;

	// Add a section to the telephony section which points to our
	// full provider section.  This is used by the DbgSetLevel program
	// supplied by JulMar to locate the provider section properly.
	TCHAR chBuff[50];
	wsprintf(chBuff, _T("Device%ld"), dwPermanentProviderID);
	RegSetValueEx (hKeyTelephony, chBuff, 0, REG_SZ, 
			(LPBYTE)GetSP()->GetProviderInfo(), 
			(lstrlen(GetSP()->GetProviderInfo())+1) * sizeof(TCHAR));

	// Close our key
	RegCloseKey(hKeyTelephony);

	// Add an entry marking this provider as "TSP++" compatible for other
	// programs which search for it.  Specifically, this allows the DBGLevel
	// program to locate compatible TSPs.
	GetSP()->WriteProfileDWord(0, _T("UsesTSPLib3"), 1);

	// TODO: Replace or remove user-interface
	wsprintf(chBuff, _T("%.30s installed"), GetSP()->GetProviderInfo());
	::MessageBox(hwndOwner, chBuff, _T(""), MB_OK);

    return FALSE;

}// TUISPI_providerInstall

/*****************************************************************************
** Procedure:  TUISPI_providerRemove
**
** Arguments: 'lpfnDLLCallback' - Callback function for TAPI
**            'hwndOwner' - Owner for any UI
**            'dwPermanentProviderID' - Assigned provider id
**
** Returns:    TAPI 2.x result code
**
** Description: This function is called by TAPI when the provider is installed.
**
*****************************************************************************/
extern "C"
LONG TSPIAPI TUISPI_providerRemove(TUISPIDLLCALLBACK /*lpfnDLLCallback*/,
			HWND /*hwndOwner*/, DWORD dwPermanentProviderID)
{
	// Open the TELEPHONY key
    HKEY hKeyTelephony;
    if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, gszTelephonyKey, 0, KEY_ALL_ACCESS, &hKeyTelephony)
		!= ERROR_SUCCESS)
		return LINEERR_OPERATIONFAILED;

	// Delete our pointer to our section from TAPI
	TCHAR chBuff[50];
	wsprintf(chBuff, _T("Device%ld"), dwPermanentProviderID);
	RegDeleteValue (hKeyTelephony, chBuff);

    // Delete our section from the profile.
    _IntRegDeleteKey (hKeyTelephony, GetSP()->GetProviderInfo());
    RegCloseKey(hKeyTelephony);

    return FALSE;

}// TUISPI_providerRemove

