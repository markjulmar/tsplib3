/*******************************************************************/
//
// TRUNK.CPP
//
// Trunk object support (superset of CLineDevice)
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
IMPLEMENT_SERIAL(CTrunk, CLineDevice, 1)

/*---------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------*/
int CTrunk::g_nCPS = 0;
DWORD CTrunk::g_dwTickCount = 0;

/**************************************************************************
** CTrunkDlg
**
** User-interface dialog for the trunk device
**
***************************************************************************/
class CTrunkDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CTrunkDlg)
	enum { IDD = IDD_TRUNK };
	int	m_iOffline;
	int m_iOriginal;
	DWORD m_dwTrunkID;
	//}}AFX_DATA

// Construction
public:
	CTrunkDlg(CWnd* pParent);   // standard constructor

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrunkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTrunkDlg)
	afx_msg void OnChange();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CreateTrunkName
**
** Arguments: 'iTrunk' - Trunk ID
**
** Returns:    String for trunk name
**
** Description: Internal function to create our trunk name
**
*****************************************************************************/
inline CString CreateTrunkName(DWORD dwTrunkID)
{
	CString strBuff, strTrunk;
	strBuff.LoadString(IDS_TRUNKNAME);
	strTrunk.Format(strBuff, dwTrunkID);
	return strTrunk;

}// CreateTrunkName

/*****************************************************************************
** Procedure:  CTrunkDlg::CTrunkDlg
**
** Arguments: 'pParent' - Parent window
**
** Returns:    void
**
** Description: Constructor for the trunk dialog
**
*****************************************************************************/
CTrunkDlg::CTrunkDlg(CWnd* pParent)
	: CBaseDialog(CTrunkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrunkDlg)
	m_iOffline = 0;
	m_dwTrunkID = 0;
	//}}AFX_DATA_INIT

}// CTrunkDlg::CTrunkDlg

/*****************************************************************************
** Procedure:  CTrunkDlg::OnInitDialog
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
BOOL CTrunkDlg::OnInitDialog() 
{
	// Call the base initialization
	CBaseDialog::OnInitDialog();
	m_iOriginal = m_iOffline;

	if (m_dwTrunkID == 0)
		GetDlgItem(IDOK)->EnableWindow(TRUE);

	return TRUE;

}// CTrunkDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CTrunkDlg::DoDataExchange
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
void CTrunkDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrunkDlg)
	DDX_Radio(pDX, IDC_ONLINE, m_iOffline);
	DDX_Text(pDX, IDC_TRUNKID, m_dwTrunkID);
	//}}AFX_DATA_MAP

}// CTrunkDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CTrunkDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CTrunkDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CTrunkDlg)
	ON_BN_CLICKED(IDC_ONLINE, OnChange)
	ON_BN_CLICKED(IDC_OFFLINE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CTrunkDlg::OnChange
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
void CTrunkDlg::OnChange() 
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);

}// CTrunkDlg::OnChange

/*****************************************************************************
** Procedure:  CTrunkDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CTrunkDlg::OnOK() 
{
	UpdateData(TRUE);
	if (m_iOffline == 1 && m_iOriginal == 0)
	{
		if (AfxMessageBox(IDS_TRUNKOFFLINE) == IDNO)
		{
			m_iOffline = 0;
			UpdateData(FALSE);
		}
	}

	CDialog::OnOK();

}// CTrunkDlg::OnOK

/*****************************************************************************
** Procedure:  CTrunk::CTrunk
**
** Arguments: 'dwTrunkID' - Trunk ID
**
** Returns:    void
**
** Description: Constructor for the trunk line
**
*****************************************************************************/
CTrunk::CTrunk(DWORD dwTrunkID) : CLineDevice(dwTrunkID, CreateTrunkName(dwTrunkID))
{
	m_tsState = Unused;

}// CTrunk::CTrunk

/*****************************************************************************
** Procedure:  CTrunk::CTrunk
**
** Arguments: void
**
** Returns:    void
**
** Description: No parameter constructor for the trunk line
**
*****************************************************************************/
CTrunk::CTrunk() : CLineDevice()
{
	m_tsState = Unused;

}// CTrunk::CTrunk

/*****************************************************************************
** Procedure:  CTrunk::~CTrunk
**
** Arguments: void
**
** Returns:    void
**
** Description: Destructor for the trunk line
**
*****************************************************************************/
CTrunk::~CTrunk()
{
}// CTrunk::~CTrunk

/*****************************************************************************
** Procedure:  CTrunk::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CTrunk::Serialize( CArchive& archive )
{
	CLineDevice::Serialize(archive);

	if (archive.IsStoring())
	{
		// Store offline or unused.
		if (m_tsState == Offline)
			archive <<  static_cast<BYTE>(Offline);
		else
			archive << static_cast<BYTE>(Unused);
	}
	else
	{
		BYTE b1;
		archive >> b1;
		m_tsState = static_cast<TrunkState>(b1);
	}

}// CTrunk::Serialize

/*****************************************************************************
** Procedure:  CTrunk::GenerateIncomingCall
**
** Arguments: 'cidAni'  - ANI information (caller)
**            'cidDnis' - DNIS information (called)
**
** Returns:    Bool result whether an incoming call was made
**
** Description: Generates an incoming call on the trunk
**
*****************************************************************************/
CCall* CTrunk::GenerateIncomingCall(const CNetInfo& cidANI, const CNetInfo& cidDNIS)
{
	// Verify that we can make an incoming call
	if (!CanAddCall())
		return NULL;

	// Create the call - this will send the CALL NOTICED event to any monitors
	CCall* pCall = new CCall(this, cidANI, cidDNIS, true);
	ASSERT (pCall != NULL);
	pCall->SetCallState(CCall::Unknown);

	return pCall;

}// CTrunk::GenerateIncomingCall

/*****************************************************************************
** Procedure:  CTrunk::CanAddCall
**
** Arguments: void
**
** Returns:    Bool result whether a new call can be put onto this line
**
** Description: Determines number of calls available on a single line
**
*****************************************************************************/
bool CTrunk::CanAddCall()
{
	return (m_tsState == Unused);

}// CTrunk::CanAddCall

/*****************************************************************************
** Procedure:  CTrunk::OnCallCreated
**
** Arguments: 'pCall' - Call to add to our list
**            'fPlaceCall' - Call was created due to a DIAL event
**
** Returns:    void
**
** Description:  Adds the specified object to our list
**
*****************************************************************************/
void CTrunk::OnCallCreated(CCall* pCall, bool fPlaceCall)
{
	// Pass through so it gets added to our array
	CLineDevice::OnCallCreated(pCall, false);

	// Mark the trunk state
	if (fPlaceCall)
	{
		m_tsState = Inbound;
		g_dwTickCount = GetTickCount();
		g_nCPS++;
	}
	else
		m_tsState = Outbound;

	GetDocument()->OnTrunkStateChanged(this);

}// CTrunk::OnCallCreated

/*****************************************************************************
** Procedure:  CTrunk::OnCallDestroyed
**
** Arguments: 'pCall' - Call to remove from our list
**
** Returns:    void
**
** Description:  Removes the specified object from our list
**
*****************************************************************************/
void CTrunk::OnCallDestroyed(CCall* pCall, bool fNotifyRelease)
{
	// Pass through so it gets removed from our array
	CLineDevice::OnCallDestroyed(pCall, fNotifyRelease);

	// Mark the trunk as available
	m_tsState = Unused;
	GetDocument()->OnTrunkStateChanged(this);

}// CTrunk::OnRemoveCall

/*****************************************************************************
** Procedure:  CTrunk::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an outgoing call placed
**              on this trunk
**
*****************************************************************************/
void CTrunk::OnReceiveCall(CCall* pCall_Source)
{
	// Map another copy of the same call on this line
	CCall* pCall_Dest = new CCall(this, *pCall_Source);

	// Mark this call as unknown since it is on the trunk.
	pCall_Dest->SetCallState(CCall::Unknown);

}// CTrunk::OnReceiveCall

/*****************************************************************************
** Procedure:  CTrunk::OnTimer
**
** Arguments: void
**
** Returns:    void
**
** Description: This member function is called peridically to allow the
**              trunk to perform various tasks
**
*****************************************************************************/
void CTrunk::OnTimer()
{
	if (GetTickCount() > g_dwTickCount + 1000)
	{
		g_nCPS = 0;
		g_dwTickCount = GetTickCount();
	}

	// If we have an outbound call, wait 15 seconds and
	// then mark it as connected
	if (m_tsState == Outbound && GetCallCount() == 1)
	{
		CCall* pCall = m_lstCalls.GetHead();
		CCall* pOther = pCall->GetAttachedCall();
		if (pOther && pOther->GetCallState() == CCall::Dialing &&
			pCall->SecondsInState() >= 15)
			pOther->SetCallState(CCall::Connected);
	}

}// CTrunk::OnTimer

/*****************************************************************************
** Procedure:  CTrunk::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CTrunk pointer
**
** Description:  This static function displays a UI dialog (CTrunkDlg)
**               and creates a CTrunk object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CTrunk* CTrunk::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	CTrunkDlg dlg(pwndOwner);
	if (dlg.DoModal() == IDOK)
	{
		// Create the trunk identifier.  If we have exceeded
		// our system limit give an error.
		DWORD dwTrunkID = pDoc->GenerateTrunkID();
		if (dwTrunkID == 0xffffffff)
		{
			AfxMessageBox(IDS_TOOMANYOBJS);
			return NULL;
		}

		// Create the trunk and mark the online/offline status
		CTrunk* pTrunk = new CTrunk(dwTrunkID);
		pTrunk->Online((dlg.m_iOffline) ? false : true);

		// Return the new trunk
		return pTrunk;
	}
	return NULL;

}// CTrunk::Create

/*****************************************************************************
** Procedure:  CTrunk::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CTrunkDlg)
**               and changes the existing CTrunk information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CTrunk::Edit(CWnd* pwndOwner)
{
	CTrunkDlg dlg(pwndOwner);

	dlg.m_dwTrunkID = GetLineID();
	dlg.m_iOffline = (m_tsState == Offline) ? 1 : 0;
	
	// Show the dialog.  If it is dismissed with an OK result,
	// change our station information.
	if (dlg.DoModal() == IDOK)
	{
		// Mark the trunk online or offline - this will notify all
		// the views on the document automatically.
		Online((dlg.m_iOffline == 1) ? false : true);
		return true;
	}
	return false;

}// CTrunk::Edit

/*****************************************************************************
** Procedure:  CTrunk::GetTrunkStateString
**
** Arguments: void
**
** Returns: Current state of the trunk in text form
**
** Description: This returns the state of the trunk
**
*****************************************************************************/
CString CTrunk::GetTrunkStateString() const
{
	static UINT uiState[] = {
		IDS_TS_OFFLINE,
		IDS_TS_INBOUND,
		IDS_TS_OUTBOUND,
		IDS_TS_UNUSED
	};

	CString strBuff;
	strBuff.LoadString(uiState[m_tsState]);
	
	ASSERT (!strBuff.IsEmpty());
	return strBuff;

}// CTrunk::GetTrunkState

/*****************************************************************************
** Procedure:  CTrunk::Online
**
** Arguments: 'fOnline' - Whether the trunk should be online
**
** Returns:    void
**
** Description:  Marks the trunk as online or not.
**
*****************************************************************************/
void CTrunk::Online(bool fOnline)
{
	if (m_tsState != Unused && m_tsState != Offline)
		return;

	m_tsState = (fOnline) ? Unused : Offline;
	GetDocument()->OnTrunkStateChanged(this);

}// CTrunk::Online

