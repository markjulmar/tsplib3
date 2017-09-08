/*******************************************************************/
//
// IVRU.H
//
// Interactive voice-response unit definitions
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

#ifndef __JPBX_IVRU_INC__
#define __JPBX_IVRU_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "line.h"
#include "call.h"

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CVRUEvent;

/**************************************************************************
** CVRU
**
** This object represents a single voice-response unit
**
***************************************************************************/
class CVRU : public CLineDevice
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
	Command m_vruCommand;		// Command to execute

// Public Constructor
public:
	CVRU(DWORD dwExtension, LPCTSTR pszDescription);
	virtual ~CVRU();

// Serializable constructor
protected:
	DECLARE_SERIAL(CVRU)
	CVRU();

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Methods from CLineDevice
public:
	virtual bool CanAddCall();
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual void OnTimer();

// Creation dialog
public:
	static CVRU* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
};	

#endif // __JPBX_VRU_INC__
