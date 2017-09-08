// STDAFX.H - Pre-compiled header support

#define _WIN32_WINNT 0x0400			// Windows NT
#include <tchar.h>					// Unicode support
#include <windows.h>				// Windows includes
#include <tspi.h>                   // TAPI support for service providers.
#include <splib.h>                  // JulMar TSP++ Class library

// Validate the version of TSP++ being used
#if (!SPLIB_CURRENT_VERSION) || (SPLIB_CURRENT_VERSION < 0x00030002)
#error "The TSP++ libraries you are using are not current enough to compile this code. Please upgrade your library to the latest release."
#endif
