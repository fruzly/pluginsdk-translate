/*********************************************************************

 Copyright (C) 2024 Foxit Corporation
 All rights reserved.

 NOTICE: Foxit permits you to use, modify, and distribute this file
 in accordance with the terms of the Foxit license agreement
 accompanying it. If you have received this file from a source other
 than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.

 ---------------------------------------------------------------------

InsertText.cpp

 - Defines the entry point for the DLL application, the entry point is PlugInMain.

*********************************************************************/

// InsertText.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "InsertText.h"



BEGIN_MESSAGE_MAP(CInsertTextApp, CWinApp)
END_MESSAGE_MAP()

// CInsertTextApp construction
CInsertTextApp theApp;

CInsertTextApp::CInsertTextApp()
{

}

// The one and only CInsertTextApp object


void InsertTextExecuteProc(void* pData)
{
	//new document
	FPD_Document pPDFDoc = FPDDocNew();
	//new page
	FPD_Object pPageDict = FPDDocCreateNewPage(pPDFDoc, 0);
	if (!pPageDict) return;

	//set page rect
	FS_FloatRect pageRect;
	pageRect.left = 0;
	pageRect.bottom = 0;
	pageRect.right = 612;
	pageRect.top = 792;
	FPDDictionarySetAtRect(pPageDict, "MediaBox", pageRect);

	//new text object
	FPD_PageObject textObj = FPDTextObjectNew();

	//new textstate, set fontsize
	FPD_TextState textState = FPDTextStateNew();
	FPD_Font font = FPDDocAddStandardFont(pPDFDoc, "Times-Bold", NULL);
	FPDTextStateSetFont(textState, font);
	FPDTextStateSetFontSize(textState, 25);
	FPDTextObjectSetTextState(textObj, textState);
	FS_FLOAT	matrix[4]{ 1, 0, 0, 1 };
	FPDTextStateSetMatrix(textState, matrix);
	FPDTextStateDestroy(textState);

	//set font color
	FPD_ColorState pColorState = FPDColorStateNew();
	if (FPD_Color pFillColor = FPDColorStateGetFillColor(pColorState))
	{
		FPDColorSetColorSpace(pFillColor, FPDColorSpaceGetStockCS(FPD_CS_DEVICERGB));
	}
	if (FPD_Color pStrokeColor = FPDColorStateGetStrokeColor(pColorState))
	{
		FPDColorSetColorSpace(pStrokeColor, FPDColorSpaceGetStockCS(FPD_CS_DEVICERGB));
	}
	FS_FLOAT rgb[3];
	rgb[0] = 1.0f;
	rgb[1] = 0.0f;
	rgb[2] = 0.0f;
	FPDColorStateSetFillColor(pColorState, FPDColorSpaceGetStockCS(FPD_CS_DEVICERGB), rgb, 3);
	FPDPageObjectSetColorState(textObj, pColorState);
	FPDColorStateDestroy(pColorState);

	FS_ByteString bsText = FSByteStringNew3("Hello Word!", -1);
	//Set position
	FPDTextObjectSetPosition(textObj, 200, 400);
	FPDTextObjectSetText(textObj, bsText);
	//set text content
	FSByteStringDestroy(bsText);

	FPD_Page fpdPage = FPDPageNew();
	FPDPageLoad(fpdPage, pPDFDoc, pPageDict, TRUE);
	if (fpdPage)
	{
		FS_POSITION pos = FPDPageGetLastObjectPosition(fpdPage);
		//add text to page
		FPDPageInsertObject(fpdPage, pos, textObj);
	}
	//refresh the page content stream
	FPDPageGenerateContent(fpdPage);

	//save file
	CString wsPath = theApp.GetSaveAsFilePath();
	FS_LPCWSTR filePath = (FS_LPCWSTR)(LPCWSTR)wsPath;
	std::string path = theApp.ConvertToLPCSTRPath((LPCWSTR)wsPath);
	if (path == "")
	{
		FRSysShowMessageBox((FS_LPCWSTR)L"The save path has some problem.", MB_OK | MB_ICONWARNING,
			NULL, NULL, FRAppGetMainFrameWnd());
		return;
	}
	FPDDocSave(pPDFDoc, (FS_LPCSTR)(LPCSTR)(path.data()), FPDFCREATE_OBJECTSTREAM, TRUE);
	FPDPageObjectDestroy(textObj);

	FRDocOpenFromFile(filePath, "", TRUE, TRUE);
}

void InsertCNTextExecuteProc(void* pData)
{
	//new document
	FPD_Document pPDFDoc = FPDDocNew();
	//new page
	FPD_Object pPageDict = FPDDocCreateNewPage(pPDFDoc, 0);
	if (!pPageDict) return;

	//set page rect
	FS_FloatRect pageRect;
	pageRect.left = 0;
	pageRect.bottom = 0;
	pageRect.right = 612;
	pageRect.top = 792;
	FPDDictionarySetAtRect(pPageDict, "MediaBox", pageRect);

	//new text object
	FPD_PageObject textObj = FPDTextObjectNew();
	//new textstate, set fontsize
	FPD_TextState textState = FPDTextStateNew();
	//create font
	HFONT hFont = ::CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, 0, 0, GB2312_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"SimSun");
	LOGFONTW lf;
	memset(&lf, 0, sizeof(LOGFONTW));

	::GetObject(hFont, sizeof(LOGFONTW), &lf);
	::DeleteObject(hFont);
	FPD_Font font = FPDDocAddWindowsFontW(pPDFDoc, &lf, FALSE, FALSE);

	FPDTextStateSetFont(textState, font);
	FPDTextStateSetFontSize(textState, 25);
	FPDTextObjectSetTextState(textObj, textState);
	FS_FLOAT	matrix[4]{ 1, 0, 0, 1 };
	FPDTextStateSetMatrix(textState, matrix);
	FPDTextStateDestroy(textState);

	//set font color
	FPD_ColorState pColorState = FPDColorStateNew();
	if (FPD_Color pFillColor = FPDColorStateGetFillColor(pColorState))
	{
		FPDColorSetColorSpace(pFillColor, FPDColorSpaceGetStockCS(FPD_CS_DEVICERGB));
	}
	if (FPD_Color pStrokeColor = FPDColorStateGetStrokeColor(pColorState))
	{
		FPDColorSetColorSpace(pStrokeColor, FPDColorSpaceGetStockCS(FPD_CS_DEVICERGB));
	}
	FS_FLOAT rgb[3];
	rgb[0] = 1.0f;
	rgb[1] = 0.0f;
	rgb[2] = 0.0f;
	FPDColorStateSetFillColor(pColorState, FPDColorSpaceGetStockCS(FPD_CS_DEVICERGB), rgb, 3);
	FPDPageObjectSetColorState(textObj, pColorState);
	FPDColorStateDestroy(pColorState);

	FS_ByteString bsText = FSByteStringNew3("你好，欢迎来到PDF世界!", -1);
	//Set position
	FPDTextObjectSetPosition(textObj, 200, 400);

	FS_LPCWSTR content = L"你好，欢迎来到PDF世界!";
	int len = lstrlen(content);
	FS_DWORD* pCharCodes = new FS_DWORD[len + 1];
	FS_FLOAT* pKern = new FS_FLOAT[len + 1];

	for (int i = 0; i < len; i++)
	{
		pCharCodes[i] = FPDFontCharCodeFromUnicode(font, content[i]);
		if ((pCharCodes[i] == 0xFFFFFFFF) || (pCharCodes[i] == 0))
		{
			pCharCodes[i] = ' ';
		}
		pKern[i] = 0;
	}
	pCharCodes[len] = 0;

	FPDTextObjectSetText3(textObj, len, pCharCodes, pKern);
	//set text content
	FSByteStringDestroy(bsText);

	FPD_Page fpdPage = FPDPageNew();
	FPDPageLoad(fpdPage, pPDFDoc, pPageDict, TRUE);
	if (fpdPage)
	{
		FS_POSITION pos = FPDPageGetLastObjectPosition(fpdPage);
		//add text to page
		FPDPageInsertObject(fpdPage, pos, textObj);
	}
	//refresh the page content stream
	FPDPageGenerateContent(fpdPage);

	//save file
	CString wsPath = theApp.GetSaveAsFilePath();
	FS_LPCWSTR filePath = (FS_LPCWSTR)(LPCWSTR)wsPath;
	std::string path = theApp.ConvertToLPCSTRPath((LPCWSTR)wsPath);
	if (path == "")
	{
		FRSysShowMessageBox((FS_LPCWSTR)L"The save path has some problem.", MB_OK | MB_ICONWARNING,
			NULL, NULL, FRAppGetMainFrameWnd());
		return;
	}
	FPDDocSave(pPDFDoc, (FS_LPCSTR)(LPCSTR)(path.data()), FPDFCREATE_OBJECTSTREAM, TRUE);
	FPDPageObjectDestroy(textObj);

	FRDocOpenFromFile(filePath, "", TRUE, TRUE);
}


CString CInsertTextApp::GetSaveAsFilePath()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString wsfilePath = L"";
	CString csFiles = L"Files";
	CString csFilterAll;
	csFilterAll.Format(_T("PDF %s (*.pdf)|*.PDF|"), csFiles);
	CFileDialog fileDlg(FALSE, NULL,L"InsertText", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilterAll, NULL);
	if (fileDlg.DoModal() != IDOK) return wsfilePath;
	wsfilePath = fileDlg.GetPathName();
	wsfilePath += L".pdf";
	return wsfilePath;
}


std::string CInsertTextApp::ConvertToLPCSTRPath(FS_LPCWSTR wsPath)
{
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wsPath, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) return "";
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return "";
	WideCharToMultiByte(CP_UTF8, 0, wsPath, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp.c_str();
}

void CInsertTextApp::CreateElementsToNewCategory()
{
	//FS_DIBitmap fs_bitmap = CTestDemoApp::GetBmpFromRes(IDR_PDF1);

	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); //The RibbonBar will free by Foixt Reader or Phantom.
	FR_RibbonCategory fr_Category = FRRibbonBarAddCategory(fr_Bar, "TextDemo", (FS_LPCWSTR)L"TextDemo");
	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, "RibbonPanel_1", (FS_LPCWSTR)L"RibbonPanel_1", NULL);

	//Create a Ribbon button
	FS_INT32 nElementCount = FRRibbonPanelGetElementCount(fr_Panel);
	FR_RibbonButton fr_Button = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"InsertText", (FS_LPCWSTR)L"InsertText", nElementCount);

	FR_RibbonElement fr_ElementButton = FRRibbonPanelGetElementByName(fr_Panel, "InsertText");
	FRRibbonElementSetExecuteProc(fr_ElementButton, InsertTextExecuteProc);

	//Create a Ribbon button
	nElementCount = FRRibbonPanelGetElementCount(fr_Panel);
	FR_RibbonButton fr_ButtonCN = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"InsertCNText", (FS_LPCWSTR)L"InsertCNText", nElementCount);

	FR_RibbonElement fr_ElementButtonCN = FRRibbonPanelGetElementByName(fr_Panel, "InsertCNText");
	FRRibbonElementSetExecuteProc(fr_ElementButtonCN, InsertCNTextExecuteProc);
}


//////////////////////////////////////////////////////////////////////////
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

void PILoadToolBarUI(void* pParentWnd)
{

}

void PIReleaseToolBarUI(void* pParentWnd)
{

}

void PILoadRibbonUI(void* pParentWnd)
{
	theApp.CreateElementsToNewCategory();
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
	pData->PIHDRegisterPlugin(pData, "InsertText", (FS_LPCWSTR)L"InsertText");

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

