/*********************************************************************

 Copyright (C) 2024 Foxit Software Company
 All rights reserved.

 NOTICE: Foxit permits you to use, modify, and distribute this file
 in accordance with the terms of the Foxit license agreement
 accompanying it. If you have received this file from a source other
 than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.

 ---------------------------------------------------------------------

Extension HFT.cpp

 - Defines the entry point for the DLL application, the entry point is PlugInMain.

*********************************************************************/

// Extension HFT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Extension HFT.h"
#include "methodsImpl.h"
#include "methodsCalls.h"

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
// CExtensionHFTApp

BEGIN_MESSAGE_MAP(CExtensionHFTApp, CWinApp)
	//{{AFX_MSG_MAP(CExtensionHFTApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtensionHFTApp construction

CExtensionHFTApp::CExtensionHFTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExtensionHFTApp object

CExtensionHFTApp theApp;

/** 
	This sample gives an explanation about how to work with extension HFTs.
	If you create a group of methods in a plug-in and you want to make the 
	other plug-ins can invoke these methods, you can create a extension HFT 
	to manage these methods by invoking FSExtensionHFTMgrNewHFT. Then you 
	can add this HFT to the host environment by invoking FSExtensionHFTMgrAddHFT.

    You can create a template file to describe groups of methods. See the 
	methodsTmpl.h header file. It is used to generate the method selector to
	manage the index of the methods. It is also used to generate the prototypes
	of the methods.

	According to the methodsTmpl.h header file, the methodsCalls.h header file describes 
	how to generate the method selector and how to generate the prototypes of the methods. 
	It also defines these methods as macros so that the other plug-ins can invoke them 
	more conveniently. The other plug-ins only need to include the methodsCalls.h header 
	file , invoke the FSExtensionHFTMgrGetHFT to get the HFT, and invoke the macros defined.
*/



/*The extension HFT.*/
FS_HFT extensionHFT = NULL;

/*Create extension HFT. This function must be invoked in the PIExportHFTs routine.*/
void CreateExtensionHFT()
{
	/*Create a new extension HFT.*/
	extensionHFT = FSExtensionHFTMgrNewHFT(ExtensionMethodsNum);

    /*Add the HFT to the host environment.*/
	FSExtensionHFTMgrAddHFT(EXTENSION_HFT_NAME, EXTENSION_HFT_VERSION, extensionHFT);

	/*Add the method's address to the extension HFT.*/
	FSExtensionHFTMgrReplaceEntry(extensionHFT, Function1SEL, (void*)&(CMethodsDefine::Function1));
	FSExtensionHFTMgrReplaceEntry(extensionHFT, Function2SEL, (void*)&(CMethodsDefine::Function2));
}

/********************************************************************/
/* The basic procedure of plug-in.                                   
/********************************************************************/


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
	/*Create extension HFT.*/
	CreateExtensionHFT();

	/* True to continue loading plug-in, false to cause plug-in loading to stop.*/
	return TRUE;
}

/** 
The application calls this function to allow it to
<ul>
<li> Import plug-in supplied HFTs.
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
	if(handshakeVersion != HANDSHAKE_V0100)
		return FALSE;
	
	/* Cast handshakeData to the appropriate type */
	PIHandshakeData_V0100* pData = (PIHandshakeData_V0100*)handshakeData;
	
	/* Set the name we want to go by */
	pData->PIHDRegisterPlugin(pData, "FoxitSamplePluginExtensionHFT", (FS_LPCWSTR)L"Extension HFT");

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

	
