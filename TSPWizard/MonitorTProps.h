#if !defined(AFX_MONITORTPROPS_H__2FBDD262_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_MONITORTPROPS_H__2FBDD262_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonitorTProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonitorTProps dialog

class CMonitorTProps : public CDialog
{
// Construction
public:
	CMonitorTProps();
	DECLARE_DYNCREATE(CMonitorTProps)

// Dialog Data
	//{{AFX_DATA(CMonitorTProps)
	enum { IDD = IDD_MONITORTONE };
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CString	m_strMaxFreq;
	CString	m_strMaxTones;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorTProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonitorTProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORTPROPS_H__2FBDD262_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
