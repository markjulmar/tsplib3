/*******************************************************************/
//
// COLORLB.CPP
//
// Color-coded listbox
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

#include "stdafx.h"
#include "colorlb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CColorLB Message map

BEGIN_MESSAGE_MAP(CColorLB, CListBox)
   //{{AFX_MSG_MAP(CColorLB)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CColorLB::MeasureItem
//
// This method processes the WM_MEASUREITEM message for our owner-drawn
// listbox.
//
void CColorLB::MeasureItem(LPMEASUREITEMSTRUCT lpmi)
{
	static int iItemHeight = 0;
	if (iItemHeight == 0)
	{
		TEXTMETRIC tm;
		CDC* pDC = GetDC();
		pDC->GetTextMetrics(&tm);
		iItemHeight = tm.tmHeight + tm.tmDescent;
		ReleaseDC(pDC);
	}
	lpmi->itemHeight = iItemHeight;

}// CColorLB::MeasureItem

////////////////////////////////////////////////////////////////////////////
// CColorLB::DrawItem
//
// This method processes the WM_DRAWITEM message for our owner-drawn
// listbox
//
void CColorLB::DrawItem(LPDRAWITEMSTRUCT lpdi)
{
	ASSERT(lpdi->CtlType == ODT_LISTBOX);
	CDC* pDC = CDC::FromHandle(lpdi->hDC);

	// If we are drawing the entire listbox entry -OR- changing
	// selection state for the item, then redraw the entry completely.
	if (lpdi->itemAction & (ODA_DRAWENTIRE | ODA_SELECT ))
	{
		COLORREF clrBack(RGB(192,192,192)), clrFore;
		CString strText;
		GetText(lpdi->itemID, strText);

		switch (strText[0])
		{
			case '0':  clrFore = RGB(255,0,0); break; // Rcvd = Red
			case '1':  clrFore = RGB(0,0,255); break; // Sent = Blue
			case '2':  clrFore = RGB(0,128,0); break; // Command = Green
			default:   clrFore = RGB(0,0,0); break;
		}
		COLORREF clrOldFore = pDC->SetTextColor(clrFore);

		// Remove our key character.
		strText = strText.Mid(1);

		// Fill in the entry with the background color
		CBrush Brush(clrBack);
		pDC->FillRect(&lpdi->rcItem, &Brush);

		// Draw our text
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, strText.GetLength(),
			(LPRECT)(const RECT FAR *)&lpdi->rcItem, 
			(DT_SINGLELINE | DT_LEFT | DT_VCENTER));

		// Restore the DC
		pDC->SetTextColor(clrOldFore);
		pDC->SetBkMode(nOldBkMode);
	}                              
   
}// CColorLB::DrawItem

