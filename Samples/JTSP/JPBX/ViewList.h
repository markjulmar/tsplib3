/*******************************************************************/
//
// VIEWLIST.H
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_JDVIEW_INC__
#define __JPBX_JDVIEW_INC__

/*----------------------------------------------------------------------------
	PRE-DEFINITIONS
-----------------------------------------------------------------------------*/
class CStation;
class CACDQueue;
class CRoutePoint;
class CTrunk;
class CPDialer;
class CVRU;

/**************************************************************************
** CJDView
**
** List view class which is used when any element is selected in our 
** tree view.
**
***************************************************************************/
class CJDView : public CListView
{
// Class data
protected:
	enum { 
		None,
		Agents,
		AgentGroups,
		Stations,
		Queues,
		Trunks,
		RoutePoints,
		PDialers,
		IVRUnits
	} m_View;

	//{{AFX_DATA(CJDView)
	CImageList* m_pilSmallIcons;
	CImageList* m_pilLargeIcons;
	//}}AFX_DATA

protected: // create from serialization only
	CJDView();
	virtual ~CJDView();
	DECLARE_DYNCREATE(CJDView)

// Attributes
public:
	CJTDocument* GetDocument();

// Internal functions
protected:
	void LoadLines();
	void InsertObject(CUIObject* pObject);
	void FillInAgent(CAgent* pAgent, int iCol, char* pszBuffer);
	void FillInGroup(CAgentGroup* pGroup, int iCol, char* pszBuffer);
	void FillInStation(CStation* pStation, int iCol, char* pszBuffer);
	void FillInTrunk(CTrunk* pTrunk, int iCol, char* pszBuffer);
	void FillInQueue(CACDQueue* pQueue, int iCol, char* pszBuffer);
	void FillInRPoint(CRoutePoint* pRP, int iCol, char* pszBuffer);
	void FillInPDialer(CPDialer* pDialer, int iCol, char* pszBuffer);
	void FillInVru(CVRU* pVRU, int iCol, char* pszBuffer);

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJDView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CJDView)
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CJDView::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to our document object
**
** Description: This override casts our document to the appropriate type
**
*****************************************************************************/
inline CJTDocument* CJDView::GetDocument()
{ 
	return dynamic_cast<CJTDocument*>(m_pDocument);

}// CJDView::GetDocument

#endif // __JPBX_JDVIEW_INC__
