/******************************************************************************/
//                                                                        
// APIHOOK.CPP - Class for hooking Win32 APIs in Winows 95 or NT.
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

/*---------------------------------------------------------------------------*/
// EXTERNALS
/*---------------------------------------------------------------------------*/
extern CAPIHook g_apiHook;
extern void TraceOut(LPCWSTR pszTrace);
extern void __stdcall DumpFunctionEntry(APIHOOK* lpParams, LPDWORD pFrame);
extern void __stdcall DumpFunctionExit(APIHOOK* lpParams, DWORD dwResult, std::vector<DWORD>& arrParams);

///////////////////////////////////////////////////////////////////////////
// CCommonReturnPoint
//
// Common return point for all functions that we've intercepted.
// Called by _AsmCommonReturnPoint in ASMRETRN.ASM
// pFrame is a pointer to the stack frame set up by the PUSHAD
//
// return_address <- pFrame[8]
// EAX            <- pFrame[7]
// ECX            <- pFrame[6]
// EDX            <- pFrame[5]
// EBX            <- pFrame[4]
// ESP            <- pFrame[3]
// EBP            <- pFrame[2]
// ESI            <- pFrame[1]
// EDI            <- pFrame[0]
//
void CCommonReturnPoint(LPDWORD pFrame)
{
	g_apiHook.APIReturn(pFrame);

}// CCommonReturnPoint

///////////////////////////////////////////////////////////////////////////
// AsmCommonReturnPoint
// 
// Return address for hooked functions
//
__declspec(naked) void AsmCommonReturnPoint()
{
	_asm sub esp,4
	_asm pushad;
	_asm mov eax, esp;
	_asm push eax;
	_asm call CCommonReturnPoint
	_asm add esp, 4;
	_asm popad;
	_asm ret;

}// AsmCommonReturnPoint

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::CAPIHook
//
// Constructor for the API hook object
//
CAPIHook::CAPIHook(APIHOOK* pTable) : m_hInstance(0), m_pTable(pTable), m_fRunningOnWin9x(false)
{
	m_tlsIndex = TlsAlloc();

}// CAPIHook::CAPIHook

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::~CAPIHook
//
// Destructor for the API hook object
//
CAPIHook::~CAPIHook()
{
}// CAPIHook::~CAPIHook

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::ProcessAttach
//
// A process is attaching to this DLL, hook it.
//
bool CAPIHook::ProcessAttach(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

    OSVERSIONINFO verInfo = {0}; 
    verInfo.dwOSVersionInfoSize = sizeof(verInfo); 
    GetVersionEx(&verInfo); 
    m_fRunningOnWin9x = (verInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

    if (InterceptFunctionsInModule())
		return ThreadAttach();
	return false;

}// CAPIHook::ProcessAttach

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::ProcessDetach
//
// A process is detaching from this DLL, unhook it.
//
bool CAPIHook::ProcessDetach()
{
	return ThreadDetach();

}// CAPIHook::ProcessDetach

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::ThreadAttach
//
// A thread is attaching to this DLL, create the TLS data we use to 
// track function calls
//
bool CAPIHook::ThreadAttach()
{
    PPER_THREAD_DATA pPerThreadData = new PER_THREAD_DATA;
    if (!pPerThreadData)
        return FALSE;
    
    pPerThreadData->FunctionStackPtr = 0;
    TlsSetValue(m_tlsIndex, pPerThreadData);
    return true;

}// CAPIHook::ThreadAttach

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::ThreadDetach
//
// A thread is detaching from this DLL, destroy the TLS data
//
bool CAPIHook::ThreadDetach()
{
    delete (PPER_THREAD_DATA) TlsGetValue(m_tlsIndex);
	return true;

}// CAPIHook::ThreadDetach

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::InterceptFunctionsInModule
//
// This function "hooks" the specified module
//
bool CAPIHook::InterceptFunctionsInModule()
{
	// Walk our intercept table and locate each function in the 
	// "hooked" module.
	APIHOOK* pTable = m_pTable;
	while (pTable->pszFunction != NULL)
	{
		std::string strName = ConvertWideToAnsi(pTable->pszFunction);
		LPVOID lpvFunction = (LPVOID) GetProcAddress(m_hInstance, strName.c_str());
		if (lpvFunction == NULL)
		{
			// Try __stdcall calling sequence (for lineAgent functions)
			strName = "_" + strName;
			char chBuff[6];
			chBuff[0] = '@';
			_itoa(static_cast<int>(pTable->nCount) * sizeof(DWORD),&chBuff[1], 10);
			strName += chBuff;
			lpvFunction = (LPVOID) GetProcAddress(m_hInstance, strName.c_str());
		}
		if (lpvFunction != NULL)
			HookFunction(lpvFunction, pTable);
		pTable++;
	}

	// Flush the procesor cache to make sure we get the changes.
	FlushInstructionCache(GetCurrentProcess(), NULL, 0);
	return (m_arrHookedFunctions.size() > 0);

}// CAPIHook::InterceptFunctionsInModule

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::HookFunction
//
// This function hooks a single function in the target module
//
void CAPIHook::HookFunction(LPVOID lpvFunction, APIHOOK* pHook)
{
	// Create a new stub for this function
    PAPIFunction pNewFunction = BuildAPIStub(lpvFunction, pHook);
	if (pNewFunction != NULL)
	{
		try 
		{
			m_arrHookedFunctions.push_back(pNewFunction);
		}
		catch (...)
		{
		}

		// Make the function "writable".  This code works for both Windows 95
		// and Windows NT.
		TSPFunction* pFunction = (TSPFunction*) lpvFunction;
		DWORD dwOldProtect;
		if (VirtualProtect(pFunction, sizeof(TSPFunction), PAGE_READWRITE, &dwOldProtect))
		{
			// Now modify the passed address with our new address
			pFunction->offset_dword_ptr_addr_to_jmp = 
				(DWORD)&(pNewFunction->instr_pushad) - (DWORD)lpvFunction - 5;

			// Reset the protection on the page
			VirtualProtect(pFunction, sizeof(TSPFunction), dwOldProtect, &dwOldProtect);
		}
	}

}// CAPIHook::HookFunction

///////////////////////////////////////////////////////////////////////////////
// CAPIHook::BuildAPIStub
//
// This function builds the API stub which is then inserted into the 
// module we are hooking.
//
PAPIFunction CAPIHook::BuildAPIStub(LPVOID lpvFunction, APIHOOK* lpParams)
{
    PAPIFunction pNewFunction = new APIFunction;
    if (pNewFunction == NULL)
        return 0;

	DWORD dwAddress = *(LPDWORD)((LPBYTE)lpvFunction+1) + (DWORD)lpvFunction + 5;
    pNewFunction->RealProcAddress			 = dwAddress;
    pNewFunction->instr_pushad				 = 0x60;
    pNewFunction->instr_lea_eax_esp_plus_32  = 0x2024448D;
    pNewFunction->instr_push_eax			 = 0x50;
	pNewFunction->instr_mov_eax_params       = 0xB8;
    pNewFunction->offset_params				 = (DWORD)lpParams;
	pNewFunction->instr_push_eax2            = 0x50;
    pNewFunction->instr_call_LogFunction	 = 0xE8;
    pNewFunction->offset_LogFunction		 = (DWORD)DumpFunctionEntry - (DWORD)&pNewFunction->instr_popad;
    pNewFunction->instr_popad				 = 0x61;
    pNewFunction->instr_jmp_dword_ptr_RealProcAddress = 0x25FF;
    pNewFunction->offset_dword_ptr_RealProcAddrss = (DWORD)pNewFunction;

	// Make this page executable -- this bypasses the DPE support in XP.
	DWORD dwOldProtect = 0;
	VirtualProtect(pNewFunction, sizeof(APIFunction), PAGE_EXECUTE_READWRITE, &dwOldProtect);

    return pNewFunction;

}// CAPIHook::BuildAPIStub

///////////////////////////////////////////////////////////////////////////
// CAPIHook::InterceptFunctionReturn
//
// Patches return value so we get result code from the function
//
bool CAPIHook::InterceptFunctionReturn(APIHOOK* pHook, LPDWORD pFrame)
{
    PPER_THREAD_DATA pThreadData = (PPER_THREAD_DATA) TlsGetValue(m_tlsIndex);
    if (!pThreadData)
	{
		// This thread was created before we hooked the module, add our TLS data
		// to it so we can use it from now on.
		if (!ThreadAttach())
			return false;
		pThreadData = (PPER_THREAD_DATA) TlsGetValue(m_tlsIndex);
	}

	// See if we have reached our max level
    if (pThreadData->FunctionStackPtr >= (MAX_HOOKED_FUNCTIONS-1))
        return false;
    
	// Otherwise, set up the next layer
    DWORD dwLevel = pThreadData->FunctionStackPtr;
    pThreadData->FunctionStackPtr++;
    pThreadData->FunctionStack[dwLevel].pfnReturnAddress = (LPVOID) pFrame[0];
    pThreadData->FunctionStack[dwLevel].pHook = pHook;
	pThreadData->FunctionStack[dwLevel].arrParams.clear();

	// Copy the parameters over
	LPDWORD pParam = (pFrame+1);
	for (int i = 0; i < pHook->nCount; i++, pParam++)
		pThreadData->FunctionStack[dwLevel].arrParams.push_back(*pParam);

    // Point the return to our assembler return point - it will call
	// the below common return point
    pFrame[0] = (DWORD)AsmCommonReturnPoint;
    return true;

}// CAPIHook::InterceptFunctionReturn

///////////////////////////////////////////////////////////////////////////
// CAPIHook::APIReturn
//
// This is the return point for our call
//
void CAPIHook::APIReturn(LPDWORD pFrame)
{
    PPER_THREAD_DATA pThreadData = (PPER_THREAD_DATA)TlsGetValue(g_apiHook.m_tlsIndex);
    if (!pThreadData)
        return;

	// Drop our level
    DWORD dwLevel = --pThreadData->FunctionStackPtr;

	// Emit the information about the function return value to the logging mechanism.
	DumpFunctionExit(pThreadData->FunctionStack[dwLevel].pHook, pFrame[7], 
					 pThreadData->FunctionStack[dwLevel].arrParams);

    // Patch the return address back to what it was when the function was originally called.
    pFrame[8] = (DWORD) pThreadData->FunctionStack[dwLevel].pfnReturnAddress;

}// CAPIHook::APIReturn

