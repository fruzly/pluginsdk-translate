#pragma once


// CRestartDlg dialog

class CGlobalDlg : public CDialog
{
	DECLARE_DYNAMIC(CGlobalDlg)

public:
	CGlobalDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGlobalDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GLOBAL	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	

public:
	virtual BOOL OnInitDialog();

};
