$$IF(VERBOSE)
/***************************************************************************
//
// Config.cpp
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

/*-------------------------------------------------------------------------------*/
// INCLUDE FILES
/*-------------------------------------------------------------------------------*/
$$ENDIF
#include "stdafx.h"
#include "$$root$$UI.h"
#include "config.h"

$$IF(VERBOSE)
/*-------------------------------------------------------------------------------*/
// MFC Message map
/*-------------------------------------------------------------------------------*/
$$ENDIF
BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CConfigDlg::CConfigDlg
**
** Arguments:  'pParent' - Parent window handle
**
** Returns:    void
**
** Description: Constructor for the sample configuration dialog
**
*****************************************************************************/
$$ENDIF
CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/) : CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}// CConfigDlg::CConfigDlg

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CConfigDlg::DoDataExchange
**
** Arguments:  'pDX' - CDataExchange element
**
** Returns:    void
**
** Description: This connects window controls up with C++ objects.
**
*****************************************************************************/
$$ENDIF
void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}// CConfigDlg::DoDataExchange

$$IF(VERBOSE)
/*****************************************************************************
** Procedure:  CConfigDlg::OnInitDialog
**
** Arguments:  void
**
** Returns:    TRUE if Windows is to decide the first control to have focus.
**
** Description: This is called during the dialog's initialization.
**
*****************************************************************************/
$$ENDIF
BOOL CConfigDlg::OnInitDialog()
{
	// Connect up the controls to the objects.
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control

}// CConfigDlg::OnInitDialog

