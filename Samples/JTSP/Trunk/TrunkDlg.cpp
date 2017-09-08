/*******************************************************************/
//
// TRUNKDLG.CPP
//
// Main Trunk program dialog
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
#include "TrunkDlg.h"
#include "Interface.h"
#include "ddx.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**************************************************************************
** CTrunkApp
**
** Application class object
**
***************************************************************************/
class CTrunkApp : public CWinApp
{
// Overrides
public:
	virtual BOOL InitInstance();
};

/*---------------------------------------------------------------*/
// GLOBAL VARIABLES
/*---------------------------------------------------------------*/
CTrunkApp theApp;

/*****************************************************************************
** Procedure:  CTrunkApp::InitInstance
**
** Arguments: void
**
** Returns:   Boolean to run message loop
**
** Description:  Initial entry point for program
**
*****************************************************************************/
BOOL CTrunkApp::InitInstance()
{
	// Initialize WinSock
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	InitCommonControls();

	// Invoke our dialog
	CTrunkDlg dlg;
	m_pMainWnd = &dlg;

	// Set our registry key section
	SetRegistryKey(_T("JulMar Technology\\TrunkDlg"));

	// Restore our data
	dlg.m_strIPAddress = GetProfileString(_T("Config"), _T("IPAddress"));
	dlg.m_nPort = atoi(GetProfileString(_T("Config"), _T("IPPort")));

	dlg.DoModal();

	WriteProfileString(_T("Config"), _T("IPAddress"), dlg.m_strIPAddress);
	CString strPort;
	strPort.Format(_T("%d"), dlg.m_nPort);
	WriteProfileString(_T("Config"), _T("IPPort"), strPort);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;

}// CTrunkApp::InitInstance

/*****************************************************************************
** Procedure:  CTrunkDlg::CTrunkDlg
**
** Arguments: void
**
** Returns:   void
**
** Description:  Constructor for the trunk dialog
**
*****************************************************************************/
CTrunkDlg::CTrunkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrunkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrunkDlg)
	m_nPort = 0;
	m_strNumber = _T("");
	m_strNumberNI = _T("");
	m_strNameNI = _T("");
	m_strCallID = _T("");
	m_pConn = NULL;
	m_fRunning = false;
	//}}AFX_DATA_INIT

}// CTrunkDlg::CTrunkDlg

/*****************************************************************************
** Procedure:  CTrunkDlg::~CTrunkDlg
**
** Arguments: void
**
** Returns:   void
**
** Description:  Destructor for the trunk dialog
**
*****************************************************************************/
CTrunkDlg::~CTrunkDlg()
{
	delete m_pConn;

}// CTrunkDlg::~CTrunkDlg

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
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrunkDlg)
	DDX_Control(pDX, IDC_PORT, m_edtPort);
	DDX_Control(pDX, IDC_NINUMBER, m_edtNumberNI);
	DDX_Control(pDX, IDC_NINAME, m_edtNameNI);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctlIPAddress);
	DDX_Control(pDX, IDC_DIALHANG, m_btnStartStop);
	DDX_Control(pDX, IDC_NUMBER, m_cbNumber);
	DDX_CBString(pDX, IDC_NUMBER, m_strNumber);
	DDX_Text(pDX, IDC_NINUMBER, m_strNumberNI);
	DDX_Text(pDX, IDC_NINAME, m_strNameNI);
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDX_IPText(pDX, IDC_IPADDRESS1, m_strIPAddress);
	//}}AFX_DATA_MAP

}// CTrunkDlg::DoDataExchange

/*---------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CTrunkDlg, CDialog)
	//{{AFX_MSG_MAP(CTrunkDlg)
	ON_MESSAGE(UM_GOTDATA, OnGetData)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DIALHANG, OnDialhang)
	ON_CBN_EDITCHANGE(IDC_NUMBER, OnChangeNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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

	// Center our window
	CenterWindow();

	// Set our limits on the edits
	m_edtPort.LimitText(4);
	m_edtNameNI.LimitText(20);
	m_edtNumberNI.LimitText(20);
	m_cbNumber.LimitText(20);

	// If we have no IP address, then require that to be filled.
	if (m_strIPAddress.IsEmpty())
		return TRUE;

	// Otherwise, move to the number and skip the IP control
	GotoDlgCtrl(&m_cbNumber);
	return FALSE;

}// CTrunkDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CTrunkDlg::OnClose
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the dialog is being closed
**
*****************************************************************************/
void CTrunkDlg::OnClose() 
{
	UpdateData(TRUE);
	CDialog::OnClose();

}// CTrunkDlg::OnClose

/*****************************************************************************
** Procedure:  CTrunkDlg::OnDialhang
**
** Arguments: void
**
** Returns:    void
**
** Description: This function starts the trunk dialer into action and stops
**              it later if desired.
**
*****************************************************************************/
void CTrunkDlg::OnDialhang() 
{
	UpdateData(TRUE);

	// Stop if running
	if (m_fRunning)
	{
		Stop();
		delete m_pConn;
		m_pConn = NULL;
	}

	else
	{
		m_pConn = new CConnection(m_strIPAddress, m_nPort);
		if (!m_pConn->IsValid())
		{
			AfxMessageBox(IDS_NOSERVER);
			delete m_pConn;
			m_pConn = NULL;
		}
		else
			Start();
	}

}// CTrunkDlg::OnDialhang

/*****************************************************************************
** Procedure:  CTrunkDlg::Stop
**
** Arguments: void
**
** Returns:    void
**
** Description: This function stops the trunk dialer
**
*****************************************************************************/
void CTrunkDlg::Stop()
{
	ASSERT (m_fRunning);
	m_fRunning = false;

	if (m_strCallID.IsEmpty() == false)
		m_pConn->SendEvent(PBXCMD_TRC, m_strCallID);

	m_btnStartStop.SetWindowText("Start");

}// CTrunkDlg::Stop

/*****************************************************************************
** Procedure:  CTrunkDlg::Start
**
** Arguments: void
**
** Returns:    void
**
** Description: This function starts the trunk dialer into action and stops
**              it later if desired.
**
*****************************************************************************/
void CTrunkDlg::Start()
{
	UpdateData(TRUE);

	ASSERT (m_fRunning == false);
	m_fRunning = true;

	m_strCallID = _T("");

	CString strText;
	m_cbNumber.GetWindowText(strText);

	// Remove all possible digits
	for (int i = 0; i < strText.GetLength(); i++)
	{
		if (strchr("0123456789() -", strText[i]) == NULL) 
		{
			strText = strText.Left(i) + strText.Mid(i+1);
			i = 0;
		}
	}

	// Add it to our combo box.
	if (m_cbNumber.FindStringExact(-1, strText) == CB_ERR)
	{
		if (m_cbNumber.InsertString(0, strText) > 10)
			m_cbNumber.DeleteString(m_cbNumber.GetCount()-1);
	}

	// Dial the number
	m_pConn->SendEvent(PBXCMD_TPC, m_strNumber, m_strNameNI, m_strNumberNI);

	m_btnStartStop.SetWindowText("Stop");

}// CTrunkDlg::Start

/*****************************************************************************
** Procedure:  CTrunkDlg::OnChangeNumber
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the number in the combo box is changed
**
*****************************************************************************/
void CTrunkDlg::OnChangeNumber() 
{
	// If the text entry field is non-blak and has at least 4 characters,
	// then enable the button.
	CString strText;
	m_cbNumber.GetWindowText(strText);

	// Remove all possible digits
	for (int i = 0; i < strText.GetLength(); i++)
	{
		if (strchr("0123456789() -", strText[i]) == NULL) 
		{
			strText = strText.Left(i) + strText.Mid(i+1);
			i = 0;
		}
	}

	// If we have enough digits to dial
	m_btnStartStop.EnableWindow((strText.GetLength() >= 4));

}// CTrunkDlg::OnChangeNumber

/*****************************************************************************
** Procedure:  CTrunkDlg::OnGetData
**
** Arguments: void
**
** Returns:    void
**
** Description: This processes received data from the simulator
**
*****************************************************************************/
LRESULT CTrunkDlg::OnGetData(WPARAM, LPARAM lParam)
{
	CString* pstrData = reinterpret_cast<CString*>(lParam);
	CString strData(*pstrData);
	if (!strData.IsEmpty())
	{
		// If a command failed, determine what it was.
		if (strData.Left(7) == _T("NAK;TPC"))
		{
			// Stop the process
			DWORD dwError;
			sscanf(strData.Mid(8, 4), "%lx", &dwError);
			switch (dwError)
			{
				case PBXERR_INVALDN: dwError = IDS_INVALIDDN; break;
				case PBXERR_RESUNAVAIL: dwError = IDS_RESUNAVAIL; break;
				default: dwError = IDS_ERROR; break;
			}
			AfxMessageBox(dwError);
			Stop();
		}
		else if (strData.Left(7) == _T("ACK;TPC"))
		{
			// Record the callid and trunkid.
			m_strCallID = strData.Mid(13);
		}
	}
	return 0;

}// CTrunkDlg::OnGetData
