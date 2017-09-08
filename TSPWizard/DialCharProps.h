#if !defined(AFX_DIALCHARPROPS_H__61EF86F6_AE0C_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_DIALCHARPROPS_H__61EF86F6_AE0C_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialCharProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialCharProps dialog

class CDialCharProps : public CDialog
{
// Construction
public:
	CDialCharProps();
	DECLARE_DYNCREATE(CDialCharProps)

// Dialog Data
	//{{AFX_DATA(CDialCharProps)
	enum { IDD = IDD_DIALCHARS };
	BOOL	m_fAnswer;
	BOOL	m_fBilling;
	BOOL	m_fDialtone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialCharProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialCharProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALCHARPROPS_H__61EF86F6_AE0C_11D2_AD10_342A14000000__INCLUDED_)
