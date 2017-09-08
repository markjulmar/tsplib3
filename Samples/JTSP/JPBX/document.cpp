/*******************************************************************/
//
// DOCUMENT.CPP
//
// Main storage area for JPBX data elements
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
#include <io.h>
#include "JPBX.h"
#include "document.h"
#include "agent.h"
#include "agentgroup.h"
#include "line.h"
#include "trunk.h"
#include "station.h"
#include "acdqueue.h"
#include "routepoint.h"
#include "pdialer.h"
#include "Interface.h"
#include "ivru.h"
#include "properties.h"
#include "gentraffic.h"

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
#define IDM_CTX_START	IDM_CTX_PROPERTIES
#define IDM_CTX_END		IDM_CTX_CALLS

/*---------------------------------------------------------------*/
// DYNAMIC RTTI SUPPORT
/*---------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CJTDocument, CDocument)

/*---------------------------------------------------------------*/
// CJTDocument message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CJTDocument, CDocument)
	//{{AFX_MSG_MAP(CJTDocument)
	ON_COMMAND(ID_ADD_AGENT, OnAddAgent)
	ON_COMMAND(ID_ADD_GROUP, OnAddGroup)
	ON_COMMAND_RANGE(IDM_CTX_START, IDM_CTX_END, OnCommand)
	ON_COMMAND(ID_ADD_ROUTEPOINT, OnAddRoutePoint)
	ON_COMMAND(ID_ADD_STATION, OnAddStation)
	ON_COMMAND(ID_ADD_TRUNK, OnAddTrunk)
	ON_COMMAND(ID_ADD_PDIALER, OnAddPdialer)
	ON_COMMAND(ID_ADD_IVRU, OnAddVRU)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_TOOLS_DIALER, OnToolsDialer)
	ON_COMMAND(ID_TOOLS_STATION, OnToolsStation)
	ON_COMMAND(ID_TOOLS_SOAKTEST, OnSoakTest)
	ON_COMMAND(ID_TOOLS_TRUNK, OnToolsTrunk)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  _STThreadEntry
**
** Arguments: 'pDoc' - Document pointer
**
** Returns:    int
**
** Description:  Thread entry function
**
*****************************************************************************/
static DWORD WINAPI _STThreadEntry(LPVOID lpvParam)
{
	CJTDocument* pDoc = reinterpret_cast<CJTDocument*>(lpvParam);
	pDoc->RunSoakTest();
	return 0;

}// _STThreadEntry

/*****************************************************************************
** Procedure:  CJTDocument::CJTDocument
**
** Arguments: void
**
** Returns:    void
**
** Description: Constructor for the document object
**
*****************************************************************************/
CJTDocument::CJTDocument() : 
	m_dwNextGroupID(0xa0010001), 
	m_dwViewStyle(LVS_ICON), 
	m_dwNextTrunkID(0),
	m_dwNextQueueID(8999),
	m_dwTreeStyle(0xffffffff),
	m_wPort(DEFAULT_IP_PORT),
	m_fRunningSoakTest(false),
	m_nSoakCPS(1),
	m_hThread_ST(0),
	m_fLogToDisk(false),
	m_strLogFile(_T(""))
{
	m_pcmConnections = new CConnectionMgr(this);
	srand((unsigned)time(NULL));

}// CJTDocument::CJTDocument

/*****************************************************************************
** Procedure:  CJTDocument::~CJTDocument
**
** Arguments: void
**
** Returns:    void
**
** Description: Destructor for the document object
**
*****************************************************************************/
CJTDocument::~CJTDocument()
{
	m_fRunningSoakTest = false;
	if (m_hThread_ST)
	{
		if (WaitForSingleObject(m_hThread_ST, 1000) == WAIT_TIMEOUT)
			TerminateThread(m_hThread_ST, 0);
	}

	DeleteContents();
	delete m_pcmConnections;

}// CJTDocument::~CJTDocument

/*****************************************************************************
** Procedure:  CJTDocument::Serialize
**
** Arguments: 'ar' - Archive to serialize data into
**
** Returns:    void
**
** Description: This function allows the document to support persistant data
**
*****************************************************************************/
void CJTDocument::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{
		ar << m_dwNextGroupID;
		ar << m_dwNextTrunkID;
		ar << m_dwNextQueueID;
		ar << m_dwViewStyle;
		ar << m_dwTreeStyle;
		ar << m_wPort;
		ar << m_fLogToDisk;
		ar << m_nSoakCPS;
		ar << m_strLogFile;

		m_arrAgentGroups.Serialize(ar);
		m_arrAgents.Serialize(ar);
		m_lstLines.Serialize(ar);
	}
	else
	{
		ar >> m_dwNextGroupID;
		ar >> m_dwNextTrunkID;
		ar >> m_dwNextQueueID;
		ar >> m_dwViewStyle;
		ar >> m_dwTreeStyle;
		ar >> m_wPort;
		ar >> m_fLogToDisk;
		ar >> m_nSoakCPS;
		ar >> m_strLogFile;

		m_arrAgentGroups.Serialize(ar);
		m_arrAgents.Serialize(ar);
		m_lstLines.Serialize(ar);

		// Setup our map for line devices
		for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
		{
			CLineDevice* pLine = dynamic_cast<CLineDevice*>(m_lstLines.GetNext(pos));
			m_mapLineDevices[pLine->GetLineID()] = pLine;
		}
	}

}// CJTDocument::Serialize

/*****************************************************************************
** Procedure:  CJTDocument::OnNewDocument
**
** Arguments: void
**
** Returns:    TRUE/FALSE if document was created successfully
**
** Description: Called by the framework as part of the File New command. 
**              The default implementation of this function calls the DeleteContents 
**              member function to ensure that the document is empty and then 
**              marks the new document as clean.
**
*****************************************************************************/
BOOL CJTDocument::OnNewDocument()
{
	CString strDataFile = GetDataPathName();
	if (_taccess(strDataFile,0) == 0)
	{
		// Read in our data file
		if (!CDocument::OnOpenDocument(strDataFile))
		{
			if (!OnNewDocument())
				return FALSE;
		}
	}
	else
	{
		AfxMessageBox(IDS_CONFIGURE);
		if (!CDocument::OnNewDocument())
			return FALSE;
	}
	return TRUE;

}// CJTDocument::OnNewDocument

/*****************************************************************************
** Procedure:  CJTDocument::DeleteContents
**
** Arguments: void
**
** Returns:    void
**
** Description: Deletes all the existing content in the document
**
*****************************************************************************/
void CJTDocument::DeleteContents()
{
	for (int i = 0; i < m_arrAgents.GetSize(); i++)
		delete m_arrAgents[i];
	for (i = 0; i < m_arrAgentGroups.GetSize(); i++)
		delete m_arrAgentGroups[i];
	for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
		delete m_lstLines.GetNext(pos);

	m_arrAgents.RemoveAll();
	m_arrAgentGroups.RemoveAll();
	m_lstLines.RemoveAll();
	m_mapLineDevices.RemoveAll();

}// CJTDocument::DeleteContents

/*****************************************************************************
** Procedure:  CJTDocument::OnCloseDocument
**
** Arguments: void
**
** Returns:    void
**
** Description: Called by the framework as part of the File Close command. 
**
*****************************************************************************/
void CJTDocument::OnCloseDocument() 
{
	// Tell all our views to save their states
	UpdateAllViews(NULL, HINT_DOCCLOSING);

	// Save the document out to disk
	SetModifiedFlag();
	OnSaveDocument(GetDataPathName());

	// Close the document	
	CDocument::OnCloseDocument();

}// CJTDocument::OnCloseDocument

/*****************************************************************************
** Procedure:  CJTDocument::GetAgentGroupList
**
** Arguments: 'pAgent' - Agent to retrieve all the groups for
**
** Returns:    String with all the groups comma-delimited
**
** Description: This function is used to collect all the groups an agent 
**              can log into.
**
*****************************************************************************/
CString CJTDocument::GetAgentGroupList(CAgent* pAgent)
{
	CString strGroups = _T("");
	for (int i = 0; i < GetAgentGroupCount(); i++)
	{
		CAgentGroup* pGroup = GetAgentGroup(i);
		if (pGroup->IsAgentInGroup(pAgent))
		{
			if (!strGroups.IsEmpty())
				strGroups += _T(",");
			strGroups += pGroup->GetGroupName();
		}
	}
	if (strGroups.IsEmpty())
		strGroups = _T("None");
	return strGroups;

}// CJTDocument::GetAgentGroupList

/*****************************************************************************
** Procedure:  CJTDocument::GetAgentsFromGroup
**
** Arguments: 'pGroup' - Group to collect all agent names for
**
** Returns:    String with all the groups comma-delimited
**
** Description: This function is used to collect all the agents which may
**              log into a specific group.
**
*****************************************************************************/
CString CJTDocument::GetAgentsFromGroup(CAgentGroup* pGroup)
{
	CString strAgents = _T("");
	for (int i = 0; i < GetAgentCount(); i++)
	{
		CAgent* pAgent = GetAgent(i);
		if (pGroup->IsAgentInGroup(pAgent))
		{
			if (!strAgents.IsEmpty())
				strAgents += _T(",");
			strAgents += pAgent->GetAgentId();
		}
	}
	if (strAgents.IsEmpty())
		strAgents = _T("None");
	return strAgents;

}// CJTDocument::GetAgentsFromGroup

/*****************************************************************************
** Procedure:  CJTDocument::OnAddAgent
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_AGENT menu option and
**              creates a new agent object
**
*****************************************************************************/
void CJTDocument::OnAddAgent() 
{
	CAgent* pAgent = CAgent::Create(AfxGetMainWnd(), this);
	if (pAgent != NULL)
	{
		m_arrAgents.Add(pAgent); 
		UpdateAllViews(NULL, HINT_RELOADAGENT);
	}

}// CJTDocument::OnAddAgent

/*****************************************************************************
** Procedure:  CJTDocument::OnAddGroup
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_GROUP menu option and
**              creates a new agent group object
**
*****************************************************************************/
void CJTDocument::OnAddGroup() 
{
	CAgentGroup* pGroup = CAgentGroup::Create(AfxGetMainWnd(), this);
	if (pGroup != NULL)
	{
		m_arrAgentGroups.Add(pGroup); 
		UpdateAllViews(NULL, HINT_RELOADGROUPS);
	}

}// CJTDocument::OnAddGroup

/*****************************************************************************
** Procedure:  CJTDocument::OnAddRoutePoint
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_RPOINT menu option and
**              creates a new route point
**
*****************************************************************************/
void CJTDocument::OnAddRoutePoint() 
{
	CRoutePoint* pRP = CRoutePoint::Create(AfxGetMainWnd(), this);
	if (pRP != NULL)
		UpdateAllViews(NULL, HINT_RELOADRPOINTS);

}// CJTDocument::OnAddRoutePoint

/*****************************************************************************
** Procedure:  CJTDocument::OnAddPDialer
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_PDIALER menu option and
**              creates a new predictive dialer
**
*****************************************************************************/
void CJTDocument::OnAddPdialer() 
{
	CPDialer* pDialer = CPDialer::Create(AfxGetMainWnd(), this);
	if (pDialer != NULL)
		UpdateAllViews(NULL, HINT_RELOADPDIALERS);

}// CJTDocument::OnAddPdialer

/*****************************************************************************
** Procedure:  CJTDocument::OnAddStation
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_STATION menu option and
**              creates a new station line
**
*****************************************************************************/
void CJTDocument::OnAddStation() 
{
	CStation* pLine = CStation::Create(AfxGetMainWnd(), this);
	if (pLine != NULL)
		UpdateAllViews(NULL, HINT_RELOADSTATION);

}// CJTDocument::OnAddStation

/*****************************************************************************
** Procedure:  CJTDocument::OnAddTrunk
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_GROUP menu option and
**              creates a new trunk
**
*****************************************************************************/
void CJTDocument::OnAddTrunk() 
{
	CTrunk* pLine = CTrunk::Create(AfxGetMainWnd(), this);
	if (pLine != NULL)
		UpdateAllViews(NULL, HINT_RELOADTRUNKS);

}// CJTDocument::OnAddTrunk

/*****************************************************************************
** Procedure:  CJTDocument::OnAddVRU
**
** Arguments: void
**
** Returns:    void
**
** Description: This handler catches the IDM_ADD_VRU menu option and
**              creates a new VRU
**
*****************************************************************************/
void CJTDocument::OnAddVRU() 
{
	CVRU* pVRU = CVRU::Create(AfxGetMainWnd(), this);
	if (pVRU != NULL)
		UpdateAllViews(NULL, HINT_RELOADIVRS);

}// CJTDocument::OnAddVRU

/*****************************************************************************
** Procedure:  CJTDocument::DeleteSingleObject
**
** Arguments: 'pBase' - Object to delete
**
** Returns:    void
**
** Description: This function removes an object from the appopriate array
**              and deletes the object.  It then sends an UpdateView event
**              so the UI is changed.
**
*****************************************************************************/
void CJTDocument::DeleteSingleObject(CUIObject* pBase)
{
	// If this is an agent..
	if (dynamic_cast<CAgent*>(pBase) != NULL)
	{
		CAgent* pAgent = dynamic_cast<CAgent*>(pBase);

		// Remove the agent from any groups it belongs to.
		for (int i = 0; i < GetAgentGroupCount(); i++)
			GetAgentGroup(i)->RemoveAgent(pAgent);

		// Remove the agent
		for (i = 0; i < m_arrAgents.GetSize(); i++)
		{
			if (m_arrAgents[i] == pAgent)
			{
				m_arrAgents.RemoveAt(i);
				delete pAgent;
				UpdateAllViews(NULL, HINT_RELOADAGENT);
			}
		}
	}

	// If this is an agent group..
	else if (dynamic_cast<CAgentGroup*>(pBase) != NULL)
	{
		CAgentGroup* pGroup = dynamic_cast<CAgentGroup*>(pBase);
		for (int i = 0; i < m_arrAgentGroups.GetSize(); i++)
		{
			if (m_arrAgentGroups[i] == pGroup)
			{
				CACDQueue* pQueue = pGroup->GetRelatedQueue();
				m_arrAgentGroups.RemoveAt(i);
				delete pGroup;
				delete pQueue;
				UpdateAllViews(NULL, HINT_RELOADGROUPS);
				UpdateAllViews(NULL, HINT_RELOADQUEUES);
			}
		}
	}


}// CJTDocument::DeleteSingleObject

/*****************************************************************************
** Procedure:  CJTDocument::FindAgentGroup
**
** Arguments: 'id' - Unique agent group identifier
**
** Returns:    void
**
** Description: This function locates an agent group by id.
**
*****************************************************************************/
CAgentGroup* CJTDocument::FindAgentGroup(DWORD id)
{
	for (int i = 0; i < GetAgentGroupCount(); i++)
	{
		CAgentGroup* pGroup = GetAgentGroup(i);
		if (id == pGroup->GetGroupID())
			return pGroup;
	}
	return NULL;

}// CJTDocument::FindAgentGroup

/*****************************************************************************
** Procedure:  CJTDocument::FindAgent
**
** Arguments: 'pszAgent' - Unique agent identifier
**
** Returns:    void
**
** Description: This function locates an agent id by name
**
*****************************************************************************/
CAgent* CJTDocument::FindAgent(LPCTSTR pszAgentID)
{
	for (int i = 0; i < GetAgentCount(); i++)
	{
		CAgent* pAgent = GetAgent(i);
		if (!lstrcmpi(pAgent->GetAgentId(), pszAgentID))
			return pAgent;
	}
	return NULL;

}// CJTDocument::FindAgent

/*****************************************************************************
** Procedure:  CJTDocument::GetServerName
**
** Arguments: void
**
** Returns:    String of current host name (TCP/IP)
**
** Description: This function returns the server name
**
*****************************************************************************/
CString CJTDocument::GetServerName() const
{
	CString strComputerName;
	if (strComputerName.IsEmpty())
	{
		char* pszComputer = strComputerName.GetBuffer(MAX_COMPUTERNAME_LENGTH + 1);
		DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
		::GetComputerName(pszComputer, &dwSize);
		strComputerName.ReleaseBuffer();
	}
	return strComputerName;

}// CJTDocument::GetServerName

/*****************************************************************************
** Procedure:  CJTDocument::GetDataPathName
**
** Arguments: void
**
** Returns:    Data file name
**
** Description: This function generates the pathname for the persistant
**              data file.
**
*****************************************************************************/
CString CJTDocument::GetDataPathName() const
{
	CString strIni;
	strIni.LoadString(IDS_INIFILE);

	// First get our local directory
	TCHAR chBuff[_MAX_PATH];
	if (::GetModuleFileName(NULL, chBuff, _MAX_PATH) != 0)
	{
		// Strip off the filename portion and add our .INI file name
		TCHAR* pszSep = _tcsrchr(chBuff, _T('\\'));
		if (pszSep != NULL)
		{
			pszSep++;
			*pszSep = _T('\0');
			strIni = CString(chBuff) + strIni;
		}
	}
	return strIni;

}// CJTDocument::GetDataPathName

/*****************************************************************************
** Procedure:  CJTDocument::OnCommand
**
** Arguments: 'nCommand' - Command to process
**
** Returns:    void
**
** Description: This function reroutes the command architecture through our
**              ui objects
**
*****************************************************************************/
void CJTDocument::OnCommand(UINT nCommand)
{
	CUIObject* pSelected = CUIObject::GetSelectedObject();
	if (pSelected != NULL && pSelected > BO_IVRUNITS)
		pSelected->OnCommand(nCommand);

}// CJTDocument::OnCommand

/*****************************************************************************
** Procedure:  CJTDocument::GetFirstQueue
**
** Arguments: 'pAgent' - Agent to locate queue for
**
** Returns: Pointer to ACD Queue which agent is logged into
**
** Description: This method locates a queue to transfer calls into
**
*****************************************************************************/
CACDQueue* CJTDocument::GetFirstQueue(CAgent* pAgent)
{
	for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
	{
		CACDQueue* pQueue = dynamic_cast<CACDQueue*>(m_lstLines.GetNext(pos));
		if (pQueue != NULL && pQueue->IsAgentInQueue(pAgent))
			return pQueue;
	}
	return NULL;

}// CJTDocument::GetFirstQueue

/*****************************************************************************
** Procedure:  CJTDocument::GetNextAvailableAgent
**
** Arguments: 'pGroup' - Group to check for
**
** Returns: Pointer to agent which has been idle the longest
**
** Description: This method locates a READY agent for a queue
**
*****************************************************************************/
CAgent* CJTDocument::GetNextAvailableAgent(CAgentGroup* pGroup)
{
	CACDQueue* pQueue = pGroup->GetRelatedQueue();
	if (pQueue != NULL)
	{
		for (POSITION pos = m_lstFreeAgents.GetHeadPosition(); pos != NULL;)
		{
			CAgent* pAgent = m_lstFreeAgents.GetNext(pos);
			if (pQueue->IsAgentInQueue(pAgent))
				return pAgent;
		}
	}
	return NULL;

}// CJTDocument::GetNextAvailableAgent

/*****************************************************************************
** Procedure:  CJTDocument::StringToDN
**
** Arguments: 'pszExtension' - Dialable number
**
** Returns: CString with numeric extension 
**
** Description: This method converts a string to a dialable number
**
*****************************************************************************/
CString CJTDocument::StringToDN(LPCTSTR pszNumber)
{
	CString strExtension;
	while (*pszNumber)
	{
		if (_istdigit(*pszNumber))
			strExtension += *pszNumber;
		pszNumber++;
	}

	// Now if the extension is invalid, return NULL
	if ((strExtension.GetLength() < 4) ||
		(strExtension.GetLength() > 4 && strExtension[0] != _T('9')))
		strExtension.Empty();

	return strExtension;

}// CJTDocument::StringToDN

/*****************************************************************************
** Procedure:  CJTDocument::FindLineByExtension
**
** Arguments: 'strExtension' - Dialable number
**
** Returns: Pointer to line device which is destination for dialable number
**
** Description: This method locates a line device by number
**
*****************************************************************************/
CLineDevice* CJTDocument::FindLineByExtension(CString& strExtension)
{
	// Convert the string - if it fails, return null.
	strExtension = StringToDN(strExtension);
	if (strExtension.IsEmpty())
		return NULL;

	// If the extension is 4 digits, it must be real line
	if (strExtension.GetLength() == 4)
		return GetLineByID(atol(strExtension));

	// Find an available trunk line.
	return FindAvailableTrunk();

}// CJTDocument::FindLineByExtension

/*****************************************************************************
** Procedure:  CJTDocument::FindAvailableTrunk
**
** Arguments: void
**
** Returns: Pointer to an unused trunk line
**
** Description: This returns the first trunk which is not allocated
**
*****************************************************************************/
CTrunk* CJTDocument::FindAvailableTrunk()
{
	for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
	{
		CLineDevice* pLine = dynamic_cast<CLineDevice*>(m_lstLines.GetNext(pos));
		CTrunk* pTrunk = dynamic_cast<CTrunk*>(pLine);
		if (pTrunk != NULL)
		{
			if (pTrunk->GetTrunkState() == CTrunk::Unused)
				return pTrunk;
		}
	}
	return NULL;

}// CJTDocument::FindAvailableTrunk

/*****************************************************************************
** Procedure:  CJTDocument::OnTrunkStateChanged
**
** Arguments: 'pTrunk' - Trunk whose state has changed
**
** Returns: void
**
** Description: This event is called by the trunk object when the state
**              changes.
**
*****************************************************************************/
void CJTDocument::OnTrunkStateChanged(CTrunk* pTrunk)
{
	// Notify the views
	UpdateAllViews(NULL, HINT_ITEMCHANGED, pTrunk);

	// Now cycle through the route points and see if any of them elect to
	// manage this trunk call.
	if (pTrunk->GetTrunkState() == CTrunk::Inbound)
	{
		CCall* pCall = pTrunk->GetCallByPosition(0);
		if (pCall != NULL)
		{
			for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
			{
				CLineDevice* pLine = dynamic_cast<CLineDevice*>(m_lstLines.GetNext(pos));
				CRoutePoint* pRP = dynamic_cast<CRoutePoint*>(pLine);
				if (pRP && pRP->WantIncomingTrunkCall(pTrunk, pCall))
				{
					pRP->OnReceiveCall(pCall);
					//if (pRP->TransferCall(NULL, pCall))
						break;
				}
			}
		}
	}

}// CJTDocument::OnTrunkStateChanged

/*****************************************************************************
** Procedure:  CJTDocument::NotifyOnLine
**
** Arguments: 'pLine' - Line which has changed
**
** Returns: void
**
** Description: This event is called when a line changes to notify the 
**              view
**
*****************************************************************************/
void CJTDocument::NotifyOnLine(CLineDevice* pLine)
{
	UpdateAllViews(NULL, HINT_ITEMCHANGED, pLine);

}// CJTDocument::NotifyOnLine

/*****************************************************************************
** Procedure:  CJTDocument::OnTimer
**
** Arguments: void
**
** Returns: void
**
** Description: This event is called peridically for timer handling
**
*****************************************************************************/
void CJTDocument::OnTimer()
{
	// Every second update our statistics view
	static DWORD g_dwTickCount = 0;
	if (GetTickCount() > g_dwTickCount+1000)
	{
		UpdateAllViews(NULL, HINT_CONNCHANGE);
		g_dwTickCount = GetTickCount();
	}

	// Run through and give the lines a timer
	for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
	{
		CLineDevice* pLine = dynamic_cast<CLineDevice*>(m_lstLines.GetNext(pos));
		if (pLine)
			pLine->OnTimer();
	}

}// CJTDocument::OnTimer

/*****************************************************************************
** Procedure:  CJTDocument::OnEditProperties
**
** Arguments: void
**
** Returns: void
**
** Description: This event shows the global properties
**
*****************************************************************************/
void CJTDocument::OnEditProperties() 
{
	CPbxPropertiesDlg dlg(AfxGetMainWnd());
	dlg.m_nPort = m_wPort;
	dlg.m_fLog = m_fLogToDisk;
	dlg.m_strFileName = m_strLogFile;

	if (dlg.DoModal() == IDOK)
	{
		m_wPort = static_cast<WORD>(dlg.m_nPort);
		m_fLogToDisk = (dlg.m_fLog == TRUE);
		m_strLogFile = dlg.m_strFileName;
	}

}// CJTDocument::OnEditProperties

/*****************************************************************************
** Procedure:  CJTDocument::OnToolsDialer
**
** Arguments: void
**
** Returns: void
**
** Description: This starts the Windows Dialer
**
*****************************************************************************/
void CJTDocument::OnToolsDialer() 
{
	StartProgram(_T("DIALER.EXE"));

}// CJTDocument::OnToolsDialer

/*****************************************************************************
** Procedure:  CJTDocument::OnToolsStation
**
** Arguments: void
**
** Returns: void
**
** Description: This starts the PBX soft-station
**
*****************************************************************************/
void CJTDocument::OnToolsStation() 
{
	StartProgram(_T("JPSTATION.EXE"));

}// CJTDocument::OnToolsStation

/*****************************************************************************
** Procedure:  CJTDocument::OnToolsTrunk
**
** Arguments: void
**
** Returns: void
**
** Description: This starts the PBX trunk
**
*****************************************************************************/
void CJTDocument::OnToolsTrunk() 
{
	StartProgram(_T("JPTRUNK.EXE"));

}// CJTDocument::OnToolsTrunk

/*****************************************************************************
** Procedure:  CJTDocument::StartProgram
**
** Arguments: 'strProgram' - Program to spawn
**
** Returns: void
**
** Description: This function spawns a program from our PBX
**
*****************************************************************************/
void CJTDocument::StartProgram(LPCTSTR pszProgram)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	if (!CreateProcess(NULL, (char*)pszProgram, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, 
			NULL, NULL, &si, &pi))
	{
		CString strBuff, strTitle;
		strBuff.LoadString(IDS_LOADFAILED);
		strTitle.Format(strBuff, pszProgram);
		AfxMessageBox(strTitle);
	}

}// CJTDocument::StartProgram

/*****************************************************************************
** Procedure:  CJTDocument::OnSoakTest
**
** Arguments: void
**
** Returns: void
**
** Description: This event is called for the "Tools/SoakTest" menu option
**
*****************************************************************************/
void CJTDocument::OnSoakTest() 
{
	CGenTraffic dlg(AfxGetMainWnd());
	dlg.m_fRunning = m_fRunningSoakTest;
	dlg.m_nCPS = m_nSoakCPS;

	if (dlg.DoModal() == IDOK)
	{
		m_fRunningSoakTest = !m_fRunningSoakTest;
		m_nSoakCPS = dlg.m_nCPS;
		if (m_fRunningSoakTest)
		{
			if (m_hThread_ST == 0)
			{
				DWORD dwThreadID;
				m_hThread_ST = CreateThread(NULL, 0, _STThreadEntry, 
					static_cast<void*>(this), 0, &dwThreadID);
			}
		}
	}

}// CJTDocument::OnSoakTest

/*****************************************************************************
** Procedure:  CJTDocument::RunSoakTest
**
** Arguments: void
**
** Returns: void
**
** Description: This is a function for a secondary thread which runs a soaktest
**
*****************************************************************************/
void CJTDocument::RunSoakTest()
{
	while (m_fRunningSoakTest)
	{
		// Generate a call
		AfxGetMainWnd()->SendMessage(UM_SOAKTEST);

		// Sleep for our period
		Sleep((1000/m_nSoakCPS));
	}

	m_hThread_ST = 0;

}// CJTDocument::RunSoakTest

/*****************************************************************************
** Procedure:  CJTDocument::GenSoakCall
**
** Arguments: void
**
** Returns: void
**
** Description: This is called when the UM_SOAKTEST message is sent through
**
*****************************************************************************/
void CJTDocument::GenSoakCall()
{
	if (!m_fRunningSoakTest)
		return;

	// Create an incoming trunk call
	CTrunk* pTrunk = FindAvailableTrunk();
	if (pTrunk != NULL)
	{
		// Invent some CID
		CNetInfo cidCaller(MakeUpName(), MakeUpNumber());
		CNetInfo cidCalled(NULL, MakeUpNumber());
		pTrunk->GenerateIncomingCall(cidCaller, cidCalled);
	}

}// CJTDocument::GenSoakCall

/*****************************************************************************
** Procedure:  CJTDocument::MakeUpName
**
** Arguments: void
**
** Returns: CString name for callerid
**
** Description: This worker makes up a new name for a caller
**
*****************************************************************************/
CString CJTDocument::MakeUpName() const
{
	static const char* szFirst[] = {
		{"Mark"}, {"Julie"}, {"David" }, {"Brenda"},
		{"Adam"}, {"Maria"}, {"Patti" }, {"John"},
		{"Neil"}, {"Patricia"}, {"Bill"}, {"Ron"}, {"Paula"}, {""}
	};
	static const char* szLast[] = {
		{"Smith"}, {"Hayes"}, {"Peterson"}, {"Franklin"},
		{"Mullen"}, {"Elliot"}, {"Feder"}, {"Johnson"}, {"Anderson"},
		{"Miller"}
	};

	// Get a random first & last name
	int iFirstName = rand()%14;
	int iLastName = rand()%10;

	CString strName = szFirst[iFirstName];
	if (!strName.IsEmpty())
	{
		strName += _T(" ");
		strName += szLast[iLastName];
	}
	return strName;
		
}// CJTDocument::MakeUpName

/*****************************************************************************
** Procedure:  CJTDocument::MakeUpNumber
**
** Arguments: void
**
** Returns: CString name for callerid
**
** Description: This worker makes up a new number for a caller
**
*****************************************************************************/
CString CJTDocument::MakeUpNumber() const
{
	CString strNumber;
	for (int i = 0; i < 10; i++)
	{
		int iNumber = rand()%10;
		strNumber += (char)('0' + iNumber);
	}
	return strNumber;

}// CJTDocument::MakeUpNumber

/*****************************************************************************
** Procedure:  CJTDocument::AppendToLog
**
** Arguments: 'pszBuff' - String to add
**
** Returns: void
**
** Description: This method adds a string to our log file
**
*****************************************************************************/
void CJTDocument::AppendToLog(char cType, LPCTSTR pszBuff)
{
	CString strBuff(_T(' '), lstrlen(pszBuff)+2);
	strBuff = cType;
	strBuff += pszBuff;
	
	// If this line is already in the log (last line) then ignore it.
	if (m_arrConnBuffer.GetSize() > 0)
	{
		if (m_arrConnBuffer.GetAt(m_arrConnBuffer.GetUpperBound()) == strBuff)
			return;
	}

	// Otherwise add it
	if (m_arrConnBuffer.Add(strBuff) > 200)
	{
		while (m_arrConnBuffer.GetSize() > 200)
			m_arrConnBuffer.RemoveAt(0);
	}

	// Inform our views
	if (AfxGetMainWnd() != NULL)
		AfxGetMainWnd()->SendMessage(UM_UPDATESTATS);

	// If logging is enabled, then add the line to the log.
	if (m_fLogToDisk)
	{
		CFile fileLog;
		if (fileLog.Open(m_strLogFile, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone), NULL))
		{
			strBuff = CTime::GetCurrentTime().Format(_T("%m/%d %H:%M:%S ")) + strBuff.Mid(1) + CString(_T("\r\n"));
			fileLog.Seek(0, CFile::end);
			fileLog.Write(strBuff, strBuff.GetLength());
			fileLog.Close();
		}
	}

}// CJTDocument::AppendToLog

/*****************************************************************************
** Procedure:  CJTDocument::OnFileExport
**
** Arguments: void
**
** Returns: void
**
** Description: This method exports the switch configuration to a file
**
*****************************************************************************/
void CJTDocument::OnFileExport() 
{
	// Get the filename
	static TCHAR BASED_CODE szFilter[] = _T("Config Files (*.cfg)|*.cfg|All Files (*.*)|*.*||");
	CFileDialog fd(FALSE, _T("CFG"), _T("JPBX.CFG"), 
			OFN_ENABLESIZING | OFN_EXPLORER | OFN_PATHMUSTEXIST,
			szFilter, AfxGetMainWnd());

	if (fd.DoModal() == IDOK)
	{
		CStdioFile fileCfg;
		if (fileCfg.Open(fd.GetPathName(), (CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText), NULL))
		{
			CString strBuff, strSep('-', 60), strCR("\n");
			strSep += _T("\n");

			// Write our header
			strBuff.Format(_T("JPBX SYSTEM CONFIGURATION %s\n"), CTime::GetCurrentTime().Format(_T("%x %X")));
			fileCfg.WriteString(strBuff);
			fileCfg.WriteString(strSep);
			fileCfg.WriteString(strCR);

			// Write the number of agents
			strBuff.Format(_T("AGENTS:%d\n"), GetAgentCount());
			fileCfg.WriteString(strBuff);
			for (int i = 0; i < GetAgentCount(); i++)
			{
				strBuff = GetAgent(i)->GetAgentId() + CString(_T("\n"));
				fileCfg.WriteString(strBuff);
			}
			fileCfg.WriteString(strCR);

			// Write the number of groups
			strBuff.Format(_T("GROUPS:%d\n"), GetAgentGroupCount());
			fileCfg.WriteString(strBuff);
			for (i = 0; i < GetAgentGroupCount(); i++)
			{
				CAgentGroup* pGroup = GetAgentGroup(i);
				strBuff.Format(_T("%lx,%s\n"), pGroup->GetGroupID(), pGroup->GetGroupName());
				fileCfg.WriteString(strBuff);
			}
			fileCfg.WriteString(strCR);

			// Write the number of lines
			strBuff.Format(_T("DEVICES:%d\n"), m_lstLines.GetCount());
			fileCfg.WriteString(strBuff);
			for (POSITION pos = m_lstLines.GetHeadPosition(); pos != NULL;)
			{
				CLineDevice* pLine = reinterpret_cast<CLineDevice*>(m_lstLines.GetNext(pos));
				ASSERT (pLine != NULL);
				char cType = 'U';
				if (dynamic_cast<CStation*>(pLine) != NULL)
					cType = 'S';
				else if (dynamic_cast<CACDQueue*>(pLine) != NULL)
					cType = 'Q';
				else if (dynamic_cast<CRoutePoint*>(pLine) != NULL)
					cType = 'R';
				else if (dynamic_cast<CPDialer*>(pLine) != NULL)
					cType = 'P';
				else if (dynamic_cast<CVRU*>(pLine) != NULL)
					cType = 'V';
				else if (dynamic_cast<CTrunk*>(pLine) != NULL)
					cType = 'T';

				ASSERT (cType != 'U');
				strBuff.Format(_T("%c,%04d,%s\n"), cType, pLine->GetLineID(), pLine->GetLineName());
				fileCfg.WriteString(strBuff);
			}
			fileCfg.WriteString(strCR);

			fileCfg.Close();
		}
	}


}// CJTDocument::OnFileExport

/*****************************************************************************
** Procedure:  CJTDocument::OnFileImport
**
** Arguments: void
**
** Returns: void
**
** Description: This method exports the switch configuration to a file
**
*****************************************************************************/
void CJTDocument::OnFileImport() 
{
	// If we have existing information, warn the user.
	if (m_lstLines.GetCount() > 0 ||
		m_arrAgentGroups.GetSize() > 0 ||
		m_arrAgents.GetSize() > 0)
	{
		if (AfxMessageBox(IDS_REMOVEALL, MB_YESNO) == IDNO)
			return;
	}

	// Prompt for a filename
	static TCHAR BASED_CODE szFilter[] = _T("Config Files (*.cfg)|*.cfg|All Files (*.*)|*.*||");
	CFileDialog fd(TRUE, _T("CFG"), _T("JPBX.CFG"), 
			OFN_ENABLESIZING | OFN_EXPLORER | OFN_PATHMUSTEXIST,
			szFilter, AfxGetMainWnd());

	// Show the dialog and if the user clicks OK, open the file and read the 
	// configuration directly.
	if (fd.DoModal() == IDOK)
	{
		CStdioFile fileCfg;
		if (fileCfg.Open(fd.GetPathName(), (CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText), NULL))
		{
			// Delete existing information
			DeleteContents();
		
			// Read each line and parse all the information from it.
			CString strLine;
			int iStep = 0;
			while (fileCfg.ReadString(strLine))
			{
				strLine.TrimRight();
				if (strLine.Left(7) == _T("GROUPS:"))
					iStep = 1;
				else if (strLine.Left(8) == _T("DEVICES:"))
					iStep = 2;
				else if (strLine.Left(7) == _T("AGENTS:"))
					iStep = 3;
				else if (strLine.IsEmpty())
					iStep = 0;

				// If we are loading groups
				else if (iStep == 1)
				{
					int iPos = strLine.Find(_T(','));
					if (iPos <= 0)
						iStep = 0;
					else
					{
						DWORD dwGroupID;
						sscanf(strLine.Left(iPos), _T("%lx"), &dwGroupID);
						if (dwGroupID > 0)
							m_arrAgentGroups.Add(new CAgentGroup(dwGroupID, strLine.Mid(iPos+1)));
					}
				}

				// Else if we are loading devices (lines)
				else if (iStep == 2)
				{
					int iPos = strLine.Find(_T(','));
					if (iPos <= 0)
						iStep = 0;
					else
					{
						CString strType = strLine.Left(iPos);
						strLine = strLine.Mid(iPos+1);
						iPos = strLine.Find(_T(','));
						if (iPos > 0)
						{
							CLineDevice* pDevice = NULL;
							DWORD dwLineID = atol(strLine.Left(iPos));
							CString strName = strLine.Mid(iPos+1);
							switch (strType[0])
							{
								case 'R': pDevice = new CRoutePoint(dwLineID, strName); break;
								case 'P': pDevice = new CPDialer(dwLineID, strName); break;
								case 'T': pDevice = new CTrunk(dwLineID); break;
								case 'S': pDevice = new CStation(dwLineID, strName); break;
								case 'V': pDevice = new CVRU(dwLineID, strName); break;
								case 'Q': 
									{
										for (int i = 0; i < m_arrAgentGroups.GetSize(); i++)
										{
											CAgentGroup* pGroup = m_arrAgentGroups[i];
											if (!lstrcmpi(pGroup->GetGroupName(), strName))
											{
												pDevice = new CACDQueue(pGroup, dwLineID);
												break;
											}
										}
									}
									break;
								default:  ASSERT(FALSE); break;
							}
						}
					}
				}
				// Else if we are loading agents
				else if (iStep == 3)
				{
					if (atol(strLine) != 0)
					{
						CAgent* pAgent = new CAgent(strLine);
						m_arrAgents.Add(pAgent);
					}
				}
			}

			// Close the file and exit.
			fileCfg.Close();

			UpdateAllViews(NULL, HINT_RELOADAGENT);
			UpdateAllViews(NULL, HINT_RELOADSTATION);
			UpdateAllViews(NULL, HINT_RELOADGROUPS);
			UpdateAllViews(NULL, HINT_RELOADTRUNKS);
			UpdateAllViews(NULL, HINT_RELOADQUEUES);
			UpdateAllViews(NULL, HINT_RELOADPDIALERS);
			UpdateAllViews(NULL, HINT_RELOADRPOINTS);
			UpdateAllViews(NULL, HINT_RELOADIVRS);
		}
		else
			AfxMessageBox(IDS_AUTOLOADERR);
	}

}// CJTDocument::OnFileImport

