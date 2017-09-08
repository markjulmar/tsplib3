/*******************************************************************/
//
// MAINFRM.H
//
// CWinFrame header file
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

#ifndef __JPBX_MAINFRM_INC__
#define __JPBX_MAINFRM_INC__

/**************************************************************************
** CMainFrame
**
** Main application frame window
**
***************************************************************************/
class CMainFrame : public CFrameWnd
{
// Class data
public:
	enum ViewType {
		DetailView,
		GlobalStatsView
	};
protected:
	CStatusBar m_wndStatusBar;
	CToolBar m_wndToolBar;
	CSplitterWnd m_wndSplitter;

protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)

// Operations
public:
	void SwitchToView(enum ViewType vt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg LRESULT OnProcessData(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateStats(WPARAM, LPARAM);
	afx_msg LRESULT OnSoakTest(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

#endif // __JPBX_MAINFRM_INC__
