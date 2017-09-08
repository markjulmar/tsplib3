#if !defined(AFX_BUTTONLAMPPROPS_H__FD5B3078_AE87_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_BUTTONLAMPPROPS_H__FD5B3078_AE87_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonLampProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonLampProps dialog

class CButtonLampProps : public CDialog
{
// Construction
public:
	CButtonLampProps();
	DECLARE_DYNCREATE(CButtonLampProps);

// Dialog Data
	//{{AFX_DATA(CButtonLampProps)
	enum { IDD = IDD_BUTTONS };
	CButton	m_btnRemove;
	CListBox	m_lbLampModes;
	CComboBox	m_cbModes;
	CListBox	m_lbList;
	CComboBox	m_cbFunctions;
	CButton	m_btnAdd;
	BOOL	m_fWriteButtons;
	BOOL	m_fWriteLamps;
	CString	m_strText;
	int		m_iFunction;
	int		m_iMode;
	//}}AFX_DATA

protected:
	void LoadButtonFeatures();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonLampProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CButtonLampProps)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnChange();
	afx_msg void OnSelchangeList();
	afx_msg void OnRemove();
	virtual void OnOK();
	afx_msg void OnSelchangeMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONLAMPPROPS_H__FD5B3078_AE87_11D2_AD10_342A14000000__INCLUDED_)
