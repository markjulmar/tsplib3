/******************************************************************************/
//                                                                        
// CALLCOMPLETEPROPS.CPP - Call completion properties
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
#include "CallCompleteProps.h"
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
IMPLEMENT_DYNCREATE(CCallCompleteProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CCallCompleteProps, CDialog)
	//{{AFX_MSG_MAP(CCallCompleteProps)
	ON_LBN_SELCHANGE(IDC_COMPLMODE, OnSelchangeComplmode)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_EN_CHANGE(IDC_MESSAGE, OnChangeMessage)
	ON_LBN_SELCHANGE(IDC_COMPLTXT, OnSelchangeCompltxt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCTSTR pszCondition;
	LPCTSTR pszTapi;

} g_Conditions[] = {

	{ "Busy", "LINECALLCOMPLCOND_BUSY" },
	{ "No Answer", "LINECALLCOMPLCOND_NOANSWER" },
	{ NULL, NULL }
};

static struct
{
	LPCTSTR pszMode;
	LPCTSTR pszTapi;

} g_Modes[] = {

	{ "Camp On", "LINECALLCOMPLMODE_CAMPON" },
	{ "Call Back", "LINECALLCOMPLMODE_CALLBACK" },
	{ "Intrude into call", "LINECALLCOMPLMODE_INTRUDE" }, 
	{ "Leave Message", "LINECALLCOMPLMODE_MESSAGE" },
	{ NULL, NULL }
};

#if _MSC_VER < 1200
int vc5_Replace(CString& strBuff, LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	// can't have empty or NULL lpszOld
	int nSourceLen = lstrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = lstrlen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	int nTotalLen = strBuff.GetLength();
	LPTSTR lpszBuffer = strBuff.GetBuffer(nTotalLen+1);
	LPTSTR lpszStart = lpszBuffer;
	LPTSTR lpszEnd = lpszStart + nTotalLen;
	LPTSTR lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += lstrlen(lpszStart) + 1;
	}

	// Release the string
	strBuff.ReleaseBuffer();

	// if any changes were made, make them
	if (nCount > 0)
	{
		// Get the size of the new buffer
		int nNewLength =  nTotalLen + (nReplacementLen-nSourceLen)*nCount;

		LPTSTR lpszBuffer = strBuff.GetBuffer(nNewLength+1);
		lpszStart = lpszBuffer;
		lpszEnd = lpszBuffer + nTotalLen;

		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nTotalLen - (lpszTarget - lpszBuffer + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
					nBalance * sizeof(TCHAR));
				memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nTotalLen += (nReplacementLen - nSourceLen);
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}
		strBuff.ReleaseBuffer(nNewLength);
	}
	return nCount;
}
#endif

///////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::CCallCompleteProps
//
// Constructor for the call completion dialog
//
CCallCompleteProps::CCallCompleteProps() : CDialog(CCallCompleteProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CCallCompleteProps)
	m_strMessage = _T("");
	m_strMaxCallCompl = _T("20");
	//}}AFX_DATA_INIT

}// CCallCompleteProps::CCallCompleteProps

///////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::DoDataExchange
//
// Dialog data exchange for the dialog
//
void CCallCompleteProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallCompleteProps)
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_TYPES, m_lbTypes);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_MESSAGE, m_edtMessage);
	DDX_Control(pDX, IDC_COMPLTXT, m_lbText);
	DDX_Control(pDX, IDC_COMPLMODE, m_lbModes);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_MAXCALLCOMPL, m_strMaxCallCompl);
	//}}AFX_DATA_MAP

}// CCallCompleteProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CCallCompleteProps::OnInitDialog() 
{
	// Connect up the controls
	CDialog::OnInitDialog();
	
	// Load our listboxes
	for (int i = 0; g_Conditions[i].pszCondition != NULL; i++)
	{
		int nPos = m_lbTypes.AddString(g_Conditions[i].pszCondition);
		m_lbTypes.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	for (i = 0; g_Modes[i].pszMode != NULL; i++)
	{
		int nPos = m_lbModes.AddString(g_Modes[i].pszMode);
		m_lbModes.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	m_ctlSpin1.SetRange32(1, 50000);

	return TRUE;

}// CCallCompleteProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnOK
//
// Dismiss the dialog
//
void CCallCompleteProps::OnOK() 
{
	if (m_strMaxCallCompl.IsEmpty())
	{
		AfxMessageBox(IDS_CANNOTBEBLANK);
		GotoDlgCtrl(GetDlgItem(IDC_MAXCALLCOMPL));
		return;
	}
	TSPWizardaw.m_Dictionary["CALLCOMPL_MAX"] = m_strMaxCallCompl;

	CString strText;
	for (int i = 0; i < m_lbTypes.GetCount(); i++)
	{
		if (m_lbTypes.GetSel(i))
		{
			if (!strText.IsEmpty())
				strText += " | ";
			strText += g_Conditions[static_cast<int>(m_lbTypes.GetItemData(i))].pszTapi;
		}
	}

	if (strText.IsEmpty())
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(&m_lbTypes);
		return;
	}

	TSPWizardaw.m_Dictionary["CALLCOMPL_COND"] = strText;
	strText.Empty();
	
	for (i = 0; i < m_lbModes.GetCount(); i++)
	{
		if (m_lbModes.GetSel(i))
		{
			if (!strText.IsEmpty())
				strText += " | ";
			strText += g_Modes[static_cast<int>(m_lbModes.GetItemData(i))].pszTapi;
		}
	}

	if (strText.IsEmpty())
	{
		AfxMessageBox(IDS_YOUMUSTSELECT);
		GotoDlgCtrl(&m_lbModes);
		return;
	}

	TSPWizardaw.m_Dictionary["CALLCOMPL_MODES"] = strText;

	if (m_edtMessage.IsWindowEnabled())
	{
		TSPWizardaw.m_Dictionary["CALLCOMPLMSG"] = "Yes";
		char chBuff[20];
		wsprintf(chBuff, "%d", m_lbText.GetCount());
		TSPWizardaw.m_Dictionary["CALLCOMPLMSG_COUNT"] = chBuff;

		for (i = 0; i < m_lbText.GetCount(); i++)
		{
			CString strBuff;
			m_lbText.GetText(i, strBuff);

#if _MSC_VER < 1200
			// Visual C++ 5.0 doesn't have CString::Replace
			vc5_Replace(strBuff, "\r", "\\r");
			vc5_Replace(strBuff, "\n", "\\n");
			vc5_Replace(strBuff, "\t", "\\t");
#else
			strBuff.Replace("\r", "\\r");
			strBuff.Replace("\n", "\\n");
			strBuff.Replace("\t", "\\t");
#endif

			strBuff = "{ _T(\"" + strBuff + "\") },";

			wsprintf(chBuff, "CALLCOMPLMSG_%d", i);
			TSPWizardaw.m_Dictionary[chBuff] = strBuff;
		}
	}
	else TSPWizardaw.m_Dictionary["CALLCOMPLMSG"] = "No";

	// Dismiss the dialog
	CDialog::OnOK();

}// CCallCompleteProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnSelchangeComplmode
//
// Enable/Disable our message section
//
void CCallCompleteProps::OnSelchangeComplmode() 
{
	for (int i = 0; i < m_lbModes.GetCount(); i++)
	{
		if (static_cast<int>(m_lbModes.GetItemData(i)) == 3)
		{
			if (m_lbModes.GetSel(i))
			{
				m_btnAdd.EnableWindow(TRUE);
				m_btnRemove.EnableWindow(TRUE);
				m_edtMessage.EnableWindow(TRUE);
				m_lbText.EnableWindow(TRUE);
			}
			else
			{
				m_btnAdd.EnableWindow(FALSE);
				m_btnRemove.EnableWindow(FALSE);
				m_edtMessage.EnableWindow(FALSE);
				m_lbText.EnableWindow(FALSE);
			}
			break;
		}
	}

}// CCallCompleteProps::OnSelchangeComplmode

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnAdd
//
// Add a string to the list
//
void CCallCompleteProps::OnAdd() 
{
	UpdateData(TRUE);
	if (m_strMessage.IsEmpty() == false)
	{
		m_lbText.AddString(m_strMessage);
		m_strMessage = "";
		UpdateData(FALSE);
		OnChangeMessage();
	}

}// CCallCompleteProps::OnAdd

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnRemove
//
// Remove a string from the list
//
void CCallCompleteProps::OnRemove() 
{
	int nCurSel = m_lbText.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	m_lbText.DeleteString(nCurSel);
	OnSelchangeCompltxt();

}// CCallCompleteProps::OnRemove

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnChangeMessage
//
// The edit has changed
//
void CCallCompleteProps::OnChangeMessage() 
{
	UpdateData(TRUE);
	m_btnAdd.EnableWindow(m_strMessage.IsEmpty() == false);

}// CCallCompleteProps::OnChangeMessage

/////////////////////////////////////////////////////////////////////////////
// CCallCompleteProps::OnSelchangeCompltxt
//
// The listbox has changed
//
void CCallCompleteProps::OnSelchangeCompltxt() 
{
	int nCurSel = m_lbText.GetCurSel();
	m_btnRemove.EnableWindow(nCurSel != LB_ERR);

}// CCallCompleteProps::OnSelchangeCompltxt
