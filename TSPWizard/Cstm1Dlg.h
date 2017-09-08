#if !defined(AFX_CSTM1DLG_H__96418C88_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CSTM1DLG_H__96418C88_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

class CCustom1Dlg : public CAppWizStepDlg
{
// Dialog Data
protected:
	//{{AFX_DATA(CCustom1Dlg)
	enum { IDD = IDD_CUSTOM1 };
	CStatic	m_ctlPicture;
	CComboBox	m_cbTapiVersion;
	CString	m_strCompanyName;
	int		m_iHasComments;
	BOOL	m_fGenerateUI;
	int		m_iTapiVersion;
	BOOL	m_fTraceEvents;
	BOOL	m_fWinnt;
	//}}AFX_DATA

// Construction
public:
	CCustom1Dlg();

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM1DLG_H__96418C88_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
