/*********************************************************************

 Copyright (C) 2024 Foxit Corporation
 All rights reserved.

 NOTICE: Foxit permits you to use, modify, and distribute this file
 in accordance with the terms of the Foxit license agreement
 accompanying it. If you have received this file from a source other
 than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.

 ---------------------------------------------------------------------

SearchText.cpp

 - Defines the entry point for the DLL application, the entry point is PlugInMain.

 - Skeleton .cpp file for a plug-in. It implements the basic
   handshaking methods required for all plug-ins.

*********************************************************************/


#include "stdafx.h"
#include "SearchText.h"
#include <memory>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CStarterApp

BEGIN_MESSAGE_MAP(CStarterApp, CWinApp)
	//{{AFX_MSG_MAP(CStarterApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStarterApp construction

CStarterApp::CStarterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


CStarterApp theApp;
void CStarterApp::CreateRibbonUI()
{
	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); //The RibbonBar will free by Foixt Reader or Phantom.
	FR_RibbonCategory fr_Category = FRRibbonBarAddCategory(fr_Bar, "SearchDemo", (FS_LPCWSTR)L"SearchDemo");
	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, "RibbonPanel_1", (FS_LPCWSTR)L"RibbonPanel_1", NULL);

	/*FR_RibbonButton fr_Button1 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"Search", (FS_LPCWSTR)L"Search", -1);
	FR_RibbonElement fr_ElementButton1 = FRRibbonPanelGetElementByName(fr_Panel, "Search");
	FRRibbonElementSetExecuteProc(fr_ElementButton1, theApp.SearchProc);*/

	FR_RibbonButton fr_Button3 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"SearchInPanel", (FS_LPCWSTR)L"SearchInPanel", -1);
	FR_RibbonElement fr_ElementButton3 = FRRibbonPanelGetElementByName(fr_Panel, "SearchInPanel");
	FRRibbonElementSetExecuteProc(fr_ElementButton3, theApp.SearchResultProc);
}

void CStarterApp::SearchProc(void* pData)
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FR_TextSelectTool toolSelect = FRTextSelectToolCreate(frDocument);
	FS_INT32 pageIndex = 0;
	FR_DocView docView = FRDocGetDocView(frDocument, pageIndex);
	FR_PageView frPageView = FRDocViewGetPageView(docView, pageIndex);
	FS_FloatRectArray allRectArry = FSFloatRectArrayNew();
	//查找page text
	theApp.SearchPageText(frDocument, pageIndex, L"行程", &allRectArry);

	//查找 form text
	if (theApp.HasTextField(frDocument, 0))
	{
		theApp.SearchFormText(frDocument, pageIndex, L"行程", &allRectArry);
	}

	//查找到的rect，通过 FR_TextSelectTool 工具进行高亮显示
	FRTextSelectToolAddSelect2(toolSelect, frPageView, allRectArry);
	FSFloatRectArrayDestroy(allRectArry);
	return;
}

//搜索页面text中的内容
void CStarterApp::SearchPageText(FR_Document frDocument, FS_INT32 pageIndex, FS_LPWSTR text, FS_FloatRectArray* textRectArr)
{
	FPD_Document pdDoc = FRDocGetPDDoc(frDocument);
	FS_INT32 count = FPDDocGetPageCount(pdDoc);

	FR_DocView docView = FRDocGetDocView(frDocument, pageIndex);
	FR_PageView frPageView = FRDocViewGetPageView(docView, pageIndex);
	FPD_Page pPage = FRPageViewGetPDPage(frPageView);
	FPD_TextPage textPage = FPDTextPageNew(pPage, pageIndex);
	FPDTextPageParseTextPage(textPage);
	if (FPDTextPageIsParsered)
		FPDTextPageParseTextPage(textPage);
	FPD_TextPageFind textFind = FPDTextPageFindNew(textPage);
	FS_BOOL bFirst = FPDTextPageFindFindFirst(textFind, text, 0, 0);
	while (FPDTextPageFindFindNext(textFind))
	{
		FS_FloatRectArray rectArry = FSFloatRectArrayNew();
		FPDTextPageFindGetRectArray(textFind, &rectArry);
		FSFloatRectArrayAppend(*textRectArr, rectArry);

		/*FS_WideString wsText = FSWideStringNew();
		FS_FloatRect rect = FSFloatRectArrayGetAt(rectArry, 0);
		FPDTextPageGetTextByRect(textPage, rect, &wsText);
		CString csText = (FS_LPCWSTR)FSWideStringCastToLPCWSTR(wsText);
		FSWideStringDestroy(wsText);*/

		FSFloatRectArrayDestroy(rectArry);
	}

	FPDTextPageFindDestroy(textFind);
}

//搜索form text中的内容
void CStarterApp::SearchFormText(FR_Document frDocument, FS_INT32 pageIndex, FS_LPWSTR text, FS_FloatRectArray* textRectArr)
{
	FPD_Document pdDoc = FRDocGetPDDoc(frDocument);
	FR_DocView docView = FRDocGetDocView(frDocument, pageIndex);
	FR_PageView frPageView = FRDocViewGetPageView(docView, pageIndex);
	FPD_Page pdPage = FRPageViewGetPDPage(frPageView);


	FR_FormTextFind formFind = FRFormTextFindCreateFormTextFind(pdDoc, pdPage);
	FS_BOOL bFirst = FRFormTextFindFindFirst(formFind, text, 0);
	while (FRFormTextFindFindNext(formFind))
	{
		FS_FloatRectArray rectArry = FSFloatRectArrayNew();
		FRFormTextFindGetRectArray(formFind, &rectArry);
		FSFloatRectArrayAppend(*textRectArr, rectArry);
		FSFloatRectArrayDestroy(rectArry);
	}
	FRFormTextFindDeleteFormTextFind(formFind);
}


struct FreeFPDPage
{
	inline void operator()(FPD_Page page) const
	{
		FPDPageDestroy(page);
	}
};

//查找当前页面是否有 form text
FS_BOOL CStarterApp::HasTextField(FR_Document frDoc, FS_INT32 pageIndex)
{
	FR_ReaderInterform rdInterFrom = FRDocGetInterForm(frDoc);
	if (!rdInterFrom) return FALSE;
	FPD_InterForm fpdInterForm = FRReaderInterformGetInterForm(rdInterFrom);
	if (!fpdInterForm) return FALSE;

	FPD_Document pdDoc = FRDocGetPDDoc(frDoc);
	FPD_Object pageDict = FPDDocGetPage(pdDoc, pageIndex);

	std::unique_ptr<_t_FPD_Page, FreeFPDPage> pPage;
	pPage.reset(FPDPageNew());
	FPDPageLoad(pPage.get(), pdDoc, pageDict, FALSE);
	FPDPageParseContent(pPage.get(), NULL);
	if (!FPDPageIsParsed(pPage.get()))
		return FALSE;

	FS_INT32 formCount = FPDInterFormCountPageControls(fpdInterForm, pPage.get());
	for (int idx = 0; idx < formCount; idx++)
	{
		FPD_FormControl formControl = FPDInterFormGetPageControl(fpdInterForm, pPage.get(), idx);
		FPD_FormField fpd_formField = FPDFormControlGetField(formControl);
		FPD_FormFieldType formType = FPDFormControlGetType(formControl);
		if (formType == FPD_FormFieldType::FormField_TextField)
			return TRUE;
	}
	return FALSE;
}

void CStarterApp::SearchResultProc(void* pData)
{
	//显示右侧搜索结果面板
	FR_ToolFormatMgr pFormatMgr = FRToolFormatMgrGet(nullptr);
	if (!pFormatMgr) return;

	FS_WideString wsFormatTitle = FSWideStringNew();
	FSWideStringFill(wsFormatTitle, L"CustomRigntPanel");

	std::string formatName = "FormatTest";
	FS_ByteString sFormatName = FSByteStringNew3(formatName.c_str(), formatName.size());

	if (FRToolFormatMgrIsToolFormatExist(pFormatMgr, sFormatName))
	{
		AFX_MANAGE_STATE(AfxGetAppModuleState());
		FRToolFormatMgrShowToolFormat(pFormatMgr, sFormatName, wsFormatTitle);
		FSWideStringDestroy(wsFormatTitle);
		FSByteStringDestroy(sFormatName);
		return;
	}
	FS_ByteString sGroupName = FSByteStringNew();
	FSByteStringFill(sGroupName, "Group1");

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	theApp.pRightPanelDlg = new CSearchResultDlg();
	//pRightPanelDlg->SetResultInfo(theApp.resultInfoMap);
	theApp.pRightPanelDlg->Create(CSearchResultDlg::IDD, NULL);

	FS_WideString wsGroupName = FSWideStringNew();
	FSWideStringFill(wsGroupName, L"SearchResult");
	FRToolFormatMgrAddGroup(pFormatMgr, sGroupName, wsGroupName, theApp.pRightPanelDlg->GetSafeHwnd());
	FRToolFormatMgrAddGroupToFormat(pFormatMgr, sFormatName, sGroupName);
	{
		AFX_MANAGE_STATE(AfxGetAppModuleState());
		FRToolFormatMgrShowToolFormat2(pFormatMgr, formatName.c_str(), FSWideStringCastToLPCWSTR(wsFormatTitle), FR_FormatType::FRUnknownFormatType, TRUE);
	}
}

void CStarterApp::SearchText(FS_LPWSTR searchtext)
{
	theApp.resultInfoMap.clear();

	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FPD_Document pdDoc = FRDocGetPDDoc(frDocument);
	FS_INT32 count = FPDDocGetPageCount(pdDoc);

	FS_LPWSTR text = searchtext;

	FR_DocView docView = FRDocGetCurrentDocView(frDocument);

	for (int i = 0; i < count; i++)
	{
		//查找 page text
		FR_PageView frPageView = FRDocViewGetPageView(docView, i);
		FPD_Page pPage = FRPageViewGetPDPage(frPageView);
		if (!FPDPageIsParsed(pPage))
		{
			FPD_Object pDict = FPDDocGetPage(pdDoc, i);
			pPage = FPDPageNew(); //initialized fpdfPage
			FPDPageLoad(pPage, pdDoc, pDict, TRUE);
			FPDPageParseContent(pPage, NULL);
		}

		FPD_TextPage textPage = FPDTextPageNew(pPage, i);
		FPDTextPageParseTextPage(textPage);
		if (FPDTextPageIsParsered)
			FPDTextPageParseTextPage(textPage);
		FPD_TextPageFind textFind = FPDTextPageFindNew(textPage);
		FS_BOOL bFirst = FPDTextPageFindFindFirst(textFind, text, 0, 0);

		FS_PtrArray resultArr = FSPtrArrayNew();
		std::vector<FindResultInfo*> vecResult;

		FS_BOOL bFind = FALSE;
		//FS_FloatRectArray rectPageArry = FSFloatRectArrayNew();
		while (FPDTextPageFindFindNext(textFind))
		{
			bFind = TRUE;
			FindResultInfo* pInfo = new FindResultInfo();
			pInfo->pageIdx_ = i;
			FS_FloatRectArray rectArry = FSFloatRectArrayNew();
			FPDTextPageFindGetRectArray(textFind, &rectArry);
			if (FSFloatRectArrayGetSize(rectArry) > 0)
			{
				pInfo->rectArray = rectArry;
			}
			else
			{
				pInfo->rectArray = NULL;
				continue;
			}
			FindTextPos textPos;
			textPos.textStartPos = FPDTextPageFindGetCurOrder(textFind);
			textPos.textEndPos = textPos.textStartPos + FPDTextPageFindGetMatchedCount(textFind) - 1;

			FS_WideString wsText = FSWideStringNew();
			FS_FloatRect rect = FSFloatRectArrayGetAt(rectArry, 0);
			FPDTextPageGetTextByRect(textPage, rect, &wsText);
			CString csText = (FS_LPCWSTR)FSWideStringCastToLPCWSTR(wsText);
			FSWideStringDestroy(wsText);
			textPos.findText = csText;
			pInfo->textPos = textPos;
			//FSPtrArrayAdd(resultArr, pInfo);
			vecResult.push_back(pInfo);
			//FSFloatRectArrayDestroy(rectArry);
		}

		/*if (bFind)
		{
			theApp.resultInfoMap.insert(std::make_pair(i, resultArr));
		}*/
		FPDTextPageFindDestroy(textFind);

		FS_BOOL bFindFormText = FALSE;
		//查找form text
		FPD_AnnotList annotList = FPDAnnotListNew(pPage);
		FS_INT32 annotcount = FPDAnnotListCount(annotList);
		if (annotcount == 0)
		{
			if (bFind)
			{
				for (auto iter = vecResult.begin(); iter != vecResult.end(); iter++)
				{
					FSPtrArrayAdd(resultArr, *iter);
				}
				theApp.resultInfoMap.insert(std::make_pair(i, resultArr));
			}
			continue;
		}

		for (int index = 0; index < annotcount; index++)
		{
			FPD_Annot pAnnot = FPDAnnotListGetAt(annotList, index);
			if (!pAnnot)
				continue;
			FPD_Form pForm = FPDAnnotGetAPForm(pAnnot, pPage, FPD_AnnotAppearanceMode::NormalAppearanceMode);
			if (!pForm)
				continue;
			FPD_TextPage textForm = FPDTextPageNew2((FS_LPVOID)pForm, 0);
			FPDTextPageParseTextPage(textForm);
			if (FPDTextPageIsParsered)
				FPDTextPageParseTextPage(textForm);
			FPD_TextPageFind textFormFind = FPDTextPageFindNew(textForm);
			FS_BOOL bFirst = FPDTextPageFindFindFirst(textFormFind, text, 0, 0);

			while (FPDTextPageFindFindNext(textFormFind))
			{
				bFindFormText = TRUE;
				FindResultInfo* pInfo = new FindResultInfo();
				pInfo->pageIdx_ = i;
				FS_FloatRectArray rectArry = FSFloatRectArrayNew();
				FPDTextPageFindGetRectArray(textFormFind, &rectArry);
				if (FSFloatRectArrayGetSize(rectArry) > 0)
				{
					pInfo->rectArray = rectArry;
				}
				else
				{
					pInfo->rectArray = NULL;
					continue;
				}

				FindTextPos textPos;
				textPos.textStartPos = FPDTextPageFindGetCurOrder(textFormFind);
				textPos.textEndPos = textPos.textStartPos + FPDTextPageFindGetMatchedCount(textFormFind) - 1;

				FS_WideString wsText = FSWideStringNew();
				FS_FloatRect rect = FSFloatRectArrayGetAt(rectArry, 0);
				FPDTextPageGetTextByRect(textForm, rect, &wsText);
				CString csText = (FS_LPCWSTR)FSWideStringCastToLPCWSTR(wsText);
				FSWideStringDestroy(wsText);
				textPos.findText = csText;
				pInfo->textPos = textPos;

				FPD_Object annotDict = FPDAnnotGetAnnotDict(pAnnot);
				FS_FloatRect annotRect = FPDDictionaryGetRect(annotDict, "Rect");
				FSFloatRectNormalize(&annotRect);

				FPD_Object formDict = FPDFormGetDict(pForm);
				if (!formDict)
					continue;

				FS_FloatRect formRect = FPDDictionaryGetRect(formDict, "BBox");
				FS_AffineMatrix form_matrix = FPDDictionaryGetMatrix(formDict, "Matrix");

				// Get form's bounding box in parent space
				FSAffineMatrixTransformRect(form_matrix, formRect);

				FS_INT32 size = FSFloatRectArrayGetSize(rectArry);
				for (int i = 0; i < size; i++)
				{
					FS_FloatRect rc = FSFloatRectArrayGetAt(rectArry, i);
					rc.left = rc.left + annotRect.left - formRect.left;
					rc.right = rc.right + annotRect.left - formRect.left;
					rc.bottom = rc.bottom + annotRect.bottom - formRect.bottom;
					rc.top = rc.top + annotRect.bottom - formRect.bottom;

					FSFloatRectArraySetAt(rectArry, i, rc);
				}
				pInfo->rectArray = rectArry;

				vecResult.push_back(pInfo);
				//FSPtrArrayAdd(resultArr, pInfo);
			}
		}


		if (bFind || bFindFormText)
		{
			int size = vecResult.size();
			std::sort(vecResult.begin(), vecResult.end(), [&](FindResultInfo* data1, FindResultInfo* data2)
			{
				FS_FloatRect rectArray1 = FSFloatRectArrayGetAt(data1->rectArray, 0);
				FS_FloatRect rectArray2 = FSFloatRectArrayGetAt(data2->rectArray, 0);

				return rectArray1.top > rectArray2.top;
			});
			for (auto iter = vecResult.begin(); iter != vecResult.end(); iter++)
			{
				FSPtrArrayAdd(resultArr, *iter);
			}

			theApp.resultInfoMap.insert(std::make_pair(i, resultArr));
		}		
	}
	pRightPanelDlg->SetResultInfo(theApp.resultInfoMap);
	pRightPanelDlg->UpdateList();
}



/////////////////////////////////////////////////////////////////////////////
// The one and only CStarterApp object


/**
	Starter is a plug-in template that provides a minimal
	implementation for a plug-in. Developers may use this plug-in a
	basis for their plug-ins.
*/



/* PIExportHFTs
** ------------------------------------------------------
**/
/**
** Create and Add the extension HFT's.
**
** @return true to continue loading plug-in,
** false to cause plug-in loading to stop.
*/
FS_BOOL PIExportHFTs(void)
{
	return TRUE;
}

/**
The application calls this function to allow it to
<ul>
<li> Import HFTs supplied by other plug-ins.
<li> Replace functions in the HFTs you're using (where allowed).
<li> Register to receive notification events.
</ul>
*/
FS_BOOL PIImportReplaceAndRegister(void)
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////
/* Plug-ins can use their Initialization procedures to hook into Foxit Reader's
	 * user interface by adding menu items, toolbar buttons, windows, and so on.
	 * It is also acceptable to modify Foxit Reader's user interface later when the plug-in is running.
	 */
void PILoadMenuBarUI(void* pParentWnd)
{

}

void PIReleaseMenuBarUI(void* pParentWnd)
{

}

//////////////////////////////////////////////////////////
void PILoadToolBarUI(void* pParentWnd)
{

}

void PIReleaseToolBarUI(void* pParentWnd)
{

}

void PILoadRibbonUI(void* pParentWnd)
{
	theApp.CreateRibbonUI();
}

void PILoadStatusBarUI(void* pParentWnd)
{

}

/* PIInit
** ------------------------------------------------------
**/
/**
	The main initialization routine.

	@return true to continue loading the plug-in,
	false to cause plug-in loading to stop.
*/
FS_BOOL PIInit(void)
{
	return TRUE;
}

/* PIUnload
** ------------------------------------------------------
**/
/**
	The unload routine.
	Called when your plug-in is being unloaded when the application quits.
	Use this routine to release any system resources you may have
	allocated.

	Returning false will cause an alert to display that unloading failed.
	@return true to indicate the plug-in unloaded.
*/
FS_BOOL PIUnload(void)
{
	return TRUE;
}

/** PIHandshake
	function provides the initial interface between your plug-in and the application.
	This function provides the callback functions to the application that allow it to
	register the plug-in with the application environment.

	Required Plug-in handshaking routine:

	@param handshakeVersion the version this plug-in works with.
	@param handshakeData OUT the data structure used to provide the primary entry points for the plug-in. These
	entry points are used in registering the plug-in with the application and allowing the plug-in to register for
	other plug-in services and offer its own.
	@return true to indicate success, false otherwise (the plug-in will not load).
*/
FS_BOOL PIHandshake(FS_INT32 handshakeVersion, void *handshakeData)
{
	if (handshakeVersion != HANDSHAKE_V0100)
		return FALSE;

	/* Cast handshakeData to the appropriate type */
	PIHandshakeData_V0100* pData = (PIHandshakeData_V0100*)handshakeData;

	/* Set the name we want to go by */
	pData->PIHDRegisterPlugin(pData, "SearchTextSample", (FS_LPCWSTR)L"SearchText");

	/* If you export your own HFT, do so in here */
	pData->PIHDSetExportHFTsCallback(pData, &PIExportHFTs);

	/*
	** If you import plug-in HFTs, replace functionality, and/or want to register for notifications before
	** the user has a chance to do anything, do so in here.
	*/
	pData->PIHDSetImportReplaceAndRegisterCallback(pData, &PIImportReplaceAndRegister);

	/* Perform your plug-in's initialization in here */
	pData->PIHDSetInitDataCallback(pData, &PIInit);

	PIInitUIProcs initUIProcs;
	INIT_CALLBACK_STRUCT(&initUIProcs, sizeof(PIInitUIProcs));
	initUIProcs.lStructSize = sizeof(PIInitUIProcs);
	initUIProcs.PILoadMenuBarUI = PILoadMenuBarUI;
	initUIProcs.PIReleaseMenuBarUI = PIReleaseMenuBarUI;
	initUIProcs.PILoadToolBarUI = PILoadToolBarUI;
	initUIProcs.PIReleaseToolBarUI = PIReleaseToolBarUI;
	initUIProcs.PILoadRibbonUI = PILoadRibbonUI;
	initUIProcs.PILoadStatusBarUI = PILoadStatusBarUI;
	pData->PIHDSetInitUICallbacks(pData, &initUIProcs);

	/* Perform any memory freeing or state saving on "quit" in here */
	pData->PIHDSetUnloadCallback(pData, &PIUnload);

	return TRUE;
}

