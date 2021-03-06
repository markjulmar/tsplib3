/***************************************************************************
//
// HOLD.CPP
//
// TAPI Service provider for TSP++ version 3.00
// Hold/Unhold support
//
// Copyright (C) 1999 JulMar Technology, Inc.
// All rights reserved
//
// Generated by the TSPWizard (C) JulMar Technology, Inc.
// 
/***************************************************************************/

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "ConfTest.h"

/*****************************************************************************
** Procedure:  CConfTestLine::OnHoldCall
**
** Arguments: 'pReq' - Request object representing this HOLD event 
**            'lpBuff' - The event block object
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineHold processing
**               for this service provider.
**
*****************************************************************************/
bool CConfTestLine::OnHoldCall(RTHold* pRequest, LPCVOID lpBuff)
{
	CTSPICallAppearance* pCall = pRequest->GetCallInfo();
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		pCall->SetCallState(LINECALLSTATE_ONHOLD);
		CompleteRequest(pRequest, 0);
	}
	return false;

}// CConfTestLine::OnHoldCall

/*****************************************************************************
** Procedure:  CConfTestLine::OnRetrieveCall
**
** Arguments: 'pReq' - Request object representing this UNHOLD event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineUnhold processing
**               for this service provider.
**
*****************************************************************************/
bool CConfTestLine::OnRetrieveCall(RTUnhold* pRequest, LPCVOID lpBuff)
{
	CTSPICallAppearance* pCall = pRequest->GetCallInfo();
	if (pRequest->EnterState(STATE_INITIAL, STATE_WAITING))
	{
		pCall->SetCallState(LINECALLSTATE_CONNECTED);
		CompleteRequest(pRequest, 0);
	}
	return false;

}// CConfTestLine::OnRetrieveCall

/*****************************************************************************
** Procedure:  CConfTestLine::OnSwapHold
**
** Arguments: 'pReq' - Request object representing this SWAPHOLD event 
**            'lpBuff' - The event block pointer
**
** Returns:    void
**
** Description:  This function manages the TSPI_lineSwapHold processing
**               for this service provider.
**
*****************************************************************************/
bool CConfTestLine::OnSwapHold(RTSwapHold* pRequest, LPCVOID lpBuff)
{
	CTSPICallAppearance* pActiveCall = pRequest->GetActiveCall();
	CTSPICallAppearance* pHeldCall = pRequest->GetHoldingCall();
	if (pHeldCall->GetCallType() == CTSPICallAppearance::Conference)
	{
		pActiveCall->SetCallState(LINECALLSTATE_ONHOLD);
		pHeldCall->SetCallState(LINECALLSTATE_CONNECTED);
		CompleteRequest(pRequest, 0);
	}
	return false;

}// CConfTestLine::OnSwapHold
