/*******************************************************************/
//
// ROUTEPOINT.CPP
//
// Route-point management.  The route point supports any number
// of queueud "ringing" calls.  It may not answer calls and is
// used as a repository for calls until they are transferred to
// either a queue or other line.
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
#include "routepoint.h"
#include "acdqueue.h"
#include "station.h"
#include "trunk.h"
#include "ivru.h"

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
IMPLEMENT_SERIAL(CRoutePoint, CLineDevice, 1)

/**************************************************************************
** CRoutePointDlg
**
** User-interface dialog for the route point
**
***************************************************************************/
class CRoutePointDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CRoutePointDlg)
	enum { IDD = IDD_RPOINT };
	CComboBox	m_cbLines;
	CEdit m_edtWaitTime;
	CListBox m_lbTrunks;
	CSpinButtonCtrl	m_ctlSpinner;
	CComboBox m_cbCommands;
	BOOL m_fAllowProgramRoute;
	BOOL m_fEditExtension;
	BOOL m_fAutoRoute;
	CString	m_strDescription;
	DWORD m_dwExtension;
	DWORD m_dwTransferTo;
	int	m_iSeconds;
	CJTDocument* m_pDocument;
	int	m_iCommand;
	CDWordArray m_arrTrunks;
	//}}AFX_DATA

// Construction
public:
	CRoutePointDlg(CWnd* pParent, CJTDocument* pDoc);
	CJTDocument* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoutePointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRoutePointDlg)
	virtual void OnOK();
	afx_msg void OnAutoroute();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCommands();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CRoutePointDlg::CRoutePointDlg
**
** Arguments: 'pParent' - Parent for the dialog
**
** Returns:    void
**
** Description: Dialog constructor
**
*****************************************************************************/
CRoutePointDlg::CRoutePointDlg(CWnd* pParent, CJTDocument* pDoc)
	: CBaseDialog(CRoutePointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoutePointDlg)
	m_fEditExtension = TRUE;
	m_fAllowProgramRoute = TRUE;
	m_fAutoRoute = FALSE;
	m_strDescription = _T("");
	m_dwExtension = 0;
	m_dwTransferTo = 0;
	m_iSeconds = 0;
	m_pDocument = pDoc;
	m_iCommand = -1;
	//}}AFX_DATA_INIT
}

/*****************************************************************************
** Procedure:  CRoutePointDlg::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to CJTDocument object
**
** Description:  This function returns a pointer to the document associated
**               with the agent we are editing.
**
*****************************************************************************/
CJTDocument* CRoutePointDlg::GetDocument() 
{ 
	return m_pDocument; 

}// CRoutePointDlg::GetDocument

/*****************************************************************************
** Procedure:  CRoutePointDlg::OnInitDialog
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
BOOL CRoutePointDlg::OnInitDialog() 
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
			dynamic_cast<CStation*>(pLine) != NULL ||
			dynamic_cast<CVRU*>(pLine) != NULL)
		{
			CString strText;
			strText.Format(_T("%04d - %s"), pLine->GetLineID(), pLine->GetLineName());
			int iPos = m_cbLines.AddString(strText);
			ASSERT (iPos != CB_ERR);
			m_cbLines.SetItemData(iPos, pLine->GetLineID());
			if (pLine->GetLineID() == m_dwTransferTo)
				m_cbLines.SetCurSel(iPos);
		}
		else if (dynamic_cast<CTrunk*>(pLine) != NULL)
		{
			CString strText;
			strText.Format(_T("%04d - %s"), pLine->GetLineID(), pLine->GetLineName());
			int iPos = m_lbTrunks.AddString(strText);
			ASSERT (iPos != CB_ERR);
			m_lbTrunks.SetItemData(iPos, pLine->GetLineID());

			// Select it if necessary
			for (int i = 0; i < m_arrTrunks.GetSize(); i++)
			{
				if (pLine->GetLineID() == m_arrTrunks[i])
				{
					m_lbTrunks.SetSel(iPos, TRUE);
					break;
				}
			}
		}
		pLine = GetDocument()->GetNextLine();
	}

	// Limit our text
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DNUM))->LimitText(EXTENSION_SIZE);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DESCRIPTION))->LimitText(LINENAME_SIZE);
	m_edtWaitTime.LimitText(3);

	// Set our updown control to go the right way
	m_ctlSpinner.SetRange(0, 999);

	// Set the initial values for autoroute
	OnAutoroute();

	// If we have an extension already, don't allow it to be changed.
	if (!m_fEditExtension)
	{
		GetDlgItem(IDC_DNUM)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	GotoDlgCtrl(GetDlgItem(IDC_DESCRIPTION));
	return FALSE;

}// CRoutePointDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CRoutePointDlg::DoDataExchange
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
void CRoutePointDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoutePointDlg)
	DDX_Control(pDX, IDC_LINES, m_cbLines);
	DDX_Control(pDX, IDC_WAITTIME, m_edtWaitTime);
	DDX_Control(pDX, IDC_TRUNKS, m_lbTrunks);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpinner);
	DDX_Control(pDX, IDC_COMMANDS, m_cbCommands);
	DDX_Check(pDX, IDC_ALLOWROUTE, m_fAllowProgramRoute);
	DDX_Check(pDX, IDC_AUTOROUTE, m_fAutoRoute);
	DDX_Text(pDX, IDC_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_WAITTIME, m_iSeconds);
	DDX_Text(pDX, IDC_DNUM, m_dwExtension);
	DDX_CBIndex(pDX, IDC_COMMANDS, m_iCommand);
	//}}AFX_DATA_MAP

}// CRoutePointDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CRoutePointDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CRoutePointDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CRoutePointDlg)
	ON_BN_CLICKED(IDC_AUTOROUTE, OnAutoroute)
	ON_CBN_SELCHANGE(IDC_COMMANDS, OnSelchangeCommands)
	ON_EN_CHANGE(IDC_DESCRIPTION, OnChange)
	ON_EN_CHANGE(IDC_WAITTIME, OnChange)
	ON_BN_CLICKED(IDC_ALLOWROUTE, OnChange)
	ON_CBN_SELCHANGE(IDC_LINES, OnChange)
	ON_LBN_SELCHANGE(IDC_TRUNKS, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CRoutePointDlg::OnSelchangeCommands
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user changes the command
**              associated with the route point
**
*****************************************************************************/
void CRoutePointDlg::OnSelchangeCommands() 
{
	OnChange();

	UpdateData(TRUE);
	if (m_fAutoRoute && m_iCommand == CRoutePoint::TransferTo)
	{
		m_cbLines.EnableWindow(TRUE);
	}
	else
	{
		m_cbLines.SetCurSel(-1);
		m_cbLines.EnableWindow(FALSE);
	}

}// CRoutePointDlg::OnSelchangeCommands

/*****************************************************************************
** Procedure:  CRoutePointDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CRoutePointDlg::OnOK() 
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
		if (m_fAutoRoute && m_iCommand == CRoutePoint::TransferTo)
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

		// Fill our trunk array
		m_arrTrunks.RemoveAll();
		for (int i = 0; i < m_lbTrunks.GetCount(); i++)
		{
			if (m_lbTrunks.GetSel(i))
				m_arrTrunks.Add(static_cast<DWORD>(m_lbTrunks.GetItemData(i)));
		}

		CBaseDialog::OnOK();
	}

}// CRoutePointDlg::OnOK

/*****************************************************************************
** Procedure:  CRoutePointDlg::OnChange
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
void CRoutePointDlg::OnChange() 
{
	if (GetDlgItem(IDOK))
		GetDlgItem(IDOK)->EnableWindow(TRUE);

}// CRoutePointDlg::OnChange

/*****************************************************************************
** Procedure:  CRoutePointDlg::OnAutoroute
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the auto-route
**              button.  It enables/disables all the other controls
**
*****************************************************************************/
void CRoutePointDlg::OnAutoroute() 
{
	OnChange();
	UpdateData(TRUE);
	if (m_fAutoRoute)
	{
		m_edtWaitTime.EnableWindow(TRUE);
		m_ctlSpinner.EnableWindow(TRUE);
		m_cbCommands.EnableWindow(TRUE);
		OnSelchangeCommands();
	}
	else
	{
		m_edtWaitTime.EnableWindow(FALSE);
		m_ctlSpinner.EnableWindow(FALSE);
		m_cbLines.EnableWindow(FALSE);
		m_cbCommands.EnableWindow(FALSE);
	}

}// CRoutePointDlg::OnAutoroute

/*****************************************************************************
** Procedure:  CRoutePoint::CRoutePoint
**
** Arguments: void
**
** Returns:    void
**
** Description:  No parameter constructor for the route point device
**
*****************************************************************************/
CRoutePoint::CRoutePoint() : CLineDevice(), m_wSeconds(0), m_dwQueue(0), 
	m_fAllowRoute(true), m_rpCommand(DoNothing), m_fAutoRoute(false)
{
}// CRoutePoint::CRoutePoint

/*****************************************************************************
** Procedure:  CRoutePoint::CRoutePoint
**
** Arguments: 'dwID' - Station id
**            'pszDescription' - Description for station
**
** Returns:    void
**
** Description:  Parameterized constructor
**
*****************************************************************************/
CRoutePoint::CRoutePoint(DWORD dwLineID, LPCTSTR pszDescription) :
	CLineDevice(dwLineID, pszDescription), m_wSeconds(0), m_dwQueue(0), 
	m_fAllowRoute(true), m_rpCommand(DoNothing), m_fAutoRoute(false)
{
}// CRoutePoint::CRoutePoint

/*****************************************************************************
** Procedure:  CRoutePoint::~CRoutePoint
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line device
**
*****************************************************************************/
CRoutePoint::~CRoutePoint()
{
}// CRoutePoint::~CRoutePoint

/*****************************************************************************
** Procedure:  CRoutePoint::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CRoutePoint::Serialize( CArchive& archive )
{
	CLineDevice::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << m_wSeconds;
		archive << m_dwQueue;
		archive << m_fAllowRoute;
		archive << m_fAutoRoute;
		archive << static_cast<BYTE>(m_rpCommand);
		m_arrTrunks.Serialize(archive);
	}
	else
	{
		BYTE b1;
		archive >> m_wSeconds;
		archive >> m_dwQueue;
		archive >> m_fAllowRoute;
		archive >> m_fAutoRoute;
		archive >> b1;
		m_rpCommand = static_cast<Command>(b1);
		m_arrTrunks.Serialize(archive);
	}

}// CRoutePoint::Serialize

/*****************************************************************************
** Procedure:  CRoutePoint::OnTimer
**
** Arguments: void
**
** Returns:    void
**
** Description: This member function is called peridically to allow the
**              route point to perform various tasks
**
*****************************************************************************/
void CRoutePoint::OnTimer()
{
	// Scan our call list and try to transfer any calls which have
	// been sitting too long.
	if (m_rpCommand != DoNothing &&	GetCallCount() > 0)
	{
		CLineDevice* pLine = NULL;
		if (m_rpCommand == TransferTo)
			pLine = GetDocument()->GetLineByID(m_dwQueue);

		for (POSITION pos = m_lstCalls.GetHeadPosition(); pos != NULL;)
		{
			CCall* pCall = m_lstCalls.GetNext(pos);
			if (pCall->SecondsInState() > m_wSeconds)
			{
				if (pLine != NULL)
					pLine->TransferCall(NULL, pCall);
				else if (m_rpCommand == Drop)
				{
					delete pCall;
				}
			}
		}
	}

}// CRoutePoint::OnTimer

/*****************************************************************************
** Procedure:  CRoutePoint::WantIncomingTrunkCall
**
** Arguments: 'pTrunk' - Trunk which now has incoming call
**            'pCall' - Call which is on the trunk
**
** Returns:    true/false whether this route point wants the incoming call
**
** Description: This member function is called when any trunk gets an
**              incoming external call
**
*****************************************************************************/
bool CRoutePoint::WantIncomingTrunkCall(CTrunk* pTrunk, CCall* /*pCall*/)
{
	DWORD dwTrunk = pTrunk->GetLineID();
	for (int i = 0; i < m_arrTrunks.GetSize(); i++)
		if (dwTrunk == m_arrTrunks[i]) return true;
	return false;

}// CRoutePoint::WantIncomingTrunkCall

/*****************************************************************************
** Procedure:  CRoutePoint::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an outgoing call placed
**              on this RP.
**
*****************************************************************************/
void CRoutePoint::OnReceiveCall(CCall* pCall_Source)
{
	// Map another copy of the same call on this line
	CCall* pCall_Dest = new CCall(this, *pCall_Source);

	// Mark this call as ringing on the route point
	pCall_Dest->SetCallState(CCall::Ringing);

	// If we are supposed to transfer any incoming call on a route
	// point immediately, then do so.
	if (m_wSeconds == 0 && m_rpCommand == TransferTo && m_dwQueue != 0)
	{
		CLineDevice* pLine = GetDocument()->GetLineByID(m_dwQueue);
		if (pLine != NULL)
			pLine->TransferCall(NULL, pCall_Dest);
	}

}// CRoutePoint::OnReceiveCall

/*****************************************************************************
** Procedure:  CRoutePoint::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CRoutePoint pointer
**
** Description:  This static function displays a UI dialog (CRoutePointDlg)
**               and creates a CRoutePoint object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CRoutePoint* CRoutePoint::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	CRoutePointDlg dlg(pwndOwner, pDoc);
	if (dlg.DoModal() == IDOK)
	{
		CString strDescription = dlg.m_strDescription;
		if (strDescription.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFRPNAME);
			strDescription.Format(strBuff, dlg.m_dwExtension);
		}

		// Create the route point
		CRoutePoint* pRP = new CRoutePoint(dlg.m_dwExtension, strDescription);

		// Save off all the information
		pRP->m_arrTrunks.RemoveAll();
		pRP->m_arrTrunks.Copy(dlg.m_arrTrunks);
		pRP->m_wSeconds = static_cast<WORD>(dlg.m_iSeconds);
		pRP->m_dwQueue = dlg.m_dwTransferTo;
		pRP->m_fAllowRoute = dlg.m_fAllowProgramRoute;
		pRP->m_fAutoRoute = dlg.m_fAutoRoute;
		pRP->m_rpCommand = static_cast<Command>(dlg.m_iCommand);

		// Return the new route point
		return pRP;
	}
	return NULL;

}// CRoutePoint::Create

/*****************************************************************************
** Procedure:  CRoutePoint::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CRoutePointDlg)
**               and changes the existing CRoutePoint information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CRoutePoint::Edit(CWnd* pwndOwner)
{
	CJTDocument* pDoc = GetDocument();
	CRoutePointDlg dlg(pwndOwner, pDoc);

	dlg.m_dwExtension = GetLineID();
	dlg.m_strDescription = GetLineName();
	dlg.m_arrTrunks.RemoveAll();
	dlg.m_arrTrunks.Copy(m_arrTrunks);
	dlg.m_iSeconds = m_wSeconds;
	dlg.m_dwTransferTo = m_dwQueue;
	dlg.m_fAllowProgramRoute = m_fAllowRoute;
	dlg.m_fAutoRoute = m_fAutoRoute;
	dlg.m_iCommand = m_rpCommand;
	dlg.m_arrTrunks.RemoveAll();
	dlg.m_arrTrunks.Copy(m_arrTrunks);
	
	// Show the dialog.  If it is dismissed with an OK result,
	// change our station information.
	if (dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strDescription;
		if (m_strName.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFRPNAME);
			m_strName.Format(strBuff, dlg.m_dwExtension);
		}

		m_arrTrunks.RemoveAll();
		m_arrTrunks.Copy(dlg.m_arrTrunks);
		m_wSeconds = static_cast<WORD>(dlg.m_iSeconds);
		m_dwQueue = dlg.m_dwTransferTo;
		m_fAllowRoute = dlg.m_fAllowProgramRoute ;
		m_rpCommand = static_cast<Command>(dlg.m_iCommand);
		m_fAutoRoute = dlg.m_fAutoRoute;
		
		// Let the UI refresh
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return true;
	}
	return false;

}// CRoutePoint::Edit
