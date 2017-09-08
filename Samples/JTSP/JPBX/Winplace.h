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

#ifndef __WINPLACE_INC__
#define __WINPLACE_INC__

class CWindowPlacement : public WINDOWPLACEMENT
{
// Constructor
public:
	CWindowPlacement();
	virtual ~CWindowPlacement();

// Methods
public:
	void SavePosition (CWnd* pwnd, LPCSTR lpszKey=NULL);
	bool RestorePosition (CWnd* pwnd, LPCSTR lpszKey=NULL);

	// Save/restore from an archive
	friend CArchive& operator<<(CArchive& ar, const CWindowPlacement& wp);
	friend CArchive& operator>>(CArchive& ar, CWindowPlacement& wp);

// Internal methods
protected:
	bool ReadWindowPlacement (LPCSTR lpszKey);
	void WriteWindowPlacement (LPCSTR lpszKey);
};

#endif // __WINPLACE_INC__
