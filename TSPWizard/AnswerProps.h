#if !defined(AFX_ANSWERPROPS_H__2FBDD257_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
#define AFX_ANSWERPROPS_H__2FBDD257_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnswerProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnswerProps dialog

class CAnswerProps : public CDialog
{
// Construction
public:
	CAnswerProps();
	DECLARE_DYNCREATE(CAnswerProps)

// Dialog Data
	//{{AFX_DATA(CAnswerProps)
	enum { IDD = IDD_ANSWER };
	int		m_iDropType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnswerProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnswerProps)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANSWERPROPS_H__2FBDD257_A8F4_11D2_AD0E_7E7D65000000__INCLUDED_)
