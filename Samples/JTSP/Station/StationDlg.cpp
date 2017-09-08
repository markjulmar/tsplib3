/*******************************************************************/
//
// STATIONDLG.CPP
//
// Main Station program dialog
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
#include "resource.h"
#include "StationDlg.h"
#include "Interface.h"
#include "logonDlg.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**************************************************************************
** CStationApp
**
** Application class object
**
***************************************************************************/
class CStationApp : public CWinApp
{
// Overrides
public:
	virtual BOOL InitInstance();
};

/*---------------------------------------------------------------*/
// GLOBAL VARIABLES
/*---------------------------------------------------------------*/
CStationApp theApp;

/*---------------------------------------------------------------*/
// CONSTANTS
/*---------------------------------------------------------------*/
#define UNAVAILABLE _T("NOT SIGNED ON")
const UINT IDT_TIMER = 100;

/*****************************************************************************
** Procedure:  atox
**
** Arguments: 'strBuff' - String
**
** Returns:   DWORD
**
** Description:  Ascii to Hex
**
*****************************************************************************/
inline DWORD atox(LPCTSTR pszBuff)
{
	DWORD dwValue=0;
	sscanf(pszBuff, "%lx", &dwValue);
	return dwValue;

}// atox

/*****************************************************************************
** Procedure:  CStationApp::InitInstance
**
** Arguments: void
**
** Returns:   Boolean to run message loop
**
** Description:  Initial entry point for program
**
*****************************************************************************/
BOOL CStationApp::InitInstance()
{
	// Initialize WinSock
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	InitCommonControls();

	// Invoke our dialog
	CStationDlg dlg;
	m_pMainWnd = &dlg;

	// Set our registry key section
	SetRegistryKey(_T("JulMar Technology\\StationDlg"));

	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;

}// CStationApp::InitInstance

/*****************************************************************************
** Procedure:  CStationDlg::CStationDlg
**
** Arguments: 'pParent' - Parent window
**
** Returns:   Station dialog constructor
**
** Description:  Constructor
**
*****************************************************************************/
CStationDlg::CStationDlg(CWnd* pParent)
	: CDialog(CStationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStationDlg)
	m_fConnected = FALSE;
	m_fReady = FALSE;
	m_fCallWork = FALSE;
	m_fPlacingCall = FALSE;
	m_iConferenceCount = 0;
	m_strDisplay = UNAVAILABLE;
	m_pConn = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwExtension = 0;
	m_dwStatus = 0;
	m_strAgentID = _T("");
	m_strDialedNumber = _T("");
	m_strLastDisplay = _T("");
	//}}AFX_DATA_INIT

}// CStationDlg::CStationDlg

/*****************************************************************************
** Procedure:  CStationDlg::~CStationDlg
**
** Arguments: void
**
** Returns:   void
**
** Description:  Destructor for the trunk dialog
**
*****************************************************************************/
CStationDlg::~CStationDlg()
{
	if (m_pConn != NULL)
	{
		if (!m_strAgentID.IsEmpty())
			m_pConn->SendEvent(PBXCMD_CAS, m_dwExtension, m_strAgentID, 'S');
		Sleep(1000);
		delete m_pConn;
	}

}// CStationDlg::~CStationDlg

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
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStationDlg)
	DDX_Control(pDX, IDC_SIGNON, m_btnSignOn);
	DDX_Control(pDX, IDC_DISPLAY, m_ctlDisplay);
	DDX_Check(pDX, IDC_READY, m_fReady);
	DDX_Check(pDX, IDC_CALLWORK, m_fCallWork);
	DDX_Text(pDX, IDC_DISPLAY, m_strDisplay);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LINE1, m_btnLine[0]);
	DDX_Control(pDX, IDC_LINE2, m_btnLine[1]);
	DDX_Control(pDX, IDC_LINE3, m_btnLine[2]);

}// CStationDlg::DoDataExchange

/*---------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CStationDlg, CDialog)
	//{{AFX_MSG_MAP(CStationDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONFERENCE, OnConference)
	ON_BN_CLICKED(IDC_HOLD, OnHold)
	ON_BN_CLICKED(IDC_LINE1, OnLine)
	ON_BN_CLICKED(IDC_READY, OnReady)
	ON_BN_CLICKED(IDC_RELEASE, OnRelease)
	ON_BN_CLICKED(IDC_SIGNON, OnSignon)
	ON_BN_CLICKED(IDC_TRANSFER, OnTransfer)
	ON_BN_CLICKED(IDC_DIGIT1, OnDigit)
	ON_BN_CLICKED(IDC_ENTER, OnEnter)
	ON_WM_TIMER()
	ON_MESSAGE(UM_GOTDATA, OnGetData)
	ON_MESSAGE(UM_CLOSECONN, OnCloseConn)
	ON_BN_CLICKED(IDC_LINE2, OnLine)
	ON_BN_CLICKED(IDC_LINE3, OnLine)
	ON_BN_CLICKED(IDC_DIGIT2, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT3, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT4, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT5, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT6, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT7, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT8, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT9, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT10, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT11, OnDigit)
	ON_BN_CLICKED(IDC_DIGIT12, OnDigit)
	ON_BN_CLICKED(IDC_CALLWORK, OnCallWork)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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
	// Connect up the dialog controls
	CDialog::OnInitDialog();

	// Change our font for all controls.
	CFont fntAnsi;
	fntAnsi.CreateStockObject(ANSI_VAR_FONT);
	CWnd* pwnd = GetWindow(GW_CHILD);
	while (pwnd != NULL && IsChild(pwnd))
	{
		pwnd->SetFont(&fntAnsi);
		pwnd = pwnd->GetWindow(GW_HWNDNEXT);
	}

	// Create our display font.  We want a larger font which
	// looks decent in the control
	m_fntDisplay.CreateFont(-14, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, ANSI_CHARSET, 
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			(FIXED_PITCH | TMPF_TRUETYPE | FF_MODERN), _T("Courier New"));
	m_ctlDisplay.SetFont(&m_fntDisplay);

	// Center our window
	CenterWindow();

	// Set the icon for this dialog.
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set our interval timer
	SetTimer(IDT_TIMER, 1000, NULL);

	return TRUE;

}// CStationDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CStationDlg::OnPaint
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called to paint the dialog.  We use it to
**              paint our minimized icon.
**
*****************************************************************************/
void CStationDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialog::OnPaint();

}// CStationDlg::OnPaint

/*****************************************************************************
** Procedure:  CStationDlg::OnQueryDragIcon
**
** Arguments: void
**
** Returns:    HCURSOR to use for dragging window
**
** Description: This event is called to get a cursor to drag the window with
**
*****************************************************************************/
HCURSOR CStationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;

}// CStationDlg::OnQueryDragIcon

/*****************************************************************************
** Procedure:  CStationDlg::OnSignon
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "SIGNON" button is pressed
**
*****************************************************************************/
void CStationDlg::OnSignon() 
{
	if (m_fConnected)
	{
		// Sign off
		m_pConn->SendEvent(PBXCMD_CAS, m_dwExtension, m_strAgentID, 'S');
	}
	else
	{
		// Show our connection dialog
		CLogonDlg dlg(this);
		dlg.m_strIPAddress = AfxGetApp()->GetProfileString(_T("Config"), _T("IPAddress"));
		dlg.m_nPort = atoi(AfxGetApp()->GetProfileString(_T("Config"), _T("IPPort")));

		if (dlg.DoModal() == IDOK)
		{
			m_pConn = new CConnection(dlg.m_strIPAddress, dlg.m_nPort);
			if (!m_pConn->IsValid())
			{
				AfxMessageBox(IDS_NOSERVER);
				delete m_pConn;
				m_pConn = NULL;
				return;
			}

			// Save off the IP address
			AfxGetApp()->WriteProfileString(_T("Config"), _T("IPAddress"), dlg.m_strIPAddress);
			CString strBuff;
			strBuff.Format(_T("%d"), dlg.m_nPort);
			AfxGetApp()->WriteProfileString(_T("Config"), _T("IPPort"), strBuff);

			// Perform a LOGON.
			m_pConn->SendEvent(PBXCMD_LOGON, dlg.m_dwExtension);

			// Perform an AGENT LOGON
			m_pConn->SendEvent(PBXCMD_ALO, dlg.m_dwExtension, dlg.m_strAgentID, dlg.m_strPassword);
		}

	}

}// CStationDlg::OnSignon

/*****************************************************************************
** Procedure:  CStationDlg::OnLine
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the line(s) button is pressed
**
*****************************************************************************/
void CStationDlg::OnLine() 
{
	// Get the line button which was pressed
	for (int iLine = 0; iLine < 3; iLine++)
	{
		if (::GetFocus() == m_btnLine[iLine].GetSafeHwnd())
			break;
	}
	ASSERT (iLine < 3);

	// If LINE is pressed when in a conference, break the conference down
	// by placing all the other calls on hold.
	if (m_iConferenceCount > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_arrCalls[i].m_fInConference && i != iLine)
				m_pConn->SendEvent(PBXCMD_HC, m_dwExtension, m_arrCalls[i].dwCallID);
		}

		// The button just pressed will be activated by our OnCallState code.
		return;
	}

	// Determine which line is active (if any), if it is the button
	// which was just pressed, ignore it.
	int iActive = GetActiveLine();
	if (iActive == iLine)
		return;

	// If there is an active line already, make it not active
	if (iActive >= 0)
	{
		// If there is an active call on that line.. hold it
		if (iActive != iLine && m_arrCalls[iActive].dwCallID != 0)
			m_pConn->SendEvent(PBXCMD_HC, m_dwExtension, m_arrCalls[iActive].dwCallID);
	}

	// Now if we have a call on this line, retrieve it.
	if (m_arrCalls[iLine].dwCallID != 0)
	{
		if (m_arrCalls[iLine].m_fConnected)
			m_pConn->SendEvent(PBXCMD_RTC, m_dwExtension, m_arrCalls[iLine].dwCallID);
		else
			m_pConn->SendEvent(PBXCMD_AN, m_dwExtension, m_arrCalls[iLine].dwCallID);
	}

	// Otherwise allow a new call to be dialed.
	else
	{
		m_btnLine[iLine].SetCheck(1);
		m_strLastDisplay = m_strDisplay;
		m_strDisplay.LoadString(IDS_ENTERNUM);
		UpdateData(FALSE);
		m_strDialedNumber = _T("");
		m_fPlacingCall = TRUE;
	}

}// CStationDlg::OnLine

/*****************************************************************************
** Procedure:  CStationDlg::OnDigit
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "TRANSFER" button is pressed
**
*****************************************************************************/
void CStationDlg::OnDigit() 
{
	static char chDigits[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#' };

	// Get the active line, if none, exit.
	int iActive = GetActiveLine();
	if (iActive == -1)
		return;

	// Determine which button was pressed.
	CWnd* pWnd = GetFocus();
	UINT uiDlgCtrl = pWnd->GetDlgCtrlID();
	ASSERT (uiDlgCtrl >= IDC_DIGIT1 && uiDlgCtrl <= IDC_DIGIT12);
	char cDigit = chDigits[uiDlgCtrl-IDC_DIGIT1];	

	// If we are placing a call, add the given digit to the display.
	if (m_fPlacingCall)
	{
		if (cDigit == '*' || cDigit == '#')
			return;

		CString strBuff;
		m_strDialedNumber += cDigit;
		strBuff.LoadString(IDS_ENTERNUM);
		m_strDisplay = strBuff + m_strDialedNumber;
		UpdateData(FALSE);
	}
	
	// Generate TONE on the line
	else if (m_arrCalls[iActive].dwCallID != 0 && m_arrCalls[iActive].m_fConnected)
		m_pConn->SendEvent(PBXCMD_GTD, m_dwExtension, m_arrCalls[iActive].dwCallID, cDigit);

}// CStationDlg::OnDigit

/*****************************************************************************
** Procedure:  CStationDlg::OnEnter
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "ENTER" button is pressed
**
*****************************************************************************/
void CStationDlg::OnEnter() 
{
	if (!m_fPlacingCall)
		return;

	// Get the active line
	int iActive = GetActiveLine();
	if (iActive == -1)
		return;

	// Turn off the ENTER NUMBER display
	m_fPlacingCall = FALSE;
	m_strDisplay = m_strLastDisplay;
	UpdateData(FALSE);

	// Validate the extension
	if (m_strDialedNumber.GetLength() < 4)
	{
		m_strDialedNumber = _T("");
		m_btnLine[iActive].SetCheck(0);
		Error(PBXERR_INVALDN);
		return;
	}

	// Send a place call command on the active line
	m_pConn->SendEvent(PBXCMD_PC, m_dwExtension, m_strDialedNumber);

}// CStationDlg::OnEnter

/*****************************************************************************
** Procedure:  CStationDlg::OnTransfer
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "TRANSFER" button is pressed
**
*****************************************************************************/
void CStationDlg::OnTransfer() 
{
	// Get the active call
	int iActive = GetActiveLine();
	if (iActive == -1)
		return;

	// If we are in a conference, just send the transfer with
	// no second call id.
	if (m_iConferenceCount > 0)
	{
		m_pConn->SendEvent(PBXCMD_TC, m_dwExtension, 0, m_arrCalls[iActive].dwCallID);
		return;
	}

	// Find the onHold call
	bool fFound = false;
	for (int iHold = 0; iHold < 3; iHold++)
	{
		if (m_arrCalls[iHold].dwCallID != 0 && iHold != iActive)
		{
			fFound = true;
			break;
		}
	}

	// If we don't have one, exit
	if (!fFound)
		return;

	// Send the transfer request
	m_pConn->SendEvent(PBXCMD_TC, m_dwExtension, m_arrCalls[iHold].dwCallID, m_arrCalls[iActive].dwCallID);
	
}// CStationDlg::OnTransfer

/*****************************************************************************
** Procedure:  CStationDlg::OnConference
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "CONFERENCE" button is pressed
**
*****************************************************************************/
void CStationDlg::OnConference() 
{
	// Get the active call
	int iActive = GetActiveLine();
	if (iActive == -1)
		return;

	// Find all the onHold calls and retrieve them
	for (int i = 0; i < 3; i++)
	{
		if (m_arrCalls[i].dwCallID != 0 && i != iActive &&
			m_arrCalls[i].m_fInConference == false)
			m_pConn->SendEvent(PBXCMD_RTC, m_dwExtension, m_arrCalls[i].dwCallID);
	}

}// CStationDlg::OnConference

/*****************************************************************************
** Procedure:  CStationDlg::OnHold
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "HOLD" button is pressed
**
*****************************************************************************/
void CStationDlg::OnHold() 
{
	// Get the active call
	int iActive = GetActiveLine();
	if (iActive == -1)
		return;

	// Place the call on hold
	if (m_arrCalls[iActive].dwCallID != 0 && m_arrCalls[iActive].m_fConnected)
		m_pConn->SendEvent(PBXCMD_HC, m_dwExtension, m_arrCalls[iActive].dwCallID);

}// CStationDlg::OnHold

/*****************************************************************************
** Procedure:  CStationDlg::OnReady
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "READY" button is pressed
**
*****************************************************************************/
void CStationDlg::OnReady() 
{
	if (!m_strAgentID.IsEmpty())
	{
		if (m_fReady)
			m_pConn->SendEvent(PBXCMD_CAS, m_dwExtension, m_strAgentID, 'N');
		else
			m_pConn->SendEvent(PBXCMD_CAS, m_dwExtension, m_strAgentID, 'R');
	}
	
}// CStationDlg::OnReady

/*****************************************************************************
** Procedure:  CStationDlg::OnCallWork
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "CALL WORK" button is pressed
**
*****************************************************************************/
void CStationDlg::OnCallWork() 
{
	if (!m_strAgentID.IsEmpty())
	{
		if (m_fCallWork)
			m_pConn->SendEvent(PBXCMD_CAS, m_dwExtension, m_strAgentID, 'N');
		else
			m_pConn->SendEvent(PBXCMD_CAS, m_dwExtension, m_strAgentID, 'W');
	}

}// CStationDlg::OnCallWork

/*****************************************************************************
** Procedure:  CStationDlg::OnRelease
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the "RELEASE" button is pressed
**
*****************************************************************************/
void CStationDlg::OnRelease() 
{
	// Get the active call
	int iActive = GetActiveLine();
	if (iActive == -1)
		return;

	// If we are in a conference, don't allow a drop
	if (m_iConferenceCount > 0)
		return;

	// If we are placing a call, stop it.
	if (m_fPlacingCall)
	{
		m_strDialedNumber = _T("");
		m_fPlacingCall = FALSE;
		m_btnLine[iActive].SetCheck(0);
		return;
	}

	// Release the call on this line.
	if (m_arrCalls[iActive].dwCallID != 0)
		m_pConn->SendEvent(PBXCMD_RC, m_dwExtension, m_arrCalls[iActive].dwCallID);

}// CStationDlg::OnRelease

/*****************************************************************************
** Procedure:  CStationDlg::Error
**
** Arguments: 'nError' - Error received
**
** Returns:    void
**
** Description: This puts an error message into the status bar
**
*****************************************************************************/
void CStationDlg::Error(int nError)
{
	UINT uiErrors[] = {
		IDS_INVALIDDEV,
		IDS_INVALIDDN,
		IDS_INVALIDPARAM,
		IDS_INVALIDCALLID,
		IDS_RESUNAVAIL,
		IDS_BADCRC,
		IDS_BADCOMMAND,
		IDS_BADSTATE,
		IDS_INVALIDLOGON,
		IDS_INVALIDAGENTID,
		IDS_INVALIDAGENTGROUP,
		IDS_INVALIDSTATE
	};

	CString strError;
	if (nError > PBXERR_INVALSTATE)
		strError.LoadString(IDS_ERRORUKNOWN);
	else
		strError.LoadString(uiErrors[nError-1]);

	m_dwStatus = GetTickCount();
	GetDlgItem(IDC_STATUS)->SetWindowText(strError);
	MessageBeep(0);

}// CStationDlg::Error

/*****************************************************************************
** Procedure:  CStationDlg::OnTimer
**
** Arguments: 'nIDEvent' - Event received
**
** Returns:    void
**
** Description: This is called periodically to handle work
**
*****************************************************************************/
void CStationDlg::OnTimer(UINT nIDEvent) 
{
	if ((m_dwStatus > 0 && GetTickCount() > m_dwStatus+5000) || (nIDEvent == 0))
	{
		CString strText;
		if (m_fConnected)
		{
			CString strBuff;
			strBuff.LoadString(IDS_LOGGEDON);
			strText.Format(_T("%s - %s"), strBuff, m_strAgentID);
		}
		else
			strText.LoadString(IDS_NOTLOGGEDON);
		GetDlgItem(IDC_STATUS)->SetWindowText(strText);
	}

}// CStationDlg::OnTimer

/*****************************************************************************
** Procedure:  CStationDlg::OnGetData
**
** Arguments: void
**
** Returns:    void
**
** Description: This processes received data from the simulator
**
*****************************************************************************/
LRESULT CStationDlg::OnGetData(WPARAM, LPARAM lParam)
{
	CString* pstrData = reinterpret_cast<CString*>(lParam);
	CMessage strData(*pstrData);
	if (!strData.IsEmpty())
	{
		CString strEvent = strData.GetNextElem();
		if (strEvent == _T("NAK"))
		{
			CString strCommand = strData.GetNextElem();
			if (strCommand == _T("ALO") || strCommand == _T("LO"))
			{
				OnCloseConn(0,0);
			}
			else if (strCommand == _T("PC"))
			{
				// Find the depressed button and release it.
				int iActive = GetActiveLine();
				ASSERT (iActive != -1);
				m_btnLine[iActive].SetCheck(0);
			}
			
			strData.Skip();
			int nError;
			sscanf(strData.GetNextElem(), "%lx", &nError);
			Error(nError);
		}
		else if (strEvent == _T("ACK"))
		{
			CString strCommand = strData.GetNextElem();
			if (strCommand == _T("LO"))
			{
				CString strExtension = strData.GetNextElem();
				m_dwExtension = atol(strExtension);
				m_fConnected = TRUE;
			}
			else if (strCommand == _T("ALO"))
			{
				strData.Skip();
				strData.Skip();
				CString strAgent = strData.GetNextElem();
				if (m_strAgentID.IsEmpty())
				{
					m_strAgentID = strAgent;
					CString strText;
					strText.LoadString(IDS_SIGNOFF);
					m_btnSignOn.SetWindowText(strText);

					CWnd* pwnd = GetWindow(GW_CHILD);
					while (pwnd != NULL && IsChild(pwnd))
					{
						if (pwnd->GetDlgCtrlID() != 0xffff)
							pwnd->EnableWindow (TRUE);
						pwnd = pwnd->GetWindow(GW_HWNDNEXT);
					}
					OnTimer(0);

					// If the display still reads "UNAVAILABLE" then
					// blank it out for now.
					if (m_strDisplay == UNAVAILABLE)
					{
						m_strDisplay = _T("");
						UpdateData(FALSE);
					}
				}
			}
			else if (strCommand == _T("CAS"))
			{
				if (m_strAgentID.IsEmpty())
					OnCloseConn(0,0);
			}
		}
		else if (strEvent == _T("PDC"))
		{
			strData.Skip();
			OnDisplayChange(strData.GetNextElem());
		}
		else if (strEvent == _T("PLC"))
		{
			strData.Skip();
			int iLampID = atoi(strData.GetNextElem());
			CString strState  = strData.GetNextElem();
			OnLampChange(iLampID, strState);
		}
		else if (strEvent == _T("CD"))
		{
			strData.Skip();
			DWORD dwCallID = atox(strData.GetNextElem());
			CString strANI = strData.GetNextElem();
			CString strDNIS = strData.GetNextElem();
			OnCallDetected(dwCallID, strANI, strDNIS);
		}
		else if (strEvent == _T("CP"))
		{
			strData.Skip();
			DWORD dwCallID = atox(strData.GetNextElem());
			CString strANI = strData.GetNextElem();
			CString strDNIS = strData.GetNextElem();
			OnCallPlaced(dwCallID, strANI, strDNIS);
		}
		else if (strEvent == _T("CR"))
		{
			OnCallRelease(atox(strData.GetNextElem()));
		}
		else if (strEvent == _T("CS"))
		{
			strData.Skip();
			DWORD dwCallID = atox(strData.GetNextElem());
			CString strState = strData.GetNextElem();
			OnCallStateChange(dwCallID, strState);
		}
		else if (strEvent == _T("CC"))
		{
			DWORD dwExtension = atol(strData.GetNextElem());
			DWORD dwCallID1 = atox(strData.GetNextElem());
			DWORD dwCallID2 = atox(strData.GetNextElem());
			DWORD dwCallID3 = atox(strData.GetNextElem());
			OnConferenceCreated(dwExtension, dwCallID1, dwCallID2, dwCallID3);
		}
		else if (strEvent == _T("CT"))
		{
			DWORD dwExtension = atol(strData.GetNextElem());
			DWORD dwCallID1 = atox(strData.GetNextElem());
			DWORD dwCallID2 = atox(strData.GetNextElem());
			DWORD dwTarget = atol(strData.GetNextElem());
			CString strANI = strData.GetNextElem();
			CString strDNIS = strData.GetNextElem();
			OnCallTransferred(dwExtension, dwCallID1, dwCallID2, dwTarget, strANI, strDNIS);
		}
		else if (strEvent == _T("ASC"))
		{
			strData.Skip();
			strData.Skip();
			CString strState = strData.GetNextElem();
			OnAgentStateChange(strState[0]);
		}
	}
	return 0;

}// CStationDlg::OnGetData

/*****************************************************************************
** Procedure:  CStationDlg::OnAgentStateChange
**
** Arguments: 'cState' - New agent state
**
** Returns:    void
**
** Description: This processes the ASC command
**
*****************************************************************************/
void CStationDlg::OnAgentStateChange(char cState)
{
	if (cState == 'S')
	{
		// Set the button text
		CString strText;
		strText.LoadString(IDS_SIGNON);
		m_btnSignOn.SetWindowText(strText);

		// Kill the agent id
		m_strAgentID = _T("");

		// Disable all our buttons
		CWnd* pwnd = GetWindow(GW_CHILD);
		while (pwnd != NULL && IsChild(pwnd))
		{
			if (pwnd->GetDlgCtrlID() != 0xffff && pwnd != &m_btnSignOn)
			{
				char chName[255];
				GetClassName(pwnd->GetSafeHwnd(), chName, 255);
				if (!stricmp(chName, "BUTTON"))
					pwnd->EnableWindow(FALSE);
			}
			pwnd = pwnd->GetWindow(GW_HWNDNEXT);
		}
	}

}// CStationDlg::OnAgentStateChange

/*****************************************************************************
** Procedure:  CStationDlg::OnDisplayChange
**
** Arguments: 'strDisplay' - New Display
**
** Returns:    void
**
** Description: This processes the PDC command
**
*****************************************************************************/
void CStationDlg::OnDisplayChange(CString& strDisplay)
{
	// If we are placing a call, ignore the state change.
	if (m_fPlacingCall)
		m_strLastDisplay = strDisplay.Left(40) + "\r" + strDisplay.Right(40);
	else
	{
		UpdateData(TRUE);
		m_strDisplay = strDisplay.Left(40) + "\r" + strDisplay.Right(40);
		UpdateData(FALSE);
	}

}// CStationDlg::OnDisplayChange

/*****************************************************************************
** Procedure:  CStationDlg::OnLampChange
**
** Arguments: 'iLampID' - Lamp id
**            'strState' - New state
**
** Returns:    void
**
** Description: This processes the PLC command
**
*****************************************************************************/
void CStationDlg::OnLampChange(int iLampID, CString& strState)
{
	UpdateData(TRUE);
	if (iLampID == 0) // Ready
	{
		m_fReady = (strState[0] == 'S');
	}
	else if (iLampID == 1) // InWork
	{
		m_fCallWork = (strState[0] == 'S');
	}
	UpdateData(FALSE);

}// CStationDlg::OnLampChange

/*****************************************************************************
** Procedure:  CStationDlg::OnCallDetected
**
** Arguments: 'dwCallID' - Call id
**            'strANI' - ANI
**            'strDNIS' - DNIS
**
** Returns:    void
**
** Description: This processes the CD event
**
*****************************************************************************/
void CStationDlg::OnCallDetected(DWORD dwCallID, CString& strANI, CString& strDNIS)
{
	// Find a button for this call
	bool fFound = false;
	for (int i = 0; i < 3; i++)
	{
		if (m_arrCalls[i].dwCallID == dwCallID)
		{
			fFound = true;
			break;
		}
	}

	if (!fFound)
	{
		for (i = 0; i < 3 && !fFound; i++)
			if (!m_arrCalls[i].InUse())
				break;
	}

	int iPos = strANI.Find(':');
	if (iPos >= 0)
		strANI = strANI.Mid(iPos+1);

	m_arrCalls[i].dwCallID = dwCallID;
	m_arrCalls[i].strNumber = strANI;

	// Set the text for the button
	if (strANI.GetLength() > 4)
		strANI = strANI.Left(4) + _T("..");
	m_btnLine[i].SetWindowText(strANI);

}// CStationDlg::OnCallDetected

/*****************************************************************************
** Procedure:  CStationDlg::OnCallRelease
**
** Arguments: 'dwCallID' - Call id
**
** Returns:    void
**
** Description: This processes the CR event
**
*****************************************************************************/
void CStationDlg::OnCallRelease(DWORD dwCallID)
{
	// Find the call
	for (int i = 0; i < 3; i++)
	{
		if (m_arrCalls[i].dwCallID == dwCallID)
		{
			if (m_arrCalls[i].m_fInConference)
			{
				if (--m_iConferenceCount == 1)
				{
					for (int x = 0; x < 3; x++)
					{
						if (m_arrCalls[x].m_fInConference)
						{
							m_arrCalls[x].m_fInConference = false;
							m_btnLine[x].SetCheck(1);
						}
					}
				}
			}

			m_arrCalls[i].Empty();
			CString strButton;
			strButton.Format(_T("Line %d"), i+1);
			m_btnLine[i].SetWindowText(strButton);
			m_btnLine[i].SetCheck(0);
			break;
		}
	}

}// CStationDlg::OnCallRelease

/*****************************************************************************
** Procedure:  CStationDlg::OnCallPlaced
**
** Arguments: 'dwCallID' - Call id
**            'strANI' - ANI
**            'strDNIS' - DNIS
**
** Returns:    void
**
** Description: This processes the CP event
**
*****************************************************************************/
void CStationDlg::OnCallPlaced(DWORD dwCallID, CString& strANI, CString& strDNIS)
{
	// Look for the depressed button.
	int iActive = GetActiveLine();
	if (iActive == -1)
	{
		for (iActive = 0; iActive < 3; iActive++)
		{
			if (m_arrCalls[iActive].dwCallID == 0)
				break;
		}
	}

	ASSERT (m_arrCalls[iActive].dwCallID == 0);

	int iPos = strDNIS.Find(':');
	if (iPos >= 0)
		strDNIS = strDNIS.Mid(iPos+1);

	m_arrCalls[iActive].dwCallID = dwCallID;
	m_arrCalls[iActive].strNumber = strDNIS;

	// Set the text for the button
	if (strDNIS.GetLength() > 4)
		strDNIS = strDNIS.Left(4) + _T("..");
	m_btnLine[iActive].SetWindowText(strDNIS);

}// CStationDlg::OnCallPlaced

/*****************************************************************************
** Procedure:  CStationDlg::OnCallStateChange
**
** Arguments: 'dwCallID' - Call id
**            'strState' - Call state change
**
** Returns:    void
**
** Description: This processes the CS event
**
*****************************************************************************/
void CStationDlg::OnCallStateChange(DWORD dwCallID, CString& strState)
{
	// Find the call
	for (int i = 0; i < 3; i++)
	{
		if (m_arrCalls[i].dwCallID == dwCallID)
		{
			if (strState[0] == 'C' || strState[0] == 'D')
			{
				m_arrCalls[i].m_fConnected = true;

				// Depress this button, kill all others.
				for (int iLine = 0; iLine < 3; iLine++)
					m_btnLine[iLine].SetCheck((iLine==i));
			}
			else if (strState[0] == 'H')
			{
				// If the call was part of a conference, take it out.
				if (m_arrCalls[i].m_fInConference)
				{
					m_arrCalls[i].m_fInConference = false;
					if (--m_iConferenceCount <= 1)
					{
						// We no longer have a conference..
						m_iConferenceCount = 0;

						// Kill the conference and set the current call as active.
						for (int x = 0; x < 3; x++)
						{
							if (m_arrCalls[x].m_fInConference)
							{
								m_arrCalls[x].m_fInConference = false;
								m_btnLine[x].SetCheck(1);
								break;
							}
						}
					}
				}
				m_btnLine[i].SetCheck(0);
			}
		}
	}

}// CStationDlg::OnCallStateChange

/*****************************************************************************
** Procedure:  CStationDlg::OnCloseConn
**
** Arguments: void
**
** Returns:    void
**
** Description: This processes the close connection event
**
*****************************************************************************/
LRESULT CStationDlg::OnCloseConn(WPARAM, LPARAM)
{
	m_fConnected = FALSE;
	m_dwExtension = 0;
	m_strDisplay = UNAVAILABLE;
	UpdateData(FALSE);
	OnTimer(0);
	OnAgentStateChange('S');

	delete m_pConn;
	m_pConn = NULL;

	return 0;

}// CStationDlg::OnCloseConn

/*****************************************************************************
** Procedure:  CStationDlg::OnCallTransferred
**
** Arguments: 'dwExtension' - Extension which is losing call
**            'dwCallID1'   - Old Callid
**            'dwCallID2'	- New Callid
**            'dwTarget'	- Target extension
**
** Returns:    void
**
** Description: This processes the CT event
**
*****************************************************************************/
void CStationDlg::OnCallTransferred(DWORD dwExtension, DWORD dwCallID1, DWORD dwCallID2, 
				DWORD dwTarget, CString& strANI, CString& strDNIS)
{
	// If there is not a new callid then simply clear the call - it has
	// been transferred.
	if (dwCallID2 == 0)
	{
		// If we are losing the call...
		if (dwExtension == m_dwExtension)
			OnCallRelease(dwCallID1);

		// Otherwise we are gaining the call - change the callerid
		else if (dwTarget == m_dwExtension)
		{
			for (int i =0; i < 3; i++)
			{
				if (m_arrCalls[i].dwCallID == dwCallID1)
				{
					int iPos = strANI.Find(':');
					if (iPos >= 0)
						strANI = strANI.Mid(iPos+1);

					m_arrCalls[i].strNumber = strANI;

					// Set the text for the button
					if (strANI.GetLength() > 4)
						strANI = strANI.Left(4) + _T("..");
					m_btnLine[i].SetWindowText(strANI);
					break;
				}
			}
		}
	}
	else
	{
		// If we are losing the call...
		if (dwExtension == m_dwExtension)
		{
			// Locate the two callid's and delete them.
			OnCallRelease(dwCallID1);
			OnCallRelease(dwCallID2);
		}

		// Or gaining call...
		else if (dwTarget == m_dwExtension)
		{
			for (int i =0; i < 3; i++)
			{
				if (m_arrCalls[i].dwCallID == dwCallID2)
				{
					// Save the new callid off
					m_arrCalls[i].dwCallID = dwCallID1;

					int iPos = strANI.Find(':');
					if (iPos >= 0)
						strANI = strANI.Mid(iPos+1);

					m_arrCalls[i].strNumber = strANI;

					// Set the text for the button
					if (strANI.GetLength() > 4)
						strANI = strANI.Left(4) + _T("..");
					m_btnLine[i].SetWindowText(strANI);
				}
			}
		}
	}

}// CStationDlg::OnCallTransferred

/*****************************************************************************
** Procedure:  CStationDlg::OnConferenceCreated
**
** Arguments: 'dwExtension' - Extension which is losing call
**            'dwCallID1'   - Callid 1
**            'dwCallID2'	- Callid 2
**            'dwCallID3'	- Callid 3
**
** Returns:    void
**
** Description: This processes the CC event
**
*****************************************************************************/
void CStationDlg::OnConferenceCreated(DWORD dwExtension, DWORD dwCallID1, DWORD dwCallID2, DWORD dwCallID3)
{
	// Ignore if this is not for us.
	if (dwExtension != m_dwExtension)
		return;

	// Mark all the calls in the conference
	m_iConferenceCount = 0;
	for (int i = 0; i < 3; i++)
	{
		if ((dwCallID1 != 0 && m_arrCalls[i].dwCallID == dwCallID1) ||
			(dwCallID2 != 0 && m_arrCalls[i].dwCallID == dwCallID2) ||
			(dwCallID3 != 0 && m_arrCalls[i].dwCallID == dwCallID3))
		{
			m_arrCalls[i].m_fInConference = true;
			m_btnLine[i].SetCheck(1);
			m_iConferenceCount++;
		}
		else
		{
			m_arrCalls[i].m_fInConference = false;
			m_btnLine[i].SetCheck(0);
		}
	}

	ASSERT (m_iConferenceCount > 1);

}// CStationDlg::OnConferenceCreated
