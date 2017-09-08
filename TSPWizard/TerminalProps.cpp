/******************************************************************************/
//                                                                        
// TERMINALPROPS.CPP
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
#include "TerminalProps.h"
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
IMPLEMENT_DYNCREATE(CTerminalProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CTerminalProps, CDialog)
	//{{AFX_MSG_MAP(CTerminalProps)
	ON_LBN_SELCHANGE(IDC_TERMINALS, OnSelchangeTerminals)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBAL DATA
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_Modes[] = {
	{ "Button-press events", "LINETERMMODE_BUTTONS" },
	{ "Lamp events", "LINETERMMODE_LAMPS" },
	{ "Display information", "LINETERMMODE_DISPLAY" },
	{ "Ringer control information", "LINETERMMODE_RINGER" },
	{ "Hookswitch events", "LINETERMMODE_HOOKSWITCH" },
	{ NULL, NULL }
};

static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_Devices[] = {
	{ "Phone", "LINETERMDEV_PHONE" },
	{ "Headset", "LINETERMDEV_HEADSET" },
	{ "Speaker", "LINETERMDEV_SPEAKER" },
	{ NULL, NULL }
};

static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;

} g_Sharing[] = {
	
	{ "Private", "LINETERMSHARING_PRIVATE" },
	{ "Shared Exclusive", "LINETERMSHARING_SHAREDEXCL" },
	{ "Shared Merged", "LINETERMSHARING_SHAREDCONF" },
	{ NULL, NULL }
};

typedef struct
{
	CString strName;
	int iDevice;
	int iSharing;
	int iModes;
	int iDefaultModes;
	BOOL fMedia;
} TERMINAL;

///////////////////////////////////////////////////////////////////////////
// CTerminalProps::CTerminalProps
//
// Constructor for the terminal properties
//
CTerminalProps::CTerminalProps() : CDialog(CTerminalProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CTerminalProps)
	m_strName = _T("");
	m_fMediaUniControl = FALSE;
	m_iDevice = 0;
	m_iSharing = 0;
	//}}AFX_DATA_INIT

}// CTerminalProps::CTerminalProps

///////////////////////////////////////////////////////////////////////////
// CTerminalProps::~CTerminalProps
//
// Destructor for the terminal properties
//
CTerminalProps::~CTerminalProps()
{
	for (int i = 0; i < m_arrTerminals.GetSize(); i++)
		delete (TERMINAL*) m_arrTerminals[i];
	m_arrTerminals.RemoveAll();

}// CTerminalProps::~CTerminalProps

///////////////////////////////////////////////////////////////////////////
// CTerminalProps::DoDataExchange
//
// Dialog data exchange
//
void CTerminalProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerminalProps)
	DDX_Control(pDX, IDC_DEFTERMMODES, m_lbDefTermModes);
	DDX_Control(pDX, IDC_TERMMODES, m_lbTermModes);
	DDX_Control(pDX, IDC_TERMINALS, m_lbTerminals);
	DDX_Control(pDX, IDC_SHARING, m_cbSharing);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_DEVTYPE, m_cbDevType);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Check(pDX, IDC_MEDIADIRECTION, m_fMediaUniControl);
	DDX_CBIndex(pDX, IDC_DEVTYPE, m_iDevice);
	DDX_CBIndex(pDX, IDC_SHARING, m_iSharing);
	//}}AFX_DATA_MAP

}// CTerminalProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CTerminalProps::OnInitDialog() 
{
	// Connect up our variables
	CDialog::OnInitDialog();

	// Fill in the listboxes and combo boxes
	for (int i = 0; g_Modes[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbTermModes.AddString(g_Modes[i].pszDesc);
		m_lbTermModes.SetItemData(nPos, static_cast<LPARAM>(i));
		nPos = m_lbDefTermModes.AddString(g_Modes[i].pszDesc);
		m_lbDefTermModes.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	for (i = 0; g_Devices[i].pszDesc != NULL; i++)
	{
		int nPos = m_cbDevType.AddString(g_Devices[i].pszDesc);
		m_cbDevType.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	for (i = 0; g_Sharing[i].pszDesc != NULL; i++)
	{
		int nPos = m_cbSharing.AddString(g_Sharing[i].pszDesc);
		m_cbSharing.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	return TRUE;

}// CTerminalProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps::OnSelchangeTerminals
//
// The selected terminal has changed
//
void CTerminalProps::OnSelchangeTerminals() 
{
	int iCurSel = m_lbTerminals.GetCurSel();
	if (iCurSel == LB_ERR)
		return;

	TERMINAL* pTerm = (TERMINAL*) m_arrTerminals[iCurSel];
	ASSERT (pTerm != NULL);

	m_strName = pTerm->strName;
	m_fMediaUniControl = pTerm->fMedia;
	m_iSharing = pTerm->iSharing;
	m_iDevice = pTerm->iDevice;

	int iMode = pTerm->iModes;
	m_lbTermModes.SelItemRange(false, 0, m_lbTermModes.GetCount());
	for (int i = 0; i < m_lbTermModes.GetCount(); i++)
	{
		if (iMode & 1)
			m_lbTermModes.SetSel(i);
		iMode >>= 1;
	}

	iMode = pTerm->iDefaultModes;
	m_lbDefTermModes.SelItemRange(false, 0, m_lbDefTermModes.GetCount());
	for (i = 0; i < m_lbDefTermModes.GetCount(); i++)
	{
		if (iMode & 1)
			m_lbDefTermModes.SetSel(i);
		iMode >>= 1;
	}

	UpdateData(FALSE);
	m_btnAdd.EnableWindow(FALSE);
	m_btnRemove.EnableWindow(TRUE);

}// CTerminalProps::OnSelchangeTerminals

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps::OnChangeName
//
// The name has changed
//
void CTerminalProps::OnChangeName() 
{
	m_btnAdd.EnableWindow(TRUE);
	m_btnRemove.EnableWindow(FALSE);
	m_lbTerminals.SetCurSel(-1);

}// CTerminalProps::OnChangeName

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps::OnAdd
//
// Add a new terminal to the listbox
//
void CTerminalProps::OnAdd() 
{
	UpdateData(TRUE);
	if (m_lbTerminals.FindStringExact(-1, m_strName) != LB_ERR)
	{
		AfxMessageBox(IDS_NODUPSALLOWED);
		GotoDlgCtrl(GetDlgItem(IDC_NAME));
		return;
	}

	TERMINAL* pTerm = new TERMINAL;
	pTerm->strName = m_strName;
	pTerm->fMedia = m_fMediaUniControl;
	pTerm->iSharing = m_iSharing;
	pTerm->iDevice = m_iDevice;

	pTerm->iModes = 0;
	for (int i = m_lbTermModes.GetCount()-1; i >= 0; i--)
	{
		pTerm->iModes <<= 1;
		if (m_lbTermModes.GetSel(i))
			pTerm->iModes |= 1;
	}

	pTerm->iDefaultModes = 0;
	for (i = m_lbDefTermModes.GetCount()-1; i >= 0; i--)
	{
		pTerm->iDefaultModes <<= 1;
		if (m_lbDefTermModes.GetSel(i))
			pTerm->iDefaultModes |= 1;
	}

	m_lbTerminals.AddString(m_strName);
	m_arrTerminals.Add(pTerm);

	m_strName = _T("");
	m_fMediaUniControl = FALSE;
	m_iDevice = 0;
	m_iSharing = 0;
	m_lbTermModes.SelItemRange(false, 0, m_lbTermModes.GetCount());
	m_lbDefTermModes.SelItemRange(false, 0, m_lbTermModes.GetCount());
	UpdateData(FALSE);

	m_btnAdd.EnableWindow(FALSE);
	GotoDlgCtrl(GetDlgItem(IDC_NAME));

}// CTerminalProps::OnAdd

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps::OnRemove
//
// Remove a terminal from the listbox
//
void CTerminalProps::OnRemove() 
{
	int iCurSel = m_lbTerminals.GetCurSel();
	if (iCurSel == LB_ERR)
		return;

	m_lbTerminals.DeleteString(iCurSel);
	delete (TERMINAL*) m_arrTerminals[iCurSel];
	m_arrTerminals.RemoveAt(iCurSel);
	m_lbTerminals.SetCurSel(-1);
	m_btnRemove.EnableWindow(FALSE);
	GotoDlgCtrl(&m_lbTerminals);

}// CTerminalProps::OnRemove

/////////////////////////////////////////////////////////////////////////////
// CTerminalProps::OnOK
//
// Dismiss the terminal page
//
void CTerminalProps::OnOK() 
{
	if (m_arrTerminals.GetSize() == 0)
	{
		AfxMessageBox(IDS_NOTERMINALS);
		return;
	}

	char chBuff[50];
	wsprintf(chBuff, "%d", m_arrTerminals.GetSize());
	TSPWizardaw.m_Dictionary["TERMINAL_COUNT"] = chBuff;

	for (int iTerm = 0; iTerm < m_arrTerminals.GetSize(); iTerm++)
	{
		TERMINAL* pTerm = (TERMINAL*) m_arrTerminals[iTerm];
		wsprintf(chBuff, "TERMINAL_NAME_%d", iTerm);
		TSPWizardaw.m_Dictionary[chBuff] = pTerm->strName;		

		wsprintf(chBuff, "TERMINAL_DEVTYPE_%d", iTerm);
		int nPos = static_cast<int>(m_cbDevType.GetItemData(pTerm->iDevice));
		TSPWizardaw.m_Dictionary[chBuff] = g_Devices[nPos].pszTapi;

		wsprintf(chBuff, "TERMINAL_SHARING_%d", iTerm);
		nPos = static_cast<int>(m_cbSharing.GetItemData(pTerm->iSharing));
		TSPWizardaw.m_Dictionary[chBuff] = g_Sharing[nPos].pszTapi;

		CString strModes = "";
		wsprintf(chBuff, "TERMINAL_MODES_%d", iTerm);
		int iMode = pTerm->iModes;
		for (int i = 0; i < m_lbTermModes.GetCount(); i++)
		{
			if (iMode & 1)
			{
				nPos = static_cast<int>(m_lbTermModes.GetItemData(i));
				if (!strModes.IsEmpty())
					strModes += " | ";
				strModes += g_Modes[i].pszTapi;
			}	
			iMode >>= 1;
		}

		if (!strModes.IsEmpty())
			strModes += " | ";
		if (pTerm->fMedia)
			strModes += "LINETERMMODE_MEDIATOLINE | LINETERMMODE_MEDIAFROMLINE";
		else
			strModes += "LINETERMMODE_MEDIABIDIRECT";
		TSPWizardaw.m_Dictionary[chBuff] = strModes;

		strModes = "";
		wsprintf(chBuff, "TERMINAL_DEFMODES_%d", iTerm);
		iMode = pTerm->iDefaultModes;
		for (i = 0; i < m_lbDefTermModes.GetCount(); i++)
		{
			if (iMode & 1)
			{
				nPos = static_cast<int>(m_lbDefTermModes.GetItemData(i));
				if (!strModes.IsEmpty())
					strModes += " | ";
				strModes += g_Modes[i].pszTapi;
			}	
			iMode >>= 1;
		}

		if (strModes.IsEmpty())
			strModes = "0";
		TSPWizardaw.m_Dictionary[chBuff] = strModes;
	}

	CDialog::OnOK();

}// CTerminalProps::OnOK
