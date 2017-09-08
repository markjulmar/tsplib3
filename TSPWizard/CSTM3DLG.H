#if !defined(AFX_CSTM3DLG_H__96418C8C_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CSTM3DLG_H__96418C8C_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

#include "checklb.h"

/////////////////////////////////////////////////////////////////////////////
// CCustom3Dlg dialog

class CCustom3Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom3Dlg();
	virtual BOOL OnKillActive();

// Dialog Data
	//{{AFX_DATA(CCustom3Dlg)
	enum { IDD = IDD_CUSTOM3 };
	CSpinButtonCtrl	m_ctlSpin2;
	CSpinButtonCtrl	m_ctlSpin1;
	CCheckListBox2 m_lbLineFuncs;
	CString	m_strAddressCount;
	CString	m_strMaxNumActiveCalls;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLineFuncs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM3DLG_H__96418C8C_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
