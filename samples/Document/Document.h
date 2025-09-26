// Document.h : main header file for the DOCUMENT DLL
//

#if !defined(AFX_DOCUMENT_H__D4143811_8B7A_48CD_A3EF_C6224AD67A1F__INCLUDED_)
#define AFX_DOCUMENT_H__D4143811_8B7A_48CD_A3EF_C6224AD67A1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CMemDocument;
/////////////////////////////////////////////////////////////////////////////
// CDocumentApp
// See Document.cpp for the implementation of this class
//

#define OPEN_FILE_BUFFER 6000
class CDocumentApp : public CWinApp
{
public:
	CDocumentApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocumentApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDocumentApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	WCHAR * m_wszBuffer;
	int DoModalOpenFileDialog(CFileDialog& dlg);

	CPtrArray m_arrDocument;	
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCUMENT_H__D4143811_8B7A_48CD_A3EF_C6224AD67A1F__INCLUDED_)
