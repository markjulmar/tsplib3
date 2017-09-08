#if !defined(AFX_CSTM2DLG_H__96418C8A_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CSTM2DLG_H__96418C8A_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

// cstm2dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustom2Dlg dialog

class CCustom2Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom2Dlg();
	virtual BOOL OnKillActive();

// Dialog Data
	//{{AFX_DATA(CCustom2Dlg)
	enum { IDD = IDD_CUSTOM2 };
	CString	m_strEventObjectName;
	CString	m_strProviderInfo;
	CString	m_strSwitchInfo;
	int		m_iCommParams;
	BOOL	m_fUsePoolMgr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom2Dlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM2DLG_H__96418C8A_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
