/*******************************************************************/
//
// ROUTEPOINT.H
//
// Route-point management
//
// Copyright (C) 1998 JulMar Technology, Inc.
// All rights reserved
//
// TSP++ Version 3.00 PBX/ACD Emulator Projects
// Internal Source Code - Do Not Release
//
// Modification History
// --------------------
// 1998/09/05 MCS@JulMar	Initial revision
//
/*******************************************************************/

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_ROUTEPOINT_INC__
#define __JPBX_ROUTEPOINT_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "line.h"

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CTrunk;

/**************************************************************************
** CRoutePoint
**
** Route point class definition
**
***************************************************************************/
class CRoutePoint : public CLineDevice
{
// Class data
public:
	enum Command {
		DoNothing = 0,
		TransferTo,
		Drop
	};

protected:
	WORD m_wSeconds;			// Seconds to wait before auto-transfer
	DWORD m_dwQueue;			// Queue or station to transfer to
	BOOL m_fAllowRoute;			// Allow programmatic routes
	BOOL m_fAutoRoute;			// Auto route
	CDWordArray m_arrTrunks;	// Trunks which we monitor on this RP.
	Command m_rpCommand;		// Command to execute

// Public constructor/destructor
public:
	CRoutePoint(DWORD dwRPID, LPCTSTR pszDescription);
	virtual ~CRoutePoint();

// Constructor
protected:
	DECLARE_SERIAL(CRoutePoint)
	CRoutePoint();

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Access functions
public:
	bool WantIncomingTrunkCall(CTrunk* pTrunk, CCall* pCall);

// Methods from CLineDevice
public:
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual void OnTimer();

// Creation dialog
public:
	static CRoutePoint* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
};

#endif // __JPBX_ROUTEPOINT_INC__
