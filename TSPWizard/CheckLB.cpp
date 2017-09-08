/*******************************************************************/
//
// CHECKLB.CPP
//
// Checkbox listbox
//
// Copyright (C) 1998 JulMar Technology, Inc.
// All rights reserved
//
/*******************************************************************/

#include "stdafx.h"
#include "checklb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CCheckListBox2 Message map

BEGIN_MESSAGE_MAP(CCheckListBox2, CCheckListBox)
   //{{AFX_MSG_MAP(CCheckListBox2)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CCheckListBox2::MeasureItem
//
// This method processes the WM_MEASUREITEM message for our owner-drawn
// listbox.
//
void CCheckListBox2::MeasureItem(LPMEASUREITEMSTRUCT lpmi)
{
	CCheckListBox::MeasureItem(lpmi);

}// CCheckListBox2::MeasureItem

////////////////////////////////////////////////////////////////////////////
// CCheckListBox2::DrawItem
//
// This method processes the WM_DRAWITEM message for our owner-drawn
// listbox
//
void CCheckListBox2::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CCheckListBox::DrawItem(lpDrawItemStruct);
	
}// CCheckListBox2::DrawItem

