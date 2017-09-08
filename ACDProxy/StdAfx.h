// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#define _WIN32_WINNT 0x0500
#define UNICODE
#define _UNICODE
#define VC_EXTRALEAN

#pragma warning(disable:4786)

#include <windows.h>		// Windows includes
#include <windowsx.h>		// Additional macros
#include <tchar.h>			// UNICODE support
#pragma warning(disable:4702)
#include <string>			// STL objects - string support
#include <typeinfo>			// STL objects
#include <map>				// STL objects - generic map support
#pragma warning(default:4702)
#include <tapi.h>			// TAPI headers

// Hide UNICODE string support
typedef std::wstring TString;
#include "..\SRC\spacd.h"	// TSP++ ACD interface	

#endif // __STDAFX_H__
