$$IF(VERBOSE)
/***************************************************************************
//
// Config.h
//
// TAPI Service provider for TSP++ version 3.0
// Configuration dialog
//
// Copyright (C) $$CURR_YEAR$$ $$COMPANY_NAME$$
// All rights reserved
//
// $$JULMAR_INFO$$
// 
/***************************************************************************/
$$ENDIF

#if !defined(AFX_CONFIG_H__CDF1605B_B424_11D2_AD1B_DCB70C000000__INCLUDED_)
#define AFX_CONFIG_H__CDF1605B_B424_11D2_AD1B_DCB70C000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***************************************************************************
** CConfigDlg
**
** This dialog is a sample configuration dialog for the service provider.
**
***************************************************************************/
class CConfigDlg : public CDialog
{
// Dialog Data
protected:
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Construction
public:
	CConfigDlg(CWnd* pParent = NULL);	// standard constructor
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG_H__CDF1605B_B424_11D2_AD1B_DCB70C000000__INCLUDED_)
