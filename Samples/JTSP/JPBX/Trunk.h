/*******************************************************************/
//
// TRUNK.H
//
// Trunk object support (superset of CLineDevice)
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

#ifndef __JPBX_TRUNK_INC__
#define __JPBX_TRUNK_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "line.h"

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
const UINT MAX_TRUNK_COUNT = 100;	// Max # of allowed trunk lines

/**************************************************************************
** CTrunk
**
** Trunk line device - end-point for a call outside the switch
**
***************************************************************************/
class CTrunk : public CLineDevice
{
// Class data
public:
	enum TrunkState {
		Offline = 0,
		Inbound,
		Outbound,
		Unused
	};
	static int g_nCPS;			// Calls per second incoming
	static DWORD g_dwTickCount;	// Timer for above
protected:
	TrunkState m_tsState;	// Current state of the trunk

// Public Constructor
public:
	CTrunk(DWORD dwTrunkID);
	virtual ~CTrunk();

// Serializable constructor
protected:
	DECLARE_SERIAL(CTrunk)
	CTrunk();

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Class methods
public:
	TrunkState GetTrunkState() const;
	CString GetTrunkStateString() const;
	CCall* GenerateIncomingCall(const CNetInfo& cidANI, const CNetInfo& cidDNIS);
	void Online(bool fOnline=true);

// Methods from CLineDevice
public:
	virtual bool CanAddCall();
	virtual void OnCallCreated(CCall* pCall, bool fPlaceCall);
	virtual void OnCallDestroyed(CCall* pCall, bool fNotifyRelease);
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual void OnTimer();

// Creation dialog
public:
	static CTrunk* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
};

/*****************************************************************************
** Procedure:  CTrunk::GetTrunkState
**
** Arguments: void
**
** Returns: Current state of the trunk
**
** Description: This returns the state of the trunk
**
*****************************************************************************/
inline CTrunk::TrunkState CTrunk::GetTrunkState() const
{
	return m_tsState;

}// CTrunk::GetTrunkState

#endif // __JPBX_TRUNK_INC__
