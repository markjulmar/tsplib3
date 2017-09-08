#if !defined(AFX_HSWITCHPROPS_H__7922B1B6_B061_11D2_AD16_6056B6000000__INCLUDED_)
#define AFX_HSWITCHPROPS_H__7922B1B6_B061_11D2_AD16_6056B6000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HSwitchProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps dialog

class CHSwitchProps : public CDialog
{
// Construction
public:
	CHSwitchProps();
	DECLARE_DYNCREATE(CHSwitchProps)

// Dialog Data
	//{{AFX_DATA(CHSwitchProps)
	enum { IDD = IDD_HSWITCH };
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CListBox	m_lbSettableModes;
	CButton	m_btnRemove;
	CListBox	m_lbList;
	CComboBox	m_cbDevType;
	CListBox	m_lbAvailableModes;
	CButton	m_btnAdd;
	CString	m_strGain;
	CString	m_strVolume;
	BOOL	m_fGainChange;
	BOOL	m_fVolChange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHSwitchProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHSwitchProps)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual void OnOK();
	afx_msg void OnSelchangeDevice();
	afx_msg void OnChange();
	afx_msg void OnSelchangeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSWITCHPROPS_H__7922B1B6_B061_11D2_AD16_6056B6000000__INCLUDED_)
