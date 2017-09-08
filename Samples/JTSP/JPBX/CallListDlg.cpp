/*******************************************************************/
//
// CALLLISTDLG.H
//
// Dialog which displays the active call list for this line.
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
#include "jpbx.h"
#include "CallListDlg.h"
#include "line.h"
#include "call.h"
#include "winplace.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*------------------------------------------------------------------------------
    GLOBAL DATA
--------------------------------------------------------------------------------*/
static LCCOLUMNS g_Columns[] = {
	{ IDS_LV_CALLID, 9 },
	{ IDS_LV_STATE, 15 },
	{ IDS_LV_ANI, 15 },
	{ IDS_LV_DNIS, 15 },
	{ 0, 0 },
};

/*****************************************************************************
** Procedure:  CCallListDlg::CCallListDlg
**
** Arguments: 'pLine' - Line to gather call information from
**
** Returns:    void
**
** Description:  Constructor for the call list dialog
**
*****************************************************************************/
CCallListDlg::CCallListDlg(CLineDevice* pLine)
	: CBaseDialog()
{
	//{{AFX_DATA_INIT(CCallListDlg)
	m_pLine = pLine;
	//}}AFX_DATA_INIT

}// CCallListDlg::CCallListDlg

/*****************************************************************************
** Procedure:  CCallListDlg::Create
**
** Arguments: 'pWnd' - Window owner
**
** Returns:    void
**
** Description:  Creates a modaless dialog
**
*****************************************************************************/
void CCallListDlg::Create(CWnd* pParent)
{
	CBaseDialog::Create(IDD, pParent);

}// CCallListDlg::Create

/*****************************************************************************
** Procedure:  CCallListDlg::DoDataExchange
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
void CCallListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallListDlg)
	DDX_Control(pDX, IDC_CALLS, m_lcCalls);
	//}}AFX_DATA_MAP

}// CCallListDlg::DoDataExchange

/*---------------------------------------------------------------*/
// CCallListDlg
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCallListDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CCallListDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_CALLS, OnGetDispInfo)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CCallListDlg::OnInitDialog
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
BOOL CCallListDlg::OnInitDialog() 
{
	// Connect up our dialog controls
	CBaseDialog::OnInitDialog();

	// Set our dialog title.
	CString strTitle, strKey;
	GetWindowText(strTitle);
	strKey.Format(_T("%s [%04d] %s"), strTitle, m_pLine->GetLineID(), m_pLine->GetLineName());
	SetWindowText(strKey);

	// Restore the position of our window
	CWindowPlacement wp;
	wp.RestorePosition(this, strKey);

	// Get our text metrics
	CDC* pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	ReleaseDC(pDC);
	
	// Load our listview columns
	for (int i = 0; g_Columns[i].uiName != 0; i++)
	{
		CString strName;
		VERIFY(strName.LoadString(g_Columns[i].uiName));
		m_lcCalls.InsertColumn (i, strName, LVCFMT_LEFT, 
			tm.tmAveCharWidth*(g_Columns[i].iWidth+1));
	}

	// Load our call information
	for (i = 0; i < m_pLine->GetCallCount(); i++)
	{
		CCall* pCall = m_pLine->GetCallByPosition(i);
		InsertCall(pCall);
	}

	return TRUE;

}// CCallListDlg::OnInitDialog

/*****************************************************************************
** Procedure:  CCallListDlg::InsertCall
**
** Arguments: 'pCall' - Call to add to our listview
**
** Returns:    void
**
** Description: This is used to insert a call into our listview
**
*****************************************************************************/
void CCallListDlg::InsertCall(CCall* pCall)
{
	// Use the following LV_ITEM structure to add all
	// the item information.
	LV_ITEM lvItem;
	lvItem.iItem = m_lcCalls.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.pszText = LPSTR_TEXTCALLBACK;
	lvItem.cchTextMax = 0;
	lvItem.iSubItem = 0;
	lvItem.lParam = reinterpret_cast<LPARAM>(pCall);

	VERIFY (m_lcCalls.InsertItem (&lvItem) == lvItem.iItem);

}// CCallListDlg::InsertCall

/*****************************************************************************
** Procedure:  CCallListDlg::OnGetDispInfo
**
** Arguments: 'pNMHDR' - ListView header
**            'pResult' - Returning result code
**
** Returns:    void
**
** Description: This event is called when the text for an item needs to
**              be displayed in our listview.
**
*****************************************************************************/
void CCallListDlg::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &pDispInfo->item;
	CCall* pCall = reinterpret_cast<CCall*>(pItem->lParam);

	static char szBuffer[50];
	ZeroMemory(szBuffer, sizeof(szBuffer));

	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
			case 0:		// Call id
				wsprintf(szBuffer, _T("%08X"), pCall->GetCallID());
				break;
			case 1:		// State
				lstrcpy(szBuffer, pCall->GetCallStateString());
				break;
			case 2:		// ANI
				lstrcpy(szBuffer, pCall->GetANI());
				break;
			case 3:		// DNIS
				lstrcpy(szBuffer, pCall->GetDNIS());
				break;
		}
	}

	pItem->pszText = szBuffer;
	*pResult = 0;

}// CCallListDlg::OnGetDispInfo

/*****************************************************************************
** Procedure:  CCallListDlg::OnSize
**
** Arguments: 'nType' - Type of size (Maximize, minimize, etc.)
**            'cx' - New Width
**            'cy' - New Height
**
** Returns:    void
**
** Description: This event is called when our window is resized.
**
*****************************************************************************/
void CCallListDlg::OnSize(UINT nType, int cx, int cy) 
{
	// Let the dialog do things
	CDialog::OnSize(nType, cx, cy);

	// Now resize our control list to our window
	if (m_lcCalls.GetSafeHwnd() != NULL)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		m_lcCalls.SetWindowPos(&CWnd::wndTop, 1, 1, rcClient.Width()-1, rcClient.Height()-1, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

}// CCallListDlg::OnSize

/*****************************************************************************
** Procedure:  CCallListDlg::OnCallCreated
**
** Arguments: 'pCall' - CCall object
**
** Returns:    void
**
** Description: This event is called by our line owner when
**              a call is created
**
*****************************************************************************/
void CCallListDlg::OnCallCreated(CCall* pCall)
{
	InsertCall(pCall);

}// CCallListDlg::OnCallCreated

/*****************************************************************************
** Procedure:  CCallListDlg::OnCallDestroyed
**
** Arguments: 'pCall' - CCall object
**
** Returns:    void
**
** Description: This event is called by our line owner when
**              a call is destroyed
**
*****************************************************************************/
void CCallListDlg::OnCallDestroyed(CCall* pCall)
{
	for (int i = 0; i < m_lcCalls.GetItemCount(); i++)
	{
		if (reinterpret_cast<CCall*>(m_lcCalls.GetItemData(i)) == pCall)
		{
			m_lcCalls.DeleteItem(i);
			break;
		}
	}

}// CCallListDlg::OnCallDestroyed

/*****************************************************************************
** Procedure:  CCallListDlg::OnCallStateChange
**
** Arguments: 'pCall' - CCall object
**
** Returns:    void
**
** Description: This event is called by our line owner when
**              a call changes state
**
*****************************************************************************/
void CCallListDlg::OnCallStateChange(CCall* pCall)
{
	for (int i = 0; i < m_lcCalls.GetItemCount(); i++)
	{
		if (reinterpret_cast<CCall*>(m_lcCalls.GetItemData(i)) == pCall)
		{
			m_lcCalls.RedrawItems(i, i);
			break;
		}
	}

}// CCallListDlg::OnCallStateChange

/*****************************************************************************
** Procedure:  CCallListDlg::OnDestroy
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the window is being destroyed.
**              It records our final position and size into the registry
**
*****************************************************************************/
void CCallListDlg::OnDestroy() 
{
	CString strTitle;
	GetWindowText(strTitle);
	CWindowPlacement wp;
	wp.SavePosition(this, strTitle);
	CBaseDialog::OnDestroy();

}// CCallListDlg::OnDestroy

/*****************************************************************************
** Procedure:  CCallListDlg::OnClose
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the window is being closed
**              It forces the window to destroy itself.
**
*****************************************************************************/
void CCallListDlg::OnClose() 
{
	DestroyWindow();

}// CCallListDlg::OnClose
