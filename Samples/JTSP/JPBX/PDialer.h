/*******************************************************************/
//
// PDIALER.H
//
// Predictive dialer support
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

#ifndef __JPBX_PDIALER_INC__
#define __JPBX_PDIALER_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "line.h"

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
const UINT DEFAULT_PDIALERMAX = 30;

/**************************************************************************
** CPDialer
**
** Predictive Dialer class definition
**
***************************************************************************/
class CPDialer : public CLineDevice
{
// Class data
protected:
	enum Command {
		ActBusy = 0,
		ActAnswerVoice,
		ActAnswerMachine,
		ActUnknown,
		ActNoAnswer,
		Random,
		PromptUser
	};
	Command m_cmType;		// What happens to outgoing calls?
	
// Public constructor/destructor
public:
	CPDialer(DWORD dwID, LPCTSTR pszDescription);
	virtual ~CPDialer();

// Constructor
protected:
	CPDialer();
	DECLARE_SERIAL(CPDialer)

// Serialize support
public:
	virtual void Serialize( CArchive& ar );

// Access functions
public:
	void OnBusy(CCall* pCall);
	void OnAnsweredVoice(CCall* pCall, DWORD dwTransferTo);
	void OnAnsweredMachine(CCall* pCall);
	void OnAnsweredUnknown(CCall* pCall, DWORD dwTransferTo);
	void OnNoAnswer(CCall* pCall);

	CCall* DialPredictiveCall(CNetInfo& cidDNIS, int iTimeout=5000, DWORD dwTransferTo=0);

// Methods from CLineDevice
public:
	virtual bool CanAddCall();
	virtual void OnReceiveCall(CCall* pCall_Source);
	virtual bool TransferCall(CCall* pCall_Consult, CCall* pCall_Source);

// Creation dialog
public:
	friend class CCallProgress;
	static CPDialer* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
};

#endif // __JPBX_PDIALER_INC__
