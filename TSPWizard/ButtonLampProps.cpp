/******************************************************************************/
//                                                                        
// BUTTONLAMPPROPS.CPP - Button/Lamp properties
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
#include "ButtonLampProps.h"
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
IMPLEMENT_DYNCREATE(CButtonLampProps, CDialog)

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCTSTR pszDesc;
	LPCTSTR pszTapi;
} g_LampModes[] = {
//	{ "None", "PHONELAMPMODE_DUMMY" },
	{ "Off", "PHONELAMPMODE_OFF" },
	{ "Steady", "PHONELAMPMODE_STEADY" },
	{ "Winking", "PHONELAMPMODE_WINK" },
	{ "Flashing", "PHONELAMPMODE_FLASH" },
	{ "Flutter", "PHONELAMPMODE_FLUTTER" },
	{ "Broken flutter", "PHONELAMPMODE_BROKENFLUTTER" },
	{ NULL, NULL }
};

static struct
{
	LPCTSTR pszDesc;
	LPCTSTR pszTapi;
} g_ButtonModes[] = {
	{ "Lamp only", "PHONEBUTTONMODE_DUMMY" },
	{ "Call Appearance", "PHONEBUTTONMODE_CALL" },
	{ "Switch Feature", "PHONEBUTTONMODE_FEATURE" },
	{ "Local Station Feature", "PHONEBUTTONMODE_LOCAL" },
	{ "Display Button", "PHONEBUTTONMODE_DISPLAY" },
	{ NULL, NULL }
};

static struct
{
	bool fDisplay;
	bool fSwitch;
	bool fLocal;
	LPCTSTR pszDesc;
	LPCTSTR pszTapi;
} g_ButtonFeatures[] = {
	{ false, false, false,  "None", "PHONEBUTTONFUNCTION_NONE" },
	{ true, true, true,  "Not listed", "PHONEBUTTONFUNCTION_UNKNOWN" },
	{ false, true, false, "Conference", "PHONEBUTTONFUNCTION_CONFERENCE" },
	{ false, true, false, "Transfer", "PHONEBUTTONFUNCTION_TRANSFER" },
	{ false, true, false, "Drop call", "PHONEBUTTONFUNCTION_DROP" },
	{ false, true, false, "Hold", "PHONEBUTTONFUNCTION_HOLD" },
	{ false, true, false, "Recall", "PHONEBUTTONFUNCTION_RECALL" },
	{ false, true, false, "Disconnect", "PHONEBUTTONFUNCTION_DISCONNECT" },
	{ false, true, false, "Reconnect", "PHONEBUTTONFUNCTION_CONNECT" },
	{ false, true, false, "MsgWaitOn", "PHONEBUTTONFUNCTION_MSGWAITON" },
	{ false, true, false, "MsgWaitOff", "PHONEBUTTONFUNCTION_MSGWAITOFF" },
	{ false, false, true, "Select Ring", "PHONEBUTTONFUNCTION_SELECTRING" },
	{ false, false, true, "Speed Dial", "PHONEBUTTONFUNCTION_ABBREVDIAL" },
	{ false, true, false, "Forward", "PHONEBUTTONFUNCTION_FORWARD" },
	{ false, true, false, "Pickup", "PHONEBUTTONFUNCTION_PICKUP" },
	{ false, true, false, "Ring Again", "PHONEBUTTONFUNCTION_RINGAGAIN" },
	{ false, true, false, "Park", "PHONEBUTTONFUNCTION_PARK" },
	{ false, true, false, "Reject call", "PHONEBUTTONFUNCTION_REJECT" },
	{ false, true, false, "Redirect", "PHONEBUTTONFUNCTION_REDIRECT" },
	{ false, false, true, "Mute", "PHONEBUTTONFUNCTION_MUTE" },
	{ false, false, true, "Volume Up", "PHONEBUTTONFUNCTION_VOLUMEUP" },
	{ false, false, true, "Volume Dn", "PHONEBUTTONFUNCTION_VOLUMEDOWN" },
	{ false, false, true, "Speaker On", "PHONEBUTTONFUNCTION_SPEAKERON" },
	{ false, false, true, "Speaker Off", "PHONEBUTTONFUNCTION_SPEAKEROFF" },
	{ false, false, false, "Call Appearance", "PHONEBUTTONFUNCTION_CALLAPP" },
	{ false, true, false, "Flash", "PHONEBUTTONFUNCTION_FLASH" },
	{ false, true, false, "Data On", "PHONEBUTTONFUNCTION_DATAON" },
	{ false, true, false, "Data Off", "PHONEBUTTONFUNCTION_DATAOFF" },
	{ false, true, false, "Do not disturb", "PHONEBUTTONFUNCTION_DONOTDISTURB" },
	{ false, true, false, "Intercom", "PHONEBUTTONFUNCTION_INTERCOM" },
	{ false, true, false, "Bridged Appearance", "PHONEBUTTONFUNCTION_BRIDGEDAPP" },
	{ false, true, false, "Make Busy", "PHONEBUTTONFUNCTION_BUSY" },
	{ true, true, false, "Date/Time", "PHONEBUTTONFUNCTION_DATETIME" },
	{ true, true, false, "Directory", "PHONEBUTTONFUNCTION_DIRECTORY" },
	{ false, true, false, "Cover", "PHONEBUTTONFUNCTION_COVER" },
	{ true, true, false, "Caller ID", "PHONEBUTTONFUNCTION_CALLID" },
	{ false, true, false, "Redial last number", "PHONEBUTTONFUNCTION_LASTNUM" },
	{ false, true, false, "Night Service", "PHONEBUTTONFUNCTION_NIGHTSRV" },
	{ false, true, false, "Send Calls", "PHONEBUTTONFUNCTION_SENDCALLS" },
	{ false, true, false, "MsgWait On/Off", "PHONEBUTTONFUNCTION_MSGINDICATOR" },
	{ false, true, false, "Repetory Dialing", "PHONEBUTTONFUNCTION_REPDIAL" },
	{ false, true, false, "Set RepDial", "PHONEBUTTONFUNCTION_SETREPDIAL" },
	{ false, true, false, "Program Speed Dial", "PHONEBUTTONFUNCTION_SYSTEMSPEED" },
	{ false, false, true, "Program Speed Dial", "PHONEBUTTONFUNCTION_STATIONSPEED" },
	{ false, true, false, "Camp On", "PHONEBUTTONFUNCTION_CAMPON" },
	{ false, false, true, "Save Repeat", "PHONEBUTTONFUNCTION_SAVEREPEAT" },
	{ false, true, false, "Queue Call", "PHONEBUTTONFUNCTION_QUEUECALL" },
	{ false, false, false, NULL }
};

typedef struct
{
	CString strName;
	int iFunction;
	int iMode;
	DWORD dwLampMode;
} BUTTONLAMP;

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CButtonLampProps, CDialog)
	//{{AFX_MSG_MAP(CButtonLampProps)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_EN_CHANGE(IDC_NAME, OnChange)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_CBN_SELCHANGE(IDC_FUNCTION, OnChange)
	ON_LBN_SELCHANGE(IDC_MODES, OnChange)
	ON_CBN_SELCHANGE(IDC_MODE, OnSelchangeMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::CButtonLampProps
//
// Constructor for the button/lamp properties
//
CButtonLampProps::CButtonLampProps() : CDialog(CButtonLampProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CButtonLampProps)
	m_fWriteButtons = FALSE;
	m_fWriteLamps = FALSE;
	m_strText = _T("");
	m_iFunction = -1;
	m_iMode = -1;
	//}}AFX_DATA_INIT

}// CButtonLampProps::CButtonLampProps

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::DoDataExchange
//
// Dialog data exchange for the dialog
//
void CButtonLampProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonLampProps)
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_MODES, m_lbLampModes);
	DDX_Control(pDX, IDC_MODE, m_cbModes);
	DDX_Control(pDX, IDC_LIST, m_lbList);
	DDX_Control(pDX, IDC_FUNCTION, m_cbFunctions);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Check(pDX, IDC_BUTTONWRITE, m_fWriteButtons);
	DDX_Check(pDX, IDC_LAMPWRITE, m_fWriteLamps);
	DDX_Text(pDX, IDC_NAME, m_strText);
	DDX_CBIndex(pDX, IDC_FUNCTION, m_iFunction);
	DDX_CBIndex(pDX, IDC_MODE, m_iMode);
	//}}AFX_DATA_MAP

}// CButtonLampProps::DoDataExchange

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnInitDialog
//
// Dialog data exchange for the dialog
//
BOOL CButtonLampProps::OnInitDialog() 
{
	// Connect the controls
	CDialog::OnInitDialog();
	
	for (int i = 0; g_LampModes[i].pszDesc != NULL; i++)
	{
		int iPos = m_lbLampModes.AddString(g_LampModes[i].pszDesc);
		m_lbLampModes.SetItemData(iPos, static_cast<LPARAM>(i));
	}

	for (i = 0; g_ButtonModes[i].pszDesc != NULL; i++)
	{
		int iPos = m_cbModes.AddString(g_ButtonModes[i].pszDesc);
		m_cbModes.SetItemData(iPos, static_cast<LPARAM>(i));
	}

	LoadButtonFeatures();
	OnChange();

	return TRUE;

}// CButtonLampProps::OnInitDialog

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::LoadButtonFeatures
//
// Load our button features based on the selected mode.
//
void CButtonLampProps::LoadButtonFeatures()
{
	UpdateData(TRUE);

	m_cbFunctions.SetCurSel(-1);
	m_cbFunctions.ResetContent();
	if (m_iMode == -1)
		return;

	int iMode = (int) m_cbModes.GetItemData(m_iMode);

	for (int i = 2; g_ButtonFeatures[i].pszDesc != NULL; i++)
	{
		if (iMode == 0 || iMode == 1 ||
			(iMode == 4 && g_ButtonFeatures[i].fDisplay) ||
			(iMode == 3 && g_ButtonFeatures[i].fLocal) ||
			(iMode == 2 && g_ButtonFeatures[i].fSwitch))
		{
			int iPos = m_cbFunctions.AddString(g_ButtonFeatures[i].pszDesc);
			m_cbFunctions.SetItemData(iPos, static_cast<LPARAM>(i));
		}
	}

	m_cbFunctions.InsertString(0, g_ButtonFeatures[1].pszDesc);
	m_cbFunctions.SetItemData(0, static_cast<LPARAM>(1));
	if (iMode == 0 || iMode == 4)
	{
		m_cbFunctions.InsertString(0, g_ButtonFeatures[0].pszDesc);
		m_cbFunctions.SetItemData(0, static_cast<LPARAM>(0));
	}

}// CButtonLampProps::LoadButtonFeatures

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnAdd
//
// Add a new button
//
void CButtonLampProps::OnAdd() 
{
	UpdateData(TRUE);
	BUTTONLAMP* pButton = new BUTTONLAMP;

	pButton->iFunction = static_cast<int>(m_cbFunctions.GetItemData(m_iFunction));
	pButton->iMode = static_cast<int>(m_cbModes.GetItemData(m_iMode));
	pButton->strName = m_strText;

	pButton->dwLampMode = 0;
	for (int i = 0; i < m_lbLampModes.GetCount(); i++)
	{
		if (m_lbLampModes.GetSel(i))
			pButton->dwLampMode |= 1;
		pButton->dwLampMode <<= 1;
	}

	int nPos = m_lbList.AddString(m_strText);
	m_lbList.SetItemDataPtr(nPos, static_cast<void*>(pButton));

}// CButtonLampProps::OnAdd 

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnChange
//
// Button information has changed
//
void CButtonLampProps::OnChange() 
{
	UpdateData(TRUE);

	if (GetFocus() == &m_cbModes ||	GetFocus() == &m_cbFunctions)
	{
		m_strText = "";
		m_lbLampModes.SelItemRange(false, 0, m_lbLampModes.GetCount());
		UpdateData(FALSE);
	}

	if (m_iMode != -1)
	{
		if (m_cbModes.GetItemData(m_iMode) == 0)
		{
			m_cbFunctions.SelectString(-1, "None");
			m_cbFunctions.EnableWindow(FALSE);
			UpdateData(TRUE);
		}
		else if (m_cbModes.GetItemData(m_iMode) == 1)
		{
			m_cbFunctions.SelectString(-1, "Call Appearance");
			m_cbFunctions.EnableWindow(FALSE);
			UpdateData(TRUE);
		}
		else
		{
			m_cbFunctions.EnableWindow(TRUE);
		}
	}
	else
	{
		m_cbFunctions.SetCurSel(-1);
		m_cbFunctions.EnableWindow(FALSE);
	}

	m_btnAdd.EnableWindow(m_iMode != -1 && m_iFunction != -1 && !m_strText.IsEmpty());

}// CButtonLampProps::OnChange

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnSelchangeList
//
// The selected button has changed
//
void CButtonLampProps::OnSelchangeList() 
{
	int iCurSel = m_lbList.GetCurSel();
	if (iCurSel == LB_ERR)
		return;

	BUTTONLAMP* pButton = static_cast<BUTTONLAMP*>(m_lbList.GetItemDataPtr(iCurSel));
	if (pButton == NULL)
		return;

	m_strText = pButton->strName;
	UpdateData(FALSE);
	m_cbModes.SelectString(-1, g_ButtonModes[pButton->iMode].pszDesc);
	OnSelchangeMode();
	m_cbFunctions.SelectString(-1, g_ButtonFeatures[pButton->iFunction].pszDesc);

	m_lbLampModes.SelItemRange(false, 0, m_lbLampModes.GetCount());
	for (int i = m_lbLampModes.GetCount(); i >= 0; i--)
	{
		if (pButton->dwLampMode & 1)
			m_lbLampModes.SetSel(i, TRUE);
		pButton->dwLampMode >>= 1;
	}

	m_btnRemove.EnableWindow(TRUE);

}// CButtonLampProps::OnSelchangeList

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnRemove
//
// Remove a button from the list
//
void CButtonLampProps::OnRemove() 
{
	int iCurSel = m_lbList.GetCurSel();
	if (iCurSel == LB_ERR)
		return;

	BUTTONLAMP* pButton = static_cast<BUTTONLAMP*>(m_lbList.GetItemDataPtr(iCurSel));
	if (pButton == NULL)
		return;

	delete pButton;
	m_lbList.DeleteString(iCurSel);
	m_lbList.SetCurSel(-1);
	m_btnRemove.EnableWindow(FALSE);

}// CButtonLampProps::OnRemove

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnOK
//
// Dismiss the dialog
//
void CButtonLampProps::OnOK() 
{
	UpdateData(TRUE);
	TSPWizardaw.m_Dictionary["BUTTON_CANWRITE"] = (m_fWriteButtons) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["LAMP_CANWRITE"] = (m_fWriteLamps) ? "Yes" : "No";

	char chBuff[50];
	CString strBuff;
	if (m_lbList.GetCount() > 0)
	{
		wsprintf(chBuff, "%d", m_lbList.GetCount());
		TSPWizardaw.m_Dictionary["BUTTONLAMP_COUNT"] = chBuff;
	}

	for (int i = 0; i < m_lbList.GetCount(); i++)
	{
		BUTTONLAMP* pButton = static_cast<BUTTONLAMP*>(m_lbList.GetItemDataPtr(i));

		CString strFlags;
		for (int x = m_lbLampModes.GetCount(); x >= 0; x--)
		{
			if (pButton->dwLampMode & 1)
			{
				if (!strFlags.IsEmpty())
					strFlags += " | ";
				strFlags += g_LampModes[static_cast<int>(m_lbLampModes.GetItemData(x))].pszTapi;
			}
			pButton->dwLampMode >>= 1;
		}
		if (strFlags.IsEmpty())
			strFlags = "PHONELAMPMODE_DUMMY";

		strBuff.Format("{ %s, %s, %s, _T(\"%s\") }",
				g_ButtonFeatures[pButton->iFunction].pszTapi,
				g_ButtonModes[pButton->iMode].pszTapi,				
				(LPCSTR) strFlags,
				(LPCSTR) pButton->strName);
		if (i != m_lbList.GetCount()-1)
			strBuff += ",";

		wsprintf(chBuff, "BUTTONLAMP_ENTRY_%d", i);
		TSPWizardaw.m_Dictionary[chBuff] = strBuff;
	}

	CDialog::OnOK();

}// CButtonLampProps::OnOK

///////////////////////////////////////////////////////////////////////////
// CButtonLampProps::OnSelchangeMode
//
// The button/lamp mode has changed
//
void CButtonLampProps::OnSelchangeMode() 
{
	LoadButtonFeatures();
	OnChange();	

}// CButtonLampProps::OnSelchangeMode
