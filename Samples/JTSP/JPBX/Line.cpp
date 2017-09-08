/*******************************************************************/
//
// LINE.CPP
//
// Basic line definitions
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
#include "jpbx.h"
#include "document.h"
#include "line.h"
#include "call.h"
#include "station.h"
#include "pdialer.h"
#include "acdqueue.h"
#include "routepoint.h"
#include "calllistdlg.h"
#include "trunk.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// RTTI INFORMATION
/*---------------------------------------------------------------*/
IMPLEMENT_SERIAL(CLineDevice, CUIObject, 1)

/*****************************************************************************
** Procedure:  CLineDevice::CLineDevice
**
** Arguments: void
**
** Returns:    void
**
** Description:  Default no-parameter constructor for the line device
**
*****************************************************************************/
CLineDevice::CLineDevice() : m_strName(_T("")), m_dwLineID(0), m_pwndSCList(0)
{
	m_pwndSCList = new CCallListDlg(this);

}// CLineDevice::CLineDevice

/*****************************************************************************
** Procedure:  CLineDevice::CLineDevice
**
** Arguments: 'id' - Device id
**            'pszName' - Name of the line device
**
** Returns:    void
**
** Description:  Parameterized constructor for the line device
**
*****************************************************************************/
CLineDevice::CLineDevice(DWORD id, const char* pszName) : 
		m_strName(pszName), m_dwLineID(id), m_pwndSCList(0)
{
	// Notify the document that this line has been created
	GetDocument()->OnCreateLine(this);
	m_pwndSCList = new CCallListDlg(this);

}// CLineDevice::CLineDevice

/*****************************************************************************
** Procedure:  CLineDevice::~CLineDevice
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line device
**
*****************************************************************************/
CLineDevice::~CLineDevice()
{
	// Delete our call list window
	delete m_pwndSCList;
	m_pwndSCList = NULL;

	// Delete all the call objects in our line array
	try 
	{
		while (m_lstCalls.GetCount() > 0)
			delete m_lstCalls.GetHead();
	} 
	catch (...)
	{
	}

	// Notify the document that this line has been removed
	GetDocument()->OnDestroyLine(this);

}// CLineDevice::~CLineDevice

/*****************************************************************************
** Procedure:  CLineDevice::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CLineDevice::Serialize( CArchive& archive )
{
	CUIObject::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << m_dwLineID;
		archive << m_strName;
	}
	else
	{
		archive >> m_dwLineID;
		archive >> m_strName;
	}

}// CLineDevice::Serialize

/*****************************************************************************
** Procedure:  CLineDevice::CanAddCall
**
** Arguments: void
**
** Returns:    true/false
**
** Description:  Returns TRUE if we can add a new call to the line
**
*****************************************************************************/
bool CLineDevice::CanAddCall()
{
	return true;

}// CLineDevice::CanAddCall

/*****************************************************************************
** Procedure:  CLineDevice::OnCallCreated
**
** Arguments: 'pCall' - Call to add to our list
**            'fPlaceCall' - Call was created due to a DIAL event
**
** Returns:    void
**
** Description:  Adds the specified object to our list
**
*****************************************************************************/
void CLineDevice::OnCallCreated(CCall* pCall, bool fPlaceCall)
{
	// Add the call to our list
	if (m_lstCalls.Find(pCall) == NULL)
		m_lstCalls.AddTail(pCall);

	// Notify our window
	if (m_pwndSCList->GetSafeHwnd() != NULL &&
		::IsWindow(m_pwndSCList->GetSafeHwnd()))
		m_pwndSCList->OnCallCreated(pCall);

	// Send a "CALL CREATED" or "CALL PLACED" notification if this is
	// the first time we have seen this call (i.e. it is not a subsequent
	// mapping of some other call)
	SendEvent(false, pCall, (fPlaceCall == false) ? PBXEVT_CD : PBXEVT_CP, 
				GetExtension(), pCall->GetCallID(), 
				pCall->GetDNIS(), pCall->GetANI());

	// Force an update of the UI
	GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);

}// CLineDevice::OnCallCreated

/*****************************************************************************
** Procedure:  CLineDevice::OnCallDestroyed
**
** Arguments: 'pCall' - Call to remove from our list
**
** Returns:    void
**
** Description:  Removes the specified object from our list
**
*****************************************************************************/
void CLineDevice::OnCallDestroyed(CCall* pCall, bool fNotifyRelease)
{
	// Remove the call from our list
	POSITION pos = m_lstCalls.Find(pCall);
	if (pos != NULL)
		m_lstCalls.RemoveAt(pos);

	// Notify our window
	if (m_pwndSCList->GetSafeHwnd() != NULL &&
		::IsWindow(m_pwndSCList->GetSafeHwnd()))
		m_pwndSCList->OnCallDestroyed(pCall);

	// Send a "CALL RELEASED" event if this call has no shadow
	if (fNotifyRelease)
		SendEvent(true, pCall, PBXEVT_CR, pCall->GetCallID());

	// Force an update of the UI
	GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);

}// CLineDevice::OnRemoveCall

/*****************************************************************************
** Procedure:  CLineDevice::OnCallStateChange
**
** Arguments: 'pCall' - Call changing state
**            'csNewState' - New state
**
** Returns:    void
**
** Description:  Notification from a call about a change in state
**
*****************************************************************************/
void CLineDevice::OnCallStateChange(CCall* pCall)

{
	static char chStates[] = { 'U', 'D', 'R', 'A', 'R', 'C', 'B', 'K', 'H', 'I', 'Q' };
	bool fSentNotify = false;

	if (m_pwndSCList->GetSafeHwnd() != NULL &&
		::IsWindow(m_pwndSCList->GetSafeHwnd()))
		m_pwndSCList->OnCallStateChange(pCall);

	// Force an update of the UI
	GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);

	// If this call is now in some active state and we have other
	// calls in an active state then we now have a conference - send
	// and event to indicate this.
	if (pCall->IsActiveState())
	{
		CObArray arrCalls;
		arrCalls.Add(pCall);

		// See if there are any others
		for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
		{
			CCall* pConf = m_lstCalls.GetNext(pos);
			if (pConf != pCall && pConf->IsActiveState())
				arrCalls.Add(pConf);
		}

		// If we have more than one call in an active state, tell all
		// our clients.
		if (arrCalls.GetSize() > 1)
		{
			CString strCalls, strBuff;
			for (int i = 0; i < arrCalls.GetSize(); i++)
			{
				CCall* pCall = dynamic_cast<CCall*>(arrCalls[i]);
				strBuff.Format(_T("%08X"), pCall->GetCallID());
				strCalls += strBuff;
				strCalls += _T(';');
			}
			fSentNotify = true;
			SendEvent(GetLineID(), PBXEVT_CC, GetExtension(), strCalls);
		}
	}

	// If we didn't notify about a conference, then send a call connected
	// event to all the monitors.
	if (!fSentNotify)
		SendEvent(false, pCall, PBXEVT_CS, GetExtension(), pCall->GetCallID(), chStates[pCall->GetCallState()]);

}// CLineDevice::OnCallStateChange

/*****************************************************************************
** Procedure:  CLineDevice::GetExtension
**
** Arguments: void
**
** Returns:    LineID as a string
**
** Description:  Returns the internal line identifier with embedded type
**
*****************************************************************************/
CString CLineDevice::GetExtension() const
{
	TCHAR chBuff[5];
	ASSERT (m_dwLineID < 9999);
	wsprintf(chBuff, _T("%04ld"), m_dwLineID);
	return chBuff;

}// CLineDevice::GetExtension

/*****************************************************************************
** Procedure:  CLineDevice::FindCallByCallID
**
** Arguments: 'dwID' - DWORD callid to locate
**
** Returns:    Call object
**
** Description:  Locates a call by ID
**
*****************************************************************************/
CCall* CLineDevice::FindCallByCallID(DWORD dwID)
{
	for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
	{
		CCall* pCall = m_lstCalls.GetNext(pos);
		if (pCall->GetCallID() == dwID)
			return pCall;
	}
	return NULL;

}// CLineDevice::FindCallByCallID

/*****************************************************************************
** Procedure:  CLineDevice::DialCall
**
** Arguments: 'pszNumber' - DN to dial
**
** Returns:    Call object
**
** Description: This function creates a call on a line to either 
**              another line or an outside call.
**
*****************************************************************************/
CCall* CLineDevice::DialCall(CNetInfo& cidDNIS)
{
	// Only stations or PDialer lines can dial.
	ASSERT (dynamic_cast<CStation*>(this) != NULL ||
			dynamic_cast<CPDialer*>(this) != NULL);

	// Get the number to dial and determine what we are dialing..
	CString strNumber(cidDNIS.GetNumber());
	CLineDevice* pDest = GetDocument()->FindLineByExtension(strNumber);

	// Validate the return value, we don't allow direct-dialing of 
	// the ACD queues or route points.
	if (pDest == NULL ||
		dynamic_cast<CACDQueue*>(pDest) != NULL ||
		dynamic_cast<CRoutePoint*>(pDest) != NULL ||
		dynamic_cast<CPDialer*>(pDest) != NULL)
		return NULL;

	// Verify we can place a call on the source and receive a call on the destination.
	// This simply verifies that we have the band-width
	if (!CanAddCall() || !pDest->CanAddCall())
		return NULL;

	// If we have an active call on this line, it goes to the holding state
	for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
	{
		CCall* pCall = m_lstCalls.GetNext(pos);
		if (pCall->GetCallState() == CCall::Connected)
			pCall->SetCallState(CCall::Holding);
		else if (pCall->GetCallState() == CCall::Busy)
			pCall->Release();
	}

	// Adjust the DNIS against this new line device if it is not a trunk
	if (dynamic_cast<CTrunk*>(pDest) == NULL)
	{
		CStation* pStation = dynamic_cast<CStation*>(pDest);
		if (pStation)
		{
			CAgent* pAgent = pStation->GetAgent();
			ASSERT (pAgent != NULL);
			cidDNIS = CNetInfo(pAgent->GetAgentId(), pDest->GetExtension());
		}
		else
			cidDNIS = CNetInfo(pDest->GetLineName(), pDest->GetExtension());
	}

	// Otherwise remove the '9'
	else
		cidDNIS = CNetInfo(NULL, strNumber.Mid(1));

	// Get the line name or agent name.
	CStation* pStation = dynamic_cast<CStation*>(this);
	CString strName;
	if (pStation)
	{
		CAgent* pAgent = pStation->GetAgent();
		ASSERT (pAgent != NULL);
		strName = pAgent->GetAgentId();
	}
	else strName = GetLineName();

	// Create the call - this will send the CALL PLACED event to any monitors
	CCall* pCall_Source = new CCall(this, CNetInfo(strName, GetExtension()), cidDNIS, true);
	ASSERT (pCall_Source != NULL);

	// Transition it to the DIALING state
	pCall_Source->SetCallState(CCall::Dialing);

	// Tell the recieving line that an incoming call is there
	pDest->OnReceiveCall(pCall_Source);

	return pCall_Source;

}// CLineDevice::DialCall

/*****************************************************************************
** Procedure:  CLineDevice::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an incoming call from
**              another station (direct-dial) or trunk (outside call)
**
*****************************************************************************/
void CLineDevice::OnReceiveCall(CCall* pCall_Source)
{
	// Map another copy of the same call on this line
	CCall* pCall_Dest = new CCall(this, *pCall_Source);

	// Mark this call as ringing (waiting for alert tone)
	pCall_Dest->SetCallState(CCall::Ringing);

}// CLineDevice::OnReceiveCall

/*****************************************************************************
** Procedure:  CLineDevice::TransferCall
**
** Arguments: 'pCall_Consult' - Consultation call which is being dissolved
**            'pCall_Source' - Call being transferred
**
** Returns:    void
**
** Description: This function is used to transfer a call to this line
**              possibly using a consultation call.
**
*****************************************************************************/
bool CLineDevice::TransferCall(CCall* pCall_Consult, CCall* pCall_Source)
{
	if (!CanAddCall())
		return false;

	// Save the previous owner
	CLineDevice* pOwner = pCall_Source->GetLineOwner();

	// If we don't have a consultation call then this is a blind
	// transfer and we have NEVER seen this call before
	if (pCall_Consult == NULL)
	{
		// If the call has no other side, then it is on a trunk and is being
		// moved to another extension (incoming)
		if (pCall_Source->GetAttachedCall() == NULL)
			pCall_Source = new CCall(this, *pCall_Source);

		// Save off the previous owner and move the call to the new line (us)
		else
			pCall_Source->MoveToLine(this);

		// Send the transfer event (two sets to all owners)
		SendEvent(pOwner->GetLineID(), PBXEVT_CT, 
				pOwner->GetExtension(), pCall_Source->GetCallID(), 0, 
				GetExtension(), pCall_Source->GetDNIS(), pCall_Source->GetANI());
		SendEvent(true, pCall_Source, PBXEVT_CT, 
			pOwner->GetExtension(), pCall_Source->GetCallID(), 0, 
			GetExtension(), pCall_Source->GetDNIS(), pCall_Source->GetANI());

		// Set the state of the call on our route point
		pCall_Source->SetCallState(CCall::Ringing);
	}

	// Otherwise merge the two calls, callid is now source call
	else
	{
		// Make pCall_1 "our" copy of the call
		CCall* pCall_1 = pCall_Consult;
		CCall* pCall_2 = pCall_Consult->GetAttachedCall();
		ASSERT (pCall_1 && pCall_2);
		if (pCall_1->GetLineOwner() != this)
		{
			pCall_1 = pCall_2;
			pCall_2 = pCall_Consult;
		}

		ASSERT (pCall_1->GetLineOwner() == this);

		// Do the same for the source call
		CCall* pCallS_1 = pCall_Source;
		CCall* pCallS_2 = pCall_Source->GetAttachedCall();
		ASSERT (pCallS_1 && pCallS_2);
		if (pCallS_1->GetLineOwner() != pCall_2->GetLineOwner())
		{
			pCallS_1 = pCallS_2;
			pCallS_2 = pCall_Source;
		}

		// Merge the calls together
		pCall_1->Merge(pCallS_2);

		// Send the transfer event (two sets to all owners)
		SendEvent(pOwner->GetLineID(), PBXEVT_CT, pOwner->GetExtension(), 
				pCall_1->GetCallID(), pCall_2->GetCallID(), 
				GetExtension(), pCall_1->GetDNIS(), pCall_1->GetANI());
		SendEvent(true, pCall_1, PBXEVT_CT, pOwner->GetExtension(), 
				pCall_1->GetCallID(), pCall_2->GetCallID(), 
				GetExtension(), pCall_1->GetDNIS(), pCall_1->GetANI());
			
		// Delete the two calls on the owner line - they are dead now.
		delete pCall_2;
		delete pCallS_1;
	}

	return true;

}// CLineDevice::TransferCall

/*****************************************************************************
** Procedure:  CLineDevice::Delete
**
** Arguments: void
**
** Returns:    void
**
** Description: This deletes the current line from the system
**
*****************************************************************************/
void CLineDevice::Delete()
{
	if (GetCallCount() > 0)
	{
		AfxMessageBox(IDS_CANNOTDELETELINE);
		return;
	}

	// If we are to delete the line, then do it.
	if (AfxMessageBox(IDS_DELETE, MB_YESNO) == IDYES)
		delete this;

}// CLineDevice::Delete

/*****************************************************************************
** Procedure:  CLineDevice::ContextMenu
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether context menu was popped-up
**
** Description:  This function displays a context menu for an station object.
**
*****************************************************************************/
bool CLineDevice::ContextMenu(CWnd* pwndOwner)
{
	return RunPopup( IDR_LINEMENU, pwndOwner );

}// CLineDevice::ContextMenu

/*****************************************************************************
** Procedure:  CLineDevice::OnCommand
**
** Arguments: 'nCommand' - WM_COMMAND message which was intercepted
**
** Returns:    void
**
** Description:  This function processes the WM_COMMAND handler for the
**               station objects.
**
*****************************************************************************/
void CLineDevice::OnCommand( UINT nCommand )
{
	switch (nCommand)
	{
		case IDM_CTX_PROPERTIES:
			Edit(AfxGetMainWnd());
			break;
		case IDM_CTX_REMOVE:
			Delete();
			break;
		case IDM_CTX_CALLS:
			ShowCallList();
			break;
	}

}// CLineDevice::OnCommand

/*****************************************************************************
** Procedure:  CLineDevice::ShowCallList
**
** Arguments: void
**
** Returns:    void
**
** Description:  This function shows the call list dialog
**
*****************************************************************************/
void CLineDevice::ShowCallList()
{
	if (m_pwndSCList->GetSafeHwnd() == NULL ||
		::IsWindow(m_pwndSCList->GetSafeHwnd()) == FALSE)
		m_pwndSCList->Create(AfxGetMainWnd());

	m_pwndSCList->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

}// CLineDevice::ShowCallList

/*****************************************************************************
** Procedure:  CLineDevice::OnTimer
**
** Arguments: void
**
** Returns:    void
**
** Description:  This function processes the interval timer
**
*****************************************************************************/
void CLineDevice::OnTimer()
{
}// CLineDevice::OnTimer
