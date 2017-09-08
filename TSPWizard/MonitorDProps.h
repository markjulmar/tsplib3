#if !defined(AFX_MONITORDPROPS_H__2FBDD263_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_MONITORDPROPS_H__2FBDD263_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonitorDProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonitorDProps dialog

class CMonitorDProps : public CDialog
{
// Construction
public:
	CMonitorDProps();
	DECLARE_DYNCREATE(CMonitorDProps)

// Dialog Data
	//{{AFX_DATA(CMonitorDProps)
	enum { IDD = IDD_MONITORD };
	CListBox	m_lbList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorDProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonitorDProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORDPROPS_H__2FBDD263_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
