#if !defined(AFX_DISPLAYPROPS_H__FD5B3077_AE87_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_DISPLAYPROPS_H__FD5B3077_AE87_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayProps dialog

class CDisplayProps : public CDialog
{
// Construction
public:
	CDisplayProps();
	DECLARE_DYNCREATE(CDisplayProps)

// Dialog Data
	//{{AFX_DATA(CDisplayProps)
	enum { IDD = IDD_DISPLAY };
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	BOOL	m_fRead;
	BOOL	m_fWrite;
	CString	m_strHeight;
	CString	m_strWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplayProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYPROPS_H__FD5B3077_AE87_11D2_AD10_342A14000000__INCLUDED_)
