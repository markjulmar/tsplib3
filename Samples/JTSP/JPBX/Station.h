/*******************************************************************/
//
// STATION.H
//
// Station emulator object
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

#ifndef __JPBX_STATION_INC__
#define __JPBX_STATION_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "agent.h"
#include "agentgroup.h"
#include "line.h"
#include "call.h"

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CLamp;

/**************************************************************************
** CStation
**
** This object represents a single phone/line station
**
***************************************************************************/
class CStation : public CLineDevice
{
// Class data
public:
	enum LampId {
		Ready = 0,
		CallWork
	};

protected:
	CAgent* m_pAgent;			// Agent that is currently logged onto station
	CDWordArray m_arrGroups;	// Groups which may log onto this station
	CTypedPtrArray<CObArray, CLamp*> m_arrLamps;
	int m_iVolume;				// Volume for the station
	int m_iGain;				// Gain for the station

// Public Constructor
public:
	CStation(DWORD dwExtension, LPCTSTR pszDescription);
	virtual ~CStation();

// Serializable constructor
protected:
	DECLARE_SERIAL(CStation)
	CStation();

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Access functions
public:
	bool LogOnAgent(CAgent* pAgent, DWORD dwGroup1, DWORD dwGroup2=0);
	CAgent* GetAgent();
	void BuildDisplay();
	bool SetGain(int iGain);
	bool SetVolume(int iVolume);

// Methods from CLineDevice
public:
	virtual bool CanAddCall();
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual bool TransferCall(CCall* pCall_Consult, CCall* pCall_Source);
	virtual void OnCallStateChange(CCall* pCall);
	virtual void OnCallCreated(CCall* pCall, bool fPlaceCall);
	virtual void OnCallDestroyed(CCall* pCall, bool fNotifyRelease);

// Events
public:
	void OnAgentStateChanged();

// Creation dialog
public:
	static CStation* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
};	

/**************************************************************************
** CLamp
**
** This object represents a lamp on the phone station
**
***************************************************************************/
class CLamp : public CObject
{
// Class data
protected:
	CStation* m_pStation;	// Station pointer
	BYTE m_bLampID;			// Lamp id
	bool m_fIsLit;			// TRUE if lit

// Constructor
public:
	CLamp(CStation* pStation, int iLampID) { 
		m_pStation = pStation; m_bLampID = static_cast<BYTE>(iLampID); 
	}

// Methods
public:
	bool IsLit() const { 
		return m_fIsLit; 
	}
	void SetState(bool fLit) { 
		if (m_fIsLit != fLit)
		{
			m_fIsLit = fLit;
			SendEvent(m_pStation->GetLineID(), PBXEVT_PLC, m_pStation->GetExtension(), 
				static_cast<int>(m_bLampID), (IsLit()) ? 'S' : 'D');
		}
	}
};

/*****************************************************************************
** Procedure:  CStation::GetAgent
**
** Arguments: void
**
** Returns: Pointer to current agent
**
** Description: This allows access to the currently logged on agent.
**
*****************************************************************************/
inline CAgent* CStation::GetAgent()
{
	return m_pAgent;

}// CStation::GetAgent

/*****************************************************************************
** Procedure:  CStation::SetGain
**
** Arguments: 'iGain' - New setting for gain
**
** Returns: void
**
** Description: This changes the gain setting
**
*****************************************************************************/
inline bool CStation::SetGain(int iGain)
{
	if (iGain >= 0x100 && iGain <= 0xffff)
	{
		m_iGain = iGain;
		SendEvent(GetLineID(), PBXEVT_PGC, GetExtension(), iGain);
		return true;
	}
	return false;

}// CStation::SetGain

/*****************************************************************************
** Procedure:  CStation::SetVolume
**
** Arguments: 'iVolume' - New setting for volume
**
** Returns: void
**
** Description: This changes the volume setting
**
*****************************************************************************/
inline bool CStation::SetVolume(int iVolume)
{
	if (iVolume >= 0 && iVolume <= 0xffff)
	{
		m_iVolume = iVolume;
		SendEvent(GetLineID(), PBXEVT_PVC, GetExtension(), iVolume);
		return true;
	}
	return false;

}// CStation::SetVolume

#endif // __JPBX_STATION_INC__
