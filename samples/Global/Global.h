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
	// main symbols

#include<iostream>

/////////////////////////////////////////////////////////////////////////////
// CStarterApp
// See Starter.cpp for the implementation of this class
//

class CStarterApp : public CWinApp
{
public:
	CStarterApp();

	DECLARE_MESSAGE_MAP()

public:
	std::wstring LoadStringFromID(int nID);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTER_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
