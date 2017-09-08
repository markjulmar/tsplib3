/*******************************************************************/
//
// GRADIENT.CPP
//
// Object to draw a gradient within an area
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

#ifndef __GRADIENT_INC__
#define __GRADIENT_INC__

/**************************************************************************
** CGradient
**
** Color Gradient class
**
***************************************************************************/
namespace CGradient
{
	// Methods
	void Draw(CDC* pDC, CRect& rcBox, COLORREF clrBack, int iDir=0);
};

#endif // __GRADIENT_INC__
