// RestartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DemoDlg.h"
#include "afxdialogex.h"


// CRestartDlg dialog

IMPLEMENT_DYNAMIC(CGlobalDlg, CDialog)

CGlobalDlg::CGlobalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlobalDlg::IDD, pParent)
{

}

CGlobalDlg::~CGlobalDlg()
{
}

void CGlobalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGlobalDlg, CDialog)
	
END_MESSAGE_MAP()


BOOL CGlobalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	FRLanguageTranslateDialog(g_pLanguage, this->GetSafeHwnd(), (FS_LPCWSTR)MAKEINTRESOURCE(IDD));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
