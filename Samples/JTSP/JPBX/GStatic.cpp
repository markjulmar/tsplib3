/*******************************************************************/
//
// GSTATIC.CPP
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "gstatic.h"
#include "gradient.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// CGStatic message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CGStatic, CStatic)
	//{{AFX_MSG_MAP(CGStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CGStatic::CGStatic
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the static control
**
*****************************************************************************/
CGStatic::CGStatic()
{
	m_clrText = GetSysColor(COLOR_WINDOWTEXT);
	m_clrBack = GetSysColor(COLOR_BACKGROUND);

}// CGStatic::CGStatic

/*****************************************************************************
** Procedure:  CGStatic::~CGStatic
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the static control
**
*****************************************************************************/
CGStatic::~CGStatic()
{
}// CGStatic::~CGStatic

/*****************************************************************************
** Procedure:  CGStatic::OnEraseBkgnd
**
** Arguments: 'pDC' - Device control to erase
**
** Returns:    TRUE/FALSE if the background is erased
**
** Description:  This event is called when the control background needs
**               to be erased
**
*****************************************************************************/
BOOL CGStatic::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcBox;
	GetClientRect(&rcBox);

	// Paint our gradient in the background
	CGradient::Draw(pDC, rcBox, m_clrBack, 2);

	return TRUE;

}// CGStatic::OnEraseBkgnd

/*****************************************************************************
** Procedure:  CGStatic::OnPaint
**
** Arguments: void
**
** Returns:    void
**
** Description:  This event is called when the control needs to be painted
**
*****************************************************************************/
void CGStatic::OnPaint() 
{
	CPaintDC dc(this);

	CRect rcBox;
	GetClientRect(&rcBox);

	// Draw our text into the box.
	rcBox.InflateRect(-2, 0, -2, 0);

	// Get the options for this control
	DWORD dwStyle = GetStyle();
	UINT nFormat = (DT_VCENTER | DT_SINGLELINE);

	if (dwStyle & SS_LEFT)
		nFormat |= DT_LEFT;
	else if (dwStyle & SS_RIGHT)
		nFormat |= DT_RIGHT;
	else if (dwStyle & SS_CENTER)
		nFormat |= DT_CENTER;

	if (dwStyle & SS_NOPREFIX)
		nFormat |= DT_NOPREFIX;

	dc.SetTextColor(m_clrText);
	dc.SetBkMode(TRANSPARENT);

	CString strText;
	GetWindowText(strText);
	dc.DrawText(strText, strText.GetLength(), rcBox, nFormat);
	
}// CGStatic::OnPaint
