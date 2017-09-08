#if !defined(AFX_DIALPROPS_H__A819382B_AA8C_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_DIALPROPS_H__A819382B_AA8C_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialProps dialog

class CDialProps : public CDialog
{
// Construction
public:
	CDialProps();
	DECLARE_DYNCREATE(CDialProps)

// Dialog Data
	//{{AFX_DATA(CDialProps)
	enum { IDD = IDD_DIALPARAMS };
	CSpinButtonCtrl	m_ctlSpin12;
	CSpinButtonCtrl	m_ctlSpin11;
	CSpinButtonCtrl	m_ctlSpin10;
	CSpinButtonCtrl	m_ctlSpin9;
	CSpinButtonCtrl	m_ctlSpin8;
	CSpinButtonCtrl	m_ctlSpin7;
	CSpinButtonCtrl	m_ctlSpin6;
	CSpinButtonCtrl	m_ctlSpin5;
	CSpinButtonCtrl	m_ctlSpin4;
	CSpinButtonCtrl	m_ctlSpin3;
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CString	m_strDefaultDigit;
	CString	m_strMaxDigit;
	CString	m_strMinDigit;
	CString	m_strDefaultDuration;
	CString	m_strMaxDuration;
	CString	m_strMinDuration;
	CString	m_strDefaultPause;
	CString	m_strMaxPause;
	CString	m_strMinPause;
	CString	m_strDefaultDialtone;
	CString	m_strMaxDialtone;
	CString	m_strMinDialtone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALPROPS_H__A819382B_AA8C_11D2_AD10_342A14000000__INCLUDED_)
