#if !defined(AFX_CALLCOMPLETEPROPS_H__F5117C68_A772_11D2_AD0D_60BF92000000__INCLUDED_)
#define AFX_CALLCOMPLETEPROPS_H__F5117C68_A772_11D2_AD0D_60BF92000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallCompleteProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps dialog

class CCallCompleteProps : public CDialog
{
// Construction
public:
	CCallCompleteProps();
	DECLARE_DYNCREATE(CCallCompleteProps)

// Dialog Data
	//{{AFX_DATA(CCallCompleteProps)
	enum { IDD = IDD_CALLCOMPL };
	CSpinButtonCtrl	m_ctlSpin1;
	CListBox	m_lbTypes;
	CButton	m_btnRemove;
	CEdit	m_edtMessage;
	CListBox	m_lbText;
	CListBox	m_lbModes;
	CButton	m_btnAdd;
	CString	m_strMessage;
	CString	m_strMaxCallCompl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallCompleteProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCallCompleteProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComplmode();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnChangeMessage();
	afx_msg void OnSelchangeCompltxt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLCOMPLETEPROPS_H__F5117C68_A772_11D2_AD0D_60BF92000000__INCLUDED_)
