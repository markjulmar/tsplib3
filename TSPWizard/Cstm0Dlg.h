#pragma once

class CCustom0Dlg : public CAppWizStepDlg
{
// Dialog Data
protected:
	//{{AFX_DATA(CCustom1Dlg)
	enum { IDD = IDD_CUSTOM0 };
	CStatic	m_ctlPicture;
	CStatic m_ctlHeader;
	CFont m_fntBold;
	CString	m_strProjectName;
	CString	m_strProjectPath;
	int	m_iCompiler;
	//}}AFX_DATA

// Construction
public:
	CCustom0Dlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom1Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
