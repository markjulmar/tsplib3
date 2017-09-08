/*******************************************************************/
//
// LINE.H
//
// Basic line definition
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

#ifndef __JPBX_LINE_INC__
#define __JPBX_LINE_INC__

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
const UINT LINENAME_SIZE = 100;
const UINT EXTENSION_SIZE = 4;

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "base.h"
#include "call.h"

/*----------------------------------------------------------------------------
	PRE-DEFINITION
-----------------------------------------------------------------------------*/
class CCallListDlg;

/**************************************************************************
** CLineDevice
**
** Basic line device object - all devices are derived from this object.
**
***************************************************************************/
class CLineDevice : public CUIObject
{
// Class data associated with all line devices
protected:
	DWORD m_dwLineID;		// Unique line identifier (extension)
	CString m_strName;		// Name of the line device
	CTypedPtrList<CObList, CCall*> m_lstCalls;
	CCallListDlg* m_pwndSCList;		// Call List window

// Public Constructor
public:
	CLineDevice(DWORD dwLineid, const char* pszName);
	virtual ~CLineDevice();

// Serializable constructor
protected:
	DECLARE_SERIAL(CLineDevice)
	CLineDevice();

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Class methods
public:
	DWORD GetLineID() const;
	const char* GetLineName() const;
	CString GetExtension() const;

// Call management support
public:
	CCall* FindCallByCallID(DWORD dwID);
	int GetCallCount() const;
	CCall* GetCallByPosition(int iPos);
	CCall* GetActiveCall();
	void Delete();
	bool HasConnectedCall() const;
	void ShowCallList();

	virtual bool CanAddCall();
	virtual void OnCallCreated(CCall* pCall, bool fPlaceCall=false);
	virtual void OnCallDestroyed(CCall* pCall, bool fNotifyRelease=false);
	virtual void OnCallStateChange(CCall* pCall);

	virtual CCall* DialCall(CNetInfo& cidDNIS);
	virtual bool TransferCall(CCall* pCall_Consult, CCall* pCall_Source);
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual void OnTimer();

// UI Management from CUIObject
public:
	virtual bool ContextMenu( CWnd* pwndOwner );
	virtual void OnCommand( UINT nCmd );
};

/*****************************************************************************
** Procedure:  CLineDevice::GetLineID
**
** Arguments: void
**
** Returns:    Unique numerical line identifier (extension)
**
** Description:  Returns the internal line identifier
**
*****************************************************************************/
inline DWORD CLineDevice::GetLineID() const
{
	return m_dwLineID;

}// CLineDevice::GetLineID

/*****************************************************************************
** Procedure:  CLineDevice::GetLineName
**
** Arguments: void
**
** Returns:    String name assigned to line
**
** Description:  Returns the internal line name
**
*****************************************************************************/
inline const char* CLineDevice::GetLineName() const
{
	return m_strName;

}// CLineDevice::GetLineName

/*****************************************************************************
** Procedure:  CLineDevice::GetCallCount
**
** Arguments: void
**
** Returns:    Number of calls in line
**
** Description:  Returns the number of calls active on the line
**
*****************************************************************************/
inline int CLineDevice::GetCallCount() const
{
	return m_lstCalls.GetCount();

}// CLineDevice::GetCallCount

/*****************************************************************************
** Procedure:  CLineDevice::GetActiveCall
**
** Arguments: void
**
** Returns: The pointer to the active call call on the line
**
** Description: This locates the first non-held call on the line
**
*****************************************************************************/
inline CCall* CLineDevice::GetActiveCall()
{
	for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
	{
		CCall* pCall = m_lstCalls.GetNext(pos);
		CCall::CallState cs = pCall->GetCallState();
		if (cs != CCall::Unknown && cs != CCall::Holding)
			return pCall;
	}
	return NULL;

}// CLineDevice::GetActiveCall

/*****************************************************************************
** Procedure:  CLineDevice::GetCallByPosition
**
** Arguments: 'iPos' - Index position for call
**
** Returns: The pointer to the given call, NULL if none
**
** Description: This returns a call by position
**
*****************************************************************************/
inline CCall* CLineDevice::GetCallByPosition(int iPos)
{
	for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
	{
		CCall* pCall = m_lstCalls.GetNext(pos);
		if (--iPos < 0)
			return pCall;
	}
	return NULL;

}// CLineDevice::GetCallByPosition

/*****************************************************************************
** Procedure:  CLineDevice::HasConnectedCall
**
** Arguments: void
**
** Returns: true/false if there is an active call on the device
**
** Description: This returns whether there is a connected call
**
*****************************************************************************/
inline bool CLineDevice::HasConnectedCall() const
{
	for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
	{
		CCall* pCall = m_lstCalls.GetNext(pos);
		if (pCall->GetCallState() == CCall::Connected ||
			pCall->GetCallState() == CCall::Busy)
			return true;
	}
	return false;

}// CLineDevice::HasConnectedCall

#endif // __JPBX_LINE_INC__

