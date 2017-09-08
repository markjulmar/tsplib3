// DbgLevel.h : main header file for the DBGLEVEL application
//

#if !defined(AFX_DBGLEVEL_H__3021FB59_C418_11D1_BBA2_006097D5EC19__INCLUDED_)
#define AFX_DBGLEVEL_H__3021FB59_C418_11D1_BBA2_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDbgLevelApp:
// See DbgLevel.cpp for the implementation of this class
//

class CDbgLevelApp : public CWinApp
{
public:
	CDbgLevelApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbgLevelApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDbgLevelApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBGLEVEL_H__3021FB59_C418_11D1_BBA2_006097D5EC19__INCLUDED_)
