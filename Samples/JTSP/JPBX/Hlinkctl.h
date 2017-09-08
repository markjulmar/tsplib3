/*******************************************************************/
//
// HLINKCTL.H
//
// Hyper-link static control
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

#ifndef __HLINKCTRL_INC__
#define __HLINKCTRL_INC__

/**************************************************************************
** CHLinkCtrl
**
** Hyper link control class which overrides a static control
**
***************************************************************************/
class CHLinkCtrl : public CStatic
{
// Class data
protected:
	enum HState {
		Initial,
		Highlighted,
		Visited
	};

	HCURSOR  m_hcursLink;		// Hand cursor
	COLORREF m_clrNormal;		// Normal color
	COLORREF m_clrVisited;		// Visited color
	COLORREF m_clrHighlight;	// Highlight color
	HState m_stCurrent;			// Current state
	HState m_stLast;			// Previous state
	CFont m_fntLink;			// Underline font

public:
	//Constructors / Destructors
	CHLinkCtrl();
	virtual ~CHLinkCtrl();

protected:
	//{{AFX_VIRTUAL(CHLinkCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHLinkCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __HLINKCTRL_INC__