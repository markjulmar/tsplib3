#if !defined(AFX_TSPWIZARDAW_H__96418C7A_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_TSPWIZARDAW_H__96418C7A_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

// TSPWizardaw.h : header file
//

#define FINISH_BUTTON (0x1b8)
#define LAST_DLG 6

class CParser;

// Primary wizard class
class CTSPWizardAppWiz : public CPropertySheet
{
public:
	CTSPWizardAppWiz(CWnd* pParentWnd = NULL);
	virtual ~CTSPWizardAppWiz();

	int Next();
	int Back();
	virtual INT_PTR DoModal();

private:
	bool ProcessFile(CParser& parser, const char* pszBuff, const char* pszFilename);
	CString ReadFileFromResource(const char* pszName);
	bool GenerateFileList();
	bool CreatePath(const char* pszPath);

protected:
	// Generated message map functions
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CMapStringToString m_Dictionary;
protected:
	HICON m_hIcon;
};

// This declares the one instance of the CTSPWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global TSPWizardaw.  (Its definition is in TSPWizardaw.cpp.)
extern CTSPWizardAppWiz TSPWizardaw;

// This class is used as the basis for each property dialog in our
// prev/next pattern.
class CAppWizStepDlg : public CPropertyPage
{
public:
	CAppWizStepDlg(UINT nIDTemplate) : CPropertyPage(nIDTemplate) {}
protected:
	afx_msg void OnPaint();
	virtual LRESULT OnWizardNext()
	{
		if (this->OnKillActive() == FALSE)
			return -1;
		return TSPWizardaw.Next();
	}

	virtual LRESULT OnWizardBack()
	{
		return TSPWizardaw.Back();
	}
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSPWIZARDAW_H__96418C7A_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
