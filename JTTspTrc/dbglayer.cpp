/******************************************************************************/
//                                                                        
// DBGLAYER.CPP - Service Provider DLL shell.                                
//                                                                        
// Copyright (C) 1998 Mark C. Smith, JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
//                                                                        
// This module intercepts the TSPI calls in the debug version of the library
// and then calls the normal layer
//
// This source code is intended only as a supplement to the
// TSP++ Class Library product documentation.  This source code cannot 
// be used in part or whole in any form outside the TSP++ library.
//                                                                        
/******************************************************************************/

/*---------------------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "dbglayer.h"
#include <ctype.h>
#include <atldef.h>
#undef ATLASSERT
#define ATLASSERT(x)
#include <atlconv.h>
#include "tapidmp.h"

/******************************************************************************/
//
// Basic DUMP functions
//
/******************************************************************************/

///////////////////////////////////////////////////////////////////////////
// HexDump
//
// Build a hex buffer from a pointer/size
//
TString HexDump(const void* lpBuff, DWORD dwSize)
{   
	USES_CONVERSION;
	
    if (lpBuff == NULL || dwSize == 0 || IsBadReadPtr(lpBuff, dwSize))
        return _T("");

	TStringStream stream;
    LPCSTR lpByte = static_cast<LPCSTR>(lpBuff);

    DWORD dwCount = 0, dwLine = 0;
	char szBuff[50], b[17];

    while (dwCount < dwSize)
    {
		// Set the line number
        wsprintfA(szBuff, "%0.8lX   ", dwLine);
		stream << A2T(szBuff);

		// Add the digits
        for (int i = 0; i < 16; i++)
        {
            if (dwSize-dwCount > 0)
            {
                b[i] = *lpByte++;
                ++dwCount;
				wsprintfA(szBuff, "%0.2X ", (int)b[i]&0xff);
				stream << A2T(szBuff);
            }
            else
			{
				stream << _T("   ");
                b[i] = 0;
			}

			// Change to a '.' if it is not printable
            if (!isprint(b[i]))
                b[i] = '.';
        }

		// Add the ascii portion
        b[16] = '\0';
		stream << _T("   ") << A2T(b) << endl;
        dwLine = dwCount;
    }

	return stream.str();

}// HexDump

///////////////////////////////////////////////////////////////////////////
// AsciiDump
//
// Build a ascii hex buffer from a pointer/size
//
TString AsciiDump(const void* lpBuff, DWORD dwSize)
{   
    if (lpBuff == NULL || dwSize == 0 || IsBadReadPtr(lpBuff, dwSize))
        return _T("\"\"");

	TStringStream stream;
	stream << _T("\"");

	try {

    LPCTSTR lpByte = static_cast<LPCTSTR>(lpBuff);
	LPWSTR lpswAllocBuff = NULL;

	// Quick check for ANSI string. Every string passed to/from the service provider
	// *should* be wide, but some fields are not verified or changed by TAPISRV
	// when going from the app to the TSP - notably the LINEAGENTGROUPENTRY.dwName
	// is not changed during the SetAgentGroup function.
	//
	// If we run across an ANSI string then change it to UNICODE for output.
	if (static_cast<DWORD>(lstrlenA(static_cast<LPCSTR>(lpBuff))) == (dwSize-1))
	{
		// Found an ANSI string - convert it to UNICODE
		LPCSTR lpszBuff = static_cast<LPCSTR>(lpBuff);
		int iSize = MultiByteToWideChar(CP_ACP, 0, lpszBuff, -1, NULL, 0);
		if (iSize > 0)
		{
			lpswAllocBuff = new wchar_t[iSize+1];
			MultiByteToWideChar(CP_ACP, 0, lpszBuff, -1, lpswAllocBuff, iSize+1);
			lpByte = lpswAllocBuff;
		}
	}
	else
		dwSize /= sizeof(TCHAR);

    DWORD dwCount = 0;
    TCHAR b[17];
    while (dwCount < dwSize)
    {
        // Grab this portion of the buffer.
        for (int i = 0; i < 16; i++)
        {
            if (dwSize-dwCount > 0)
            {
                b[i] = *lpByte++;
				if (!_istprint(b[i]))
					b[i] = '.';
                dwCount++;
            }
            else
                b[i] = 0;
        }
		b[i] = '\0';
		stream << b;
    }

	// Delete any temp buffer used for conversion.
	delete [] lpswAllocBuff;

	} 
	catch (...) 
	{
	}

	stream << _T("\"");
	return stream.str();

}// AsciiDump

///////////////////////////////////////////////////////////////////////////
// DumpEntryParameter
//
// Function which dumps out the "simple" parameter types
//
TString DumpEntryParameter(PARAMETER* pParam, DWORD dwParam)
{
	TStringStream stream;
	stream << pParam->pszDescription << _T("=");

	switch (pParam->bParameterType)
	{
		// Output as 32-bit value in decimal
		case PARAM_SIZE:
			stream << dwParam << _T(" bytes"); 
			break;

		// ANSI string
		case PARAM_LPCSTR:
			stream << _T("0x") << hex << dwParam << _T(" ") << AsciiDump((LPVOID)dwParam, lstrlenA((LPCSTR)dwParam)).c_str();
			break;

		// UNICODE string
		case PARAM_LPCWSTR:
			stream << _T("0x") << hex << dwParam << _T(" ") << _Dump((LPCWSTR)dwParam);
			break;

		// Process ID
		case PARAM_HPROCESS:
			stream << _T("0x") << hex << dwParam;
			break;

		// Bitmask
		case PARAM_BITMASK:
			stream << _T("0x") << hex << dwParam << _DumpBits(dwParam, pParam->pBitMask);
			break;

		// Index
		case PARAM_INDEX:
			stream << _T("0x") << hex << dwParam << _DumpExact(dwParam, pParam->pBitMask);
			break;
			
		// GUID
		case PARAM_GUID:
			stream << _DumpGUID(*((LPGUID)dwParam));
			break;

		// Yes/No value
		case PARAM_BOOL:
			stream << (dwParam != 0) ? _T("Yes") : _T("No");
			break;

		// Output as 32-bit value in hex
		default:
			stream << _T("0x") << hex << dwParam;
			break;
	}
	return stream.str();

}// DumpEntryParameter

///////////////////////////////////////////////////////////////////////////
// DumpExitParameter
//
// Function which dumps out the "simple" parameter types
//
TString DumpExitParameter(PARAMETER* pParam, DWORD dwParam)
{
	TStringStream stream;
	stream << pParam->pszDescription << _T("=");

	switch (pParam->bParameterType)
	{
		// ANSI string
		case PARAM_LPSTR:
			stream << _T("0x") << hex << dwParam << _T(" ") << AsciiDump((LPVOID)dwParam, lstrlenA((LPCSTR)dwParam)).c_str();
			break;

		// UNICODE string
		case PARAM_LPWSTR:
			stream << _T("0x") << hex << dwParam << _T(" ") << _Dump((LPCWSTR)dwParam);
			break;

		// Bitmask
		case PARAM_LPBITMASK:
			stream << _T("0x") << hex << dwParam << _DumpBits(*((LPDWORD)dwParam), pParam->pBitMask);
			break;

		// GUID
		case PARAM_LPGUID:
			stream << _DumpGUID(*((LPGUID)dwParam));
			break;

		// Output as 32-bit value in hex
		case PARAM_LPDWORD:
		case PARAM_LPHANDLE:
			if (dwParam != 0)
				stream << _T("0x") << hex << *((LPDWORD)dwParam);
			else
				stream << _T("NULL");
			break;

		// A pointer to a structure
		default:
			stream << _T("0x") << hex << dwParam;
			break;

	}
	return stream.str();

}// DumpExitParameter

///////////////////////////////////////////////////////////////////////////
// DumpStructure
//
// Function which dumps out a single TAPI structure
//
TString DumpStructure(PARAMETER* pParam, LPDWORD pdwParam)
{
	// If the current trace level isn't high enough, exit.
	if ((GetTraceLevel() & TRC_STRUCT) == 0	|| pdwParam == NULL || *pdwParam == '\0')
		return _T("");

	TStringStream stream;

	// If this is a variable-sized block, locate the PARAM_SIZE which 
	// goes with it.
	if (pParam->bParameterType == PARAM_LPCVOID ||
		pParam->bParameterType == PARAM_LPVOID_IO ||
		pParam->bParameterType == PARAM_LPVOID)
	{
		PARAMETER* pParamSize = pParam+1;
		if (pParamSize->bParameterType != PARAM_SIZE)
			return _T("");

		DWORD dwSize = *(pdwParam+1);
		if (*pdwParam != 0 && dwSize > 0)
		{
			stream << _T("Contents of ") << pParam->pszDescription << _T("=") << endl;
			stream << _Dump((const void*) *pdwParam, dwSize);
		}
		return stream.str();
	}

	switch (pParam->bParameterType)
	{
		case PARAM_VARSTRING:
			stream << _Dump((LPVARSTRING) *pdwParam).c_str();
			break;

		case PARAM_STRUCT_LINECALLPARAMS:
			stream << _Dump((LPLINECALLPARAMS) *pdwParam).c_str();
			break;

		case PARAM_STRUCT_LINEFORWARDLIST:
			stream << _Dump((LPLINEFORWARDLIST) *pdwParam).c_str();
			break;

		case PARAM_STRUCT_LINEADDRESSCAPS:
			stream << _Dump((LPLINEADDRESSCAPS) *pdwParam).c_str();
			break;

		case PARAM_STRUCTURE_LINEGENERATETONE:
			{
				// Back up one for the count
				DWORD dwCount = *(pdwParam-1);
				// and 3 for the tone mode
				DWORD dwToneMode = *(pdwParam-3);
				stream << _Dump(dwToneMode, (LPLINEGENERATETONE)*pdwParam, dwCount).c_str();
			}
			break;

		case PARAM_STRUCT_CALLHUBTACKINGINFO_IN:
		case PARAM_STRUCT_CALLHUBTACKINGINFO_OUT:
			stream << _Dump((LPLINECALLHUBTRACKINGINFO)*pdwParam);
			break;

		case PARAM_STRUCT_LINEADDRESSSTATUS:
			stream << _Dump((LPLINEADDRESSSTATUS)*pdwParam);
			break;

		case PARAM_STRUCT_LINECALLINFO:
			stream << _Dump((LPLINECALLINFO)*pdwParam);
			break;

		case PARAM_STRUCT_LINECALLSTATUS:
			stream << _Dump((LPLINECALLSTATUS) *pdwParam).c_str();
			break;

		case PARAM_STRUCT_LINEDEVCAPS:
			stream << _Dump((LPLINEDEVCAPS)*pdwParam);
			break;

		case PARAM_STRUCT_LINEEXTENSIONID:
			stream << _Dump((LPLINEEXTENSIONID) *pdwParam).c_str();
			break;

		case PARAM_STRUCT_LINEDEVSTATUS:
			stream << _Dump((LPLINEDEVSTATUS)*pdwParam);
			break;

		case PARAM_STRUCT_LINEAGENTGROUPLIST_IN:
		case PARAM_STRUCT_LINEAGENTGROUPLIST_OUT :
			stream << _Dump((LPLINEAGENTGROUPLIST)*pdwParam);
			break;

		case PARAM_STRUCT_LINEAGENTINFO:
			stream << _Dump((LPLINEAGENTINFO)*pdwParam);
			break;

		case PARAM_STRUCT_AGENTSESSIONLIST:
			stream << _Dump((LPLINEAGENTSESSIONLIST)*pdwParam);
			break;

		case PARAM_STRUCT_AGENTSESSIONINFO:
			stream << _Dump((LPLINEAGENTSESSIONINFO)*pdwParam);
			break;

		case PARAM_STRUCT_LINEQUEUELIST:
			stream << _Dump((LPLINEQUEUELIST)*pdwParam);
			break;

		case PARAM_STRUCT_QUEUEINFO:
			stream << _Dump((LPLINEQUEUEINFO)*pdwParam);
			break;

		case PARAM_STRUCT_AGENTGROUPLIST:
			stream << _Dump((LPLINEAGENTGROUPLIST) *pdwParam);
			break;

		case PARAM_STRUCT_PHONEBUTTONINFO_IN:
		case PARAM_STRUCT_PHONEBUTTONINFO_OUT:
			stream << _Dump((LPPHONEBUTTONINFO)*pdwParam);
			break;

		case PARAM_STRUCT_PHONEDEVCAPS:
			stream << _Dump((LPPHONECAPS)*pdwParam);
			break;

		case PARAM_STRUCT_PHONEEXTENSIONID:
			stream << _Dump((LPPHONEEXTENSIONID) *pdwParam).c_str();
			break;

		case PARAM_STRUCT_PHONESTATUS:
			stream << _Dump((LPPHONESTATUS)*pdwParam);
			break;

		case PARAM_STRUCT_LINEAGENTSTATUS:
			stream << _Dump((LPLINEAGENTSTATUS)*pdwParam);
			break;

		case PARAM_STRUCT_LINEAGENTCAPS:
			stream << _Dump((LPLINEAGENTCAPS)*pdwParam);
			break;

		case PARAM_STRUCT_LINEAGENTACTIVITYLIST:
			stream << _Dump((LPLINEAGENTACTIVITYLIST)*pdwParam);
			break;

		case PARAM_STRUCT_LINEMONITORTONE:
			stream << _Dump((LPLINEMONITORTONE)*pdwParam);
			break;

		case PARAM_STRUCT_LINEDIALPARAMS:
			stream << _Dump(true, (LPLINEDIALPARAMS)*pdwParam);
			break;

		case PARAM_STRUCT_LINEMEDIACONTROLDIGIT:
			break;

		case PARAM_STRUCT_LINEMEDIACONTROLMEDIA:
			break;

		case PARAM_STRUCT_LINEMEDIACONTROLTONE:
			break;

		case PARAM_STRUCT_LINEMEDIACONTROLCALLSTATE:
			break;

	}

	return stream.str();
}

///////////////////////////////////////////////////////////////////////////
// DumpFunctionEntry
//
// Function which is called when a "hooked" API is entered
//
// [0] Return Address
// [1] First Parameter
// [2] Second Parameter
// [n] nth Parameter
//
void __stdcall DumpFunctionEntry(APIHOOK* lpParams, LPDWORD pFrame)
{
	try
	{
		TStringStream stream;
		stream << lpParams->pszFunction << _T(" ");

		// Now walk the parameter list and dump each one.
		LPDWORD pParams = (pFrame+1);
		for (int i = 0; i < lpParams->nCount; i++)
		{
			if (i > 0)
				stream << _T(", ");
			stream << DumpEntryParameter(&lpParams->arrParams[i], *pParams).c_str();
			pParams++;
		}

		stream << endl;

		// Now walk back through and dump any ENTRY structures
		pParams = (pFrame+1);
		for (i = 0; i < lpParams->nCount; i++)
		{
			if (lpParams->arrParams[i].bParameterType < PT_OUTPUT_ON_EXIT_START)
				stream << DumpStructure(&lpParams->arrParams[i], pParams).c_str();
			pParams++;
		}

		// Throw it out the stream
		TraceOut(stream.str().c_str());
	}
	catch(SEH_Exception& e)
	{
		TCHAR chBuff[512];
		e.GetErrorMessage(chBuff, 512);
		TraceOut(chBuff);
	}
	catch (...)
	{
	}

    // Patch the return address of this function so that returns to us
    g_apiHook.InterceptFunctionReturn(lpParams, pFrame);

}// DumpFunctionEntry

///////////////////////////////////////////////////////////////////////////
// DumpFunctionExit
//
// Function which is called when a "hooked" API is exited
//
void __stdcall DumpFunctionExit(APIHOOK* lpParams, DWORD dwResult, std::vector<DWORD>& arrParams)
{
	try
	{
		TStringStream stream;
		stream << lpParams->pszFunction << _T(" rc=0x") << hex << dwResult;
		if (dwResult != 0 && dwResult >= 0x80000000)
			stream << _T(" (") << _DumpExact(dwResult, _ErrorCodes) << _T(")") << endl;
		
		// If there was not an error then dump the parameter list as well
		else
		{
			// Dump the basic parameters (no structures)
			for (int i = 0; i < lpParams->nCount; i++)
			{
				if (lpParams->arrParams[i].bParameterType > PT_OUTPUT_ON_EXIT_START)
				{
					stream << _T(", ");
					stream << DumpExitParameter(&lpParams->arrParams[i], arrParams[i]).c_str();
				}
			}

			stream << endl;

			// Now walk back through and dump any EXIT structures
			for (i = 0; i < lpParams->nCount; i++)
			{
				if (lpParams->arrParams[i].bParameterType > PT_OUTPUT_ON_EXIT_START)
					stream << DumpStructure(&lpParams->arrParams[i], ((LPDWORD)&arrParams[i])).c_str();
			}
		}
		TraceOut(stream.str().c_str());
	}
	catch(SEH_Exception& e)
	{
		TCHAR chBuff[512];
		e.GetErrorMessage(chBuff, 512);
		TraceOut(chBuff);
	}
	catch (...)
	{
	}

}// DumpFunctionExit

/////////////////////////////////////////////////////////////////////////////
// SEH_Exception -- this class was created so that we can catch
// Win32 SEH exceptions using the standard C++ try/catch functions.
// Normally, SEH can only be cause through the __try/__except handlers
// or through a catch(...) which is useless since that drops off the
// exception information.  This class hooks into the SEH handler and
// throws a normal class which may then be caught.
SEH_Exception::SEH_Exception(UINT nSeCode, _EXCEPTION_POINTERS* pExcPointers)
{ 
	m_nSeCode = nSeCode;
	m_pExcPointers = pExcPointers;
}

SEH_Exception::SEH_Exception(const SEH_Exception& e)
{
	m_nSeCode = e.m_nSeCode;
	m_pExcPointers = e.m_pExcPointers;
}

void* SEH_Exception::GetExceptionAddress()
{
	if (m_pExcPointers && m_pExcPointers->ExceptionRecord)
		return m_pExcPointers->ExceptionRecord->ExceptionAddress;
	return NULL;
}

bool SEH_Exception::GetErrorMessage(LPTSTR lpszError, int nMaxError)
{
	static LPCTSTR pszFormat = _T("Exception %s (0x%.8x) at address 0x%.8x");
	static struct
	{
		DWORD dwCode;
		LPCTSTR pszCode;
	} g_Exceptions[] = {

		{  EXCEPTION_ACCESS_VIOLATION, _T("Access Violation") },
		{  EXCEPTION_DATATYPE_MISALIGNMENT, _T("Datatype Misalignment") },
		{  EXCEPTION_BREAKPOINT, _T("Breakpoint") },
		{  EXCEPTION_SINGLE_STEP, _T("Single Step") },
		{  EXCEPTION_ARRAY_BOUNDS_EXCEEDED, _T("Array bounds exceeded") },
		{  EXCEPTION_FLT_DENORMAL_OPERAND, _T("Float denormal operand") },
		{  EXCEPTION_FLT_DIVIDE_BY_ZERO, _T("Float divide by Zero") },
		{  EXCEPTION_FLT_INEXACT_RESULT, _T("Float Inexact result") },
		{  EXCEPTION_FLT_INVALID_OPERATION, _T("Float invalid operation") },
		{  EXCEPTION_FLT_OVERFLOW, _T("Float overflow") },
		{  EXCEPTION_FLT_STACK_CHECK, _T("Float stack check") },
		{  EXCEPTION_FLT_UNDERFLOW, _T("Float underflow") },
		{  EXCEPTION_INT_DIVIDE_BY_ZERO, _T("Integer divide by zero") },
		{  EXCEPTION_INT_OVERFLOW, _T("Integer underflow") },
		{  EXCEPTION_PRIV_INSTRUCTION, _T("Privileged instruction") },
		{  EXCEPTION_IN_PAGE_ERROR, _T("In-page error") },
		{  EXCEPTION_ILLEGAL_INSTRUCTION, _T("Illegal instruction") },
		{  EXCEPTION_NONCONTINUABLE_EXCEPTION, _T("Non-continuable exception") },
		{  EXCEPTION_STACK_OVERFLOW, _T("Stack overflow") },
		{  EXCEPTION_INVALID_DISPOSITION, _T("Invalid disposition") },
		{  EXCEPTION_GUARD_PAGE, _T("Guard page") },
		{  EXCEPTION_INVALID_HANDLE, _T("Invalid handle") },
		{  0, _T("Unknown exception") }
	};

	TCHAR chBuff[512];

	for (int i = 0; i < (sizeof(g_Exceptions)/sizeof(g_Exceptions[0])); i++)
	{
		if (m_nSeCode == g_Exceptions[i].dwCode || g_Exceptions[i].dwCode == 0)
		{
			wsprintf(chBuff, pszFormat, g_Exceptions[i].pszCode, m_nSeCode, m_pExcPointers->ExceptionRecord->ExceptionAddress);
			CopyMemory(lpszError, chBuff, min(lstrlen(chBuff), static_cast<int>(nMaxError*sizeof(TCHAR))));
			break;
		}
	}
	return (g_Exceptions[i].dwCode != 0);
}
