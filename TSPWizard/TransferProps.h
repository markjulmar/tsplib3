#if !defined(AFX_TRANSFERPROPS_H__2FBDD258_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_TRANSFERPROPS_H__2FBDD258_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransferProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransferProps dialog

class CTransferProps : public CDialog
{
// Construction
public:
	CTransferProps();
	DECLARE_DYNCREATE(CTransferProps)

// Dialog Data
	//{{AFX_DATA(CTransferProps)
	enum { IDD = IDD_TRANSFER };
	CSpinButtonCtrl	m_ctlSpin1;
	BOOL	m_fHoldTransfer;
	BOOL	m_fMakeCallTransfer;
	CString	m_strMaxOnHoldPend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransferProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERPROPS_H__2FBDD258_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
