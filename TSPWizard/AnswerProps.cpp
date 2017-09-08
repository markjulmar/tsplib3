/******************************************************************************/
//                                                                        
// ANSWERPROPS.CPP - Answer properties
//                                                                        
// Copyright (C) 1998 Mark C. Smith, JulMar Entertainment Technology, Inc.
// All rights reserved                                                    
// For internal use only
//                                                                        
/******************************************************************************/

/*---------------------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "tspwizard.h"
#include "AnswerProps.h"
#include "TSPWizardaw.h"

/*---------------------------------------------------------------------------*/
// DEBUG STATEMENTS
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------------------*/
// RTTI
/*---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CAnswerProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAnswerProps, CDialog)
	//{{AFX_MSG_MAP(CAnswerProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CAnswerProps::CAnswerProps
//
// Answer property dialog
//
CAnswerProps::CAnswerProps() : CDialog(CAnswerProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CAnswerProps)
	m_iDropType = 0;
	//}}AFX_DATA_INIT
}// CAnswerProps::CAnswerProps

///////////////////////////////////////////////////////////////////////////
// CAnswerProps::DoDataExchange
//
// Dialog data exchange for the answer page
//
void CAnswerProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnswerProps)
	DDX_Radio(pDX, IDC_DROPPED, m_iDropType);
	//}}AFX_DATA_MAP

}// CAnswerProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CAnswerProps::OnOK
// 
// Dismiss the dialog
//
void CAnswerProps::OnOK() 
{
	static LPCTSTR pszProps[] = {
		{ "LINEANSWERMODE_HOLD" },
		{ "LINEANSWERMODE_DROP" },
		{ "LINEANSWERMODE_NONE" },
	};

	UpdateData(TRUE);
	TSPWizardaw.m_Dictionary["ANSWERMODE"] = pszProps[m_iDropType];
	CDialog::OnOK();

}// CAnswerProps::OnOK
