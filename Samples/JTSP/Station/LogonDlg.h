/*******************************************************************/
//
// LOGONDLG.H
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_LOGON_INC__
#define __JPBX_LOGON_INC__

/**************************************************************************
** CLogonDlg
**
** This dialog allows a logon against the PBX
**
***************************************************************************/
class CLogonDlg : public CDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CLogonDlg)
	enum { IDD = IDD_SIGNON };
	CButton	m_btnOK;
	CIPAddressCtrl	m_ctlIPAddress;
	CString	m_strAgentID;
	CString	m_strPassword;
	CString m_strIPAddress;
	int		m_nPort;
	DWORD	m_dwExtension;
	//}}AFX_DATA

// Construction
public:
	CLogonDlg(CWnd* pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLogonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChange();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __JPBX_LOGON_INC__
