// CustomTool.h : main header file for the STARTER DLL
//

#if !defined(AFX_CUSTOMTOOL_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
#define AFX_CUSTOMTOOL_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCustomToolApp
// See Customtool.cpp for the implementation of this class
//
#include <map>

class CCustomToolApp : public CWinApp
{
public:
	CCustomToolApp();

	void Init();

	void SetTool(FR_Tool tool);

	//void drawLineStyle()

public:
	FR_Tool m_tool;
	FR_PageView m_page;

	FS_DevicePoint m_begin;
	FS_DevicePoint m_end;
	FPD_Path m_pPathFill;
	bool m_bDraw = false;
	enum KEYINFO { DEFAULT, CTRL, SHIFT };

	std::map<KEYINFO, FS_ARGB> m_map;
	KEYINFO m_keyinfo = DEFAULT;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomToolApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCustomToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTER_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
