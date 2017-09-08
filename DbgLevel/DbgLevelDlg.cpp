// DbgLevelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DbgLevel.h"
#include "DbgLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR gszTelephonyKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Telephony";

struct
{
	LPCTSTR pszName;
	DWORD dwLevel;

} g_Levels[] = {

	{ _T("Basic/minimal traces"),			0x0001 },
	{ _T("TSPI entrypoints"),				0x0002 },
	{ _T("TAPI Structures"),				0x0004 },
	{ _T("Dump offset/size pointers"),		0x0008 },
	{ _T("Event notification statistics"),	0x0010 },
	{ _T("Line/Phone/Addr objects"),		0x0020 },
	{ _T("Thread create/destroy"),			0x0040 },
	{ _T("Request create/destroy"),			0x0080 },
	{ _T("Call create/destroy"),			0x0100 },
	{ _T("Call-id map create/destroy"),     0x0200 },
	{ _T("Runtime Warnings/Errors"),        0x0400 },
	{ _T("Pool worker thread create/destroy"), 0x0800 },
	{ _T("Lock/unlock of each object (very slow)"), 0x1000 },
	{ _T("Critical secton create/destroy"), 0x2000 },
	{ _T("Agent proxy messages"),           0x4000 },
	{ _T("User-defined messages"),          0x0FF00000 },
};

#define TOTAL_COUNT (sizeof(g_Levels)/sizeof(g_Levels[0]))

/////////////////////////////////////////////////////////////////////////////
// CDbgLevelDlg dialog

CDbgLevelDlg::CDbgLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDbgLevelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbgLevelDlg)
	m_iTALevel = 0;
	m_iTSLevel = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDbgLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbgLevelDlg)
	DDX_Control(pDX, IDC_SPLIST, m_cbProviders);
	DDX_Control(pDX, IDC_SPLEVEL, m_lbDebugLevel);
	DDX_CBIndex(pDX, IDC_TALEVEL, m_iTALevel);
	DDX_CBIndex(pDX, IDC_TSLEVEL, m_iTSLevel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDbgLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CDbgLevelDlg)
	ON_CBN_SELCHANGE(IDC_SPLIST, OnChangeSP)
	ON_CLBN_CHKCHANGE(IDC_SPLEVEL, OnChange)
	ON_CBN_SELCHANGE(IDC_TALEVEL, OnChange)
	ON_CBN_SELCHANGE(IDC_TSLEVEL, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbgLevelDlg message handlers

BOOL CDbgLevelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Locate all the service providers
    DWORD dwSize = sizeof(LINEPROVIDERLIST) + (sizeof(LINEPROVIDERENTRY) * 10);
    LPLINEPROVIDERLIST lpProvList = NULL;
    CStringArray arrProviders;
	CDWordArray arrProviderID;
    while (TRUE)
    {
        lpProvList = (LPLINEPROVIDERLIST) malloc(dwSize);
        if (lpProvList == NULL)
			break;

		lpProvList->dwTotalSize = dwSize;
		if (lineGetProviderList (0x00020000, lpProvList) != 0)
		{
			free(lpProvList);
			break;
		}
    
		if (lpProvList->dwNeededSize <= dwSize)
			break;

        dwSize = lpProvList->dwNeededSize;
        free(lpProvList);
        lpProvList = NULL;
    }        

    // Now parse through the provider list and build all our structures.
	if (lpProvList != NULL)
	{
		LPLINEPROVIDERENTRY lpEntry = (LPLINEPROVIDERENTRY) ((LPSTR)lpProvList + lpProvList->dwProviderListOffset);
		for (int i = 0; i < (int) lpProvList->dwNumProviders; i++)
		{
			arrProviderID.Add(lpEntry->dwPermanentProviderID);
			arrProviders.Add((LPCSTR)((LPSTR)lpProvList+lpEntry->dwProviderFilenameOffset));
			lpEntry++;
		}
		free(lpProvList);
	}

	// Now determine which providers are built with TSP++.  We do this by searching 
	// for a section in the registry which matches the given provider name.
	for (int i = 0; i < arrProviders.GetSize(); i++)
	{
		CString strBuff;
		strBuff.Format("Device%ld", arrProviderID[i]);
		CString strEntry = ReadProfileString(NULL, strBuff);
		if (strEntry.IsEmpty() == FALSE &&
			ReadProfileDWord(strEntry, "UsesTSPLib3", 0))
		{
			int iPos = m_cbProviders.AddString(strEntry);
			ASSERT(iPos != LB_ERR);
			m_cbProviders.SetItemData(iPos, arrProviderID[i]);
		}
	}

	if (m_cbProviders.GetCount() > 0)
	{
		m_cbProviders.SetCurSel(0);

		// Add all our debug information
		for (i = 0; i < TOTAL_COUNT; i++)
		{
			int iPos = m_lbDebugLevel.AddString(g_Levels[i].pszName);
			ASSERT (iPos != LB_ERR);
			m_lbDebugLevel.SetItemData(iPos, g_Levels[i].dwLevel);
		}
	}
	OnChangeSP();

	// Load our current values
	m_iTALevel = (int) ReadProfileDWord(NULL, "Tapi32DebugLevel", 0);
	m_iTSLevel = (int) ReadProfileDWord(NULL, "TapiSrvDebugLevel", 0);
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

////////////////////////////////////////////////////////////////////////////
// CDbgLevelDlg::ReadProfileString
//
// Read a string from our profile section in the registry.  This
// function is limited to 512 characters.
//
CString CDbgLevelDlg::ReadProfileString (LPCSTR pszSection, LPCTSTR pszEntry, LPCTSTR lpszDefault/*=""*/)
{
	TCHAR szBuff[512];

    // Open the master registry key.
    HKEY hTelephonyKey;
    if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, gszTelephonyKey, 0, KEY_ALL_ACCESS, &hTelephonyKey) != ERROR_SUCCESS)
        return lpszDefault;

    // Open our Provider section
    HKEY hProviderKey;
	if (pszSection != NULL)
	{
		if (RegOpenKeyEx (hTelephonyKey, pszSection, 0, KEY_ALL_ACCESS, &hProviderKey) != ERROR_SUCCESS)
		{
			RegCloseKey (hTelephonyKey);
			return lpszDefault;
		}
	} else hProviderKey = hTelephonyKey;

	// Open our device section
    DWORD dwDataSize = sizeof(szBuff), dwDataType;

	// Query the value requested.
	if (RegQueryValueEx (hProviderKey, pszEntry, 0, &dwDataType, (LPBYTE)szBuff, &dwDataSize) == ERROR_SUCCESS)
	{
		szBuff[dwDataSize] = _T('\0');
		lpszDefault = szBuff;
	}

	RegCloseKey (hProviderKey);
	if (hProviderKey != hTelephonyKey)
		RegCloseKey (hTelephonyKey);

    return lpszDefault;

}// CDbgLevelDlg::ReadProfileString

////////////////////////////////////////////////////////////////////////////
// CDbgLevelDlg::ReadProfileDWord
//
// Read a DWORD from our profile section in the registry.
//
DWORD CDbgLevelDlg::ReadProfileDWord (LPCTSTR pszSection, LPCTSTR pszEntry, DWORD dwDefault/*=0*/)
{
    // Open the master registry key.
    HKEY hTelephonyKey;
    if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, gszTelephonyKey, 0, KEY_ALL_ACCESS, &hTelephonyKey) != ERROR_SUCCESS)
		return dwDefault;

    // Open our Provider section
    HKEY hProviderKey;
	if (pszSection != NULL)
	{
		if (RegOpenKeyEx (hTelephonyKey, pszSection, 0, KEY_ALL_ACCESS, &hProviderKey) != ERROR_SUCCESS)
		{
			RegCloseKey (hTelephonyKey);
			return dwDefault;
		}
	}
	else hProviderKey = hTelephonyKey;

	// Open our device section
	DWORD dwDataSize = sizeof(DWORD), dwDataType, dwData;

	// Query the value requested.
	if (RegQueryValueEx (hProviderKey, pszEntry, 0, &dwDataType, (LPBYTE)&dwData, &dwDataSize) != ERROR_SUCCESS)
		dwData = dwDefault;
	
	RegCloseKey (hProviderKey);
	if (hProviderKey != hTelephonyKey)
		RegCloseKey (hTelephonyKey);

    return dwData;

}// CDbgLevelDlg::ReadProfileDWord

////////////////////////////////////////////////////////////////////////////
// CDbgLevelDlg::WriteProfileDWord
//
// Write a DWORD into our registry profile.
//
BOOL CDbgLevelDlg::WriteProfileDWord (LPCSTR pszSection, LPCTSTR pszEntry, DWORD dwValue)
{
    DWORD dwDisposition;

    // Attempt to create the telephony registry section - it should really exist if our
    // driver has been loaded by TAPI.
    HKEY hKeyTelephony;
    if (RegCreateKeyEx (HKEY_LOCAL_MACHINE, gszTelephonyKey, 0, "", REG_OPTION_NON_VOLATILE,
                                    KEY_ALL_ACCESS, NULL, &hKeyTelephony, &dwDisposition) != ERROR_SUCCESS)
        return FALSE;

    // Now create our provider section if necessary.
    HKEY hKeyProvider;
	if (pszSection != NULL)
	{
		if (RegCreateKeyEx (hKeyTelephony, pszSection, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
							NULL, &hKeyProvider, &dwDisposition) != ERROR_SUCCESS)
		{
			RegCloseKey (hKeyTelephony);
			return FALSE;
		}
	}
	else hKeyProvider = hKeyTelephony;

	// Store the key.
	RegSetValueEx (hKeyProvider, pszEntry, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	RegCloseKey (hKeyProvider);
	if (hKeyProvider != hKeyTelephony)
		RegCloseKey (hKeyTelephony);

	return TRUE;

}// CDbgLevelDlg::WriteProfileDWord

void CDbgLevelDlg::OnChangeSP() 
{
	UpdateData(TRUE);
	int iSel = m_cbProviders.GetCurSel();

	DWORD dwSPLevel = 0;

	if (iSel == CB_ERR)
		m_lbDebugLevel.EnableWindow(FALSE);
	else
	{
		CString strBuff;
		strBuff.Format("Device%ld", m_cbProviders.GetItemData(iSel));
		CString strEntry = ReadProfileString(NULL, strBuff);
		if (strEntry.IsEmpty() == FALSE)
		{
			dwSPLevel = ReadProfileDWord(strEntry, "DebugLevel", 0);
			m_lbDebugLevel.EnableWindow(TRUE);
		}
	}

	// Set the debug level
	for (int i = 0; i < m_lbDebugLevel.GetCount(); i++)
		m_lbDebugLevel.SetCheck(i, ((dwSPLevel & m_lbDebugLevel.GetItemData(i)) != 0));

	UpdateData(FALSE);
}

void CDbgLevelDlg::OnChange() 
{
	UpdateData(TRUE);
}

void CDbgLevelDlg::OnOK()
{
	WriteProfileDWord(NULL, "Tapi32DebugLevel", m_iTALevel);
	WriteProfileDWord(NULL, "TapiSrvDebugLevel", m_iTSLevel);

	// Write the TSP level out.
	int iSel = m_cbProviders.GetCurSel();
	if (iSel != LB_ERR)
	{
		DWORD dwLevel = 0;	
		for (int i = 0; i < m_lbDebugLevel.GetCount(); i++)
		{
			if (m_lbDebugLevel.GetCheck(i))
				dwLevel |= m_lbDebugLevel.GetItemData(i);
		}

		CString strBuff;
		strBuff.Format("Device%ld", m_cbProviders.GetItemData(iSel));
		CString strEntry = ReadProfileString(NULL, strBuff);
		if (strEntry.IsEmpty() == FALSE)
			WriteProfileDWord(strEntry, "DebugLevel", dwLevel);
	}

	CDialog::OnOK();
}