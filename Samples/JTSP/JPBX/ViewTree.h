/*******************************************************************/
//
// VIEWTREE.H
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __JPBX_JTVIEW_INC__
#define __JPBX_JTVIEW_INC__

/**************************************************************************
** CJTView
**
** Tree view class which is used as the left side of our JPBX emulator
**
***************************************************************************/
class CJTView : public CTreeView
{
// Class data
protected:
	//{{AFX_DATA(CJTView)
	CImageList * m_pilIcons;
	HTREEITEM m_htiRoot;
	HTREEITEM m_htiGroups;
	HTREEITEM m_htiAgents;
	HTREEITEM m_htiStations;
	HTREEITEM m_htiQueues;
	HTREEITEM m_htiTrunks;
	HTREEITEM m_htiRPoints;
	HTREEITEM m_htiPDialers;
	HTREEITEM m_htiVRUs;
	CUIObject* m_pSelected;
	//}}AFX_DATA

protected: // create from serialization only
	CJTView();
	virtual ~CJTView();
	DECLARE_DYNCREATE(CJTView)

// Attributes
public:
	CJTDocument* GetDocument();
	bool IsHeader(CUIObject* pBase) const;

// Internal functions
protected:
	void LoadGroups();
	void LoadAgents();
	void LoadTrunks();
	void LoadStations();
	void LoadRPoints();
	void LoadQueues();
	void LoadPDialers();
	void LoadVrus();

	HTREEITEM FindItem(CUIObject* pItem);
	void UpdateItem(CObject* pItem);

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJTView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CJTView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*****************************************************************************
** Procedure:  CJTView::GetDocument
**
** Arguments: void
**
** Returns:    Pointer to our document object
**
** Description: This override casts our document to the appropriate type
**
*****************************************************************************/
inline CJTDocument* CJTView::GetDocument()
{ 
	return dynamic_cast<CJTDocument*>(m_pDocument);

}// CJTView::GetDocument

/*****************************************************************************
** Procedure:  CJTView::IsHeader
**
** Arguments: 'pBase' - Returns whether the selected item is a header
**             node.
**
** Returns:    true/false
**
** Description: This checks the item to determine if it is valid
**
*****************************************************************************/
inline bool CJTView::IsHeader(CUIObject* pBase) const
{
	return (pBase >= BO_NONE && pBase <= BO_IVRUNITS);

}// CJTView::IsHeader

#endif // __JPBX_JTVIEW_INC__
