/******************************************************************************/
//                                                                        
// APIHOOK.H - Class for hooking Win32 APIs in Winows 95 or NT.
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

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _SPLIB_APIHOOK_INC_
#define _SPLIB_APIHOOK_INC_

/**************************************************************************
** DWtoString
**
** Structure which converts between a bitmask and strings
**
***************************************************************************/
typedef struct
{
	DWORD dwBitMask;
	LPCTSTR pszText;

} DWtoString;

/**************************************************************************
** PARAMETERBLOCK
**
** Structure which details the parameter list for a hooked function
**
***************************************************************************/
typedef struct 
{
	BYTE bParameterType;
	LPCTSTR pszDescription;
	DWtoString* pBitMask;

} PARAMETER;

// Parameter types in the bParameters bytes
enum ParameterTypes {
	PARAM_DWORD = 0,					// 32-bit value output in HEX
	PARAM_SIZE,							// 32-bit value output in DEC (used as LPVOID size)
	PARAM_HANDLE,						// 32-bit value output in HEX
	PARAM_LPCSTR,						// Ptr to NULL-terminated ANSI string, output on entry
	PARAM_LPCWSTR,						// Ptr to NULL-terminated UNICODE string, output on entry
	PARAM_LPCVOID,						// Ptr to sized data value, output on entry
	PARAM_LPVOID_IO,					// Ptr to sized data value, output on entry/exit
	PARAM_HPROCESS,						// Ptr to process, output process name
	PARAM_BITMASK,						// Bitmask, use pBitMask to output
	PARAM_INDEX,                        // Index, use DumpExact
	PARAM_BOOL,							// Ptr to 1/0 value, output Yes/No
	PARAM_GUID,							// GUID
	PARAM_FUNCPTR,						// Ptr to function, output module name if possible	
	PARAM_STRUCT_LINECALLPARAMS,		// output on entry
	PARAM_STRUCT_LINEFORWARDLIST,		// output on entry
	PARAM_STRUCTURE_LINEGENERATETONE,	// output on entry
	PARAM_STRUCT_LINEMONITORTONE,		// output on entry
	PARAM_STRUCT_LINEDIALPARAMS,		// output on entry
	PARAM_STRUCT_LINEMEDIACONTROLDIGIT,	// output on entry
	PARAM_STRUCT_LINEMEDIACONTROLMEDIA,	// output on entry
	PARAM_STRUCT_LINEMEDIACONTROLTONE,	// output on entry
	PARAM_STRUCT_LINEMEDIACONTROLCALLSTATE,	// output on entry
	PARAM_STRUCT_PHONEBUTTONINFO_IN,	// output on entry
	PARAM_STRUCT_LINEAGENTGROUPLIST_IN, // output on entry
	PARAM_STRUCT_CALLHUBTACKINGINFO_IN, // output on entry
	PARAM_STRUCT_LINEAGENTINFO,			// output on exit
	PARAM_STRUCT_AGENTSESSIONLIST,		// output on exit
	PARAM_STRUCT_AGENTSESSIONINFO,		// output on exit
	PARAM_STRUCT_LINEQUEUELIST,			// output on exit
	PARAM_STRUCT_QUEUEINFO,				// output on exit
	PARAM_STRUCT_AGENTGROUPLIST,		// output on exit

	PT_OUTPUT_ON_EXIT_START,

	// Output on exit!

	PARAM_LPSTR,						// Ptr to NULL-terminated ANSI string, output on exit
	PARAM_LPWSTR,						// Ptr to NULL-terminated UNICODE string, output on entry
	PARAM_LPVOID,						// Ptr to sized data value, output on exit
	PARAM_LPDWORD,						// Ptr to DWORD, output on exit
	PARAM_LPHANDLE,						// Ptr to handle, output on exit
	PARAM_VARSTRING,					// Ptr to VARSTRING, output on exit
	PARAM_LPBITMASK,					// Ptr to bitmask, output on exit
	PARAM_LPGUID,						// Ptr to GUID
	PARAM_STRUCT_LINEADDRESSSTATUS,		// output on exit
	PARAM_STRUCT_LINEADDRESSCAPS,		// output on exit
	PARAM_STRUCT_LINECALLINFO,			// output on exit
	PARAM_STRUCT_LINECALLSTATUS,		// output on exit
	PARAM_STRUCT_LINEDEVCAPS,			// output on exit
	PARAM_STRUCT_LINEEXTENSIONID,		// output on exit
	PARAM_STRUCT_LINEDEVSTATUS,			// output on exit
	PARAM_STRUCT_PHONEBUTTONINFO_OUT,	// output on exit
	PARAM_STRUCT_PHONEDEVCAPS,			// output on exit
	PARAM_STRUCT_PHONEEXTENSIONID,		// output on exit
	PARAM_STRUCT_PHONESTATUS,			// output on exit
	PARAM_STRUCT_LINEAGENTSTATUS,		// output on exit
	PARAM_STRUCT_LINEAGENTCAPS,			// output on exit
	PARAM_STRUCT_LINEAGENTACTIVITYLIST,	// output on exit
	PARAM_STRUCT_LINEAGENTGROUPLIST_OUT, // output on exit
	PARAM_STRUCT_CALLHUBTACKINGINFO_OUT, // output on exit
};

/**************************************************************************
** APIHOOK
**
** Structure which provides the "map" for functions to hook in the target
**
***************************************************************************/
typedef struct
{
	LPCTSTR pszFunction;
	BYTE nCount;
	PARAMETER arrParams[12];

} APIHOOK;

/**************************************************************************
** APIFunction
**
** "stub" function which replaces the original address of the JMP
**
***************************************************************************/
#pragma pack(1)
typedef struct
{
    DWORD   RealProcAddress;
    BYTE    instr_pushad;							// pushad
    DWORD   instr_lea_eax_esp_plus_32;				// lea eax, esp+32
    BYTE    instr_push_eax;							// push eax
    BYTE    instr_mov_eax_params;					// mov eax, _pParams
    DWORD   offset_params;
    BYTE    instr_push_eax2;						// push eax
    BYTE    instr_call_LogFunction;					// call _logFunction
    DWORD   offset_LogFunction;
    BYTE    instr_popad;							// popad
    WORD    instr_jmp_dword_ptr_RealProcAddress;	// jmp OriginalAddress
    DWORD   offset_dword_ptr_RealProcAddrss;
} APIFunction, *PAPIFunction;
#pragma pack ()

/**************************************************************************
** TSPFunction
**
** Maps the JMP which should be the entry point for the TSP
**
***************************************************************************/
#pragma pack(1)
typedef struct
{
    BYTE    instr_jmp;
    DWORD   offset_dword_ptr_addr_to_jmp;

} TSPFunction, *PTSPFunction;
#pragma pack()

/**************************************************************************
** TLS Data
**
** Data structures stored as TLS data so we can keep track of returing
** function calls
**
***************************************************************************/
#define MAX_HOOKED_FUNCTIONS 50

typedef struct
{
    LPVOID pfnReturnAddress;
	APIHOOK* pHook;
	std::vector<DWORD> arrParams;
} HOOKED_FUNCTION;

typedef struct
{
    HOOKED_FUNCTION FunctionStack[MAX_HOOKED_FUNCTIONS];
    DWORD FunctionStackPtr;
} PER_THREAD_DATA, *PPER_THREAD_DATA;

/**************************************************************************
** CAPIHook
**
** This object manages the API hook functions
**
***************************************************************************/
class CAPIHook
{
// Class data
protected:
	bool m_fRunningOnWin9x;
	DWORD m_tlsIndex;		// TLS Index to use
	HINSTANCE m_hInstance;	// Module instance handle
	APIHOOK* m_pTable;		// Functions to hook
	std::vector<PAPIFunction> m_arrHookedFunctions;

// Constructor
public:
	CAPIHook(APIHOOK* pAPITable);
	~CAPIHook();

// Methods
public:
	bool ProcessAttach(HINSTANCE hInstance);
	bool ProcessDetach();
	bool ThreadAttach();
	bool ThreadDetach();
	void APIReturn(LPDWORD pFrame);
	bool InterceptFunctionReturn(APIHOOK* pHook, LPDWORD pFrame);

// Internal functions
protected:
	bool InterceptFunctionsInModule();
	PAPIFunction BuildAPIStub(LPVOID lpvFunction, APIHOOK* lpParams);
	void CAPIHook::HookFunction(LPVOID lpvFunction, APIHOOK* pHook);
};

#endif // _SPLIB_APIHOOK_INC_