#if !defined(AFX_BLINDXFERPROPS_H__8A3130D1_A943_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_BLINDXFERPROPS_H__8A3130D1_A943_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlindXferProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlindXferProps dialog

class CBlindXferProps : public CDialog
{
// Construction
public:
	CBlindXferProps();
	DECLARE_DYNCREATE(CBlindXferProps)

// Dialog Data
	//{{AFX_DATA(CBlindXferProps)
	enum { IDD = IDD_BLINDXFER };
	BOOL	m_fNoInternal;
	BOOL	m_fNoExternal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlindXferProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlindXferProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLINDXFERPROPS_H__8A3130D1_A943_11D2_AD0E_7E7D65000000__INCLUDED_)
