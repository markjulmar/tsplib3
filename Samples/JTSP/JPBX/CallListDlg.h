/*******************************************************************/
//
// CALLLISTDLG.H
//
// Dialog which displays the active call list for this line.
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

#ifndef __JPBX_CALLLIST_INC__
#define __JPBX_CALLLIST_INC__

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "base.h"

/*---------------------------------------------------------------*/
// PRE-DEFINITIONS
/*---------------------------------------------------------------*/
class CLineDevice;
class CCall;

/**************************************************************************
** CCallListDlg
**
** This dialog shows the call list for a line
**
***************************************************************************/
class CCallListDlg : public CBaseDialog
{
// Dialog Data
protected:
	//{{AFX_DATA(CCallListDlg)
	enum { IDD = IDD_CALLLIST };
	CListCtrl m_lcCalls;
	CLineDevice* m_pLine;
	//}}AFX_DATA

// Construction
public:
	CCallListDlg(CLineDevice* pLineOwner);
	void Create(CWnd* pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Event methods called by CLineDialog owner
public:
	void OnCallCreated(CCall* pCall);
	void OnCallDestroyed(CCall* pCall);
	void OnCallStateChange(CCall* pCall);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Internal methods
protected:
	void InsertCall(CCall* pCall);

};

#endif // __JPBX_CALLLIST_INC__
