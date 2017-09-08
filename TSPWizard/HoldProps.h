#if !defined(AFX_HOLDPROPS_H__2FBDD259_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_HOLDPROPS_H__2FBDD259_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoldProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoldProps dialog

class CHoldProps : public CDialog
{
// Construction
public:
	CHoldProps();
	DECLARE_DYNCREATE(CHoldProps)

// Dialog Data
	//{{AFX_DATA(CHoldProps)
	enum { IDD = IDD_HOLD };
	CSpinButtonCtrl	m_ctlSpin1;
	CString	m_strMaxOnHold;
	BOOL	m_fHoldMakesNew;
	BOOL	m_fSwapHold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoldProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHoldProps)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLDPROPS_H__2FBDD259_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
