/*******************************************************************/
//
// BASE.CPP
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "JPBX.h"
#include "document.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// RTTI INFORMATION
/*---------------------------------------------------------------*/
IMPLEMENT_SERIAL(CUIObject, CObject, 1)

/*---------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------*/
CUIObject* CUIObject::g_pSelected = NULL;

/*****************************************************************************
** Procedure:  CUIObject::CUIObject
**
** Arguments: void
**
** Returns:    void
**
** Description:  Default no-parameter constructor for UI object
**
*****************************************************************************/
CUIObject::CUIObject()
{
}// CUIObject::CUIObject

/*****************************************************************************
** Procedure:  CUIObject::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to main document object for program
**
** Description:  This function searches the frame window and locates the
**               main document object
**
*****************************************************************************/
CJTDocument* CUIObject::GetDocument()
{
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
	{
		POSITION pos = pApp->GetFirstDocTemplatePosition();
		if (pos != NULL)
		{
			CDocTemplate* ptDoc = pApp->GetNextDocTemplate(pos);
			ASSERT(pos == NULL);
			if (ptDoc != NULL)
			{
				pos = ptDoc->GetFirstDocPosition();
				if (pos != NULL)
					return dynamic_cast<CJTDocument*>(ptDoc->GetNextDoc(pos));
			}
		}
	}
	return NULL;

}// CUIObject::GetDocument

/*****************************************************************************
** Procedure:  CUIObject::RunPopup
**
** Arguments: 'nMenuID' - Popup menu to show
**            'pwndOwner' - Owner for popup menu
**            'pDoc' - Document to use
**
** Returns:    true/false if popup was shown
**
** Description:  This function creates a context pop-up menu
**
*****************************************************************************/
bool CUIObject::RunPopup(UINT nMenuID, CWnd* pwndOwner)
{
	CRect rcWnd;
	pwndOwner->GetWindowRect(&rcWnd);
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	UINT nFlags = TPM_RIGHTBUTTON;
	if ((rcWnd.right - ptCursor.x) <= 20)
		nFlags |= TPM_RIGHTALIGN;
	else
		nFlags |= TPM_LEFTALIGN;

	// Mark this object for WM_COMMAND route.
	Select();

	// Show the menu
	CMenu cMenu;
	if (cMenu.LoadMenu(nMenuID))
	{
		CMenu* pMenu = cMenu.GetSubMenu(0);
	    ::SetMenuDefaultItem(pMenu->m_hMenu, IDM_CTX_PROPERTIES, FALSE);
		return (pMenu->TrackPopupMenu(nFlags, ptCursor.x, ptCursor.y,
					pwndOwner, NULL) != 0);
	}
	return false;

}// CUIObject::RunPopup

/*****************************************************************************
** Procedure:  CBaseDialog::CBaseDialog
**
** Arguments: 'pParent' - Parent window handle
**            'pDoc' - CJTDocument for displaying group information
**
** Returns:    void
**
** Description:  Constructor for the agent dialog
**
*****************************************************************************/
CBaseDialog::CBaseDialog(UINT nTemplateID, CWnd* pParent)
	: CDialog(nTemplateID, pParent)
{
	//{{AFX_DATA_INIT(CBaseDialog)
	//}}AFX_DATA_INIT

}// CBaseDialog::CBaseDialog

/*****************************************************************************
** Procedure:  CBaseDialog::DoDataExchange
**
** Arguments: 'pDX' - Dialog data exchange pointer
**
** Returns:    void
**
** Description: Called by the framework to exchange and validate dialog 
**              data.  This connects windows controls up to class elements
**              in the C++ object.
**
*****************************************************************************/
void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseDialog)
	//}}AFX_DATA_MAP

}// CBaseDialog::DoDataExchange

/*---------------------------------------------------------------*/
// CBaseDialog message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CBaseDialog, CDialog)
	//{{AFX_MSG_MAP(CBaseDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CBaseDialog::OnInitDialog
**
** Arguments: void
**
** Returns:    TRUE/FALSE if Windows should set focus to the first control.
**
** Description: This member function is called in response to the 
**              WM_INITDIALOG message. This message is sent to the dialog box 
**              during the Create, CreateIndirect, or DoModal calls, which occur 
**              immediately before the dialog box is displayed. 
**
*****************************************************************************/
BOOL CBaseDialog::OnInitDialog() 
{
	// Call the base initialization
	CDialog::OnInitDialog();

	// Change our font for all controls.
	CFont fntAnsi;
	fntAnsi.CreateStockObject(ANSI_VAR_FONT);
	CWnd* pwnd = GetWindow(GW_CHILD);
	while (pwnd != NULL && IsChild(pwnd))
	{
		pwnd->SetFont(&fntAnsi);
		pwnd = pwnd->GetWindow(GW_HWNDNEXT);
	}

	// Center our window on the screen.
	CenterWindow();

	return TRUE;

}// CBaseDialog::OnInitDialog

