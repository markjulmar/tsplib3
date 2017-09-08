#if !defined(AFX_CHOOSER_H__96418C86_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
#define AFX_CHOOSER_H__96418C86_D07E_11D1_BBAD_006097D5EC19__INCLUDED_

class CDialogChooser
{
public:
	CDialogChooser();
	~CDialogChooser();

	// All calls by mfcapwz.dll to CTSPWizardAppWiz::Next
	//  & CTSPWizardAppWiz::Back are delegated to these member
	//  functions, which keep track of what dialog is up
	//  now, and what to pop up next.
	CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
	CAppWizStepDlg* Back(CAppWizStepDlg* pDlg);

protected:
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSER_H__96418C86_D07E_11D1_BBAD_006097D5EC19__INCLUDED_)
