#pragma once

// CRestartDlg dialog

class CSearchResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchResultDlg)

public:
	CSearchResultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchResultDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RESULTLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	
	afx_msg void OnSelchangeList();
	afx_msg void OnSearchClick();

public:
	virtual BOOL OnInitDialog();
	void SetResultInfo(std::map<FS_INT32, FS_PtrArray> info);
	void UpdateList();

public:
	CListBox	m_lstResults;
	CString     m_searchText;
	std::map<FS_INT32, FS_PtrArray> resultInfoMap;
};
