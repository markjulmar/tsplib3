/******************************************************************************/
//                                                                        
// UUIPROPS.CPP - UserUser information properties
//                                                                        
// Copyright (C) 1998 Mark C. Smith, JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
// For internal use only
//                                                                        
/******************************************************************************/

/*---------------------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "tspwizard.h"
#include "UUIProps.h"
#include "TSPWizardaw.h"

/*---------------------------------------------------------------------------*/
// DEBUG STATEMENTS
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------------------*/
// RTTI
/*---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CUUIProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CUUIProps, CDialog)
	//{{AFX_MSG_MAP(CUUIProps)
	ON_BN_CLICKED(IDC_CHECK1, OnUUIChecked)
	ON_BN_CLICKED(IDC_CHECK2, OnUUIChecked)
	ON_BN_CLICKED(IDC_CHECK3, OnUUIChecked)
	ON_BN_CLICKED(IDC_CHECK4, OnUUIChecked)
	ON_BN_CLICKED(IDC_CHECK5, OnUUIChecked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CUUIProps::CUUIProps
//
// Constructor for the UUI property page
//
CUUIProps::CUUIProps() : CDialog(CUUIProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CUUIProps)
	m_strAccept = _T("0");
	m_strAnswer = _T("0");
	m_strDrop = _T("0");
	m_strMakeCall = _T("0");
	m_strSendUUI = _T("0");
	//}}AFX_DATA_INIT

}// CUUIProps::CUUIProps

///////////////////////////////////////////////////////////////////////////
// CUUIProps::DoDataExchange
//
// Dialog Data Exchange for the UUI property page
//
void CUUIProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUUIProps)
	DDX_Control(pDX, IDC_SPIN6, m_ctlSpin5);
	DDX_Control(pDX, IDC_SPIN5, m_ctlSpin4);
	DDX_Control(pDX, IDC_SPIN4, m_ctlSpin3);
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Text(pDX, IDC_ACCEPT, m_strAccept);
	DDX_Text(pDX, IDC_ANSWER, m_strAnswer);
	DDX_Text(pDX, IDC_DROPCALL, m_strDrop);
	DDX_Text(pDX, IDC_PLACE, m_strMakeCall);
	DDX_Text(pDX, IDC_SEND, m_strSendUUI);
	//}}AFX_DATA_MAP

}// CUUIProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CUUIProps::OnInitDialog
//
// Handle the initialization of the dialog
//
BOOL CUUIProps::OnInitDialog() 
{
	// Attach controls
	CDialog::OnInitDialog();
	
	// Limit the spinboxes to 4k
	m_ctlSpin1.SetRange32(0, 4096*1024);
	m_ctlSpin2.SetRange32(0, 4096*1024);
	m_ctlSpin3.SetRange32(0, 4096*1024);
	m_ctlSpin4.SetRange32(0, 4096*1024);
	m_ctlSpin5.SetRange32(0, 4096*1024);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CUUIProps::OnOK
//
// Dismiss the dialog
//
void CUUIProps::OnOK() 
{
	if (UpdateData(TRUE))
	{
		if (m_strAccept.IsEmpty())
			m_strAccept = _T("0");
		if (m_strAnswer.IsEmpty())
			m_strAnswer = _T("0");
		if (m_strDrop.IsEmpty())
			m_strDrop = _T("0");
		if (m_strMakeCall.IsEmpty())
			m_strMakeCall = _T("0");
		if (m_strSendUUI.IsEmpty())
			m_strSendUUI = _T("0");

		TSPWizardaw.m_Dictionary["UUI_ACCEPT"] = m_strAccept;
		TSPWizardaw.m_Dictionary["UUI_ANSWER"] = m_strAnswer;
		TSPWizardaw.m_Dictionary["UUI_DROP"] = m_strDrop;
		TSPWizardaw.m_Dictionary["UUI_MAKECALL"] = m_strMakeCall;
		TSPWizardaw.m_Dictionary["UUI_SEND"] = m_strSendUUI;

		int iMaxValue = 0;
		if (atoi(m_strAccept) > iMaxValue)
			iMaxValue = atoi(m_strAccept);
		if (atoi(m_strAnswer) > iMaxValue)
			iMaxValue = atoi(m_strAnswer);
		if (atoi(m_strDrop) > iMaxValue)
			iMaxValue = atoi(m_strDrop);
		if (atoi(m_strMakeCall) > iMaxValue)
			iMaxValue = atoi(m_strMakeCall);
		if (atoi(m_strSendUUI) > iMaxValue)
			iMaxValue = atoi(m_strSendUUI);

		if (iMaxValue == 0)
		{
			AfxMessageBox(IDS_NOUUISELECTED);
			GotoDlgCtrl(GetDlgItem(IDC_CHECK1));
			return;
		}

		char chBuff[50];
		wsprintf(chBuff, "%d", iMaxValue);
		TSPWizardaw.m_Dictionary["UUI_MAXSIZE"] = chBuff;

		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUUIProps::OnUUIChecked
//
// A UUI item has been checked - enable/disable the proper edit.
//
void CUUIProps::OnUUIChecked() 
{
	CButton* pWnd = reinterpret_cast<CButton*>(GetFocus());
	CWnd* pWndNext = pWnd->GetWindow(GW_HWNDNEXT);
	if (pWnd != NULL && pWndNext != NULL)
	{
		if (pWnd->GetCheck() != 0)
		{
			pWndNext->EnableWindow(TRUE);
			GotoDlgCtrl(pWndNext);
		}
		else
		{
			pWndNext->SetWindowText("0");
			UpdateData(TRUE);
			pWndNext->EnableWindow(FALSE);
		}
	}

}// CUUIProps::OnUUIChecked
