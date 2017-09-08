#if !defined(AFX_SECUREPROPS_H__2FBDD25D_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_SECUREPROPS_H__2FBDD25D_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SecureProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSecureProps dialog

class CSecureProps : public CDialog
{
// Construction
public:
	CSecureProps();
	DECLARE_DYNCREATE(CSecureProps)

// Dialog Data
	//{{AFX_DATA(CSecureProps)
	enum { IDD = IDD_SECURE };
	BOOL	m_fSecure;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecureProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSecureProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECUREPROPS_H__2FBDD25D_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
