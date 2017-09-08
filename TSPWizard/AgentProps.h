#if !defined(AFX_AGENTPROPS_H__668CBA8F_A6B0_11D2_AD0D_60BF92000000__INCLUDED_)
#define AFX_AGENTPROPS_H__668CBA8F_A6B0_11D2_AD0D_60BF92000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgentProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAgentProps dialog

class CAgentProps : public CDialog
{
// Construction
public:
	CAgentProps();
	DECLARE_DYNCREATE(CAgentProps)

// Dialog Data
	//{{AFX_DATA(CAgentProps)
	enum { IDD = IDD_AGENTPROP };
	CEdit	m_edtAgentGroups;
	CSpinButtonCtrl	m_ctlSpin1;
	CListBox	m_lbStates;
	BOOL	m_fActivities;
	BOOL	m_fGroups;
	BOOL	m_fStates;
	CString	m_strMaxAgentGroups;
	BOOL	m_fRequiresID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgentProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnGroups();
	afx_msg void OnRequiresid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENTPROPS_H__668CBA8F_A6B0_11D2_AD0D_60BF92000000__INCLUDED_)
