// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DB9F5024_6409_11D2_A217_006008D1D6DF__INCLUDED_)
#define AFX_STDAFX_H__DB9F5024_6409_11D2_A217_006008D1D6DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _UNICODE
#define UNICODE
#define TAPI_CURRENT_VERSION 0x00030000
#include <tchar.h>
#include <windows.h>
#include <wtypes.h>
/*-------------------------------------------------------------------------------*/
// STL headers required for building library
/*-------------------------------------------------------------------------------*/
#pragma warning(disable:4702)
#include <string>
#include <typeinfo>
#include <algorithm>				// STL algorithms
#include <vector>					// Vector array class
#include <sstream>					// Stream support (for debug)
#include <ios>						// Stream support (for debug)
#include <iomanip>					// Stream manipulators
#pragma warning(default:4702)
#include <tapi.h>
#include <tspi.h>

using std::hex;
using std::dec;
using std::setw;
using std::setfill;
using std::endl;

extern std::string _stdcall ConvertWideToAnsi (LPCWSTR lpszInput);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DB9F5024_6409_11D2_A217_006008D1D6DF__INCLUDED_)
