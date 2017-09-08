/*******************************************************************/
//
// ABOUT.H
//
// About box dialog support
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

#ifndef __JPBX_ABOUT_INC__
#define __JPBX_ABOUT_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "hlinkctl.h"

/**************************************************************************
** CAboutDlg
**
** About box dialog class
**
***************************************************************************/
class CAboutDlg : public CDialog
{
// Class data
protected:
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHLinkCtrl	m_ctlWebAddress;
	//}}AFX_DATA

// Constructor
public:
	CAboutDlg();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __JPBX_ABOUT_INC__
