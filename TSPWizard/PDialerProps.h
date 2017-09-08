#if !defined(AFX_PDIALERPROPS_H__2FBDD25F_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_PDIALERPROPS_H__2FBDD25F_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PDialerProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPDialerProps dialog

class CPDialerProps : public CDialog
{
// Construction
public:
	CPDialerProps();
	DECLARE_DYNCREATE(CPDialerProps)

// Dialog Data
	//{{AFX_DATA(CPDialerProps)
	enum { IDD = IDD_PREDDIALER };
	CEdit	m_edtMaxTimeout;
	CListBox	m_lbStates;
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CString	m_strMaxCalls;
	CString	m_strMaxTimeout;
	BOOL	m_fTimeout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPDialerProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPDialerProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimeout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDIALERPROPS_H__2FBDD25F_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
