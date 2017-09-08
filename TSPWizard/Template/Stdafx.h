// STDAFX.H - Pre-compiled header support

#pragma once

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#define VC_EXTRALEAN				// Exclude rarely-used stuff from Windows headers
$$IF(USE_WINSOCK)
#define _WINSOCKAPI_				// Don't include Winsock.h
$$ENDIF
#include <tchar.h>					// Unicode support
#include <windows.h>				// Windows includes
$$IF(USE_WINSOCK)
#include <winsock2.h>				// Winsock 2
$$ENDIF
#include <splib.h>                  // JulMar TSP++ Class library

