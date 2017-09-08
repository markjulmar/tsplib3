/*******************************************************************/
//
// AGENTGROUP.H
//
// Agent group management objects and dialogs
// Each agent group is considered an ACD QUEUE
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

#ifndef __JPBX_AGENTGROUP_INC__
#define __JPBX_AGENTGROUP_INC__

/*----------------------------------------------------------------------------
	INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "agent.h"
#include "document.h"

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CACDQueue;

/*----------------------------------------------------------------------------
	CONSTANTS
-----------------------------------------------------------------------------*/
const UINT GROUPNAME_SIZE = 100;

/**************************************************************************
** CAgentGroup
**
** This object represents a collection of agents which are related
** in some fashion or form.
**
***************************************************************************/
class CAgentGroup : public CUIObject
{
// Class data
public:
	DWORD m_dwGroupID;		// Unique group id associated with this group
	CString m_strName;		// Name of this group
	CACDQueue* m_pQueue;	// Queue related to this group
	CTypedPtrArray<CObArray,CAgent*> m_arrAgents;

// Public Constructor
public:
	CAgentGroup( DWORD id, const char* pszName );

// Internal constructor for serialization
protected:
	CAgentGroup();
	DECLARE_SERIAL(CAgentGroup)

// Persistant storage support
public:
	void Serialize( CArchive& archive );

// Access functions
public:
	DWORD GetGroupID() const;
	const char* GetGroupName() const;

	bool IsAgentInGroup( CAgent* pAgent ) const;
	int AddAgent( CAgent* pAgent );
	void RemoveAgent( CAgent* pAgent );

	CACDQueue* GetRelatedQueue() const;
	void AssignQueue(CACDQueue* pQueue);

// Creation dialog
public:
	static CAgentGroup* Create( CWnd* pwndOwner, CJTDocument* pDoc );
	virtual bool Edit( CWnd* pwndOwner );
	virtual bool ContextMenu( CWnd* pwndOwner );
	virtual void OnCommand( UINT nCmd );

// Private functions
private:
	int FindAgent( CAgent* pAgent ) const;
};

/*****************************************************************************
** Procedure:  CAgentGroup::CAgentGroup
**
** Arguments: void
**
** Returns:    void
**
** Description:  Default no-parameter constructor for the agent group object
**
*****************************************************************************/
inline CAgentGroup::CAgentGroup() : m_dwGroupID(0), m_strName(_T("")), m_pQueue(0)
{
}// CAgentGroup::CAgentGroup

/*****************************************************************************
** Procedure:  CAgentGroup::CAgentGroup
**
** Arguments: void
**
** Returns:    void
**
** Description:  Parameterized constructor for the agent group object
**
*****************************************************************************/
inline CAgentGroup::CAgentGroup(DWORD id, const char* pszName) : 
	m_dwGroupID(id), m_strName(pszName), m_pQueue(0)
{
}// CAgentGroup::CAgentGroup

/*****************************************************************************
** Procedure:  CAgentGroup::IsAgentInGroup
**
** Arguments: 'pAgent' - Agent to locate in our group index
**
** Returns: true/false whether agent is in our group object
**
** Description:  This function searches the group to determine if the
**               specified agent may log into the group.
**
*****************************************************************************/
inline bool CAgentGroup::IsAgentInGroup(CAgent* pAgent) const
{ 
	return (FindAgent(pAgent) != -1); 

}// CAgentGroup::IsAgentInGroup

/*****************************************************************************
** Procedure:  CAgentGroup::AddAgent
**
** Arguments: 'pAgent' - Agent to add to our group
**
** Returns: Integer position where the agent was added; -1 if failure.
**
** Description:  This function adds the specified agent this this group.
**
*****************************************************************************/
inline int CAgentGroup::AddAgent(CAgent* pAgent) 
{ 
	ASSERT (pAgent != NULL);
	if (!IsAgentInGroup(pAgent)) 
	{
		int iPos = m_arrAgents.Add(pAgent); 
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
		return iPos;
	}
	return -1; 

}// CAgentGroup::AddAgent

/*****************************************************************************
** Procedure:  CAgentGroup::RemoveAgent
**
** Arguments: 'pAgent' - Agent to remove from this group
**
** Returns: void
**
** Description: This function removes the specified agent (if it exists)
**              from our group. 
**
*****************************************************************************/
inline void CAgentGroup::RemoveAgent(CAgent* pAgent) 
{
	ASSERT (pAgent != NULL);
	int iPos = FindAgent(pAgent); 
	if (iPos != -1) 
	{
		m_arrAgents.RemoveAt(iPos); 
		GetDocument()->UpdateAllViews(NULL, HINT_ITEMCHANGED, this);
	}

}// CAgentGroup::RemoveAgent

/*****************************************************************************
** Procedure:  CAgentGroup::FindAgent
**
** Arguments: 'pAgent' - Agent to locate in our array
**
** Returns: Index position of the specified agent
**
** Description:  This internal functions searches the internal group
**               array to locate the specified agent object
**
*****************************************************************************/
inline int CAgentGroup::FindAgent(CAgent* pAgent) const
{
	for (int i=0; i<m_arrAgents.GetSize(); i++)
		if (m_arrAgents[i] == pAgent) 
			return i;
	return -1; 

}// CAgentGroup::FindAgent

/*****************************************************************************
** Procedure:  CAgentGroup::GetRelatedQueue
**
** Arguments: void
**
** Returns: The pointer to the ACD Queue object which is associated with
**          this group.
**
** Description:  This returns the queue associated with this group.
**
*****************************************************************************/
inline CACDQueue* CAgentGroup::GetRelatedQueue() const
{
	return m_pQueue;

}// CAgentGroup::GetRelatedQueue

/*****************************************************************************
** Procedure:  CAgentGroup::AssignQueue
**
** Arguments: 'pQueue' - Queue which is associated to this group.
**
** Returns: void
**
** Description:  This function sets the queue pointer for this group.
**
*****************************************************************************/
inline void CAgentGroup::AssignQueue(CACDQueue* pQueue)
{
	m_pQueue = pQueue;

}// CAgentGroup::AssociateQueue

/*****************************************************************************
** Procedure:  CAgentGroup::GetGroupID
**
** Arguments: void
**
** Returns: The unique identifier for this group
**
** Description:  This returns the group identifier for this agent group
**
*****************************************************************************/
inline DWORD CAgentGroup::GetGroupID() const
{
	return m_dwGroupID;

}// CAgentGroup::GetGroupID

/*****************************************************************************
** Procedure:  CAgentGroup::GetGroupName
**
** Arguments: void
**
** Returns: The string name assigned to this group
**
** Description:  This returns the group name for this agent group
**
*****************************************************************************/
inline const char* CAgentGroup::GetGroupName() const
{
	return m_strName;

}// CAgentGroup::GetGroupName

#endif // __JPBX_AGENTGROUP_INC__