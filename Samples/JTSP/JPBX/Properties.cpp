/*******************************************************************/
//
// PROPERTIES.CPP
//
// Global PBX properties dialog
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
#include "Properties.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*****************************************************************************
** Procedure:  CPbxPropertiesDlg::CPbxPropertiesDlg
**
** Arguments: void
**
** Returns:    void
**
** Description: Constructor for the properties dialog
**
*****************************************************************************/
CPbxPropertiesDlg::CPbxPropertiesDlg(CWnd* pParent)
	: CBaseDialog(CPbxPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPbxPropertiesDlg)
	m_nPort = 0;
	m_fLog = FALSE;
	m_strFileName = _T("");
	//}}AFX_DATA_INIT

}// CPbxPropertiesDlg::CPbxPropertiesDlg

/*****************************************************************************
** Procedure:  CPbxPropertiesDlg::DoDataExchange
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
void CPbxPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPbxPropertiesDlg)
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDX_Check(pDX, IDC_ENABLELOGGING, m_fLog);
	DDX_Text(pDX, IDC_FILENAME, m_strFileName);
	//}}AFX_DATA_MAP

}// CPbxPropertiesDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CPbxPropertiesDlg message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CPbxPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CPbxPropertiesDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CPbxPropertiesDlg::OnInitDialog
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
BOOL CPbxPropertiesDlg::OnInitDialog() 
{
	// Call the base initialization
	CBaseDialog::OnInitDialog();

	// Limit our text
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_PORT))->LimitText(4);
	
	return TRUE;

}// CPbxPropertiesDlg

/*****************************************************************************
** Procedure:  CPbxPropertiesDlg::OnOK
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the user presses the "OK" button.
**              It is responsible for dismissing the dialog.
**
*****************************************************************************/
void CPbxPropertiesDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		if (m_nPort <= 0)
		{
			AfxMessageBox(IDS_INVALIDPORT);
			GotoDlgCtrl(GetDlgItem(IDC_PORT));
			return;
		}

		if (m_fLog && m_strFileName.IsEmpty())
		{
			AfxMessageBox(IDS_INVALIDFILE);
			GotoDlgCtrl(GetDlgItem(IDC_FILENAME));
			return;
		}
	}
	CDialog::OnOK();

}// CPbxPropertiesDlg::OnOK

/*****************************************************************************
** Procedure:  CPbxPropertiesDlg::OnBrowse
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when user is browsing for a new log file.
**
*****************************************************************************/
void CPbxPropertiesDlg::OnBrowse() 
{
	static TCHAR BASED_CODE szFilter[] = _T("Log Files (*.log)|*.log|All Files (*.*)|*.*||");
	CFileDialog fd(TRUE, _T("LOG"), _T("JPBX.LOG"), 
			OFN_ENABLESIZING | OFN_EXPLORER | OFN_PATHMUSTEXIST,
			szFilter, AfxGetMainWnd());

	if (fd.DoModal() == IDOK)
		m_strFileName = fd.GetPathName();

}// CPbxPropertiesDlg::OnBrowse
