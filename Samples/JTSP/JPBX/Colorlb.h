/*******************************************************************/
//
// COLORLB.H
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_COLORLB_INC__
#define __JPBX_COLORLB_INC__

/**************************************************************************
** CColorLB
**
** Color-coded listbox control
**
***************************************************************************/
class CColorLB : public CListBox
{
// Overrides
protected:
   //{{AFX_MSG(CColorLB)
   virtual void DrawItem(LPDRAWITEMSTRUCT lpdi);
   virtual void MeasureItem(LPMEASUREITEMSTRUCT lpmi);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#endif // __JPBX_COLORLB_INC__