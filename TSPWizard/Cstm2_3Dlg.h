#if !defined(AFX_CSTM2_3DLG_H__96418C8C_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CSTM2_3DLG_H__96418C8C_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

#include "checklb.h"

/////////////////////////////////////////////////////////////////////////////
// CCustom2_3Dlg dialog

class CCustom2_3Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom2_3Dlg();
	virtual BOOL OnKillActive();

// Dialog Data
	//{{AFX_DATA(CCustom2_3Dlg)
	enum { IDD = IDD_CUSTOM2_3 };
	CListBox	m_lbMediaList;
	CCheckListBox2 m_lbOptions;
	BOOL	m_fOverrideAddr;
	BOOL	m_fOverrideCall;
	BOOL	m_fOverrideConfCall;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom2_3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom2_3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFunctions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM2_3DLG_H__96418C8C_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
