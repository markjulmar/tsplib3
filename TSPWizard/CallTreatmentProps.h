#if !defined(AFX_CALLTREATMENTPROPS_H__2FBDD25E_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_CALLTREATMENTPROPS_H__2FBDD25E_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallTreatmentProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallTreatmentProps dialog

class CCallTreatmentProps : public CDialog
{
// Construction
public:
	CCallTreatmentProps();
	DECLARE_DYNCREATE(CCallTreatmentProps)

// Dialog Data
	//{{AFX_DATA(CCallTreatmentProps)
	enum { IDD = IDD_CALLTREATMENT };
	CListBox	m_lbList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallTreatmentProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCallTreatmentProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLTREATMENTPROPS_H__2FBDD25E_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
