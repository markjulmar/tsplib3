/*******************************************************************/
//
// STATIONDLG.H
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __JPBX_STATION_INC__
#define __JPBX_STATION_INC__

/*---------------------------------------------------------------*/
// PREDINITIONS
/*---------------------------------------------------------------*/
class CConnection;

/**************************************************************************
** CCall
**
** Call object
**
***************************************************************************/
class CCall
{
public:
	DWORD dwCallID;
	CString strNumber;
	bool m_fConnected;
	bool m_fInConference;
	CCall() : dwCallID(0), m_fConnected(false), m_fInConference(false) {}
	void Empty() { dwCallID = 0; strNumber = _T(""); m_fConnected = false; m_fInConference = false; }
	bool InUse() { return (dwCallID != 0); }
};

/**************************************************************************
** CStationDlg
**
** Dialog for the station
**
***************************************************************************/
class CStationDlg : public CDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CStationDlg)
	enum { IDD = IDD_STATION_DIALOG };
	CButton	m_btnSignOn;
	CConnection* m_pConn;
	CStatic	m_ctlDisplay;
	BOOL	m_fReady;
	BOOL	m_fCallWork;
	BOOL    m_fConnected;
	BOOL    m_fPlacingCall;
	CString	m_strDisplay;
	CString m_strLastDisplay;
	CString m_strDialedNumber;
	HICON m_hIcon;
	CFont m_fntDisplay;
	DWORD m_dwExtension;
	CString m_strAgentID;
	DWORD m_dwStatus;
	int   m_iConferenceCount;
	//}}AFX_DATA
	CCall m_arrCalls[3];
	CButton	m_btnLine[3];

// Construction
public:
	CStationDlg(CWnd* pParent = NULL);
	virtual ~CStationDlg();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Methods
protected:
	void Error(int nError);
	void OnAgentStateChange(char cState);
	void OnDisplayChange(CString& strDisplay);
	void OnLampChange(int iLampID, CString& strState);
	void OnCallDetected(DWORD dwCallID, CString& strANI, CString& strDNIS);
	void OnCallPlaced(DWORD dwCallID, CString& strANI, CString& strDNIS);
	void OnCallRelease(DWORD dwCallID);
	void OnCallStateChange(DWORD dwCallID, CString& strState);
	void OnCallTransferred(DWORD dwExtension, DWORD dwCallID1, DWORD dwCallID2, DWORD dwTarget, CString& strANI, CString& strDNIS);
	void OnConferenceCreated(DWORD dwExtension, DWORD dwCallID1, DWORD dwCallID2, DWORD dwCallID3);
	int GetActiveLine() const;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConference();
	afx_msg void OnHold();
	afx_msg void OnLine();
	afx_msg void OnReady();
	afx_msg void OnRelease();
	afx_msg void OnSignon();
	afx_msg void OnTransfer();
	afx_msg void OnDigit();
	afx_msg void OnEnter();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnGetData(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnCloseConn(WPARAM, LPARAM);
	afx_msg void OnCallWork();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CStationDlg::GetActiveLine
**
** Arguments: void
**
** Returns:    index of active line
**
** Description: This returns the active line (pressed)
**
*****************************************************************************/
inline int CStationDlg::GetActiveLine() const
{
	bool fFound = false;
	for (int i = 0; i < 3; i++)
	{
		if ((m_btnLine[i].GetState() & 0x0003) == 1)
		{
			fFound = true;
			break;
		}
	}
	if (fFound)
		return i;
	return -1;

}// CStationDlg::GetActiveLine

#endif // __JPBX_STATION_INC__
