#if !defined(AFX_CSTM5DLG_H__96418C90_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CSTM5DLG_H__96418C90_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

// cstm5dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustom5Dlg dialog

#define TOTAL_CLASSES 7

class CCustom5Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom5Dlg();
	virtual BOOL OnKillActive();

// Dialog Data
	//{{AFX_DATA(CCustom5Dlg)
	enum { IDD = IDD_CUSTOM5 };
	int m_nCurrSel;
	CListBox	m_lbClasses;
	CString	m_strBaseClass;
	//}}AFX_DATA
	CString m_strClassNames[TOTAL_CLASSES];
	CString m_strCPPFiles[TOTAL_CLASSES];
	CString m_strHFiles[TOTAL_CLASSES];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool Validate();

	// Generated message map functions
	//{{AFX_MSG(CCustom5Dlg)
	afx_msg void OnSelchangeClasses();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM5DLG_H__96418C90_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
