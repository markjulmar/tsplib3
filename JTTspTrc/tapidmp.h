#include "bitmask.h"
#include <windowsx.h>

/******************************************************************************/
//
// Inline dump routines for TAPI data structures
//
/******************************************************************************/

extern int GetTraceLevel();
extern void TraceOut(LPCWSTR pszTrace);
extern TString HexDump(const void* lpBuff, DWORD dwSize);
extern TString AsciiDump(const void* lpBuff, DWORD dwSize);

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a generic block structure
//
inline TString _Dump(const void* lpStruct, DWORD dwSize)
{
	if (lpStruct == NULL || dwSize == 0)
		return _T("");
	return HexDump(lpStruct, dwSize);

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dump out a buffer with attached size/offset fields.
//
inline TString _Dump(const void* lpStruct, DWORD dwOffset, DWORD dwSize)
{
	if (dwOffset == 0 || dwSize == 0)
		return _T("");
	return HexDump((LPBYTE)lpStruct + dwOffset, dwSize);

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _DumpASCII
//
// Dump out an ASCII buffer with attached size/offset fields.
//
inline TString _DumpASCII(const void* lpStruct, DWORD dwOffset, DWORD dwSize)
{
	if (dwOffset == 0 || dwSize == 0)
		return _T("");
	return AsciiDump((LPBYTE)lpStruct + dwOffset, dwSize);

}// _DumpASCII

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dump out a buffer with MBCS
//
inline TString _Dump(LPCWSTR lpszString)
{
	if (lpszString == NULL)
		return _T("");

	TStringStream stream;
	stream << _T("\"") << lpszString << _T("\"");
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dump out a buffer of a SBCS
//
inline TString _Dump(LPCSTR lpszString, DWORD dwSize)
{
	if (lpszString == NULL || dwSize == 0)
		return _T("");

	// Allocate a buffer
	LPWSTR pszBuff = (LPWSTR) GlobalAllocPtr(GHND, dwSize*sizeof(wchar_t));

	// Convert the string to a wide string and dump it out.
	MultiByteToWideChar(CP_ACP, 0, lpszString, -1, pszBuff, dwSize*sizeof(wchar_t));

	TStringStream stream;
	stream << _T("\"") << pszBuff << _T("\"");

	GlobalFreePtr(pszBuff);

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _DumpGUID
//
// Dumps out a COM GUID
//
inline TString _DumpGUID(GUID& guid)
{
	// Convert the GUID to a string --
	TCHAR chBuff[_MAX_PATH];
	StringFromGUID2(guid, chBuff, _MAX_PATH);

	TStringStream stream;
	stream << _T("\"") << chBuff << _T("\"");

	return stream.str();

}// _DumpGUID

///////////////////////////////////////////////////////////////////////////
// _DumpBits
//
// Dumps out a bit-flag mask
//
inline TString _DumpBits(DWORD dwBits, DWtoString* parrCheck)
{
	TStringStream stream;
	bool fFirstOne = true;
	while (parrCheck->pszText != NULL)
	{
		if ((dwBits & parrCheck->dwBitMask) == parrCheck->dwBitMask)
		{
			if (fFirstOne)
			{
				stream << _T(" (");
				fFirstOne = false;
			}
			else
				stream << _T(",");
			stream << parrCheck->pszText;
		}
		parrCheck++;
	}

	if (!fFirstOne)
		stream << _T(")");
	return stream.str();

}// _DumpBits

///////////////////////////////////////////////////////////////////////////
// _DumpExact
//
// Dumps out an exact value from DWtoString
//
inline TString _DumpExact(DWORD dwValue, DWtoString* parrCheck)
{
	TStringStream stream;
	while (parrCheck->pszText != NULL)
	{
		if (dwValue == parrCheck->dwBitMask)
		{
			stream << _T("(") << parrCheck->pszText << _T(")");
			break;
		}
		parrCheck++;
	}
	return stream.str();

}// _DumpExact

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEDIALPARAMS structure
//
inline TString _Dump(bool fTitle, LPLINEDIALPARAMS lpDialParams)
{
	// Build the trace stream
	TStringStream stream;

	if (fTitle)
		stream << _T("Dumping LINEDIALPARAMS at 0x") << hex << (DWORD_PTR) lpDialParams << endl;

	// Dump the remainder of the structure
	if (lpDialParams == NULL || IsBadReadPtr(lpDialParams, sizeof(LINEDIALPARAMS)))
	{
		if (lpDialParams != NULL && fTitle)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	if (!fTitle)  stream << _T("    ");
	stream << _T("  dwDialPause         = 0x")	<< hex << lpDialParams->dwDialPause << endl;
	if (!fTitle)  stream << _T("    ");
    stream << _T("  dwDialSpeed         = 0x")  << hex << lpDialParams->dwDialSpeed << endl;
	if (!fTitle)  stream << _T("    ");
    stream << _T("  dwDigitDuration     = 0x")  << hex << lpDialParams->dwDigitDuration << endl;
	if (!fTitle)  stream << _T("    ");
    stream << _T("  dwWaitForDialtone   = 0x")  << hex << lpDialParams->dwWaitForDialtone << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dump a VARSTRING buffer
//
inline TString _Dump (const LPVARSTRING lpVarString)
{                
	TStringStream stream;
	stream << _T("VarString: 0x")		  << hex << (DWORD_PTR)lpVarString << endl;
    stream << _T("  dwTotalSize    = 0x") << hex << lpVarString->dwTotalSize << endl;
    stream << _T("  dwNeededSize   = 0x") << hex << lpVarString->dwNeededSize << endl;
    stream << _T("  dwUsedSize     = 0x") << hex << lpVarString->dwUsedSize << endl;
	stream << _T("  dwStringFormat = 0x") << hex << lpVarString->dwStringFormat << _T(" ");
	stream << _DumpExact(lpVarString->dwStringFormat, _StringFormats) << endl;
    stream << _T("  dwStringSize   = 0x") << hex << lpVarString->dwStringSize << endl;
    stream << _T("  dwStringOffset = 0x") << hex << lpVarString->dwStringOffset;
    if (lpVarString->dwStringOffset > 0 && lpVarString->dwStringSize > 0)
    {
		if (IsBadReadPtr((LPCTSTR)lpVarString + lpVarString->dwStringOffset, lpVarString->dwStringSize))
			stream << _T(" [Bad Pointer]") << endl;
		else if (lpVarString->dwStringFormat == STRINGFORMAT_ASCII)
			stream << _T(" [") << (LPCTSTR)((LPCTSTR)lpVarString + lpVarString->dwStringOffset) << _T("]") << endl; 
		else
			stream << endl << _Dump(lpVarString, lpVarString->dwStringOffset, lpVarString->dwStringSize);
    }                            
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINECALLHUBTRACKINGINFO buffer
//
inline TString _Dump(const LPLINECALLHUBTRACKINGINFO lpcht)
{
	// Build the trace stream
	TStringStream stream;
	stream << _T("LINECALLHUBTRACKING at 0x")	<< hex << (DWORD_PTR)lpcht << endl;

	// Dump the remainder of the structure
	if (lpcht == NULL || IsBadReadPtr(lpcht, sizeof(LINECALLHUBTRACKINGINFO)))
	{
		if (lpcht != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize	          = 0x")	<< hex << lpcht->dwTotalSize << endl;
	stream << _T("  dwNeededSize          = 0x")	<< hex << lpcht->dwNeededSize << endl;
	stream << _T("  dwUsedSize            = 0x")	<< hex << lpcht->dwUsedSize << endl;
	stream << _T("  dwAvailableTracking   = 0x")	<< hex << lpcht->dwAvailableTracking << _T(" ");
	stream << _DumpBits(lpcht->dwAvailableTracking, _CallHubTrackModes) << endl;
	stream << _T("  dwCurrentTracking     = 0x")    << hex << lpcht->dwCurrentTracking << _T(" ");
	stream << _DumpExact(lpcht->dwCurrentTracking, _CallHubTrackModes) << endl;

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINECALLPARAMS buffer
//
inline TString _Dump(const LPLINECALLPARAMS lpCallParams)
{
	// Build the trace stream
	TStringStream stream;
	stream << _T("LINECALLPARAMS at 0x")			<< hex << (DWORD_PTR)lpCallParams << endl;

	// Dump the remainder of the structure
	if (lpCallParams == NULL || IsBadReadPtr(lpCallParams, sizeof(LINECALLPARAMS)))
	{
		if (lpCallParams != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize	          = 0x")	<< hex << lpCallParams->dwTotalSize << endl;
	stream << _T("  dwBearerMode          = 0x")	<< hex << lpCallParams->dwBearerMode << endl;
	stream << _T("  dwMinRate             = 0x")	<< hex << lpCallParams->dwMinRate << endl;
	stream << _T("  dwMaxRate             = 0x")	<< hex << lpCallParams->dwMaxRate << endl;
	stream << _T("  dwMediaMode           = 0x")	<< hex << lpCallParams->dwMediaMode << endl;
	stream << _T("  dwCallParamFlags      = 0x")	<< hex << lpCallParams->dwCallParamFlags << endl;
	stream << _T("  dwAddressMode         = 0x")	<< hex << lpCallParams->dwAddressMode << endl;
	stream << _T("  dwAddressID           = 0x")	<< hex << lpCallParams->dwAddressID << endl;
	stream << _T("  DialParams            =") << endl;
	stream << _Dump(false, &lpCallParams->DialParams);
	stream << _T("  dwOrigAddressSize     = 0x")	<< hex << lpCallParams->dwOrigAddressSize << endl;
	stream << _T("  dwOrigAddressOffset	  = 0x")	<< hex << lpCallParams->dwOrigAddressOffset << _T(" ");
	stream << _DumpASCII(lpCallParams, lpCallParams->dwOrigAddressOffset, lpCallParams->dwOrigAddressSize) << endl;
	stream << _T("  dwDisplayableAddressSize  = 0x") << hex << lpCallParams->dwDisplayableAddressSize << endl;
	stream << _T("  dwDisplayableAddressOffset= 0x") << hex << lpCallParams->dwDisplayableAddressOffset << _T(" ");
	stream << _DumpASCII(lpCallParams, lpCallParams->dwDisplayableAddressOffset, lpCallParams->dwDisplayableAddressSize) << endl;
	stream << _T("  dwCalledPartySize     = 0x")	<< hex << lpCallParams->dwCalledPartySize << endl;
	stream << _T("  dwCalledPartyOffset	  = 0x")	<< hex << lpCallParams->dwCalledPartyOffset << _T(" ");
	stream << _DumpASCII(lpCallParams, lpCallParams->dwCalledPartyOffset, lpCallParams->dwCalledPartySize) << endl;
	stream << _T("  dwCommentSize         = 0x")	<< hex << lpCallParams->dwCommentSize << endl;
	stream << _T("  dwCommentOffset		  = 0x")	<< hex << lpCallParams->dwCommentOffset << _T(" ");
	stream << _DumpASCII(lpCallParams, lpCallParams->dwCommentOffset, lpCallParams->dwCommentSize) << endl;
	stream << _T("  dwUserUserInfoSize	  = 0x")	<< hex << lpCallParams->dwUserUserInfoSize << endl;
	stream << _T("  dwUserUserInfoOffset  = 0x")	<< hex << lpCallParams->dwUserUserInfoOffset << endl;
	stream << _Dump(lpCallParams, lpCallParams->dwUserUserInfoOffset, lpCallParams->dwUserUserInfoSize);
	stream << _T("  dwHighLevelCompSize	  = 0x")	<< hex << lpCallParams->dwHighLevelCompSize << endl;
	stream << _T("  dwHighLevelCompOffset = 0x")	<< hex << lpCallParams->dwHighLevelCompOffset << endl;
	stream << _Dump(lpCallParams, lpCallParams->dwHighLevelCompOffset, lpCallParams->dwHighLevelCompSize);
	stream << _T("  dwLowLevelCompSize	  = 0x")	<< hex << lpCallParams->dwLowLevelCompSize << endl;
	stream << _T("  dwLowLevelCompOffset  = 0x")	<< hex << lpCallParams->dwLowLevelCompOffset << endl;
	stream << _Dump(lpCallParams, lpCallParams->dwLowLevelCompOffset, lpCallParams->dwLowLevelCompSize);
	stream << _T("  dwDevSpecificSize     = 0x")	<< hex << lpCallParams->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset   = 0x")	<< hex << lpCallParams->dwDevSpecificOffset << endl;
	stream << _Dump(lpCallParams, lpCallParams->dwDevSpecificOffset, lpCallParams->dwDevSpecificSize);
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEGENERATETONE buffer
//
inline TString _Dump(DWORD dwToneMode, LPLINEGENERATETONE const lpcTone, DWORD dwNumTones)
{
	// If the current trace level isn't high enough, exit.
	if (dwToneMode != LINETONEMODE_CUSTOM || lpcTone == NULL || dwNumTones == 0)
		return _T("");

	LPLINEGENERATETONE lpTone = (LPLINEGENERATETONE) lpcTone;	

	// Build the trace stream
	TStringStream stream;

	for (unsigned int i = 0; i < dwNumTones; i++)
	{
		stream << _T("LINEGENERATETONE at 0x") << hex << (DWORD_PTR)lpTone << endl;

		// Dump the remainder of the structure
		if (lpTone == NULL || IsBadReadPtr(lpTone, sizeof(LINEGENERATETONE)))
		{
			if (lpTone != NULL)
				stream << _T("  ** Bad Pointer **") << endl;
			return stream.str();
		}

		stream << _T("  dwFrequency  = 0x") << hex << lpTone->dwFrequency << endl;
		stream << _T("  dwCadenceOn  = 0x") << hex << lpTone->dwCadenceOn << endl;
		stream << _T("  dwCadenceOff = 0x") << hex << lpTone->dwCadenceOff << endl;
		stream << _T("  dwVolume     = 0x") << hex << lpTone->dwVolume << endl;

		lpTone++;
	}

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _DumpCallTreatmentRecs
//
// Dumps out a series of CALLTREATMENT structures
//
inline TString _DumpCallTreatmentRecs(LPVOID lpvBuff, DWORD dwOffset, DWORD dwCount, DWORD dwTotalSize)
{
	// If the current trace level isn't high enough, exit.
	if ((GetTraceLevel() & TRC_STRUCT) == 0 || dwCount == 0 || dwTotalSize == 0)
		return _T("");

	// Build the trace stream
	TStringStream stream;

	// Quick validation of the records
	if ((sizeof(LINECALLTREATMENTENTRY) * dwCount) > dwTotalSize)
	{
		stream << _T("  ** Bad Pointer **") << endl;
		return (stream.str());
	}

	// Dump each of the records
	LPLINECALLTREATMENTENTRY const lpTreatment = reinterpret_cast<LPLINECALLTREATMENTENTRY const>((LPBYTE)lpvBuff+dwOffset);
	for (unsigned int i = 0; i < dwCount; i++)
	{
		stream << _T("LINECALLTREATMENTENTRY at 0x") << hex << (DWORD_PTR)lpTreatment << endl;
		stream << _T("  dwCallTreatmentID = 0x")	<< hex << lpTreatment->dwCallTreatmentID << endl;
		stream << _T("  CallTreatmentName = ") << _DumpASCII(lpvBuff, lpTreatment->dwCallTreatmentNameOffset, lpTreatment->dwCallTreatmentNameSize) << endl;
	}

	return (stream.str());
	
}// _DumpCallTreatmentRecs

///////////////////////////////////////////////////////////////////////////
// _DumpCompletionRecs
//
// Dumps out a series of Completion message records
//
inline TString _DumpCompletionRecs(LPVOID lpvBuff, DWORD /*dwOffset*/, DWORD dwEntrySize, DWORD dwCount, DWORD dwTotalSize)
{
	// If the current trace level isn't high enough, exit.
	if ((GetTraceLevel() & TRC_STRUCT) == 0 || dwCount == 0 || dwTotalSize == 0)
		return _T("");

	// Build the trace stream
	TStringStream stream;

	// Quick validation of the records
	if ((dwEntrySize * dwCount) > dwTotalSize)
	{
		stream << _T("  ** Bad Pointer **") << endl;
		return (stream.str());
	}

	// Dump each of the records
	for (unsigned int i = 0; i < dwCount; i++)
	{
		stream << _T("COMPLETIONRECORD at 0x") << hex << (DWORD_PTR)((LPBYTE)lpvBuff+(dwEntrySize*i)) << _T(" ");
		stream << _DumpASCII(lpvBuff, (dwEntrySize*i), dwEntrySize) << endl;
	}

	return (stream.str());

}// _DumpCompletionRecs

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEDEVCAPS
//
inline TString _Dump(LPLINEDEVCAPS lpLineDevCaps)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINEDEVCAPS at 0x") << hex << (DWORD_PTR) lpLineDevCaps << endl;

	// Dump the remainder of the structure
	if (lpLineDevCaps == NULL || IsBadReadPtr(lpLineDevCaps, sizeof(LINEDEVCAPS)))
	{
		if (lpLineDevCaps != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize             = 0x")	<< hex << lpLineDevCaps->dwTotalSize << endl;
	stream << _T("  dwNeededSize            = 0x")	<< hex << lpLineDevCaps->dwNeededSize << endl;
	stream << _T("  dwUsedSize              = 0x")	<< hex << lpLineDevCaps->dwUsedSize << endl;
	stream << _T("  dwProviderInfoSize      = 0x")	<< hex << lpLineDevCaps->dwProviderInfoSize << endl;
	stream << _T("  dwProviderInfoOffset    = 0x")	<< hex << lpLineDevCaps->dwProviderInfoOffset << _T(" ");
	stream << _DumpASCII(lpLineDevCaps, lpLineDevCaps->dwProviderInfoOffset, lpLineDevCaps->dwProviderInfoSize) << endl;
	stream << _T("  dwSwitchInfoSize        = 0x")	<< hex << lpLineDevCaps->dwSwitchInfoSize << endl;
	stream << _T("  dwSwitchInfoOffset      = 0x")	<< hex << lpLineDevCaps->dwSwitchInfoOffset << _T(" ");
	stream << _DumpASCII(lpLineDevCaps, lpLineDevCaps->dwSwitchInfoOffset, lpLineDevCaps->dwSwitchInfoSize) << endl;
	stream << _T("  dwPermanentLineID       = 0x")	<< hex << lpLineDevCaps->dwPermanentLineID << endl;
	stream << _T("  dwLineNameSize          = 0x")	<< hex << lpLineDevCaps->dwLineNameSize << endl;
	stream << _T("  dwLineNameOffset        = 0x")	<< hex << lpLineDevCaps->dwLineNameOffset << _T(" ");
	stream << _DumpASCII(lpLineDevCaps, lpLineDevCaps->dwLineNameOffset, lpLineDevCaps->dwLineNameSize) << endl;
	stream << _T("  dwStringFormat          = 0x")	<< hex << lpLineDevCaps->dwStringFormat << _T(" ");
	stream << _DumpExact(lpLineDevCaps->dwStringFormat, _StringFormats) << endl;
	stream << _T("  dwAddressModes          = 0x")	<< hex << lpLineDevCaps->dwAddressModes << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwAddressModes, _AddressModes) << endl;
	stream << _T("  dwNumAddresses          = 0x")	<< hex << lpLineDevCaps->dwNumAddresses << endl;
	stream << _T("  dwBearerModes           = 0x")	<< hex << lpLineDevCaps->dwBearerModes << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwBearerModes, _BearerModes) << endl;
	stream << _T("  dwMaxRate               = 0x")	<< hex << lpLineDevCaps->dwMaxRate << endl;
	stream << _T("  dwMediaModes            = 0x")	<< hex << lpLineDevCaps->dwMediaModes << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwMediaModes, _MediaModes) << endl;
	stream << _T("  dwGenerateToneModes     = 0x")	<< hex << lpLineDevCaps->dwGenerateToneModes << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwGenerateToneModes, _ToneModes) << endl;
	stream << _T("  dwGenerateToneMaxNumFreq= 0x")	<< hex << lpLineDevCaps->dwGenerateToneMaxNumFreq << endl;
	stream << _T("  dwGenerateDigitModes    = 0x")	<< hex << lpLineDevCaps->dwGenerateDigitModes << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwGenerateDigitModes, _DigitModes) << endl;
	stream << _T("  dwMonitorToneMaxNumFreq = 0x")	<< hex << lpLineDevCaps->dwMonitorToneMaxNumFreq << endl;
	stream << _T("  dwMonitorToneMaxNumEntries = 0x")	<< hex << lpLineDevCaps->dwMonitorToneMaxNumEntries << endl;
	stream << _T("  dwMonitorDigitModes     = 0x")	<< hex << lpLineDevCaps->dwMonitorDigitModes << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwMonitorDigitModes, _DigitModes) << endl;
	stream << _T("  dwGatherDigitsMinTimeout= 0x")	<< hex << lpLineDevCaps->dwGatherDigitsMinTimeout << endl;
	stream << _T("  dwGatherDigitsMaxTimeout= 0x")	<< hex << lpLineDevCaps->dwGatherDigitsMaxTimeout << endl;
	stream << _T("  dwMedCtlDigitMaxListSize= 0x")	<< hex << lpLineDevCaps->dwMedCtlDigitMaxListSize << endl;
	stream << _T("  dwMedCtlMediaMaxListSize= 0x")	<< hex << lpLineDevCaps->dwMedCtlMediaMaxListSize << endl;
	stream << _T("  dwMedCtlToneMaxListSize = 0x")	<< hex << lpLineDevCaps->dwMedCtlToneMaxListSize << endl;
	stream << _T("  dwMedCtlCallStateMaxListSize = 0x")	<< hex << lpLineDevCaps->dwMedCtlCallStateMaxListSize << endl;
	stream << _T("  dwDevCapFlags           = 0x")	<< hex << lpLineDevCaps->dwDevCapFlags << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwDevCapFlags, _DevCapFlags) << endl;
	stream << _T("  dwMaxNumActiveCalls     = 0x")	<< hex << lpLineDevCaps->dwMaxNumActiveCalls << endl;
	stream << _T("  dwAnswerMode            = 0x")	<< hex << lpLineDevCaps->dwAnswerMode << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwAnswerMode, _AnswerMode) << endl;
	stream << _T("  dwRingModes             = 0x")	<< hex << lpLineDevCaps->dwRingModes << endl;
	stream << _T("  dwLineStates            = 0x")	<< hex << lpLineDevCaps->dwLineStates << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwLineStates, _DevStates) << endl;
	stream << _T("  dwUUIAcceptSize         = 0x")	<< hex << lpLineDevCaps->dwUUIAcceptSize << endl;
	stream << _T("  dwUUIAnswerSize         = 0x")	<< hex << lpLineDevCaps->dwUUIAnswerSize << endl;
	stream << _T("  dwUUIMakeCallSize       = 0x")	<< hex << lpLineDevCaps->dwUUIMakeCallSize << endl;
	stream << _T("  dwUUIDropSize           = 0x")	<< hex << lpLineDevCaps->dwUUIDropSize << endl;
	stream << _T("  dwUUISendUserUserInfoSize = 0x")	<< hex << lpLineDevCaps->dwUUISendUserUserInfoSize << endl;
	stream << _T("  dwUUICallInfoSize       = 0x")	<< hex << lpLineDevCaps->dwUUICallInfoSize << endl;
	stream << _T("  MinDialParams           =") << endl;
	stream << _Dump(false, &lpLineDevCaps->MinDialParams);
	stream << _T("  MaxDialParams           =") << endl;
	stream << _Dump(false, &lpLineDevCaps->MaxDialParams);
	stream << _T("  DefaultDialParams       =") << endl;
	stream << _Dump(false, &lpLineDevCaps->DefaultDialParams);
	stream << _T("  dwNumTerminals          = 0x")	<< hex << lpLineDevCaps->dwNumTerminals << endl;
	stream << _T("  dwTerminalCapsSize      = 0x")	<< hex << lpLineDevCaps->dwTerminalCapsSize << endl;
	stream << _T("  dwTerminalCapsOffset    = 0x")	<< hex << lpLineDevCaps->dwTerminalCapsOffset << endl;
	
	if (lpLineDevCaps->dwNumTerminals > 0 &&
		lpLineDevCaps->dwTerminalCapsSize >= sizeof(LINETERMCAPS) &&
		lpLineDevCaps->dwTerminalCapsOffset > 0)
	{
		LPLINETERMCAPS lpTermCaps = (LPLINETERMCAPS)((LPBYTE)lpLineDevCaps + lpLineDevCaps->dwTerminalCapsOffset);
		for (unsigned int i = 0; i < lpLineDevCaps->dwNumTerminals; i++)
		{
			stream << _T("  LINETERMCAP[")	<< i << _T("]") << endl;
			stream << _T("      dwTermDev     = 0x") << hex << lpTermCaps->dwTermDev << _T(" ");
			stream << _DumpBits(lpTermCaps->dwTermDev, _TermDevices) << endl;
			stream << _T("      dwTermModes   = 0x") << hex << lpTermCaps->dwTermModes << _T(" ");
			stream << _DumpBits(lpTermCaps->dwTermModes, _TermModes) << endl;
			stream << _T("      dwTermSharing = 0x") << hex << lpTermCaps->dwTermSharing << _T(" ");
			stream << _DumpBits(lpTermCaps->dwTermSharing, _ToneModes) << endl;
			lpTermCaps++;
		}
	}

	stream << _T("  dwTerminalTextEntrySize = 0x")	<< hex << lpLineDevCaps->dwTerminalTextEntrySize << endl;
	stream << _T("  dwTerminalTextSize      = 0x")	<< hex << lpLineDevCaps->dwTerminalTextSize << endl;
	stream << _T("  dwTerminalTextOffset    = 0x")	<< hex << lpLineDevCaps->dwTerminalTextOffset << endl;

	if (lpLineDevCaps->dwNumTerminals > 0 &&
		lpLineDevCaps->dwTerminalTextEntrySize > 0 &&
		lpLineDevCaps->dwTerminalTextSize > 0 &&
		lpLineDevCaps->dwTerminalTextOffset > 0)
	{
		for (unsigned int i = 0; i < lpLineDevCaps->dwTerminalTextSize / lpLineDevCaps->dwTerminalTextEntrySize; i++)
		{
			stream << _T("  Entry[")	<< i << _T("] = ");
			stream << _DumpASCII(lpLineDevCaps, (lpLineDevCaps->dwTerminalTextOffset + (i*lpLineDevCaps->dwTerminalTextEntrySize)), lpLineDevCaps->dwTerminalTextEntrySize);
			stream << endl;
		}
	}

	stream << _T("  dwDevSpecificSize       = 0x")	<< hex << lpLineDevCaps->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset     = 0x")	<< hex << lpLineDevCaps->dwDevSpecificOffset << endl;
	stream << _Dump(lpLineDevCaps, lpLineDevCaps->dwDevSpecificOffset, lpLineDevCaps->dwDevSpecificSize);
	stream << _T("  dwLineFeatures          = 0x")  << hex << lpLineDevCaps->dwLineFeatures << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwLineFeatures, _LineFeatures) << endl;
	stream << _T("  dwSettableDevStatus     = 0x")	<< hex << lpLineDevCaps->dwSettableDevStatus << _T(" ");
	stream << _DumpBits(lpLineDevCaps->dwSettableDevStatus, _DevStatusFlags) << endl;
	stream << _T("  dwDeviceClassesSize     = 0x")	<< hex << lpLineDevCaps->dwDeviceClassesSize << endl;
	stream << _T("  dwDeviceClassesOffset   = 0x")	<< hex << lpLineDevCaps->dwDeviceClassesOffset << endl;
	stream << _Dump(lpLineDevCaps, lpLineDevCaps->dwDeviceClassesOffset, lpLineDevCaps->dwDeviceClassesSize);
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEDEVSTATUS structure
//
inline TString _Dump(LPLINEDEVSTATUS lpLineDevStatus)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINEDEVSTATUS at 0x") << hex << (DWORD_PTR) lpLineDevStatus << endl;

	// Dump the remainder of the structure
	if (lpLineDevStatus == NULL || IsBadReadPtr(lpLineDevStatus, sizeof(LINEDEVSTATUS)))
	{
		if (lpLineDevStatus != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize             = 0x")	<< hex << lpLineDevStatus->dwTotalSize << endl;
	stream << _T("  dwNeededSize            = 0x")	<< hex << lpLineDevStatus->dwNeededSize << endl;
	stream << _T("  dwUsedSize              = 0x")	<< hex << lpLineDevStatus->dwUsedSize << endl;
	stream << _T("  dwNumOpens              = 0x")	<< hex << lpLineDevStatus->dwNumOpens << endl;
	stream << _T("  dwOpenMediaModes        = 0x")	<< hex << lpLineDevStatus->dwOpenMediaModes << _T(" ");
	stream << _DumpBits(lpLineDevStatus->dwOpenMediaModes, _MediaModes) << endl;
	stream << _T("  dwNumActiveCalls        = 0x")	<< hex << lpLineDevStatus->dwNumActiveCalls << endl;
	stream << _T("  dwNumOnHoldCalls        = 0x")	<< hex << lpLineDevStatus->dwNumOnHoldCalls << endl;
	stream << _T("  dwNumOnHoldPendCalls    = 0x")	<< hex << lpLineDevStatus->dwNumOnHoldPendCalls << endl;
	stream << _T("  dwLineFeatures          = 0x")	<< hex << lpLineDevStatus->dwLineFeatures << _T(" ");
	stream << _DumpBits(lpLineDevStatus->dwLineFeatures, _LineFeatures) << endl;
	stream << _T("  dwNumCallCompletions    = 0x")	<< hex << lpLineDevStatus->dwNumCallCompletions << endl;
	stream << _T("  dwRingMode              = 0x")	<< hex << lpLineDevStatus->dwRingMode << endl;
	stream << _T("  dwSignalLevel           = 0x")	<< hex << lpLineDevStatus->dwSignalLevel << endl;
	stream << _T("  dwBatteryLevel          = 0x")	<< hex << lpLineDevStatus->dwBatteryLevel << endl;
	stream << _T("  dwRoamMode              = 0x")	<< hex << lpLineDevStatus->dwRoamMode << endl;
	stream << _T("  dwDevStatusFlags        = 0x")	<< hex << lpLineDevStatus->dwDevStatusFlags << _T(" ");
	stream << _DumpBits(lpLineDevStatus->dwDevStatusFlags, _DevStatusFlags) << endl;
	stream << _T("  dwTerminalModesSize     = 0x")	<< hex << lpLineDevStatus->dwTerminalModesSize << endl;
	stream << _T("  dwTerminalModesOffset   = 0x")	<< hex << lpLineDevStatus->dwTerminalModesOffset << endl;

	if (lpLineDevStatus->dwTerminalModesSize > 0 && 
		lpLineDevStatus->dwTerminalModesOffset > 0)
	{
		LPDWORD lpTermModes = (LPDWORD)((LPBYTE)lpLineDevStatus + lpLineDevStatus->dwTerminalModesOffset);
		for (unsigned int i = 0; i < lpLineDevStatus->dwTerminalModesSize/sizeof(DWORD); i++)
		{
			stream << _T("  TERMINAL[")				<< i << _T("]") << _T(" = ");
			stream << _DumpBits(*lpTermModes, _TermModes) << endl;
			lpTermModes++;
		}
	}

	stream << _T("  dwDevSpecificSize       = 0x")	<< hex << lpLineDevStatus->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset     = 0x")	<< hex << lpLineDevStatus->dwDevSpecificOffset << endl;
	stream << _Dump(lpLineDevStatus, lpLineDevStatus->dwDevSpecificOffset, lpLineDevStatus->dwDevSpecificSize);

	stream << _T("  dwAvailableMediaModes   = 0x")	<< hex << lpLineDevStatus->dwAvailableMediaModes << _T(" ");
	stream << _DumpBits(lpLineDevStatus->dwAvailableMediaModes, _MediaModes) << endl;

	stream << _T("  dwAppInfoSize           = 0x")	<< hex << lpLineDevStatus->dwAppInfoSize << endl;
	stream << _T("  dwAppInfoOffset         = 0x")	<< hex << lpLineDevStatus->dwAppInfoOffset << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEADDRESSCAPS
//
inline TString _Dump(LPLINEADDRESSCAPS lpAddressCaps)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINEADDRESSCAPS at 0x") << hex << (DWORD_PTR) lpAddressCaps << endl;

	// Dump the remainder of the structure
	if (lpAddressCaps == NULL || IsBadReadPtr(lpAddressCaps, sizeof(LINEADDRESSCAPS)))
	{
		if (lpAddressCaps != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize             = 0x")	<< hex << lpAddressCaps->dwTotalSize << endl;
	stream << _T("  dwNeededSize            = 0x")	<< hex << lpAddressCaps->dwNeededSize << endl;
	stream << _T("  dwUsedSize              = 0x")	<< hex << lpAddressCaps->dwUsedSize << endl;
	stream << _T("  dwLineDeviceID          = 0x")  << hex << lpAddressCaps->dwLineDeviceID << endl;
	stream << _T("  dwAddressSize           = 0x")	<< hex << lpAddressCaps->dwAddressSize << endl;
	stream << _T("  dwAddressOffset         = 0x")	<< hex << lpAddressCaps->dwAddressOffset << _T(" ");
	stream << _DumpASCII(lpAddressCaps, lpAddressCaps->dwAddressOffset, lpAddressCaps->dwAddressSize) << endl;
	stream << _T("  dwDevSpecificSize       = 0x")  << hex << lpAddressCaps->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset     = 0x")  << hex << lpAddressCaps->dwDevSpecificOffset << endl;
	stream << _Dump(lpAddressCaps, lpAddressCaps->dwDevSpecificSize, lpAddressCaps->dwDevSpecificOffset);
	stream << _T("  dwAddressSharing        = 0x")	<< hex << lpAddressCaps->dwAddressSharing << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwAddressSharing, _AddressSharing) << endl;
	stream << _T("  dwAddressStates         = 0x")	<< hex << lpAddressCaps->dwAddressStates << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwAddressStates, _AddressStates) << endl;
	stream << _T("  dwCallInfoStates        = 0x")	<< hex << lpAddressCaps->dwCallInfoStates << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallInfoStates, _CIStates) << endl;
	stream << _T("  dwCallerIDFlags         = 0x")	<< hex << lpAddressCaps->dwCallerIDFlags << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallerIDFlags, _CIDFLags) << endl;
	stream << _T("  dwCalledIDFlags         = 0x")	<< hex << lpAddressCaps->dwCalledIDFlags << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCalledIDFlags, _CIDFLags) << endl;
	stream << _T("  dwConnectedIDFlags      = 0x")	<< hex << lpAddressCaps->dwConnectedIDFlags << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwConnectedIDFlags, _CIDFLags) << endl;
	stream << _T("  dwRedirectionIDFlags    = 0x")	<< hex << lpAddressCaps->dwRedirectionIDFlags << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwRedirectionIDFlags, _CIDFLags) << endl;
	stream << _T("  dwRedirectingIDFlags    = 0x")	<< hex << lpAddressCaps->dwRedirectingIDFlags << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwRedirectingIDFlags, _CIDFLags) << endl;
	stream << _T("  dwCallStates            = 0x")	<< hex << lpAddressCaps->dwCallStates << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallStates, _CallStates) << endl;
	stream << _T("  dwDialToneModes         = 0x")	<< hex << lpAddressCaps->dwDialToneModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwDialToneModes, _DialToneModes) << endl;
	stream << _T("  dwBusyModes             = 0x")	<< hex << lpAddressCaps->dwBusyModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwBusyModes, _BusyModes) << endl;
	stream << _T("  dwSpecialInfo           = 0x")	<< hex << lpAddressCaps->dwSpecialInfo << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwSpecialInfo, _SpecialInfo) << endl;
	stream << _T("  dwDisconnectModes       = 0x")	<< hex << lpAddressCaps->dwDisconnectModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwDisconnectModes, _DisconnectModes) << endl;
	stream << _T("  dwMaxNumActiveCalls     = 0x")	<< hex << lpAddressCaps->dwMaxNumActiveCalls << endl;
	stream << _T("  dwMaxNumOnHoldCalls     = 0x")	<< hex << lpAddressCaps->dwMaxNumOnHoldCalls << endl;
	stream << _T("  dwMaxNumOnHoldPendingCalls = 0x")	<< hex << lpAddressCaps->dwMaxNumOnHoldPendingCalls << endl;
	stream << _T("  dwMaxNumConference      = 0x")	<< hex << lpAddressCaps->dwMaxNumConference << endl;
	stream << _T("  dwMaxNumTransConf       = 0x")	<< hex << lpAddressCaps->dwMaxNumTransConf << endl;
	stream << _T("  dwAddrCapFlags          = 0x")  << hex << lpAddressCaps->dwAddrCapFlags << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwAddrCapFlags, _AddressCaps) << endl;
	stream << _T("  dwCallFeatures          = 0x")	<< hex << lpAddressCaps->dwCallFeatures << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallFeatures, _CallFeatures) << endl;
	stream << _T("  dwRemoveFromConfCaps    = 0x")	<< hex << lpAddressCaps->dwRemoveFromConfCaps << _T(" ");
	stream << _DumpExact(lpAddressCaps->dwRemoveFromConfCaps, _RemoveFromConf) << endl;
	stream << _T("  dwRemoveFromConfState   = 0x")	<< hex << lpAddressCaps->dwRemoveFromConfState << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwRemoveFromConfState, _CallStates) << endl;
	stream << _T("  dwTransferModes         = 0x")	<< hex << lpAddressCaps->dwTransferModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwTransferModes, _TransferModes) << endl;
	stream << _T("  dwParkModes             = 0x")	<< hex << lpAddressCaps->dwParkModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwParkModes, _ParkModes) << endl;
	stream << _T("  dwForwardModes          = 0x")	<< hex << lpAddressCaps->dwForwardModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwForwardModes, _ForwardModes) << endl;
	stream << _T("  dwMaxForwardEntries     = ")	<< lpAddressCaps->dwMaxForwardEntries << endl;
	stream << _T("  dwMaxSpecificEntries    = ")	<< lpAddressCaps->dwMaxSpecificEntries << endl;
	stream << _T("  dwMinFwdNumRings        = ")	<< lpAddressCaps->dwMinFwdNumRings << endl;
	stream << _T("  dwMaxFwdNumRings        = ")	<< lpAddressCaps->dwMaxFwdNumRings << endl;
	stream << _T("  dwMaxCallCompletions    = ")	<< lpAddressCaps->dwMaxCallCompletions << endl;
	stream << _T("  dwCallCompletionConds   = 0x")	<< hex << lpAddressCaps->dwCallCompletionConds << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallCompletionConds, _CompletionConditions) << endl;
	stream << _T("  dwCallCompletionModes   = 0x")	<< hex << lpAddressCaps->dwCallCompletionModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallCompletionModes, _CompletionModes) << endl;
	stream << _T("  dwNumCompletionMessages = ")	<< lpAddressCaps->dwNumCompletionMessages << endl;
	stream << _DumpCompletionRecs(lpAddressCaps, lpAddressCaps->dwCompletionMsgTextOffset, lpAddressCaps->dwCompletionMsgTextEntrySize, lpAddressCaps->dwNumCompletionMessages, lpAddressCaps->dwCompletionMsgTextSize).c_str();
	stream << _T("  dwPredictiveAutoTransferStates = 0x")	<< hex << lpAddressCaps->dwPredictiveAutoTransferStates << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwPredictiveAutoTransferStates, _CallStates) << endl;
	stream << _T("  dwNumCallTreatments     = ")	<< hex << lpAddressCaps->dwNumCallTreatments << endl;
	stream << _DumpCallTreatmentRecs(lpAddressCaps, lpAddressCaps->dwCallTreatmentListOffset, lpAddressCaps->dwNumCallTreatments, lpAddressCaps->dwCallTreatmentListSize).c_str();
	stream << _T("  dwDeviceClassesSize     = ")	<< lpAddressCaps->dwDeviceClassesSize << endl;
	stream << _T("  dwDeviceClassesOffset   = 0x")	<< lpAddressCaps->dwDeviceClassesOffset << endl;
	stream << _Dump(lpAddressCaps, lpAddressCaps->dwDeviceClassesOffset, lpAddressCaps->dwDeviceClassesSize);
	stream << _T("  dwMaxCallDataSize       = ")	<< lpAddressCaps->dwMaxCallDataSize << _T(" bytes") << endl;
	stream << _T("  dwCallFeatures2         = 0x")	<< hex << lpAddressCaps->dwCallFeatures2 << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwCallFeatures2, _CallFeatures2) << endl;
	stream << _T("  dwMaxNoAnswerTimeout    = 0x")	<< hex << lpAddressCaps->dwMaxNoAnswerTimeout << endl;
	stream << _T("  dwConnectedModes        = 0x")	<< hex << lpAddressCaps->dwConnectedModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwConnectedModes, _ConnectedModes) << endl;
	stream << _T("  dwOfferingModes         = 0x")	<< hex << lpAddressCaps->dwOfferingModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwOfferingModes, _OfferingModes) << endl;
	stream << _T("  dwAvailableMediaModes   = 0x")	<< hex << lpAddressCaps->dwAvailableMediaModes << _T(" ");
	stream << _DumpBits(lpAddressCaps->dwAvailableMediaModes, _MediaModes) << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a SYSTEMTIME structure
//
inline TString _Dump(SYSTEMTIME& st)
{
	// Build the trace stream
	TStringStream stream;
	stream << (DWORD)st.wMonth << _T("-") << (DWORD)st.wDay    << _T("-") << (DWORD)st.wYear << _T(" ");
	stream << (DWORD)st.wHour  << _T(":") << (DWORD)st.wMinute << _T(":") << (DWORD)st.wSecond << _T(".") << (DWORD)st.wMilliseconds;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEEXTENSION structure
//
inline TString _Dump(LPLINEEXTENSIONID lpExtensionID)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINEEXTENSIONID at 0x") << hex << (DWORD_PTR) lpExtensionID << endl;

	// Dump the remainder of the structure
	if (lpExtensionID == NULL || IsBadReadPtr(lpExtensionID, sizeof(LINEEXTENSIONID)))
	{
		if (lpExtensionID != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwExtensionID0          = 0x")	<< hex << lpExtensionID->dwExtensionID0 << endl;
	stream << _T("  dwExtensionID1          = 0x")	<< hex << lpExtensionID->dwExtensionID1 << endl;
	stream << _T("  dwExtensionID2          = 0x")	<< hex << lpExtensionID->dwExtensionID2 << endl;
	stream << _T("  dwExtensionID3          = 0x")	<< hex << lpExtensionID->dwExtensionID3 << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINECALLSTATUS buffer
//
inline TString _Dump(LPLINECALLSTATUS const lpCallStatus)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINECALLSTATUS at 0x") << hex << (DWORD_PTR) lpCallStatus << endl;

	// Dump the remainder of the structure
	if (lpCallStatus == NULL || IsBadReadPtr(lpCallStatus, sizeof(LINECALLSTATUS)))
	{
		if (lpCallStatus != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize             = 0x")	<< hex << lpCallStatus->dwTotalSize << endl;
	stream << _T("  dwNeededSize            = 0x")	<< hex << lpCallStatus->dwNeededSize << endl;
	stream << _T("  dwUsedSize              = 0x")	<< hex << lpCallStatus->dwUsedSize << endl;
	stream << _T("  dwCallState             = 0x")	<< hex << lpCallStatus->dwCallState << _T(" ");
	stream << _DumpBits(lpCallStatus->dwCallState, _CallStates) << endl;
	stream << _T("  dwCallStateMode         = 0x")	<< hex << lpCallStatus->dwCallStateMode << _T(" ");

	switch (lpCallStatus->dwCallState)
	{
		case LINECALLSTATE_CONNECTED:		stream << _DumpBits(lpCallStatus->dwCallStateMode, _ConnectedModes); break;
		case LINECALLSTATE_DISCONNECTED:    stream << _DumpBits(lpCallStatus->dwCallStateMode, _DisconnectModes); break;
		case LINECALLSTATE_DIALTONE:		stream << _DumpBits(lpCallStatus->dwCallStateMode, _DialToneModes); break;
		case LINECALLSTATE_OFFERING:		stream << _DumpBits(lpCallStatus->dwCallStateMode, _OfferingModes); break;
		case LINECALLSTATE_BUSY:			stream << _DumpBits(lpCallStatus->dwCallStateMode, _BusyModes); break;
	}
	stream << endl;

	stream << _T("  dwCallPrivilege         = 0x")	<< hex << lpCallStatus->dwCallPrivilege << _T(" ");
	stream << _DumpBits(lpCallStatus->dwCallPrivilege, _CallPrivilege) << endl;
	stream << _T("  dwCallFeatures          = 0x")	<< hex << lpCallStatus->dwCallFeatures << _T(" ");
	stream << _DumpBits(lpCallStatus->dwCallFeatures, _CallFeatures) << endl;
	stream << _T("  dwDevSpecificSize       = 0x")	<< hex << lpCallStatus->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset     = 0x")	<< hex << lpCallStatus->dwDevSpecificOffset << endl;
	stream << _Dump(lpCallStatus, lpCallStatus->dwDevSpecificSize, lpCallStatus->dwDevSpecificOffset);
	stream << _T("  dwCallFeatures2         = 0x")	<< hex << lpCallStatus->dwCallFeatures2 << _T(" ");
	stream << _DumpBits(lpCallStatus->dwCallFeatures2, _CallFeatures2) << endl;
	stream << _T("  tStateEntryTime         = ") << _Dump(lpCallStatus->tStateEntryTime) << endl;

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEADDRESSSTATUS buffer
//
inline TString _Dump(LPLINEADDRESSSTATUS const lpAddressStatus)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINEADDRESSSTATUS at 0x") << hex << (DWORD_PTR) lpAddressStatus << endl;

	// Dump the remainder of the structure
	if (lpAddressStatus == NULL || IsBadReadPtr(lpAddressStatus, sizeof(LINEADDRESSSTATUS)))
	{
		if (lpAddressStatus != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize             = 0x")	<< hex << lpAddressStatus->dwTotalSize << endl;
	stream << _T("  dwNeededSize            = 0x")	<< hex << lpAddressStatus->dwNeededSize << endl;
	stream << _T("  dwUsedSize              = 0x")	<< hex << lpAddressStatus->dwUsedSize << endl;
	stream << _T("  dwNumInUse              = 0x")	<< hex << lpAddressStatus->dwNumInUse << endl;
	stream << _T("  dwNumActiveCalls        = 0x")	<< hex << lpAddressStatus->dwNumActiveCalls << endl;
	stream << _T("  dwNumOnHoldCalls        = 0x")	<< hex << lpAddressStatus->dwNumOnHoldCalls << endl;
	stream << _T("  dwNumOnHoldPendCalls    = 0x")	<< hex << lpAddressStatus->dwNumOnHoldPendCalls << endl;
	stream << _T("  dwAddressFeatures       = 0x")	<< hex << lpAddressStatus->dwAddressFeatures << _T(" ");
	stream << _DumpBits(lpAddressStatus->dwAddressFeatures, _AddressFeatures) << endl;
	stream << _T("  dwNumRingsNoAnswer      = 0x")	<< hex << lpAddressStatus->dwNumRingsNoAnswer << endl;
	stream << _T("  dwForwardNumEntries     = 0x")	<< hex << lpAddressStatus->dwForwardNumEntries << endl;
	stream << _T("  dwForwardSize           = 0x")	<< hex << lpAddressStatus->dwForwardSize << endl;
	stream << _T("  dwForwardOffset         = 0x")	<< hex << lpAddressStatus->dwForwardOffset << endl;

	if (lpAddressStatus->dwForwardNumEntries > 0 &&
		lpAddressStatus->dwForwardSize >= sizeof(LINEFORWARD) &&
		lpAddressStatus->dwForwardOffset > 0)
	{
		LINEFORWARD* lpForward = (LINEFORWARD*)((LPBYTE)lpAddressStatus + lpAddressStatus->dwForwardOffset);
		for (unsigned int i = 0; i < lpAddressStatus->dwForwardNumEntries; i++)
		{
			stream << _T("  LINEFORWARD[")				<< i << _T("]") << endl;
			stream << _T("    dwForwardMode         = 0x")	<< hex << lpForward->dwForwardMode;
			stream << _T(" (") << _DumpBits(lpForward->dwForwardMode, _ForwardModes) << _T(")") << endl;
			stream << _T("    dwCallerAddressSize   = 0x")	<< hex << lpForward->dwCallerAddressSize << endl;
			stream << _T("    dwCallerAddressOffset = 0x")	<< hex << lpForward->dwCallerAddressOffset << _T(" ");
			stream << _DumpASCII(lpAddressStatus, lpForward->dwCallerAddressOffset, lpForward->dwCallerAddressSize) << endl;
			stream << _T("    dwDestCountryCode     = ")	<< lpForward->dwDestCountryCode << endl;
			stream << _T("    dwDestAddressSize     = ")    << lpForward->dwDestAddressSize << endl;
			stream << _T("    dwDestAddressOffset   = ")    << lpForward->dwDestAddressOffset << _T(" ");
			stream << _DumpASCII(lpAddressStatus, lpForward->dwDestAddressOffset, lpForward->dwDestAddressSize) << endl;
			lpForward++;
		}
	}

	stream << _T("  dwTerminalModesSize     = 0x")	<< hex << lpAddressStatus->dwTerminalModesSize << endl;
	stream << _T("  dwTerminalModesOffset   = 0x")	<< hex << lpAddressStatus->dwTerminalModesOffset << endl;
	
	if (lpAddressStatus->dwTerminalModesSize > 0 && 
		lpAddressStatus->dwTerminalModesOffset > 0)
	{
		LPDWORD lpTermModes = (LPDWORD)((LPBYTE)lpAddressStatus + lpAddressStatus->dwTerminalModesOffset);
		for (unsigned int i = 0; i < lpAddressStatus->dwTerminalModesSize/sizeof(DWORD); i++)
		{
			stream << _T("  TERMINAL[")				<< i << _T("]") << _T(" = ");
			stream << _DumpBits(*lpTermModes, _TermModes) << endl;
			lpTermModes++;
		}
	}

	stream << _T("  dwDevSpecificSize       = 0x")	<< hex << lpAddressStatus->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset     = 0x")	<< hex << lpAddressStatus->dwDevSpecificOffset << endl;
	stream << _Dump(lpAddressStatus, lpAddressStatus->dwDevSpecificSize, lpAddressStatus->dwDevSpecificOffset);
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINECALLINFO buffer
//
inline TString _Dump(LPLINECALLINFO const lpCallInfo)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("LINECALLINFO at 0x") << hex << (DWORD_PTR) lpCallInfo << endl;

	// Dump the remainder of the structure
	if (lpCallInfo == NULL || IsBadReadPtr(lpCallInfo, sizeof(LINECALLINFO)))
	{
		if (lpCallInfo != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize             = 0x")	<< hex << lpCallInfo->dwTotalSize << endl;
	stream << _T("  dwNeededSize            = 0x")	<< hex << lpCallInfo->dwNeededSize << endl;
	stream << _T("  dwUsedSize              = 0x")	<< hex << lpCallInfo->dwUsedSize << endl;
	stream << _T("  hLine                   = 0x")	<< hex << (DWORD_PTR)lpCallInfo->hLine << endl;
	stream << _T("  dwLineDeviceID          = 0x")	<< hex << lpCallInfo->dwLineDeviceID << endl;
	stream << _T("  dwAddressID             = 0x")	<< hex << lpCallInfo->dwAddressID << endl;
	stream << _T("  dwBearerMode            = 0x")	<< hex << lpCallInfo->dwBearerMode << _T(" ");
	stream << _DumpBits(lpCallInfo->dwBearerMode, _BearerModes) << endl;
	stream << _T("  dwRate                  = 0x")	<< hex << lpCallInfo->dwRate << endl;
	stream << _T("  dwMediaMode             = 0x")	<< hex << lpCallInfo->dwMediaMode << _T(" ");
	stream << _DumpBits(lpCallInfo->dwMediaMode, _MediaModes) << endl;
	stream << _T("  dwAppSpecific           = 0x")	<< hex << lpCallInfo->dwAppSpecific << endl;
	stream << _T("  dwCallID                = 0x")	<< hex << lpCallInfo->dwCallID << endl;
	stream << _T("  dwRelatedCallID         = 0x")	<< hex << lpCallInfo->dwRelatedCallID << endl;
	stream << _T("  dwCallParamFlags        = 0x")	<< hex << lpCallInfo->dwCallParamFlags << _T(" ");
	stream << _DumpBits(lpCallInfo->dwCallParamFlags, _CallParamFlags) << endl;
	stream << _T("  dwCallStates            = 0x")	<< hex << lpCallInfo->dwCallStates << _T(" ");
	stream << _DumpBits(lpCallInfo->dwCallStates, _CallStates) << endl;
	stream << _T("  dwMonitorDigitModes     = 0x")	<< hex << lpCallInfo->dwMonitorDigitModes << _T(" ");
	stream << _DumpBits(lpCallInfo->dwMonitorDigitModes, _DigitModes) << endl;
	stream << _T("  dwMonitorMediaModes     = 0x")	<< hex << lpCallInfo->dwMonitorMediaModes << _T(" ");
	stream << _DumpBits(lpCallInfo->dwMonitorMediaModes, _MediaModes) << endl;
	stream << _T("  dwOrigin                = 0x")	<< hex << lpCallInfo->dwOrigin << _T(" ");
	stream << _DumpBits(lpCallInfo->dwOrigin, _CallOrigins) << endl;
	stream << _T("  dwReason                = 0x")	<< hex << lpCallInfo->dwReason << _T(" ");
	stream << _DumpBits(lpCallInfo->dwReason, _CallReasons) << endl;
	stream << _T("  dwCompletionID          = 0x")	<< hex << lpCallInfo->dwCompletionID << endl;
	stream << _T("  dwNumOwners             = 0x")	<< hex << lpCallInfo->dwNumOwners << endl;
	stream << _T("  dwNumMonitors           = 0x")	<< hex << lpCallInfo->dwNumMonitors << endl;
	stream << _T("  dwCountryCode           = 0x")	<< hex << lpCallInfo->dwCountryCode << endl;
	stream << _T("  dwTrunk                 = 0x")	<< hex << lpCallInfo->dwTrunk << endl;
	stream << _T("  dwCallerIDFlags         = 0x")	<< hex << lpCallInfo->dwCallerIDFlags << _T(" ");
	stream << _DumpBits(lpCallInfo->dwCallerIDFlags, _CIDFLags) << endl;
	stream << _T("  dwCallerIDSize          = 0x")	<< hex << lpCallInfo->dwCallerIDSize << endl;
	stream << _T("  dwCallerIDOffset        = 0x")	<< hex << lpCallInfo->dwCallerIDOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwCallerIDOffset, lpCallInfo->dwCallerIDSize) << endl;
	stream << _T("  dwCallerIDNameSize      = 0x")	<< hex << lpCallInfo->dwCallerIDNameSize << endl;
	stream << _T("  dwCallerIDNameOffset    = 0x")	<< hex << lpCallInfo->dwCallerIDNameOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwCallerIDNameOffset, lpCallInfo->dwCallerIDNameSize) << endl;
	stream << _T("  dwCalledIDFlags         = 0x")	<< hex << lpCallInfo->dwCalledIDFlags << _T(" ");
	stream << _DumpBits(lpCallInfo->dwCalledIDFlags, _CIDFLags) << endl;
	stream << _T("  dwCalledIDSize          = 0x")	<< hex << lpCallInfo->dwCalledIDSize << endl;
	stream << _T("  dwCalledIDOffset        = 0x")	<< hex << lpCallInfo->dwCalledIDOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwCalledIDOffset, lpCallInfo->dwCalledIDSize) << endl;
	stream << _T("  dwCalledIDNameSize      = 0x")	<< hex << lpCallInfo->dwCalledIDNameSize << endl;
	stream << _T("  dwCalledIDNameOffset    = 0x")	<< hex << lpCallInfo->dwCalledIDNameOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwCalledIDNameOffset, lpCallInfo->dwCalledIDNameSize) << endl;
	stream << _T("  dwConnectedIDFlags      = 0x")	<< hex << lpCallInfo->dwConnectedIDFlags << _T(" ");
	stream << _DumpBits(lpCallInfo->dwConnectedIDFlags, _CIDFLags) << endl;
	stream << _T("  dwConnectedIDSize       = 0x")	<< hex << lpCallInfo->dwConnectedIDSize << endl;
	stream << _T("  dwConnectedIDOffset     = 0x")	<< hex << lpCallInfo->dwConnectedIDOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwConnectedIDOffset, lpCallInfo->dwConnectedIDSize) << endl;
	stream << _T("  dwConnectedIDNameSize   = 0x")	<< hex << lpCallInfo->dwConnectedIDNameSize << endl;
	stream << _T("  dwConnectedIDNameOffset = 0x")	<< hex << lpCallInfo->dwConnectedIDNameOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwConnectedIDNameOffset, lpCallInfo->dwConnectedIDNameSize) << endl;
	stream << _T("  dwRedirectionIDFlags    = 0x")	<< hex << lpCallInfo->dwRedirectionIDFlags << _T(" ");
	stream << _DumpBits(lpCallInfo->dwRedirectionIDFlags, _CIDFLags) << endl;
	stream << _T("  dwRedirectionIDSize     = 0x")	<< hex << lpCallInfo->dwRedirectionIDSize << endl;
	stream << _T("  dwRedirectionIDOffset   = 0x")	<< hex << lpCallInfo->dwRedirectionIDOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwRedirectionIDOffset, lpCallInfo->dwRedirectionIDSize) << endl;
	stream << _T("  dwRedirectionIDNameSize = 0x")	<< hex << lpCallInfo->dwRedirectionIDNameSize << endl;
	stream << _T("  dwRedirectionIDNameOffset  = 0x") << hex << lpCallInfo->dwRedirectionIDNameOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwRedirectionIDNameOffset, lpCallInfo->dwRedirectionIDNameSize) << endl;
	stream << _T("  dwRedirectingIDFlags    = 0x")	<< hex << lpCallInfo->dwRedirectingIDFlags << _T(" ");
	stream << _DumpBits(lpCallInfo->dwRedirectingIDFlags, _CIDFLags) << endl;
	stream << _T("  dwRedirectingIDSize     = 0x")	<< hex << lpCallInfo->dwRedirectingIDSize << endl;
	stream << _T("  dwRedirectingIDOffset   = 0x")	<< hex << lpCallInfo->dwRedirectingIDOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwRedirectingIDOffset, lpCallInfo->dwRedirectingIDSize) << endl;
	stream << _T("  dwRedirectingIDNameSize = 0x")	<< hex << lpCallInfo->dwRedirectingIDNameSize << endl;
	stream << _T("  dwRedirectingIDNameOffset  = 0x") << hex << lpCallInfo->dwRedirectingIDNameOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwRedirectingIDNameOffset, lpCallInfo->dwRedirectingIDNameSize) << endl;
	stream << _T("  dwAppNameSize           = 0x")	<< hex << lpCallInfo->dwAppNameSize << endl;
	stream << _T("  dwAppNameOffset         = 0x")	<< hex << lpCallInfo->dwAppNameOffset << endl;
	stream << _T("  dwDisplayableAddressSize   = 0x") << hex << lpCallInfo->dwDisplayableAddressSize << endl;
	stream << _T("  dwDisplayableAddressOffset = 0x") << hex << lpCallInfo->dwDisplayableAddressOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwDisplayableAddressOffset, lpCallInfo->dwDisplayableAddressSize) << endl;
	stream << _T("  dwCalledPartySize       = 0x")	<< hex << lpCallInfo->dwCalledPartySize << endl;
	stream << _T("  dwCalledPartyOffset     = 0x")	<< hex << lpCallInfo->dwCalledPartyOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwCalledPartyOffset, lpCallInfo->dwCalledPartySize) << endl;
	stream << _T("  dwCommentSize           = 0x")	<< hex << lpCallInfo->dwCommentSize << endl;
	stream << _T("  dwCommentOffset         = 0x")	<< hex << lpCallInfo->dwCommentOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwCommentOffset, lpCallInfo->dwCommentSize) << endl;
	stream << _T("  dwDisplaySize           = 0x")	<< hex << lpCallInfo->dwDisplaySize << endl;
	stream << _T("  dwDisplayOffset         = 0x")	<< hex << lpCallInfo->dwDisplayOffset << _T(" ");
	stream << _DumpASCII(lpCallInfo, lpCallInfo->dwDisplayOffset, lpCallInfo->dwDisplaySize) << endl;
	stream << _T("  dwUserUserInfoSize      = 0x")	<< hex << lpCallInfo->dwUserUserInfoSize << endl;
	stream << _T("  dwUserUserInfoOffset    = 0x")	<< hex << lpCallInfo->dwUserUserInfoOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwUserUserInfoOffset, lpCallInfo->dwUserUserInfoSize);
	stream << _T("  dwHighLevelCompSize     = 0x")	<< hex << lpCallInfo->dwHighLevelCompSize << endl;
	stream << _T("  dwHighLevelCompOffset   = 0x")	<< hex << lpCallInfo->dwHighLevelCompOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwHighLevelCompOffset, lpCallInfo->dwHighLevelCompSize);
	stream << _T("  dwLowLevelCompSize      = 0x")	<< hex << lpCallInfo->dwLowLevelCompSize << endl;
	stream << _T("  dwLowLevelCompOffset    = 0x")	<< hex << lpCallInfo->dwLowLevelCompOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwLowLevelCompOffset, lpCallInfo->dwLowLevelCompSize);
	stream << _T("  dwChargingInfoSize      = 0x")	<< hex << lpCallInfo->dwChargingInfoSize << endl;
	stream << _T("  dwChargingInfoOffset    = 0x")	<< hex << lpCallInfo->dwChargingInfoOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwChargingInfoOffset, lpCallInfo->dwChargingInfoSize);
	stream << _T("  dwTerminalModesSize     = 0x")	<< hex << lpCallInfo->dwTerminalModesSize << endl;
	stream << _T("  dwTerminalModesOffset   = 0x")	<< hex << lpCallInfo->dwTerminalModesOffset << endl;

	if (lpCallInfo->dwTerminalModesSize > 0 && 
		lpCallInfo->dwTerminalModesOffset > 0)
	{
		LPDWORD lpTermModes = (LPDWORD)((LPBYTE)lpCallInfo + lpCallInfo->dwTerminalModesOffset);
		for (unsigned int i = 0; i < lpCallInfo->dwTerminalModesSize/sizeof(DWORD); i++)
		{
			stream << _T("  TERMINAL[")				<< i << _T("]") << _T(" = ");
			stream << _DumpBits(*lpTermModes, _TermModes) << endl;
			lpTermModes++;
		}
	}

	stream << _T("  dwDevSpecificSize       = 0x")	<< hex << lpCallInfo->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset     = 0x")	<< hex << lpCallInfo->dwDevSpecificOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwDevSpecificSize, lpCallInfo->dwDevSpecificOffset);

	stream << _T("  dwCallTreatment         = 0x")	<< hex << lpCallInfo->dwCallTreatment << _T(" ");
	stream << _DumpBits(lpCallInfo->dwCallTreatment, _CallTreatments) << endl;
	stream << _T("  dwCallDataSize          = 0x")	<< hex << lpCallInfo->dwCallDataSize << endl;
	stream << _T("  dwCallDataOffset        = 0x")	<< hex << lpCallInfo->dwCallDataOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwCallDataOffset, lpCallInfo->dwCallDataSize);
	stream << _T("  dwSendingFlowspecSize   = 0x")	<< hex << lpCallInfo->dwSendingFlowspecSize << endl;
	stream << _T("  dwSendingFlowspecOffset = 0x")	<< hex << lpCallInfo->dwSendingFlowspecOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwSendingFlowspecOffset, lpCallInfo->dwSendingFlowspecSize);
	stream << _T("  dwReceivingFlowspecSize = 0x")	<< hex << lpCallInfo->dwReceivingFlowspecSize << endl;
	stream << _T("  dwReceivingFlowspecOffset  = 0x") << hex << lpCallInfo->dwReceivingFlowspecOffset << endl;
	stream << _Dump(lpCallInfo, lpCallInfo->dwReceivingFlowspecOffset, lpCallInfo->dwReceivingFlowspecSize);

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEFORWARDLIST buffer
//
inline TString _Dump(LPLINEFORWARDLIST const lpForwardList)
{
	// Build the trace stream
	TStringStream stream;
	stream << _T("LINEFORWARDLIST at 0x") << hex << (DWORD_PTR)lpForwardList << endl;

	// Dump the remainder of the structure
	if (lpForwardList == NULL || IsBadReadPtr(lpForwardList, sizeof(LINEFORWARDLIST)))
	{
		if (lpForwardList != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize  = 0x")	<< hex << lpForwardList->dwTotalSize << endl;
	stream << _T("  dwNumEntries = 0x") << hex << lpForwardList->dwNumEntries << endl;

	if (IsBadReadPtr(lpForwardList, lpForwardList->dwTotalSize) ||
		lpForwardList->dwTotalSize < (lpForwardList->dwNumEntries * sizeof(LINEFORWARD)))
	{
		stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	// Dump the entries
	LINEFORWARD* lpForward = lpForwardList->ForwardList;
	for (unsigned int i = 0; i < lpForwardList->dwNumEntries; i++)
	{
		stream << _T("  LINEFORWARD[")				<< i << _T("]") << endl;
		stream << _T("    dwForwardMode         = 0x")	<< hex << lpForward->dwForwardMode;
		stream << _T(" (") << _DumpBits(lpForward->dwForwardMode, _ForwardModes) << _T(")") << endl;
		stream << _T("    dwCallerAddressSize   = 0x")	<< hex << lpForward->dwCallerAddressSize << endl;
		stream << _T("    dwCallerAddressOffset = 0x")	<< hex << lpForward->dwCallerAddressOffset << _T(" ");
		stream << _DumpASCII(lpForwardList, lpForward->dwCallerAddressOffset, lpForward->dwCallerAddressSize) << endl;
		stream << _T("    dwDestCountryCode     = ")	<< lpForward->dwDestCountryCode << endl;
		stream << _T("    dwDestAddressSize     = ")    << lpForward->dwDestAddressSize << endl;
		stream << _T("    dwDestAddressOffset   = ")    << lpForward->dwDestAddressOffset << _T(" ");
		stream << _DumpASCII(lpForwardList, lpForward->dwDestAddressOffset, lpForward->dwDestAddressSize) << endl;
		lpForward++;
	}
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a PHONEEXTENSION structure
//
inline TString _Dump(LPPHONEEXTENSIONID lpExtensionID)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("PHONEEXTENSIONID at 0x") << hex << (DWORD_PTR) lpExtensionID << endl;

	// Dump the remainder of the structure
	if (lpExtensionID == NULL || IsBadReadPtr(lpExtensionID, sizeof(PHONEEXTENSIONID)))
	{
		if (lpExtensionID != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwExtensionID0          = 0x")	<< hex << lpExtensionID->dwExtensionID0 << endl;
	stream << _T("  dwExtensionID1          = 0x")	<< hex << lpExtensionID->dwExtensionID1 << endl;
	stream << _T("  dwExtensionID2          = 0x")	<< hex << lpExtensionID->dwExtensionID2 << endl;
	stream << _T("  dwExtensionID3          = 0x")	<< hex << lpExtensionID->dwExtensionID3 << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a PHONEBUTTONINFO structure
//
inline TString _Dump(LPPHONEBUTTONINFO lpButtonInfo)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping PHONEBUTTONINFO at 0x") << hex << (DWORD_PTR) lpButtonInfo << endl;

	// Dump the remainder of the structure
	if (lpButtonInfo == NULL || IsBadReadPtr(lpButtonInfo, sizeof(PHONEBUTTONINFO)))
	{
		if (lpButtonInfo != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize         = 0x")	<< hex << lpButtonInfo->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x")	<< hex << lpButtonInfo->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x")	<< hex << lpButtonInfo->dwUsedSize << endl;
	stream << _T("  dwButtonMode        = 0x")	<< hex << lpButtonInfo->dwButtonMode << _T(" ");
	stream << _DumpBits(lpButtonInfo->dwButtonMode, _ButtonModes) << endl;
	stream << _T("  dwButtonFunction    = 0x")	<< hex << lpButtonInfo->dwButtonFunction << _T(" ");
	stream << _DumpExact(lpButtonInfo->dwButtonFunction, _ButtonFunctions) << endl;
	stream << _T("  dwButtonTextSize    = 0x")	<< hex << lpButtonInfo->dwButtonTextSize << endl;
	stream << _T("  dwButtonTextOffset  = 0x")	<< hex << lpButtonInfo->dwButtonTextOffset << _T(" ");
	stream << _DumpASCII(lpButtonInfo, lpButtonInfo->dwButtonTextOffset, lpButtonInfo->dwButtonTextSize) << endl;
	stream << _T("  dwDevSpecificSize   = 0x")	<< hex << lpButtonInfo->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset = 0x")	<< hex << lpButtonInfo->dwDevSpecificOffset << endl;
	stream << _Dump(lpButtonInfo, lpButtonInfo->dwDevSpecificOffset, lpButtonInfo->dwDevSpecificSize);

	return stream.str();
}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a PHONECAPS structure
//
inline TString _Dump(LPPHONECAPS lpPhoneCaps)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping PHONECAPS at 0x") << hex << (DWORD_PTR) lpPhoneCaps << endl;

	// Dump the remainder of the structure
	if (lpPhoneCaps == NULL || IsBadReadPtr(lpPhoneCaps, sizeof(PHONECAPS)))
	{
		if (lpPhoneCaps != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize         = 0x")	<< hex << lpPhoneCaps->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x")	<< hex << lpPhoneCaps->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x")	<< hex << lpPhoneCaps->dwUsedSize << endl;
	stream << _T("  dwProviderInfoSize  = 0x")	<< hex << lpPhoneCaps->dwProviderInfoSize << endl;
	stream << _T("  dwProviderInfoOffset= 0x")	<< hex << lpPhoneCaps->dwProviderInfoOffset << _T(" ");
	stream << _DumpASCII(lpPhoneCaps, lpPhoneCaps->dwProviderInfoOffset, lpPhoneCaps->dwProviderInfoSize) << endl;
	stream << _T("  dwPhoneInfoSize     = 0x")	<< hex << lpPhoneCaps->dwPhoneInfoSize << endl;
	stream << _T("  dwPhoneInfoOffset   = 0x")	<< hex << lpPhoneCaps->dwPhoneInfoOffset << _T(" ");
	stream << _DumpASCII(lpPhoneCaps, lpPhoneCaps->dwPhoneInfoOffset, lpPhoneCaps->dwPhoneInfoSize) << endl;
	stream << _T("  dwPermanentPhoneID  = 0x")	<< hex << lpPhoneCaps->dwPermanentPhoneID << endl;
	stream << _T("  dwPhoneNameSize     = 0x")	<< hex << lpPhoneCaps->dwPhoneNameSize << endl;
	stream << _T("  dwPhoneNameOffset   = 0x")	<< hex << lpPhoneCaps->dwPhoneNameOffset << _T(" ");
	stream << _DumpASCII(lpPhoneCaps, lpPhoneCaps->dwPhoneNameOffset, lpPhoneCaps->dwPhoneNameSize) << endl;
	stream << _T("  dwStringFormat      = 0x")	<< hex << lpPhoneCaps->dwStringFormat << _T(" ");
	stream << _DumpExact(lpPhoneCaps->dwStringFormat, _StringFormats) << endl;
	stream << _T("  dwPhoneStates       = 0x")	<< hex << lpPhoneCaps->dwPhoneStates << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwPhoneStates, _PhoneStates) << endl;
	stream << _T("  dwHookSwitchDevs    = 0x")	<< hex << lpPhoneCaps->dwHookSwitchDevs << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwHookSwitchDevs, _HookswitchDevs) << endl;
	stream << _T("  dwHandsetHookSwitchModes = 0x")	<< hex << lpPhoneCaps->dwHandsetHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwHandsetHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwSpeakerHookSwitchModes = 0x")	<< hex << lpPhoneCaps->dwSpeakerHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwSpeakerHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwHeadsetHookSwitchModes = 0x")	<< hex << lpPhoneCaps->dwHeadsetHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwHeadsetHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwVolumeFlags       = 0x")	<< hex << lpPhoneCaps->dwVolumeFlags << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwVolumeFlags, _HookswitchDevs) << endl;
	stream << _T("  dwGainFlags         = 0x")	<< hex << lpPhoneCaps->dwGainFlags << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwGainFlags, _HookswitchDevs) << endl;
	stream << _T("  dwDisplayNumRows    = 0x")	<< hex << lpPhoneCaps->dwDisplayNumRows << endl;
	stream << _T("  dwDisplayNumColumns = 0x")	<< hex << lpPhoneCaps->dwDisplayNumColumns << endl;
	stream << _T("  dwNumRingModes      = 0x")	<< hex << lpPhoneCaps->dwNumRingModes << endl;
	stream << _T("  dwNumButtonLamps    = 0x")	<< hex << lpPhoneCaps->dwNumButtonLamps << endl;
	stream << _T("  dwButtonModesSize   = 0x")	<< hex << lpPhoneCaps->dwButtonModesSize << endl;
	stream << _T("  dwButtonModesOffset = 0x")	<< hex << lpPhoneCaps->dwButtonModesOffset << endl;

	if (lpPhoneCaps->dwButtonModesSize > 0 &&
		lpPhoneCaps->dwButtonModesOffset > 0)
	{
		LPDWORD lpMode = (LPDWORD)((LPBYTE)lpPhoneCaps + lpPhoneCaps->dwButtonModesOffset);
		for (unsigned int i = 0; i < lpPhoneCaps->dwButtonModesSize/sizeof(DWORD); i++)
		{
			stream << _T("    BUTTONMODE[") << dec << i << _T("] = ");
			stream << _DumpBits(*lpMode, _ButtonModes) << endl;
			lpMode++;
		}
	}

	stream << _T("  dwButtonFunctionsSize   = 0x")	<< hex << lpPhoneCaps->dwButtonFunctionsSize << endl;
	stream << _T("  dwButtonFunctionsOffset = 0x")	<< hex << lpPhoneCaps->dwButtonFunctionsOffset << endl;

	if (lpPhoneCaps->dwButtonFunctionsSize > 0 &&
		lpPhoneCaps->dwButtonFunctionsOffset > 0)
	{
		LPDWORD lpMode = (LPDWORD)((LPBYTE)lpPhoneCaps + lpPhoneCaps->dwButtonFunctionsOffset);
		for (unsigned int i = 0; i < lpPhoneCaps->dwButtonFunctionsSize/sizeof(DWORD); i++)
		{
			stream << _T("    BUTTONFUNCTION[") << i << _T("] = ");
			stream << _DumpExact(*lpMode, _ButtonFunctions) << endl;
			lpMode++;
		}
	}

	stream << _T("  dwLampModesSize     = 0x")	<< hex << lpPhoneCaps->dwLampModesSize << endl;
	stream << _T("  dwLampModesOffset   = 0x")	<< hex << lpPhoneCaps->dwLampModesOffset << endl;

	if (lpPhoneCaps->dwLampModesSize > 0 &&
		lpPhoneCaps->dwLampModesOffset > 0)
	{
		LPDWORD lpMode = (LPDWORD)((LPBYTE)lpPhoneCaps + lpPhoneCaps->dwLampModesOffset);
		for (unsigned int i = 0; i < lpPhoneCaps->dwLampModesSize/sizeof(DWORD); i++)
		{
			stream << _T("    LAMPMODE[") << i << _T("] = ");
			stream << _DumpBits(*lpMode, _LampModes) << endl;
			lpMode++;
		}
	}

	stream << _T("  dwNumSetData        = 0x")	<< hex << lpPhoneCaps->dwNumSetData << endl;
	stream << _T("  dwSetDataSize       = 0x")	<< hex << lpPhoneCaps->dwSetDataSize << endl;
	stream << _T("  dwSetDataOffset     = 0x")	<< hex << lpPhoneCaps->dwSetDataOffset << endl;
	stream << _Dump(lpPhoneCaps, lpPhoneCaps->dwSetDataOffset, lpPhoneCaps->dwSetDataSize);
	stream << _T("  dwNumGetData        = 0x")	<< hex << lpPhoneCaps->dwNumGetData << endl;
	stream << _T("  dwGetDataSize       = 0x")	<< hex << lpPhoneCaps->dwGetDataSize << endl;
	stream << _T("  dwGetDataOffset     = 0x")	<< hex << lpPhoneCaps->dwGetDataOffset << endl;
	stream << _Dump(lpPhoneCaps, lpPhoneCaps->dwGetDataOffset, lpPhoneCaps->dwGetDataSize);
	stream << _T("  dwDevSpecificSize   = 0x")	<< hex << lpPhoneCaps->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset = 0x")	<< hex << lpPhoneCaps->dwDevSpecificOffset << endl;
	stream << _Dump(lpPhoneCaps, lpPhoneCaps->dwDevSpecificOffset, lpPhoneCaps->dwDevSpecificSize);
	stream << _T("  dwDeviceClassesSize = 0x")	<< hex << lpPhoneCaps->dwDeviceClassesSize << endl;
	stream << _T("  dwDeviceClassesOffset = 0x")	<< hex << lpPhoneCaps->dwDeviceClassesOffset << endl;
	stream << _Dump(lpPhoneCaps, lpPhoneCaps->dwDeviceClassesOffset, lpPhoneCaps->dwDeviceClassesSize);
	stream << _T("  dwPhoneFeatures     = 0x")	<< hex << lpPhoneCaps->dwPhoneFeatures << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwPhoneFeatures, _PhoneFeatures) << endl;
	stream << _T("  dwSettableHandsetHookSwitchModes  = 0x") << hex << lpPhoneCaps->dwSettableHandsetHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwSettableHandsetHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwSettableSpeakerHookSwitchModes  = 0x") << hex << lpPhoneCaps->dwSettableSpeakerHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwSettableSpeakerHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwSettableHeadsetHookSwitchModes  = 0x") << hex << lpPhoneCaps->dwSettableHeadsetHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwSettableHeadsetHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwMonitoredHandsetHookSwitchModes = 0x") << hex << lpPhoneCaps->dwMonitoredHandsetHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwMonitoredHandsetHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwMonitoredSpeakerHookSwitchModes = 0x") << hex << lpPhoneCaps->dwMonitoredSpeakerHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwMonitoredSpeakerHookSwitchModes, _HookswitchModes) << endl;
	stream << _T("  dwMonitoredHeadsetHookSwitchModes = 0x") << hex << lpPhoneCaps->dwMonitoredHeadsetHookSwitchModes << _T(" ");
	stream << _DumpBits(lpPhoneCaps->dwMonitoredHeadsetHookSwitchModes, _HookswitchModes) << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a PHONESTATUS structure
//
inline TString _Dump(LPPHONESTATUS lpPhoneStatus)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping PHONESTATUS at 0x") << hex << (DWORD_PTR) lpPhoneStatus << endl;

	// Dump the remainder of the structure
	if (lpPhoneStatus == NULL || IsBadReadPtr(lpPhoneStatus, sizeof(PHONESTATUS)))
	{
		if (lpPhoneStatus != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize         = 0x")	<< hex <<  lpPhoneStatus->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x")	<< hex <<  lpPhoneStatus->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x")	<< hex <<  lpPhoneStatus->dwUsedSize << endl;
	stream << _T("  dwStatusFlags       = 0x")	<< hex <<  lpPhoneStatus->dwStatusFlags << _T(" ");
	stream << _DumpBits(lpPhoneStatus->dwStatusFlags, _PhoneStatusFlags) << endl;
	stream << _T("  dwNumOwners         = 0x")	<< hex <<  lpPhoneStatus->dwNumOwners << endl;
	stream << _T("  dwNumMonitors       = 0x")	<< hex <<  lpPhoneStatus->dwNumMonitors << endl;
	stream << _T("  dwRingMode          = 0x")	<< hex <<  lpPhoneStatus->dwRingMode << endl;
	stream << _T("  dwRingVolume        = 0x")	<< hex <<  lpPhoneStatus->dwRingVolume << endl;
	stream << _T("  dwHandsetHookSwitchMode = 0x")	<< hex << lpPhoneStatus->dwHandsetHookSwitchMode << _T(" ");
	stream << _DumpBits(lpPhoneStatus->dwHandsetHookSwitchMode, _HookswitchModes) << endl;
	stream << _T("  dwHandsetVolume     = 0x")	<< hex << lpPhoneStatus->dwHandsetVolume << endl;
	stream << _T("  dwHandsetGain       = 0x")	<< hex << lpPhoneStatus->dwHandsetGain << endl;
	stream << _T("  dwSpeakerHookSwitchMode = 0x")	<< hex << lpPhoneStatus->dwSpeakerHookSwitchMode << _T(" ");
	stream << _DumpBits(lpPhoneStatus->dwSpeakerHookSwitchMode, _HookswitchModes) << endl;
	stream << _T("  dwSpeakerVolume     = 0x")	<< hex << lpPhoneStatus->dwSpeakerVolume << endl;
	stream << _T("  dwSpeakerGain       = 0x")	<< hex << lpPhoneStatus->dwSpeakerGain << endl;
	stream << _T("  dwHeadsetHookSwitchMode = 0x")	<< hex << lpPhoneStatus->dwHeadsetHookSwitchMode << _T(" ");
	stream << _DumpBits(lpPhoneStatus->dwHeadsetHookSwitchMode, _HookswitchModes) << endl;
	stream << _T("  dwHeadsetVolume     = 0x")	<< hex << lpPhoneStatus->dwHeadsetVolume << endl;
	stream << _T("  dwHeadsetGain       = 0x")	<< hex << lpPhoneStatus->dwHeadsetGain << endl;
	stream << _T("  dwDisplaySize       = 0x")	<< hex << lpPhoneStatus->dwDisplaySize << endl;
	stream << _T("  dwDisplayOffset     = 0x")	<< hex << lpPhoneStatus->dwDisplayOffset << _T(" ");
	stream << _Dump((LPCSTR)((LPBYTE)lpPhoneStatus+lpPhoneStatus->dwDisplayOffset), lpPhoneStatus->dwDisplaySize) << endl;
	stream << _T("  dwLampModesSize     = 0x")	<< hex << lpPhoneStatus->dwLampModesSize << endl;
	stream << _T("  dwLampModesOffset   = 0x")	<< hex << lpPhoneStatus->dwLampModesOffset << endl;

	if (lpPhoneStatus->dwLampModesSize > 0 &&
		lpPhoneStatus->dwLampModesOffset > 0)
	{
		LPDWORD lpMode = (LPDWORD)((LPBYTE)lpPhoneStatus + lpPhoneStatus->dwLampModesOffset);
		for (unsigned int i = 0; i < lpPhoneStatus->dwLampModesSize/sizeof(DWORD); i++)
		{
			stream << _T("    LAMPMODE[") << i << _T("] = ");
			stream << _DumpBits(*lpMode, _LampModes) << endl;
			lpMode++;
		}
	}

	stream << _T("  dwOwnerNameSize     = 0x")	<< hex << lpPhoneStatus->dwOwnerNameSize << endl;
	stream << _T("  dwOwnerNameOffset   = 0x")	<< hex << lpPhoneStatus->dwOwnerNameOffset << _T(" ");
	stream << _DumpASCII(lpPhoneStatus, lpPhoneStatus->dwOwnerNameOffset, lpPhoneStatus->dwOwnerNameSize) << endl;
	stream << _T("  dwDevSpecificSize   = 0x")	<< hex << lpPhoneStatus->dwDevSpecificSize << endl;
	stream << _T("  dwDevSpecificOffset = 0x")	<< hex << lpPhoneStatus->dwDevSpecificOffset << endl;
	stream << _Dump(lpPhoneStatus, lpPhoneStatus->dwDevSpecificOffset, lpPhoneStatus->dwDevSpecificSize);
	stream << _T("  dwPhoneFeatures     = 0x")	<< hex << lpPhoneStatus->dwPhoneFeatures << _T(" ");
	stream << _DumpBits(lpPhoneStatus->dwPhoneFeatures, _PhoneFeatures) << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINAGENTSTATUS structure
//
inline TString _Dump(LPLINEAGENTSTATUS lpAgentStatus)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTSTATUS at 0x") << hex << (DWORD_PTR) lpAgentStatus << endl;

	// Dump the remainder of the structure
	if (lpAgentStatus == NULL || IsBadReadPtr(lpAgentStatus, sizeof(LINEAGENTSTATUS)))
	{
		if (lpAgentStatus != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize         = 0x")	<< hex << lpAgentStatus->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x")	<< hex << lpAgentStatus->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x")	<< hex << lpAgentStatus->dwUsedSize << endl;
	stream << _T("  dwNumEntries        = 0x")	<< hex << lpAgentStatus->dwNumEntries << endl;
	stream << _T("  dwGroupListSize     = 0x")	<< hex << lpAgentStatus->dwGroupListSize << endl;
	stream << _T("  dwGroupListOffset   = 0x")	<< hex << lpAgentStatus->dwGroupListOffset << endl;

	if (lpAgentStatus->dwGroupListSize > 0 &&
		lpAgentStatus->dwGroupListOffset > 0 &&
		lpAgentStatus->dwNumEntries > 0)
	{
		LPLINEAGENTGROUPENTRY lpEntry = (LPLINEAGENTGROUPENTRY)((LPBYTE)lpAgentStatus + lpAgentStatus->dwGroupListOffset);
		for (unsigned int i = 0; i < lpAgentStatus->dwNumEntries; i++)
		{
			stream << _T("  LINEGROUPENTRY[") << i << _T("]") << endl;
			stream << _T("       GroupID.dwGroupID1 = 0x") << hex << lpEntry->GroupID.dwGroupID1 << endl;
			stream << _T("       GroupID.dwGroupID2 = 0x") << hex << lpEntry->GroupID.dwGroupID2 << endl;
			stream << _T("       GroupID.dwGroupID3 = 0x") << hex << lpEntry->GroupID.dwGroupID3 << endl;
			stream << _T("       GroupID.dwGroupID4 = 0x") << hex << lpEntry->GroupID.dwGroupID4 << endl;
			stream << _T("		 dwNameSize         = 0x") << hex << lpEntry->dwNameSize << endl;
			stream << _T("		 dwNameOffset       = 0x") << hex << lpEntry->dwNameOffset << _T(" ");
			stream << _DumpASCII(lpAgentStatus, lpEntry->dwNameOffset, lpEntry->dwNameSize) << endl;
			lpEntry++;
		}
	}

	stream << _T("  dwState             = 0x")	<< hex << lpAgentStatus->dwState << _T(" ");
	stream << _DumpBits(lpAgentStatus->dwState, _AgentStates) << endl;
	stream << _T("  dwNextState         = 0x")	<< hex << lpAgentStatus->dwNextState << _T(" ");
	stream << _DumpBits(lpAgentStatus->dwNextState, _AgentStates) << endl;
	stream << _T("  dwActivityID        = 0x")	<< hex << lpAgentStatus->dwActivityID << endl;
	stream << _T("  dwActivitySize      = 0x")	<< hex << lpAgentStatus->dwActivitySize << endl;
	stream << _T("  dwActivityOffset    = 0x")	<< hex << lpAgentStatus->dwActivityOffset << _T(" ");
	stream << _DumpASCII(lpAgentStatus, lpAgentStatus->dwActivityOffset, lpAgentStatus->dwActivitySize) << endl;
	stream << _T("  dwAgentFeatures     = 0x")	<< hex << lpAgentStatus->dwAgentFeatures << _T(" ");
	stream << _DumpBits(lpAgentStatus->dwAgentFeatures, _AgentFeatures) << endl;
	stream << _T("  dwValidStates       = 0x")	<< hex << lpAgentStatus->dwValidStates << _T(" ");
	stream << _DumpBits(lpAgentStatus->dwValidStates, _AgentStates) << endl;
	stream << _T("  dwValidNextStates   = 0x")	<< hex << lpAgentStatus->dwValidNextStates << _T(" ");
	stream << _DumpBits(lpAgentStatus->dwValidNextStates, _AgentStates) << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINAGENTCAPS structure
//
inline TString _Dump(LPLINEAGENTCAPS lpAgentCaps)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTCAPS at 0x") << hex << (DWORD_PTR) lpAgentCaps << endl;

	// Dump the remainder of the structure
	if (lpAgentCaps == NULL || IsBadReadPtr(lpAgentCaps, sizeof(LINEAGENTCAPS)))
	{
		if (lpAgentCaps != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize         = 0x")	<< hex << lpAgentCaps->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x")	<< hex << lpAgentCaps->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x")	<< hex << lpAgentCaps->dwUsedSize << endl;
	stream << _T("  dwAgentHandlerInfoSize   = 0x")	<< hex << lpAgentCaps->dwAgentHandlerInfoSize << endl;
	stream << _T("  dwAgentHandlerInfoOffset = 0x")	<< hex << lpAgentCaps->dwAgentHandlerInfoOffset << _T(" ");
	stream << _DumpASCII(lpAgentCaps, lpAgentCaps->dwAgentHandlerInfoOffset, lpAgentCaps->dwAgentHandlerInfoSize) << endl;
	stream << _T("  dwCapsVersion       = 0x")	<< hex << lpAgentCaps->dwCapsVersion << endl;
	stream << _T("  dwFeatures          = 0x")	<< hex << lpAgentCaps->dwFeatures << _T(" ");
	stream << _DumpBits(lpAgentCaps->dwFeatures, _AgentFeatures) << endl;
	stream << _T("  dwStates            = 0x")	<< hex << lpAgentCaps->dwStates << _T(" ");
	stream << _DumpBits(lpAgentCaps->dwStates, _AgentStates) << endl;
	stream << _T("  dwNextStates        = 0x")	<< hex << lpAgentCaps->dwNextStates << _T(" ");
	stream << _DumpBits(lpAgentCaps->dwNextStates, _AgentStates) << endl;
	stream << _T("  dwMaxNumGroupEntries   = 0x")	<< hex << lpAgentCaps->dwMaxNumGroupEntries << endl;
	stream << _T("  dwAgentStatusMessages  = 0x")	<< hex << lpAgentCaps->dwAgentStatusMessages << _T(" ");
	stream << _DumpBits(lpAgentCaps->dwAgentStatusMessages, _AgentStatusFlags) << endl;
	stream << _T("  dwNumAgentExtensionIDs = 0x")	<< hex << lpAgentCaps->dwNumAgentExtensionIDs << endl;
	stream << _T("  dwAgentExtensionIDListSize   = 0x")	<< hex << lpAgentCaps->dwAgentExtensionIDListSize << endl;
	stream << _T("  dwAgentExtensionIDListOffset = 0x")	<< hex << lpAgentCaps->dwAgentExtensionIDListOffset << endl;
	stream << _Dump(lpAgentCaps, lpAgentCaps->dwAgentExtensionIDListOffset, lpAgentCaps->dwAgentExtensionIDListSize);
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEAGENTACTIVITYLIST
//
inline TString _Dump(LPLINEAGENTACTIVITYLIST lpActivityList)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTACTIVITYLIST at 0x") << hex << (DWORD_PTR) lpActivityList << endl;

	// Dump the remainder of the structure
	if (lpActivityList == NULL || IsBadReadPtr(lpActivityList, sizeof(LINEAGENTACTIVITYLIST)))
	{
		if (lpActivityList != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize         = 0x") << hex << lpActivityList->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x") << hex << lpActivityList->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x") << hex << lpActivityList->dwUsedSize << endl;
	stream << _T("  dwNumEntries        = 0x") << hex << lpActivityList->dwNumEntries << endl;
	stream << _T("  dwListSize          = 0x") << hex << lpActivityList->dwListSize << endl;
	stream << _T("  dwListOffset        = 0x") << hex << lpActivityList->dwListOffset << endl;

	if (lpActivityList->dwListOffset > 0 &&
		lpActivityList->dwListSize > 0 &&
		lpActivityList->dwNumEntries)
	{
		LPLINEAGENTACTIVITYENTRY lpEntry = (LPLINEAGENTACTIVITYENTRY)((LPBYTE)lpActivityList + lpActivityList->dwListOffset);
		for (unsigned int i = 0; i < lpActivityList->dwNumEntries; i++)
		{
			stream << _T("  LINEACTIVITYENTRY[") << i << _T("]") << endl;
			stream << _T("       dwID         = 0x") << hex << lpEntry->dwID << endl;
			stream << _T("       dwNameSize   = 0x") << hex << lpEntry->dwNameSize << endl;
			stream << _T("       dwNameOffset = 0x") << hex << lpEntry->dwNameOffset << _T(" ");
			stream << _DumpASCII(lpActivityList, lpEntry->dwNameOffset, lpEntry->dwNameSize) << endl;
			lpEntry++;
		}
	}
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEAGENTGROUPLIST
//
inline TString _Dump(LPLINEAGENTGROUPLIST lpGroupList)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTGROUPLIST at 0x") << hex << (DWORD_PTR) lpGroupList << endl;

	// Dump the remainder of the structure
	if (lpGroupList == NULL || IsBadReadPtr(lpGroupList, sizeof(LINEAGENTGROUPLIST)))
	{
		if (lpGroupList != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}
	
	stream << _T("  dwTotalSize         = 0x")	<< hex << lpGroupList->dwTotalSize << endl;
	stream << _T("  dwNeededSize        = 0x")	<< hex << lpGroupList->dwNeededSize << endl;
	stream << _T("  dwUsedSize          = 0x")	<< hex << lpGroupList->dwUsedSize << endl;
	stream << _T("  dwNumEntries        = 0x")	<< hex << lpGroupList->dwNumEntries << endl;
	stream << _T("  dwListSize          = 0x")	<< hex << lpGroupList->dwListSize << endl;
	stream << _T("  dwListOffset        = 0x")	<< hex << lpGroupList->dwListOffset << endl;

	if (lpGroupList->dwListSize > 0 &&
		lpGroupList->dwListOffset > 0 &&
		lpGroupList->dwNumEntries > 0)
	{
		LPLINEAGENTGROUPENTRY lpEntry = (LPLINEAGENTGROUPENTRY)((LPBYTE)lpGroupList + lpGroupList->dwListOffset);
		for (unsigned int i = 0; i < lpGroupList->dwNumEntries; i++)
		{
			stream << _T("  LINEGROUPENTRY[") << i << _T("]") << endl;
			stream << _T("       GroupID.dwGroupID1 = 0x") << hex << lpEntry->GroupID.dwGroupID1 << endl;
			stream << _T("       GroupID.dwGroupID2 = 0x") << hex << lpEntry->GroupID.dwGroupID2 << endl;
			stream << _T("       GroupID.dwGroupID3 = 0x") << hex << lpEntry->GroupID.dwGroupID3 << endl;
			stream << _T("       GroupID.dwGroupID4 = 0x") << hex << lpEntry->GroupID.dwGroupID4 << endl;
			stream << _T("		 dwNameSize         = 0x") << hex << lpEntry->dwNameSize << endl;
			stream << _T("		 dwNameOffset       = 0x") << hex << lpEntry->dwNameOffset << _T(" ");
			stream << _DumpASCII(lpGroupList, lpEntry->dwNameOffset, lpEntry->dwNameSize) << endl;
			lpEntry++;
		}
	}
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINMONITORTONE structure
//
inline TString _Dump(LPLINEMONITORTONE lpTone)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEMONITORTONE at 0x") << hex << (DWORD_PTR) lpTone << endl;

	// Dump the remainder of the structure
	if (lpTone == NULL || IsBadReadPtr(lpTone, sizeof(LINEMONITORTONE)))
	{
		if (lpTone != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwAppSpecific       = 0x")  << hex << lpTone->dwAppSpecific << endl;
	stream << _T("  dwDuration          = 0x")  << hex << lpTone->dwDuration << endl;
    stream << _T("  dwFrequency1        = 0x")  << hex << lpTone->dwFrequency1 << endl;
    stream << _T("  dwFrequency2        = 0x")  << hex << lpTone->dwFrequency2 << endl;
    stream << _T("  dwFrequency3        = 0x")  << hex << lpTone->dwFrequency3 << endl;
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEAGENTINFO structure
//
inline TString _Dump(LPLINEAGENTINFO lpAgentInfo)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTINFO at 0x") << hex << (DWORD_PTR) lpAgentInfo << endl;

	// Dump the remainder of the structure
	if (lpAgentInfo == NULL || IsBadReadPtr(lpAgentInfo, sizeof(LINEAGENTINFO)))
	{
		if (lpAgentInfo != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize            = 0x")  << hex << lpAgentInfo->dwTotalSize << endl;
	stream << _T("  dwNeededSize           = 0x")  << hex << lpAgentInfo->dwNeededSize << endl;
	stream << _T("  dwUsedSize             = 0x")  << hex << lpAgentInfo->dwUsedSize << endl;
	stream << _T("  dwAgentState           = 0x")  << hex << lpAgentInfo->dwAgentState << endl;
	stream << _T("  dwNextAgentState       = 0x")  << hex << lpAgentInfo->dwNextAgentState << endl;
	stream << _T("  dwMeasurementPeriod    = 0x")  << hex << lpAgentInfo->dwMeasurementPeriod << endl;
	stream << _T("  cyOverallCallRate      = 0x")  << hex << lpAgentInfo->cyOverallCallRate.int64 << endl;
	stream << _T("  dwNumberOfACDCalls     = 0x")  << hex << lpAgentInfo->dwNumberOfACDCalls << endl;
	stream << _T("  dwNumberOfIncomingCalls = 0x")  << hex << lpAgentInfo->dwNumberOfIncomingCalls << endl;
	stream << _T("  dwNumberOfOutgoingCalls = 0x")  << hex << lpAgentInfo->dwNumberOfOutgoingCalls << endl;
	stream << _T("  dwTotalACDTalkTime     = 0x")  << hex << lpAgentInfo->dwTotalACDTalkTime << endl;
	stream << _T("  dwTotalACDCallTime     = 0x")  << hex << lpAgentInfo->dwTotalACDCallTime << endl;
	stream << _T("  dwTotalACDWrapUpTime   = 0x")  << hex << lpAgentInfo->dwTotalACDWrapUpTime << endl;

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEAGENTSESSIONLIST structure
//
inline TString _Dump(LPLINEAGENTSESSIONLIST lpAgentSessionList)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTSESSIONLIST at 0x") << hex << (DWORD_PTR) lpAgentSessionList << endl;

	// Dump the remainder of the structure
	if (lpAgentSessionList == NULL || IsBadReadPtr(lpAgentSessionList, sizeof(LINEAGENTSESSIONLIST)))
	{
		if (lpAgentSessionList != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize            = 0x")  << hex << lpAgentSessionList->dwTotalSize << endl;
	stream << _T("  dwNeededSize           = 0x")  << hex << lpAgentSessionList->dwNeededSize << endl;
	stream << _T("  dwUsedSize             = 0x")  << hex << lpAgentSessionList->dwUsedSize << endl;
	stream << _T("  dwNumEntries           = 0x")  << hex << lpAgentSessionList->dwNumEntries << endl;
	stream << _T("  dwListSize             = 0x")  << hex << lpAgentSessionList->dwListSize << endl;
	stream << _T("  dwListOffset           = 0x")  << hex << lpAgentSessionList->dwListOffset << endl;

	if (lpAgentSessionList->dwListSize > 0 &&
		lpAgentSessionList->dwListOffset > 0 &&
		lpAgentSessionList->dwNumEntries > 0)
	{
		LPLINEAGENTSESSIONENTRY lpEntry = (LPLINEAGENTSESSIONENTRY)((LPBYTE)lpAgentSessionList + lpAgentSessionList->dwListOffset);
		for (unsigned int i = 0; i < lpAgentSessionList->dwNumEntries; i++)
		{
			stream << _T("  LINEAGENTSESSIONENTRY[") << i << _T("]") << endl;
			stream << _T("		 hAgentSession      = 0x") << hex << (DWORD_PTR) lpEntry->hAgentSession << endl;
			stream << _T("		 hAgent             = 0x") << hex << (DWORD_PTR) lpEntry->hAgent << endl;
			stream << _T("       GroupID            = ") << _DumpGUID(lpEntry->GroupID) << endl;
			stream << _T("		 dwWorkingAddress   = 0x") << hex << lpEntry->dwWorkingAddressID << endl;
			lpEntry++;
		}
	}
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEAGENTSESSIONINFO structure
//
inline TString _Dump(LPLINEAGENTSESSIONINFO lpAgentSessionInfo)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEAGENTSESSIONINFO at 0x") << hex << (DWORD_PTR) lpAgentSessionInfo << endl;

	// Dump the remainder of the structure
	if (lpAgentSessionInfo == NULL || IsBadReadPtr(lpAgentSessionInfo, sizeof(LINEAGENTSESSIONINFO)))
	{
		if (lpAgentSessionInfo != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	SYSTEMTIME sysTime;
	VariantTimeToSystemTime(lpAgentSessionInfo->dateSessionStartTime, &sysTime);

	stream << _T("  dwTotalSize              = 0x")  << hex << lpAgentSessionInfo->dwTotalSize << endl;
	stream << _T("  dwNeededSize             = 0x")  << hex << lpAgentSessionInfo->dwNeededSize << endl;
	stream << _T("  dwUsedSize               = 0x")  << hex << lpAgentSessionInfo->dwUsedSize << endl;
	stream << _T("  dwAgentSessionState      = 0x")  << hex << lpAgentSessionInfo->dwAgentSessionState << endl;
	stream << _T("  dwNextAgentSessionState  = 0x")  << hex << lpAgentSessionInfo->dwNextAgentSessionState << endl;
	stream << _T("  dateSessionStartTime     = ")	 << _Dump(sysTime) << endl;
	stream << _T("  dwSessionDuration        = 0x")  << hex << lpAgentSessionInfo->dwSessionDuration << endl;
	stream << _T("  dwNumberOfCalls          = 0x")  << hex << lpAgentSessionInfo->dwNumberOfCalls << endl;
	stream << _T("  dwTotalTalkTime          = 0x")  << hex << lpAgentSessionInfo->dwTotalTalkTime << endl;
	stream << _T("  dwAverageTalkTime        = 0x")  << hex << lpAgentSessionInfo->dwAverageTalkTime << endl;
	stream << _T("  dwTotalCallTime          = 0x")  << hex << lpAgentSessionInfo->dwTotalCallTime << endl;
	stream << _T("  dwAverageCallTime        = 0x")  << hex << lpAgentSessionInfo->dwAverageCallTime << endl;
	stream << _T("  dwTotalWrapUpTime        = 0x")  << hex << lpAgentSessionInfo->dwTotalWrapUpTime << endl;
	stream << _T("  dwAverageWrapUpTime      = 0x")  << hex << lpAgentSessionInfo->dwAverageWrapUpTime << endl;
	stream << _T("  cyACDCallRate            = 0x")  << hex << lpAgentSessionInfo->cyACDCallRate.int64 << endl;
	stream << _T("  dwLongestTimeToAnswer    = 0x")  << hex << lpAgentSessionInfo->dwLongestTimeToAnswer << endl;
	stream << _T("  dwAverageTimeToAnswer    = 0x")  << hex << lpAgentSessionInfo->dwAverageTimeToAnswer << endl;

	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEQUEUELIST structure
//
inline TString _Dump(LPLINEQUEUELIST lpQueueList)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEQUEUELIST at 0x") << hex << (DWORD_PTR) lpQueueList << endl;

	// Dump the remainder of the structure
	if (lpQueueList == NULL || IsBadReadPtr(lpQueueList, sizeof(LINEQUEUELIST)))
	{
		if (lpQueueList != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize            = 0x")  << hex << lpQueueList->dwTotalSize << endl;
	stream << _T("  dwNeededSize           = 0x")  << hex << lpQueueList->dwNeededSize << endl;
	stream << _T("  dwUsedSize             = 0x")  << hex << lpQueueList->dwUsedSize << endl;
	stream << _T("  dwNumEntries           = 0x")  << hex << lpQueueList->dwNumEntries << endl;
	stream << _T("  dwListSize             = 0x")  << hex << lpQueueList->dwListSize << endl;
	stream << _T("  dwListOffset           = 0x")  << hex << lpQueueList->dwListOffset << endl;

	if (lpQueueList->dwListSize > 0 &&
		lpQueueList->dwListOffset > 0 &&
		lpQueueList->dwNumEntries > 0)
	{
		LPLINEQUEUEENTRY lpEntry = (LPLINEQUEUEENTRY)((LPBYTE)lpQueueList + lpQueueList->dwListOffset);
		for (unsigned int i = 0; i < lpQueueList->dwNumEntries; i++)
		{
			stream << _T("  LINEQUEUENTRY[") << i << _T("]") << endl;
			stream << _T("		 dwQueueID    = 0x") << hex << lpEntry->dwQueueID << endl;
			stream << _T("		 dwNameSize   = 0x") << hex << lpEntry->dwNameSize << endl;
			stream << _T("		 dwNameOffset = 0x") << hex << lpEntry->dwNameOffset << _T(" ");
			stream << _DumpASCII(lpQueueList, lpEntry->dwNameOffset, lpEntry->dwNameSize) << endl;
			lpEntry++;
		}
	}
	return stream.str();

}// _Dump

///////////////////////////////////////////////////////////////////////////
// _Dump
//
// Dumps out a LINEQUEUEINFO structure
//
inline TString _Dump(LPLINEQUEUEINFO lpQueueInfo)
{
	// Build the trace stream
	TStringStream stream;

	stream << _T("Dumping LINEQUEUEINFO at 0x") << hex << (DWORD_PTR) lpQueueInfo << endl;

	// Dump the remainder of the structure
	if (lpQueueInfo == NULL || IsBadReadPtr(lpQueueInfo, sizeof(LINEQUEUEINFO)))
	{
		if (lpQueueInfo != NULL)
			stream << _T("  ** Bad Pointer **") << endl;
		return stream.str();
	}

	stream << _T("  dwTotalSize              = 0x")  << hex << lpQueueInfo->dwTotalSize << endl;
	stream << _T("  dwNeededSize             = 0x")  << hex << lpQueueInfo->dwNeededSize << endl;
	stream << _T("  dwUsedSize               = 0x")  << hex << lpQueueInfo->dwUsedSize << endl;
	stream << _T("  dwMeasurementPeriod      = 0x")  << hex << lpQueueInfo->dwMeasurementPeriod << endl;
	stream << _T("  dwTotalCallsQueued       = 0x")  << hex << lpQueueInfo->dwTotalCallsQueued << endl;
	stream << _T("  dwCurrentCallsQueued     = 0x")  << hex << lpQueueInfo->dwCurrentCallsQueued << endl;
	stream << _T("  dwTotalCallsAbandoned    = 0x")  << hex << lpQueueInfo->dwTotalCallsAbandoned << endl;
	stream << _T("  dwTotalCallsFlowedIn     = 0x")  << hex << lpQueueInfo->dwTotalCallsFlowedIn << endl;
	stream << _T("  dwTotalCallsFlowedOut    = 0x")  << hex << lpQueueInfo->dwTotalCallsFlowedOut << endl;
	stream << _T("  dwLongestEverWaitTime    = 0x")  << hex << lpQueueInfo->dwLongestEverWaitTime << endl;
	stream << _T("  dwCurrentLongestWaitTime = 0x")  << hex << lpQueueInfo->dwCurrentLongestWaitTime << endl;
	stream << _T("  dwAverageWaitTime        = 0x")  << hex << lpQueueInfo->dwAverageWaitTime << endl;
	stream << _T("  dwFinalDisposition       = 0x")  << hex << lpQueueInfo->dwFinalDisposition << endl;

	return stream.str();

}// _Dump

