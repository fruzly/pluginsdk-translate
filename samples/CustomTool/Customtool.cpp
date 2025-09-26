/*********************************************************************

 Copyright (C) 2024 Foxit Corporation
 All rights reserved.

 NOTICE: Foxit permits you to use, modify, and distribute this file
 in accordance with the terms of the Foxit license agreement
 accompanying it. If you have received this file from a source other
 than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.

 ---------------------------------------------------------------------

CustomTool.cpp

 - Defines the entry point for the DLL application, the entry point is PlugInMain.

 - Skeleton .cpp file for a plug-in. It implements the basic
   handshaking methods required for all plug-ins.

*********************************************************************/


#include "stdafx.h"
#include "Customtool.h"
#include "Tool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
FS_ARGB ArgbEncode(int a, int r, int g, int b) { return (a << 24) | (r << 16) | (g << 8) | b; }


extern CCustomToolApp theApp;

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
// CCustomToolApp

BEGIN_MESSAGE_MAP(CCustomToolApp, CWinApp)
	//{{AFX_MSG_MAP(CCustomToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomToolApp construction

CCustomToolApp::CCustomToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CCustomToolApp::Init()
{

	m_map[DEFAULT] = ArgbEncode(100, 0, 255, 0);
	m_map[CTRL] = ArgbEncode(100, 0, 0, 255);
	m_map[SHIFT] = ArgbEncode(100, 255, 0, 0);


	FR_ToolCallbacksRec tc;
	memset(&tc, 0x00, sizeof(tc));
	tc.lStructSize = sizeof(FR_ToolCallbacksRec);
	tc.clientData = this;

	tc.FRToolOnInit = &CustomToolOnInit;
	tc.FRToolDestroy = &CustomToolDestroy;
	tc.FRToolOnActivate = &CustomToolOnActivate;
	tc.FRToolOnDeactivate = &CustomToolOnDeactivate;
	tc.FRToolOnKeyDown = &CustomToolOnKeyDown;
	tc.FRToolOnKeyUp = &CustomToolOnKeyUp;
	tc.FRToolOnChar = &CustomToolOnChar;
	tc.FRToolOnLeavePage = &CustomToolOnLeavePage;
	tc.FRToolIsEnabled = &CustomToolIsEnabled;
	tc.FRToolOnLButtonDown = &CustomToolOnLButtonDown;
	tc.FRToolOnLButtonUp = &CustomToolOnLButtonUp;
	tc.FRToolOnLButtonDblClk = &CustomToolOnLButtonDblClk;
	tc.FRToolOnMouseMove = &CustomToolOnMouseMove;
	tc.FRToolOnRButtonDown = &CustomToolOnRButtonDown;
	tc.FRToolOnRButtonUp = &CustomToolOnRButtonUp;
	tc.FRToolOnRButtonDblClk = &CustomToolOnRButtonDblClk;
	tc.FRToolOnMouseWheel = &CustomToolOnMouseWheel;
	tc.FRToolOnDraw = &CustomToolOnDraw;
	tc.FRToolIsProcessing = &CustomToolIsProcessing;
	tc.FRToolOnMouseHover = &CustomToolOnMouseHover;
	tc.FRToolIsWndCapturing = &CustomToolIsWndCapturing;

	FR_Tool pTool = FRToolNew(PE_CUSTOM_TOOL_NAME, &tc);
	if (pTool)
	{
		FRAppRegisterTool(pTool);
		theApp.SetTool(pTool);
	}


}

void CCustomToolApp::SetTool(FR_Tool tool)
{
	this->m_tool = tool;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCustomToolApp object

CCustomToolApp theApp;

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
void CustomerDrawLineExecProc(void * pData)
{
	FRAppSetActiveTool(FRAppGetToolByName(PE_CUSTOM_TOOL_NAME), TRUE);
}




void PILoadRibbonUI(void* pParentWnd)
{
	FR_RibbonBar pRibbonbar = FRAppGetRibbonBar(NULL);
	if (!pRibbonbar) return;

	FR_RibbonCategory pRibbonCategory = FRRibbonBarGetCategoryByName(pRibbonbar, CATEGORY_CATEGORY_CUSTOMER);
	if (!pRibbonCategory)
	{
		FRRibbonBarAddCategory(pRibbonbar, CATEGORY_CUSTOMER, CATEGORY_CUSTOMERW);
		pRibbonCategory = FRRibbonBarGetCategoryByName(pRibbonbar, CATEGORY_CUSTOMER);
	}

	FR_RibbonPanel pRibbonPanel = FRRibbonCategoryGetPanelByName(pRibbonCategory, CATEGORY_CUSTOMER);
	if (!pRibbonPanel)
	{
		pRibbonPanel = FRRibbonCategoryAddPanel(pRibbonCategory, CATEGORY_CUSTOMER, CATEGORY_CUSTOMERW, NULL);
		if (pRibbonPanel)
		{


			if (FR_RibbonButton pRibbonButton = (FR_RibbonButton)FRRibbonPanelAddElement(pRibbonPanel, FR_RIBBON_BUTTON
				, "draw line", L"draw line", 0))
			{
				if (FR_RibbonElement pRibbonElement = FRRibbonButtonGetElement(pRibbonButton))
				{
					FRRibbonElementSetTooltip(pRibbonElement, L"draw line");
					FRRibbonElementSetDescription(pRibbonElement, L"draw line");
					FRRibbonButtonSetDefaultCommand(pRibbonButton, TRUE);
					FRRibbonElementSetClientData(pRibbonElement, NULL, NULL);
 					FRRibbonElementSetExecuteProc(pRibbonElement, CustomerDrawLineExecProc);
				}
			}
		}

	}
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
	theApp.Init();
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
	pData->PIHDRegisterPlugin(pData, "Customtool", (FS_LPCWSTR)L"Customtool");

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

