/*****************************************************************************/
//
// ACDPROXY.H - Main include file for the JTAProxy application
//                                                                        
// Copyright (C) 1998-2002 JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
//                                                                        
// This file contains the main include file for the ACD proxy.
//
// This source code is intended only as a supplement to the
// TSP++ Class Library product documentation.  This source code cannot 
// be used in part or whole in any form outside the TSP++ library.
// 
/*****************************************************************************/

#ifndef _ACDPROXY_INC_
#define _ACDPROXY_INC_

#include "resource.h"       // main symbols
#include <limits.h>

/**************************************************************************************
** CTapiInfo
**
** Object which represents TAPI in our proxy
**
**************************************************************************************/
class CTapiInfo
{
// Class data
protected:
	HLINEAPP m_hLineApp;		// TAPI application handle
	DWORD m_dwNumLines;			// Total number of lines at startup
	DWORD m_dwApiVersion;		// Version to negotiate to.
	DWORD m_dwThreadID;			// Thread ID of our processing thread.
	HANDLE m_hIOCPort;			// Use I/O completion port for NT
	HANDLE m_hThread;			// Processing thread
	CSharedMemMgr m_smMgr;		// Shared-memory connection to TSP
	std::map<HLINE,DWORD> m_mapLineToId;
	std::map<DWORD,HLINE> m_mapIdToLine;	
	std::map<DWORD,DWORD> m_mapIdToProviderID;
// Constructor/Destructor
public:
	CTapiInfo();
	~CTapiInfo();

// Functions
public:
	BOOL Initialize (HINSTANCE hInstance, LPCTSTR pszAppName);
	void Run(HANDLE hevtClose);
	void PullAgentRequests();
	void QueueAgentMessage(HLINE hLine, LPLINEPROXYREQUEST lpProxy);
	BOOL OpenDevice(DWORD dwProviderID, DWORD dwDeviceID, DWORD dwFeatures);
	void CloseDevices(DWORD dwProviderID=-1L);
	void CloseLineDevice(DWORD dwDeviceID);
	DWORD LineToDeviceID(HLINE hLine);
	HLINE DeviceIDToLine(DWORD dwDeviceID);
	DWORD DeviceIDToProviderID(DWORD dwDeviceID);
};

/*****************************************************************************
** Procedure:  CTapiInfo::CTapiInfo
**
** Arguments:  void
**
** Returns:    void
**
** Description:  Constructor for our TAPI object
**
*****************************************************************************/
inline CTapiInfo::CTapiInfo() : m_hLineApp(NULL), m_dwNumLines(0), 
	m_dwApiVersion(TAPI_CURRENT_VERSION), m_dwThreadID(0), 
	m_hIOCPort(NULL)
{
}// CTapiInfo::CTapiInfo

/*****************************************************************************
** Procedure:  CTapiInfo::LineToDeviceID
**
** Arguments:  'hLine' - Line to lookup
**
** Returns:    DWORD index of line in device array
**
** Description:  This function converts between a given hLine and device id.
**
*****************************************************************************/
inline DWORD CTapiInfo::LineToDeviceID(HLINE hLine)
{
	std::map<HLINE,DWORD>::iterator iPos = m_mapLineToId.find(hLine);
	if (iPos != m_mapLineToId.end())
		return (*iPos).second;
	return _UI32_MAX;

}// CTapiInfo::LineToDeviceID

/*****************************************************************************
** Procedure:  CTapiInfo::DeviceIDToLine
**
** Arguments:  'dwDeviceID'
**
** Returns:    HLINE from TAPI
**
** Description:  This function converts between a given device id and an hline.
**
*****************************************************************************/
inline HLINE CTapiInfo::DeviceIDToLine(DWORD dwDeviceID)
{
	std::map<DWORD,HLINE>::iterator iPos = m_mapIdToLine.find(dwDeviceID);
	if (iPos != m_mapIdToLine.end())
		return (*iPos).second;
	return NULL;

}// CTapiInfo::DeviceIDToLine

/*****************************************************************************
** Procedure:  CTapiInfo::DeviceIDToProviderID
**
** Arguments:  'dwDeviceID'
**
** Returns:    ProviderID associated with device.
**
** Description:  This function converts between a given device id and a providerID.
**
*****************************************************************************/
inline DWORD CTapiInfo::DeviceIDToProviderID(DWORD dwDeviceID)
{
	std::map<DWORD,DWORD>::iterator iPos = m_mapIdToProviderID.find(dwDeviceID);
	if (iPos != m_mapIdToProviderID.end())
		return (*iPos).second;
	return NULL;

}// CTapiInfo::DeviceIDToProvideriD

#endif // _ACDPROXY_INC
