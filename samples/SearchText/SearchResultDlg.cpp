// RestartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchResultDlg.h"
#include "afxdialogex.h"
#include "SearchText.h"



IMPLEMENT_DYNAMIC(CSearchResultDlg, CDialog)

CSearchResultDlg::CSearchResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchResultDlg::IDD, pParent)
{

}

CSearchResultDlg::~CSearchResultDlg()
{

}

void CSearchResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResults);
	DDX_Text(pDX, IDC_EDIT_SearchText, m_searchText);
}


BEGIN_MESSAGE_MAP(CSearchResultDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_RESULT, &CSearchResultDlg::OnSelchangeList)
	ON_BN_CLICKED(IDC_BtnSearch, &CSearchResultDlg::OnSearchClick)
END_MESSAGE_MAP()


void CSearchResultDlg::OnSelchangeList()
{
	int index = m_lstResults.GetCurSel();
	if (index == -1)
	{
		return;
	}	

	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FR_TextSelectTool toolSelect = FRTextSelectToolCreate(frDocument);


	FR_DocView docView = FRDocGetDocView(frDocument, 0);
	FindResultInfo* pSelectedItemInfo = (FindResultInfo*)m_lstResults.GetItemDataPtr(index);
	if (pSelectedItemInfo)
	{
		FR_PageView frPageView = FRDocViewGetPageView(docView, pSelectedItemInfo->pageIdx_);
		FS_FloatRectArray rectArray = pSelectedItemInfo->rectArray;
		FS_FloatRect rect = FSFloatRectArrayGetAt(rectArray, 0);
		FRTextSelectToolAddSelect2(toolSelect, frPageView, rectArray);
		FS_FLOAT scale = FRPageViewGetPageScale(frPageView);
		FRDocViewGotoPageViewByRect2(docView, pSelectedItemInfo->pageIdx_, rect, false, scale);
	}
}


// CRestartDlg message handlers
BOOL CSearchResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	
	FS_INT32 index = 0;
	for (auto iter = resultInfoMap.begin(); iter != resultInfoMap.end(); ++iter)
	{		
		FS_PtrArray infoArr = iter->second;
		FS_INT32 size = FSPtrArrayGetSize(infoArr);
		for (int i = 0; i < size; i++)
		{
			FindResultInfo* pInfo = (FindResultInfo*)FSPtrArrayGetAt(infoArr, i);
			CString text = pInfo->textPos.findText;
			m_lstResults.InsertString(index,text);
			m_lstResults.SetItemDataPtr(index, pInfo);
			index++;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchResultDlg::SetResultInfo(std::map<FS_INT32, FS_PtrArray> info)
{
	resultInfoMap = info;
}

void CSearchResultDlg::OnSearchClick()
{	
	CString tmp;
	GetDlgItem(IDC_EDIT_SearchText)->GetWindowText(tmp);
	theApp.SearchText(tmp.GetBuffer());
}

void CSearchResultDlg::UpdateList()
{
	m_lstResults.ResetContent();
	FS_INT32 index = 0;
	for (auto iter = resultInfoMap.begin(); iter != resultInfoMap.end(); ++iter)
	{
		FS_PtrArray infoArr = iter->second;
		FS_INT32 size = FSPtrArrayGetSize(infoArr);
		for (int i = 0; i < size; i++)
		{
			FindResultInfo* pInfo = (FindResultInfo*)FSPtrArrayGetAt(infoArr, i);
			if (pInfo)
			{
				CString text = pInfo->textPos.findText;
				m_lstResults.InsertString(index, text);
				m_lstResults.SetItemDataPtr(index, pInfo);
				index++;
			}
		}
	}
}