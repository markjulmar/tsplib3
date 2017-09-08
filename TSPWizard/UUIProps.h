#if !defined(AFX_UUIPROPS_H__668CBA90_A6B0_11D2_AD0D_60BF92000000__INCLUDED_)
#define AFX_UUIPROPS_H__668CBA90_A6B0_11D2_AD0D_60BF92000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UUIProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUUIProps dialog

class CUUIProps : public CDialog
{
// Construction
public:
	CUUIProps();
	DECLARE_DYNCREATE(CUUIProps)

// Dialog Data
	//{{AFX_DATA(CUUIProps)
	enum { IDD = IDD_UUIPROP };
	CSpinButtonCtrl	m_ctlSpin5;
	CSpinButtonCtrl	m_ctlSpin4;
	CSpinButtonCtrl	m_ctlSpin3;
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CString	m_strAccept;
	CString	m_strAnswer;
	CString	m_strDrop;
	CString	m_strMakeCall;
	CString	m_strSendUUI;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUUIProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUUIProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUUIChecked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UUIPROPS_H__668CBA90_A6B0_11D2_AD0D_60BF92000000__INCLUDED_)
