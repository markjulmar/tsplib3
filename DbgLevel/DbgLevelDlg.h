// DbgLevelDlg.h : header file
//

#if !defined(AFX_DBGLEVELDLG_H__3021FB5B_C418_11D1_BBA2_006097D5EC19__INCLUDED_)
#define AFX_DBGLEVELDLG_H__3021FB5B_C418_11D1_BBA2_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CDbgLevelDlg dialog

class CDbgLevelDlg : public CDialog
{
// Construction
public:
	CDbgLevelDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDbgLevelDlg)
	enum { IDD = IDD_DBGLEVEL_DIALOG };
	CComboBox m_cbProviders;
	CCheckListBox m_lbDebugLevel;
	int	m_iTALevel;
	int	m_iTSLevel;
	HICON m_hIcon;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbgLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD ReadProfileDWord (LPCTSTR pszSection, LPCTSTR pszEntry, DWORD dwDefault=0);
	CString ReadProfileString (LPCSTR pszSection, LPCTSTR pszEntry, LPCTSTR lpszDefault="");
	BOOL WriteProfileDWord (LPCSTR pszSection, LPCTSTR pszEntry, DWORD dwValue=0);

	// Generated message map functions
	//{{AFX_MSG(CDbgLevelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeSP();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBGLEVELDLG_H__3021FB5B_C418_11D1_BBA2_006097D5EC19__INCLUDED_)
