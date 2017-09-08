#if !defined(AFX_CSTM4DLG_H__96418C8E_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CSTM4DLG_H__96418C8E_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

// cstm4dlg.h : header file
//

#include "checklb.h"

/////////////////////////////////////////////////////////////////////////////
// CCustom4Dlg dialog

class CCustom4Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom4Dlg();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CCustom4Dlg)
	enum { IDD = IDD_CUSTOM4 };
	CCheckListBox2 m_lbPhoneFuncs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom4Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePhoneFuncs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM4DLG_H__96418C8E_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
