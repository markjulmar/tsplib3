#if !defined(AFX_CONFPROP_H__668CBA8A_A6B0_11D2_AD0D_60BF92000000__INCLUDED_)
#define AFX_CONFPROP_H__668CBA8A_A6B0_11D2_AD0D_60BF92000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfProp dialog

class CConfProp : public CDialog
{
// Construction
public:
	CConfProp();
	DECLARE_DYNCREATE(CConfProp);

// Dialog Data
	//{{AFX_DATA(CConfProp)
	enum { IDD = IDD_CONFERENCE };
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CListBox	m_lbRemoveType;
	CComboBox	m_cbRemoveState;
	BOOL	m_fAddHeld;
	BOOL	m_fCreateNoHold;
	BOOL	m_fDrop;
	CString	m_strMaxInConference;
	CString	m_strMaxTransfer;
	BOOL	m_fCreateNoCall;
	BOOL	m_fCrossAddrConf;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfProp)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeRemovewhen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFPROP_H__668CBA8A_A6B0_11D2_AD0D_60BF92000000__INCLUDED_)
