/****************************************************************************/
//
// WINPLACE.CPP
//
// Window placement class
//
// Copyright (C) 1996, Mark C. Smith
// All rights reserved.
//
// This product is for internal use only - do NOT 
// distribute the source code.
//
// Change History
// ------------------------------------------------------------
// 02/10/96	Mark Smith (MCS)	Initial revision.
//
/****************************************************************************/

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "winplace.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// CWindowPlacement::CWindowPlacement
//
// Constructor for the window placement code
//
CWindowPlacement::CWindowPlacement()
{
	length = sizeof(WINDOWPLACEMENT);

}// CWindowPlacement::CWindowPlacement

//////////////////////////////////////////////////////////////////////////////
// CWindowPlacement::~CWindowPlacement
//
// Destructor for the window placement code
//
CWindowPlacement::~CWindowPlacement()
{
	/* Do nothing */

}// CWindowPlacement::~CWindowPlacement

//////////////////////////////////////////////////////////////////////////////
// CWindowPlacement::ReadWindowPlacement
//
// Retrieve the window placement into our structure.
//
bool CWindowPlacement::ReadWindowPlacement(LPCSTR lpKey)
{
	CString strPosition = AfxGetApp()->GetProfileString(lpKey, _T("WindowState"));
	if (strPosition.IsEmpty())
		return false;

	sscanf(strPosition, _T("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"),
					&showCmd, &flags, &ptMinPosition.x, &ptMinPosition.y,
					&ptMaxPosition.x, &ptMaxPosition.y,
					&rcNormalPosition.left, &rcNormalPosition.right,
					&rcNormalPosition.top, &rcNormalPosition.bottom);
	return true;

}// CWindowPlacement::ReadWindowPlacement

//////////////////////////////////////////////////////////////////////////////
// CWindowPlacement::WriteWindowPlacement
//
// Set the structure back to our persistant storage.
//
void CWindowPlacement::WriteWindowPlacement (LPCSTR lpKey)
{
	CString strPosition;
	strPosition.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"),
					showCmd, flags, ptMinPosition.x, ptMinPosition.y,
					ptMaxPosition.x, ptMaxPosition.y,
					rcNormalPosition.left, rcNormalPosition.right,
					rcNormalPosition.top, rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(lpKey, _T("WindowState"), strPosition);

}// CWindowPlacement::WriteWindowPlacement

//////////////////////////////////////////////////////////////////////////////
// CWindowPlacement::SavePosition
//
// Save our position in a specified location
//
void CWindowPlacement::SavePosition (CWnd* pwnd, LPCSTR lpszKey)
{
	ASSERT (pwnd && ::IsWindow (pwnd->GetSafeHwnd()));
	pwnd->GetWindowPlacement (this);
	if (lpszKey != NULL)
		WriteWindowPlacement (lpszKey);

}// CWindowPlacement::SavePosition

//////////////////////////////////////////////////////////////////////////////
// CWindowPlacement::RestorePosition
//
// Restore the position of a window based on .INI information
//
bool CWindowPlacement::RestorePosition (CWnd* pwnd, LPCSTR lpszKey)
{
	// Read it from the .INI locations
	if (lpszKey != NULL && ReadWindowPlacement (lpszKey))
	{
		CRect rcWindow, rcScreen (0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		if (rcWindow.IntersectRect(&rcScreen, &rcNormalPosition))
		{
			ASSERT (pwnd && ::IsWindow (pwnd->GetSafeHwnd()));
			pwnd->SetWindowPlacement (this);
			return true;
		}
	}

	// Default behavior is to center window
	pwnd->CenterWindow();
	return false;

}// CWindowPlacement::RestorePosition

//////////////////////////////////////////////////////////////////////////////
// operator<<
//
// Archive operator from our object
//
CArchive& operator<< (CArchive& ar, const CWindowPlacement& wp)
{
	ar << (LONG) wp.length;
	ar << (LONG) wp.flags;
	ar << (LONG) wp.showCmd;
	ar << wp.ptMinPosition;
	ar << wp.ptMaxPosition;
	ar << wp.rcNormalPosition;
	return ar;

}// operator<<

//////////////////////////////////////////////////////////////////////////////
// operator>>
//
// Archive operator into our object
//
CArchive& operator>> (CArchive& ar, CWindowPlacement& wp)
{
	ar >> (LONG&) wp.length;
	ar >> (LONG&) wp.flags;
	ar >> (LONG&) wp.showCmd;
	ar >> wp.ptMinPosition;
	ar >> wp.ptMaxPosition;
	ar >> wp.rcNormalPosition;
	return ar;

}// operator>>
