#include <windows.h>
#include <tapi.h>
#include <tchar.h>
#include <iostream>
#include <string>
using namespace std;

#pragma comment(lib, "tapi32.lib")

#define TAPIVER_20 0x00020000
typedef std::string TString;
inline void* AllocMem(int size) { return malloc(size); }
inline void FreeMem(void* p) { free(p); }

class LOCATIONINFO
{
// Class data
public:
	DWORD dwCurrentLocation;
	TString strCountryCode;
	TString strAreaCode;
	TString strIntlCode;
	TString strLocalAccess;
	TString strLongDistanceAccess;
	TString strCallWaiting;
	TString strNationalCode;

// Constructor
public:
	LOCATIONINFO();
};

// Globals
LOCATIONINFO* m_pcurrLocation = NULL;

TString GetDialableNumber (LPCTSTR pszNumber, LPCTSTR pszAllow)
{                 
    TString strReturn;
    if (pszNumber != NULL)
    {
        while (*pszNumber) {   
            if (_istdigit(*pszNumber) || (pszAllow && _tcschr(pszAllow, *pszNumber)))
                strReturn += *pszNumber;
            pszNumber++;
        }
    }                
    return strReturn;

}

LOCATIONINFO::LOCATIONINFO() : dwCurrentLocation(0)
{
	// Allocate a block of memory to retrieve all the location entries.
	DWORD dwTotalSize = 8192;
	for ( ;; )
	{
		LPLINETRANSLATECAPS lpCaps = reinterpret_cast<LPLINETRANSLATECAPS>(AllocMem(dwTotalSize));
		if (lpCaps == NULL)
			return;

		// Set the total size
		lpCaps->dwTotalSize = dwTotalSize;
		if (lineGetTranslateCaps(NULL, TAPIVER_20, lpCaps) != 0)
		{
			FreeMem(lpCaps);
			return;
		}

		// Go through the list of retrieved locations and find our current location.
		if (lpCaps->dwNeededSize <= lpCaps->dwTotalSize)
		{
			// If the current location is zero, then reset to what TAPI says.
			if (dwCurrentLocation == 0)
				dwCurrentLocation = lpCaps->dwCurrentLocationID;

			// Walk through all the entry structures looking for our current
			// location id.
			LPLINELOCATIONENTRY lpEntry = reinterpret_cast<LPLINELOCATIONENTRY>(reinterpret_cast<LPBYTE>(lpCaps) + lpCaps->dwLocationListOffset);
			for (unsigned int i = 0; i < lpCaps->dwNumLocations; i++)
			{
				// If this is our entry then copy the information from it.
				if (lpEntry->dwPermanentLocationID == dwCurrentLocation)
				{
					// Copy the country code
					TCHAR chBuff[10];
					_ltot(lpEntry->dwCountryCode, chBuff, 10);
					strCountryCode = chBuff;

					// Copy the area code
					if (lpEntry->dwCityCodeSize > 0)
						strAreaCode = reinterpret_cast<LPCSTR>(reinterpret_cast<LPBYTE>(lpCaps) + lpEntry->dwCityCodeOffset);

					// Copy the long-distance access code
					if (lpEntry->dwLongDistanceAccessCodeSize > 0)
						strLongDistanceAccess = GetDialableNumber(reinterpret_cast<LPCTSTR>(reinterpret_cast<LPBYTE>(lpCaps) + lpEntry->dwLongDistanceAccessCodeOffset), NULL);

					// Copy the local access code
					if (lpEntry->dwLocalAccessCodeSize > 0)
						strLocalAccess = GetDialableNumber(reinterpret_cast<LPCTSTR>(reinterpret_cast<LPBYTE>(lpCaps) + lpEntry->dwLocalAccessCodeOffset), NULL);

					// Copy the call-waiting sequence.
					if (lpEntry->dwCancelCallWaitingSize > 0)
						strCallWaiting = GetDialableNumber(reinterpret_cast<LPCTSTR>(reinterpret_cast<LPBYTE>(lpCaps) + lpEntry->dwCancelCallWaitingOffset), _T("#*"));

					// Now using the country id, retrieve the international dialing rules
					// for this country. This includes the prefix which must be dialed
					// in order to make an international call.
					DWORD dwCountryID = lpEntry->dwCountryID;
					if (dwCountryID != 1)  // Don't bother for USA -- preset for that environment.
					{
						LPLINECOUNTRYLIST lpList = reinterpret_cast<LPLINECOUNTRYLIST>(lpCaps);
						lpList->dwTotalSize = lpCaps->dwTotalSize;
						if (lineGetCountry(dwCountryID, TAPIVER_20, lpList) == 0)
						{
							// We should always have enough for the country list - it
							// is rather small compared to the location entries.
							if (lpList->dwNeededSize <= lpList->dwTotalSize)
							{
								LPLINECOUNTRYENTRY lpEntry = reinterpret_cast<LPLINECOUNTRYENTRY>(reinterpret_cast<LPBYTE>(lpList) + lpList->dwCountryListOffset);
								for (i = 0; i < lpList->dwNumCountries; i++)
								{
									// When we locate our country, copy out the international
									// prefix information
									if (lpEntry->dwCountryID == dwCountryID)
									{
										if (lpEntry->dwInternationalRuleSize > 0)
										{
											strIntlCode = reinterpret_cast<LPCTSTR>(reinterpret_cast<LPBYTE>(lpList) + lpEntry->dwInternationalRuleOffset);
											for (i = 0; i < strIntlCode.length(); i++)
											{
												if (!_istdigit(strIntlCode[i]))
												{
													strIntlCode.resize(i);
													break;
												}
											}
										}

										// Copy the long-distance rule information (v3.04)
										if (lpEntry->dwLongDistanceRuleSize > 0)
										{
											strNationalCode = reinterpret_cast<LPCTSTR>(reinterpret_cast<LPBYTE>(lpList) + lpEntry->dwLongDistanceRuleOffset);
											for (i = 0; i < strNationalCode.length(); i++)
											{
												if (!_istdigit(strNationalCode[i]))
												{
													strNationalCode.resize(i);
													break;
												}
											}
										}
										break;
									}
								}
							}
						}
					}
					else // USA
					{
						strNationalCode = _T("1");
						strIntlCode = _T("011");
					}
					break;
				}
				lpEntry++;
			}

			// Free the data structure
			FreeMem(lpCaps);
			break;
		}

		// Otherwise we didn't allocate enough memory, free this and realloc.
		else
		{
			dwTotalSize = lpCaps->dwNeededSize;
			FreeMem(lpCaps);
			lpCaps = NULL;
		}
	}

}

TString DetermineCountryCode(const TString& strInput)
{
	// List of valid known country codes in size order
	static unsigned short Countries9[] = {
		995,994,993,977,976,975,974,973,972,971,968,967,966,965,964,963,962,961,960,98,95,94,93,92,91,90,0 };
	static unsigned short Countries8[] = {
		886,880,874,873,872,871,870,856,855,853,852,850,800,86,84,82,81,0 };
	static unsigned short Countries7[] = {
		7,0 };
	static unsigned short Countries6[] = {
		692,691,690,689,688,687,686,685,684,683,682,681,680,679,678,677,676,675,674,673,672,670,66,65,64,63,62,61,60,0 };
	static unsigned short Countries5[] = {
		599,598,597,596,595,594,593,592,591,590,509,508,507,506,505,504,503,502,501,500,58,57,56,55,54,53,52,51,0 };
	static unsigned short Countries4[] = {
		421,420,49,48,47,46,45,44,43,41,40,0 };
	static unsigned short Countries3[] = {
		389,387,386,385,381,380,378,377,376,375,374,373,372,371,370,359,358,357,356,355,354,353,352,351,350,39,39,36,34,33,32,31,30,0 };
	static unsigned short Countries2[] = {
		299,298,297,291,290,269,268,267,266,265,264,263,262,261,260,258,257,256,255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,218,216,213,212,27,20,0 };
	static unsigned short Countries1[] = {
		1,0 };

	unsigned short* pSearch = NULL;
	switch (strInput[0])
	{
		case _T('9'):	pSearch = Countries9; break;
		case _T('8'):	pSearch = Countries8; break;
		case _T('7'):	pSearch = Countries7; break;
		case _T('6'):	pSearch = Countries6; break;
		case _T('5'):	pSearch = Countries5; break;
		case _T('4'):	pSearch = Countries4; break;
		case _T('3'):	pSearch = Countries3; break;
		case _T('2'):	pSearch = Countries2; break;
		case _T('1'):	pSearch = Countries1; break;
	}

	// Search the sub-list based on the first character
	if (pSearch != NULL)
	{
		TCHAR chBuff[5];
		while (*pSearch != 0)
		{
			_itot(*pSearch,chBuff,10);
			if (!_tcsncmp(strInput.c_str(), chBuff, lstrlen(chBuff)))
				return chBuff;
			++pSearch;
		}
	}
	return strInput;

}// DetermineCountryCode

TString DetermineAreaCode(const TString& strCountryCode, TString& strInput)
{
	TString strAreaCode = _T("");

	// If the country code doesn't match our current location, then don't try
	// to determine an area code.  Since the international rules are not fully
	// described within TAPI, the bulk of area-code translation would be upon
	// the service provider.  The TSP should override this functon for other countries.
	if (strCountryCode != m_pcurrLocation->strCountryCode)
	{
		const int nUSAreaCodeLength = 3;
		const int nUSFullNumberLength = 10;

		// Go ahead and implement a simple parser for US numbers -- since 
		// the area code rules are consistant in that country.
		if (strCountryCode == _T("1"))
		{
			if (strInput.length() >= nUSFullNumberLength)
			{
				strAreaCode = strInput.substr(0,nUSAreaCodeLength);
				strInput = strInput.substr(nUSAreaCodeLength);
			}
		}
	}

	// See if we have area code information embedded in the number. We try
	// to determine this by using the NA standard of a 7-digit number with
	// some area code length.  If we have enough digits left in the string, then
	// pull the first 'n' digits off as the area code.  Otherwise, we will assume
	// that the area code is not present and that this is within the default
	// area code for our location. (V3.044)
	else // same country
	{
		strAreaCode = m_pcurrLocation->strAreaCode;
		if (strInput.length() > 7)
		{
			if (strInput.substr(0, m_pcurrLocation->strAreaCode.length()) == m_pcurrLocation->strAreaCode)
			{
				// Strip it out of the number.
				strInput = strInput.substr(m_pcurrLocation->strAreaCode.length());

			}
			else
			{
				// We will assume in this generic function that the primary number is always
				// 7-digits and use whatever remains at the beginning as the area code for 
				// this national number.
				if (strInput.substr(0, strInput.length()-7) != m_pcurrLocation->strAreaCode)
					strAreaCode = strInput.substr(0,strInput.length()-7);

				// Remove the area code from the input string. This should _not_ done if we didn't
				// have enough digits to include it..
				strInput = strInput.substr(strInput.length()-7);
			}
		}
	}
	return strAreaCode;

}// CServiceProvider::DetermineAreaCode

TString ConvertDialableToCanonical(LPCTSTR pszNumber, DWORD /*dwCountryCode*/, bool fInbound/*=false*/)
{   
	// If the number is NULL, then return a blank string so it can be assigned to
	// a std::basic_string collection.
	if (pszNumber == NULL)
		return TString(_T(""));

	// If the number is already in canonical format then use that data.
	if (*pszNumber == _T('+'))
		return pszNumber;

	// If we have not yet loaded our country information, do so now.
	//if (!m_pcurrLocation->IsLoaded())
	//	m_pcurrLocation->Reload();

	// If the number of digits we were passed is less than required to build a 
	// canonical formatted number then simply pass the same digits back. This
	// is specifically for in-switch calls
    TString strInput(GetDialableNumber(pszNumber, (m_pcurrLocation->strCallWaiting.empty()) ? 
						NULL : m_pcurrLocation->strCallWaiting.c_str()));
	if (strInput.length() < 7 || m_pcurrLocation == NULL)
		return pszNumber;

	TString strCountryCode, strAreaCode, strNumber, strOutput;

	// If this is an outbound call, then check for prefix digits.
	if (!fInbound)
	{
		// Strip out any prefix digits required to get an outside line from the dialing string.
		// This should be the first element in the string.
		if (!m_pcurrLocation->strLocalAccess.empty() &&
			strInput.substr(0, m_pcurrLocation->strLocalAccess.length()) ==
			m_pcurrLocation->strLocalAccess)
			strInput = strInput.substr(m_pcurrLocation->strLocalAccess.length());
		else if (!m_pcurrLocation->strLongDistanceAccess.empty() &&
				strInput.substr(0, m_pcurrLocation->strLongDistanceAccess.length()) ==
				m_pcurrLocation->strLongDistanceAccess)
			strInput = strInput.substr(m_pcurrLocation->strLongDistanceAccess.length());

		// Strip out call waiting information from the dialing string
		if (!m_pcurrLocation->strCallWaiting.empty() &&
			strInput.substr(0, m_pcurrLocation->strCallWaiting.length()) ==
			m_pcurrLocation->strCallWaiting)
			strInput = strInput.substr(m_pcurrLocation->strCallWaiting.length());
	}

	// See if this is an international call based on the international digits.
	if (!m_pcurrLocation->strIntlCode.empty() &&
		strInput.substr(0, m_pcurrLocation->strIntlCode.length()) == 
		m_pcurrLocation->strIntlCode)
	{
		// Remove the international sequence.
		strInput = strInput.substr(m_pcurrLocation->strIntlCode.length());

		// Get the country code we are dialing.
		strCountryCode = DetermineCountryCode(strInput.substr(0,3));
		strInput = strInput.substr(strCountryCode.length());

		// Determine the area code for the number.
		strAreaCode = DetermineAreaCode(strCountryCode, strInput);
	}

	// Non-international call -- parse out the long-distance code (if included) and
	// the area code information from the call.
	else
	{
		// If we have a national code for our country, then strip that information out
		// of the string if it is present.  This value is the sequence of digits which allow
		// the user to place a non-local call.
		//
		// Thanks to Martin Roth @ Ascom for this piece of code.
		if (!m_pcurrLocation->strNationalCode.empty() &&
				      strInput.substr(0, m_pcurrLocation->strNationalCode.length()) ==
					  m_pcurrLocation->strNationalCode)
		{
			// Strip out the national code
			strInput = strInput.substr(m_pcurrLocation->strNationalCode.length());
		}

		// If the country code is included in the number, then strip it out.
		// This happens in some dialing areas with caller-id.
		else if (!m_pcurrLocation->strCountryCode.empty() &&
			strInput.length() > 10 && 
			strInput.substr(0, m_pcurrLocation->strCountryCode.length()) ==
			m_pcurrLocation->strCountryCode)
		{
			// Strip out the coutry code
			strInput = strInput.substr(m_pcurrLocation->strCountryCode.length());
		}

		// Set the country code
		strCountryCode = m_pcurrLocation->strCountryCode;

		// Determine the area code for the number.
		strAreaCode = DetermineAreaCode(strCountryCode, strInput);
	}

	// Now determine what the number is based on the remaining digits in the
	// dialable string.
	unsigned int nPos = strInput.find_first_not_of(_T("0123456789"));
	if (nPos == TString::npos)
		nPos = strInput.length();
	strNumber = strInput.substr(0,nPos);

	// First character in canonical format is "+" followed by country code.
	if (!strCountryCode.empty())
	{
		strOutput = _T("+");
		strOutput += strCountryCode;
		strOutput += _T(" ");
	}

	// Create our phone number
    if (!strAreaCode.empty())
		strOutput += _T("(") + strAreaCode + _T(") ");

	// Append the numerical number to the output.
	strOutput += strNumber;

    return strOutput;    
    
}// CServiceProvider::ConvertDialableToCanonical

void help()
{
	cout << "Program usage:" << endl;
	cout << "test [i,o] phone_number" << endl;
	cout << "  where i/o indicates inbound or outbound" << endl;
	cout << endl << "The number is interpreted according to the current TAPI dialing rules" << endl;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		help();
		return 0;
	}

	char ch = toupper(*(argv[1]));
	if (ch != 'I' && ch != 'O')
	{
		help();
		return 0;
	}

	bool fInbound = ch == L'I';
	m_pcurrLocation = new LOCATIONINFO;

	cout << "Location information:" << endl;
	cout << "dwCurrentLocation = " << m_pcurrLocation->dwCurrentLocation << endl;
	cout << "NationalCode = " <<m_pcurrLocation->strNationalCode << endl;
	cout << "CountryCode = " << m_pcurrLocation->strCountryCode.c_str() << endl;
	cout << "AreaCode = " << m_pcurrLocation->strAreaCode.c_str() << endl;
	cout << "International Code = " << m_pcurrLocation->strIntlCode << endl;
	cout << "Local Access = " <<m_pcurrLocation->strLocalAccess << endl;
	cout << "Long Distance = " <<m_pcurrLocation->strLongDistanceAccess << endl;
	cout << "Callwaiting = " <<m_pcurrLocation->strCallWaiting << endl;
	cout << endl;

	TString strNumber;
	for (int i = 2; i < argc; i++)
	{
		strNumber += argv[i];
		strNumber += " ";
	}

	TString strOut = ConvertDialableToCanonical(strNumber.c_str(), 0, fInbound);
	cout << strNumber.c_str() << " = " << strOut.c_str() << endl;

	delete m_pcurrLocation;
	
	return 0;
}
