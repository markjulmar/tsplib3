/*******************************************************************/
//
// STATION.CPP
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "jpbx.h"
#include "document.h"
#include "station.h"
#include "acdqueue.h"
#include "base.h"

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
IMPLEMENT_SERIAL(CStation, CLineDevice, 1)

/*---------------------------------------------------------------*/
// CONSTANTS
/*---------------------------------------------------------------*/
UINT LAMP_READY = 0;
UINT LAMP_INWORK = 1;

/**************************************************************************
** CStationDlg
**
** User-interface dialog for the station
**
***************************************************************************/
class CStationDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CStationDlg)
	enum { IDD = IDD_STATION };
	CListBox m_lbGroups;
	DWORD m_dwExtension;
	CJTDocument* m_pDocument;
	CDWordArray m_arrGroups;
	CString	m_strDescription;
	BOOL m_fEditExtension;
	//}}AFX_DATA

// Construction
public:
	CStationDlg(CWnd* pParent, CJTDocument* pDoc);

// Publics
public:
	CJTDocument* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStationDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CStationDlg::CStationDlg
**
** Arguments: 'pParent' - Parent for the dialog
**
** Returns:    void
**
** Description: Dialog constructor
**
*****************************************************************************/
CStationDlg::CStationDlg(CWnd* pParent, CJTDocument* pDoc)
	: CBaseDialog(CStationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStationDlg)
	m_fEditExtension = TRUE;
	m_dwExtension = 0;
	m_pDocument = pDoc;
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

}// CStationDlg::CStationDlg

/*****************************************************************************
** Procedure:  CStationDlg::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to CJTDocument object
**
** Description:  This function returns a pointer to the document associated
**               with the agent we are editing.
**
*****************************************************************************/
CJTDocument* CStationDlg::GetDocument() 
{ 
	return m_pDocument; 

}// CStationDlg::GetDocument

/*****************************************************************************
** Procedure:  CStationDlg::OnInitDialog
**
** Arguments: void
**
** Returns:    TRUE/FALSE if Windows should set focus to the first control.
**
** Description: This member function is called in response to the 
**              WM_INITDIALOG message. This message is sent to the dialog box 
**              during the Create, CreateIndirect, or DoModal calls, which occur 
**              immediately before the dialog box is displayed. 
**
*****************************************************************************/
BOOL CStationDlg::OnInitDialog() 
{
	// If we couldn't find a unique extension, exit
	if (m_dwExtension == 0)
	{
		m_dwExtension = GetDocument()->GenerateExtension();
		if (m_dwExtension == 0xffffffff)
		{
			AfxMessageBox(IDS_TOOMANYLINES);
			OnCancel();
			return FALSE;
		}
	}
	else
		m_fEditExtension = FALSE;
	
	// Call the base initialization
	CBaseDialog::OnInitDialog();

	// Set the initial state of our controls.
	OnChange();

	// Fill in the agent groups.
	for (int i = 0; i < GetDocument()->GetAgentGroupCount(); i++)
	{
		CAgentGroup* pGroup = GetDocument()->GetAgentGroup(i);
		if (pGroup != NULL)
		{
			int iPos = m_lbGroups.AddString(pGroup->GetGroupName());
			ASSERT(iPos != LB_ERR);
			m_lbGroups.SetItemData(iPos, pGroup->GetGroupID());
		}
	}

	// Now determine which groups this station allows and
	// select them in the box.
	for (i = 0; i < m_arrGroups.GetSize(); i++)
	{
		int iGroup = (int) m_arrGroups[i];
		for (int iLB = 0; iLB < m_lbGroups.GetCount(); iLB++)
		{
			if ((int)m_lbGroups.GetItemData(iLB) == iGroup)
				m_lbGroups.SetSel(iLB, TRUE);
		}
	}

	// Limit our text
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DNUM))->LimitText(EXTENSION_SIZE);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DESCRIPTION))->LimitText(LINENAME_SIZE);

	// If we have an extension already, don't allow it to be changed.
	if (!m_fEditExtension)
	{
		GetDlgItem(IDC_DNUM)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	GotoDlgCtrl(GetDlgItem(IDC_DESCRIPTION));
	return FALSE;

}// CStationDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CStationDlg::DoDataExchange
**
** Arguments: 'pDX' - Dialog data exchange pointer
**
** Returns:    void
**
** Description: Called by the framework to exchange and validate dialog 
**              data.  This connects windows controls up to class elements
**              in the C++ object.
**
*****************************************************************************/
void CStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStationDlg)
	DDX_Control(pDX, IDC_GROUPS, m_lbGroups);
	DDX_Text(pDX, IDC_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_DNUM, m_dwExtension);
	//}}AFX_DATA_MAP

}// CStationDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CStationDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CStationDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CStationDlg)
	ON_EN_CHANGE(IDC_DNUM, OnChange)
	ON_EN_CHANGE(IDC_DESCRIPTION, OnChange)
	ON_LBN_SELCHANGE(IDC_GROUPS, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CStationDlg::OnChange
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user changes any of the data in 
**              our dialog.  It enables/disables the OK button based on 
**              whether we have enough information to dismiss the dialog.
**
*****************************************************************************/
void CStationDlg::OnChange() 
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(m_dwExtension > 0);

}// CStationDlg::OnChange

/*****************************************************************************
** Procedure:  CStationDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CStationDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		// Validate the extension - it should NOT be in our line map currently.
		if (m_fEditExtension)
		{
			if (GetDocument()->GetLineByID(m_dwExtension) != NULL)
			{
				AfxMessageBox(IDS_LINEEXISTS);
				GotoDlgCtrl(GetDlgItem(IDC_DNUM));
				return;
			}

			// Validate the range
			if (m_dwExtension < 1000 || m_dwExtension > 8999)
			{
				AfxMessageBox(IDS_EXTENSIONINVALID);
				GotoDlgCtrl(GetDlgItem(IDC_DNUM));
				return;
			}
		}

		// Save off the groups
		m_arrGroups.RemoveAll();
		for (int iLB = 0; iLB < m_lbGroups.GetCount(); iLB++)
		{
			if (m_lbGroups.GetSel(iLB))
				m_arrGroups.Add(m_lbGroups.GetItemData(iLB));
		}
		CBaseDialog::OnOK();
	}

}// CStationDlg::OnOK

/*****************************************************************************
** Procedure:  CStation::CStation
**
** Arguments: 'dwID' - Station id
**            'pszDescription' - Description for station
**
** Returns:    void
**
** Description:  Parameterized constructor
**
*****************************************************************************/
CStation::CStation(DWORD dwLineID, LPCTSTR pszDescription) :
	CLineDevice(dwLineID, pszDescription), m_pAgent(NULL), 
	m_iGain(0x5000), m_iVolume(0x5000)
{
	m_arrLamps.Add(new CLamp(this, LAMP_READY));
	m_arrLamps.Add(new CLamp(this, LAMP_INWORK));

}// CStation::CStation

/*****************************************************************************
** Procedure:  CStation::CStation
**
** Arguments: void
**
** Returns:    void
**
** Description:  No parameter constructor for the queue device
**
*****************************************************************************/
CStation::CStation() : CLineDevice(), m_pAgent(NULL),
	m_iGain(0x5000), m_iVolume(0x5000)
{
	m_arrLamps.Add(new CLamp(this, LAMP_READY));
	m_arrLamps.Add(new CLamp(this, LAMP_INWORK));

}// CStation::CStation

/*****************************************************************************
** Procedure:  CStation::~CStation
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line device
**
*****************************************************************************/
CStation::~CStation()
{
	if (m_pAgent != NULL)
		m_pAgent->AssociateStation(NULL);

	// Delete the lamp objects
	for (int i = 0; i < m_arrLamps.GetSize(); i++)
		delete m_arrLamps[i];

}// CStation::~CStation

/*****************************************************************************
** Procedure:  CStation::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CStation::Serialize( CArchive& archive )
{
	CLineDevice::Serialize(archive);
	if (archive.IsStoring())
	{
	}
	else
	{
	}
	m_arrGroups.Serialize(archive);

}// CStation::Serialize

/*****************************************************************************
** Procedure:  CStation::OnCallCreated
**
** Arguments: 'pCall' - Call to add to our list
**            'fPlaceCall' - Call was created due to a DIAL event
**
** Returns:    void
**
** Description:  Adds the specified object to our list
**
*****************************************************************************/
void CStation::OnCallCreated(CCall* pCall, bool fPlaceCall)
{
	// Pass to the base class
	CLineDevice::OnCallCreated(pCall, fPlaceCall);

}// CStation::OnCallCreated

/*****************************************************************************
** Procedure:  CStation::OnCallDestroyed
**
** Arguments: 'pCall' - Call to remove from our list
**
** Returns:    void
**
** Description:  Removes the specified object from our list
**
*****************************************************************************/
void CStation::OnCallDestroyed(CCall* pCall, bool fNotifyRelease)
{
	// Pass to the base class
	CLineDevice::OnCallDestroyed(pCall, fNotifyRelease);

	// Send an event concerning the display
	BuildDisplay();

	// If we no longer have any calls, mark the hook switch state
	if (GetCallCount() == 0)
		SendEvent(GetLineID(), PBXEVT_PHC, GetExtension(), _T('O'));

	// Adjust the agent state
	if (GetAgent() != NULL && GetCallCount() == 0)
		m_pAgent->GotoNextState();

}// CStation::OnCallDestroyed

/*****************************************************************************
** Procedure:  CStation::CanAddCall
**
** Arguments: void
**
** Returns:    true/false if we can add the call
**
** Description: This member function is called to determine if it is ok
**              to add a new call to the station object
**
*****************************************************************************/
bool CStation::CanAddCall()
{
	// Cannot take calls if there is no agent.
	if (GetAgent() == NULL)
		return false;

	// Only have 3 addresses per line
	return (GetCallCount() == 3) ? false : true;
	
}// CStation::CanAddCall

/*****************************************************************************
** Procedure:  CStation::OnAgentStateChanged
**
** Arguments: void
**
** Returns:    void
**
** Description: This member function is called when an agent changes state
**
*****************************************************************************/
void CStation::OnAgentStateChanged()
{
	// If we have an agent on this line, adjust the state
	if (m_pAgent != NULL)
	{
		// If the agent is signing out, remove it's reference to 
		// this station.
		if (m_pAgent->GetState() == CAgent::SignedOut)
		{
			CAgent* pAgent = m_pAgent;
			m_pAgent = NULL;
			pAgent->AssociateStation(NULL);

			// Darken all our lamps.
			for (int i = 0; i < m_arrLamps.GetSize(); i++)
				m_arrLamps[i]->SetState(false);

			// Find an available queue.
			CACDQueue* pQueue = GetDocument()->GetFirstQueue(m_pAgent);

			// If we have any calls pending in our call list, move them
			// to our 1st agent group.  If we have no groups available, kill them.
			for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL; )
			{
				CCall* pCall = m_lstCalls.GetNext(pos);
				if (pCall->GetCallState() == CCall::Ringing ||
					pCall->GetCallState() == CCall::Alerting)
				{
					if (pQueue != NULL)
						pQueue->TransferCall(NULL, pCall);
					else
						delete pCall;
				}
				else
					delete pCall;
			}
		}
		else
		{
			// If the agent is going Ready, see if there is a call which
			// can be answered.
			if (m_pAgent->GetState() == CAgent::Ready && !HasConnectedCall())
			{
				for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
				{
					CCall* pCall = m_lstCalls.GetNext(pos);
					if (pCall->GetCallState() == CCall::Ringing ||
						pCall->GetCallState() == CCall::Alerting)
					{
						pCall->SetCallState(CCall::Connected);
						break;
					}
				}
			}

			// Change the lamp states
			for (int i = 0; i < 2; i++)
			{
				enum CAgent::AgentState as = (i == 0) ? m_pAgent->GetState() : m_pAgent->GetNextState();
				if (as == CAgent::Ready)
				{
					m_arrLamps[LAMP_READY]->SetState(true);
					m_arrLamps[LAMP_INWORK]->SetState(false);
					break;
				}
				else if (as == CAgent::NotReady)
				{
					m_arrLamps[LAMP_READY]->SetState(false);
					m_arrLamps[LAMP_INWORK]->SetState(false);
					break;
				}
				else if (as == CAgent::InCallWork)
				{
					m_arrLamps[LAMP_READY]->SetState(false);
					m_arrLamps[LAMP_INWORK]->SetState(true);
					break;
				}
			}
		}
	}

	// Send an event concerning the display
	if (m_pAgent == NULL || (m_pAgent->GetState() != CAgent::BusyACD &&
		m_pAgent->GetState() != CAgent::Busy))
		BuildDisplay();

}// CStation::OnAgentStateChanged

/*****************************************************************************
** Procedure:  CStation::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an incoming call
**
*****************************************************************************/
void CStation::OnReceiveCall(CCall* pCall_Source)
{
	ASSERT (GetAgent() != NULL);
	enum CAgent::AgentState as = m_pAgent->GetState();
	CLineDevice::OnReceiveCall(pCall_Source);

	// If our agent is in the READY state, accept the call.
	if (as == CAgent::Ready)
		pCall_Source->SetCallState(CCall::Connected);

	// Send an event concerning the display
	BuildDisplay();

}// CLineDevice::OnReceiveCall

/*****************************************************************************
** Procedure:  CStation::TransferCall
**
** Arguments: 'pCall_Consult' - Consultation call which is being dissolved
**            'pCall_Source' - Call being transferred
**
** Returns:    void
**
** Description: This function is a notification for an incoming transfer/route
**
*****************************************************************************/
bool CStation::TransferCall(CCall* pCall_Consult, CCall* pCall_Source)
{
	bool fSuccess = false;
	if (GetAgent() != NULL)
	{
		enum CAgent::AgentState as = m_pAgent->GetState();
		fSuccess = CLineDevice::TransferCall(pCall_Consult, pCall_Source);
		if (fSuccess)
		{
			// Accept the call if we were in the READY state
			if (pCall_Consult == NULL && as == CAgent::Ready)
				pCall_Source->SetCallState(CCall::Connected);
		}

		// Send an event concerning the display
		BuildDisplay();
	}

	return fSuccess;

}// CStation::TransferCall

/*****************************************************************************
** Procedure:  CStation::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CStation pointer
**
** Description:  This static function displays a UI dialog (CStationDlg)
**               and creates a CStation object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CStation* CStation::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	CStationDlg dlg(pwndOwner, pDoc);
	if (dlg.DoModal() == IDOK)
	{
		CString strDescription = dlg.m_strDescription;
		if (strDescription.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFLINENAME);
			strDescription.Format(strBuff, dlg.m_dwExtension);
		}

		// Create the station
		CStation* pStation = new CStation(dlg.m_dwExtension, strDescription);

		// Add the available groups for logon
		pStation->m_arrGroups.RemoveAll();
		pStation->m_arrGroups.Copy(dlg.m_arrGroups);

		// Return the new station
		return pStation;
	}
	return NULL;

}// CStation::Create

/*****************************************************************************
** Procedure:  CStation::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CStationDlg)
**               and changes the existing CStation information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CStation::Edit(CWnd* pwndOwner)
{
	CJTDocument* pDoc = GetDocument();
	CStationDlg dlg(pwndOwner, pDoc);

	dlg.m_dwExtension = GetLineID();
	dlg.m_strDescription = GetLineName();
	dlg.m_arrGroups.Copy(m_arrGroups);
	
	// Show the dialog.  If it is dismissed with an OK result,
	// change our station information.
	if (dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strDescription;
		if (m_strName.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFLINENAME);
			m_strName.Format(strBuff, dlg.m_dwExtension);
		}

		// Add the available groups for logon
		m_arrGroups.RemoveAll();
		m_arrGroups.Copy(dlg.m_arrGroups);

		// Let the UI refresh
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return true;
	}
	return false;

}// CStation::Edit

/*****************************************************************************
** Procedure:  CStation::LogOnAgent
**
** Arguments: 'pAgent' - Agent to log into this line
**
** Returns:    true/false result whether agent logged on
**
** Description:  This function logs the agent into the line and given groups.
**
*****************************************************************************/
bool CStation::LogOnAgent(CAgent* pAgent, DWORD dwGroup1, DWORD dwGroup2)
{
	// Validate that this agent/groups may log onto this station
	if (m_arrGroups.GetSize() > 0 &&
		dwGroup1 != 0 || dwGroup2 != 0)
	{
		for (int i = 0; i < m_arrGroups.GetSize(); i++)
		{
			CAgentGroup* pGroup = GetDocument()->FindAgentGroup(m_arrGroups[i]);
			if (pGroup != NULL && pGroup->GetGroupID() == dwGroup1 ||
				pGroup->GetGroupID() == dwGroup2)
				break;
		}

		// If agent is not allowed, exit.
		if (i == m_arrGroups.GetSize())
			return false;
	}

	// If this is a request to re-logon an agent, simply change the
	// agent groups.
	if (m_pAgent != NULL && m_pAgent == pAgent)
	{
		// First logout of all the groups.
		CLineDevice* pLine = GetDocument()->GetFirstLine();
		while (pLine != NULL)
		{
			CACDQueue* pQueue = dynamic_cast<CACDQueue*>(pLine);
			if (pQueue != NULL)
				pQueue->OnAgentStateChanged(pAgent, CAgent::SignedOut);
			pLine = GetDocument()->GetNextLine();
		}
	}

	// Validate the request
	else if (m_pAgent != NULL || pAgent == NULL || pAgent->GetStation() != NULL)
		return false;

	// Get the agent group
	CAgentGroup* pGroup = NULL;
	if (dwGroup1 != 0)
	{
		pGroup = GetDocument()->FindAgentGroup(dwGroup1);
		if (pGroup == NULL || !pGroup->IsAgentInGroup(pAgent))
			return false;
	}

	// Assign the agent to this station.  This will cause an
	// agent-state-change to be sent for this station/agent.
	m_pAgent = pAgent;
	pAgent->AssociateStation(this);

	// Now associate the groups for this agent.
	if (pGroup != NULL)
	{
		CACDQueue* pQueue = pGroup->GetRelatedQueue();
		if (pQueue != NULL)
			pQueue->AddAgent(pAgent);
		else dwGroup1 = 0;
	}
	else dwGroup1 = 0;

	// Now do the secondary group
	if (dwGroup2)
	{
		pGroup = GetDocument()->FindAgentGroup(dwGroup2);
		if (pGroup != NULL && pGroup->IsAgentInGroup(pAgent))
		{
			CACDQueue* pQueue = pGroup->GetRelatedQueue();
			if (pQueue != NULL)
				pQueue->AddAgent(pAgent);
			else dwGroup2 = 0;
		}
		else dwGroup2 = 0;
	}

	// Send the agent group update
	SendEvent(GetLineID(), PBXEVT_AGC, GetExtension(), m_pAgent->GetAgentId(), dwGroup1, dwGroup2);
	return true;

}// CStation::LogOnAgent

/*****************************************************************************
** Procedure:  CStation::OnCallStateChange
**
** Arguments: 'pCall' - Call changing state
**            'csNewState' - New state
**
** Returns:    void
**
** Description:  Notification from a call about a change in state
**
*****************************************************************************/
void CStation::OnCallStateChange(CCall* pCall)
{
	// Pass through
	CLineDevice::OnCallStateChange(pCall);
	enum CCall::CallState csNewState = pCall->GetCallState();

	// If this is the first call, mark our hookswitch state as OFFHOOK
	if (pCall->IsActiveState())
	{
		for (int i = 0; i < GetCallCount(); i++)
		{
			CCall* pCallT = GetCallByPosition(i);
			if (pCallT != pCall && pCallT->IsActiveState())
				break;
		}
		if (i == GetCallCount())
			SendEvent(GetLineID(), PBXEVT_PHC, GetExtension(), _T('F'));
	}

	// If we now have no calls and we have an agent logged on,
	// then force the agent into it's next proper state
	if (GetAgent() != NULL)
	{
		if (csNewState != CCall::Disconnected)
		{
			// Set our agent state
			if (m_pAgent->GetState() != CAgent::Busy &&
				m_pAgent->GetState() != CAgent::BusyACD)
			{
				if (csNewState == CCall::Dialing)
					m_pAgent->SetState(CAgent::Busy);
				else if (m_pAgent->GetState() == CAgent::Ready)
					m_pAgent->SetState(CAgent::BusyACD);
			}
		}
	}

	// Send an event concerning the display
	BuildDisplay();

}// CStation::OnCallStateChange

/*****************************************************************************
** Procedure:  CStation::BuildDisplay
**
** Arguments: void
**
** Returns:    String representing display contents
**
** Description:  Builds the current display contents
**
*****************************************************************************/
void CStation::BuildDisplay()
{
	static CString g_strDisplay;
	CString strLine1, strLine2(_T(' '), 40);

	// If we have no calls, then it is in the form of:
	// AGENTSTATE           DATE/TIME
	if (GetCallCount() == 0)
	{
		// Format the date/time entry
		CString strDateTime = CTime::GetCurrentTime().Format(_T("%m/%d %H:%M:%S"));
		if (GetAgent() == NULL)
			strLine1.LoadString(IDS_INACTIVE);
		else
			strLine1 = m_pAgent->GetStateText();

		strLine1 += CString(_T(' '), 40 - strDateTime.GetLength() - strLine1.GetLength());
		strLine1 += strDateTime;
	}

	// We have calls in our call array
	else
	{
		CString strPending, strHolding;
		strPending.LoadString(IDS_PENDING);
		strHolding.LoadString(IDS_HOLDING);

		// Count the pending calls and active
		int iPendCalls = 0, iActiveCalls = 0, iHolding = 0;
		CCall* pActive = NULL;
		for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
		{
			CCall* pCall = m_lstCalls.GetNext(pos);
			if (pCall->GetCallState() == CCall::Ringing ||
				pCall->GetCallState() == CCall::Alerting)
				iPendCalls++;
			else if (pCall->GetCallState() == CCall::Holding)
				iHolding++;
			else if (pCall->GetCallState() == CCall::Connected ||
				pCall->GetCallState() == CCall::Dialing ||
				pCall->GetCallState() == CCall::WaitForAnswer ||
				pCall->GetCallState() == CCall::OtherSideHolding ||
				pCall->GetCallState() == CCall::Busy)
			{
				pActive = pCall;
				iActiveCalls++;
			}
		}

		// If we have no active calls (only pending/holding)
		if (iActiveCalls == 0)
		{
			// Use the first alerting call for display information
			for (pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
			{
				CCall* pCall = m_lstCalls.GetNext(pos);
				if (pCall->GetCallState() == CCall::Alerting)
				{
					pActive = pCall;
					break;
				}
			}

			// Use the active call to show information
			if (pActive != NULL)
			{
				CString strCallInfo;
				if (pActive->GetCallerIDInfo().GetNumber() == GetExtension())
					strCallInfo = pActive->GetANI();
				else
				{
					// Transferred call?
					if (pActive->GetCalledIDInfo().GetNumber() != GetExtension())
						strCallInfo = pActive->GetANI();
					else
						strCallInfo = pActive->GetDNIS();
				}

				int nPos = strCallInfo.Find(_T(':'));
				if (nPos > 0)
					strCallInfo.SetAt(nPos, _T(' '));
				else if (nPos == 0)
					strCallInfo = strCallInfo.Mid(1);
				strLine1 = strCallInfo;
				strLine1 += CString(_T(' '), 40-strLine1.GetLength());
			}
			else
			{
				// Format the date/time entry
				CString strDateTime = CTime::GetCurrentTime().Format(_T("%m/%d %H:%M:%S"));
				if (GetAgent() == NULL)
					strLine1.LoadString(IDS_INACTIVE);
				else
					strLine1 = m_pAgent->GetStateText();

				strLine1 += CString(_T(' '), 40 - strDateTime.GetLength() - strLine1.GetLength());
				strLine1 += strDateTime;
			}

			if (iPendCalls > 0 || iHolding > 0)
			{
				CString strBuff1, strBuff2;
				strBuff1.Format(strHolding, iHolding);
				strBuff2.Format(strPending, iPendCalls);
				strLine2 = CString(_T(' '), 40-(strBuff1.GetLength() + strBuff2.GetLength() + 1));
				strLine2 += strBuff1;
				strLine2 += _T(' ');
				strLine2 += strBuff2;
			}
		}

		// If we are in a conference, show different information
		if (iActiveCalls > 1)
		{
			CString strCallInfo;
			strLine1.LoadString(IDS_CONFERENCED);
			strLine2 = _T("");
			for (pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
			{
				CCall* pCall = m_lstCalls.GetNext(pos);
				if (pCall->GetCallerIDInfo().GetNumber() == GetExtension())
					strCallInfo = pCall->GetCalledIDInfo().GetNumber();
				else
					strCallInfo = pCall->GetCallerIDInfo().GetNumber();

				if (strLine2.IsEmpty() == FALSE)
					strLine2 += _T(",");
				strLine2 += strCallInfo;
			}
			if (strLine2.GetLength() < 40)
				strLine2 = CString(_T(' '), 40-strLine2.GetLength()) + strLine2;
			else
				strLine2 = strLine2.Left(40);
		}
		else if (iActiveCalls == 1)
		{
			CString strCallInfo;
			if (pActive->GetCallerIDInfo().GetNumber() == GetExtension())
				strCallInfo = pActive->GetANI();
			else
			{
				// Transferred call?
				if (pActive->GetCalledIDInfo().GetNumber() != GetExtension())
					strCallInfo = pActive->GetANI();
				else
					strCallInfo = pActive->GetDNIS();
			}

			int nPos = strCallInfo.Find(_T(':'));
			if (nPos > 0)
				strCallInfo.SetAt(nPos, _T(' '));
			else if (nPos == 0)
				strCallInfo = strCallInfo.Mid(1);

			strLine1 = strCallInfo;
			strLine1 += CString(_T(' '), 40-strLine1.GetLength());

			if (iPendCalls > 0 || iHolding > 0)
			{
				CString strBuff1, strBuff2;
				strBuff1.Format(strHolding, iHolding);
				strBuff2.Format(strPending, iPendCalls);
				strLine2 = CString(_T(' '), 40-(strBuff1.GetLength() + strBuff2.GetLength() + 1));
				strLine2 += strBuff1;
				strLine2 += _T(' ');
				strLine2 += strBuff2;
			}
		}
	}

	// Upper case both strings
	strLine1.MakeUpper();
	strLine2.MakeUpper();

	// Build the final display string
	CString strNewDisplay = CString(strLine1+strLine2);
	if (strNewDisplay != g_strDisplay)
	{
		g_strDisplay = strNewDisplay;
		SendEvent(GetLineID(), PBXEVT_PDC, GetExtension(), g_strDisplay);
	}

}// CStation::BuildDisplay
