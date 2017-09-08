/*******************************************************************/
//
// DRIVER.CPP 
//
// Driver routines
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "JPBX.h"
#include "mainfrm.h"
#include "document.h"
#include "agent.h"
#include "agentgroup.h"
#include "line.h"
#include "trunk.h"
#include "station.h"
#include "acdqueue.h"
#include "routepoint.h"
#include "pdialer.h"
#include "ivru.h"
#include "cmds.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*****************************************************************************
** Procedure:  CMainFrame::OnProcessData
**
** Arguments: 'strCommand' - Command to process
**
** Returns: void
**
** Description: This is called to process data from connection objects
**
*****************************************************************************/
LRESULT CMainFrame::OnProcessData(WPARAM, LPARAM lpData)
{
	COMMANDBLOCK* pCB = reinterpret_cast<COMMANDBLOCK*>(lpData);
	CJTDocument* pDoc = CUIObject::GetDocument();
	if (pDoc)
		pDoc->OnProcessData(pCB);
	return 0;

}// CMainFrame::OnProcessData

/*****************************************************************************
** Procedure:  CJTDocument::OnProcessData
**
** Arguments: 'pCB' - Command block
**
** Returns: void
**
** Description: This is called to process data from connection objects
**
*****************************************************************************/
void CJTDocument::OnProcessData(COMMANDBLOCK* pCB)
{
	// Log the command
	AppendToLog(LT_RECV, pCB->GetCommand());

	// Ask the command block to process the command
	DWORD dwError = pCB->Process(this);
	m_pcmConnections->Ack(pCB, dwError);

	delete pCB;

}// CJTDocument::OnProcessData

/*****************************************************************************
** Procedure:  COMMANDBLOCK::Create
**
** Arguments: 'strCommand' - Command received from socket
**            'pvData' - Itemdata
**
** Returns: Pointer to new COMMANDBLOCK to process command
**
** Description: This is a factory for the COMMANDBLOCK object
**
*****************************************************************************/
COMMANDBLOCK* COMMANDBLOCK::Create(CConnection* pConn, CString& strInput, LPVOID lpvData)
{
	CString strData(strInput);

	// Check the CRC
	if (StripCRC(strData))
	{
		// The first element is always the command.
		CString strCommand = GetNextElem(strData);
		if (!strCommand.IsEmpty())
		{
			DWORD dwExtension = 0;
			if (!strData.IsEmpty())
				dwExtension = atoi(GetNextElem(strData));

			strCommand.MakeUpper();
			if (strCommand == _T("VER"))
				return new VERSION(strInput, dwExtension, lpvData);
			else if (strCommand == _T("LO"))
				return new LOGON(strInput, dwExtension, lpvData);
			else if (strCommand == _T("TPC"))
				return new PLACETCALL(strInput, dwExtension, lpvData, strData);
			else if (strCommand == _T("TRC"))
				return new RELEASETCALL(strInput, dwExtension, lpvData, strData);
			else if (strCommand == _T("QAS"))
				return new QUERYAGENTSTATE(strInput, dwExtension, lpvData);
			else if (dwExtension != 0 &&
					(pConn->GetExtension() == 0xffff ||
						pConn->GetExtension() == dwExtension))
			{
				// If this is an agent logon command then process it.
				if (strCommand == _T("ALO"))
					return new AGENTLOGON(strInput, dwExtension, lpvData, strData);
				else
				{
					// Extension must be valid
					CLineDevice* pLine = CUIObject::GetDocument()->GetLineByID(dwExtension);
					if (pLine == NULL)
						return new COMMANDERROR(strInput, 0, lpvData, PBXERR_INVALDEV);

					// If this is a station, there MUST be an agent logged on.
					CStation* pStation = dynamic_cast<CStation*>(pLine);
					if (pStation != NULL && pStation->GetAgent() == NULL)
						return new COMMANDERROR(strInput, 0, lpvData, PBXERR_RESUNAVAIL);
				}

				// Check the call-related commands
				if (strCommand == _T("PC"))
					return new PLACECALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("RC"))
					return new RELEASECALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("AC"))
					return new ACCEPTCALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("AN"))
					return new ANSWERCALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("HC"))
					return new HOLDCALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("RTC"))
					return new RETRIEVECALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("CAS"))
					return new AGENTSTATE(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("SPG"))
					return new SETGAIN(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("SPV"))
					return new SETVOLUME(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("SPH"))
					return new SETHOOKSWITCH(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("GTD"))
					return new GENERATETD(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("BTC"))
					return new BLINDTRANSFERCALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("TC"))
					return new TRANSFERCALL(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("GCS"))
					return new GETCALLSTATS(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("GQS"))
					return new GETQUEUESTATS(strInput, dwExtension, lpvData, strData);
				else if (strCommand == _T("PPC"))
					return new PLACEPDCALL(strInput, dwExtension, lpvData, strData);
			}
			else
				return new COMMANDERROR(strInput, 0, lpvData, PBXERR_INVALLOGON);
		}
		else
			return new COMMANDERROR(strInput, 0, lpvData, PBXERR_BADCOMMAND);
	}

	return new COMMANDERROR(strInput, 0, lpvData, PBXERR_BADCRC);
	
}// COMMANDBLOCK::Create

/*****************************************************************************
** Procedure:  COMMANDBLOCK::StripCRC
**
** Arguments: 'strData' - Data to retrieve from
**
** Returns: true/false if CRC is correct
**
** Description: This strips the CRC from the string
**
*****************************************************************************/
bool COMMANDBLOCK::StripCRC(CString&)
{
#if 0
	if (strData.IsEmpty())
		return false;
	BYTE chChar = reinterpret_cast<BYTE>(strData.Right(1));
	strData = strData.Left(strData.GetLength()-1);

	BYTE chMine = 0;
	for (int i = 0; i < strData.GetLength(); i++)
		chMine += reinterpret_cast<BYTE>(strData[i]);
	return (chMine == chChar);
#else
	return true;
#endif

}// COMMANDBLOCK::StripCRC

/*****************************************************************************
** Procedure:  COMMANDBLOCK::GetNextElem
**
** Arguments: 'strData' - Data to retrieve from
**
** Returns: CString with element
**
** Description: This returns the next key in the data string
**
*****************************************************************************/
CString COMMANDBLOCK::GetNextElem(CString& strData)
{
	int iPos = strData.Find(_T(';'));
	if (iPos == -1)
		iPos = strData.GetLength();
	else if (iPos < 0)
	{
		strData = strData.Mid(1);
		return _T("");
	}
	
	CString strBuff = strData.Left(iPos);
	if (iPos < strData.GetLength())
		strData = strData.Mid(iPos+1);
	else
		strData.Empty();
	return strBuff;

}// COMMANDBLOCK::GetNextElem

/*****************************************************************************
** Procedure:  LOGON::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the logon request
**
*****************************************************************************/
PBXERROR LOGON::Process(CJTDocument* pDoc) 
{
	// If this is a single extension, validate the extension.
	if (m_dwExtension > 0)
	{
		CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
		if (pLine == NULL)
			return PBXERR_INVALDEV;

		// Must be a station
		if (dynamic_cast<CStation*>(pLine) == NULL)
			return PBXERR_INVALDEV;
	}
	else
		m_dwExtension = 0xffff;

	return PBXERR_SUCCESS;

}// LOGON::Process

/*****************************************************************************
** Procedure:  PLACECALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the place call request
**
*****************************************************************************/
PBXERROR PLACECALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station device.
	CStation * pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL)
		return PBXERR_BADCOMMAND;

	// The agent state cannot be Ready
	CAgent* pAgent = pStation->GetAgent();
	if (pAgent == NULL || pAgent->GetState() == CAgent::Ready)
		return PBXERR_INVALSTATE;

	m_strDestination.TrimRight();
	m_strDestination.TrimLeft();

	// Validate the extension
	if (m_strDestination.IsEmpty() ||
		m_strDestination.GetLength() < 4)
		return PBXERR_INVALDN;

	// If this station already has a call placed to the given destination -or-
	// the destination indicates the current station then fail.
	if (!lstrcmpi(m_strDestination, pStation->GetExtension()))
		return PBXERR_INVALDN;
	for (int i = 0; i < pStation->GetCallCount(); i++)
	{
		CCall* pCall = pStation->GetCallByPosition(i)->GetAttachedCall();
		if (pCall != NULL && pCall->GetLineOwner()->GetExtension() == m_strDestination)
			return PBXERR_INVALDN;
	}

	// Create a callerid structure
	CNetInfo ni(NULL, m_strDestination);

	// Place the call
	CCall* pCall = pLine->DialCall(ni);
	if (pCall == NULL)
		return PBXERR_RESUNAVAIL;
	
	return PBXERR_SUCCESS;

}// PLACECALL::Process

/*****************************************************************************
** Procedure:  RELEASECALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the release call request
**
*****************************************************************************/
PBXERROR RELEASECALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// If the call is active then release it
	pCall->Release();

	return PBXERR_SUCCESS;

}// RELEASECALL::Process

/*****************************************************************************
** Procedure:  ACCEPTCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the Accept call request
**
*****************************************************************************/
PBXERROR ACCEPTCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station or VRU
	if (dynamic_cast<CStation*>(pLine) == NULL &&
		dynamic_cast<CVRU*>(pLine) == NULL)
		return PBXERR_BADCOMMAND;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// If the call is ringing (inactive), then accept it.
	if (pCall->GetCallState() == CCall::Ringing)
		pCall->SetCallState(CCall::Alerting);

	return PBXERR_SUCCESS;

}// ACCEPTCALL::Process

/*****************************************************************************
** Procedure:  ANSWERCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the answer call request
**
*****************************************************************************/
PBXERROR ANSWERCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station or VRU
	if (dynamic_cast<CStation*>(pLine) == NULL &&
		dynamic_cast<CVRU*>(pLine) == NULL)
		return PBXERR_BADCOMMAND;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// If the call is ringing or accepted, then answer it.
	if (pCall->GetCallState() == CCall::Ringing ||
		pCall->GetCallState() == CCall::Alerting)
	{
		if (pLine->HasConnectedCall())
			return PBXERR_BADSTATE;
		pCall->SetCallState(CCall::Connected);
	}

	return PBXERR_SUCCESS;

}// ANSWERCALL::Process

/*****************************************************************************
** Procedure:  HOLDCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the hold call request
**
*****************************************************************************/
PBXERROR HOLDCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station
	if (dynamic_cast<CStation*>(pLine) == NULL)
		return PBXERR_BADCOMMAND;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// If the call is connected or dialing then shift it to onHold
	if (pCall->GetCallState() == CCall::Dialing ||
		pCall->GetCallState() == CCall::Connected)
	{
		pCall->SetCallState(CCall::Holding);
	}

	return PBXERR_SUCCESS;

}// HOLDCALL::Process

/*****************************************************************************
** Procedure:  RETRIEVECALLCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the retrieve call request
**
*****************************************************************************/
PBXERROR RETRIEVECALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station
	if (dynamic_cast<CStation*>(pLine) == NULL)
		return PBXERR_BADCOMMAND;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// If the call is on Hold and we can bring it off hold
	if (pCall->GetCallState() == CCall::Holding)
	{
		// Determine what the state is based on the OTHER side of the call.
		CCall* pOther = pCall->GetAttachedCall();
		if (pOther != NULL)
		{
			switch (pOther->GetCallState())
			{
				case CCall::Alerting:  
					pCall->SetCallState(CCall::WaitForAnswer); 
					break;
				case CCall::WaitForAnswer: 
					pCall->SetCallState(CCall::Alerting); 
					break;
				case CCall::Ringing:
					pCall->SetCallState(CCall::Dialing);
					break;
				case CCall::Holding:
					pCall->SetCallState(CCall::OtherSideHolding);
					break;
				default:
					pCall->SetCallState(CCall::Connected);
					break;
			}
		}
		else
			pCall->SetCallState(CCall::Connected);
	}

	return PBXERR_SUCCESS;

}// RETRIEVECALL::Process

/*****************************************************************************
** Procedure:  AGENTLOGON::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the agent logon request
**
*****************************************************************************/
PBXERROR AGENTLOGON::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL)
		return PBXERR_BADCOMMAND;

	// Check the parameters
	if (m_strAgentID.IsEmpty())
		return PBXERR_INVALPARAM;

	// Find the agent from the agent id.
	CAgent* pAgent = pDoc->FindAgent(m_strAgentID);
	if (pAgent == NULL)
		return PBXERR_INVALAGENTID;

	// If the agent is already logged into the station, allow
	// this command to continue (changes agent groups)
	if (pAgent != pStation->GetAgent())
	{
		// There must not be an agent logged on currently.
		if (pStation->GetAgent() != NULL)
			return PBXERR_RESUNAVAIL;

		// The agent cannot be logged in somewhere else
		if (pAgent->GetStation() != NULL)
			return PBXERR_RESUNAVAIL;
	}

	// Verify the password
	if (!pAgent->ValidatePassword(m_strPassword))
		return PBXERR_INVALLOGON;

	// Check the group(s)
	if  (m_dwGroup1 != 0 && pDoc->FindAgentGroup(m_dwGroup1) == NULL ||
		(m_dwGroup2 != 0 && pDoc->FindAgentGroup(m_dwGroup2) == NULL))
		return PBXERR_INVALAGENTGROUP;

	// If no agent group was specified, log the agent into the first
	// two groups in the list.
	if (m_fGroupIsBlank)
	{
		for (int i = 0; i < pDoc->GetAgentGroupCount(); i++)
		{
			CAgentGroup* pGroup = pDoc->GetAgentGroup(i);
			if (pGroup->IsAgentInGroup(pAgent))
			{
				if (m_dwGroup1 == 0)
					m_dwGroup1 = pGroup->GetGroupID();
				else
				{
					m_dwGroup2 = pGroup->GetGroupID();
					break;
				}
			}
		}
	}

	// Log the agent into the line
	if (!pStation->LogOnAgent(pAgent, m_dwGroup1, m_dwGroup2))
		return PBXERR_INVALAGENTGROUP;

	// Return the logged on agent id
	m_strReturn = pAgent->GetAgentId();

	return PBXERR_SUCCESS;

}// AGENTLOGON::Process

/*****************************************************************************
** Procedure:  AGENTSTATE::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the agent state change request
**
*****************************************************************************/
PBXERROR AGENTSTATE::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL)
		return PBXERR_BADCOMMAND;

	// Check the parameters
	if (m_strAgentID.IsEmpty() || m_strState.IsEmpty())
		return PBXERR_INVALPARAM;

	// Find the agent from the agent id.
	CAgent* pAgent = pDoc->FindAgent(m_strAgentID);
	if (pAgent == NULL)
		return PBXERR_INVALAGENTID;

	// The given agent must be logged on
	if (pStation->GetAgent() != pAgent)
		return PBXERR_INVALAGENTID;

	// Convert the character to an agent state
	enum CAgent::AgentState iState = CAgent::Unknown;
	switch (toupper(m_strState[0]))
	{
		case 'S':	iState = CAgent::SignedOut; break;
		case 'R':	iState = CAgent::Ready; break;
		case 'N':	iState = CAgent::NotReady; break;
		case 'W':	iState = CAgent::InCallWork; break;
	}

	// Validate the state
	if (iState == CAgent::Unknown ||
		pAgent->GetState() == iState ||
		(pAgent->GetState() != CAgent::NotReady && iState == CAgent::SignedOut))
		return PBXERR_INVALSTATE;

	// If we are currently in call, set the NEXT state
	if (pAgent->GetState() == CAgent::Busy ||
		pAgent->GetState() == CAgent::BusyACD)
		pAgent->SetNextState(iState);
	else
		pAgent->SetState(iState);

	return PBXERR_SUCCESS;

}// AGENTSTATE::Process

/*****************************************************************************
** Procedure:  SETHOOKSWITCH::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the hookswitch state
**
*****************************************************************************/
PBXERROR SETHOOKSWITCH::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station with active calls
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL || pStation->HasConnectedCall() == false)
		return PBXERR_BADCOMMAND;

	// The code passed should be a '0' or '1'.
	if (m_strState.GetLength() == 0)
		return PBXERR_INVALPARAM;

	switch (m_strState[0])
	{
		case _T('0'):	m_strState = _T("M"); break;
		case _T('1'):	m_strState = _T("F"); break;
		default:		return PBXERR_INVALPARAM;
	}

	// Send the proper event back to indicate the new hookswitch state
	SendEvent(pStation->GetLineID(), PBXEVT_PHC, pStation->GetExtension(), m_strState[0]);
	
	return PBXERR_SUCCESS;

}// SETHOOKSWITCH::Process

/*****************************************************************************
** Procedure:  SETVOLUME::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the volume level
**
*****************************************************************************/
PBXERROR SETVOLUME::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station with active calls
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL || pStation->HasConnectedCall() == false)
		return PBXERR_BADCOMMAND;

	if (pStation->SetVolume(m_iVolume))
		return PBXERR_SUCCESS;
	return PBXERR_INVALPARAM;

}// SETVOLUME::Process

/*****************************************************************************
** Procedure:  SETGAIN::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the gain level
**
*****************************************************************************/
PBXERROR SETGAIN::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station with active calls
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL || pStation->HasConnectedCall() == false)
		return PBXERR_BADCOMMAND;

	if (pStation->SetGain(m_iGain))
		return PBXERR_SUCCESS;
	return PBXERR_INVALPARAM;

}// SETGAIN::Process

/*****************************************************************************
** Procedure:  GENERATETD::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the generate tone/digit command
**
*****************************************************************************/
PBXERROR GENERATETD::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station with active calls
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL || pStation->HasConnectedCall() == false)
		return PBXERR_BADCOMMAND;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL || pCall->GetCallState() != CCall::Connected)
		return PBXERR_INVALCALLID;

	// The digit must be 0-9,#,*
	m_strDigit = m_strDigit.Left(1);
	if (m_strDigit.SpanIncluding(_T("0123456789#*")) != m_strDigit)
		return PBXERR_INVALPARAM;

	// See if this call has a shadow
	CCall* pOther = pCall->GetAttachedCall();
	if (pOther != NULL)
		SendEvent(false, pOther, PBXEVT_DD, pOther->GetCallID(), m_strDigit[0]);

	return PBXERR_SUCCESS;

}// GENERATETD::Process

/*****************************************************************************
** Procedure:  PLACETCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the place trunk call request
**
*****************************************************************************/
PBXERROR PLACETCALL::Process(CJTDocument* pDoc)
{
	// Find an available trunk line.
	CTrunk* pTrunk = pDoc->FindAvailableTrunk();
	if (pTrunk == NULL)
		return PBXERR_RESUNAVAIL;

	// Validate the extension
	if (m_strDestination.IsEmpty() ||
		m_strDestination.GetLength() < 4)
		return PBXERR_INVALDN;

	// Place the call
	CCall* pCall = pTrunk->GenerateIncomingCall(
					CNetInfo(m_strCallerName, m_strCallerNumber),
					CNetInfo(NULL, m_strDestination));
	if (pCall == NULL)
		return PBXERR_RESUNAVAIL;
	
	// Return the trunk and callid
	m_strReturn.Format(_T("%04d;%08X"), pTrunk->GetLineID(), pCall->GetCallID());

	return PBXERR_SUCCESS;

}// PLACECALL::Process

/*****************************************************************************
** Procedure:  RELEASETCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the release trunk call request
**
*****************************************************************************/
PBXERROR RELEASETCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwTrunkID);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// Locate the call object by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// If the call is active then release it
	pCall->Release();

	return PBXERR_SUCCESS;

}// RELEASETCALL::Process

/*****************************************************************************
** Procedure:  TRANSFERCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the transfer call request
**
*****************************************************************************/
PBXERROR TRANSFERCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a station
	CStation* pStation = dynamic_cast<CStation*>(pLine);
	if (pStation == NULL)
		return PBXERR_BADCOMMAND;

	// Locate the call objects by callid.
	CCall* pCall_1 = pLine->FindCallByCallID(m_dwCallID1);
	CCall* pCall_2 = pLine->FindCallByCallID(m_dwCallID2);
	if (pCall_1 == NULL || pCall_2 == NULL)
		return PBXERR_INVALCALLID;

	// Get the other side of the second call.
	CCall* pCall_3 = pCall_2->GetAttachedCall();
	if (pCall_3 == NULL)
		return PBXERR_INVALCALLID;

	// We are transferring the call to THAT line owner.
	pLine = pCall_3->GetLineOwner();
	ASSERT (pLine != pStation);

	// Perform the transfer
	if (!pLine->TransferCall(pCall_3, pCall_1))
		return PBXERR_FAILED;

	return PBXERR_SUCCESS;

}// TRANSFERCALL::Process

/*****************************************************************************
** Procedure:  BLINDTRANSFERCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the transfer call request
**
*****************************************************************************/
PBXERROR BLINDTRANSFERCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pSourceLine = pDoc->GetLineByID(m_dwExtension);
	if (pSourceLine == NULL)
		return PBXERR_INVALDEV;

	// Locate the call objects by callid.
	CCall* pCall = pSourceLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;

	// Attempt to locate the destination by the given extension.
	CLineDevice* pDestLine = pDoc->GetLineByID(m_dwTarget);
	if (pDestLine == NULL || pDestLine == pSourceLine)
		return PBXERR_INVALPARAM;

	// Transfer the call
	if (!pDestLine->TransferCall(NULL, pCall))
		return PBXERR_FAILED;

	return PBXERR_SUCCESS;

}// BLINDTRANSFERCALL::Process

/*****************************************************************************
** Procedure:  GETCALLSTATS::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the GCS request
**
*****************************************************************************/
PBXERROR GETCALLSTATS::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a queue
	CACDQueue* pQueue = dynamic_cast<CACDQueue*>(pLine);
	if (pQueue == NULL)
		return PBXERR_BADCOMMAND;

	// Locate the call objects by callid.
	CCall* pCall = pLine->FindCallByCallID(m_dwCallID);
	if (pCall == NULL)
		return PBXERR_INVALCALLID;
	
	// Return the time this call has been queued.
	m_strReturn.Format(_T("%d"), pCall->SecondsInState());
	return PBXERR_SUCCESS;

}// GETCALLSTATS::Process

/*****************************************************************************
** Procedure:  GETQUEUESTATS::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the GQS request
**
*****************************************************************************/
PBXERROR GETQUEUESTATS::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a queue
	CACDQueue* pQueue = dynamic_cast<CACDQueue*>(pLine);
	if (pQueue == NULL)
		return PBXERR_BADCOMMAND;

	// Return the number of queued calls, longest queued call, and number of agents.
	int iSecs = 0;
	if (pQueue->GetCallCount() > 0)
	{
		CCall* pCall = pQueue->GetCallByPosition(0);
		iSecs = pCall->SecondsInState();
	}

	m_strReturn.Format(_T("%d;%d;%d"), pQueue->GetCallCount(), iSecs, pQueue->GetAgentCount());
	return PBXERR_SUCCESS;

}// GETQUEUESTATS::Process

/*****************************************************************************
** Procedure:  PLACEPDCALL::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the place predictive dialer call request
**
*****************************************************************************/
PBXERROR PLACEPDCALL::Process(CJTDocument* pDoc)
{
	// Get the source line
	CLineDevice* pLine = pDoc->GetLineByID(m_dwExtension);
	if (pLine == NULL)
		return PBXERR_INVALDEV;

	// It must be a predictive dialer device.
	CPDialer * pDialer = dynamic_cast<CPDialer*>(pLine);
	if (pDialer == NULL)
		return PBXERR_BADCOMMAND;

	// Validate the extension if one is given.
	if (m_dwTransferTo > 0 && pDoc->GetLineByID(m_dwTransferTo) == NULL)
		return PBXERR_INVALDN;

	// Validate the timeout.
	if (m_iTimeout > 60)
		return PBXERR_INVALPARAM;

	// Convert to msecs.
	m_iTimeout *= 1000;

	if ((m_strNumber.GetLength() < 7) || m_strNumber[0] == '9')
		return PBXERR_INVALPARAM;

	// Add a '9' to the number to dial
	m_strNumber = _T("9") + m_strNumber;

	// Create a calledid structure
	CNetInfo ni(NULL, m_strNumber);

	// Place the call
	CCall* pCall = pDialer->DialPredictiveCall(ni, m_iTimeout, m_dwTransferTo);
	if (pCall == NULL)
		return PBXERR_RESUNAVAIL;
	
	return PBXERR_SUCCESS;

}// PLACEPDCALL::Process

/*****************************************************************************
** Procedure:  QUERYAGENTSTATE::Process
**
** Arguments: 'pDoc' - Document
**
** Returns: PBXERR
**
** Description: This manages the query-agent-state command
**
*****************************************************************************/
PBXERROR QUERYAGENTSTATE::Process(CJTDocument* pDoc)
{
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CStation* pStation = dynamic_cast<CStation*>(pLine);
		if (pStation != NULL)
		{
			CAgent* pAgent = pStation->GetAgent();
			if (pAgent != NULL)
				pAgent->NotifyState();
		}
		pLine = pDoc->GetNextLine(pLine);
	}
	return PBXERR_SUCCESS;

}// QUERYAGENTSTATE::Process
