#include "stdafx.h"
#include "Document.h"
#include "menu.h"

//external window header file.
#include "DisplayPDFDlg.h"

extern CDocumentApp theApp;

// ribbon
void CRibbonHelper::OnButtonExecuteProc(void* clientDate)
{
	/*Get the active FR_Document.*/
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();

	const FS_CHAR* name = (const FS_CHAR*)clientDate;	
	if(_stricmp(name, DOCUMENT_BUTTON_NAME_EXTERNALWINDOW) == 0)
	{
		FPD_Document pdfDoc = FRDocGetPDDoc(frDocument);

		/*Show the external window.*/
		CDisplayPDFDlg dlg;
		dlg.SetPDFDoc(pdfDoc);
		dlg.DoModal();
	}	
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_CREATEFORMCONTROL) == 0)
	{
		CRibbonHelper::CreateFormControl();
	}
}

void CRibbonHelper::OnSubButtonExecuteProc(void* clientDate)
{
	const FS_CHAR* name = (const FS_CHAR*)clientDate;
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FS_BOOL bRet = FALSE;
	if (_stricmp(name, "to html") == 0)
	{
		FS_LPCWSTR  convertFilePath = L"F:\\convert.html";
		FS_LPCSTR convertType = "HTML (*.htm,*.html)|*.htm;*.html";
		bRet = FRDocConvertPdfToOtherFormat2(convertFilePath, convertType);
	}
	else if (_stricmp(name, "to docx") == 0)
	{
		FS_LPCWSTR  convertFilePath = L"F:\\convert.docx";
		FS_LPCSTR convertType = "Word Document (*.docx)|*.docx";
		bRet = FRDocConvertPdfToOtherFormat2(convertFilePath, convertType);
	}
	else if (_stricmp(name, "to png") == 0)
	{
		FS_LPCWSTR  convertFilePath = L"F:\\convert.png";
		FS_LPCSTR convertType = "PNG (*.png)|*.png";
		bRet = FRDocConvertPdfToOtherFormat2(convertFilePath, convertType);
	}
	if (bRet)
	{
		if (::IsWindowVisible(FRAppGetMainFrameWnd()))
			FRSysShowMessageBox(L"Convert success!", MB_OK | MB_ICONINFORMATION, NULL, NULL, FRAppGetMainFrameWnd());
	}
}

void CRibbonHelper::OnSubDocButtonExecuteProc(void* clientDate)
{
	const FS_CHAR* name = (const FS_CHAR*)clientDate;	
	if (_stricmp(name, DOCUMENT_BUTTON_NAME_OPENDOCUMENT) == 0)
	{
		CRibbonHelper::OpenDocument();
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_CLOSEDOCUMENT) == 0)
	{
		FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
		FPD_Document fpdDoc = FRDocGetPDDoc(frDocument);
		FR_Document frDocTemp = FRDocFromPDDoc(fpdDoc);
		bool isClosed = FRDocClose(frDocument, FALSE, TRUE, FALSE);
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_SAVEDOCUMENT) == 0)
	{
		FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
		FRDocDoSave2(frDocument, NULL, NULL, TRUE, TRUE);
		/* savs As
		FR_SaveDocOption option;
		//If need to optimize document. If true,
		//it will clear all cached appearance, when the application changed any appearance settings.
		option.bDocPDFOptimizer = TRUE;
		//If show prompt after save finished.
		option.bPromptInfo = TRUE;
		//If save as a temp file.
		option.bSaveAsTempFile = FALSE;
		//If show progress bar.
		option.bShowProgressBar = TRUE;

		CString filePath = CRibbonHelper::GetSaveAsFilePath(L"saveasTemp");
		if (filePath.IsEmpty())
			return;
		FS_LPCWSTR inputfile = (FS_LPCWSTR)(LPCWSTR)filePath;
		FRDocDoSaveAs3(frDocument, inputfile, NULL, NULL, option);
		//FRDocDoSaveAs(frDocument);*/
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_PRINTDOCUMENT) == 0)
	{
		FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
		FRDocDoPrint(frDocument);

		/*FPD_Document fpdDocument = FRDocGetPDDoc(frDocument);
		int pageCount = FPDDocGetPageCount(fpdDocument);
		FRDocPrintPages(frDocument,0, pageCount-1);*/
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_PERMISSIONDOCUMENT) == 0)
	{
		CRibbonHelper::GetSetPermission();
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_REPLACEPAGES) == 0)
	{
		CRibbonHelper::ReplacePages();
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_NEXTPAGE) == 0)
	{
		FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
		/*Get the current page view of the FR_Document.*/
		FR_DocView docView = FRDocGetCurrentDocView(frDocument);
		FR_PageView pageView = FRDocViewGetCurrentPageView(docView);

		/*Get the index of the current page view.*/
		FS_INT32 iPageIndex = FRPageViewGetPageIndex(pageView);

		/*Go to the next page view.*/
		iPageIndex += 1;
		FRDocViewGotoPageView(docView, iPageIndex++);
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_PREVIOUSPAGE) == 0)
	{
		FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
		/*Get the current page view of the FR_Document.*/
		FR_DocView docView = FRDocGetCurrentDocView(frDocument);
		FR_PageView pageView = FRDocViewGetCurrentPageView(docView);

		/*Get the page index of the current page view.*/
		FS_INT32 iPageIndex = FRPageViewGetPageIndex(pageView);

		/*Go to the previous page view.*/
		iPageIndex -= 1;
		FRDocViewGotoPageView(docView, iPageIndex++);
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_INSERTPAGES) == 0)
	{
		CRibbonHelper::InsertPages();
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_EXTRACTPAGES) == 0)
	{
		CRibbonHelper::ExtractPages();
	}
}

FS_BOOL CRibbonHelper::OnButtonMarkedProc(void* clientDate)
{
	return FALSE;
}
FS_BOOL CRibbonHelper::OnButtonEnableProc(void* clientDate)
{
	const FS_CHAR* name = (const FS_CHAR*)clientDate;

	if(_stricmp(name, DOCUMENT_BUTTON_NAME_OPENDOCUMENT) == 0)
	{
		return TRUE;
	}

	/*No document.*/
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	if (frDocument == NULL)
	{
		return FALSE;
	}

	if(_stricmp(name, DOCUMENT_BUTTON_NAME_NEXTPAGE) == 0)
	{
		FR_DocView docView = FRDocGetCurrentDocView(frDocument);	
		FR_PageView pageView = FRDocViewGetCurrentPageView(docView);	
		FS_INT32 iPageIndex = FRPageViewGetPageIndex(pageView);

		/*The next page.*/
		FS_INT32 iPageCount = FRDocViewCountPageViews(docView);
		if (iPageIndex == iPageCount - 1)
		{
			return FALSE;
		}
		return TRUE;
	}
	else if(_stricmp(name, DOCUMENT_BUTTON_NAME_PREVIOUSPAGE) == 0)
	{
		FR_DocView docView = FRDocGetCurrentDocView(frDocument);
		FR_PageView pageView = FRDocViewGetCurrentPageView(docView);
		FS_INT32 iPageIndex = FRPageViewGetPageIndex(pageView);

		/*The first page.*/
		if (iPageIndex == 0)
		{
			return FALSE;
		}
		return TRUE;
	}
	else if(_stricmp(name, DOCUMENT_BUTTON_NAME_EXTERNALWINDOW) == 0)
	{
		return TRUE;
	}
	else if(_stricmp(name, DOCUMENT_BUTTON_NAME_CLOSEDOCUMENT) == 0)
	{
		return TRUE;
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_SAVEDOCUMENT) == 0)
	{
		if(FRDocGetChangeMark(frDocument))
			return TRUE;
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_PRINTDOCUMENT) == 0)
	{		
		return TRUE;
	}
	else if (_stricmp(name, DOCUMENT_BUTTON_NAME_PERMISSIONDOCUMENT) == 0
		|| _stricmp(name, DOCUMENT_BUTTON_NAME_REPLACEPAGES) == 0
		|| _stricmp(name, DOCUMENT_BUTTON_NAME_INSERTPAGES) == 0
		|| _stricmp(name, DOCUMENT_BUTTON_NAME_EXTRACTPAGES) == 0
		|| _stricmp(name, DOCUMENT_BUTTON_NAME_CONVERTDOCUMENT) == 0
		|| _stricmp(name, DOCUMENT_BUTTON_NAME_CREATEFORMCONTROL) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

void CRibbonHelper::OnFreeDataProc(void *clientData)
{

}

void CRibbonHelper::CreateRibbonButton(FR_RibbonPanel fr_Panel, const FS_CHAR* name, FS_LPCWSTR title, FS_LPCWSTR tooltip)
{
	//Create a Ribbon button
	FS_INT32 nElementCount = FRRibbonPanelGetElementCount(fr_Panel);
	FR_RibbonButton fr_Button =  (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		name, (FS_LPCWSTR)title, nElementCount);

	FR_RibbonElement fr_ElementButton = FRRibbonPanelGetElementByName(fr_Panel, name);
	FRRibbonElementSetTooltip(fr_ElementButton, (FS_LPCWSTR)tooltip);

	FRRibbonElementSetClientData(fr_ElementButton, (void*)name, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementButton, CRibbonHelper::OnButtonEnableProc);
	FRRibbonElementSetComputeMarkedProc(fr_ElementButton, CRibbonHelper::OnButtonMarkedProc);
	FRRibbonElementSetExecuteProc(fr_ElementButton, CRibbonHelper::OnButtonExecuteProc);
}

void CRibbonHelper::CreateButtonToNewCategory()
{
	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); //The RibbonBar will free by Foixt Reader or Phantom.
	FR_RibbonCategory fr_Category = FRRibbonBarAddCategory(fr_Bar, DOCUMENT_CATEGORY_NAME, (FS_LPCWSTR)DOCUMENT_CATEGORY_TITLE);
	CreateDocumentSubButton();
	CreatePagesButton();

	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, DOCUMENT_PANEL_NAME, (FS_LPCWSTR)DOCUMENT_PANEL_TITLE, NULL);
	
	CreateRibbonButton(fr_Panel, DOCUMENT_BUTTON_NAME_EXTERNALWINDOW, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_EXTERNALWINDOW, (FS_LPCWSTR)DOCUMENT_BUTTON_TOOLTIP_EXTERNALWINDOW);	
	CreateRibbonButton(fr_Panel, DOCUMENT_BUTTON_NAME_CONVERTDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_CONVERTDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TOOLTIP_CONVERTDOCUMENT);
	CreateRibbonButton(fr_Panel, DOCUMENT_BUTTON_NAME_CREATEFORMCONTROL, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_CREATEFORMCONTROL, (FS_LPCWSTR)DOCUMENT_BUTTON_TOOLTIP_CREATEFORMCONTROL);
	CreateConvertSubButton(fr_Panel);
}

void CRibbonHelper::CreateDocumentSubButton()
{
	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); //The RibbonBar will free by Foixt Reader or Phantom.
	FR_RibbonCategory fr_Category = FRRibbonBarGetCategoryByName(fr_Bar, DOCUMENT_CATEGORY_NAME);
	if(!fr_Category)
		fr_Category = FRRibbonBarAddCategory(fr_Bar, DOCUMENT_CATEGORY_NAME, (FS_LPCWSTR)DOCUMENT_CATEGORY_TITLE);
	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, "Document", L"Document", NULL);

	CreateRibbonButton(fr_Panel, DOCUMENT_BUTTON_NAME_DOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_DOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TOOLTIP_DOCUMENT);
	FR_RibbonElement fr_ElementButton = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_DOCUMENT);
	if (!fr_ElementButton)
		return;

	FR_RibbonButton fr_SubOpenDocItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_OPENDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_OPENDOCUMENT, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubOpenDoc = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_OPENDOCUMENT);
	FRRibbonElementSetClientData(fr_ElementSubOpenDoc, (void*)DOCUMENT_BUTTON_NAME_OPENDOCUMENT, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubOpenDoc, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubOpenDoc, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubCloseDocItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_CLOSEDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_CLOSEDOCUMENT, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubCloseDoc = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_CLOSEDOCUMENT);
	FRRibbonElementSetClientData(fr_ElementSubCloseDoc, (void*)DOCUMENT_BUTTON_NAME_CLOSEDOCUMENT, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubCloseDoc, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubCloseDoc, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubSaveDocItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_SAVEDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_SAVEDOCUMENT, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubSaveDoc = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_SAVEDOCUMENT);
	FRRibbonElementSetClientData(fr_ElementSubSaveDoc, (void*)DOCUMENT_BUTTON_NAME_SAVEDOCUMENT, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubSaveDoc, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubSaveDoc, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubPrintDocItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_PRINTDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_PRINTDOCUMENT, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubPrintDoc = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_PRINTDOCUMENT);
	FRRibbonElementSetClientData(fr_ElementSubPrintDoc, (void*)DOCUMENT_BUTTON_NAME_PRINTDOCUMENT, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubPrintDoc, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubPrintDoc, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubPermissionDocItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_PERMISSIONDOCUMENT, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_PERMISSIONDOCUMENT, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubPermissionDoc = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_PERMISSIONDOCUMENT);
	FRRibbonElementSetClientData(fr_ElementSubPermissionDoc, (void*)DOCUMENT_BUTTON_NAME_PERMISSIONDOCUMENT, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubPermissionDoc, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubPermissionDoc, CRibbonHelper::OnButtonEnableProc);
}

void CRibbonHelper::CreatePagesButton()
{
	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); //The RibbonBar will free by Foixt Reader or Phantom.
	FR_RibbonCategory fr_Category = FRRibbonBarGetCategoryByName(fr_Bar, DOCUMENT_CATEGORY_NAME);
	if (!fr_Category)
		fr_Category = FRRibbonBarAddCategory(fr_Bar, DOCUMENT_CATEGORY_NAME, (FS_LPCWSTR)DOCUMENT_CATEGORY_TITLE);
	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, "Pages", L"Pages", NULL);
	CreateRibbonButton(fr_Panel, "Page", (FS_LPCWSTR)L"Page", (FS_LPCWSTR)L"Page");
	FR_RibbonElement fr_ElementButton = FRRibbonPanelGetElementByName(fr_Panel, "Page");
	if (!fr_ElementButton)
		return;

	FR_RibbonButton fr_SubNextPageItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_NEXTPAGE, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_NEXTPAGE, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubNextPage = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_NEXTPAGE);
	FRRibbonElementSetClientData(fr_ElementSubNextPage, (void*)DOCUMENT_BUTTON_NAME_NEXTPAGE, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubNextPage, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubNextPage, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubLastPageItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_PREVIOUSPAGE, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_PREVIOUSPAGE, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubLastPage = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_PREVIOUSPAGE);
	FRRibbonElementSetClientData(fr_ElementSubLastPage, (void*)DOCUMENT_BUTTON_NAME_PREVIOUSPAGE, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubLastPage, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubLastPage, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubExtractPageItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_EXTRACTPAGES, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_EXTRACTPAGES, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubExtractPage = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_EXTRACTPAGES);
	FRRibbonElementSetClientData(fr_ElementSubExtractPage, (void*)DOCUMENT_BUTTON_NAME_EXTRACTPAGES, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubExtractPage, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubExtractPage, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubReplacePageItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_REPLACEPAGES, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_REPLACEPAGES, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubReplacePage = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_REPLACEPAGES);
	FRRibbonElementSetClientData(fr_ElementSubReplacePage, (void*)DOCUMENT_BUTTON_NAME_REPLACEPAGES, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubReplacePage, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubReplacePage, CRibbonHelper::OnButtonEnableProc);

	FR_RibbonButton fr_SubInsertPageItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		DOCUMENT_BUTTON_NAME_INSERTPAGES, (FS_LPCWSTR)DOCUMENT_BUTTON_TITLE_INSERTPAGES, -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubInsertPage = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_INSERTPAGES);
	FRRibbonElementSetClientData(fr_ElementSubInsertPage, (void*)DOCUMENT_BUTTON_NAME_INSERTPAGES, CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubInsertPage, CRibbonHelper::OnSubDocButtonExecuteProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementSubInsertPage, CRibbonHelper::OnButtonEnableProc);
}


void CRibbonHelper::CreateConvertSubButton(FR_RibbonPanel fr_Panel)
{
	FR_RibbonElement fr_ElementButton = FRRibbonPanelGetElementByName(fr_Panel, DOCUMENT_BUTTON_NAME_CONVERTDOCUMENT);
	if (!fr_ElementButton)
		return;

	FR_RibbonButton fr_SubHtmlItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		"to html", (FS_LPCWSTR)L"to html", -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubHtml = FRRibbonPanelGetElementByName(fr_Panel, "to html");
	FRRibbonElementSetClientData(fr_ElementSubHtml, (void*)"to html", CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubHtml, CRibbonHelper::OnSubButtonExecuteProc);

	FR_RibbonButton fr_SubDocxItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		"to docx", (FS_LPCWSTR)L"to docx", -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubDocx = FRRibbonPanelGetElementByName(fr_Panel, "to docx");
	FRRibbonElementSetClientData(fr_ElementSubDocx, (void*)"to docx", CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubDocx, CRibbonHelper::OnSubButtonExecuteProc);

	FR_RibbonButton fr_SubPngItem = (FR_RibbonButton)FRRibbonElementAddSubItem(fr_ElementButton, FR_RIBBON_BUTTON,
		"to png", (FS_LPCWSTR)L"to png", -1, TRUE, FALSE, FALSE);
	FR_RibbonElement fr_ElementSubPng = FRRibbonPanelGetElementByName(fr_Panel, "to png");
	FRRibbonElementSetClientData(fr_ElementSubPng, (void*)"to png", CRibbonHelper::OnFreeDataProc);
	FRRibbonElementSetExecuteProc(fr_ElementSubPng, CRibbonHelper::OnSubButtonExecuteProc);
}

CString CRibbonHelper::GetOpenDocumentPath()
{
	HWND hMain = FRAppGetMainFrameWnd();

	CString csFiles = L"Files";
	CString csFilterAll;
	csFilterAll.Format(_T("PDF %s (*.pdf)|*.PDF|"), csFiles);

	LPCTSTR lpszFilter = csFilterAll.GetBuffer(csFilterAll.GetLength());
	csFilterAll.ReleaseBuffer();

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST /*| OFN_ALLOWMULTISELECT*/,
		lpszFilter, CWnd::FromHandle(hMain));
	if (theApp.DoModalOpenFileDialog(dlg) == IDOK)
	{
		POSITION  pos = dlg.GetStartPosition();
		while (pos)
		{
			CString csFileName = dlg.GetNextPathName(pos);
			return csFileName;
		}
	}
	return L"";
}

CString CRibbonHelper::GetSaveAsFilePath(CString suggestFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString wsfilePath = L"";
	CString csFiles = L"Files";
	CString csFilterAll;
	csFilterAll.Format(_T("PDF %s (*.pdf)|*.PDF|"), csFiles);
	LPCTSTR fileName = L"saveFile";
	if (!suggestFileName.IsEmpty())
		fileName = (LPCTSTR)suggestFileName;

	CFileDialog fileDlg(FALSE, NULL, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilterAll, NULL);
	if (fileDlg.DoModal() != IDOK) return wsfilePath;
	wsfilePath = fileDlg.GetPathName();
	wsfilePath += L".pdf";
	return wsfilePath;
}

void CRibbonHelper::OpenDocument()
{
	CString filePath = CRibbonHelper::GetOpenDocumentPath();
	if (filePath.IsEmpty())
		return;
	FS_LPCWSTR openFilePath = (FS_LPCWSTR)(LPCWSTR)filePath;
	FR_Document frDoc = FRDocOpenFromFile(openFilePath,"",TRUE,TRUE);
}

void CRibbonHelper::GetSetPermission()
{
	//Get,set doc permission
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FS_DWORD dPermission = FRDocGetPermissions(frDocument);
	if (!(dPermission & FR_PERM_PRINT))
	{
		if (::IsWindowVisible(FRAppGetMainFrameWnd()))
			FRSysShowMessageBox(L"There is no print permission!", MB_OK | MB_ICONINFORMATION, NULL, NULL, FRAppGetMainFrameWnd());
	}
	FS_DWORD aPermissions = dPermission & ~FR_PERM_PRINT;
	FRDocSetPermissions(frDocument, aPermissions);

	FS_BOOL bHasDocPermissions = FRDocGetPermissionsII(frDocument, FR_PERM_PRINT);
	bHasDocPermissions = FRDocGetPermissionsII(frDocument, FR_PERM_MODIFY_CONTENT);

	//Get,Set app permission,Set the corresponding APP permission of the current document. 
	//This permission information will not be written into the document. 
	//It is only for the restriction of the function permission of the corresponding
	//APP after the document is opened for the first time
	FS_DWORD dAppStyle;
	dAppStyle = FRDocGetAppPermissionsII(frDocument);
	dAppStyle &= ~FR_PERM_ANNOTATE;
	FRDocSetAppPermissions(frDocument, dAppStyle);
	bHasDocPermissions = FRDocGetAppPermissions(frDocument, FR_PERM_ANNOTATE);
	bHasDocPermissions = FRDocGetAppPermissions(frDocument, FR_PERM_MODIFY_CONTENT);
	FS_DWORD appPermissions = FRDocGetAppPermissionsII(frDocument);
}

void CRibbonHelper::ReplacePages()
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();

	//Open and parse the replaced file	
	CString filePath = CRibbonHelper::GetOpenDocumentPath();
	if (filePath.IsEmpty())
		return;
	FS_LPCWSTR inputfilePath = (FS_LPCWSTR)(LPCWSTR)filePath;
	FR_Document frsrcDocument = FRDocOpenFromFile(inputfilePath, (FS_LPCSTR)L"", false, false);
	FPD_Document fpdDocument = FRDocGetPDDoc(frsrcDocument);
	//Set the number of pages to be replaced 
	FS_WordArray arr = FSWordArrayNew();
	FSWordArrayAdd(arr, 0);
	//The second parameter is that the original file needs to be replaced from the first page
	FS_BOOL bRet = FRDocReplacePages(frDocument, 0, fpdDocument, arr);	
	FRSysShowMessageBox((FS_LPCWSTR)L"It has replace first page with the specified file.", MB_OK | MB_ICONWARNING,
		NULL, NULL, FRAppGetMainFrameWnd());
	FSWordArrayDestroy(arr);
}

void CRibbonHelper::ExtractPages()
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	//create a memory document
	FPD_Document fpdDoc = FPDDocNew();
	//Set the number of pages to be extracted
	FS_WordArray arr = FSWordArrayNew();
	FSWordArrayAdd(arr, 0);
	FS_BOOL bExtract = FRDocExtractPages(frDocument, arr, fpdDoc);
	if (bExtract)
	{
		//Save this memory file after extract	
		CString filePath = CRibbonHelper::GetSaveAsFilePath(L"extract-0");
		if (filePath.IsEmpty())
			return;
		FS_LPCWSTR inputfile = (FS_LPCWSTR)(LPCWSTR)filePath;
		FS_BOOL bSave = FPDDocSave2(fpdDoc, inputfile, 0, FALSE);
		FRDocOpenFromFile(inputfile, (FS_LPCSTR)L"", true, true);
		
		FRSysShowMessageBox((FS_LPCWSTR)L"It has extracted first page to new file", MB_OK | MB_ICONWARNING,
			NULL, NULL, FRAppGetMainFrameWnd());		
	}
	FSWordArrayDestroy(arr);
}

void CRibbonHelper::InsertPages()
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	//Open the file to be inserted 
	CString filePath = CRibbonHelper::GetOpenDocumentPath();
	if (filePath.IsEmpty())
		return;
	FS_LPCWSTR inputfile = (FS_LPCWSTR)(LPCWSTR)filePath;
	FR_Document frsrcDocument = FRDocOpenFromFile(inputfile, (FS_LPCSTR)L"", false, false);
	FPD_Document fpdDocument = FRDocGetPDDoc(frsrcDocument);
	//Set the page number of the file to be inserted
	FS_WordArray arr = FSWordArrayNew();
	FSWordArrayAdd(arr, 0);
	FS_WideString wsName = FSWideStringNew3(L"InsertDoc", -1);
	//Set the corresponding page to insert the file from the specified position 
	FS_BOOL bRet = FRDocInsertPages(frDocument, 0, fpdDocument, arr, TRUE, FALSE, FALSE, wsName, TRUE);

	FRSysShowMessageBox((FS_LPCWSTR)L"It has inserted file to first page.", MB_OK | MB_ICONWARNING,
		NULL, NULL, FRAppGetMainFrameWnd());
	FSWordArrayDestroy(arr);
	FSWideStringDestroy(wsName);
}

void CRibbonHelper::CreateFormControl()
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FS_FloatRect rect;
	rect.left = 100;
	rect.top = 600;
	rect.right = 200;
	rect.bottom = 550;
	FS_WideString wsName = FSWideStringNew3(L"pushButton111", -1);
	FS_INT32 pageIndex = 0;
	//create form control
	FR_Annot frAnnot = FRDocCreateFormControl(frDocument, pageIndex, rect, FPD_FORM_FIELDTYPE_TEXTFIELD, 90, wsName, FALSE);
	FRDocReloadPage(frDocument, pageIndex, FALSE);
	FSWideStringDestroy(wsName);
}