#if !defined(AFX_FORWARDPROPS_H__2FBDD25A_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_FORWARDPROPS_H__2FBDD25A_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ForwardProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CForwardProps dialog

class CForwardProps : public CDialog
{
// Construction
public:
	CForwardProps();
	DECLARE_DYNCREATE(CForwardProps)

// Dialog Data
	//{{AFX_DATA(CForwardProps)
	enum { IDD = IDD_FORWARD };
	CEdit	m_edtMaxRings;
	CEdit	m_edtMinRings;
	CEdit	m_edtCallid;
	CSpinButtonCtrl	m_ctlSpin4;
	CSpinButtonCtrl	m_ctlSpin3;
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CListBox	m_lbModes;
	BOOL	m_fBusyNA;
	BOOL	m_fRequiresConsult;
	BOOL	m_fIntExtSeperate;
	BOOL	m_fStatusValid;
	CString	m_strMaxFwdCallid;
	CString	m_strMaxFwdEntries;
	CString	m_strMaxRings;
	CString	m_strMinRings;
	BOOL	m_fSupportsCallerid;
	BOOL	m_fSpecifyRings;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForwardProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CForwardProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCallerid();
	afx_msg void OnSpecifyrings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORWARDPROPS_H__2FBDD25A_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
