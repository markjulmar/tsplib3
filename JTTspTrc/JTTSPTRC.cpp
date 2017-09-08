/******************************************************************************/
//                                                                        
// JTTSPTRC.CPP - TSP Tracing facility DLL
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
#include "apihook.h"
#include "tsphook.h"
#include "dbglayer.h"

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
TRACEPROC g_Trace = NULL;
TRACELEVEL g_TraceLevel = NULL;
CAPIHook g_apiHook(g_FunctionsToHook);

/******************************************************************************/
// DLLMain
// 
// Main entrypoint for the DLL - processes all thread attach and detach
// requests.
//
/******************************************************************************/
BOOL APIENTRY DllMain(HANDLE /*hModule*/, DWORD dwReason, LPVOID /*lpReserved*/)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:	return TRUE;
        case DLL_THREAD_ATTACH:		return g_apiHook.ThreadAttach();
        case DLL_PROCESS_DETACH:	return g_apiHook.ProcessDetach();
        case DLL_THREAD_DETACH:		return g_apiHook.ThreadDetach();
    }
	return FALSE;

}// DllMain

/******************************************************************************/
// SeTranslator and SetSEHExceptionHandler
// 
// Captures SEH exceptions in our driver
//
/******************************************************************************/
void SeTranslator(UINT nSeCode, _EXCEPTION_POINTERS* pExcPointers)
{
	throw SEH_Exception(nSeCode,pExcPointers);
}

bool SetSEHExceptionHandler()
{
	_set_se_translator(SeTranslator);		
	return true;
}

/******************************************************************************/
// InitializeDebugLayer
// 
// This is called by the TSP library to initialize the layer DLL
//
/******************************************************************************/
extern "C" bool _declspec(dllexport) InitializeDebugLayer(HINSTANCE hInstance)
{
	// Load our TRACE function
	g_Trace = (TRACEPROC) GetProcAddress(hInstance, "_TraceOutW@4");
	g_TraceLevel = (TRACELEVEL) GetProcAddress(hInstance, "_GetTraceLevel@0");
	if (g_Trace == NULL || g_TraceLevel == NULL)
	{
		OutputDebugStringA("JTTSPTRC: Unable to locate required TraceOutW/GetTraceLevel exports in TSP!\r\n");
		return false;
	}

	// Set our exception handler - note this is a per-process event. This
	// means that all TAPISRV's exceptions are now coming through us!
	SetSEHExceptionHandler();

	return g_apiHook.ProcessAttach(hInstance);

}// InitializeDebugLayer

///////////////////////////////////////////////////////////////////////////
// ConvertWideToAnsi
//
// Utility function included with non-UNICODE build to convert the
// UNICODE strings to normal ANSI single-byte strings.
//
std::string _stdcall ConvertWideToAnsi (LPCWSTR lpszInput)
{
	std::string strReturn;
	if (lpszInput != NULL)
	{
		char szBuffer[255];
		int iSize = WideCharToMultiByte (CP_ACP, 0, lpszInput, -1, NULL, 0, NULL, NULL);
		if (iSize > 0)
		{
			LPSTR lpszBuff = (iSize > 254) ? new char [iSize+1] : szBuffer;
			if (lpszBuff != NULL)
				WideCharToMultiByte (CP_ACP, 0, lpszInput, -1, lpszBuff, iSize, NULL, NULL);
			strReturn = lpszBuff;
			if (iSize > 255)
				delete [] lpszBuff;
		}
	}
	return strReturn;

}// ConvertWideToAnsi

///////////////////////////////////////////////////////////////////////////
// TraceOut
//
// Outputs a line via the trace mechanism in the TSP
//
void TraceOut(LPCWSTR pszTrace)
{
	if (g_Trace != NULL && !IsBadCodePtr((FARPROC)g_Trace))
	{
		try 
		{
			g_Trace(pszTrace);
			return;
		}
		catch (...)
		{
		}
	}

}// TraceOut

///////////////////////////////////////////////////////////////////////////
// GetTraceLevel
//
// Returns the trace level
//
int GetTraceLevel()
{
	if (g_TraceLevel != NULL && !IsBadCodePtr((FARPROC)g_TraceLevel))
	{
		try
		{
			return g_TraceLevel();
		}
		catch (...)
		{
		}
	}

	return 0;

}// GetTraceLevel
