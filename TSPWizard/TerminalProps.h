#if !defined(AFX_TERMINALPROPS_H__68FB9B67_AA11_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_TERMINALPROPS_H__68FB9B67_AA11_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerminalProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps dialog

class CTerminalProps : public CDialog
{
// Construction
public:
	CTerminalProps();
	virtual ~CTerminalProps();
	DECLARE_DYNCREATE(CTerminalProps)

// Dialog Data
	//{{AFX_DATA(CTerminalProps)
	enum { IDD = IDD_TERMINALS };
	CListBox	m_lbDefTermModes;
	CListBox	m_lbTermModes;
	CListBox	m_lbTerminals;
	CComboBox	m_cbSharing;
	CButton	m_btnRemove;
	CComboBox	m_cbDevType;
	CButton	m_btnAdd;
	CString	m_strName;
	BOOL	m_fMediaUniControl;
	int		m_iDevice;
	int		m_iSharing;
	CPtrArray m_arrTerminals;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerminalProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTerminalProps)
	afx_msg void OnSelchangeTerminals();
	afx_msg void OnChangeName();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMINALPROPS_H__68FB9B67_AA11_11D2_AD10_342A14000000__INCLUDED_)
