/*******************************************************************/
//
// LOGONDLG.CPP
//
// Logon Dialog
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
#include "LogonDlg.h"
#include "ddx.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
const UINT AGENTID_SIZE = 25;
const UINT AGENTPW_SIZE = 9;

/*****************************************************************************
** Procedure:  CLogonDlg::CLogonDlg
**
** Arguments: 'pParent' - parent window
**
** Returns:   void
**
** Description:  Constructor for the logon dialog
**
*****************************************************************************/
CLogonDlg::CLogonDlg(CWnd* pParent)
	: CDialog(CLogonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogonDlg)
	m_strAgentID = _T("");
	m_strPassword = _T("");
	m_nPort = 0;
	m_dwExtension = 1000;
	//}}AFX_DATA_INIT

}// CLogonDlg::CLogonDlg

/*****************************************************************************
** Procedure:  CLogonDlg::DoDataExchange
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
void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctlIPAddress);
	DDX_Text(pDX, IDC_AGENTID, m_strAgentID);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_IPText(pDX, IDC_IPADDRESS1, m_strIPAddress);
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDX_Text(pDX, IDC_EXTENSION, m_dwExtension);
	//}}AFX_DATA_MAP

}// CLogonDlg::DoDataExchange

/*---------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//{{AFX_MSG_MAP(CLogonDlg)
	ON_EN_CHANGE(IDC_AGENTID, OnChange)
// BUGBUG: This notification has some problem in the latest VC6 implementation
// Don't have time to debug this, but causes crash with stack problems (looks like
// not all elements are being popped off the stack) even if the OnChange() member
// is empty.
//	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, OnChange)
	ON_EN_CHANGE(IDC_PASSWORD, OnChange)
	ON_EN_CHANGE(IDC_PORT, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CLogonDlg::OnInitDialog
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
BOOL CLogonDlg::OnInitDialog() 
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

	// Limit the text on the logon
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_PORT))->LimitText(4);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_AGENTID))->LimitText(AGENTID_SIZE);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_PASSWORD))->LimitText(AGENTPW_SIZE);

	// If we have no IP address, then require that to be filled.
	if (m_strIPAddress.IsEmpty())
		return TRUE;

	// Otherwise, move to the number and skip the IP control
	GotoDlgCtrl(GetDlgItem(IDC_EXTENSION));
	return FALSE;

}// CLogonDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CLogonDlg::OnChange
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
void CLogonDlg::OnChange() 
{
	UpdateData(TRUE);
	m_strAgentID.TrimRight();
	m_btnOK.EnableWindow(!m_strIPAddress.IsEmpty() && m_nPort > 0 &&
						 !m_strAgentID.IsEmpty() && m_dwExtension > 0);

}// CLogonDlg::OnChange

/*****************************************************************************
** Procedure:  CLogonDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CLogonDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		if (m_strIPAddress.IsEmpty())
		{
			AfxMessageBox(IDS_ENTERIPADDR);
			GotoDlgCtrl(GetDlgItem(IPN_FIELDCHANGED));
			return;
		}

		m_strAgentID.TrimRight();
		if (m_dwExtension < 1000 || m_dwExtension > 8999)
		{
			AfxMessageBox(IDS_BADEXTENSION);
			GotoDlgCtrl(GetDlgItem(IDC_EXTENSION));
			return;
		}

		CDialog::OnOK();
	}

}// CLogonDlg::OnOK
