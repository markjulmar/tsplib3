/*******************************************************************/
//
// DOCUMENT.H
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

#ifndef __DOCUMENT_INC__
#define __DOCUMENT_INC__

/*----------------------------------------------------------------------------
	PRE-DECLARATIONS
-----------------------------------------------------------------------------*/
class CAgent;
class CAgentGroup;
class CTrunk;
class COMMANDBLOCK;
class CACDQueue;

/*----------------------------------------------------------------------------
	INCLUDES
-----------------------------------------------------------------------------*/
#include "line.h"
#include "trunk.h"

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
// Internal message used between threads
#define UM_PROCDATA		(WM_USER+1)
#define UM_UPDATESTATS	(WM_USER+2)
#define UM_SOAKTEST     (WM_USER+3)

// Hints passed around to views in the program
const UINT HINT_SETCURRENTITEM	= 1;
const UINT HINT_RELOADAGENT		= 2;
const UINT HINT_RELOADSTATION	= 3;
const UINT HINT_RELOADGROUPS	= 4;
const UINT HINT_RELOADTRUNKS	= 5;
const UINT HINT_RELOADQUEUES	= 6;
const UINT HINT_RELOADPDIALERS	= 7;
const UINT HINT_RELOADRPOINTS	= 8;
const UINT HINT_RELOADIVRS		= 9;
const UINT HINT_DOCCLOSING		= 10;
const UINT HINT_CONNBUFFCHANGE	= 11;
const UINT HINT_CONNCHANGE      = 12;
const UINT HINT_ITEMCHANGED     = 13;

// Log types
const char LT_INFO = '0';
const char LT_SENT = '1';
const char LT_RECV = '2';

// Default IP port
const WORD DEFAULT_IP_PORT = 4020;

// Tree nodes defined as data
#define BO_NONE		((CUIObject*)0)
#define BO_AGENTS	((CUIObject*)1)
#define BO_GROUPS	((CUIObject*)2)
#define BO_STATIONS ((CUIObject*)3)
#define BO_TRUNKS   ((CUIObject*)4)
#define BO_RPOINTS  ((CUIObject*)5)
#define BO_QUEUES   ((CUIObject*)6)
#define BO_PDIALERS ((CUIObject*)7)
#define BO_IVRUNITS ((CUIObject*)8)

/**************************************************************************
** CJTDocument
**
** Main storage document for the PBX application
**
***************************************************************************/
class CJTDocument : public CDocument
{
// Class data
protected:
	CConnectionMgr* m_pcmConnections;
	CTypedPtrArray<CObArray, CAgent*> m_arrAgents;
	CTypedPtrArray<CObArray, CAgentGroup*> m_arrAgentGroups;
	CTypedPtrArray<CObList, CLineDevice*> m_lstLines;
	CList<CAgent*, CAgent*> m_lstFreeAgents;
	CMap<DWORD, DWORD, CLineDevice*, CLineDevice*> m_mapLineDevices;
	DWORD m_dwNextGroupID;
	DWORD m_dwNextTrunkID;
	DWORD m_dwNextQueueID;
	DWORD m_dwViewStyle;
	DWORD m_dwTreeStyle;
	POSITION m_posLines;
	CStringArray m_arrConnBuffer;
	WORD m_wPort;
	HANDLE m_hThread_ST;
	BOOL m_fLogToDisk;
	bool m_fRunningSoakTest;
	int m_nSoakCPS;
	CString m_strLogFile;

protected: // create from serialization only
	CJTDocument();
	DECLARE_DYNCREATE(CJTDocument)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJTDocument)
	public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJTDocument();
	CString GetServerName() const;
	CString GetDataPathName() const;
	DWORD GetViewStyle() const;
	void SetViewStyle(DWORD dwViewStyle);
	DWORD GetTreeStyle() const;
	void SetTreeStyle(DWORD dwTreeStyle);

	// Connection buffer
	void AppendToLog(char cType, LPCTSTR pszBuff);
	void AppendToLog(char cType, UINT uidString);
	void ClearLog();
	CStringArray& GetLogBuffer();

	// TCP/IP port support
	CConnectionMgr& GetConnectionMgr();
	WORD GetIPPort() const;

	// Line map support
	void OnCreateLine(CLineDevice* pLine);
	void OnDestroyLine(CLineDevice* pLine);
	CLineDevice* GetLineByID(DWORD dwID);
	CLineDevice* FindLineByExtension(CString& strExtension);
	DWORD GenerateExtension();

	// Line enumeration support
	CLineDevice* GetFirstLine();
	CLineDevice* GetNextLine(CLineDevice* pStart=NULL);
	void NotifyOnLine(CLineDevice* pLine);

	// Agent support
	int GetAgentCount() const;
	CAgent* GetAgent(int i);
	CAgent* FindAgent(LPCTSTR pszAgentID);
	CACDQueue* GetFirstQueue(CAgent* pAgent);

	// Agent group support
	int GetAgentGroupCount() const;
	CAgentGroup* GetAgentGroup(int i);
	CAgentGroup* FindAgentGroup(DWORD id);
	DWORD GenerateAgentGroupID();

	// Free agent (idle) support
	void OnAgentIdle(CAgent* pAgent);
	void OnAgentNotIdle(CAgent* pAgent);
	CAgent* GetNextAvailableAgent(CAgentGroup* pGroup);

	// Agent<->AgentGroup list management
	CString GetAgentGroupList(CAgent* pAgent);
	CString GetAgentsFromGroup(CAgentGroup* pGroup);

	// Trunk support
	CTrunk* FindAvailableTrunk();
	void OnTrunkStateChanged(CTrunk* pTrunk);
	DWORD GenerateTrunkID();

	// Queue support
	DWORD GenerateQueueID();

	// Command processing from Connections
	void OnProcessData(COMMANDBLOCK* pBlock);
	COMMANDBLOCK* DecodeCommand(CString& strBuff);

	// Misc. functions
	void DeleteSingleObject(CUIObject* pObj);
	bool ValidateExtension(DWORD dwExtension) const;
	void OnTimer();

	// Soak test support
	bool IsRunningSoakTest() const;
	int GetCPS() const;
	void RunSoakTest();
	CString MakeUpName() const;
	CString MakeUpNumber() const;
	void GenSoakCall();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJTDocument)
	afx_msg void OnAddAgent();
	afx_msg void OnAddGroup();
	afx_msg void OnCommand(UINT nCommand);
	afx_msg void OnAddRoutePoint();
	afx_msg void OnAddStation();
	afx_msg void OnAddTrunk();
	afx_msg void OnAddPdialer();
	afx_msg void OnAddVRU();
	afx_msg void OnEditProperties();
	afx_msg void OnToolsDialer();
	afx_msg void OnToolsStation();
	afx_msg void OnSoakTest();
	afx_msg void OnToolsTrunk();
	afx_msg void OnFileExport();
	afx_msg void OnFileImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Internal functions
protected:
	CString StringToDN(LPCTSTR pszExtension);
	void StartProgram(LPCTSTR pszProgram);
};

/*****************************************************************************
** Procedure:  CJTDocument::GetAgentCount
**
** Arguments: void
**
** Returns: Count of items in agent array
**
** Description: This returns the number of agents in our array
**
*****************************************************************************/
inline int CJTDocument::GetAgentCount() const 
{ 
	return m_arrAgents.GetSize(); 

}// CJTDocument::GetAgentCount

/*****************************************************************************
** Procedure:  CJTDocument::GetAgentGroupCount
**
** Arguments: void
**
** Returns: Count of items in group array
**
** Description: This returns the number of groups in our array
**
*****************************************************************************/
inline int CJTDocument::GetAgentGroupCount() const 
{ 
	return m_arrAgentGroups.GetSize();
	
}// CJTDocument::GetAgentGroupCount

/*****************************************************************************
** Procedure:  CJTDocument::GetAgent
**
** Arguments: 'i' - Index position to retrieve agent from
**
** Returns: Agent pointer
**
** Description: This returns an agent object at the specified position
**
*****************************************************************************/
inline CAgent* CJTDocument::GetAgent(int i)
{ 
	if (i >= 0 && i < GetAgentCount())
		return m_arrAgents[i];
	return NULL;

}// CJTDocument::GetAgent

/*****************************************************************************
** Procedure:  CJTDocument::OnAgentIdle
**
** Arguments: 'pAgent' - Agent which is now READY
**
** Returns: void
**
** Description: This method adds the specified agent to our ready list
**
*****************************************************************************/
inline void CJTDocument::OnAgentIdle(CAgent* pAgent)
{
	// Make sure the agent isn't already in the list.
	if (m_lstFreeAgents.Find(pAgent) == NULL)
		m_lstFreeAgents.AddTail(pAgent);

}// CJTDocument::OnAgentIdle

/*****************************************************************************
** Procedure:  CJTDocument::OnAgentNotIdle
**
** Arguments: 'pAgent' - Agent to remove from idle list
**
** Returns: void
**
** Description: This method removes an agent from the READY list
**
*****************************************************************************/
inline void CJTDocument::OnAgentNotIdle(CAgent* pAgent)
{
	POSITION pos = m_lstFreeAgents.Find(pAgent);
	if (pos != NULL)
		m_lstFreeAgents.RemoveAt(pos);

}// CJTDocument::OnAgentNotIdle

/*****************************************************************************
** Procedure:  CJTDocument::GetAgentGroup
**
** Arguments: 'i' - Index position to retrieve group from
**
** Returns: Agent group pointer
**
** Description: This returns an agent group object at the specified position
**
*****************************************************************************/
inline CAgentGroup* CJTDocument::GetAgentGroup(int i) 
{ 
	if (i >= 0 && i < GetAgentGroupCount())
		return m_arrAgentGroups[i];
	return NULL;

}// CJTDocument::GetAgentGroup

/*****************************************************************************
** Procedure:  CJTDocument::GenerateAgentGroupID
**
** Arguments: void
**
** Returns: Unique group id identifier
**
** Description: This returns a DWORD group identifier
**
*****************************************************************************/
inline DWORD CJTDocument::GenerateAgentGroupID()
{
	return m_dwNextGroupID++;

}// CJTDocument::GenerateAgentGroupID

/*****************************************************************************
** Procedure:  CJTDocument::GenerateTrunkID
**
** Arguments: void
**
** Returns: Unique trunk id identifier
**
** Description: This returns a DWORD trunk identifier
**
*****************************************************************************/
inline DWORD CJTDocument::GenerateTrunkID()
{
	// If we haven't used up all our 1000 pre-allocated trunks
	if (++m_dwNextTrunkID < 1000)
		return m_dwNextTrunkID;

	// Otherwise slow method - search for an empty position in our map.
	// TODO: Compress indexes at some point.
	m_dwNextTrunkID = 1000;
	for (DWORD dwID = 1; dwID < 1000; dwID++)
	{
		CLineDevice* pLine;
		if (!m_mapLineDevices.Lookup(dwID, pLine))
			return dwID;
	}
	return 0xffffffff;

}// CJTDocument::GenerateTrunkID

/*****************************************************************************
** Procedure:  CJTDocument::GenerateQueueID
**
** Arguments: void
**
** Returns: Unique queue id identifier
**
** Description: This returns a DWORD queue identifier
**
*****************************************************************************/
inline DWORD CJTDocument::GenerateQueueID()
{
	// If we haven't used up our 1000 pre-allocated queue ids
	if (++m_dwNextQueueID < 10000)
		return m_dwNextQueueID;

	// Otherwise slow method - search for an empty position in our map.
	// TODO: Compress indexes at some point.
	m_dwNextQueueID = 10000;
	for (DWORD dwID = 9000; dwID < 10000; dwID++)
	{
		CLineDevice* pLine;
		if (!m_mapLineDevices.Lookup(dwID, pLine))
			return dwID;
	}
	return 0xffffffff;

}// CJTDocument::GenerateQueueID

/*****************************************************************************
** Procedure:  CJTDocument::GenerateExtension
**
** Arguments: void
**
** Returns: Unique extension id
**
** Description: This returns a DWORD station identifier
**
*****************************************************************************/
inline DWORD CJTDocument::GenerateExtension()
{
	for (DWORD dwID = 1000; dwID < 8999; dwID++)
	{
		CLineDevice* pLine;
		if (!m_mapLineDevices.Lookup(dwID, pLine))
			return dwID;
	}
	return 0xffffffff;

}// CJTDocument::GenerateExtension

/*****************************************************************************
** Procedure:  CJTDocument::GetViewStyle
**
** Arguments: void
**
** Returns: Last known state of the view
**
** Description: This returns a DWORD view style (LVS_xx)
**
*****************************************************************************/
inline DWORD CJTDocument::GetViewStyle() const
{
	return m_dwViewStyle;

}// CJTDocument::GetViewStyle

/*****************************************************************************
** Procedure:  CJTDocument::SetViewStyle
**
** Arguments: 'dwStyle' - Style (LVS_xxx) of the view
**
** Returns: void
**
** Description: This sets the last known state of the view
**
*****************************************************************************/
inline void CJTDocument::SetViewStyle(DWORD dwViewStyle)
{
	m_dwViewStyle = (dwViewStyle & LVS_TYPEMASK);

}// CJTDocument::SetViewStyle

/*****************************************************************************
** Procedure:  CJTDocument::GetTreeStyle
**
** Arguments: void
**
** Returns: Last known state of the view
**
** Description: This returns a DWORD view style (LVS_xx)
**
*****************************************************************************/
inline DWORD CJTDocument::GetTreeStyle() const
{
	return m_dwTreeStyle;

}// CJTDocument::GetTreeStyle

/*****************************************************************************
** Procedure:  CJTDocument::SetTreeStyle
**
** Arguments: 'dwStyle' - Style (LVS_xxx) of the view
**
** Returns: void
**
** Description: This sets the last known state of the view
**
*****************************************************************************/
inline void CJTDocument::SetTreeStyle(DWORD dwTreeStyle)
{
	m_dwTreeStyle = dwTreeStyle;

}// CJTDocument::SetTreeStyle

/*****************************************************************************
** Procedure:  CJTDocument::OnCreateLine
**
** Arguments: 'pLine' - Line object which has been created
**
** Returns: void
**
** Description: This event is called when a line object is created
**
*****************************************************************************/
inline void CJTDocument::OnCreateLine (CLineDevice* pLine)
{
	ASSERT (m_mapLineDevices[pLine->GetLineID()] == NULL);
	ASSERT (m_lstLines.Find(pLine) == NULL);

	// Add the entry to our map
	m_mapLineDevices[pLine->GetLineID()] = pLine;

	// Add the line to our line list.
	m_lstLines.AddTail(pLine);

}// CJTDocument::OnCreateLine

/*****************************************************************************
** Procedure:  CJTDocument::OnDestroyLine
**
** Arguments: 'pLine' - Line object which has been removed
**
** Returns: void
**
** Description: This event is called when a line object is destroyed
**
*****************************************************************************/
inline void CJTDocument::OnDestroyLine (CLineDevice* pLine)
{
	// Remove the entry to our map
	m_mapLineDevices.RemoveKey(pLine->GetLineID());

	// Delete the line in our list.
	POSITION pos = m_lstLines.Find(pLine);
	ASSERT (pos != NULL);
	m_lstLines.RemoveAt(pos);

	// Send the proper notification
	NotifyOnLine(pLine);

}// CJTDocument::OnDestroyLine

/*****************************************************************************
** Procedure:  CJTDocument::GetLineByID
**
** Arguments: 'dwLineID' - Line id to search for
**
** Returns: Pointer to relevant line object (if any)
**
** Description: This method searches for a specific line object by extension
**
*****************************************************************************/
inline CLineDevice* CJTDocument::GetLineByID(DWORD dwLineID)
{
	return m_mapLineDevices[dwLineID];

}// CJTDocument::GetLineByID

/*****************************************************************************
** Procedure:  CJTDocument::GetFirstLine
**
** Arguments: void
**
** Returns: Returns pointer to the first line in our list
**
** Description: This method sequentially walks the line list
**
*****************************************************************************/
inline CLineDevice* CJTDocument::GetFirstLine()
{
	m_posLines = m_lstLines.GetHeadPosition();
	return GetNextLine();

}// CJTDocument::GetFirstLine

/*****************************************************************************
** Procedure:  CJTDocument::GetNextLine
**
** Arguments: void
**
** Returns: Returns pointer to the next line in our list
**
** Description: This method sequentially walks the line list
**
*****************************************************************************/
inline CLineDevice* CJTDocument::GetNextLine(CLineDevice* pStart)
{
	try
	{
		if (pStart != NULL)
		{
			for (m_posLines = m_lstLines.GetHeadPosition(); 
				 m_posLines != NULL; )
			{
				if (pStart == dynamic_cast<CLineDevice*>(m_lstLines.GetNext(m_posLines)))
					break;
			}
		}

		if (m_posLines != NULL)
			return dynamic_cast<CLineDevice*>(m_lstLines.GetNext(m_posLines));
	}
	catch (...)
	{
	}

	return NULL;

}// CJTDocument::GetNextLine

/*****************************************************************************
** Procedure:  CJTDocument::AppendToLog
**
** Arguments: 'uidString' - String id
**
** Returns: void
**
** Description: This method adds a string to our log file
**
*****************************************************************************/
inline void CJTDocument::AppendToLog(char cType, UINT uidString)
{
	CString strBuff;
	if (strBuff.LoadString(uidString))
		AppendToLog(cType, strBuff);

}// CJTDocument::AppendToLog

/*****************************************************************************
** Procedure:  CJTDocument::ClearLog
**
** Arguments: void
**
** Returns: void
**
** Description: This method clears the connection log
**
*****************************************************************************/
inline void CJTDocument::ClearLog()
{
	m_arrConnBuffer.RemoveAll();
	UpdateAllViews(NULL, HINT_CONNBUFFCHANGE);

}// CJTDocument::ClearLog

/*****************************************************************************
** Procedure:  CJTDocument::GetLogBuffer
**
** Arguments: void
**
** Returns: Reference to string array
**
** Description: This method returns our string array for manipulation
**
*****************************************************************************/
inline CStringArray& CJTDocument::GetLogBuffer()
{
	return m_arrConnBuffer;

}// CJTDocument::GetLogBuffer

/*****************************************************************************
** Procedure:  CJTDocument::GetIPPort
**
** Arguments: void
**
** Returns: Integer port
**
** Description: This method returns our TCP/IP port to connect on
**
*****************************************************************************/
inline WORD CJTDocument::GetIPPort() const
{
	return m_wPort;

}// CJTDocument::GetIPPort

/*****************************************************************************
** Procedure:  CJTDocument::GetConnectionMgr
**
** Arguments: void
**
** Returns: Reference to connection manager
**
** Description: This method provides access to the connection manager
**
*****************************************************************************/
inline CConnectionMgr& CJTDocument::GetConnectionMgr()
{
	return *m_pcmConnections;

}// CJTDocument::GetConnectionMgr

/*****************************************************************************
** Procedure:  CJTDocument::IsRunningSoakTest
**
** Arguments: void
**
** Returns: Boolean
**
** Description: This method returns whether we are running a soak test
**
*****************************************************************************/
inline bool CJTDocument::IsRunningSoakTest() const
{
	return m_fRunningSoakTest;

}// CJTDocument::IsRunningSoakTest

/*****************************************************************************
** Procedure:  CJTDocument::GetCPS
**
** Arguments: void
**
** Returns: Integer
**
** Description: This method returns the current incoming calls per second
**
*****************************************************************************/
inline int CJTDocument::GetCPS() const
{
	return CTrunk::g_nCPS;

}// CJTDocument::GetCPS

#endif // __DOCUMENT_INC__
