#if !defined(AFX_UPLOADBUFFPROPS_H__FD5B3076_AE87_11D2_AD10_342A14000000__INCLUDED_)
#define AFX_UPLOADBUFFPROPS_H__FD5B3076_AE87_11D2_AD10_342A14000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UploadBuffProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUploadBuffProps dialog

class CUploadBuffProps : public CDialog
{
// Construction
public:
	CUploadBuffProps();
	DECLARE_DYNCREATE(CUploadBuffProps)

// Dialog Data
	//{{AFX_DATA(CUploadBuffProps)
	enum { IDD = IDD_UPDOWNBUFF };
	CEdit	m_edtSize;
	CSpinButtonCtrl	m_ctlSpin1;
	CButton	m_btnRemove;
	CListBox	m_lbBuffers;
	CButton	m_btnAdd;
	CString	m_strBufferSize;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUploadBuffProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUploadBuffProps)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnSelchangeBuffers();
	afx_msg void OnRemove();
	afx_msg void OnChangeBuffersize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPLOADBUFFPROPS_H__FD5B3076_AE87_11D2_AD10_342A14000000__INCLUDED_)
