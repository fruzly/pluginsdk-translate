
// InsertText.h : main header file for the InsertText application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

// CInsertTextApp:
// See InsertText.cpp for the implementation of this class
//
#include <string>  

class CInsertTextApp : public CWinApp
{
public:
	CInsertTextApp();

	void CreateElementsToNewCategory();
	CString GetSaveAsFilePath();
	std::string ConvertToLPCSTRPath(LPCWSTR wsPath);
// Overrides
public:
// Implementation

public:	
	DECLARE_MESSAGE_MAP()
};

extern CInsertTextApp theApp;
