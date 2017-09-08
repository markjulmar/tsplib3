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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_GENTRAFFIC_INC__
#define __JPBX_GENTRAFFIC_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "base.h"

/**************************************************************************
** CGenTraffic
**
** Dialog which controls the soak/load test
**
***************************************************************************/
class CGenTraffic : public CBaseDialog
{
// Dialog Data
public:
	//{{AFX_DATA(CGenTraffic)
	enum { IDD = IDD_GENTRAFFIC };
	CEdit	m_edtCPS;
	CButton	m_btnOK;
	CSpinButtonCtrl	m_ctlSpinner;
	int	m_nCPS;
	bool m_fRunning;
	//}}AFX_DATA

// Construction
public:
	CGenTraffic(CWnd* pParent);   // standard constructor

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenTraffic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGenTraffic)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __JPBX_GENTRAFFIC_INC__
