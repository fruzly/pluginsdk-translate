// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__63366858_25E6_418A_803A_E1919404F5AB__INCLUDED_)
#define AFX_STDAFX_H__63366858_25E6_418A_803A_E1919404F5AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Foxit Reader SDK Headers.
#include "../../include/basic/fs_callsInclude.h"
#include "../../include/basic/fs_pidata.h"

#include "../../include/basic/fs_callsInclude.h"
#include "../../include/fdrm/fdrm_pdfCalls.h"
#include "../../include/fdrm/fdrm_pkiCalls.h"
#include "../../include/basic/fs_pidata.h"
#include "Security.h"
extern CSecurityApp theApp;


// const PSTR   g_szWRAPPER = "Wrapper",
// g_szWRAPPER_TYPE = "Type",
// g_szWRAPPER_TYPEVALUE = "FoxitRMS",
// g_szWRAPPER_TYPEVALUEV2 = "FoxitRMSV2",
// g_szWRAPPER_MICROSOFTTYPEVALUE = "MicrosoftIRMServices",
// g_szWRAPPER_APPLICATION = "Application",
// #ifdef FXNET_FPM_PER
// g_szWRAPPER_APPLICATIONVALUE = "Foxit Phantom",
// #else
// g_szWRAPPER_APPLICATIONVALUE = "Foxit PhantomPDF",
// #endif
// g_szWRAPPER_URI = "https://www.foxitsoftware.com/products/rms/",
// g_szWRAPPER_DESCRIPTION = "This PDF document is protected by Foxit. To view the document, you will need a compatible PDF document reader. You can download the latest version of Foxit Reader and PhantomPDF at https://www.foxitsoftware.com/products/rms/",
// g_szWRAPPER_MICROSOFT_DES = "This embedded file is encrypted using MicrosoftIRMServices filter",
// g_szWRAPPER_MICROSOFT_FILE_NAME = "MicrosoftIRMServices Protected PDF.pdf",
// g_szWRAPPER_APPLICATIONVALUE_READER = "Foxit Enerprise Reader",
// g_szWRAPPER_DESCRIPTION_READER = "This is a wrapper PDF file, created by Foxit Enterprise Reader",
// g_szWRAPPER_WRAPPEROFFSET = "WrapperOffset",
// g_szWRAPPER_VERSION = "Version";

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__63366858_25E6_418A_803A_E1919404F5AB__INCLUDED_)





















