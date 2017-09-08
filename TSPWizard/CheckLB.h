/*******************************************************************/
//
// CHECKLB.H
//
// Checkbox listbox
//
// Copyright (C) 1998 JulMar Technology, Inc.
// All rights reserved
//
/*******************************************************************/

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __CHECKLB_INC__
#define __CHECKLB_INC__

/**************************************************************************
** CCheckListBox2
**
** Checkbox listbox control
**
***************************************************************************/
class CCheckListBox2 : public CCheckListBox
{
// Overrides
protected:
   //{{AFX_MSG(CCheckListBox2)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#endif // __CHECKLB_INC__