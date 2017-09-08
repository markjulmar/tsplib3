#if !defined(AFX_MEDIACTLPROPS_H__68FB9B66_AA11_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_MEDIACTLPROPS_H__68FB9B66_AA11_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MediaCtlProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMediaCtlProps dialog

class CMediaCtlProps : public CDialog
{
// Construction
public:
	CMediaCtlProps();
	DECLARE_DYNCREATE(CMediaCtlProps)

// Dialog Data
	//{{AFX_DATA(CMediaCtlProps)
	enum { IDD = IDD_MEDIACONTROL };
	CSpinButtonCtrl	m_ctlSpin4;
	CSpinButtonCtrl	m_ctlSpin3;
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CString	m_strMaxCallStates;
	CString	m_strMaxDigits;
	CString	m_strMaxMediaModes;
	CString	m_strMaxTones;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaCtlProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMediaCtlProps)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIACTLPROPS_H__68FB9B66_AA11_11D2_AD10_342A14000000__INCLUDED_)
