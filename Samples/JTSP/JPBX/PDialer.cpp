/*******************************************************************/
//
// PDIALER.CPP
//
// Predictive dialer support - outgoing only progress-detection
// line support
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
#include "pdialer.h"
#include "document.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "resource.h"

/*---------------------------------------------------------------*/
// RTTI INFORMATION
/*---------------------------------------------------------------*/
IMPLEMENT_SERIAL(CPDialer, CLineDevice, 1)

/*---------------------------------------------------------------*/
// CONSTANTS
/*---------------------------------------------------------------*/
const UINT IDT_TIMER = 100;

/**************************************************************************
** CCallProgress
**
** User-interface dialog which allows the call progress to be altered
** for a predictive dialed call
**
***************************************************************************/
class CCallProgress : public CBaseDialog
{
// Dialog Data
	//{{AFX_DATA(CCallProgress)
	enum { IDD = IDD_CALLSTATE };
	CCall* m_pCall;
	CPDialer* m_pLine;
	int m_iType;
	int m_iTimeout;
	DWORD m_dwTransferTo;
	//}}AFX_DATA

// Construction
public:
	CCallProgress(CWnd* pParent, CCall* pCall, CPDialer* pLine, int iType, int iTimeout, DWORD dwExtension);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnDismiss();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Internal methods
protected:
	void Process(UINT uiCtrl);

};

/**************************************************************************
** CPDialerDlg
**
** User-interface dialog for the predictive dialer
**
***************************************************************************/
class CPDialerDlg : public CBaseDialog
{
// Class data
public:
// Dialog Data
	//{{AFX_DATA(CPDialerDlg)
	enum { IDD = IDD_PDIALER };
	CComboBox	m_cbOptions;
	int m_nChoice;
	CString	m_strDescription;
	//}}AFX_DATA

// Construction
public:
	CPDialerDlg(CWnd* pParent);   // standard constructor

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPDialerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPDialerDlg)
	afx_msg void OnChange();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CPDialerDlg::CPDialerDlg
**
** Arguments: 'pParent' - Parent for the dialog
**
** Returns:    void
**
** Description: Dialog constructor
**
*****************************************************************************/
CPDialerDlg::CPDialerDlg(CWnd* pParent)
	: CBaseDialog(CPDialerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPDialerDlg)
	m_nChoice = -1;
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

}// CPDialerDlg::CPDialerDlg

/*****************************************************************************
** Procedure:  CPDialerDlg::OnInitDialog
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
BOOL CPDialerDlg::OnInitDialog() 
{
	static UINT uiChoices[] = {
		IDS_PD_BUSY,
		IDS_PD_VOICE,
		IDS_PD_AMACHINE,
		IDS_PD_UNKNOWN,
		IDS_PD_NOANSWER,
		IDS_PD_RANDOM,
		IDS_PD_PROMPT,
		0
	};

	// Call the base initialization
	CBaseDialog::OnInitDialog();

	// Load our combo box with choices
	for (int i = 0; uiChoices[i] != 0; i++)
	{
		CString strBuff;
		VERIFY(strBuff.LoadString(uiChoices[i]));
		int iCurSel = m_cbOptions.AddString(strBuff);
		ASSERT (iCurSel != CB_ERR);
		m_cbOptions.SetItemData(iCurSel, static_cast<DWORD>(i));
		if (i == m_nChoice)
			m_cbOptions.SetCurSel(iCurSel);
	}

	// Limit our text
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_DESCRIPTION))->LimitText(LINENAME_SIZE);

	return TRUE;

}// CPDialerDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CPDialerDlg::DoDataExchange
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
void CPDialerDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPDialerDlg)
	DDX_Control(pDX, IDC_OPTIONS, m_cbOptions);
	DDX_Text(pDX, IDC_DESCRIPTION, m_strDescription);
	//}}AFX_DATA_MAP

}// CPDialerDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CPDialerDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CPDialerDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CPDialerDlg)
	ON_CBN_SELCHANGE(IDC_OPTIONS, OnChange)
	ON_EN_CHANGE(IDC_DESCRIPTION, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CPDialerDlg::OnChange
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
void CPDialerDlg::OnChange() 
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);

}// CPDialerDlg::OnChange

/*****************************************************************************
** Procedure:  CPDialerDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CPDialerDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		int iCurSel = m_cbOptions.GetCurSel();
		if (iCurSel == CB_ERR)
		{
			AfxMessageBox(IDS_MISSINGPDIALTYPE);
			GotoDlgCtrl(GetDlgItem(IDC_OPTIONS));
			return;
		}
		m_nChoice = static_cast<int>(m_cbOptions.GetItemData(iCurSel));
		CBaseDialog::OnOK();
	}

}// CPDialerDlg::OnOK

/*****************************************************************************
** Procedure:  CCallProgress::CCallProgress
**
** Arguments: void
**
** Returns:    void
**
** Description: Constructor for the call progress dialog
**
*****************************************************************************/
CCallProgress::CCallProgress(CWnd* pParent, CCall* pCall, CPDialer* pLine, int iType,
							 int iTimeout, DWORD dwExtension)
	: CBaseDialog(CCallProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCallProgress)
	m_pCall = pCall;
	m_pLine = pLine;
	m_iType = iType;
	m_iTimeout = iTimeout;
	m_dwTransferTo = dwExtension;
	//}}AFX_DATA_INIT

	// Create the dialog
	Create(IDD, pParent);

}// CCallProgress::CCallProgress

/*****************************************************************************
** Procedure:  CCallProgress::OnInitDialog
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
BOOL CCallProgress::OnInitDialog() 
{
	// Connect dialog controls
	CBaseDialog::OnInitDialog();
	if (m_iType == CPDialer::PromptUser)
	{
		// Set the title
		CString strTitle, strBuff;
		GetWindowText(strTitle);
		strBuff.Format(_T("%s [%08X] %s"), strTitle, m_pCall->GetCallID(), m_pCall->GetANI());
		SetWindowText(strBuff);

		// Make it topmost
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
	else
	{
		int iTimer = 0;
		while (iTimer < 2000)
			iTimer = (rand()%30000);

		// If our timeout is greater than what the caller wanted to
		// wait for then act as if no answer always.
		if (iTimer > m_iTimeout)
			m_iType = CPDialer::ActNoAnswer;

		ShowWindow(SW_HIDE);
		if (SetTimer(IDT_TIMER, min(iTimer, m_iTimeout), NULL) == 0)
			OnTimer(IDT_TIMER);
	}

	return TRUE;

}// CCallProgress::OnInitDialog

/*****************************************************************************
** Procedure:  CCallProgress::DoDataExchange
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
void CCallProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallProgress)
	//}}AFX_DATA_MAP

}// CCallProgress::DoDataExchange

/*---------------------------------------------------------------*/
// CCallProgress message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCallProgress, CBaseDialog)
	//{{AFX_MSG_MAP(CCallProgress)
	ON_BN_CLICKED(IDC_BUSY, OnDismiss)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_VOICE, OnDismiss)
	ON_BN_CLICKED(IDC_MACHINE, OnDismiss)
	ON_BN_CLICKED(IDC_UNKNOWN, OnDismiss)
	ON_BN_CLICKED(IDC_NOANSWER, OnDismiss)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CCallProgress::OnTimer
**
** Arguments: 'nIDEvent' - Event which fired
**
** Returns:    void
**
** Description: This is an interval timer routine which 
**              of the state buttons
**
*****************************************************************************/
void CCallProgress::OnTimer(UINT nIDEvent) 
{
	// Destroy the timer
	KillTimer(nIDEvent);

	// Now dismiss the dialog with the proper type.
	switch (m_iType)
	{
		case CPDialer::ActBusy:				Process(IDC_BUSY); break;
		case CPDialer::ActAnswerVoice:		Process(IDC_VOICE); break;
		case CPDialer::ActAnswerMachine:	Process(IDC_MACHINE); break;
		case CPDialer::ActUnknown:			Process(IDC_UNKNOWN); break;
		case CPDialer::ActNoAnswer:			Process(IDC_NOANSWER); break;
		case CPDialer::Random:				Process(IDC_BUSY + (rand() % 5)); break;
		default:							ASSERT (FALSE); break;
	}

}// CCallProgress::OnTimer

/*****************************************************************************
** Procedure:  CCallProgress::OnDismiss
**
** Arguments: void
**
** Returns:    void
**
** Description: This is invoked when the dialog is dismissed by using on
**              of the state buttons
**
*****************************************************************************/
void CCallProgress::OnDismiss()
{
	CWnd* pWnd = GetFocus();
	if (pWnd != NULL)
		Process(pWnd->GetDlgCtrlID());

}// CCallProgress::OnDismiss

/*****************************************************************************
** Procedure:  CCallProgress::Process
**
** Arguments: 'uiCtrl' - Type of event to dismiss the dialog with
**
** Returns:    void
**
** Description: This is invoked when the dialog is dismissed by using on
**              of the state buttons
**
*****************************************************************************/
void CCallProgress::Process(UINT uiCtrl)
{
	switch (uiCtrl)
	{
		case IDC_BUSY:
			m_pLine->OnBusy(m_pCall);
			break;
		case IDC_VOICE:
			m_pLine->OnAnsweredVoice(m_pCall, m_dwTransferTo);
			break;
		case IDC_MACHINE:
			m_pLine->OnAnsweredMachine(m_pCall);
			break;
		case IDC_UNKNOWN:
			m_pLine->OnAnsweredUnknown(m_pCall, m_dwTransferTo);
			break;
		case IDC_NOANSWER:
			m_pLine->OnNoAnswer(m_pCall);
			break;
	}

	// Destroy this window
	DestroyWindow();
	delete this;

}// CCallProgress::Process

/*****************************************************************************
** Procedure:  CPDialer::CPDialer
**
** Arguments: 'dwID' - Dialer id
**            'pszDescription' - Description for station
**
** Returns:    void
**
** Description:  No parameter constructor for the predictive dialer device
**
*****************************************************************************/
CPDialer::CPDialer(DWORD dwID, LPCTSTR pszDescription) : CLineDevice(dwID, pszDescription), 
	m_cmType(PromptUser)
{
}// CPDialer::CPDialer

/*****************************************************************************
** Procedure:  CPDialer::CPDialer
**
** Arguments: void
**
** Returns:    void
**
** Description:  No parameter constructor for the predictive dialer device
**
*****************************************************************************/
CPDialer::CPDialer() : CLineDevice(), m_cmType(PromptUser)
{
}// CPDialer::CPDialer

/*****************************************************************************
** Procedure:  CPDialer::~CPDialer
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the line device
**
*****************************************************************************/
CPDialer::~CPDialer()
{
}// CPDialer::~CPDialer

/*****************************************************************************
** Procedure:  CPDialer::Serialize
**
** Arguments: 'archive' - Archive stream to serialize to
**
** Returns:    void
**
** Description: This member function is called to save our object state
**              into a persistant serialize stream
**
*****************************************************************************/
void CPDialer::Serialize( CArchive& archive )
{
	CLineDevice::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << static_cast<BYTE>(m_cmType);
	}
	else
	{
		BYTE b1;
		archive >> b1;
		m_cmType = static_cast<Command>(b1);
	}

}// CPDialer::Serialize

/*****************************************************************************
** Procedure:  CPDialer::OnReceiveCall
**
** Arguments: 'pCall' - Existing call object (if any)
**
** Returns:    void
**
** Description: This function is a notification for an incoming call on
**              the predictive dialer
**
*****************************************************************************/
void CPDialer::OnReceiveCall(CCall*)
{
	ASSERT(FALSE);

}// CPDialer::OnReceiveCall

/*****************************************************************************
** Procedure:  CPDialer::TransferCall
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
bool CPDialer::TransferCall(CCall*, CCall*)
{
	ASSERT (FALSE);
	return false;

}// CPDialer::TransferCall

/*****************************************************************************
** Procedure:  CPDialer::CanAddCall
**
** Arguments: void
**
** Returns:    true/false
**
** Description:  Returns TRUE if we can add a new call to the line
**
*****************************************************************************/
bool CPDialer::CanAddCall()
{
	return (GetCallCount() < DEFAULT_PDIALERMAX);

}// CPDialer::CanAddCall

/*****************************************************************************
** Procedure:  CPDialer::DialPredictiveCall
**
** Arguments: 'pszNumber' - DN to dial
**
** Returns:    Call object
**
** Description: This function creates a call on a line to either 
**              another line or an outside call.
**
*****************************************************************************/
CCall* CPDialer::DialPredictiveCall(CNetInfo& cidDNIS, int iTimeout, DWORD dwTransferTo)
{
	// Dial the call on this line
	CCall* pCall = CLineDevice::DialCall(cidDNIS);
	if (pCall == NULL)
		return pCall;

	// If the timeout is zero, we wait forever.
	if (iTimeout == 0)
		iTimeout = 65535;

	// If we are to prompt the user concerning this call, show the 
	// dialog to do so.
	new CCallProgress(AfxGetMainWnd(), pCall, this, m_cmType, iTimeout, dwTransferTo);
	return pCall;

}// CPDialer::DialPredictiveCall

/*****************************************************************************
** Procedure:  CPDialer::OnBusy
**
** Arguments: void
**
** Returns:   void
**
** Description: This event function is called when a dialed call has decided
**              what it's media will be.
**
*****************************************************************************/
void CPDialer::OnBusy(CCall* pCall)
{
	SendEvent(false, pCall, PBXEVT_CMD, pCall->GetCallID(), _T('B'));
	pCall->SetCallState(CCall::Disconnected);

}// CPDialer::OnBusy

/*****************************************************************************
** Procedure:  CPDialer::OnAnsweredVoice
**
** Arguments: void
**
** Returns:   void
**
** Description: This event function is called when a dialed call has decided
**              what it's media will be.
**
*****************************************************************************/
void CPDialer::OnAnsweredVoice(CCall* pCall, DWORD dwTransferTo)
{
	SendEvent(false, pCall, PBXEVT_CMD, pCall->GetCallID(), _T('V'));

	// Transfer the call to the target destination
	if (dwTransferTo > 0)
	{
		CLineDevice* pLine = GetDocument()->GetLineByID(dwTransferTo);
		if (pLine != NULL)
			pLine->TransferCall(NULL, pCall);
		return;
	}
	pCall->SetCallState(CCall::Connected);

}// CPDialer::OnAnsweredVoice

/*****************************************************************************
** Procedure:  CPDialer::OnAnsweredMachine
**
** Arguments: void
**
** Returns:   void
**
** Description: This event function is called when a dialed call has decided
**              what it's media will be.
**
*****************************************************************************/
void CPDialer::OnAnsweredMachine(CCall* pCall)
{
	SendEvent(false, pCall, PBXEVT_CMD, pCall->GetCallID(), _T('M'));
	pCall->SetCallState(CCall::Connected);

}// OnAnsweredMachine

/*****************************************************************************
** Procedure:  CPDialer::OnAnsweredUnknown
**
** Arguments: void
**
** Returns:   void
**
** Description: This event function is called when a dialed call has decided
**              what it's media will be.
**
*****************************************************************************/
void CPDialer::OnAnsweredUnknown(CCall* pCall, DWORD dwTransferTo)
{
	SendEvent(false, pCall, PBXEVT_CMD, pCall->GetCallID(), _T('U'));

	// Transfer the call to the target destination
	if (dwTransferTo > 0)
	{
		CLineDevice* pLine = GetDocument()->GetLineByID(dwTransferTo);
		if (pLine != NULL)
			pLine->TransferCall(NULL, pCall);
		return;
	}
	pCall->SetCallState(CCall::Connected);

}// OnAnsweredUnknown

/*****************************************************************************
** Procedure:  CPDialer::NoAnswer
**
** Arguments: void
**
** Returns:   void
**
** Description: This event function is called when a dialed call has decided
**              what it's media will be.
**
*****************************************************************************/
void CPDialer::OnNoAnswer(CCall* pCall)
{
	SendEvent(false, pCall, PBXEVT_CMD, pCall->GetCallID(), _T('N'));
	pCall->SetCallState(CCall::Disconnected);

}// CPDialer::NoAnswer

/*****************************************************************************
** Procedure:  CPDialer::Create
**
** Arguments: 'pwndOwner' - Owner window handle
**            'pDoc' - Master CJTDocument 
**
** Returns:    CPDialer pointer
**
** Description:  This static function displays a UI dialog (CPDialerDlg)
**               and creates a CPDialer object if the dialog is dismissed
**               with an OK result.
**
*****************************************************************************/
CPDialer* CPDialer::Create(CWnd* pwndOwner, CJTDocument* pDoc)
{
	// Show our creation dialog
	CPDialerDlg dlg(pwndOwner);
	if (dlg.DoModal() == IDOK)
	{
		// Create the dialer identifier.  If we have exceeded
		// our system limit give an error.
		DWORD dwDialerID = pDoc->GenerateTrunkID();
		if (dwDialerID == 0xffffffff)
		{
			AfxMessageBox(IDS_TOOMANYOBJS);
			return NULL;
		}

		// Save off the description
		CString strDescription = dlg.m_strDescription;
		if (strDescription.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFDIALERNAME);
			strDescription.Format(strBuff, dwDialerID);
		}

		// Create the station
		CPDialer* pDialer = new CPDialer(dwDialerID, strDescription);
		pDialer->m_cmType = static_cast<Command>(dlg.m_nChoice);

		// Return the new dialer
		return pDialer;
	}
	return NULL;

}// CPDialer::Create

/*****************************************************************************
** Procedure:  CPDialer::Edit
**
** Arguments: 'pwndOwner' - Owner window handle
**
** Returns:    true/false result whether object was successfully editted
**
** Description:  This function displays a UI dialog (CPDialerDlg)
**               and changes the existing CPDialer information if it is
**               dismissed with an OK result.
**
*****************************************************************************/
bool CPDialer::Edit(CWnd* pwndOwner)
{
	CPDialerDlg dlg(pwndOwner);

	dlg.m_strDescription = GetLineName();
	dlg.m_nChoice = static_cast<int>(m_cmType);
	
	// Show the dialog.  If it is dismissed with an OK result,
	// change our station information.
	if (dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strDescription;
		if (m_strName.IsEmpty())
		{
			CString strBuff;
			strBuff.LoadString(IDS_DEFDIALERNAME);
			m_strName.Format(strBuff, GetLineID());
		}

		m_cmType = static_cast<Command>(dlg.m_nChoice);

		// Let the UI refresh
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return true;
	}
	return false;

}// CPDialer::Edit
