#if !defined(AFX_ACCEPTPROPS_H__2FBDD256_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_ACCEPTPROPS_H__2FBDD256_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcceptProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAcceptProps dialog

class CAcceptProps : public CDialog
{
// Construction
public:
	CAcceptProps();
	DECLARE_DYNCREATE(CAcceptProps)

// Dialog Data
	//{{AFX_DATA(CAcceptProps)
	enum { IDD = IDD_ACCEPT };
	BOOL	m_fAcceptToAlert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcceptProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAcceptProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCEPTPROPS_H__2FBDD256_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
