// STDAFX.H - Pre-compiled header support

#define _WIN32_WINNT 0x0400			// Windows NT

#pragma warning(disable:4710)		// Function not inlined warning
#ifdef _DEBUG
#pragma warning(disable:4786)		// Template expansion > 255 characters
#pragma warning(disable: 4127)		// constant expression for _TSP_ASSERT
#else
#pragma warning(disable: 4702)		// unreachable code caused by optimizations in STL
#pragma warning(disable: 4791)		// loss of debugging info in release version
#endif

#include <tchar.h>					// Unicode support
#include <windows.h>				// Windows includes
#include <tspi.h>                   // TAPI support for service providers.
#include <splib.h>                  // JulMar TSP++ Class library

// Validate the version of TSP++ being used
#if (!SPLIB_CURRENT_VERSION) || (SPLIB_CURRENT_VERSION < 0x00030002)
#error "The TSP++ libraries you are using are not current enough to compile this code. Please upgrade your library to the latest release."
#endif
