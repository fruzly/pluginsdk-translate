// Security.h : main header file for the SECURITY DLL
//

#if !defined(AFX_SECURITY_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
#define AFX_SECURITY_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_

#include <string>
#include <locale>
#include <codecvt>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSecurityApp
// See Security.cpp for the implementation of this class
//
#include <memory>
#include <map>
class CFDocInfo;
class CFSecurityDocInfo;
class CSecurityApp : public CWinApp
{
public:
	CSecurityApp();

	void InitCallbacks();
	void ReleaseCallbacks();
	void InitGlobal();

	HRESULT GetDocInfo(FR_Document frDoc, CFDocInfo** ppDocInfo);
	HRESULT AddDocInfo(CFDocInfo* pDocInfo);
	HRESULT GetSecurityDocInfo(FR_Document frDoc, CFSecurityDocInfo** ppSecurityDocInfo);

	FS_PtrArray fsDocInfoPtrArr;
	std::unique_ptr<FR_CryptoCallbacksRec> m_pFRCryptoCallbacks;


	// convert string to wstring
	inline std::wstring to_wide_string(const std::string& input)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(input);
	}
	// convert wstring to string 
	inline std::string to_byte_string(const std::wstring& input)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(input);
	}
	bool m_bsecurity = false;
	// try default password count 
	int m_TryPasswordCount = 0;
	std::string m_defpassword = "abcdefg";

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecurityApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSecurityApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	std::unique_ptr<FR_ContentProviderCallbacksRec> m_pFRContentProviderCallbacks;
	std::unique_ptr<FR_SecurityCallbacksRec> m_pFRSecurityCallbacks;
	std::unique_ptr<FR_SecurityMethodCallbacksRec> m_pFRSecurityMethodCallbacks;



};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURITY_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
