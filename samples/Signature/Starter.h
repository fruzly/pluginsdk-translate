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

#include "resource.h"		// main symbols
#include <memory>

/////////////////////////////////////////////////////////////////////////////
// CStarterApp
// See Starter.cpp for the implementation of this class
//

#define SIG_ENCODING_TYPE			 X509_ASN_ENCODING | PKCS_7_ASN_ENCODING
enum LOADCERTSTORE
{
	LOADSTORERROR = 0,
	USAGEEROR,
	SUCCESS
};

class CGW_CertCommon
{
public:
	static PCCERT_CONTEXT	CertClone(PCCERT_CONTEXT pCertContext, FS_ByteString* pChainParentSn = nullptr);
	static void	GetCertSN(PCCERT_CONTEXT pCertContext, FS_ByteString* bsSN);
};

class CStarterApp : public CWinApp
{
public:
	CStarterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStarterApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CStarterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	

public:
	void CreateRibbonUI();
	static void SignatureProc(void* pData);
	static FS_BOOL MarkProc(void* pData);
	static void OnFreeDataProc(void *clientData);
	static FS_BOOL OnButtonEnableProc(void* clientDate);
	static void OnePageSignatureProc(void* pData);
	static void TwoPageSignatureProc(void* pData);
	static void SealSignatureProc(void* pData);

	static void MultipyPageSignatureProc(void* pData);
	void MultipySignature(FS_Int32Array pageArr, FS_LPCSTR subfilter = "adbe.pkcs7.detached");
	
	FPD_Object CreateAppearanceDict(FPD_Document pdfDoc, FS_DIBitmap bitmap, int nRotate, FS_DWORD *pObjNum);	
	FPD_Object CreateFieldDict(FR_Document frDoc, FS_INT32 pageIndex, FS_DWORD dwVObjNum);

	FPD_Object CreateSealFieldDict(FR_Document frDoc, FS_INT32 pageIndex, FS_DWORD dwVObjNum, FS_INT32 pagecount, FS_DIBitmap pBitmap);
	FPD_Object CreateSealAppearanceDict(FPD_Document pdfDoc, FS_DIBitmap bitmap, int nRotate, FS_DWORD *pObjNum, int pageIndex, FS_INT32 pagecount);

public:
	void * m_pCustomSignHandler = nullptr;
	std::unique_ptr<FR_SignatureHandlerCallbacksRec> m_pFRCustomSignHandlerCallbacks;
	void InitFoxitSignSignatureHandlerCallbacks();
	void ReleaseFoxitSignSignatureHandler();
	FS_INT32 index;
	FS_LPCSTR subfilter;
	FS_FLOAT first_page_percent;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTER_H__AF29C9AF_EA18_4B72_A11C_667186A0D1E2__INCLUDED_)
