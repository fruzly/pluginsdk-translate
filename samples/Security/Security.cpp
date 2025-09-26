/*********************************************************************

 Copyright (C) 2024 Foxit Corporation
 All rights reserved.

 NOTICE: Foxit permits you to use, modify, and distribute this file
 in accordance with the terms of the Foxit license agreement
 accompanying it. If you have received this file from a source other
 than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.

 ---------------------------------------------------------------------

Security.cpp

 - Defines the entry point for the DLL application, the entry point is PlugInMain.

 - Skeleton .cpp file for a plug-in. It implements the basic
   handshaking methods required for all plug-ins.

*********************************************************************/


#include "stdafx.h"
//#include "ContentProviderCallbacks.h"
#include "include/Callbacks/FoxitContentProviderCallbacks.h"
#include "include/Callbacks/FoxitSecurityCallbacks.h"
#include "include/Callbacks/FoxitCryptoCallbacks.h"
#include "include/Callbacks/FoxitSecurityMethodCallbacks.h"
#include "UIExtention.h"
#include "Security.h"
#include "FDocInfo.h"

#define CATEGORY_SECURITY "Ribbon_Category_Security"

#define CATEGORY_CUSTOMER "Security_Sample"
#define CATEGORY_CUSTOMERW L"Security_Sample"

#define CATEGPRU_STANDARD "Standard_encryption"
#define CATEGPRU_STANDARDW L"Standard_encryption"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSecurityApp theApp;
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
// CSecurityApp

BEGIN_MESSAGE_MAP(CSecurityApp, CWinApp)
	//{{AFX_MSG_MAP(CSecurityApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecurityApp construction

CSecurityApp::CSecurityApp()
{
	fsDocInfoPtrArr = NULL;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSecurityApp object



/** 
	Security is a plug-in template that provides a minimal 
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
void ExecuteProc(void* pData)
{
	FPD_Object objEncryptDict = NULL;
	FR_Document frDoc = FRAppGetActiveDocOfPDDoc();
	objEncryptDict = FPDDictionaryNew(); // need not to destroy
	BOOL bMetaData = FALSE;
	FPDDictionarySetAtBoolean(objEncryptDict, "EncryptMetadata", bMetaData);

	
	FS_WideString fswsFilePath = FSWideStringNew();
	FSWideStringFill(fswsFilePath, (FS_LPCWSTR)"d:\\sss.pdf");
	FPD_Creator fpdCreator = FRDocGetPDFCreator(frDoc);
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FPD_Parser fpdParser = FPDDocGetParser(fpdDoc);


	FS_DWORD objNum = 0;
	objNum = FPDDocAddIndirectObject(fpdDoc, objEncryptDict);
	FPD_Object trailerDict = FPDParserGetTrailer(fpdParser);
	FPDDictionarySetAtReferenceToDoc(trailerDict, "Encrypt", fpdDoc, objNum);
	void* pSecurityData = FPDCreatorSetCustomSecurity(fpdCreator, objEncryptDict, theApp.m_pFRCryptoCallbacks.get(), bMetaData);
	

	FS_BOOL bChanged = FRDocGetChangeMark(frDoc);
	FRDocSetChangeMark(frDoc);
	FS_BOOL bSaveAsResult = FALSE;
	bSaveAsResult = FRDocDoSaveAs2(
		frDoc,
		FSWideStringCastToLPCWSTR(fswsFilePath),
		NULL,
		NULL,
		FALSE,
		FALSE);

	//FPDCreatorReleaseProgressiveEncryptHandler(fpdPEH);

	if (pSecurityData != NULL)
	{
		FPDCreatorReleaseCustomSecurityData(fpdCreator, pSecurityData);
	}

	if (!bSaveAsResult)
	{
		if (!bChanged)
			FRDocClearChangeMark(frDoc);
		//FMIPFileEncryptor::RemoveSecurityInfo(frDoc);
	}

}

FS_BOOL EnabledProc(void* pData)
{
	return TRUE;
}


void CreatePasswordCategory(FR_RibbonCategory pRibbonCategory)
{
	FR_RibbonPanel pRibbonPanel = FRRibbonCategoryGetPanelByName(pRibbonCategory, CATEGORY_CUSTOMER);
	if (!pRibbonPanel)
	{
		pRibbonPanel = FRRibbonCategoryAddPanel(pRibbonCategory, CATEGORY_CUSTOMER, CATEGORY_CUSTOMERW, NULL);
		if (pRibbonPanel)
		{

			if (FR_RibbonButton pRibbonButton = (FR_RibbonButton)FRRibbonPanelAddElement(pRibbonPanel, FR_RIBBON_BUTTON
				, "custom encrypt", L"custom encrypt", 0))
			{
				if (FR_RibbonElement pRibbonElement = FRRibbonButtonGetElement(pRibbonButton))
				{
					FRRibbonElementSetTooltip(pRibbonElement, L"custom encrypt");
					FRRibbonElementSetDescription(pRibbonElement, L"custom encrypt");
					FRRibbonButtonSetDefaultCommand(pRibbonButton, TRUE);
					FRRibbonElementSetClientData(pRibbonElement, NULL, NULL);
					FRRibbonElementSetExecuteProc(pRibbonElement, CRibbonUIManager::CustomerEncryptExecProc);
					FRRibbonElementSetComputeEnabledProc(pRibbonElement, CRibbonUIManager::CustomerEnabledProc);
				}
			}


			if (FR_RibbonButton pRibbonButton = (FR_RibbonButton)FRRibbonPanelAddElement(pRibbonPanel, FR_RIBBON_BUTTON
				, "customer decrypt", L"Load custom decrypt", 0))
			{
				if (FR_RibbonElement pRibbonElement = FRRibbonButtonGetElement(pRibbonButton))
				{
					FRRibbonElementSetTooltip(pRibbonElement, L"custom decrypt");
					FRRibbonElementSetDescription(pRibbonElement, L"custom decrypt");
					FRRibbonButtonSetDefaultCommand(pRibbonButton, TRUE);
					FRRibbonElementSetClientData(pRibbonElement, NULL, NULL);
					FRRibbonElementSetExecuteProc(pRibbonElement, CRibbonUIManager::CustomerDecryptExecProc);
					FRRibbonElementSetComputeEnabledProc(pRibbonElement, CRibbonUIManager::CustomerEnabledProc);
				}
			}

		}

	}
}
void CreateStandardCategory(FR_RibbonCategory pRibbonCategory)
{
	FR_RibbonPanel pRibbonPanel = FRRibbonCategoryGetPanelByName(pRibbonCategory, CATEGPRU_STANDARD);
	if (!pRibbonPanel)
	{
		pRibbonPanel = FRRibbonCategoryAddPanel(pRibbonCategory, CATEGPRU_STANDARD, CATEGPRU_STANDARDW, NULL);
		if (pRibbonPanel)
		{

			if (FR_RibbonButton pRibbonButton = (FR_RibbonButton)FRRibbonPanelAddElement(pRibbonPanel, FR_RIBBON_BUTTON
				, "standard encrypt password:(abcdefg)", L"standard encrypt password:(abcdefg)", 0))
			{

				if (FR_RibbonElement pRibbonElement = FRRibbonButtonGetElement(pRibbonButton))
				{
					FRRibbonElementSetTooltip(pRibbonElement, L"standard encrypt");
					FRRibbonElementSetDescription(pRibbonElement, L"standard encrypt");
					FRRibbonButtonSetDefaultCommand(pRibbonButton, TRUE);
					FRRibbonElementSetClientData(pRibbonElement, NULL, NULL);
					FRRibbonElementSetExecuteProc(pRibbonElement, CRibbonUIManager::StandardEncryptExecProc);
					FRRibbonElementSetComputeEnabledProc(pRibbonElement, CRibbonUIManager::CustomerEnabledProc);
				}
			}


			if (FR_RibbonButton pRibbonButton = (FR_RibbonButton)FRRibbonPanelAddElement(pRibbonPanel, FR_RIBBON_BUTTON
				, "standard password", L"Load standard password", 0))
			{
				if (FR_RibbonElement pRibbonElement = FRRibbonButtonGetElement(pRibbonButton))
				{
					FRRibbonElementSetTooltip(pRibbonElement, L"standard password");
					FRRibbonElementSetDescription(pRibbonElement, L"standard password");
					FRRibbonButtonSetDefaultCommand(pRibbonButton, TRUE);
					FRRibbonElementSetClientData(pRibbonElement, NULL, NULL);
					FRRibbonElementSetExecuteProc(pRibbonElement, CRibbonUIManager::StandardDecryptExecProc);
					FRRibbonElementSetComputeEnabledProc(pRibbonElement, CRibbonUIManager::CustomerEnabledProc);
				}
			}

		}

	}
}

void PILoadRibbonUI(void* pParentWnd)
{
	FR_RibbonBar pRibbonbar = FRAppGetRibbonBar(NULL);
	if (!pRibbonbar) return;

	FR_RibbonCategory pRibbonCategory = FRRibbonBarGetCategoryByName(pRibbonbar, "Ribbon_Category_Security");
	if (!pRibbonCategory)
	{
		FRRibbonBarAddCategory(pRibbonbar, CATEGORY_SECURITY, CATEGORY_CUSTOMERW);
		pRibbonCategory = FRRibbonBarGetCategoryByName(pRibbonbar, CATEGORY_SECURITY);
	}
	CreatePasswordCategory(pRibbonCategory);
	CreateStandardCategory(pRibbonCategory);
	
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
	theApp.fsDocInfoPtrArr = FSPtrArrayNew();
	theApp.InitGlobal();

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
	if(handshakeVersion != HANDSHAKE_V0100)
		return FALSE;
	
	/* Cast handshakeData to the appropriate type */
	PIHandshakeData_V0100* pData = (PIHandshakeData_V0100*)handshakeData;
	
	/* Set the name we want to go by */
	pData->PIHDRegisterPlugin(pData, "Security", (FS_LPCWSTR)L"Security");

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



void CSecurityApp::InitCallbacks()
{
	//Content Provider
	m_pFRContentProviderCallbacks = std::make_unique<FR_ContentProviderCallbacksRec>();
	m_pFRContentProviderCallbacks->lStructSize = sizeof(FR_ContentProviderCallbacksRec);
	m_pFRContentProviderCallbacks->FRConProviderOnFileOpen = FoxitContentProviderCallbacks::FRConProviderOnFileOpen;
	m_pFRContentProviderCallbacks->FRConProviderOnGetPermissions = FoxitContentProviderCallbacks::FRConProviderOnGetPermissions;
	m_pFRContentProviderCallbacks->FRConProviderOnGetContentSize = FoxitContentProviderCallbacks::FRConProviderOnGetContentSize;
	m_pFRContentProviderCallbacks->FRConProviderOnReadContent = FoxitContentProviderCallbacks::FRConProviderOnReadContent;
	m_pFRContentProviderCallbacks->FRConProviderOnWriteContent = FoxitContentProviderCallbacks::FRConProviderOnWriteContent;
	m_pFRContentProviderCallbacks->FRConProviderOnBackFillContent = FoxitContentProviderCallbacks::FRConProviderOnBackFillContent;
	m_pFRContentProviderCallbacks->FRConProviderIsWriteContentProgressive = FoxitContentProviderCallbacks::FRConProviderIsWriteContentProgressive;
	m_pFRContentProviderCallbacks->FRConProviderWriteContentProgressiveFinish = FoxitContentProviderCallbacks::FRConProviderWriteContentProgressiveFinish;
	m_pFRContentProviderCallbacks->FRConProviderOnFileClose = FoxitContentProviderCallbacks::FRConProviderOnFileClose;
	m_pFRContentProviderCallbacks->FRConProviderOnWriteAttachmentContent = FoxitContentProviderCallbacks::FRConProviderOnWriteAttachmentContent;
	m_pFRContentProviderCallbacks->FRConProviderOnGetAttachmentSize = FoxitContentProviderCallbacks::FRConProviderOnGetAttachmentSize;
	m_pFRContentProviderCallbacks->FRConProviderOnReadAttachmentContent = FoxitContentProviderCallbacks::FRConProviderOnReadAttachmentContent;
	m_pFRContentProviderCallbacks->FRConProviderIsPageAvail = FoxitContentProviderCallbacks::FRConProviderIsPageAvail;
	m_pFRContentProviderCallbacks->FRConProviderGetPageSize = FoxitContentProviderCallbacks::FRConProviderGetPageSize;
	m_pFRContentProviderCallbacks->FRConProviderDrawOwnerPage = FoxitContentProviderCallbacks::FRConProviderDrawOwnerPage;
	m_pFRContentProviderCallbacks->FRConProviderNeedReopenDoc = FoxitContentProviderCallbacks::FRConProviderNeedReopenDoc;
	m_pFRContentProviderCallbacks->FRConProviderCanBeSaved = FoxitContentProviderCallbacks::FRConProviderCanBeSaved;
	m_pFRContentProviderCallbacks->FRContentProviderIsProcessErrMsg = FoxitContentProviderCallbacks::FRContentProviderIsProcessErrMsg;

	FRAppRegisterContentProvider(m_pFRContentProviderCallbacks.get());


	m_pFRSecurityCallbacks = std::make_unique<FR_SecurityCallbacksRec>(); 
	m_pFRSecurityCallbacks->lStructSize = sizeof(FR_SecurityCallbacksRec);

	m_pFRSecurityCallbacks->clientData = this;
	m_pFRSecurityCallbacks->FRSecurityCreateCryptoHandler = FoxitSecurityCallbacks::FRSecurityCreateCryptoHandler;
	m_pFRSecurityCallbacks->FRSecurityOnInit = FoxitSecurityCallbacks::FRSecurityOnInit;
	m_pFRSecurityCallbacks->FRSecurityFinishHandler = FoxitSecurityCallbacks::FRSecurityFinishHandler;
	m_pFRSecurityCallbacks->FRSecurityGetCryptInfo = FoxitSecurityCallbacks::FRSecurityGetCryptInfo;
 	m_pFRSecurityCallbacks->FRSecurityGetPermissions = FoxitSecurityCallbacks::FRSecurityGetPermissions;
 	m_pFRSecurityCallbacks->FRSecurityIsMetadataEncrypted = FoxitSecurityCallbacks::FRSecurityIsMetadataEncrypted;
 	m_pFRSecurityCallbacks->FRSecurityIsOwner = FoxitSecurityCallbacks::FRSecurityIsOwner;

	FRAppRegisterSecurityHandler("filter", m_pFRSecurityCallbacks.get());


	m_pFRCryptoCallbacks = std::make_unique<FR_CryptoCallbacksRec>();
	m_pFRCryptoCallbacks->lStructSize = sizeof(FR_CryptoCallbacksRec);
	m_pFRCryptoCallbacks->FREnryptoCreateHandler = FoxitCryptoCallbacks::FREnryptoCreateHandler;
	m_pFRCryptoCallbacks->FREnryptoDecryptGetSize = FoxitCryptoCallbacks::FREnryptoDecryptGetSize;
	m_pFRCryptoCallbacks->FREnryptoDecryptStart = FoxitCryptoCallbacks::FREnryptoDecryptStart;
	m_pFRCryptoCallbacks->FREnryptoDecryptStream = FoxitCryptoCallbacks::FREnryptoDecryptStream;
	m_pFRCryptoCallbacks->FREnryptoDecryptFinish = FoxitCryptoCallbacks::FREnryptoDecryptFinish;
	m_pFRCryptoCallbacks->FREnryptoEncryptGetSize = FoxitCryptoCallbacks::FREnryptoEncryptGetSize;
	m_pFRCryptoCallbacks->FREnryptoEncryptContent = FoxitCryptoCallbacks::FREnryptoEncryptContent;
	m_pFRCryptoCallbacks->FREnryptoFinishHandler = FoxitCryptoCallbacks::FREnryptoFinishHandler;
	m_pFRCryptoCallbacks->FREnryptoProgressiveEncryptStart = FoxitCryptoCallbacks::FREnryptoProgressiveEncryptStart;
	m_pFRCryptoCallbacks->FREnryptoProgressiveEncryptContent = FoxitCryptoCallbacks::FREnryptoProgressiveEncryptContent;
	m_pFRCryptoCallbacks->FREnryptoProgressiveEncryptFinish = FoxitCryptoCallbacks::FREnryptoProgressiveEncryptFinish;


	SECURITY_CLIENTDATA* pData = new SECURITY_CLIENTDATA;
	pData->wsMethodTitle = FSWideStringNew();
	FSWideStringFill(pData->wsMethodTitle, L"Foxt Demo Encrypt");
	m_pFRSecurityMethodCallbacks = std::make_unique <FR_SecurityMethodCallbacksRec>();
	m_pFRSecurityMethodCallbacks->clientData = pData;
	m_pFRSecurityMethodCallbacks->lStructSize = sizeof(FR_SecurityMethodCallbacksRec);
	m_pFRSecurityMethodCallbacks->FRSecurityMethodIsMyMethod = FoxitSecurityMethodCallbacks::FRSecurityMethodIsMyMethod;
	m_pFRSecurityMethodCallbacks->FRSecurityMethodGetName = FoxitSecurityMethodCallbacks::FRSecurityMethodGetName;
	m_pFRSecurityMethodCallbacks->FRSecurityMethodGetTitle = FoxitSecurityMethodCallbacks::FRSecurityMethodGetTitle;
	FRAppRegisterSecurityMethod(m_pFRSecurityMethodCallbacks.get());


}

void CSecurityApp::ReleaseCallbacks()
{

}

void CSecurityApp::InitGlobal()
{
	InitCallbacks();

}

HRESULT CSecurityApp::GetSecurityDocInfo(FR_Document frDoc, CFSecurityDocInfo** ppSecurityDocInfo)
{
	if (NULL == frDoc || NULL == ppSecurityDocInfo)
		return S_FALSE;

	return S_FALSE;
}


HRESULT CSecurityApp::GetDocInfo(FR_Document frDoc, CFDocInfo** ppDocInfo)
{
	if (NULL == frDoc || NULL == ppDocInfo)
		return S_FALSE;

	HRESULT hr = S_FALSE;

	int nCount = FSPtrArrayGetSize(fsDocInfoPtrArr);
	while (nCount)
	{
		CFDocInfo* pFDocInfo = NULL;
		pFDocInfo = (CFDocInfo*)FSPtrArrayGetAt(fsDocInfoPtrArr, nCount - 1);
		if (frDoc == pFDocInfo->frDoc)
		{
			*ppDocInfo = pFDocInfo;
			hr = S_OK;
			break;
		}
		nCount--;
	}

	return hr;
}

HRESULT CSecurityApp::AddDocInfo(CFDocInfo* pDocInfo)
{
	if (NULL == pDocInfo)
		return S_FALSE;
	FSPtrArrayAdd(fsDocInfoPtrArr, pDocInfo);
	return S_OK;
}