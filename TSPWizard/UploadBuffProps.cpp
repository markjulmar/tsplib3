/******************************************************************************/
//                                                                        
// UPLOADBUFPROPS.CPP - Upload page properties
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
#include "UploadBuffProps.h"
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
IMPLEMENT_DYNCREATE(CUploadBuffProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CUploadBuffProps, CDialog)
	//{{AFX_MSG_MAP(CUploadBuffProps)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_LBN_SELCHANGE(IDC_BUFFERS, OnSelchangeBuffers)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_EN_CHANGE(IDC_BUFFERSIZE, OnChangeBuffersize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::CUploadBuffProps
//
// Constructor for the upload buffer properties
//
CUploadBuffProps::CUploadBuffProps() : CDialog(CUploadBuffProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CUploadBuffProps)
	m_strBufferSize = _T("");
	//}}AFX_DATA_INIT

}// CUploadBuffProps::CUploadBuffProps

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::DoDataExchange
//
// Dialog data exchange for the upload buffer properties
//
void CUploadBuffProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUploadBuffProps)
	DDX_Control(pDX, IDC_BUFFERSIZE, m_edtSize);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUFFERS, m_lbBuffers);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Text(pDX, IDC_BUFFERSIZE, m_strBufferSize);
	//}}AFX_DATA_MAP

}// CUploadBuffProps::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::OnInitDialog
//
// Init dialog processing
//
BOOL CUploadBuffProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ctlSpin1.SetRange32(0, 0xffff);

	return TRUE;

}// CUploadBuffProps::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::OnOK
//
// Dismiss the dialog
//
void CUploadBuffProps::OnOK() 
{
	if (m_lbBuffers.GetCount() == 0)
	{
		AfxMessageBox(IDS_NOBUFFERS);
		GotoDlgCtrl(GetDlgItem(IDC_BUFFERSIZE));
		return;
	}

	char chBuff[20], chBuff2[30];
	wsprintf(chBuff, "%d", m_lbBuffers.GetCount());
	TSPWizardaw.m_Dictionary["UPLOADBUFFER_COUNT"] = chBuff;
	for (int i = 0; i < m_lbBuffers.GetCount(); i++)
	{
		int iSize = static_cast<int>(m_lbBuffers.GetItemData(i));		
		wsprintf(chBuff, "%d", iSize);
		wsprintf(chBuff2, "UPLOADBUFFER_SIZE_%d", i);
		TSPWizardaw.m_Dictionary[chBuff2] = chBuff;
	}

	CDialog::OnOK();

}// CUploadBuffProps::OnOK

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::OnAdd
//
// Add the buffer to the dialog
//
void CUploadBuffProps::OnAdd() 
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

}// CUploadBuffProps::OnAdd

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::OnSelchangeBuffers
//
// Enable/Disable the Remove button
//
void CUploadBuffProps::OnSelchangeBuffers() 
{
	m_btnRemove.EnableWindow(m_lbBuffers.GetCurSel() != LB_ERR);

}// CUploadBuffProps::OnSelchangeBuffers

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::OnRemove
//
// Remove an entry from the list
//
void CUploadBuffProps::OnRemove() 
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

}// CUploadBuffProps::OnRemove

///////////////////////////////////////////////////////////////////////////
// CUploadBuffProps::OnChangeBuffersize
//
// Enable/Disable the Add button
//
void CUploadBuffProps::OnChangeBuffersize() 
{
	if (m_btnAdd.GetSafeHwnd())
	{
		UpdateData(TRUE);
		m_btnAdd.EnableWindow(m_strBufferSize.IsEmpty() == false);
	}

}// CUploadBuffProps::OnChangeBuffersize
