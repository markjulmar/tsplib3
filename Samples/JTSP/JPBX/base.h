/*******************************************************************/
//
// BASE.H
//
// Basic object definitions
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

#ifndef __JPBX_BASE_INC__
#define __JPBX_BASE_INC__

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CJTDocument;

/*----------------------------------------------------------------------------
	INCLUDES
-----------------------------------------------------------------------------*/
#include "Interface.h"

/*----------------------------------------------------------------------------
	STRUCTURES
-----------------------------------------------------------------------------*/

// LISTVIEW COLUMN STRUCTURE
// This structure defines the column information which is setup for
// a listview control on our main view.
typedef struct
{
	UINT uiName;	// Name of the field (text)
	int iWidth;		// Size of the column in characters
} LCCOLUMNS;

/**************************************************************************
** CUIObject
**
** Basic user-interface object for our PBX interface.
** This object supports serialization, context menus, and dialog creation.
**
***************************************************************************/
class CUIObject : public CObject
{
// Class data
protected:
	static CUIObject* g_pSelected;

// Default Constructor
public:
	CUIObject();
	DECLARE_SERIAL(CUIObject)

// Overridable Methods
public:
	static CUIObject* GetSelectedObject();
	bool RunPopup(UINT nMenuID, CWnd* pwndOwner);
	virtual bool Edit(CWnd* pwndOwner);
	virtual bool ContextMenu(CWnd* pwnd);
	virtual void OnCommand(UINT nCommand);
	void Select();
	static CJTDocument* GetDocument();
};

/**************************************************************************
** CBaseDialog
**
** Basic dialog object which has support required for all popup dialogs
** used in this program.
**
***************************************************************************/
class CBaseDialog : public CDialog
{
// Construction
public:
	CBaseDialog( UINT nTemplate, CWnd* pParent );
	CBaseDialog() : CDialog() {}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBaseDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CUIObject::Edit
**
** Arguments: 'pwnd' - Owner for properties dialog
**
** Returns:    true/false if properties were changed for this object
**
** Description:  This function shows an object-specific dialog for changing
**               the properties.
**
*****************************************************************************/
inline bool CUIObject::Edit(CWnd*)
{
	return false;

}// CUIObject::Edit

/*****************************************************************************
** Procedure:  CUIObject::ContextMenu
**
** Arguments: 'pwnd' - Owner for pop-up menu
**
** Returns:    true/false if the popup menu was shown
**
** Description:  This function is called to show a popup menu for an object
**
*****************************************************************************/
inline bool CUIObject::ContextMenu(CWnd*)
{
	return false;

}// CUIObject::ContextMenu

/*****************************************************************************
** Procedure:  CUIObject::OnCommand
**
** Arguments: 'nCommand' - WM_COMMAND id
**
** Returns:    void
**
** Description:  This function is called when a WM_COMMAND event is intercepted
**               for a specific UI object
**
*****************************************************************************/
inline void CUIObject::OnCommand(UINT)
{
}// CUIObject::OnCommand

/*****************************************************************************
** Procedure:  CUIObject::GetSelectedObject
**
** Arguments: void
**
** Returns:    Pointer to the selected object
**
** Description:  This function returns the last selected object
**
*****************************************************************************/
inline CUIObject* CUIObject::GetSelectedObject()
{
	return g_pSelected;

}// CUIObject::OnCommand

/*****************************************************************************
** Procedure:  CUIObject::Select
**
** Arguments: void
**
** Returns:    void
**
** Description:  Marks the current object as "selected"
**
*****************************************************************************/
inline void CUIObject::Select()
{
	g_pSelected = this;

}// CUIObject::Select

#endif // __JPBX_BASE_INC__