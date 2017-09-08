#if !defined(AFX_PICKUPPROPS_H__2FBDD25C_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_PICKUPPROPS_H__2FBDD25C_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickupProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPickupProps dialog

class CPickupProps : public CDialog
{
// Construction
public:
	CPickupProps();
	DECLARE_DYNCREATE(CPickupProps)

// Dialog Data
	//{{AFX_DATA(CPickupProps)
	enum { IDD = IDD_PICKUP };
	BOOL	m_fCallWaiting;
	BOOL	m_fGroupId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPickupProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPickupProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKUPPROPS_H__2FBDD25C_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
