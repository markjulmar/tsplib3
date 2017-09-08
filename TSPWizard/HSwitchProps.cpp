/******************************************************************************/
//                                                                        
// HSWITCHPROPS.CPP - Hookswitch properties
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
#include "HSwitchProps.h"
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
IMPLEMENT_DYNCREATE(CHSwitchProps, CDialog)

/*---------------------------------------------------------------------------*/
// MESSAGE MAP
/*---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CHSwitchProps, CDialog)
	//{{AFX_MSG_MAP(CHSwitchProps)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_CBN_SELCHANGE(IDC_DEVICE, OnSelchangeDevice)
	ON_EN_CHANGE(IDC_DEFGAIN, OnChange)
	ON_EN_CHANGE(IDC_DEFVOL, OnChange)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*---------------------------------------------------------------------------*/
// GLOBALS
/*---------------------------------------------------------------------------*/
static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;
} g_HSTypes[] = {

	{ "Handset", "PHONEHOOKSWITCHDEV_HANDSET" },
	{ "Speaker", "PHONEHOOKSWITCHDEV_SPEAKER" },
	{ "Headset", "PHONEHOOKSWITCHDEV_HEADSET" },
	{ NULL, NULL }
};

static struct
{
	LPCSTR pszDesc;
	LPCSTR pszTapi;
} g_HSModes[] = {

	{ "On hook", "PHONEHOOKSWITCHMODE_ONHOOK" },
	{ "Mic active", "PHONEHOOKSWITCHMODE_MIC" },
	{ "Speaker active", "PHONEHOOKSWITCHMODE_SPEAKER" },
	{ "Mic/Speaker active", "PHONEHOOKSWITCHMODE_MICSPEAKER" },
	{ NULL, NULL }
};

typedef struct
{
	int iDevType;
	DWORD dwAvailModes;
	DWORD dwSetModes;
	int iVolume;
	int iGain;
	BOOL fVolChange;
	BOOL fGainChange;

} HOOKSWITCH;

///////////////////////////////////////////////////////////////////////////
// CHSwitchProps::CHSwitchProps
//
// Constructor for the hookswitch properties
//
CHSwitchProps::CHSwitchProps() : CDialog(CHSwitchProps::IDD, NULL)
{
	//{{AFX_DATA_INIT(CHSwitchProps)
	m_strGain = _T("50");
	m_strVolume = _T("50");
	m_fGainChange = FALSE;
	m_fVolChange = FALSE;
	//}}AFX_DATA_INIT

}// CHSwitchProps::CHSwitchProps

///////////////////////////////////////////////////////////////////////////
// CHSwitchProps::DoDataExchange
//
// Dialog data exchange for the dialog
//
void CHSwitchProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHSwitchProps)
	DDX_Control(pDX, IDC_SPIN3, m_ctlSpin2);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpin1);
	DDX_Control(pDX, IDC_SETTABLE, m_lbSettableModes);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_LIST, m_lbList);
	DDX_Control(pDX, IDC_DEVICE, m_cbDevType);
	DDX_Control(pDX, IDC_AVAIL, m_lbAvailableModes);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Text(pDX, IDC_DEFGAIN, m_strGain);
	DDX_Text(pDX, IDC_DEFVOL, m_strVolume);
	DDX_Check(pDX, IDC_GAINCHANGE, m_fGainChange);
	DDX_Check(pDX, IDC_VOLCHANGE, m_fVolChange);
	//}}AFX_DATA_MAP

}// CHSwitchProps::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnInitDialog
//
// Initialize the dialog
//
BOOL CHSwitchProps::OnInitDialog() 
{
	// Connect the controls up
	CDialog::OnInitDialog();

	for (int i = 0; g_HSTypes[i].pszDesc != NULL; i++)
	{
		int nPos = m_cbDevType.AddString(g_HSTypes[i].pszDesc);
		m_cbDevType.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	for (i = 0; g_HSModes[i].pszDesc != NULL; i++)
	{
		int nPos = m_lbAvailableModes.AddString(g_HSModes[i].pszDesc);
		m_lbAvailableModes.SetItemData(nPos, static_cast<LPARAM>(i));
		nPos = m_lbSettableModes.AddString(g_HSModes[i].pszDesc);
		m_lbSettableModes.SetItemData(nPos, static_cast<LPARAM>(i));
	}

	m_ctlSpin1.SetRange(0, 100);
	m_ctlSpin2.SetRange(0, 100);
	((CEdit*)GetDlgItem(IDC_DEFVOL))->LimitText(3);
	((CEdit*)GetDlgItem(IDC_DEFGAIN))->LimitText(3);

	return TRUE;

}// CHSwitchProps::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnAdd
//
// Add a new hookswitch to the system
//
void CHSwitchProps::OnAdd() 
{
	UpdateData(TRUE);

	HOOKSWITCH* pHS = NULL;
	if (m_lbList.GetCurSel() != LB_ERR)
		pHS = static_cast<HOOKSWITCH*>(m_lbList.GetItemDataPtr(m_lbList.GetCurSel()));
	else
		pHS = new HOOKSWITCH;

	pHS->fGainChange = m_fGainChange;
	pHS->fVolChange = m_fVolChange;
	pHS->iDevType = static_cast<int>(m_cbDevType.GetItemData(m_cbDevType.GetCurSel()));
	pHS->iGain = atoi(m_strGain);
	pHS->iVolume = atoi(m_strVolume);

	pHS->dwAvailModes = pHS->dwSetModes = 0;
	for (int i = 0; i < m_lbAvailableModes.GetCount(); i++)
	{
		if (m_lbAvailableModes.GetSel(i))
			pHS->dwAvailModes |= 1;
		pHS->dwAvailModes <<= 1;
	}

	for (i = 0; i < m_lbSettableModes.GetCount(); i++)
	{
		if (m_lbSettableModes.GetSel(i))
			pHS->dwSetModes |= 1;
		pHS->dwSetModes <<= 1;
	}

	if (m_lbList.GetCurSel() == LB_ERR)
	{
		i = m_lbList.AddString(g_HSTypes[pHS->iDevType].pszDesc);
		m_lbList.SetItemDataPtr(i, pHS);
		m_lbList.SetCurSel(i);
	}

	OnSelchangeList();

}// CHSwitchProps::OnAdd

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnRemove
//
// Remove a hookswitch from the system
//
void CHSwitchProps::OnRemove() 
{
	int iCurSel = m_lbList.GetCurSel();
	if (iCurSel == LB_ERR)
		return;

	HOOKSWITCH* pHS = static_cast<HOOKSWITCH*>(m_lbList.GetItemDataPtr(iCurSel));
	delete pHS;
	m_lbList.DeleteString(iCurSel);
	m_lbList.SetCurSel(-1);
	OnSelchangeList();

}// CHSwitchProps::OnRemove

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnOK
//
// Dismiss the dialog
//
void CHSwitchProps::OnOK() 
{
	if (m_lbList.GetCount() == 0)
	{
		AfxMessageBox(IDS_MUSTADDDEVS);
		return;
	}

	bool fChangeGain = false;
	bool fChangeVol = false;
	bool fSettable = false;

	char chBuff[30];
	wsprintf(chBuff, "%d", m_lbList.GetCount());
	TSPWizardaw.m_Dictionary["HOOKSWITCH_COUNT"] = chBuff;

	for (int i = 0; i < m_lbList.GetCount(); i++)
	{
		HOOKSWITCH* pHS = static_cast<HOOKSWITCH*>(m_lbList.GetItemDataPtr(i));
		ASSERT (pHS != NULL);

		if (pHS->fGainChange)
			fChangeGain = true;
		if (pHS->fVolChange)
			fChangeVol = true;
		
		CString strLine, strAvailModes, strSetModes;
		for (int x = m_lbAvailableModes.GetCount(); x >= 0; x--)
		{
			if (pHS->dwAvailModes & 1)
			{
				int iPos = static_cast<int>(m_lbAvailableModes.GetItemData(x));
				if (!strAvailModes.IsEmpty())
					strAvailModes += " | ";
				strAvailModes += g_HSModes[iPos].pszTapi;
			}
			pHS->dwAvailModes >>= 1;
		}

		if (!strAvailModes.IsEmpty())
			strAvailModes += " | ";
		strAvailModes += "PHONEHOOKSWITCHMODE_UNKNOWN";

		if (pHS->dwSetModes != 0)
			fSettable = true;

		for (x = m_lbSettableModes.GetCount(); x >= 0; x--)
		{
			if (pHS->dwSetModes & 1)
			{
				int iPos = static_cast<int>(m_lbSettableModes.GetItemData(x));
				if (!strSetModes.IsEmpty())
					strSetModes += " | ";
				strSetModes += g_HSModes[iPos].pszTapi;
			}
			pHS->dwSetModes >>= 1;
		}

		if (strSetModes.IsEmpty())
			strSetModes = "0";

		unsigned int iVolume;
		if (pHS->iVolume == 0)
			iVolume = 0;
		else
		{
			iVolume = 0xffff * (unsigned int)pHS->iVolume;
			iVolume /= 100;
		}

		unsigned int iGain;
		if (pHS->iGain == 0)
			pHS->iGain = 0;
		else
		{
			iGain = 0xffff * (unsigned int)pHS->iGain;
			iGain /= 100;
		}

		strLine.Format("{ %s,\n"\
			           "      %s,\n"\
					   "      %s,\n"\
					   "      %d, %d, %s, %s }",
				g_HSTypes[pHS->iDevType].pszTapi,
				(LPCSTR) strAvailModes, (LPCSTR) strSetModes,
				iVolume, iGain, 
				(pHS->fVolChange) ? "true" : "false",
				(pHS->fGainChange) ? "true" : "false");

		if (i != m_lbList.GetCount()-1)
			strLine += ",";

		wsprintf(chBuff, "HOOKSWITCH_ENTRY_%d", i);
		TSPWizardaw.m_Dictionary[chBuff] = strLine;
	}

	TSPWizardaw.m_Dictionary["GAIN_CANWRITE"] = (fChangeGain) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["VOLUME_CANWRITE"] = (fChangeVol) ? "Yes" : "No";
	TSPWizardaw.m_Dictionary["HOOKSWITCH_CANWRITE"] = (fSettable) ? "Yes" : "No";

	CDialog::OnOK();

}// CHSwitchProps::OnOK

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnSelchangeDevice
//
// The device selection has changed, see if we already have a device
// of this type and select it.
//
void CHSwitchProps::OnSelchangeDevice() 
{
	int nPos = m_cbDevType.GetCurSel();
	if (nPos == CB_ERR)
		return;

	int iPos = static_cast<int>(m_cbDevType.GetItemData(nPos));
	for (int i = 0; i < m_lbList.GetCount(); i++)
	{
		HOOKSWITCH* pHS = static_cast<HOOKSWITCH*>(m_lbList.GetItemDataPtr(i));
		if (pHS != NULL && pHS->iDevType == iPos)
		{
			m_lbList.SetCurSel(i);
			OnSelchangeList();
			return;
		}
	}

	m_lbList.SetCurSel(LB_ERR);
	OnSelchangeList();

	m_strVolume = _T("50");
	m_strGain = _T("50");
	m_fVolChange = FALSE;
	m_fGainChange = FALSE;
	UpdateData(FALSE);

	m_lbAvailableModes.SelItemRange(false, 0, m_lbAvailableModes.GetCount());
	m_lbSettableModes.SelItemRange(false, 0, m_lbSettableModes.GetCount());

}// CHSwitchProps::OnSelchangeDevice

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnChange
//
// Enable/Disable the Add button
//
void CHSwitchProps::OnChange() 
{
	if (m_btnAdd.GetSafeHwnd())
	{
		UpdateData(TRUE);
		m_btnAdd.EnableWindow (m_cbDevType.GetCurSel() != CB_ERR &&
			!m_strVolume.IsEmpty() &&
			!m_strGain.IsEmpty());
	}

}// CHSwitchProps::OnChange

/////////////////////////////////////////////////////////////////////////////
// CHSwitchProps::OnSelchangeList
//
// Enable/Disable the Remove button
//
void CHSwitchProps::OnSelchangeList() 
{
	CString strEdit;
	int nPos = m_lbList.GetCurSel();
	if (nPos == LB_ERR)
	{
		strEdit.LoadString(IDS_ADD);
		m_btnAdd.SetWindowText(strEdit);
		m_btnRemove.EnableWindow(FALSE);
		OnChange();
		return;
	}

	strEdit.LoadString(IDS_EDIT);
	m_btnAdd.SetWindowText(strEdit);

	m_btnRemove.EnableWindow(TRUE);
	HOOKSWITCH* pHS = static_cast<HOOKSWITCH*>(m_lbList.GetItemDataPtr(nPos));
	if (pHS == NULL)
		return;

	m_cbDevType.SelectString(-1, g_HSTypes[pHS->iDevType].pszDesc);
	m_strVolume.Format("%d", pHS->iVolume);
	m_strGain.Format("%d", pHS->iGain);
	m_fVolChange = pHS->fVolChange;
	m_fGainChange = pHS->fGainChange;
	UpdateData(FALSE);

	DWORD dwValue = pHS->dwAvailModes;
	m_lbAvailableModes.SelItemRange(false, 0, m_lbAvailableModes.GetCount());
	for (int i = m_lbAvailableModes.GetCount(); i >= 0; i--)
	{
		if (dwValue & 1)
			m_lbAvailableModes.SetSel(i);
		dwValue >>= 1;
	}

	dwValue = pHS->dwSetModes;
	m_lbSettableModes.SelItemRange(false, 0, m_lbSettableModes.GetCount());
	for (i = m_lbSettableModes.GetCount(); i >= 0; i--)
	{
		if (dwValue & 1)
			m_lbSettableModes.SetSel(i);
		dwValue >>= 1;
	}

	OnChange();

}// CHSwitchProps::OnSelchangeList
