#if !defined(AFX_DIALTONEPROPS_H__6C84BA47_AC15_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_DIALTONEPROPS_H__6C84BA47_AC15_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialToneProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialToneProps dialog

class CDialToneProps : public CDialog
{
// Construction
public:
	CDialToneProps();
	DECLARE_DYNCREATE(CDialToneProps)

// Dialog Data
	//{{AFX_DATA(CDialToneProps)
	enum { IDD = IDD_DIALTONES };
	CListBox	m_lbDialtones;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialToneProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialToneProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALTONEPROPS_H__6C84BA47_AC15_11D2_AD10_342A14000000__INCLUDED_)
