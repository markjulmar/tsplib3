/******************************************************************************/
//                                                                        
// CSTM0DLG.CPP - Introduction page for the TSP Wizard
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
#include "TSPWizard.h"
#include "cstm0dlg.h"
#include "TSPWizardaw.h"
#include <io.h>

/*---------------------------------------------------------------------------*/
// DEBUG STATEMENTS
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCustom0Dlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CCustom0Dlg)
	ON_BN_CLICKED(IDC_BROWSE, OnSelectPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CCustom0Dlg::CCustom0Dlg
//
// Constructor for the introduction page
//
CCustom0Dlg::CCustom0Dlg() : CAppWizStepDlg(CCustom0Dlg::IDD)
{
	//{{AFX_DATA_INIT(CCustom0Dlg)
	m_iCompiler = 0;
	//}}AFX_DATA_INIT
}// CCustom0Dlg::CCustom0Dlg

///////////////////////////////////////////////////////////////////////////
// CCustom0Dlg::DoDataExchange
//
// Dialog Data Exchange for the introduction page
//
void CCustom0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom0Dlg)
	DDX_Control(pDX, IDC_PICTURE, m_ctlPicture);
	DDX_Control(pDX, IDC_HEADER, m_ctlHeader);
	DDX_Text(pDX, IDC_PROJECTNAME, m_strProjectName);
	DDX_Text(pDX, IDC_PATH, m_strProjectPath);
	DDX_CBIndex(pDX, IDC_COMPILERVER, m_iCompiler);
	//}}AFX_DATA_MAP

}// CCustom0Dlg::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CCustom0Dlg::OnInitDialog
//
// This is called to initialize the dialog
//
BOOL CCustom0Dlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();

	LOGFONT lf;
	GetFont()->GetObject(sizeof(lf), &lf);
	lf.lfWeight = FW_BOLD;
	m_fntBold.CreateFontIndirect(&lf);
	m_ctlHeader.SetFont(&m_fntBold);

	return TRUE;

}// CCustom0Dlg::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CCustom0Dlg::OnKillActive
//
// This is called when the user presses "Next" to move to the next pane.
//
BOOL CCustom0Dlg::OnKillActive()
{
	static LPCSTR pszCompilerVer[] = { "VS6", "VS2002", "VS2003" };

	// Validate the data on the dialog
	if (!UpdateData(TRUE))
		return FALSE;

	if (m_strProjectName.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_PROJECTNAME));
		return FALSE;
	}

	if (m_strProjectPath.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_PATH));
		return FALSE;
	}

	if (m_strProjectPath.GetLength() < 3 ||
		(m_strProjectPath.Left(2) != "\\\\" &&
		m_strProjectPath[1] != ':'))
	{
		AfxMessageBox(IDS_BADDIRECTORYPATH);
		GotoDlgCtrl(GetDlgItem(IDC_PATH));
		return FALSE;
	}

	CString projName = m_strProjectName;
	projName.Replace(' ', '_');

	// Check to see if the project already exists.
	CString strPath = m_strProjectPath; strPath.Trim();
	if (strPath.Right(1) != '\\')
		strPath += '\\';
	strPath += projName;
	strPath += '\\';
	strPath += projName;
	strPath += ".cpp";
	if (_access(strPath,0) != -1)
	{
		AfxMessageBox(IDS_PROJECTEXISTS);
		GotoDlgCtrl(GetDlgItem(IDC_PATH));
		return FALSE;
	}

	// Set the compiler version
	for (int i = 0; i < sizeof(pszCompilerVer) / sizeof(pszCompilerVer[0]); ++i)
		TSPWizardaw.m_Dictionary[pszCompilerVer[i]] = (i == m_iCompiler) ? "Yes" : "No";

	// Add the dictionary elements from this dialog pane.
	TSPWizardaw.m_Dictionary["PATH"] = m_strProjectPath;	
	TSPWizardaw.m_Dictionary["root"] = m_strProjectName;
	TSPWizardaw.m_Dictionary["Root"] = m_strProjectName;
	TSPWizardaw.m_Dictionary["ROOT"] = CString(m_strProjectName).MakeUpper();
	TSPWizardaw.m_Dictionary["Safe_root"] = projName;
	TSPWizardaw.m_Dictionary["safe_root"] = projName;
	TSPWizardaw.m_Dictionary["SAFE_ROOT"] = projName;

	// Set the filenames so the user can press Finish before the last page
	CString strBase;
	VERIFY (TSPWizardaw.m_Dictionary.Lookup("Safe_root", strBase) != 0);

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed

}// CCustom1Dlg::OnKillActive

///////////////////////////////////////////////////////////////////////////
// CCustom0Dlg::OnSetActive
//
// This is called when the user presses "Prev" to move back to this pane.
//
BOOL CCustom0Dlg::OnSetActive()
{
	// Turn on the finish button
	TSPWizardaw.SetWizardButtons(PSWIZB_NEXT);
	return TRUE;

}// CCustom0Dlg::OnSetActive

///////////////////////////////////////////////////////////////////////////
// CCustom0Dlg::OnSelectPath
//
// This is called to display a "Browse" for folder dialog.
//
void CCustom0Dlg::OnSelectPath()
{
	UpdateData(TRUE);

	char chPath[MAX_PATH];
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));
	strncpy(chPath, m_strProjectPath, MAX_PATH);
	bi.hwndOwner = GetParent()->GetSafeHwnd();
	bi.lpszTitle = "Select the path for the project";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	bi.pszDisplayName = chPath;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		// get the name of the folder
		if (SHGetPathFromIDList(pidl, chPath))
		{
			m_strProjectPath = chPath;
			UpdateData(FALSE);
		}

		CComPtr<IMalloc> pMalloc;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
			pMalloc->Free(pidl);
	}

}// CCustom0Dlg::OnSelectPath
