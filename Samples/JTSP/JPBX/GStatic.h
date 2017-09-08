/*******************************************************************/
//
// GSTATIC.H
//
// Gradient static control
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

#ifndef __GSTATIC_INC__
#define __GSTATIC_INC__

/**************************************************************************
** CGStatic
**
** Gradient static control
**
***************************************************************************/
class CGStatic : public CStatic
{
// Class data
protected:
	COLORREF m_clrBack;		// Background color
	COLORREF m_clrText;		// Text color

// Constructor
public:
	CGStatic();
	virtual ~CGStatic();

// Methods
public:
	void SetTextColor(COLORREF clrText);
	void SetBackColor(COLORREF clrBack);

protected:
	//{{AFX_VIRTUAL(CGStatic)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGStatic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CGStatic::SetTextColor
**
** Arguments: 'clrText' - Text color
**
** Returns: void
**
** Description: This sets the text color for the control
**
*****************************************************************************/
inline void CGStatic::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	Invalidate();

}// CGStatic::SetTextColor

/*****************************************************************************
** Procedure:  CGStatic::SetBackColor
**
** Arguments: 'clrBack' - Background color
**
** Returns: void
**
** Description: This sets the background color for the control
**
*****************************************************************************/
inline void CGStatic::SetBackColor(COLORREF clrBack)
{
	m_clrBack = clrBack;
	Invalidate();

}// CGStatic::SetBackColor

#endif // __GSTATIC_INC__
