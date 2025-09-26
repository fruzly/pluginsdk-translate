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

#define CATEGORY_CATEGORY_CUSTOMER "Ribbon_Category_Customertool"

#define CATEGORY_CUSTOMER "Customer_Sample"
#define CATEGORY_CUSTOMERW L"Customer_Sample"


#define PE_CUSTOM_TOOL_NAME "Custom_Tool_Name"

#define FSARGB_A(argb) ((FS_BYTE)((argb) >> 24))
/** @brief Extract the red component. */
#define FSARGB_R(argb) ((FS_BYTE)((argb) >> 16))
/** @brief Extract the green component. */
#define FSARGB_G(argb) ((FS_BYTE)((argb) >> 8))
/** @brief Extract the blue component. */
#define FSARGB_B(argb) ((FS_BYTE)(argb))

#define FSSYS_RGB(r, g, b)  ((r) | ((g) << 8) | ((b) << 16))
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__63366858_25E6_418A_803A_E1919404F5AB__INCLUDED_)





















