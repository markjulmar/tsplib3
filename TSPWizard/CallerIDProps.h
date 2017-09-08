#if !defined(AFX_CALLERIDPROPS_H__F5117C69_A772_11D2_AD0D_60BF92000000__INCLUDED_)
#define AFX_CALLERIDPROPS_H__F5117C69_A772_11D2_AD0D_60BF92000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallerIDProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallerIDProps dialog

class CCallerIDProps : public CDialog
{
// Construction
public:
	CCallerIDProps();
	DECLARE_DYNCREATE(CCallerIDProps)

// Dialog Data
	//{{AFX_DATA(CCallerIDProps)
	enum { IDD = IDD_CALLERID };
	CListBox	m_lbCallerFlags;
	BOOL	m_fBlocked;
	BOOL	m_fOverride;
	BOOL	m_fAppSupplied;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallerIDProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCallerIDProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLERIDPROPS_H__F5117C69_A772_11D2_AD0D_60BF92000000__INCLUDED_)
