/*******************************************************************/
//
// VIEWSTATS.CPP
//
// Right-pane view for the JPBX eulator
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
#include "viewstats.h"
#include "ddx.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// RTTI Support
/*---------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CJSView, CFormView)

/*---------------------------------------------------------------*/
// CJSView message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CJSView, CFormView)
	//{{AFX_MSG_MAP(CJSView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CJSView::CJSView
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the view object
**
*****************************************************************************/
CJSView::CJSView() : CFormView(IDD)
{
	//{{AFX_DATA_INIT(CJSView)
	m_fRunningTest = false;
	//}}AFX_DATA_INIT

}// CJSView::CJSView constructor

/*****************************************************************************
** Procedure:  CJSView::~CJSView
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the view object
**
*****************************************************************************/
CJSView::~CJSView()
{
}// CJSView::~CJSView

/*****************************************************************************
** Procedure:  CJSView::DoDataExchange
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
void CJSView::DoDataExchange(CDataExchange* pDX)
{
	CJTDocument* pDoc = GetDocument();

	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJSView)
	DDX_Control(pDX, IDC_HEADER2, m_ctlHeader1);
	DDX_Control(pDX, IDC_HEADER1, m_ctlStatBar);
	DDX_Control(pDX, IDC_DEBUGLIST, m_lbDebugOut);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate)
	{
		int iConnections = GetDocument()->GetConnectionMgr().GetConnectionCount();
		DDX_Text(pDX, IDC_CONNECTIONS, iConnections);
		iConnections = GetDocument()->GetCPS();
		DDX_Text(pDX, IDC_CPS, iConnections);

		if (m_fRunningTest != GetDocument()->IsRunningSoakTest())
		{
			m_fRunningTest = GetDocument()->IsRunningSoakTest();
			GetDlgItem(IDC_SOAKTEST)->Invalidate(FALSE);
		}

		// Get the number of trunks siezed.
		iConnections = 0;
		CLineDevice* pLine = pDoc->GetFirstLine();
		while (pLine != NULL)
		{
			CTrunk* pTrunk = dynamic_cast<CTrunk*>(pLine);
			if (pTrunk != NULL)
			{
				if (pTrunk->GetTrunkState() == CTrunk::Outbound ||
					pTrunk->GetTrunkState() == CTrunk::Inbound)
					iConnections++;
			}
			pLine = pDoc->GetNextLine();
		}
		DDX_Text(pDX, IDC_TRUNKSALLOC, iConnections);
	}

}// CJSView::DoDataExchange

/*****************************************************************************
** Procedure:  CJSView::OnInitialUpdate
**
** Arguments: void
**
** Returns:    void
**
** Description: This is called during the creation process to update
**              the window data for the first time.
**
*****************************************************************************/
void CJSView::OnInitialUpdate()
{
	// Allow the base class to initialize.
	CFormView::OnInitialUpdate();

	// Set our gradient colors
	m_ctlHeader1.SetTextColor(RGB(64,64,64));
	m_ctlHeader1.SetBackColor(RGB(192,192,192));
	m_ctlStatBar.SetTextColor(RGB(64,64,64));
	m_ctlStatBar.SetBackColor(RGB(192,192,192));

	// Force our resize code to fire
	CRect rcBox;
	GetClientRect(&rcBox);
	OnSize(SIZE_RESTORED, rcBox.Width(), rcBox.Height());

	// Force our initial load
	OnUpdate(NULL, HINT_CONNBUFFCHANGE, NULL);

}// CJSView::OnInitialUpdate

/*****************************************************************************
** Procedure:  CJSView::OnSize
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
void CJSView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	UINT uiControls[] = { IDC_HEADER1, IDC_HEADER2, 0 };

	// Resize our header controls.
	CRect rcBox, rcWnd, rcStatus;
	for (int i = 0; uiControls[i]; i++)
	{
		CWnd* pwnd = GetDlgItem(uiControls[i]);
		if (pwnd != NULL)
		{
			pwnd->GetWindowRect(&rcBox);
			ScreenToClient(rcBox);
			pwnd->SetWindowPos( &CWnd::wndTop, 0, rcBox.top, cx, rcBox.Height(), 
				SWP_NOZORDER | SWP_NOACTIVATE );
		}
	}

	// Resize our status bar at the bottom
	if (m_ctlStatBar.GetSafeHwnd() != NULL && m_lbDebugOut.GetSafeHwnd() != NULL)
	{
		GetClientRect(&rcWnd);
		m_lbDebugOut.GetWindowRect(&rcBox);
		ScreenToClient(&rcBox);
		m_lbDebugOut.SetWindowPos(&CWnd::wndTop, 0, rcBox.top, cx, rcWnd.bottom - rcBox.top,  
			SWP_NOZORDER | SWP_NOACTIVATE );
	}

}// CJSView::OnSize

/*****************************************************************************
** Procedure:  CJSView::OnCtlColor
**
** Arguments: 'pDC' - Device context
**            'pWnd' - Window to return colors for
**            'nCtlColor' - Control color
**
** Returns:    Brush to paint background with
**
** Description: This event is called to determine view colors
**
*****************************************************************************/
HBRUSH CJSView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		hbr = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == 0xffff)
			pDC->SetTextColor(RGB(255,255,255));
		else
		{
			if (pWnd->GetDlgCtrlID() == IDC_SOAKTEST)
			{
				if (GetDocument()->IsRunningSoakTest())
					pDC->SetTextColor(RGB(255,0,0));
				else
					pDC->SetTextColor(RGB(128,128,128));
			}
			else
				pDC->SetTextColor(RGB(0,255,0));
		}
	}
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		hbr = (HBRUSH) ::GetStockObject(LTGRAY_BRUSH);
	}

	return hbr;

}// CJSView::OnCtlColor

/*****************************************************************************
** Procedure:  CJSView::OnUpdate
**
** Arguments: 'pSender' - which view sent the update
**            'lHint' - Hint of which object has changed
**            'pObject' - Object pointer (may be null)
**
** Returns:    void
**
** Description: This is called when something in our document changes and
**              we possibly need to refresh our view of the data
**
*****************************************************************************/
void CJSView::OnUpdate(CView*, LPARAM lHint, CObject*) 
{
	if (lHint == HINT_CONNBUFFCHANGE)
	{
		CDataExchange dx(this, false);
		DDX_LBStringArray(&dx, IDC_DEBUGLIST, GetDocument()->GetLogBuffer());
	}
	else if (lHint == HINT_CONNCHANGE)
		UpdateData(FALSE);

}// CJSView::OnUpdate
