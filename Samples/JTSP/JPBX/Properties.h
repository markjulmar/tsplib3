/*******************************************************************/
//
// PROPERTIES.H
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_PROPERTIES_INC__
#define __JPBX_PROPERTIES_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "base.h"

/**************************************************************************
** CPbxPropertiesDlg
**
** This dialog shows our global PBX properties
**
***************************************************************************/
class CPbxPropertiesDlg : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CPbxPropertiesDlg)
	enum { IDD = IDD_PROPERTIES };
	int	m_nPort;
	BOOL	m_fLog;
	CString	m_strFileName;
	//}}AFX_DATA

// Construction
public:
	CPbxPropertiesDlg(CWnd* pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPbxPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPbxPropertiesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __JPBX_PROPERTIES_INC__
