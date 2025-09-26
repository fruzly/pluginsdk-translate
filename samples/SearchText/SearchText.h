// Starter.h : main header file for the STARTER DLL
//

#if !defined(AFX_STARTER_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
#define AFX_STARTER_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SearchResultDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CStarterApp
// See Starter.cpp for the implementation of this class
//

struct FindTextPos
{
	int pageIdx = -1;
	int textStartPos = -1;
	int textEndPos = -1;
	CString findText = L"";
};

struct FindResultInfo
{
	int pageIdx_ = -1;
	FindTextPos textPos;
	FS_FloatRectArray rectArray;
};

class CStarterApp : public CWinApp
{
public:
	CStarterApp();

	DECLARE_MESSAGE_MAP()

public:
	void CreateRibbonUI();
	static void SearchProc(void* pData);
	static void SearchResultProc(void* pData);

	void SearchPageText(FR_Document frDocument, FS_INT32 pageIndex, FS_LPWSTR text, FS_FloatRectArray* textRectArr);
	void SearchFormText(FR_Document frDocument, FS_INT32 pageIndex, FS_LPWSTR text, FS_FloatRectArray* textRectArr);
	FS_BOOL HasTextField(FR_Document frDoc, FS_INT32 pageIndex);

	void SearchText(FS_LPWSTR searchtext);

public:
	std::map<FS_INT32, FS_FloatRectArray> rectMap;
	std::map<FS_INT32, FS_PtrArray> resultInfoMap;

public:
	CSearchResultDlg* pRightPanelDlg;
};


#endif