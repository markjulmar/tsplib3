/******************************************************************************/
//                                                                        
// DOWNLOADBUFPROPS.CPP - Download page properties
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
#include "DownloadBuffProps.h"
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
IMPLEMENT_DYNCREATE(CDownloadBuffProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CDownloadBuffProps, CDialog)
	//{{AFX_MSG_MAP(CDownloadBuffProps)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_LBN_SELCHANGE(IDC_BUFFERS, OnSelchangeBuffers)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_EN_CHANGE(IDC_BUFFERSIZE, OnChangeBuffersize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::CDownloadBuffProps
//
// Constructor for the upload buffer properties
//
CDownloadBuffProps::CDownloadBuffProps() : CDialog(CDownloadBuffProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CDownloadBuffProps)
	m_strBufferSize = _T("");
	//}}AFX_DATA_INIT

}// CDownloadBuffProps::CDownloadBuffProps

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::DoDataExchange
//
// Dialog data exchange for the upload buffer properties
//
void CDownloadBuffProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadBuffProps)
	DDX_Control(pDX, IDC_BUFFERSIZE, m_edtSize);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUFFERS, m_lbBuffers);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Text(pDX, IDC_BUFFERSIZE, m_strBufferSize);
	//}}AFX_DATA_MAP

}// CDownloadBuffProps::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::OnInitDialog
//
// Init dialog processing
//
BOOL CDownloadBuffProps::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.LoadString(IDS_DOWNLOADPROPS);
	SetWindowText(strTitle);

	m_ctlSpin1.SetRange32(0, 0xffff);

	return TRUE;

}// CDownloadBuffProps::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::OnOK
//
// Dismiss the dialog
//
void CDownloadBuffProps::OnOK() 
{
	if (m_lbBuffers.GetCount() == 0)
	{
		AfxMessageBox(IDS_NOBUFFERS);
		GotoDlgCtrl(GetDlgItem(IDC_BUFFERSIZE));
		return;
	}

	char chBuff[20], chBuff2[30];
	wsprintf(chBuff, "%d", m_lbBuffers.GetCount());
	TSPWizardaw.m_Dictionary["DOWNLOADBUFFER_COUNT"] = chBuff;
	for (int i = 0; i < m_lbBuffers.GetCount(); i++)
	{
		int iSize = static_cast<int>(m_lbBuffers.GetItemData(i));		
		wsprintf(chBuff, "%d", iSize);
		wsprintf(chBuff2, "DOWNLOADBUFFER_SIZE_%d", i);
		TSPWizardaw.m_Dictionary[chBuff2] = chBuff;
	}

	CDialog::OnOK();

}// CDownloadBuffProps::OnOK

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::OnAdd
//
// Add the buffer to the dialog
//
void CDownloadBuffProps::OnAdd() 
{
	UpdateData(TRUE);
	if (!m_strBufferSize.IsEmpty())
	{
		char chBuff[30];
		wsprintf(chBuff, "Buffer #%d - %d bytes", m_lbBuffers.GetCount() + 1, atoi(m_strBufferSize));
		int iPos = m_lbBuffers.AddString(chBuff);
		m_lbBuffers.SetItemData(iPos, static_cast<LPARAM>(atoi(m_strBufferSize)));
		m_edtSize.SetSel(0, -1, FALSE);
	}

}// CDownloadBuffProps::OnAdd

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::OnSelchangeBuffers
//
// Enable/Disable the Remove button
//
void CDownloadBuffProps::OnSelchangeBuffers() 
{
	m_btnRemove.EnableWindow(m_lbBuffers.GetCurSel() != LB_ERR);

}// CDownloadBuffProps::OnSelchangeBuffers

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::OnRemove
//
// Remove an entry from the list
//
void CDownloadBuffProps::OnRemove() 
{
	int nCurSel = m_lbBuffers.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	m_lbBuffers.DeleteString(nCurSel);

	for (int i = 0; i < m_lbBuffers.GetCount(); i++)
	{
		char chBuff[30];
		int iSize = static_cast<int>(m_lbBuffers.GetItemData(i));
		wsprintf(chBuff, "Buffer #%d - %d bytes", i, iSize);
		m_lbBuffers.DeleteString(i);
		m_lbBuffers.InsertString(i, chBuff);
		m_lbBuffers.SetItemData(i, static_cast<LPARAM>(iSize));
	}

	OnSelchangeBuffers();

}// CDownloadBuffProps::OnRemove

///////////////////////////////////////////////////////////////////////////
// CDownloadBuffProps::OnChangeBuffersize
//
// Enable/Disable the Add button
//
void CDownloadBuffProps::OnChangeBuffersize() 
{
	if (m_btnAdd.GetSafeHwnd())
	{
		UpdateData(TRUE);
		m_btnAdd.EnableWindow(m_strBufferSize.IsEmpty() == false);
	}

}// CDownloadBuffProps::OnChangeBuffersize
