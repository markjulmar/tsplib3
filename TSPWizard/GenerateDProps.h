#if !defined(AFX_GENERATEDPROPS_H__2FBDD260_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_GENERATEDPROPS_H__2FBDD260_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenerateDProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenerateDProps dialog

class CGenerateDProps : public CDialog
{
// Construction
public:
	CGenerateDProps();
	DECLARE_DYNCREATE(CGenerateDProps)

// Dialog Data
	//{{AFX_DATA(CGenerateDProps)
	enum { IDD = IDD_GENERATEDIGITS };
	BOOL	m_fPulse;
	BOOL	m_fTone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenerateDProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenerateDProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERATEDPROPS_H__2FBDD260_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
