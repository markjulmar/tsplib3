/*******************************************************************/
//
// VIEWTREE.CPP
//
// Left-pane view for the JPBX eulator
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
#include "mainfrm.h"
#include "agent.h"
#include "agentgroup.h"
#include "viewtree.h"
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

/*---------------------------------------------------------------*/
// GLOBALS and CONSTANTS
/*---------------------------------------------------------------*/
// Image tiles
//
// CLOSED = Main closed view
// OPEN   = Main open view
// CLOSED2 = Sub item closed view
// OPEN2   = Sub item open view
//
const UINT IMAGE_HEADER			= 0;
const UINT IMAGE_AGENT_CLOSED	= 1;
const UINT IMAGE_AGENT_OPEN		= IMAGE_AGENT_CLOSED;
const UINT IMAGE_AGENT_CLOSED2	= 2;
const UINT IMAGE_AGENT_OPEN2	= IMAGE_AGENT_CLOSED2;
const UINT IMAGE_GROUP_CLOSED	= 3;
const UINT IMAGE_GROUP_OPEN		= IMAGE_GROUP_CLOSED;
const UINT IMAGE_GROUP_CLOSED2	= 4;
const UINT IMAGE_GROUP_OPEN2	= IMAGE_GROUP_CLOSED2;
const UINT IMAGE_QUEUE_CLOSED   = 5;
const UINT IMAGE_QUEUE_OPEN     = IMAGE_QUEUE_CLOSED;
const UINT IMAGE_QUEUE_CLOSED2  = IMAGE_QUEUE_CLOSED;
const UINT IMAGE_QUEUE_OPEN2    = IMAGE_QUEUE_CLOSED;
const UINT IMAGE_STATION_OPEN	= 6;
const UINT IMAGE_STATION_CLOSED	= IMAGE_STATION_OPEN;
const UINT IMAGE_STATION_OPEN2	= IMAGE_STATION_OPEN;
const UINT IMAGE_STATION_CLOSED2= IMAGE_STATION_OPEN;
const UINT IMAGE_TRUNK_OPEN		= 7;
const UINT IMAGE_TRUNK_CLOSED   = IMAGE_TRUNK_OPEN;
const UINT IMAGE_TRUNK_OPEN2    = IMAGE_TRUNK_OPEN;
const UINT IMAGE_TRUNK_CLOSED2  = IMAGE_TRUNK_OPEN;
const UINT IMAGE_RPOINT_OPEN    = 8;
const UINT IMAGE_RPOINT_CLOSED  = IMAGE_RPOINT_OPEN;
const UINT IMAGE_RPOINT_OPEN2   = IMAGE_RPOINT_OPEN;
const UINT IMAGE_RPOINT_CLOSED2 = IMAGE_RPOINT_OPEN;
const UINT IMAGE_PDIALER_OPEN    = 9;
const UINT IMAGE_PDIALER_CLOSED  = IMAGE_PDIALER_OPEN;
const UINT IMAGE_PDIALER_OPEN2   = IMAGE_PDIALER_OPEN;
const UINT IMAGE_PDIALER_CLOSED2 = IMAGE_PDIALER_OPEN;
const UINT IMAGE_VRU_OPEN		= 10;
const UINT IMAGE_VRU_CLOSED     = IMAGE_VRU_OPEN;
const UINT IMAGE_VRU_OPEN2      = IMAGE_VRU_OPEN;
const UINT IMAGE_VRU_CLOSED2    = IMAGE_VRU_OPEN;

/*---------------------------------------------------------------*/
// RTTI INFORMATION
/*---------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CJTView, CTreeView)

/*---------------------------------------------------------------*/
// CJTView message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CJTView, CTreeView)
	//{{AFX_MSG_MAP(CJTView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CJTView::CJTView
**
** Arguments: void
**
** Returns:    void
**
** Description:  Constructor for the view object
**
*****************************************************************************/
CJTView::CJTView()
{
	//{{AFX_DATA_INIT(CJTView)
	m_pilIcons = new CImageList;
	m_htiRoot = NULL;
	m_htiGroups = NULL;
	m_htiAgents = NULL;
	m_htiStations = NULL;
	m_htiQueues = NULL;
	m_htiTrunks = NULL;
	m_htiRPoints = NULL;
	m_htiPDialers = NULL;
	m_htiVRUs = NULL;
	m_pSelected = BO_NONE;
	//}}AFX_DATA_INIT

}// CJTView::CJTView

/*****************************************************************************
** Procedure:  CJTView::~CJTView
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the view object
**
*****************************************************************************/
CJTView::~CJTView()
{
	delete m_pilIcons;

}// CJTView::~CJTView

/*****************************************************************************
** Procedure:  CJTView::DoDataExchange
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
void CJTView::DoDataExchange(CDataExchange* pDX)
{
	CTreeView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJTView)
	//}}AFX_DATA_MAP

}// CJTView::DoDataExchange

/*****************************************************************************
** Procedure:  CJTView::PreCreateWindow
**
** Arguments: 'cs' - CREATESTRUCT with window parameters
**
** Returns:    TRUE/FALSE to continue window creation
**
** Description: This is called during the procesing of the OnCreate function
**              to create our client windows.
**
*****************************************************************************/
BOOL CJTView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	return CTreeView::PreCreateWindow(cs);

}// CJTView::PreCreateWindow

/*****************************************************************************
** Procedure:  CJTView::OnInitialUpdate
**
** Arguments: void
**
** Returns:    void
**
** Description: This is called during the creation process to update
**              the window data for the first time.
**
*****************************************************************************/
void CJTView::OnInitialUpdate()
{
	// Pass it on
	CTreeView::OnInitialUpdate();

	// Create our image list.
	m_pilIcons->Create(IDB_TREE_IMAGES, 16, 1, RGB(0,255,0));
	
	// Set the image list into the tree.
	GetTreeCtrl().SetImageList(m_pilIcons, TVSIL_NORMAL);

	// Add our "root" element
	CString strHeader;
	strHeader.LoadString(IDS_ROOT);
	strHeader += GetDocument()->GetServerName();
	m_htiRoot = GetTreeCtrl().InsertItem (
		TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT,
		strHeader,
		IMAGE_HEADER,
		IMAGE_HEADER,
		0, 0, (LPARAM)BO_NONE, NULL, NULL);

	// Load each of our components.
	LoadGroups();
	LoadAgents();
	LoadStations();
	LoadQueues();
	LoadTrunks();
	LoadRPoints();
	LoadPDialers();
	LoadVrus();

	// Set the initial state of the tree based on what we have saved.
	DWORD dwTreeState = GetDocument()->GetTreeStyle();
	HTREEITEM hCurrent = m_htiRoot;
	while (dwTreeState != 0 && hCurrent != NULL)
	{
		if (dwTreeState & 0x1)
			GetTreeCtrl().Expand(hCurrent, TVE_EXPAND);

		if (GetTreeCtrl().ItemHasChildren(hCurrent))
		{
			hCurrent = GetTreeCtrl().GetChildItem(hCurrent);
			if (!GetTreeCtrl().ItemHasChildren(hCurrent))
			{
				hCurrent = GetTreeCtrl().GetParentItem(hCurrent);
				hCurrent = GetTreeCtrl().GetNextSiblingItem(hCurrent);
			}
		}
		else
		{
			if (GetTreeCtrl().GetParentItem(hCurrent) != GetTreeCtrl().GetRootItem())
				hCurrent = GetTreeCtrl().GetParentItem(hCurrent);
			hCurrent = GetTreeCtrl().GetNextSiblingItem(hCurrent);
		} 
		dwTreeState >>= 1;
	}

}// CJTView::OnInitialUpdate

/*****************************************************************************
** Procedure:  CJTView::OnUpdate
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
void CJTView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
		case HINT_RELOADAGENT:	  LoadAgents(); break;
		case HINT_RELOADGROUPS:	  LoadGroups(); break;
		case HINT_RELOADSTATION:  LoadStations(); break;
		case HINT_RELOADQUEUES:	  LoadQueues(); break;
		case HINT_RELOADTRUNKS:	  LoadTrunks(); break;
		case HINT_RELOADRPOINTS:  LoadRPoints(); break;
		case HINT_RELOADPDIALERS: LoadPDialers(); break;
		case HINT_RELOADIVRS:	  LoadVrus(); break;
		case HINT_ITEMCHANGED:	  UpdateItem(pHint); break;

		case HINT_DOCCLOSING:
			{
				// Record our tree state
				DWORD dwTreeState = 0;
				HTREEITEM hCurrent = m_htiRoot;
				while (hCurrent != NULL)
				{
					dwTreeState >>= 1;
					if (GetTreeCtrl().GetItemState(hCurrent, TVIS_EXPANDED))
						dwTreeState |= 0x40;

					if (GetTreeCtrl().ItemHasChildren(hCurrent))
					{
						hCurrent = GetTreeCtrl().GetChildItem(hCurrent);
						if (!GetTreeCtrl().ItemHasChildren(hCurrent))
						{
							hCurrent = GetTreeCtrl().GetParentItem(hCurrent);
							hCurrent = GetTreeCtrl().GetNextSiblingItem(hCurrent);
						}
					}
					else
					{
						if (GetTreeCtrl().GetParentItem(hCurrent) != GetTreeCtrl().GetRootItem())
							hCurrent = GetTreeCtrl().GetParentItem(hCurrent);
						hCurrent = GetTreeCtrl().GetNextSiblingItem(hCurrent);
					} 
				}

				GetDocument()->SetTreeStyle(dwTreeState);

			}
			break;
	}

}// CJTView::OnUpdate

/*****************************************************************************
** Procedure:  CJTView::LoadGroups
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the group list into our tree view
**
*****************************************************************************/
void CJTView::LoadGroups()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Agent Group" header
	if (m_htiGroups == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_GROUPS);
		m_htiGroups = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_GROUP_CLOSED,
			IMAGE_GROUP_OPEN,
			0, 0, (LPARAM)BO_GROUPS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the activity section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiGroups);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the agent groups
	for (int i = 0; i < GetDocument()->GetAgentGroupCount(); i++)
	{
		CAgentGroup* pGroup = GetDocument()->GetAgentGroup(i);
		GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			pGroup->GetGroupName(),
			IMAGE_GROUP_CLOSED2,
			IMAGE_GROUP_OPEN2,
			0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pGroup)), m_htiGroups, NULL);
	}

}// CJTViedw::LoadGroups

/*****************************************************************************
** Procedure:  CJTView::LoadAgents
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the agent list into our tree view
**
*****************************************************************************/
void CJTView::LoadAgents()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Agents" header
	if (m_htiAgents == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_AGENTS);
		m_htiAgents = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_AGENT_CLOSED,
			IMAGE_AGENT_OPEN,
			0, 0, (LPARAM)BO_AGENTS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the activity section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiAgents);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the agents
	for (int i = 0; i < GetDocument()->GetAgentCount(); i++)
	{
		CAgent* pAgent = GetDocument()->GetAgent(i);
		GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			pAgent->GetAgentId(),
			IMAGE_AGENT_CLOSED2,
			IMAGE_AGENT_OPEN2,
			0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pAgent)), m_htiAgents, NULL);
	}

}// CJTViedw::LoadAgents

/*****************************************************************************
** Procedure:  CJTView::LoadStations
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the line station list into our tree view
**
*****************************************************************************/
void CJTView::LoadStations()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Stations" header
	if (m_htiStations == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_STATIONS);
		m_htiStations = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_STATION_CLOSED,
			IMAGE_STATION_OPEN,
			0, 0, (LPARAM)BO_STATIONS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiStations);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the stations
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CStation* pStation = dynamic_cast<CStation*>(pLine);
		if (pStation != NULL)
		{
			GetTreeCtrl().InsertItem(
				TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
				pStation->GetLineName(),
				IMAGE_STATION_CLOSED2,
				IMAGE_STATION_OPEN2,
				0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pStation)), m_htiStations, NULL);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJTViedw::LoadStations

/*****************************************************************************
** Procedure:  CJTView::LoadTrunks
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the line trunk list into our tree view
**
*****************************************************************************/
void CJTView::LoadTrunks()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Trunks" header
	if (m_htiTrunks == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_TRUNKS);
		m_htiTrunks = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_TRUNK_CLOSED,
			IMAGE_TRUNK_OPEN,
			0, 0, (LPARAM)BO_TRUNKS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiTrunks);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the trunks
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CTrunk* pTrunk = dynamic_cast<CTrunk*>(pLine);
		if (pTrunk != NULL)
		{
			GetTreeCtrl().InsertItem(
				TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
				pTrunk->GetLineName(),
				IMAGE_TRUNK_CLOSED2,
				IMAGE_TRUNK_OPEN2,
				0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pTrunk)), m_htiTrunks, NULL);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJTViedw::LoadTrunks

/*****************************************************************************
** Procedure:  CJTView::LoadQueues
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the line queue list into our tree view
**
*****************************************************************************/
void CJTView::LoadQueues()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Queues" header
	if (m_htiQueues == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_QUEUES);
		m_htiQueues = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_QUEUE_CLOSED,
			IMAGE_QUEUE_OPEN,
			0, 0, (LPARAM)BO_QUEUES, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiQueues);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the queues
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CACDQueue* pQueue = dynamic_cast<CACDQueue*>(pLine);
		if (pQueue != NULL)
		{
			GetTreeCtrl().InsertItem(
				TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
				pQueue->GetLineName(),
				IMAGE_QUEUE_CLOSED2,
				IMAGE_QUEUE_OPEN2,
				0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pQueue)), m_htiQueues, NULL);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJTViedw::LoadQueues

/*****************************************************************************
** Procedure:  CJTView::LoadRPoints
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the line route points list into our tree view
**
*****************************************************************************/
void CJTView::LoadRPoints()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Route Points" header
	if (m_htiRPoints == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_RPOINTS);
		m_htiRPoints = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_RPOINT_CLOSED,
			IMAGE_RPOINT_OPEN,
			0, 0, (LPARAM)BO_RPOINTS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiRPoints);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the route points
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CRoutePoint* pRP = dynamic_cast<CRoutePoint*>(pLine);
		if (pRP != NULL)
		{
			GetTreeCtrl().InsertItem(
				TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
				pRP->GetLineName(),
				IMAGE_RPOINT_CLOSED2,
				IMAGE_RPOINT_OPEN2,
				0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pRP)), m_htiRPoints, NULL);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJTViedw::LoadRPoints

/*****************************************************************************
** Procedure:  CJTView::LoadPDialers
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the line dialer list into our tree view
**
*****************************************************************************/
void CJTView::LoadPDialers()
{
	// If we have no root header then this is the first
	// time this has been called - add the "Dialer" header
	if (m_htiPDialers == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_PDIALERS);
		m_htiPDialers = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_PDIALER_CLOSED,
			IMAGE_PDIALER_OPEN,
			0, 0, (LPARAM)BO_PDIALERS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiPDialers);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the stations
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CPDialer* pPDialer = dynamic_cast<CPDialer*>(pLine);
		if (pPDialer != NULL)
		{
			GetTreeCtrl().InsertItem(
				TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
				pPDialer->GetLineName(),
				IMAGE_PDIALER_CLOSED2,
				IMAGE_PDIALER_OPEN2,
				0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pPDialer)), m_htiPDialers, NULL);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJTViedw::LoadPDialers

/*****************************************************************************
** Procedure:  CJTView::LoadVrus
**
** Arguments: void
**
** Returns:    void
**
** Description: This loads the line VRUs list into our tree view
**
*****************************************************************************/
void CJTView::LoadVrus()
{
	// If we have no root header then this is the first
	// time this has been called - add the "VRU" header
	if (m_htiVRUs == NULL)
	{
		CString strHeader;
		strHeader.LoadString(IDS_VRUS);
		m_htiVRUs = GetTreeCtrl().InsertItem(
			TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
			strHeader,
			IMAGE_VRU_CLOSED,
			IMAGE_VRU_OPEN,
			0, 0, (LPARAM)BO_IVRUNITS, m_htiRoot, NULL);
	}
	else
	{
		// First delete all the things under the section.
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(m_htiVRUs);
		while (hChild != NULL)
		{
			HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
			GetTreeCtrl().DeleteItem(hChild);
			hChild = hNext;
		}
	}

	// Add the vrus
	CJTDocument* pDoc = GetDocument();
	CLineDevice* pLine = pDoc->GetFirstLine();
	while (pLine != NULL)
	{
		CVRU* pVRU = dynamic_cast<CVRU*>(pLine);
		if (pVRU != NULL)
		{
			GetTreeCtrl().InsertItem(
				TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM,
				pVRU->GetLineName(),
				IMAGE_VRU_CLOSED2,
				IMAGE_VRU_OPEN2,
				0, 0, reinterpret_cast<LPARAM>(dynamic_cast<CUIObject*>(pVRU)), m_htiVRUs, NULL);
		}
		pLine = pDoc->GetNextLine();
	}

}// CJTViedw::LoadVrus

/*****************************************************************************
** Procedure:  CJTView::OnSelchanged
**
** Arguments: 'pNMHDR' - TREE view header
**            'pResult' - Returning result code
**
** Returns:    void
**
** Description: This notification is called when the selection changes
**              for the view.
**
*****************************************************************************/
void CJTView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CUIObject* pBase = reinterpret_cast<CUIObject*>(pNMTreeView->itemNew.lParam);

	// If the selection is moving to the root node, then display
	// the statistics panel.
	if (pBase == BO_NONE && m_pSelected != BO_NONE)
		dynamic_cast<CMainFrame*>(GetParentFrame())->SwitchToView(CMainFrame::GlobalStatsView);

	// If the selection is moving to any other node, then display
	// the appropriate listview panel.
	else if (pBase != BO_NONE && m_pSelected == BO_NONE)
		dynamic_cast<CMainFrame*>(GetParentFrame())->SwitchToView(CMainFrame::DetailView);

	// Mark the new display.
	m_pSelected = pBase;
	if (!IsHeader(pBase))
		pBase->Select();

	// Tell the other view
	GetDocument()->UpdateAllViews(this, HINT_SETCURRENTITEM, m_pSelected);

	*pResult = 0;

}// CJTView::OnSelchanged

/*****************************************************************************
** Procedure:  CJTView::OnDblclk
**
** Arguments: 'pNMHDR' - TREE view header
**            'pResult' - Returning result code
**
** Returns:    void
**
** Description: This notification is called when a selection is double-clicked
**
*****************************************************************************/
void CJTView::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	TV_ITEM tvItem;
	ZeroMemory(&tvItem, sizeof(TV_ITEM));
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = GetTreeCtrl().GetSelectedItem();
	if (GetTreeCtrl().GetItem(&tvItem))
	{
		// Show the properties of an individual item.
		CUIObject* pBase = reinterpret_cast<CUIObject*>(tvItem.lParam);
		if (!IsHeader(pBase))
			pBase->Edit(AfxGetMainWnd());
	}
	*pResult = 0;

}// CJTView::OnDblclk

/*****************************************************************************
** Procedure:  CJTView::OnRightclk
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
void CJTView::OnRightclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	POINT pt;
	GetCursorPos(&pt); 
    
	CRect rcTree;
	GetTreeCtrl().GetWindowRect(&rcTree); 

	TV_HITTESTINFO htInfo;
    htInfo.pt.x = pt.x - rcTree.left; 
    htInfo.pt.y = pt.y - rcTree.top; 
 
    // hittest to get the tree view item 
    HTREEITEM hItem = GetTreeCtrl().HitTest(&htInfo); 
    if (hItem != NULL && (htInfo.flags & TVHT_ONITEM)) 
		GetTreeCtrl().Select(hItem, TVGN_CARET);

	TV_ITEM tvItem;
	ZeroMemory(&tvItem, sizeof(TV_ITEM));
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = GetTreeCtrl().GetSelectedItem();
	if (GetTreeCtrl().GetItem(&tvItem))
	{
		// Show the properties of an individual item.
		CUIObject* pBase = reinterpret_cast<CUIObject*>(tvItem.lParam);
		if (!IsHeader(pBase))
			pBase->ContextMenu(AfxGetMainWnd());
	}

	*pResult = 0;

}// CJTView::OnRightclk

/*****************************************************************************
** Procedure:  CJTView::UpdateItem
**
** Arguments: 'pItem' - Item to locate and update
**
** Returns:    void
**
** Description: This locates a single item in our tree and updates it
**
*****************************************************************************/
void CJTView::UpdateItem(CObject* pItem)
{
	HTREEITEM hItem = FindItem(reinterpret_cast<CUIObject*>(pItem));
	if (hItem != NULL)
	{
		TV_ITEM tvItem;
		ZeroMemory(&tvItem, sizeof(TV_ITEM));
		tvItem.mask = TVIF_TEXT;
		tvItem.hItem = hItem;
		
		if (dynamic_cast<CLineDevice*>(pItem))
			tvItem.pszText = (LPSTR) (dynamic_cast<CLineDevice*>(pItem)->GetLineName());
		else if (dynamic_cast<CAgentGroup*>(pItem))
			tvItem.pszText = (LPSTR) (dynamic_cast<CAgentGroup*>(pItem)->GetGroupName());
		else if (dynamic_cast<CAgent*>(pItem))
			tvItem.pszText = (LPSTR) (dynamic_cast<CAgent*>(pItem)->GetAgentId());
		GetTreeCtrl().SetItem(&tvItem);
	}

}// CJTView::UpdateItem

/*****************************************************************************
** Procedure:  CJTView::FindItem
**
** Arguments: 'pItem' - Item to locate and update
**
** Returns:    HTREEITEM for item
**
** Description: This locates a single item in our tree and updates it
**
*****************************************************************************/
HTREEITEM CJTView::FindItem(CUIObject* pItem)
{
	// Take a chance - see if the current selected item is the item in
	// question.
	TV_ITEM tvItem;
	ZeroMemory(&tvItem, sizeof(TV_ITEM));
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = GetTreeCtrl().GetSelectedItem();
	if (GetTreeCtrl().GetItem(&tvItem) &&
		reinterpret_cast<CUIObject*>(tvItem.lParam) == pItem)
		return tvItem.hItem;

	// Quick & Easy failed, walk the tree looking for the item.
	HTREEITEM hItem = GetTreeCtrl().GetRootItem();
	while (hItem != NULL)
	{
		// Drop down through all the parent roots.
		while (GetTreeCtrl().ItemHasChildren(hItem))
			hItem = GetTreeCtrl().GetChildItem(hItem);

		// We are on a child, get the itemdata and check it
		tvItem.hItem = hItem;
		if (GetTreeCtrl().GetItem(&tvItem) &&
			reinterpret_cast<CUIObject*>(tvItem.lParam) == pItem)
			return tvItem.hItem;

		// Get the next item in the list - move up if necessary
		HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hItem);
		if (hNext == NULL)
		{
			hNext = GetTreeCtrl().GetParentItem(hItem);
			hItem = GetTreeCtrl().GetNextSiblingItem(hNext);
		}
		else
			hItem = hNext;
	}

	// Not found?
	return NULL;

}// CJTView::FindItem