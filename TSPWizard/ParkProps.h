#if !defined(AFX_PARKPROPS_H__2FBDD25B_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_PARKPROPS_H__2FBDD25B_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParkProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParkProps dialog

class CParkProps : public CDialog
{
// Construction
public:
	CParkProps();
	DECLARE_DYNCREATE(CParkProps)

// Dialog Data
	//{{AFX_DATA(CParkProps)
	enum { IDD = IDD_PARK };
	BOOL	m_fDirected;
	BOOL	m_fNonDirected;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParkProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParkProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARKPROPS_H__2FBDD25B_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
