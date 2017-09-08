$$IF(VERBOSE)
/***************************************************************************
//
// INSTALL.CPP
//
// TAPI Service provider for TSP++ version 3.0
// Installation and removal functions and support
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
/*----------------------------------------------------------------------------
	GLOBALS
-----------------------------------------------------------------------------*/
$$ENDIF
static LPCTSTR gszTelephonyKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Telephony");

$$IF(VERBOSE)
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
$$ENDIF
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

$$IF(VERBOSE)
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
$$ENDIF
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

$$IF(VERBOSE)
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
$$ENDIF
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

