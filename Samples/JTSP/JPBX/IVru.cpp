/*******************************************************************/
//
// IVRU.CPP
//
// Voice-response unit emulator object
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
#include "ivru.h"
#include "base.h"
#include "acdqueue.h"
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
IMPLEMENT_SERIAL(CVRU, CLineDevice, 1)

/**************************************************************************
** CVRUDlg
**
** User-interface dialog for the vru object
**
***************************************************************************/
class CVRUDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CVRUDlg)
	enum { IDD = IDD_VRU };
	CString	m_strDescription;
	DWORD	m_dwExtension;
	CJTDocument* m_pDocument;
	bool m_fEditExtension;
	CComboBox	m_cbLines;
	CEdit m_edtWaitTime;
	CSpinButtonCtrl	m_ctlSpinner;
	CComboBox m_cbCommands;
	DWORD m_dwTransferTo;
	int	m_iSeconds;
	int	m_iCommand;
	//}}AFX_DATA

// Construction
public:
	CVRUDlg(CWnd* pParent, CJTDocument* pDoc);
	CJTDocument* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVRUDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CVRUDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCommands();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CVRUDlg::CVRUDlg
**
** Arguments: 'pParent' - Parent for the dialog
**            'pDoc' - Document
**
** Returns:    void
**
** Description:  Dialog constructor
**
*****************************************************************************/
CVRUDlg::CVRUDlg(CWnd* pParent, CJTDocument* pDoc)
	: CBaseDialog(CVRUDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVRUDlg)
	m_strDescription = _T("");
	m_dwExtension = 0;
	m_pDocument = pDoc;
	m_fEditExtension = TRUE;
	m_dwTransferTo = 0;
	m_iSeconds = 0;
	m_iCommand = -1;
	//}}AFX_DATA_INIT

}// CVRUDlg::CVRUDlg

/*****************************************************************************
** Procedure:  CVRUDlg::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to CJTDocument object
**
** Description:  This function returns a pointer to the document associated
**               with the agent we are editing.
**
*****************************************************************************/
CJTDocument* CVRUDlg::GetDocument() 
{ 
	return m_pDocument; 

}// CVRUDlg::GetDocument

/*****************************************************************************
** Procedure:  CVRUDlg::OnInitDialog
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
BOOL CVRUDlg::OnInitDialog() 
{
	static UINT arrCommands[] = {
		IDS_RP_DONOTHING,
		IDS_RP_TRANSFERTO,
		IDS_RP_DROP,
		0
	};

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

	// Fill our command information
	for (int i = 0; arrCommands[i] != 0; i++)
	{
		CString strCommand;
		strCommand.LoadString(arrCommands[i]);
		m_cbCommands.AddString(strCommand);
	}
	m_cbCommands.SetCurSel(m_iCommand);

	// Fill in our lines combo box
	CLineDevice* pLine = GetDocument()->GetFirstLine();
	while (pLine != NULL)
	{
		if (dynamic_cast<CACDQueue*>(pLine) != NULL ||
			dynamic_cast<CStation*>(pLine) != NULL)
		{
			CString strText;
			strText.Format(_T("%04d - %s"), pLine->GetLineID(), pLine->GetLineName());
			int iPos = m_cbLines.AddString(strText);
			ASSERT (iPos != CB_ERR);
			m_cbLines.SetItemData(iPos, pLine->GetLineID());
			if (pLine->GetLineID() == m_dwTransferTo)
				m_cbLines.SetCurSel(iPos);
		}
		pLine = GetDocument()->GetNextLine();
	}

	// Limit our text
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DNUM))->LimitText(EXTENSION_SIZE);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DESCRIPTION))->LimitText(LINENAME_SIZE);
	m_edtWaitTime.LimitText(3);

	// Set our updown control to go the right way
	m_ctlSpinner.SetRange(0, 999);

	// Set our enabled controls
	OnSelchangeCommands();

	// If we have an extension already, don't allow it to be changed.
	if (!m_fEditExtension)
	{
		GetDlgItem(IDC_DNUM)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	GotoDlgCtrl(GetDlgItem(IDC_DESCRIPTION));
	return FALSE;

}// CVRUDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CVRUDlg::DoDataExchange
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
void CVRUDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVRUDlg)
	DDX_Control(pDX, IDC_LINES, m_cbLines);
	DDX_Control(pDX, IDC_WAITTIME, m_edtWaitTime);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpinner);
	DDX_Control(pDX, IDC_COMMANDS, m_cbCommands);
	DDX_Text(pDX, IDC_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_WAITTIME, m_iSeconds);
	DDX_Text(pDX, IDC_DNUM, m_dwExtension);
	DDX_CBIndex(pDX, IDC_COMMANDS, m_iCommand);
	//}}AFX_DATA_MAP

}// CVRUDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CVRUDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CVRUDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CVRUDlg)
	ON_EN_CHANGE(IDC_DESCRIPTION, OnChange)
	ON_CBN_SELCHANGE(IDC_COMMANDS, OnSelchangeCommands)
	ON_EN_CHANGE(IDC_WAITTIME, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CVRUDlg::OnSelchangeCommands
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user changes the command
**              associated with the route point
**
*****************************************************************************/
void CVRUDlg::OnSelchangeCommands() 
{
	OnChange();

	UpdateData(TRUE);
	if (m_iCommand == CVRU::TransferTo)
	{
		m_cbLines.EnableWindow(TRUE);
	}
	else
	{
		m_cbLines.SetCurSel(-1);
		m_cbLines.EnableWindow(FALSE);
	}

}// CVRUDlg::OnSelchangeCommands

/*****************************************************************************
** Procedure:  CVRUDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CVRUDlg::OnOK() 
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

		// Get the transfer to
		if (m_iCommand == CVRU::TransferTo)
		{
			int iCurSel = m_cbLines.GetCurSel();
			if (iCurSel == CB_ERR)
			{
				AfxMessageBox(IDS_MISSINGTRANSFERTO);
				GotoDlgCtrl(&m_cbLines);
				return;
			}
			m_dwTransferTo = m_cbLines.GetItemData(iCurSel);
		}

		CBaseDialog::OnOK();
	}

}// CVRUDlg::OnOK

/*****************************************************************************
** Procedure:  CVRUDlg::OnChange
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
void CVRUDlg::OnChange() 
{
	if (GetDlgItem(IDOK) != NULL)
	{
		UpdateData(TRUE);
		GetDlgItem(IDOK)->EnableWindow(m_dwExtension > 0);
	}

}// CVRUDlg::OnChange

/*****************************************************************************
** Procedure:  CVRU::CVRU
**
** Arguments: 'dwID' - Station id
**            'pszDescription' - Description for station
**
** Returns:    void
**
** Description:  Parameterized constructor
**
*****************************************************************************/
CVRU::CVRU(DWORD dwLineID, LPCTSTR pszDescription) :
	CLineDevice(dwLineID, pszDescription), 
	m_wSeconds(0), m_dwQueue(0), m_vruCommand(DoNothing)
{
}// CVRU::CVRU

/*****************************************************************************
** Procedure:  CVRU::CVRU
**
** Arguments: void
**
** Returns:    void
**
** Description:  No parameter constructor for the queue device
**
*****************************************************************************/
CVRU::CVRU() : CLineDevice(), m_wSeconds(0), m_dwQueue(0), m_vruCommand(DoNothing)
{
}// CVRU::CVRU

/*****************************************************************************
** Procedure:  CVRU::~CVRU
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line device
**
*****************************************************************************/
CVRU::~CVRU()
{
}// CVRU::~CVRU

/*****************************************************************************
** Procedure:  CVRU::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CVRU::Serialize( CArchive& archive )
{
	CLineDevice::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << m_wSeconds;
		archive << m_dwQueue;
		archive << static_cast<BYTE>(m_vruCommand);
	}
	else
	{
		BYTE b1;
		archive >> m_wSeconds;
		archive >> m_dwQueue;
		archive >> b1;
		m_vruCommand = static_cast<Command>(b1);
	}


}// CVRU::Serialize

/*****************************************************************************
** Procedure:  CVRU::CanAddCall
**
** Arguments: void
**
** Returns:    true/false if we can add the call
**
** Description: This member function is called to determine if it is ok
**              to add a new call to the station object
**
*****************************************************************************/
bool CVRU::CanAddCall()
{
	// Only have 1 address per line
	return (GetCallCount() == 1) ? false : true;
	
}// CVRU::CanAddCall

/*****************************************************************************
** Procedure:  CVRU::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an outgoing call placed
**              on this RP.
**
*****************************************************************************/
void CVRU::OnReceiveCall(CCall* pCall_Source)
{
	// Map another copy of the same call on this line
	CCall* pCall_Dest = new CCall(this, *pCall_Source);

	// Mark this call as ringing on the VRU
	pCall_Dest->SetCallState(CCall::Ringing);

	// If we are supposed to transfer any incoming call on the VRU
	// immediately, then do so.
	if (m_wSeconds == 0 && m_vruCommand == TransferTo && m_dwQueue != 0)
	{
		CLineDevice* pLine = GetDocument()->GetLineByID(m_dwQueue);
		if (pLine != NULL)
			pLine->TransferCall(NULL, pCall_Source);
	}

	// Otherwise go connected with the call.
	else
		pCall_Dest->SetCallState(CCall::Connected);

}// CVRU::OnReceiveCall

/*****************************************************************************
** Procedure:  CVRU::OnTimer
**
** Arguments: void
**
** Returns:    void
**
** Description: This member function is called peridically to allow the
**              VRU to perform various tasks
**
*****************************************************************************/
void CVRU::OnTimer()
{
	// Scan our call list and try to transfer any calls which have
	// been sitting too long.
	if (m_vruCommand != DoNothing &&	GetCallCount() > 0)
	{
		CLineDevice* pLine = NULL;
		if (m_vruCommand == TransferTo)
			pLine = GetDocument()->GetLineByID(m_dwQueue);

		for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
		{
			CCall* pCall = m_lstCalls.GetNext(pos);
			if (pCall->SecondsInState() > m_wSeconds)
			{
				if (pLine != NULL)
					pLine->TransferCall(NULL, pCall);
				else if (m_vruCommand == Drop)
				{
					delete pCall;
				}
			}
		}
	}

}// CVRU::OnTimer

/*****************************************************************************
** Procedure:  CVRU::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CVRU pointer
**
** Description:  This static function displays a UI dialog (CVRUDlg)
**               and creates a CVRU object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CVRU* CVRU::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	CVRUDlg dlg(pwndOwner, pDoc);
	if (dlg.DoModal() == IDOK)
	{
		CString strDescription = dlg.m_strDescription;
		if (strDescription.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFVRUNAME);
			strDescription.Format(strBuff, dlg.m_dwExtension);
		}

		// Create the vru
		CVRU* pVru = new CVRU(dlg.m_dwExtension, strDescription);

		// Save off all the information
		pVru->m_wSeconds = static_cast<WORD>(dlg.m_iSeconds);
		pVru->m_dwQueue = dlg.m_dwTransferTo;
		pVru->m_vruCommand = static_cast<Command>(dlg.m_iCommand);

		// Return the new vru
		return pVru;
	}
	return NULL;

}// CVRU::Create

/*****************************************************************************
** Procedure:  CVRU::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CVRUDlg)
**               and changes the existing CVRU information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CVRU::Edit(CWnd* pwndOwner)
{
	CJTDocument* pDoc = GetDocument();
	CVRUDlg dlg(pwndOwner, pDoc);

	dlg.m_dwExtension = GetLineID();
	dlg.m_strDescription = GetLineName();
	dlg.m_iSeconds = m_wSeconds;
	dlg.m_dwTransferTo = m_dwQueue;
	dlg.m_iCommand = m_vruCommand;
	
	// Show the dialog.  If it is dismissed with an OK result,
	// change our station information.
	if (dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strDescription;
		if (m_strName.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFVRUNAME);
			m_strName.Format(strBuff, dlg.m_dwExtension);
		}

		m_wSeconds = static_cast<WORD>(dlg.m_iSeconds);
		m_dwQueue = dlg.m_dwTransferTo;
		m_vruCommand = static_cast<Command>(dlg.m_iCommand);
		
		// Let the UI refresh
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return true;
	}
	return false;

}// CVRU::Edit
