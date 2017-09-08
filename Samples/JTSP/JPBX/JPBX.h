/*******************************************************************/
//
// JPBX.H
//
// Main PBX project header file
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

#ifndef __JPBX_INC__
#define __JPBX_INC__

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "resource.h"       // main symbols
#include "base.h"			// basic object definitions

// Lengths
#define DIALABLENUM_SIZE  25
#define ADDROWNER_SIZE    65
#define AGENTACT_SIZE     255

/**************************************************************************
** CJPBXApp
**
** Main application object which drives the execution of the program
**
***************************************************************************/
class CJPBXApp : public CWinApp
{
// Constructor
public:
	CJPBXApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJPBXApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CJPBXApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __JPBX_INC__
