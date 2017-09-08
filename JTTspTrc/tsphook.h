/******************************************************************************/
//                                                                        
// TSPHOOK.H - Definition of the API hook table
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

#include "bitmask.h"

/*---------------------------------------------------------------------------*/
// APIHOOK structures
/*---------------------------------------------------------------------------*/
APIHOOK g_FunctionsToHook[] = {
	// TSPI_lineAccept(DRV_REQUESTID, HDRVCALL, LPCSTR, DWORD);
	{ _T("TSPI_lineAccept"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCVOID,	_T("lpsUserUserInfo"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineAddToConference(DRV_REQUESTID, HDRVCALL, HDRVCALL);
	{ _T("TSPI_lineAddToConference"), 3, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdConf"), NULL },
			{ PARAM_HANDLE,		_T("hdCons"), NULL }
		}
	},

	// TSPI_lineAnswer(DRV_REQUESTID, HDRVCALL, LPCSTR, DWORD);
	{ _T("TSPI_lineAnswer"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCVOID,	_T("lpsUserUserInfo"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineBlindTransfer(DRV_REQUESTID, HDRVCALL, LPCWSTR, DWORD);
	{ _T("TSPI_lineBlindTransfer"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL },
			{ PARAM_DWORD,		_T("dwCountryCode"), NULL }
		}
	},

	// TSPI_lineClose(HDRVLINE);
	{ _T("TSPI_lineClose"), 1, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
		}
	},

	// TSPI_lineCloseCall(HDRVCALL);
	{ _T("TSPI_lineCloseCall"), 1, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
		}
	},

	// TSPI_lineCompleteCall(DRV_REQUESTID, HDRVCALL, LPDWORD, DWORD, DWORD);
	{ _T("TSPI_lineCompleteCall"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPDWORD,    _T("lpdwCompletionID"), NULL },
			{ PARAM_BITMASK,    _T("dwCompletionMode"), _CompletionModes },
			{ PARAM_DWORD,		_T("dwMessageID"), NULL }
		}
	},

	// TSPI_lineCompleteTransfer(DRV_REQUESTID, HDRVCALL, HDRVCALL, HTAPICALL, LPHDRVCALL, DWORD);
	{ _T("TSPI_lineCompleteTransfer"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_HANDLE,		_T("hdConsultCall"), NULL },
			{ PARAM_HANDLE,		_T("htConfCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdConfCall"), NULL },
			{ PARAM_BITMASK,	_T("dwTransferMode"), _TransferModes }
		}
	},

	// TSPI_lineConditionalMediaDetection(HDRVLINE, DWORD, LPLINECALLPARAMS);
	{ _T("TSPI_lineConditionalMediaDetection"), 3, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_BITMASK,	_T("dwMediaModes"), _MediaModes },
			{ PARAM_STRUCT_LINECALLPARAMS, _T("lpCallParams"), NULL }
		}
	},

	// TSPI_lineDevSpecific(DRV_REQUESTID, HDRVLINE, DWORD, HDRVCALL, LPVOID, DWORD);
	{ _T("TSPI_lineDevSpecific"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPVOID_IO,	_T("lpParams"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineDevSpecificFeature(DRV_REQUESTID, HDRVLINE, DWORD, LPVOID, DWORD);
	{ _T("TSPI_lineDevSpecificFeature"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_DWORD,		_T("dwFeature"), NULL },
			{ PARAM_LPVOID_IO,	_T("lpParams"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineDial(DRV_REQUESTID, HDRVCALL, LPCWSTR, DWORD);
	{ _T("TSPI_lineDial"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL },
			{ PARAM_DWORD,		_T("dwCountryCode"), NULL }
		}
	},

	// TSPI_lineDrop(DRV_REQUESTID, HDRVCALL, LPCWSTR, DWORD);
	{ _T("TSPI_lineDrop"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCVOID,	_T("lpsUserUserInfo"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineForward(DRV_REQUESTID, HDRVLINE, DWORD, DWORD, LPLINEFORWARDLIST const, DWORD, HTAPICALL, LPHDRVCALL, LPLINECALLPARAMS const);
	{ _T("TSPI_lineForward"), 8, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_BOOL,       _T("bAllAddresses"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEFORWARDLIST, _T("lpLineForwardList"), NULL },
			{ PARAM_SIZE,		_T("dwNumRingsNoAnswer"), NULL },
			{ PARAM_HANDLE,		_T("htConsultCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdConsultCall"), NULL },
			{ PARAM_STRUCT_LINECALLPARAMS, _T("lpCallParams"), NULL }
		}
	},

	// TSPI_lineGatherDigits(HDRVCALL, DWORD, DWORD, LPWSTR, DWORD, LPCWSTR, DWORD, DWORD);
	{ _T("TSPI_lineGatherDigits"), 8, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_DWORD,		_T("dwEndToEnd"), NULL },
			{ PARAM_BITMASK,	_T("dwDigitModes"), _DigitModes },
			{ PARAM_DWORD,		_T("lpsDigits"), NULL },
			{ PARAM_SIZE,		_T("dwNumDigits"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszTerminationDigits"), NULL },
			{ PARAM_DWORD,		_T("dwFirstDigitTimeout"), NULL },
			{ PARAM_DWORD,		_T("dwInterdigitTimeout"), NULL }
		}
	},

	// TSPI_lineGenerateDigits(HDRVCALL, DWORD, DWORD, LPCWSTR, DWORD);
	{ _T("TSPI_lineGenerateDigits"), 5, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_DWORD,		_T("dwEndToEnd"), NULL },
			{ PARAM_BITMASK,	_T("dwDigitMode"), _DigitModes },
			{ PARAM_LPCWSTR,	_T("lpszDigits"), NULL },
			{ PARAM_DWORD,		_T("dwDuration"), NULL }
		}
	},

	// TSPI_lineGenerateTone(HDRVCALL, DWORD, DWORD, DWORD, DWORD, LPLINEGENERATETONE const);
	{ _T("TSPI_lineGenerateTone"), 6, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_DWORD,		_T("dwEndToEnd"), NULL },
			{ PARAM_BITMASK,	_T("dwToneMode"), _ToneModes },
			{ PARAM_DWORD,		_T("dwDuration"), NULL },
			{ PARAM_STRUCTURE_LINEGENERATETONE,	_T("lpTones"), NULL }
		}
	},

	// TSPI_lineGetAddressCaps(DWORD, DWORD, DWORD, DWORD, LPLINEADDRESSCAPS);
	{ _T("TSPI_lineGetAddressCaps"), 5, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwExtVersion"), NULL },
			{ PARAM_STRUCT_LINEADDRESSCAPS, _T("lpAddressCaps"), NULL }
		}
	},

	// TSPI_lineGetAddressID(HDRVLINE, LPDWORD, DWORD, LPCWSTR, DWORD);
	{ _T("TSPI_lineGetAddressID"), 5, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_LPDWORD,	_T("lpdwAddressID"), NULL },
			{ PARAM_BITMASK,	_T("dwAddressMode"), _AddressModes },
			{ PARAM_LPCWSTR,	_T("lpsAddress"), NULL },
			{ PARAM_DWORD,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineGetAddressStatus(HDRVLINE, DWORD, LPLINEADDRESSSTATUS);
	{ _T("TSPI_lineGetAddressStatus"), 3, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEADDRESSSTATUS, _T("lpAddressStatus"), NULL }
		}
	},

	// TSPI_lineGetCallAddressID(HDRVCALL, LPDWORD);
	{ _T("TSPI_lineGetCallAddressID"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwAddressID"), NULL }
		}
	},

	// TSPI_lineGetCallInfo(HDRVCALL, LPLINECALLINFO);
	{ _T("TSPI_lineGetCallInfo"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_STRUCT_LINECALLINFO, _T("lpCallInfo"), NULL }
		}
	},

	// TSPI_lineGetCallStatus(HDRVCALL, LPLINECALLSTATUS);
	{ _T("TSPI_lineGetCallStatus"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_STRUCT_LINECALLSTATUS, _T("lpCallStatus"), NULL }
		}
	},

	// TSPI_lineGetDevCaps(DWORD, DWORD, DWORD, LPLINEDEVCAPS);
	{ _T("TSPI_lineGetDevCaps"), 4, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwExtVersion"), NULL },
			{ PARAM_STRUCT_LINEDEVCAPS, _T("lpLineDevCaps"), NULL },
		}
	},

	// TSPI_lineGetDevConfig(DWORD, LPVARSTRING, LPCWSTR);
	{ _T("TSPI_lineGetDevConfig"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_VARSTRING,	_T("lpDeviceConfig"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDeviceClass"), NULL }
		}
	},

	// TSPI_lineGetExtensionID(DWORD, DWORD, LPLINEEXTENSIONID);
	{ _T("TSPI_lineGetExtensionID"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_STRUCT_LINEEXTENSIONID, _T("lpExtensionID"), NULL }			
		}
	},

	// TSPI_lineGetIcon(DWORD, LPCWSTR, LPHICON):
	{ _T("TSPI_lineGetIcon"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDeviceClass"), NULL },
			{ PARAM_LPHANDLE,	_T("lphIcon"), NULL }
		}
	},

	// TSPI_lineGetID(HDRVLINE, DWORD, HDRVCALL, DWORD, LPVARSTRING, LPCWSTR, HANDLE);
	{ _T("TSPI_lineGetID"), 7, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwSelect"), _LineIDSelect },
			{ PARAM_VARSTRING,	_T("lpDeviceID"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDeviceClass"), NULL },
			{ PARAM_HPROCESS,   _T("hTargetProces"), NULL }
		}
	},

	// TSPI_lineGetLineDevStatus(HDRVLINE, LPLINEDEVSTATUS);
	{ _T("TSPI_lineGetLineDevStatus"), 2, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_STRUCT_LINEDEVSTATUS, _T("lpLineDevStatus"), NULL }
		}
	},

	// TSPI_lineGetNumAddressIDs(HDRVLINE, LPDWORD);
	{ _T("TSPI_lineGetNumAddressIDs"), 2, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_LPDWORD,	_T("lpdwAddressIDs"), NULL }
		}
	},

	// TSPI_lineHold(DRV_REQUESTID, HDRVCALL);
	{ _T("TSPI_lineHold"), 2, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }
		}
	},

	// TSPI_lineMakeCall(DRV_REQUESTID, HDRVLINE, HTAPICALL, LPHDRVCALL, LPCWSTR, DWORD, LPLINECALLPARAMS const);
	{ _T("TSPI_lineMakeCall"), 7, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_HANDLE,		_T("htCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdCall"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL },
			{ PARAM_DWORD,		_T("dwCountryCode"), NULL },
			{ PARAM_STRUCT_LINECALLPARAMS, _T("lpCallParams"), NULL }
		}
	},

	// TSPI_lineMonitorDigits(HDRVCALL, DWORD);
	{ _T("TSPI_lineMonitorDigits"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwDigitModes"), _DigitModes }
		}
	},

	// TSPI_lineMonitorMedia(HDRVCALL, DWORD);
	{ _T("TSPI_lineMonitorMedia"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwMediaModes"), _MediaModes }
		}
	},

	// TSPI_lineMonitorTones(HDRVCALL, DWORD, LPLINEMONITORTONE const, DWORD);
	{ _T("TSPI_lineMonitorTones"), 4, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_DWORD,		_T("dwToneListID"), NULL },
			{ PARAM_STRUCT_LINEMONITORTONE, _T("lpToneList"), NULL },
			{ PARAM_SIZE,		_T("dwNumEntries"), NULL }
		}
	},

	// TSPI_lineNegotiateExtVersion(DWORD, DWORD, DWORD, LPDWORD);
	{ _T("TSPI_lineNegotiateExtVersion"), 4, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwLoVersion"), NULL },
			{ PARAM_DWORD,		_T("dwHiVersion"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwExtVersion"), NULL }
		}
	},

	// TSPI_lineNegotiateTSPIVersion(DWORD, DWORD, DWORD, LPDWORD);
	{ _T("TSPI_lineNegotiateTSPIVersion"), 4, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwLoVersion"), NULL },
			{ PARAM_DWORD,		_T("dwHiVersion"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwTSPIVersion"), NULL }
		}
	},

	// TSPI_lineOpen(DWORD, HTAPILINE, LPHDRVLINE, DWORD, LINEEVENT);
	{ _T("TSPI_lineOpen"), 5, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("htLine"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdLine"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_FUNCPTR,	_T("lpfnEventProc"), NULL }
		}
	},

	// TSPI_linePark(DRV_REQUESTID, HDRVCALL, DWORD, LPCWSTR, LPVARSTRING);
	{ _T("TSPI_linePark"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }, 
			{ PARAM_BITMASK,	_T("dwParkMode"), _ParkModes },
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL },
			{ PARAM_VARSTRING,	_T("lpNonDirAddress"), NULL }
		}
	},

	// TSPI_linePickup(DRV_REQUESTID, HDRVLINE, DWORD, HTAPICALL, LPHDRVCALL, LPCWSTR, LPCWSTR);
	{ _T("TSPI_linePickup"), 7, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL }, 
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_HANDLE,		_T("htCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdCall"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszGroupID"), NULL }
		}
	},

	// TSPI_linePrepareAddToConference(DRV_REQUESTID, HDRVCALL, HTAPICALL, LPHDRVCALL, LPLINECALLPARAMS const);
	{ _T("TSPI_linePrepareAddToConference"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }, 
			{ PARAM_HANDLE,		_T("htCall"), NULL }, 
			{ PARAM_LPHANDLE,	_T("lphdCall"), NULL }, 
			{ PARAM_STRUCT_LINECALLPARAMS, _T("lpCallParams"), NULL }
		}
	},

	// TSPI_lineRedirect(DRV_REQUESTID, HDRVCALL, LPCWSTR, DWORD);
	{ _T("TSPI_lineRedirect"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }, 
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL },
			{ PARAM_DWORD,		_T("dwCountryCode"), NULL }
		}
	},

	// TSPI_lineReleaseUserUserInfo(DRV_REQUESTID, HDRVCALL);
	{ _T("TSPI_lineReleaseUserUserInfo"), 2, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }
		}
	},

	// TSPI_lineRemoveFromConference(DRV_REQUESTID, HDRVCALL);
	{ _T("TSPI_lineRemoveFromConference"), 2, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }
		}
	},

	// TSPI_lineSecureCall(DRV_REQUESTID, HDRVCALL);
	{ _T("TSPI_lineSecureCall"), 2, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }
		}
	},

	// TSPI_lineSelectExtVersion(HDRVLINE, DWORD);
	{ _T("TSPI_lineSelectExtVersion"), 2, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("dwExtVersion"), NULL }
		}
	},

	// TSPI_lineSendUserUserInfo(DRV_REQUESTID, HDRVCALL, LPCSTR, DWORD);
	{ _T("TSPI_lineSendUserUserInfo"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCVOID,	_T("lpsUserUserInfo"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineSetAppSpecific(HDRVCALL, DWORD);
	{ _T("TSPI_lineSetAppSpecific"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_DWORD,		_T("dwAppSpecific"), NULL }
		}
	},

	// TSPI_lineSetCallData(DRV_REQUESTID, HDRVCALL, LPVOID, DWORD);
	{ _T("TSPI_lineSetCallData"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCVOID,	_T("lpCallData"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineSetCallParams(DRV_REQUESTID, HDRVCALL, DWORD, DWORD, DWORD, LPLINEDIALPARAMS const);
	{ _T("TSPI_lineSetCallParams"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwBearerMode"), _BearerModes },
			{ PARAM_DWORD,		_T("dwMinRate"), NULL },
			{ PARAM_DWORD,		_T("dwMaxRate"), NULL },
			{ PARAM_STRUCT_LINEDIALPARAMS, _T("lpDialParams"), NULL }
		}
	},

	// TSPI_lineSetCallQualityOfService(DRV_REQUESTID, HDRVCALL, LPVOID, DWORD, LPVOID, DWORD);
	{ _T("TSPI_lineSetCallQualityOfService"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_LPCVOID,	_T("lpSendingFlowSpec"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL },
			{ PARAM_LPCVOID,	_T("lpReceivingFlowSpec"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineSetCallTreatment(DRV_REQUESTID, HDRVCALL, DWORD);
	{ _T("TSPI_lineSetCallTreatment"), 3, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwCallTreatment"), _CallTreatments }
		}
	},

	// TSPI_lineSetCurrentLocation(DWORD);
	{ _T("TSPI_lineSetCurrentLocation"), 1, {
			{ PARAM_DWORD,		_T("dwLocation"), NULL },
		}
	},

	// TSPI_lineSetDefaultMediaDetection(HDRVLINE,DWORD);
	{ _T("TSPI_lineSetDefaultMediaDetection"), 2, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_BITMASK,		_T("dwMediaModes"), _MediaModes }
		}
	},

	// TSPI_lineSetDevConfig(DWORD, LPVOID const, DWORD, LPCWSTR);
	{ _T("TSPI_lineSetDevConfig"), 4, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPCVOID,	_T("lpDeviceConfig"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDeviceClass"), NULL }
		}
	},

	// TSPI_lineSetLineDevStatus(DRV_REQUESTID, HDRVLINE, DWORD, DWORD);
	{ _T("TSPI_lineSetLineDevStatus"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_BITMASK,	_T("dwStatusToChange"), _DevStatusFlags },
			{ PARAM_BOOL,		_T("fStatus"), NULL }
		}
	},

	// TSPI_lineSetMediaControl(HDRVLINE, DWORD, HDRVCALL, DWORD, LPLINEMEDIACONTROLDIGIT const, DWORD, LPLINEMEDIACONTROLMEDIA const, DWORD, LPLINEMEDIACONTROLTONE const, DWORD, LPLINEMEDIACONTROLCALLSTATE const, DWORD);
	{ _T("TSPI_lineSetMediaControl"), 12, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwSelect"), _LineIDSelect },
			{ PARAM_STRUCT_LINEMEDIACONTROLDIGIT, _T("lpDigitList"), NULL },
			{ PARAM_DWORD,		_T("dwDigitNumEntries"), NULL },
			{ PARAM_STRUCT_LINEMEDIACONTROLMEDIA, _T("lpMediaList"), NULL },
			{ PARAM_DWORD,		_T("dwMediaNumEntries"), NULL },
			{ PARAM_STRUCT_LINEMEDIACONTROLTONE, _T("lpToneList"), NULL },
			{ PARAM_DWORD,		_T("dwToneNumEntries"), NULL },
			{ PARAM_STRUCT_LINEMEDIACONTROLCALLSTATE, _T("lpCallStateList"), NULL },
			{ PARAM_DWORD,		_T("dwCallStateNumEntries"), NULL }
		}
	},

	// TSPI_lineSetMediaMode(HDRVCALL,DWORD);
	{ _T("TSPI_lineSetMediaMode"), 2, {
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwMediaMode"), _MediaModes }
		}
	},

	// TSPI_lineSetStatusMessages(HDRVLINE, DWORD);
	{ _T("TSPI_lineSetStatusMessages"), 2, {
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_BITMASK,	_T("dwLineStates"), _DevStates }
		}
	},

	// TSPI_lineSetTerminal(DRV_REQUESTID, HDRVLINE, DWORD, HDRVCALL, DWORD, DWORD, DWORD, DWORD);
	{ _T("TSPI_lineSetTerminal"), 8, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_BITMASK,	_T("dwSelect"), _LineIDSelect },
			{ PARAM_BITMASK,	_T("dwTerminalModes"), _TermModes },
			{ PARAM_DWORD,		_T("dwTerminalID"), NULL },
			{ PARAM_BOOL,		_T("bEnable"), NULL }
		}
	},

	// TSPI_lineSetupConference(DRV_REQUESTID, HDRVCALL, HDRVLINE, HTAPICALL, LPHDRVCALL, HTAPICALL, LPHDRVCALL, DWORD, LPLINECALLPARAMS const);
	{ _T("TSPI_lineSetupConference"), 9, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_HANDLE,		_T("htConfCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdConfCall"), NULL },
			{ PARAM_HANDLE,		_T("htConsultCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdConsultCall"), NULL },
			{ PARAM_SIZE,		_T("dwNumParties"), NULL },
			{ PARAM_STRUCT_LINECALLPARAMS, _T("lpCallParams"), NULL }
		}
	},

	// TSPI_lineSetupTransfer(DRV_REQUESTID, HDRVCALL, HTAPICALL, LPHDRVCALL, LPLINECALLPARAMS const);
	{ _T("TSPI_lineSetupTransfer"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL },
			{ PARAM_HANDLE,		_T("htConsultCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdConsultCall"), NULL },
			{ PARAM_STRUCT_LINECALLPARAMS, _T("lpCallParams"), NULL }
		}
	},

	// TSPI_lineSwapHold(DRV_REQUESTID, HDRVCALL, HDRVCALL);
	{ _T("TSPI_lineSwapHold"), 3, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdActiveCall"), NULL },
			{ PARAM_HANDLE,		_T("hdHeldCall"), NULL }
		}
	},

	// TSPI_lineUncompleteCall(DRV_REQUESTID, HDRVLINE, DWORD);
	{ _T("TSPI_lineUncompleteCall"), 3, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("dwCompletionID"), NULL }
		}
	},

	// TSPI_lineUnhold(DRV_REQUESTID, HDRVCALL);
	{ _T("TSPI_lineUnhold"), 2, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdCall"), NULL }
		}
	},

	// TSPI_lineUnpark(DRV_REQUESTID, HDRVLINE, DWORD, HTAPICALL, LPHDRVCALL, LPCWSTR);
	{ _T("TSPI_lineUnpark"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_HANDLE,		_T("htCall"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdCall"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDestAddress"), NULL }
		}
	},

	// TSPI_phoneClose(HDRVPHONE);
	{ _T("TSPI_phoneClose"), 1, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }
		}
	},

	// TSPI_phoneDevSpecific(DRV_REQUESTID, HDRVPHONE, LPVOID, DWORD);
	{ _T("TSPI_phoneDevSpecific"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_LPVOID_IO,	_T("lpParams"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_phoneGetButtonInfo(HDRVPHONE, DWORD, LPPHONEBUTTONINFO);
	{ _T("TSPI_phoneGetButtonInfo"), 3, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwButtonLampID"), NULL },
			{ PARAM_STRUCT_PHONEBUTTONINFO_OUT, _T("lpButtonInfo"), NULL }
		}
	},

	// TSPI_phoneGetData(HDRVPHONE, DWORD, LPVOID, DWORD);
	{ _T("TSPI_phoneGetData"), 4, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwDataID"), NULL },
			{ PARAM_LPVOID,		_T("lpData"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_phoneGetDevCaps(DWORD, DWORD, DWORD, LPPHONECAPS);
	{ _T("TSPI_phoneGetDevCaps"), 4, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwExtVersion"), NULL },
			{ PARAM_STRUCT_PHONEDEVCAPS, _T("lpPhoneDevCaps"), NULL },
		}
	},

	// TSPI_phoneGetDisplay(HDRVPHONE, LPVARSTRING);
	{ _T("TSPI_phoneGetDisplay"), 2, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_VARSTRING, _T("lpDisplay"), NULL }
		}
	},

	// TSPI_phoneGetExtensionID(DWORD, DWORD, LPPHONEEXTENSIONID);
	{ _T("TSPI_phoneGetExtensionID"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_STRUCT_PHONEEXTENSIONID, _T("lpExtensionID"), NULL }
		}
	},

	// TSPI_phoneGetGain(HDRVPHONE, DWORD, LPDWORD);
	{ _T("TSPI_phoneGetGain"), 3, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_BITMASK,	_T("dwHookSwitchDev"), _HookswitchDevs},
			{ PARAM_LPDWORD,	_T("lpdwGain"), NULL }
		}
	},

	// TSPI_phoneGetHookSwitch(HDRVPHONE, LPDWORD);
	{ _T("TSPI_phoneGetHookSwitch"), 2, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_LPBITMASK,	_T("lpdwHookSwitchDevs"), _HookswitchDevs}
		}
	},

	// TSPI_phoneGetIcon(DWORD, LPCWSTR, LPHICON);
	{ _T("TSPI_phoneGetIcon"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDeviceClass"), NULL },
			{ PARAM_LPHANDLE,	_T("lphIcon"), NULL }
		}
	},

	// TSPI_phoneGetID(HDRVPHONE, LPVARSTRING, LPCWSTR, HANDLE);
	{ _T("TSPI_phoneGetID"), 4, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_VARSTRING,	_T("lpDeviceID"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszDeviceClass"), NULL },
			{ PARAM_HPROCESS,   _T("hTargetProces"), NULL }
		}
	},

	// TSPI_phoneGetLamp(HDRVPHONE, DWORD, LPDWORD);
	{ _T("TSPI_phoneGetLamp"), 3, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwButtonLampID"), NULL },
			{ PARAM_LPBITMASK,	_T("lpdwLampMode"), _LampModes }
		}
	},

	// TSPI_phoneGetRing(HDRVPHONE, LPDWORD, LPDWORD);
	{ _T("TSPI_phoneGetRing"), 3, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_LPDWORD,	_T("lpdwRingMode"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwVolume"), NULL }
		}
	},

	// TSPI_phoneGetStatus(HDRVPHONE, LPPHONESTATUS);
	{ _T("TSPI_phoneGetStatus"), 2, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_STRUCT_PHONESTATUS, _T("lpPhoneStatus"), NULL }
		}
	},

	// TSPI_phoneGetVolume(HDRVPHONE, DWORD, LPDWORD);
	{ _T("TSPI_phoneGetVolume"), 3, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_BITMASK,	_T("dwHookSwitchDev"), _HookswitchDevs },
			{ PARAM_LPDWORD,	_T("lpdwVolume"), NULL }
		}
	},

	// TSPI_phoneNegotiateExtVersion(DWORD, DWORD, DWORD, DWORD, LPDWORD);
	{ _T("TSPI_phoneNegotiateExtVersion"), 5, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwLoVersion"), NULL },
			{ PARAM_DWORD,		_T("dwHiVersion"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwExtVersion"), NULL }
		}
	},

	// TSPI_phoneNegotiateTSPIVersion(DWORD, DWORD, DWORD, LPDWORD);
	{ _T("TSPI_phoneNegotiateTSPIVersion"), 4, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwLoVersion"), NULL },
			{ PARAM_DWORD,		_T("dwHiVersion"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwTSPIVersion"), NULL }
		}
	},

	// TSPI_phoneOpen(DWORD, HTAPIPHONE, LPDRVPHONE, DWORD, PHONEEVENT);
	{ _T("TSPI_phoneOpen"), 5, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("htPhone"), NULL },
			{ PARAM_LPHANDLE,	_T("lphdPhone"), NULL },
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_FUNCPTR,	_T("lpfnEventProc"), NULL }
		}
	},

	// TSPI_phoneSelectExtVersion(HDRVPHONE, DWORD);
	{ _T("TSPI_phoneSelectExtVersion"), 2, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwExtVersion"), NULL }
		}
	},

	// TSPI_phoneSetButtonInfo(DRV_REQUESTID, HDRVPHONE, DWORD, LPPHONEBUTTONINFO const);
	{ _T("TSPI_phoneSetButtonInfo"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwButtonLampID"), NULL },
			{ PARAM_STRUCT_PHONEBUTTONINFO_IN, _T("lpButtonInfo"), NULL }
		}
	},

	// TSPI_phoneSetData(DRV_REQUESTID, HDRVPHONE, DWORD, LPVOID const, DWORD);
	{ _T("TSPI_phoneSetData"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwDataID"), NULL },
			{ PARAM_LPCVOID,	_T("lpData"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_phoneSetDisplay(DRV_REQUESTID, HDRVPHONE, DWORD, DWORD, LPCWSTR, DWORD);
	{ _T("TSPI_phoneSetDisplay"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwRow"), NULL },
			{ PARAM_DWORD,		_T("dwColumn"), NULL },
			{ PARAM_LPCVOID,	_T("lpsDisplay"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_phoneSetGain(DRV_REQUESTID, HDRVPHONE, DWORD, DWORD);
	{ _T("TSPI_phoneSetGain"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_BITMASK,	_T("dwHookSwitchDev"), _HookswitchDevs },
			{ PARAM_DWORD,		_T("dwGain"), NULL }
		}
	},

	// TSPI_phoneSetHookSwitch(DRV_REQUESTID, HDRVPHONE, DWORD, DWORD);
	{ _T("TSPI_phoneSetHookSwitch"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_BITMASK,	_T("dwHookSwitchDev"), _HookswitchDevs },
			{ PARAM_BITMASK,	_T("dwHookSwitchMode"), _HookswitchModes }
		}
	},

	// TSPI_phoneSetLamp(DRV_REQUESTID, HDRVPHONE, DWORD, DWORD);
	{ _T("TSPI_phoneSetLamp"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwButtonLampID"), NULL },
			{ PARAM_BITMASK,	_T("dwLampMode"), _LampModes }
		}
	},

	// TSPI_phoneSetRing(DRV_REQUESTID, HDRVPHONE, DWORD, DWORD);
	{ _T("TSPI_phoneSetRing"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_DWORD,		_T("dwRingMode"), NULL },
			{ PARAM_DWORD,		_T("dwVolume"), NULL }
		}
	},

	// TSPI_phoneSetStatusMessages(HDRVPHONE, DWORD, DWORD, DWORD);
	{ _T("TSPI_phoneSetStatusMessages"), 4, {
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_BITMASK,	_T("dwPhoneStates"), _PhoneStates },
			{ PARAM_BITMASK,	_T("dwButtonModes"), _ButtonModes },
			{ PARAM_BITMASK,	_T("dwButtonStates"), _ButtonStates }
		}
	},

	// TSPI_phoneSetVolume(DRV_REQUESTID, HDRVPHONE, DWORD, DWORD);
	{ _T("TSPI_phoneSetVolume"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_HANDLE,		_T("hdPhone"), NULL }, 
			{ PARAM_BITMASK,	_T("dwHookSwitchDev"), _HookswitchDevs },
			{ PARAM_DWORD,		_T("dwVolume"), NULL }
		}
	},

	// TSPI_providerCreateLineDevice(DWORD, DWORD);
	{ _T("TSPI_providerCreateLineDevice"), 2, {
			{ PARAM_DWORD,		_T("dwTempID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL }
		}
	},

	// TSPI_providerCreatePhoneDevice(DWORD, DWORD);
	{ _T("TSPI_providerCreatePhoneDevice"), 2, {
			{ PARAM_DWORD,		_T("dwTempID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL }
		}
	},

	// TSPI_providerEnumDevices(DWORD, LPDWORD, LPDWORD, HPROVIDER, LINEEVENT, PHONEEVENT);
	{ _T("TSPI_providerEnumDevices"), 6, {
			{ PARAM_DWORD,		_T("dwPermanentProviderID"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwNumLines"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwNumPhones"), NULL },
			{ PARAM_HANDLE,		_T("hProvider"), NULL },
			{ PARAM_FUNCPTR,	_T("lpfnLineCreateProc"), NULL },
			{ PARAM_FUNCPTR,	_T("lpfnPhoneCreateProc"), NULL }
		}
	},

	// TSPI_providerFreeDialogInstance(HDRVDIALOGINSTANCE);
	{ _T("TSPI_providerFreeDialogInstance"), 1, {
			{ PARAM_HANDLE,		_T("hdDlgInst"), NULL }
		}
	},

	// TSPI_providerGenericDialogData(DWORD, DWORD, LPVOID, DWORD);
	{ _T("TSPI_providerGenericDialogData"), 4, {
			{ PARAM_DWORD,		_T("dwObjectID"), NULL },
			{ PARAM_INDEX,		_T("dwObjectType"), _ObjectTypes },
			{ PARAM_LPCVOID,	_T("lpParams"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_providerInit(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, ASYNCH_COMPLETION, LPDWORD);
	{ _T("TSPI_providerInit"), 8, {
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwPermanentProviderID"), NULL },
			{ PARAM_DWORD,		_T("dwLineDeviceIDBase"), NULL },
			{ PARAM_DWORD,		_T("dwPhoneDeviceIDBase"), NULL },
			{ PARAM_DWORD,		_T("dwNumLines"), NULL },
			{ PARAM_DWORD,		_T("dwNumPhones"), NULL },
			{ PARAM_FUNCPTR,	_T("lpfnCompletionProc"), NULL },
			{ PARAM_LPBITMASK,	_T("lpdwTSPIOptions"), _TSPIOptions }
		}
	},

	// TSPI_providerShutdown(DWORD, DWORD);
	{ _T("TSPI_providerShutdown"), 2, {
			{ PARAM_DWORD,		_T("dwTSPIVersion"), NULL },
			{ PARAM_DWORD,		_T("dwPermanentProviderID"), NULL }
		}
	},

	// TSPI_providerUIIdentify(LPWSTR);
	{ _T("TSPI_providerUIIdentify"), 1, {
			{ PARAM_LPWSTR,		_T("lpszUIDLLName"), NULL }
		}
	},

	// TSPI_lineSetAgentGroup(DRV_REQUESTID, DWORD, DWORD, LPLINEAGENTGROUPLIST)
	{ _T("TSPI_lineSetAgentGroup"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEAGENTGROUPLIST_IN, _T("lpAgentGroupList"), NULL }
		}
	},

	// TSPI_lineSetAgentState(DRV_REQUESTID, DWORD, DWORD, DWORD, DWORD)
	{ _T("TSPI_lineSetAgentState"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_DWORD,		_T("dwAgentState"), NULL },
			{ PARAM_DWORD,		_T("dwNextAgentState"), NULL }
		}
	},
	
	// TSPI_lineSetAgentActivity(DRV_REQUESTID, DWORD, DWORD, DWORD)
	{ _T("TSPI_lineSetAgentActivity"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_DWORD,		_T("dwActivityID"), NULL }
		}
	},
	
	// TSPI_lineGetAgentStatus(DRV_REQUESTID, DWORD, DWORD, LPLINEAGENTSTATUS)
	{ _T("TSPI_lineGetAgentStatus"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEAGENTSTATUS, _T("lpAgentStatus"), NULL }
		}
	},
	
	// TSPI_lineGetAgentCaps(DRV_REQUESTID, DWORD, DWORD, LPLINEAGENTCAPS)
	{ _T("TSPI_lineGetAgentCaps"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEAGENTCAPS, _T("lpAgentCaps"), NULL }
		}
	},
	
	// TSPI_lineGetAgentActivityList(DRV_REQUESTID, DWORD, DWORD, LPLINEAGENTACTIVITYLIST)
	{ _T("TSPI_lineGetAgentActivityList"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEAGENTACTIVITYLIST, _T("lpAgentActivityList"), NULL }
		}
	},
	
	// TSPI_lineGetAgentGroupList(DRV_REQUESTID, DWORD, DWORD, LPLINEAGENTGROUPLIST)
	{ _T("TSPI_lineGetAgentGroupList"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_STRUCT_LINEAGENTGROUPLIST_OUT, _T("lpAgentGroupList"), NULL }
		}
	},

	// TSPI_lineAgentSpecific(DRV_REQUESTID, DWORD, DWORD, DWORD, LPVOID, DWORD)
	{ _T("TSPI_lineAgentSpecific"), 6, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_DWORD,		_T("dwAgentExtensionID"), NULL },
			{ PARAM_LPVOID_IO,	_T("lpParams"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// TSPI_lineCreateAgent(DRV_REQUESTID dwRequestID, DWORD dwDeviceID, LPHAGENT lphAgent, LPCWSTR pszMachineName, LPCWSTR pszUserName, LPCWSTR pszAgentID, LPCWSTR pszAgentPIN)
	{ _T("TSPI_lineCreateAgent"), 7, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPHANDLE,	_T("lphAgent"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszMachineName"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszUserName"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszAgentName"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszAgentPIN"), NULL }
		}
	},

	// TSPI_lineSetAgentMeasurementPeriod(DRV_REQUESTID dwRequestID, DWORD dwDeviceID, HAGENT hAgent, DWORD dwMeasurementPeriod)
	{ _T("TSPI_lineSetAgentMeasurementPeriod"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("hAgent"), NULL },
			{ PARAM_DWORD,		_T("dwMeasurementPeriod"), NULL }
		}
	},

	// TSPI_lineGetAgentInfo(DWORD dwDeviceID, HAGENT hAgent, LPLINEAGENTINFO lpAgentInfo)
	{ _T("TSPI_lineGetAgentInfo"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("hAgent"), NULL },
			{ PARAM_STRUCT_LINEAGENTINFO, _T("lpAgentInfo"), NULL }
		}
	},

	// TSPI_lineCreateAgentSession(DRV_REQUESTID dwRequestID, DWORD dwDeviceID, LPHAGENTSESSION lphSession, HAGENT hAgent, LPCWSTR pszAgentPIN, const GUID* pGUID, DWORD dwWorkingAddressID)
	{ _T("TSPI_lineCreateAgentSession"), 7, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPHANDLE,	_T("lphAgentSession"), NULL },
			{ PARAM_HANDLE,		_T("hAgent"), NULL },
			{ PARAM_LPCWSTR,	_T("lpszAgentPIN"), NULL },
			{ PARAM_LPGUID,		_T("pGuid"), NULL },
			{ PARAM_DWORD,		_T("dwWorkingAddressID"), NULL }
		}
	},

	// TSPI_lineGetAgentSessionList(DWORD dwDeviceID, HAGENT hAgent, LPLINEAGENTSESSIONLIST lpSessionList)
	{ _T("TSPI_lineGetAgentSessionList"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("hAgent"), NULL },
			{ PARAM_STRUCT_AGENTSESSIONLIST, _T("lpSessionList"), NULL }
		}
	},

	// TSPI_lineSetAgentSessionState(DRV_REQUESTID dwRequestID, DWORD dwDeviceID, HAGENTSESSION hSession, DWORD dwAgentSessionState, DWORD dwNextAgentSessionState)
	{ _T("TSPI_lineSetAgentSessionState"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("hAgentSession"), NULL },
			{ PARAM_DWORD,		_T("dwState"), NULL },
			{ PARAM_DWORD,		_T("dwNextState"), NULL },
		}
	},

	// TSPI_lineGetAgentSessionInfo(DWORD dwDeviceID, HAGENTSESSION hAgentSession, LPLINEAGENTSESSIONINFO lpSessionInfo)
	{ _T("TSPI_lineGetAgentSessionInfo"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("hAgentSession"), NULL },
			{ PARAM_STRUCT_AGENTSESSIONINFO, _T("lpAgentSessionInfo"), NULL },
		}
	},

	// TSPI_lineGetQueueList(DWORD dwDeviceID, const GUID* pGroupID, LPLINEQUEUELIST lpQueueList)
	{ _T("TSPI_lineGetQueueList"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPGUID,		_T("pGroupID"), NULL },
			{ PARAM_STRUCT_LINEQUEUELIST, _T("lpAgentQueueList"), NULL },
		}
	},

	// TSPI_lineSetQueueMeasurementPeriod(DRV_REQUESTID dwRequestID, DWORD dwDeviceID, DWORD dwQueueID, DWORD dwMeasurementPeriod)
	{ _T("TSPI_lineSetQueueMeasurementPeriod"), 4, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwQueueID"), NULL },
			{ PARAM_DWORD,		_T("dwMeasurementPeriod"), NULL }
		}
	},

	// TSPI_lineGetQueueInfo(DWORD dwDeviceID, DWORD dwQueueID, LPLINEQUEUEINFO lpQueueInfo)
	{ _T("TSPI_lineGetQueueInfo"), 3, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_DWORD,		_T("dwQueueID"), NULL },
			{ PARAM_STRUCT_QUEUEINFO,	_T("lpQueueInfo"), NULL }
		}
	},

	// TSPI_lineGetGroupList(DWORD dwDeviceID, LPLINEAGENTGROUPLIST lpGroupList)
	{ _T("TSPI_lineGetGroupList"), 2, {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_STRUCT_AGENTGROUPLIST, _T("lpGroupList"), NULL }
		}
	},

	// TSPI_lineSetAgentStateEx(DRV_REQUESTID dwRequestID, DWORD dwDeviceID, HAGENT hAgent, DWORD dwState, DWORD dwNextState)
	{ _T("TSPI_lineSetAgentStateEx"), 5, {
			{ PARAM_DWORD,		_T("DRV_REQUESTID"), NULL },
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_HANDLE,		_T("hAgent"), NULL },
			{ PARAM_DWORD,		_T("dwState"), NULL },
			{ PARAM_DWORD,		_T("dwNextState"), NULL },
		}
	},

	// TSPI_lineGetCallHubTracking(HDRVLINE, LPLINECALLHUBTRACKINGINFO)
	{ _T("TSPI_lineGetCallHubTracking"), 2, {
			{ PARAM_DWORD,		_T("hdLine"), NULL },
			{ PARAM_STRUCT_CALLHUBTACKINGINFO_OUT, _T("lpCallHubInfo"), NULL }
		}
	},

	// TSPI_lineSetCallHubTracking(HDRVLINE, LPLINECALLHUBTRACKINGINFO)
	{ _T("TSPI_lineSetCallHubTracking"), 2, {
			{ PARAM_DWORD,		_T("hdLine"), NULL },
			{ PARAM_STRUCT_CALLHUBTACKINGINFO_IN, _T("lpCallHubInfo"), NULL }
		}
	},

	// TSPI_lineGetCallIDs(HDRVCALL, LPDWORD, LPDWORD, LPDWORD)
	{ _T("TSPI_lineGetCallIDs"), 4, {
			{ PARAM_DWORD,		_T("hdCall"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwAddressID"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwCallID"), NULL },
			{ PARAM_LPDWORD,	_T("lpdwRelatedCallID"), NULL }
		}
	},

	// TSPI_lineMSPIdentify(DWORD dwDeviceID, GUID* pGUID)
	{ _T("TSPI_lineMSPIdentify"), 2,  {
			{ PARAM_DWORD,		_T("dwDeviceID"), NULL },
			{ PARAM_LPGUID,		_T("pGuid"), NULL }
		}
	},

	// TSPI_lineCreateMSPInstance(HDRVLINE, DWORD, HTAPIMSPLINE, LPHDRVMSPLINE)
	{ _T("TSPI_lineCreateMSPInstance"), 4, {
			{ PARAM_DWORD,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("dwAddressID"), NULL },
			{ PARAM_DWORD,		_T("htMSPLine"), NULL },
			{ PARAM_LPDWORD,	_T("lphdMSPLine"), NULL },
		}
	},

	// TSPI_lineCloseMSPInstance(HDRVMSPLINE)
	{ _T("TSPI_lineCloseMSPInstance"), 1, {
			{ PARAM_DWORD,		_T("hdMSPLine"), NULL },
		}
	},

	// TSPI_lineReceiveMSPData(HDRVLINE, HDRVCALL, HDRVMSPLINE, LPVOID, DWORD)
	{ _T("TSPI_lineReceiveMSPData"), 5, {
			{ PARAM_DWORD,		_T("hdLine"), NULL },
			{ PARAM_DWORD,		_T("hdCall"), NULL },
			{ PARAM_DWORD,		_T("hdMSPLine"), NULL },
			{ PARAM_LPCVOID,	_T("lpData"), NULL },
			{ PARAM_SIZE,		_T("dwSize"), NULL }
		}
	},

	// End of table marker
	{ NULL, 0 }
};
