/*******************************************************************/
//
// TRUNKDLG.H
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __TRUNKDLG_INC__
#define __TRUNKDLG_INC__

/*---------------------------------------------------------------*/
// PREDINITIONS
/*---------------------------------------------------------------*/
class CConnection;

/**************************************************************************
** CTrunkDlg
**
** Trunk object
**
***************************************************************************/
class CTrunkDlg : public CDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CTrunkDlg)
	enum { IDD = IDD_TRUNK_DIALOG };
	CConnection* m_pConn;
	CEdit	m_edtPort;
	CEdit	m_edtNumberNI;
	CEdit	m_edtNameNI;
	CIPAddressCtrl	m_ctlIPAddress;
	CButton	m_btnStartStop;
	CComboBox	m_cbNumber;
	BOOL    m_fRunning;
	UINT	m_nPort;
	CString m_strIPAddress;
	CString	m_strNumberNI;
	CString	m_strNumber;
	CString	m_strNameNI;
	CString m_strCallID;
	//}}AFX_DATA

// Construction
public:
	CTrunkDlg(CWnd* pParent=NULL);
	virtual ~CTrunkDlg();

	void Start();
	void Stop();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrunkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTrunkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGenerateTraffic();
	afx_msg void OnClose();
	afx_msg void OnDialhang();
	afx_msg void OnChangeNumber();
	afx_msg LRESULT OnGetData(WPARAM, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __TRUNKDLG_INC__
