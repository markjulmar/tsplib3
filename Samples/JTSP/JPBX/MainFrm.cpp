/*******************************************************************/
//
// MAINFRM.CPP
//
// CWinFrame implementation file
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
#include "MainFrm.h"
#include "document.h"
#include "viewtree.h"
#include "viewlist.h"
#include "viewstats.h"
#include "winplace.h"

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
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

/*---------------------------------------------------------------*/
// GLOBAL VARIABLES
/*---------------------------------------------------------------*/
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/*---------------------------------------------------------------*/
// CMainFrame message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_PROCDATA, OnProcessData)
	ON_MESSAGE(UM_UPDATESTATS, OnUpdateStats)
	ON_MESSAGE(UM_SOAKTEST, OnSoakTest)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CMainFrame::OnCreate
**
** Arguments: 'lpCreateStruct' - CREATESTRUCT with window parameters
**
** Returns:    Must return 0 to continue the creation of the CWnd object. 
**             If the application returns 1, the window will be destroyed.
**
** Description: The framework calls this member function when an application 
**              requests that the Windows window be created by calling the 
**              Create or CreateEx member function. The CWnd object receives 
**              this call after the window is created but before it becomes 
**              visible. OnCreate is called before the Create or CreateEx 
**              member function returns. 
**
*****************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Pass through to base class
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create our toolbar
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER);
	if (!m_wndToolBar.CreateEx(this, 0, dwStyle) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	return 0;
}

/*****************************************************************************
** Procedure:  CMainFrame::OnCreateClient
**
** Arguments: 'lpCreateStruct' - CREATESTRUCT with window parameters
**            'pContext' - CCreateContext
**
** Returns:    Nonzero if successful; otherwise 0
**
** Description: This is called during the procesing of the OnCreate function
**              to create our client windows.
**
*****************************************************************************/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	// Create our splitter window which will seperate the two
	// views used by our admin program.
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	int cxCurr = AfxGetApp()->GetProfileInt(_T("Position"), _T("Splitter"), 175);

	// Create our basic views
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CJTView), CSize(cxCurr,0), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CJSView), CSize(0,0), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;

}// CMainFrame::OnCreateClient

/*****************************************************************************
** Procedure:  CMainFrame::PreCreateWindow
**
** Arguments: 'cs' - CREATESTRUCT with window parameters
**
** Returns:    TRUE/FALSE to continue window creation
**
** Description: This is called during the procesing of the OnCreate function
**              to create our client windows.
**
*****************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;
	return CFrameWnd::PreCreateWindow(cs);

}// CMainFrame::PreCreateWindow

/*****************************************************************************
** Procedure:  CMainFrame::SwitchToView
**
** Arguments: void
**
** Returns:    void
**
** Description: This function swaps the right side pane out of our 
**              splitter window for a detailed listview.
**
*****************************************************************************/
void CMainFrame::SwitchToView(enum ViewType vt)
{
	CRuntimeClass* prtClass = NULL;
	switch (vt)
	{
		case DetailView:		prtClass = RUNTIME_CLASS(CJDView); break;
		case GlobalStatsView:	prtClass = RUNTIME_CLASS(CJSView); break;
		default: ASSERT(FALSE);
	}

	CCreateContext Context;
	Context.m_pCurrentDoc = (dynamic_cast<CView*>(m_wndSplitter.GetPane(0,0)))->GetDocument();
	Context.m_pCurrentFrame = this;

	m_wndSplitter.DeleteView(0,1);
	m_wndSplitter.CreateView(0, 1, prtClass, CSize(0,0), &Context);
	m_wndSplitter.RecalcLayout();

	CView* pView = (CView*) m_wndSplitter.GetPane(0,1);
	pView->OnInitialUpdate();

}// CMainFrame::SwitchToView

/*****************************************************************************
** Procedure:  CMainFrame::OnUpdateViewStyles
**
** Arguments: 'pCmdUI' - CommandUI update facility
**
** Returns:    void
**
** Description: This function turns on the toolbar buttons if the view may
**              be altered
**
*****************************************************************************/
void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	CJDView* pView = NULL;
	try 
	{
		pView = dynamic_cast<CJDView*>(m_wndSplitter.GetPane(0,1));
	}
	catch (...)
	{
	}

	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		// If the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;
		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// Otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
				case ID_VIEW_DETAILS:	bChecked = (dwStyle == LVS_REPORT); break;
				case ID_VIEW_SMALLICON: bChecked = (dwStyle == LVS_SMALLICON); break;
				case ID_VIEW_LARGEICON: bChecked = (dwStyle == LVS_ICON); break;
				case ID_VIEW_LIST:		bChecked = (dwStyle == LVS_LIST); break;
				default:				bChecked = FALSE; break;
			}
			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}

}// CMainFrame::OnUpdateViewStyles

/*****************************************************************************
** Procedure:  CMainFrame::OnViewStyles
**
** Arguments: 'nCommand' - WM_COMMAND to execute
**
** Returns:    void
**
** Description: This function changes the listview style for our JDView
**
*****************************************************************************/
void CMainFrame::OnViewStyle(UINT nCommandID)
{
	CJDView* pView = NULL;
	try 
	{
		pView = dynamic_cast<CJDView*>(m_wndSplitter.GetPane(0,1));
	}
	catch (...)
	{
	}

	if (pView != NULL)
	{
		DWORD dwStyle = 0xffffffff;
		switch (nCommandID)
		{
			case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

			// other commands change the style on the list control
			case ID_VIEW_DETAILS:
				dwStyle = LVS_REPORT;
				break;

			case ID_VIEW_SMALLICON:
				dwStyle = LVS_SMALLICON;
				break;

			case ID_VIEW_LARGEICON:
				dwStyle = LVS_ICON;
				break;

			case ID_VIEW_LIST:
				dwStyle = LVS_LIST;
				break;
		}

		// Change the style; window will repaint automatically
		if (dwStyle != 0xffffffff)
		{
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
			pView->GetDocument()->SetViewStyle(dwStyle);
		}
	}

}// CMainFrame::OnViewStyle

/*****************************************************************************
** Procedure:  CMainFrame::OnDestroy
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the window is being destroyed.
**              It records our final position and size into the registry
**
*****************************************************************************/
void CMainFrame::OnDestroy() 
{
	// Save our position/state
	CWindowPlacement wp;
	wp.SavePosition(this, _T("Position"));

	// Save off our divider bar position.
	int cxCurr, cxMin;
	m_wndSplitter.GetColumnInfo(0, cxCurr, cxMin);
	AfxGetApp()->WriteProfileInt(_T("Position"), _T("Splitter"), cxCurr);

	CFrameWnd::OnDestroy();

}// CMainFrame::OnDestroy

/*****************************************************************************
** Procedure:  CMainFrame::OnUpdateStats
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when the statistics change in another 
**              thread.
**
*****************************************************************************/
LRESULT CMainFrame::OnUpdateStats(WPARAM, LPARAM)
{
	CUIObject::GetDocument()->UpdateAllViews(NULL, HINT_CONNCHANGE);
	CUIObject::GetDocument()->UpdateAllViews(NULL, HINT_CONNBUFFCHANGE);
	return 0;

}// CMainFrame::OnUpdateStats

/*****************************************************************************
** Procedure:  CMainFrame::OnSoakTest
**
** Arguments: void
**
** Returns:    void
**
** Description: This event is called when a soak test is running
**
*****************************************************************************/
LRESULT CMainFrame::OnSoakTest(WPARAM, LPARAM)
{
	CUIObject::GetDocument()->GenSoakCall();
	return 0;

}// CMainFrame::OnSoakTest
