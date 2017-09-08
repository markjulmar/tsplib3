#if !defined(AFX_GENERATETPROPS_H__2FBDD261_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_GENERATETPROPS_H__2FBDD261_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenerateTProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenerateTProps dialog

class CGenerateTProps : public CDialog
{
// Construction
public:
	CGenerateTProps();
	DECLARE_DYNCREATE(CGenerateTProps)

// Dialog Data
	//{{AFX_DATA(CGenerateTProps)
	enum { IDD = IDD_GENERATET };
	CListBox	m_lbTones;
	CSpinButtonCtrl	m_ctlSpin1;
	CEdit	m_edtMaxTones;
	CString	m_strMaxTones;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenerateTProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenerateTProps)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTonelist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERATETPROPS_H__2FBDD261_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
