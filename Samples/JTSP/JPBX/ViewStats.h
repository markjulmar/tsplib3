/*******************************************************************/
//
// VIEWSTATS.H
//
// Right-pane view for the JPBX eulator
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "gstatic.h"
#include "colorlb.h"

#ifndef __JPBX_JSVIEW_INC__
#define __JPBX_JSVIEW_INC__

/**************************************************************************
** CJSView
**
** Form view class which is used as the right side of our PBX app when
** no specific header is chosen (world).
**
***************************************************************************/
class CJSView : public CFormView
{
// Class data
protected:
	//{{AFX_DATA(CJSView)
	enum { IDD = IDD_STATISTICS };
	CGStatic m_ctlHeader1;
	CGStatic m_ctlStatBar;
	CColorLB m_lbDebugOut;
	bool m_fRunningTest;
	//}}AFX_DATA

protected: // create from serialization only
	CJSView();
	virtual ~CJSView();
	DECLARE_DYNCREATE(CJSView)

// Attributes
public:
	CJTDocument* GetDocument();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJSView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CJSView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CJSView::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to our document object
**
** Description: This override casts our document to the appropriate type
**
*****************************************************************************/
inline CJTDocument* CJSView::GetDocument()
{ 
	return dynamic_cast<CJTDocument*>(m_pDocument);

}// CJSView::GetDocument


#endif // __JPBX_JSVIEW_INC__
