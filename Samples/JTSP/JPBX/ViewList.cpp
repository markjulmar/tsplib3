/*******************************************************************/
//
// VIEWLIST.CPP
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
#include "agent.h"
#include "agentgroup.h"
#include "mainfrm.h"
#include "viewlist.h"
#include "line.h"
#include "station.h"
#include "acdqueue.h"
#include "trunk.h"
#include "routepoint.h"
#include "pdialer.h"
#include "ivru.h"

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*------------------------------------------------------------------------------
    CONSTANTS
--------------------------------------------------------------------------------*/
// Image tiles
const UINT IMAGE_GROUP = 0;
const UINT IMAGE_AGENT = 1;
const UINT IMAGE_QUEUE = 2;
const UINT IMAGE_STATION = 3;
const UINT IMAGE_TRUNK = 4;
const UINT IMAGE_RPOINT = 5;
const UINT IMAGE_PDIALER = 6;
const UINT IMAGE_IVR = 7;

/*------------------------------------------------------------------------------
    GLOBAL DATA
--------------------------------------------------------------------------------*/
static LCCOLUMNS g_AgentsColumns[] = {
	{ IDS_LV_NAME, 20 },
	{ IDS_LV_STATE, 20 },
	{ IDS_LV_GROUPS, 256 },
	{ 0, 0 },
};

static LCCOLUMNS g_GroupsColumns[] = {
	{ IDS_LV_NAME, 20 },
	{ IDS_LV_GROUPNUM, 20 },
	{ IDS_LV_AGENTS, 256 },
	{ 0, 0 },
};

static LCCOLUMNS g_StationColumns[] = {
	{ IDS_LV_EXTENSION, 10 },
	{ IDS_LV_NAME, 20 },
	{ 0, 0 },
};

static LCCOLUMNS g_TrunkColumns[] = {
	{ IDS_LV_TRUNKID, 7 },
	{ IDS_LV_STATE, 30 },
	{ IDS_LV_CALLINFO, 256 },
	{ 0, 0 },
};

static LCCOLUMNS g_QueueColumns[] = {
	{ IDS_LV_NAME, 25 },
	{ IDS_LV_QUEUEID, 10 },
	{ IDS_LV_CALLSQUEUED, 13 },
	{ IDS_LV_OLDESTCALL, 10 },
	{ IDS_LV_AGENTCOUNT, 10 },
	{ 0, 0 },
};

static LCCOLUMNS g_RPColumns[] = {
	{ IDS_LV_EXTENSION, 10 },
	{ IDS_LV_NAME, 20 },
	{ IDS_LV_CALLSQUEUED, 13 },
	{ 0, 0 },
};

static LCCOLUMNS g_PDColumns[] = {
	{ IDS_LV_NAME, 20 },
	{ IDS_LV_PDID, 10 },
	{ IDS_LV_CALLSQUEUED, 13 },
	{ 0, 0 },
};

static LCCOLUMNS g_VRUColumns[] = {
	{ IDS_LV_EXTENSION, 10 },
	{ IDS_LV_NAME, 20 },
	{ IDS_LV_CALLSQUEUED, 13 },
	{ 0, 0 },
};

/*---------------------------------------------------------------*/
// RTTI Support
/*---------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CJDView, CListView)

/*---------------------------------------------------------------*/
// CJSView message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CJDView, CListView)
	//{{AFX_MSG_MAP(CJDView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CJDView::CJDView
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the view object
**
*****************************************************************************/
CJDView::CJDView() : m_View(None)
{
	//{{AFX_DATA_INIT(CJDView)
	m_pilSmallIcons = new CImageList;
	m_pilLargeIcons = new CImageList;
	//}}AFX_DATA_INIT

}// CJDView::CJDView

/*****************************************************************************
** Procedure:  CJDView::~CJDView
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the view object
**
*****************************************************************************/
CJDView::~CJDView()
{
	delete m_pilSmallIcons;
	delete m_pilLargeIcons;

}// CJDView::~CJDView

/*****************************************************************************
** Procedure:  CJDView::DoDataExchange
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
void CJDView::DoDataExchange(CDataExchange* pDX)
{
	CListView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJDView)
	//}}AFX_DATA_MAP

}// CJDView::DoDataExchange

/*****************************************************************************
** Procedure:  CJDView::PreCreateWindow
**
** Arguments: 'cs' - CREATESTRUCT with window parameters
**
** Returns:    TRUE/FALSE to continue window creation
**
** Description: This is called during the procesing of the OnCreate function
**              to create our client windows.
**
*****************************************************************************/
BOOL CJDView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= (LVS_ICON | LVS_NOSORTHEADER);
	return CListView::PreCreateWindow(cs);

}// CJDView::PreCreateWindow

/*****************************************************************************
** Procedure:  CJDView::OnInitialUpdate
**
** Arguments: void
**
** Returns:    void
**
** Description: This is called during the creation process to update
**              the window data for the first time.
**
*****************************************************************************/
void CJDView::OnInitialUpdate()
{
	// Set our correct window style based on the saved information
	// in the document
	ModifyStyle(LVS_TYPEMASK, GetDocument()->GetViewStyle());

	// Create our image list.
	m_pilLargeIcons->Create(IDB_LIST_LARGE, 32, 1, RGB(255,0,0));
	m_pilSmallIcons->Create(IDB_LIST_SMALL, 16, 1, RGB(255,0,0));

	// Set them into our listview
	GetListCtrl().SetImageList(m_pilLargeIcons, LVSIL_NORMAL);
	GetListCtrl().SetImageList(m_pilSmallIcons, LVSIL_SMALL);

}// CJDView::OnInitialUpdate

/*****************************************************************************
** Procedure:  CJDView::OnUpdate
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
void CJDView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	if (lHint == HINT_SETCURRENTITEM ||
		(lHint == HINT_RELOADAGENT && m_View == Agents) ||
		(lHint == HINT_RELOADAGENT && m_View == AgentGroups) ||
		(lHint == HINT_RELOADGROUPS && m_View == Agents) ||
		(lHint == HINT_RELOADGROUPS && m_View == AgentGroups) ||
		(lHint == HINT_RELOADTRUNKS && m_View == Trunks) ||
		(lHint == HINT_RELOADQUEUES && m_View == Queues) ||
		(lHint == HINT_RELOADPDIALERS && m_View == PDialers) ||
		(lHint == HINT_RELOADRPOINTS && m_View == RoutePoints) ||
		(lHint == HINT_RELOADIVRS && m_View == IVRUnits) ||
		(lHint == HINT_RELOADSTATION && m_View == Stations))
	{
		// Get the metrics which are being used for this control.
		CDC* pDC = GetDC();
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		ReleaseDC(pDC);

		// Delete everything in the list view.
		GetListCtrl().DeleteAllItems();
		while (GetListCtrl().DeleteColumn(0))
			;

		// Determine what to load into our listview based on the hint
		// or view type.
		LCCOLUMNS* pCI = NULL;
		CUIObject* pBase = NULL;
		if (lHint == HINT_SETCURRENTITEM)
		{
			m_View = None;
			if (pHint == BO_AGENTS)
				m_View = Agents;
			else if (pHint == BO_GROUPS)
				m_View = AgentGroups;
			else if (pHint == BO_STATIONS)
				m_View = Stations;
			else if (pHint == BO_TRUNKS)
				m_View = Trunks;
			else if (pHint == BO_QUEUES)
				m_View = Queues;
			else if (pHint == BO_RPOINTS)
				m_View = RoutePoints;
			else if (pHint == BO_PDIALERS)
				m_View = PDialers;
			else if (pHint == BO_IVRUNITS)
				m_View = IVRUnits;
			else
			{
				try
				{
					pBase = dynamic_cast<CUIObject*>(pHint);
				}
				catch (...)
				{
				}

				try
				{
					if (dynamic_cast<CAgent*>(pBase) != NULL)
						m_View = Agents;
					else if (dynamic_cast<CAgentGroup*>(pBase) != NULL)
						m_View = AgentGroups;
					else if (dynamic_cast<CStation*>(pBase) != NULL)
						m_View = Stations;
					else if (dynamic_cast<CTrunk*>(pBase) != NULL)
						m_View = Trunks;
					else if (dynamic_cast<CACDQueue*>(pBase) != NULL)
						m_View = Queues;
					else if (dynamic_cast<CRoutePoint*>(pBase) != NULL)
						m_View = RoutePoints;
					else if (dynamic_cast<CPDialer*>(pBase) != NULL)
						m_View = PDialers;
					else if (dynamic_cast<CVRU*>(pBase) != NULL)
						m_View = IVRUnits;
					else ASSERT (FALSE);
				}
				catch (...)
				{
					ASSERT (FALSE);
				}
			}
		}

		// Get the proper column information based on our current view
		if (m_View == Agents)
			pCI = g_AgentsColumns;
		else if (m_View == AgentGroups)
			pCI = g_GroupsColumns;
		else if (m_View == Stations)
			pCI = g_StationColumns;
		else if (m_View == Trunks)
			pCI = g_TrunkColumns;
		else if (m_View == Queues)
			pCI = g_QueueColumns;
		else if (m_View == RoutePoints)
			pCI = g_RPColumns;
		else if (m_View == PDialers)
			pCI = g_PDColumns;
		else if (m_View == IVRUnits)
			pCI = g_VRUColumns;
		else ASSERT (FALSE);

		// Add our column headers.
		if (pCI != NULL)
		{
			for (int i = 0; pCI->uiName != 0; i++, pCI++)
			{
				CString strName;
				VERIFY(strName.LoadString(pCI->uiName));
				GetListCtrl().InsertColumn (i, strName, LVCFMT_LEFT, 
					tm.tmAveCharWidth*(pCI->iWidth+1));
			}

			// Now load our information into the box.
			if (pBase == NULL)
			{
				if (m_View == Agents)
				{
					for (int i = 0; i < GetDocument()->GetAgentCount(); i++)
						InsertObject(GetDocument()->GetAgent(i));
				}
				else if (m_View == AgentGroups)
				{
					for (int i = 0; i < GetDocument()->GetAgentGroupCount(); i++)
						InsertObject(GetDocument()->GetAgentGroup(i));
				}
				else
				{
					LoadLines();
				}
			}
			else
				InsertObject(pBase);
		}
	}
	else if (lHint == HINT_ITEMCHANGED && pHint != NULL)
	{
		for (int i = 0; i < GetListCtrl().GetItemCount(); i++)
		{
			if (reinterpret_cast<CObject*>(GetListCtrl().GetItemData(i)) == pHint)
			{
				GetListCtrl().RedrawItems(i,i);
				break;
			}
		}
	}
	else if (lHint == HINT_CONNCHANGE && m_View == Queues)
		GetListCtrl().RedrawItems(0, GetListCtrl().GetItemCount());

}// CJDView::OnUpdate

/*****************************************************************************
** Procedure:  CJDView::LoadLines
**
** Arguments: void
**
** Returns:    void
**
** Description: This is used to insert all the lines of the given type
**
*****************************************************************************/
void CJDView::LoadLines()
{
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		if (m_View == Stations)
		{
			CStation* pStation = dynamic_cast<CStation*>(pLine);
			if (pStation != NULL)
				InsertObject(pStation);
		}
		else if (m_View == Queues)
		{
			CACDQueue* pQueue = dynamic_cast<CACDQueue*>(pLine);
			if (pQueue != NULL)
				InsertObject(pQueue);
		}
		else if (m_View == Trunks)
		{
			CTrunk* pTrunk = dynamic_cast<CTrunk*>(pLine);
			if (pTrunk != NULL)
				InsertObject(pTrunk);
		}
		else if (m_View == RoutePoints)
		{
			CRoutePoint* pRP = dynamic_cast<CRoutePoint*>(pLine);
			if (pRP != NULL)
				InsertObject(pRP);
		}
		else if (m_View == PDialers)
		{
			CPDialer* pPD = dynamic_cast<CPDialer*>(pLine);
			if (pPD != NULL)
				InsertObject(pPD);
		}
		else if (m_View == IVRUnits)
		{
			CVRU* pVru = dynamic_cast<CVRU*>(pLine);
			if (pVru != NULL)
				InsertObject(pVru);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJDView::LoadLines

/*****************************************************************************
** Procedure:  CJDView::InsertObject
**
** Arguments: 'pObject' - Object to insert into the list
**
** Returns:    void
**
** Description: This is used to insert a single object into our listview
**
*****************************************************************************/
void CJDView::InsertObject(CUIObject* pObject)
{
	// Use the following LV_ITEM structure to add all
	// the item information.
	LV_ITEM lvItem;
	lvItem.iItem = GetListCtrl().GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM | LVIF_IMAGE;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.pszText = LPSTR_TEXTCALLBACK;
	lvItem.cchTextMax = 0;
	lvItem.iSubItem = 0;
	lvItem.lParam = reinterpret_cast<LPARAM>(pObject);

	// Set the appropriate image
	if (m_View == Agents)
		lvItem.iImage = IMAGE_AGENT;
	else if (m_View == AgentGroups)
		lvItem.iImage = IMAGE_GROUP;
	else if (m_View == Stations)
		lvItem.iImage = IMAGE_STATION;
	else if (m_View == Trunks)
		lvItem.iImage = IMAGE_TRUNK;
	else if (m_View == Queues)
		lvItem.iImage = IMAGE_QUEUE;
	else if (m_View == RoutePoints)
		lvItem.iImage = IMAGE_RPOINT;
	else if (m_View == PDialers)
		lvItem.iImage = IMAGE_PDIALER;
	else if (m_View == IVRUnits)
		lvItem.iImage = IMAGE_IVR;
	else ASSERT (FALSE);

	VERIFY (GetListCtrl().InsertItem (&lvItem) == lvItem.iItem);

}// CJDView::InsertObject

/*****************************************************************************
** Procedure:  CJDView::OnGetDispInfo
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
void CJDView::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &pDispInfo->item;
	CUIObject* pBase = reinterpret_cast<CUIObject*>(pItem->lParam);

	static char szBuffer[257];
	ZeroMemory(szBuffer, sizeof(szBuffer));

#ifdef _DEBUG
	szBuffer[256] = '\xaa';
#endif
	if (pItem->mask & LVIF_TEXT)
	{
		try 
		{
			if (dynamic_cast<CAgent*>(pBase) != NULL)
				FillInAgent(dynamic_cast<CAgent*>(pBase), pItem->iSubItem, szBuffer);

			else if (dynamic_cast<CAgentGroup*>(pBase) != NULL)
				FillInGroup(dynamic_cast<CAgentGroup*>(pBase), pItem->iSubItem, szBuffer);
			
			else if (dynamic_cast<CStation*>(pBase) != NULL)
				FillInStation(dynamic_cast<CStation*>(pBase), pItem->iSubItem, szBuffer);

			else if (dynamic_cast<CTrunk*>(pBase) != NULL)
				FillInTrunk(dynamic_cast<CTrunk*>(pBase), pItem->iSubItem, szBuffer);

			else if (dynamic_cast<CACDQueue*>(pBase) != NULL)
				FillInQueue(dynamic_cast<CACDQueue*>(pBase), pItem->iSubItem, szBuffer);

			else if (dynamic_cast<CRoutePoint*>(pBase) != NULL)
				FillInRPoint(dynamic_cast<CRoutePoint*>(pBase), pItem->iSubItem, szBuffer);

			else if (dynamic_cast<CPDialer*>(pBase) != NULL)
				FillInPDialer(dynamic_cast<CPDialer*>(pBase), pItem->iSubItem, szBuffer);

			else if (dynamic_cast<CVRU*>(pBase) != NULL)
				FillInVru(dynamic_cast<CVRU*>(pBase), pItem->iSubItem, szBuffer);
		}
		catch (...)
		{
			ASSERT (FALSE);
		}
	}
#ifdef _DEBUG
	ASSERT (szBuffer[256] == '\xaa');
#endif

	pItem->pszText = szBuffer;
	*pResult = 0;

}// CJDView::OnGetDispInfo

/*****************************************************************************
** Procedure:  CJDView::FillInGroup
**
** Arguments: 'pGroup' - Group object to retrieve information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a group object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInGroup(CAgentGroup* pGroup, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Name
			lstrcpy(pszBuffer, pGroup->GetGroupName());
			break;
		case 1:		// ID
			wsprintf(pszBuffer, _T("0x%04x"), pGroup->GetGroupID());
			break;
		case 2:		// Agents
			lstrcpyn(pszBuffer, GetDocument()->GetAgentsFromGroup(pGroup), 255);
			break;
	}

}// CJDView::FillInGroup

/*****************************************************************************
** Procedure:  CJDView::FillInAgent
**
** Arguments: 'pAgent' - Agent object to retrieve information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from an agent object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInAgent(CAgent* pAgent, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Name
			lstrcpy(pszBuffer, pAgent->GetAgentId());
			break;
		case 1:		// State
			lstrcpy(pszBuffer, pAgent->GetStateText());
			break;
		case 2:		// Groups
			lstrcpyn(pszBuffer, GetDocument()->GetAgentGroupList(pAgent), 255);
			break;
	}

}// CJDView::FillInAgent

/*****************************************************************************
** Procedure:  CJDView::FillInStation
**
** Arguments: 'pStation' - Station object to retrieve information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a station object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInStation(CStation* pStation, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Extension
			lstrcpy(pszBuffer, pStation->GetExtension());
			break;
		case 1:		// Name
			lstrcpy(pszBuffer, pStation->GetLineName());
			break;
	}

}// CJDView::FillInStation

/*****************************************************************************
** Procedure:  CJDView::FillInTrunk
**
** Arguments: 'pTrunk' - Trunk to fill in 
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a trunk object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInTrunk(CTrunk* pTrunk, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// TrunkID
			lstrcpy(pszBuffer, pTrunk->GetExtension());
			break;
		case 1:		// State
			lstrcpy(pszBuffer, pTrunk->GetTrunkStateString());
			break;
		case 2:	    // Call information
			{
				CCall* pCall = pTrunk->GetCallByPosition(0);
				if (pCall != NULL)
					wsprintf(pszBuffer, _T("%s, %s"), pCall->GetDNIS(), pCall->GetANI());
			}
			break;
	}

}// CJDView::FillInTrunk

/*****************************************************************************
** Procedure:  CJDView::FillInQueue
**
** Arguments: 'pQueue' - Queue object to retrieve information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a queue object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInQueue(CACDQueue* pQueue, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Name
			lstrcpy(pszBuffer, pQueue->GetLineName());
			break;
		case 1:		// Queue id
			lstrcpy(pszBuffer, pQueue->GetExtension());
			break;
		case 2:		// Count of calls queued
			wsprintf(pszBuffer, _T("%d"), pQueue->GetCallCount());
			break;
		case 3:		// Oldest call
			if (pQueue->GetCallCount() > 0)
			{
				CCall* pCall = pQueue->GetHeadCall();
				if (pCall != NULL)
					wsprintf(pszBuffer, _T("%d sec"), pCall->SecondsInState());
			}
			break;
		case 4:		// Agent count
			wsprintf(pszBuffer, _T("%d"), pQueue->GetAgentCount());
			break;
	}

}// CJDView::FillInQueue

/*****************************************************************************
** Procedure:  CJDView::FillInPDialer
**
** Arguments: 'pDialer' - Predictive dialer to fill in information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a dialer object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInPDialer(CPDialer* pDialer, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Name
			lstrcpy(pszBuffer, pDialer->GetLineName());
			break;
		case 1:		// ID
			wsprintf(pszBuffer, _T("%04d"), pDialer->GetLineID());
			break;
		case 2:		// Count of calls queued
			wsprintf(pszBuffer, _T("%d"), pDialer->GetCallCount());
			break;
	}

}// CJDView::FillInPDialer

/*****************************************************************************
** Procedure:  CJDView::FillInRPoint
**
** Arguments: 'pRPoint' - Route object to retrieve information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a route point object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInRPoint(CRoutePoint* pRP, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Extension
			lstrcpy(pszBuffer, pRP->GetExtension());
			break;
		case 1:		// Name
			lstrcpy(pszBuffer, pRP->GetLineName());
			break;
		case 2:		// Count of calls queued
			wsprintf(pszBuffer, _T("%d"), pRP->GetCallCount());
			break;
	}

}// CJDView::FillInRPoint

/*****************************************************************************
** Procedure:  CJDView::FillInVru
**
** Arguments: 'pVRU' - VRU object to retrieve information for
**            'iCol' - Column to fill in
**            'pszBuffer' - Buffer to load string into
**
** Returns:    void
**
** Description: This pulls information from a VRU object and loads the
**              details into our listview.
**
*****************************************************************************/
void CJDView::FillInVru(CVRU* pVRU, int iCol, char* pszBuffer)
{
	switch (iCol)
	{
		case 0:		// Extension
			lstrcpy(pszBuffer, pVRU->GetExtension());
			break;
		case 1:		// Name
			lstrcpy(pszBuffer, pVRU->GetLineName());
			break;
		case 2:		// Count of calls queued
			wsprintf(pszBuffer, _T("%d"), pVRU->GetCallCount());
			break;
	}

}// CJDView::FillInVRU

/*****************************************************************************
** Procedure:  CJDView::OnDblclk
**
** Arguments: 'pNMHDR' - TREE view header
**            'pResult' - Returning result code
**
** Returns:    void
**
** Description: This notification is called when a selection is double-clicked
**
*****************************************************************************/
void CJDView::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	for (int i = 0; i < GetListCtrl().GetItemCount(); i++)
	{
		if (GetListCtrl().GetItemState(i, LVIS_FOCUSED) & LVIS_FOCUSED)
		{
			// Show the properties of an individual item.
			CUIObject* pBase = reinterpret_cast<CUIObject*>(GetListCtrl().GetItemData(i));
			pBase->Edit(AfxGetMainWnd());
			break;
		}
	}
	*pResult = 0;

}// CJDView::OnDblclk

/*****************************************************************************
** Procedure:  CJDView::OnRightclk
**
** Arguments: 'pNMHDR' - TREE view header
**            'pResult' - Returning result code
**
** Returns:    void
**
** Description: This notification is called when a selection is right-clicked
**              to obtain a context menu
**
*****************************************************************************/
void CJDView::OnRightclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	for (int i = 0; i < GetListCtrl().GetItemCount(); i++)
	{
		if (GetListCtrl().GetItemState(i, LVIS_FOCUSED) & LVIS_FOCUSED)
		{
			// Show the properties of an individual item.
			CUIObject* pBase = reinterpret_cast<CUIObject*>(GetListCtrl().GetItemData(i));
			pBase->ContextMenu(AfxGetMainWnd());
			break;
		}
	}
	*pResult = 0;

}// CJDView::OnRightclk

