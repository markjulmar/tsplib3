/*******************************************************************/
//
// GENTRAFFIC.H
//
// Soak/Load test management dialog
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
#include "GenTraffic.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// GLOBALS and CONSTANTS
/*---------------------------------------------------------------*/
const int MAX_CPS = 20;

/*****************************************************************************
** Procedure:  CGenTraffic::CGenTraffic
**
** Arguments: void
**
** Returns:    void
**
** Description: Constructor for the traffic generation dialog
**
*****************************************************************************/
CGenTraffic::CGenTraffic(CWnd* pParent)
	: CBaseDialog(CGenTraffic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenTraffic)
	m_nCPS = 0;
	m_fRunning = false;
	//}}AFX_DATA_INIT

}// CGenTraffic::CGenTraffic

/*****************************************************************************
** Procedure:  CGenTraffic::DoDataExchange
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
void CGenTraffic::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenTraffic)
	DDX_Control(pDX, IDC_CPS, m_edtCPS);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpinner);
	DDX_Text(pDX, IDC_CPS, m_nCPS);
	//}}AFX_DATA_MAP

}// CGenTraffic::DoDataExchange

/*---------------------------------------------------------------*/
// CGenTraffic message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CGenTraffic, CBaseDialog)
	//{{AFX_MSG_MAP(CGenTraffic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CGenTraffic::OnInitDialog
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
BOOL CGenTraffic::OnInitDialog() 
{
	// Call the base initialization
	CBaseDialog::OnInitDialog();

	// Limit our text
	m_edtCPS.LimitText(2);
	m_ctlSpinner.SetRange(0, MAX_CPS);

	// Change our button if necessary
	if (m_fRunning)
	{
		m_edtCPS.EnableWindow(FALSE);
		m_ctlSpinner.EnableWindow(FALSE);

		CString strBuff;
		strBuff.LoadString(IDS_STOP);
		m_btnOK.SetWindowText(strBuff);
	}

	return TRUE;

}// CGenTraffic::OnInitDialog

/*****************************************************************************
** Procedure:  CGenTraffic::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CGenTraffic::OnOK() 
{
	if (UpdateData(TRUE))
	{
		if (!m_fRunning && (m_nCPS < 1 || m_nCPS > MAX_CPS))
		{
			AfxMessageBox(IDS_BADCPS);
			GotoDlgCtrl(&m_edtCPS);
			return;
		}
	}

	CBaseDialog::OnOK();

}// CGenTraffic::OnOK
