/*******************************************************************/
//
// AGENTGROUP.CPP
//
// Agent group management objects and dialogs
// Each agent group is considered an ACD QUEUE
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
#include "AgentGroup.h"
#include "AcdQueue.h"

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
IMPLEMENT_SERIAL(CAgentGroup, CUIObject, 1)

/**************************************************************************
** CAgentGroupDlg (internal)
**
** Agent group management dialog.  This allows for the creation
** of new agent groups and the modification of existing groups.
** It is invoked using the CAgentGroup::Create and CAgentGroup::Edit
** methods.
**
***************************************************************************/
class CAgentGroupDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CAgentGroupDlg)
	enum { IDD = IDD_AGENTGROUP };
	CString	m_strName;
	CTypedPtrArray<CObArray, CAgent*> m_arrAgents;
	CJTDocument* m_pDocument;
	CButton	m_btnOK;
	CListBox	m_lbAgents;
	//}}AFX_DATA

// Construction
public:
	CAgentGroupDlg(CWnd* pParent, CJTDocument* pDoc);

// Public
public:
	CJTDocument* GetDocument() { return m_pDocument; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgentGroupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CAgentGroup::Serialize
**
** Arguments: 'archive' - CArchive object to dump agent group ito
**
** Returns:    void
**
** Description:  This function stores this agent group object to some persistant
**               media (typically a file).
**
*****************************************************************************/
void CAgentGroup::Serialize( CArchive& archive )
{
	CUIObject::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << m_dwGroupID;
		archive << m_strName;
	}
	else
	{
		archive >> m_dwGroupID;
		archive >> m_strName;
	}

}// CAgentGroup::Serialize

/*****************************************************************************
** Procedure:  CAgentGroup::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CAgent pointer
**
** Description:  This static function displays a UI dialog (CAgentGroupDlg)
**               and creates a CAgentGroup object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CAgentGroup* CAgentGroup::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	CAgentGroupDlg dlg(pwndOwner, pDoc);
	if (dlg.DoModal() == IDOK)
	{
		DWORD dwQueueID = pDoc->GenerateQueueID();
		if (dwQueueID == 0xffffffff)
		{
			AfxMessageBox(IDS_TOOMANYOBJS);
			return NULL;
		}

		CAgentGroup* pAgentGroup = new 
			CAgentGroup(pDoc->GenerateAgentGroupID(), dlg.m_strName);

		// Add the agents.
		for (int i = 0; i < dlg.m_arrAgents.GetSize(); i++)
			pAgentGroup->AddAgent(dlg.m_arrAgents[i]);

		// Create a queue object to match this agent group.
		CACDQueue* pQueue = new CACDQueue(pAgentGroup, dwQueueID);
		pDoc->NotifyOnLine(pQueue);

		return pAgentGroup;
	}
	return NULL;

}// CAgentGroup::Create

/*****************************************************************************
** Procedure:  CAgent::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CAgentGroupDlg)
**               and changes the existing CAgentGroup information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CAgentGroup::Edit(CWnd* pwndOwner)
{
	CJTDocument* pDoc = GetDocument();
	CAgentGroupDlg dlg(pwndOwner, pDoc);

	// Copy the group information into the dialog
	dlg.m_strName = m_strName;
	for (int i = 0; i < m_arrAgents.GetSize(); i++)
		dlg.m_arrAgents.Add(m_arrAgents[i]);

	// Invoke the dialog
	if (dlg.DoModal() == IDOK)
	{
		// Copy the modified information into our group object
		m_strName = dlg.m_strName;
		m_arrAgents.RemoveAll();
		for (int i = 0; i < dlg.m_arrAgents.GetSize(); i++)
			m_arrAgents.Add(dlg.m_arrAgents[i]);
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return true;
	}
	return false;

}// CAgentGroup::Edit

/*****************************************************************************
** Procedure:  CAgentGroup::ContextMenu
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether context menu was popped-up
**
** Description:  This function displays a context menu for an agent group object.
**
*****************************************************************************/
bool CAgentGroup::ContextMenu(CWnd* pwndOwner)
{
	return RunPopup( IDM_ACTPROP, pwndOwner );

}// CAgentGroup::ContextMenu

/*****************************************************************************
** Procedure:  CAgentGroup::OnCommand
**
** Arguments: 'nCommand' - WM_COMMAND message which was intercepted
**
** Returns:    void
**
** Description:  This function processes the WM_COMMAND handler for the
**               agent group objects.
**
*****************************************************************************/
void CAgentGroup::OnCommand(UINT nCommand)
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

}// CAgentGroup::OnCommand

/*****************************************************************************
** Procedure:  CAgentGroupDlg::CAgentGroupDlg
**
** Arguments: 'pParent' - Parent window handle
**            'pDoc' - CJTDocument for displaying group information
**
** Returns:    void
**
** Description:  Constructor for the agent group dialog
**
*****************************************************************************/
CAgentGroupDlg::CAgentGroupDlg(CWnd* pParent, CJTDocument* pDoc)
	: CBaseDialog(CAgentGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentGroupDlg)
	m_strName = _T("");
	m_pDocument = pDoc;
	//}}AFX_DATA_INIT

}// CAgentGroupDlg::CAgentGroupDlg

/*****************************************************************************
** Procedure:  CAgentGroupDlg::DoDataExchange
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
void CAgentGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentGroupDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_MEMBERS, m_lbAgents);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP

}// CAgentGroupDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CAgentGroupDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAgentGroupDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CAgentGroupDlg)
	ON_EN_CHANGE(IDC_NAME, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CAgentGroupDlg::OnInitDialog
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
BOOL CAgentGroupDlg::OnInitDialog() 
{
	// Pass through to base class
	CBaseDialog::OnInitDialog();

	// Load all the agent names
	for (int i = 0; i < GetDocument()->GetAgentCount(); i++)
	{
		CAgent* pAgent = GetDocument()->GetAgent(i);
		int iPos = m_lbAgents.AddString(pAgent->GetAgentId());
		ASSERT(iPos != LB_ERR);
		m_lbAgents.SetItemDataPtr(iPos, pAgent);
	}
	
	// Select all our available agents
	for (i = 0; i < m_arrAgents.GetSize(); i++)
	{
		for (int iLB = 0; iLB < m_lbAgents.GetCount(); iLB++)
			if (m_arrAgents[i] == static_cast<CAgent*>(m_lbAgents.GetItemDataPtr(iLB)))
				m_lbAgents.SetSel(iLB, TRUE);
	}

	// Set the limits on our edit controls
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_NAME))->LimitText(GROUPNAME_SIZE);

	// Establish our enabled state
	OnChange();

	return TRUE;

}// CAgentGroupDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CAgentGroupDlg::OnChange
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
void CAgentGroupDlg::OnChange() 
{
	UpdateData(TRUE);
	m_btnOK.EnableWindow(!m_strName.IsEmpty());

}// CAgentGroupDlg::OnChange

/*****************************************************************************
** Procedure:  CAgentGroupDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CAgentGroupDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		m_arrAgents.RemoveAll();		
		for (int iLB = 0; iLB < m_lbAgents.GetCount(); iLB++)
		{
			if (m_lbAgents.GetSel(iLB))
				m_arrAgents.Add(static_cast<CAgent*>(m_lbAgents.GetItemDataPtr(iLB)));
		}
	}

	// Dismiss the dialog
	CBaseDialog::OnOK();

}// CAgentGroupDlg::OnOK

