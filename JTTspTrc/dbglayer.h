/******************************************************************************/
//                                                                        
// DBGLAYER.H - Service Provider DLL shell.                                
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

#ifndef _SPLIB_DBGLAYER_INC_
#define _SPLIB_DBGLAYER_INC_

#include "apihook.h"

/*---------------------------------------------------------------------------*/
// EXTERNALS
/*---------------------------------------------------------------------------*/
extern CAPIHook g_apiHook;

/*---------------------------------------------------------------------------*/
// TYPEDEFS
/*---------------------------------------------------------------------------*/
typedef void (__stdcall *TRACEPROC)(LPCWSTR);
typedef DWORD (__stdcall *TRACELEVEL)(void);
typedef std::wstring TString;
typedef std::wstringstream TStringStream;

// Must match splib.h
enum { 
	TRC_NONE		= 0x00000000,	// No Tracing
	TRC_MIN			= 0x00000001,	// Minimum tracing, TRACE macro, DTRACE macro
	TRC_API			= 0x00000002,	// TAPI api traces (parameters only)
	TRC_STRUCT		= 0x00000004,	// All structures to/from TAPI
	TRC_DUMP		= 0x00000008,	// Offset/Size pointers within structures to/from TAPI
	TRC_STATS       = 0x00000010,	// Statistics (event notification)
	TRC_OBJECTS     = 0x00000020,	// Basic telephony object creation/destruction (addr/line/phone)
	TRC_THREADS		= 0x00000040,	// Thread creation/destruction
	TRC_REQUESTS	= 0x00000080,	// Request creation/destruction
	TRC_CALLS		= 0x00000100,	// Call creation/destruction
	TRC_CALLMAP		= 0x00000200,	// Call id map
	TRC_WARNINGS	= 0x00000400,	// Warnings/Errors
	TRC_WORKERTHRD  = 0x00000800,	// Worker thread execution
	TRC_LOCKS		= 0x00001000,	// Full lock/unlock notifications
	TRC_CRITSEC     = 0x00002000,	// Win32 Critical section create/destroy
	TRC_AGENTPROXY  = 0x00004000,	// Agent proxy support
	TRC_USERDEFINED = 0x0FF00000,	// Derived TSP traces
	TRC_FULL		= 0x0FFFFFFF,	// All of the above
};

/*---------------------------------------------------------------------------*/
// Handle SEH exceptions as C++ exceptions
/*---------------------------------------------------------------------------*/
class SEH_Exception
{
// Class data
private:
	UINT m_nSeCode;
	_EXCEPTION_POINTERS* m_pExcPointers;

// Constructor
public:    
	SEH_Exception(UINT nSeCode, _EXCEPTION_POINTERS* pExcPointers);
	SEH_Exception(const SEH_Exception& e);

// Accessors
public:
    UINT GetSeCode() { return m_nSeCode; }
	_EXCEPTION_POINTERS* GetSePointers() { return m_pExcPointers; }
	void* GetExceptionAddress();
	bool GetErrorMessage(LPTSTR lpszError, int nMaxError);
};

#endif // _SPLIB_DBGLAYER_INC_