/*******************************************************************/
//
// AGENT.CPP
//
// Agent management objects and dialogs
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
#include "document.h"
#include "Agent.h"
#include "AgentGroup.h"
#include "acdQueue.h"
#include "station.h"

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
IMPLEMENT_SERIAL(CAgent, CUIObject, 1)

/**************************************************************************
** CAgentDlg
**
** Dialog to enter and edit information about a single CAgent
** object.  This is invoked using the CAgent::Create and 
** CAgent::Edit methods.
**
***************************************************************************/
class CAgentDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CAgentDlg)
	enum { IDD = IDD_AGENT };
	CString	m_strID;
	CString	m_strPassword;
	CUIntArray m_arrGroups;
	CButton	m_btnOK;
	CListBox	m_lbGroups;
	CJTDocument* m_pDocument;
	BOOL	m_fAutoReady;
	CAgent* m_pAgent;
	//}}AFX_DATA

// Construction
public:
	CAgentDlg( CWnd* pParent, CJTDocument* pDoc );

// Publics
public:
	CJTDocument* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAgentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChange();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CAgent::~CAgent
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor
**
*****************************************************************************/
CAgent::~CAgent()
{
	// Unload from any line.
	SetState(CAgent::SignedOut);

}// CAgent::~CAgent

/*****************************************************************************
** Procedure:  CAgent::Serialize
**
** Arguments: 'archive' - CArchive object to dump agent ito
**
** Returns:    void
**
** Description:  This function stores this agent object to some persistant
**               media (typically a file).
**
*****************************************************************************/
void CAgent::Serialize( CArchive& archive )
{
	CUIObject::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << m_strID;
		archive << m_strPassword;
		archive << GetFlags();

		// Gather the agent group information we are about
		// to store in the archive.
		CDWordArray arrGroups;
		for (int i = 0; i < GetDocument()->GetAgentGroupCount(); i++)
		{
			CAgentGroup* pGroup = GetDocument()->GetAgentGroup(i);
			if (pGroup != NULL && pGroup->IsAgentInGroup(this))
				arrGroups.Add(pGroup->GetGroupID());
		}

		// Dump the count of groups and then each group identifier
		archive << static_cast<WORD>(arrGroups.GetSize());
		for (i = 0; i < arrGroups.GetSize(); i++)
			archive << arrGroups[i];

	}
	else
	{
		archive >> m_strID;
		archive >> m_strPassword;
		archive >> m_afFlags;

		// Read in the agent groups this agent belongs to.
		// Note: the agent groups MUST have been serialized already!
		WORD wCount;
		archive >> wCount;
		for (int i = 0; i < wCount; i++)
		{
			DWORD dwGroupID;
			archive >> dwGroupID;
			CAgentGroup* pGroup = GetDocument()->FindAgentGroup(dwGroupID);
			if (pGroup != NULL)
				pGroup->AddAgent(this);
		}
	}

}// CAgent::Serialize

/*****************************************************************************
** Procedure:  CAgent::GotoNextState
**
** Arguments: void
**
** Returns:    void
**
** Description:  This function sets next state for this agent
**
*****************************************************************************/
void CAgent::GotoNextState()
{
	if (m_anState == Unknown)
		m_anState = ((m_afFlags & AutoReady) == AutoReady) ? Ready : NotReady;
	SetState(m_anState);
	m_anState = Unknown;

}// CAgent::GotoNextState

/*****************************************************************************
** Procedure:  CAgent::SetNextState
**
** Arguments: 'as' - Agent state
**
** Returns:    void
**
** Description:  This function sets next state for this agent
**
*****************************************************************************/
void CAgent::SetNextState(AgentState as)
{
	if (m_aState == Busy || m_aState == BusyACD)
		m_anState = as;

}// CAgent::SetNextState

/*****************************************************************************
** Procedure:  CAgent::SetState
**
** Arguments: 'as' - Agent state
**
** Returns:    void
**
** Description:  This function sets the dynamic agent state
**
*****************************************************************************/
void CAgent::SetState(AgentState as)
{
	bool fSendNotify = (m_aState != as);
	m_aState = as;

	// If the agent is now idle and ready, move this agent to the free list.
	if (as == Ready)
		GetDocument()->OnAgentIdle(this);
	else
		GetDocument()->OnAgentNotIdle(this);

	// Go through all the groups this agent is part of and inform
	// them that our state has changed.
	for (int i = 0; i < GetDocument()->GetAgentGroupCount(); i++)
	{
		CAgentGroup* pGroup = GetDocument()->GetAgentGroup(i);
		if (pGroup != NULL && pGroup->IsAgentInGroup(this))
			pGroup->GetRelatedQueue()->OnAgentStateChanged(this, as);
	}

	// Tell the attached station
	if (m_pStation != NULL)
	{
		if (fSendNotify)
			NotifyState();
		m_pStation->OnAgentStateChanged();
	}

	// Force an update of the UI
	GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);

}// CAgent::SetState

/*****************************************************************************
** Procedure:  CAgent::NotifyState
**
** Arguments: void
**
** Returns:    void
**
** Description:  This sends an event concerning the agent state
**
*****************************************************************************/
void CAgent::NotifyState() const
{
	static char chStates[] = { 'U', 'S', 'R', 'N', 'W', 'A', 'B' };
	if (m_pStation != NULL)
		SendEvent(m_pStation->GetLineID(), PBXEVT_ASC, 
			m_pStation->GetExtension(), GetAgentId(), chStates[m_aState]);

}// CAgent::NotifyState

/*****************************************************************************
** Procedure:  CAgent::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CAgent pointer
**
** Description:  This static function displays a UI dialog (CAgentDlg)
**               and creates a CAgent object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CAgent* CAgent::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	CAgentDlg dlg(pwndOwner, pDoc);
	if (dlg.DoModal() == IDOK)
	{
		CAgent* pAgent = new CAgent(dlg.m_strID, dlg.m_strPassword);
		for (int i = 0; i < dlg.m_arrGroups.GetSize(); i++)
		{
			CAgentGroup* pGroup = pDoc->FindAgentGroup(dlg.m_arrGroups[i]);
			if (pGroup != NULL)
				pGroup->AddAgent(pAgent);
		}

		if (dlg.m_fAutoReady)
			pAgent->m_afFlags |= AutoReady;
		else
			pAgent->m_afFlags &= ~AutoReady;

		return pAgent;
	}
	return NULL;

}// CAgent::Create

/*****************************************************************************
** Procedure:  CAgent::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CAgentDlg)
**               and changes the existing CAgent information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CAgent::Edit(CWnd* pwndOwner)
{
	CJTDocument* pDoc = GetDocument();
	CAgentDlg dlg(pwndOwner, pDoc);

	dlg.m_strID = m_strID;
	dlg.m_strPassword = m_strPassword;
	dlg.m_pAgent = this;
	dlg.m_fAutoReady = ((GetFlags() & AutoReady) == AutoReady);

	// Load the agent groups.
	for (int i = 0; i < pDoc->GetAgentGroupCount(); i++)
	{
		CAgentGroup* pGroup = pDoc->GetAgentGroup(i);
		if (pGroup->IsAgentInGroup(this))
			dlg.m_arrGroups.Add(pGroup->GetGroupID());
	}
	
	// Show the dialog.  If it is dismissed with an OK result,
	// change our agent information.
	if (dlg.DoModal() == IDOK)
	{
		m_strID = dlg.m_strID;
		m_strPassword = dlg.m_strPassword;
		if (dlg.m_fAutoReady)
			m_afFlags |= AutoReady;
		else
			m_afFlags &= ~AutoReady;

		// Remove all references to the agent groups.
		for (int i = 0; i < pDoc->GetAgentGroupCount(); i++)
			pDoc->GetAgentGroup(i)->RemoveAgent(this);

		// Re-add them
		for (i = 0; i < dlg.m_arrGroups.GetSize(); i++)
		{
			CAgentGroup* pGroup = pDoc->FindAgentGroup(dlg.m_arrGroups[i]);
			if (pGroup != NULL)
				pGroup->AddAgent(this);
		}

		// Let the UI refresh
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return true;
	}
	return false;

}// CAgent::Edit

/*****************************************************************************
** Procedure:  CAgent::ContextMenu
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether context menu was popped-up
**
** Description:  This function displays a context menu for an agent object.
**
*****************************************************************************/
bool CAgent::ContextMenu(CWnd* pwndOwner)
{
	return RunPopup( IDM_ACTPROP, pwndOwner );

}// CAgent::ContextMenu

/*****************************************************************************
** Procedure:  CAgent::OnCommand
**
** Arguments: 'nCommand' - WM_COMMAND message which was intercepted
**
** Returns:    void
**
** Description:  This function processes the WM_COMMAND handler for the
**               agent objects.
**
*****************************************************************************/
void CAgent::OnCommand( UINT nCommand )
{
	switch (nCommand)
	{
		case IDM_CTX_PROPERTIES:
			Edit(AfxGetMainWnd());
			break;
		case IDM_CTX_REMOVE:
			if (AfxMessageBox(IDS_DELETE, MB_YESNO) == IDYES)
				GetDocument()->DeleteSingleObject(this);
			break;
	}

}// CAgent::OnCommand

/*****************************************************************************
** Procedure:  CAgent::GetStateText
**
** Arguments: void
**
** Returns:    CString containing text for current agent state
**
** Description:  This function examines the current state of the agent and
**               returns a textual string describing it.
**
*****************************************************************************/
CString CAgent::GetStateText() const
{
	static UINT uiStates[] = {
		IDS_AS_UNKNOWN,
		IDS_AS_SIGNEDOUT,
		IDS_AS_READY,
		IDS_AS_NOTREADY,
		IDS_AS_INCALLWORK,
		IDS_AS_BUSYACD,
		IDS_AS_BUSY
	};

	CString strState;
	if (!strState.LoadString(uiStates[GetState()]))
		strState.LoadString(IDS_AS_UNKNOWN);
	return strState;

}// CAgent::GetStateText

/*****************************************************************************
** Procedure:  CAgentDlg::CAgentDlg
**
** Arguments: 'pParent' - Parent window handle
**            'pDoc' - CJTDocument for displaying group information
**
** Returns:    void
**
** Description:  Constructor for the agent dialog
**
*****************************************************************************/
CAgentDlg::CAgentDlg(CWnd* pParent, CJTDocument* pDoc)
	: CBaseDialog(CAgentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentDlg)
	m_strID = _T("");
	m_strPassword = _T("");
	m_pDocument = pDoc;
	m_fAutoReady = FALSE;
	m_pAgent = NULL;
	//}}AFX_DATA_INIT

}// CAgentDlg::CAgentDlg

/*****************************************************************************
** Procedure:  CAgentDlg::DoDataExchange
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
void CAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentDlg)
	DDX_Text(pDX, IDC_NAME, m_strID);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_GROUPS, m_lbGroups);
	DDX_Check(pDX, IDC_AUTOREADY, m_fAutoReady);
	//}}AFX_DATA_MAP

}// CAgentDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CAgentDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAgentDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CAgentDlg)
	ON_EN_CHANGE(IDC_NAME, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CAgentDlg::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to CJTDocument object
**
** Description:  This function returns a pointer to the document associated
**               with the agent we are editing.
**
*****************************************************************************/
CJTDocument* CAgentDlg::GetDocument() 
{ 
	return m_pDocument; 

}// CAgentDlg::GetDocument

/*****************************************************************************
** Procedure:  CAgentDlg::OnInitDialog
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
BOOL CAgentDlg::OnInitDialog() 
{
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

	// Now determine which groups this agent is part of and
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
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_NAME))->LimitText(AGENTID_SIZE);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_PASSWORD))->LimitText(AGENTPW_SIZE);
	
	return TRUE;

}// CAgentDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CAgentDlg::OnChange
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
void CAgentDlg::OnChange() 
{
	UpdateData(TRUE);
	m_btnOK.EnableWindow(!m_strID.IsEmpty());

}// CAgentDlg::OnChange

/*****************************************************************************
** Procedure:  CAgentDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CAgentDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		// Check the agent-id name.
		m_strID.TrimRight();
		CAgent* pAgent = GetDocument()->FindAgent(m_strID);
		if (pAgent != NULL && pAgent != m_pAgent)
		{
			AfxMessageBox(IDS_AGENTEXISTS);
			GotoDlgCtrl(GetDlgItem(IDC_NAME));
			return;
		}

		// Gather the groups.
		m_arrGroups.RemoveAll();
		for (int iLB = 0; iLB < m_lbGroups.GetCount(); iLB++)
		{
			if (m_lbGroups.GetSel(iLB))
				m_arrGroups.Add(m_lbGroups.GetItemData(iLB));
		}
	}
	CBaseDialog::OnOK();

}// CAgentDlg::OnOK

