/*********************************************************************

 Copyright (C) 2024 Foxit Corporation
 All rights reserved.

 NOTICE: Foxit permits you to use, modify, and distribute this file
 in accordance with the terms of the Foxit license agreement
 accompanying it. If you have received this file from a source other
 than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.

 ---------------------------------------------------------------------

Signature.cpp

 - Defines the entry point for the DLL application, the entry point is PlugInMain.

 - Skeleton .cpp file for a plug-in. It implements the basic
   handshaking methods required for all plug-ins.

*********************************************************************/


#include "stdafx.h"
#include "EventHandler.h"
#include <iostream>
#include <string>
#include<vector>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CStarterApp

BEGIN_MESSAGE_MAP(CStarterApp, CWinApp)
	//{{AFX_MSG_MAP(CStarterApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStarterApp construction

CStarterApp::CStarterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	index = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CStarterApp object
CStarterApp theApp;

void CStarterApp::CreateRibbonUI()
{
	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); //The RibbonBar will free by Foixt Reader or Phantom.
	FR_RibbonCategory fr_Category = FRRibbonBarAddCategory(fr_Bar, "SignDemo", (FS_LPCWSTR)L"SignDemo");
	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, "RibbonPanel_1", (FS_LPCWSTR)L"RibbonPanel_1", NULL);

	FR_RibbonButton fr_Button1 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"Sign", (FS_LPCWSTR)L"Sign", -1);
	FR_RibbonElement fr_ElementButton1 = FRRibbonPanelGetElementByName(fr_Panel, "Sign");
	FRRibbonElementSetExecuteProc(fr_ElementButton1, theApp.SignatureProc);
	FRRibbonElementSetComputeMarkedProc(fr_ElementButton1, theApp.MarkProc);
	FRRibbonElementSetClientData(fr_ElementButton1, (void*)"Sign", theApp.OnFreeDataProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementButton1, theApp.OnButtonEnableProc);

	FR_RibbonButton fr_Button2 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"MultipyPageSign", (FS_LPCWSTR)L"MultipyPageSign", -1);
	FR_RibbonElement fr_ElementButton2 = FRRibbonPanelGetElementByName(fr_Panel, "MultipyPageSign");
	FRRibbonElementSetExecuteProc(fr_ElementButton2, theApp.MultipyPageSignatureProc);
	FRRibbonElementSetComputeMarkedProc(fr_ElementButton2, theApp.MarkProc);
	FRRibbonElementSetClientData(fr_ElementButton2, (void*)"MultipyPageSign", theApp.OnFreeDataProc);
	FRRibbonElementSetComputeEnabledProc(fr_ElementButton2, theApp.OnButtonEnableProc);

	/*FR_RibbonButton fr_Button3 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"OnePageSign", (FS_LPCWSTR)L"OnePageSign", -1);
	FR_RibbonElement fr_ElementButton3 = FRRibbonPanelGetElementByName(fr_Panel, "OnePageSign");
	FRRibbonElementSetExecuteProc(fr_ElementButton3, theApp.OnePageSignatureProc);
	FRRibbonElementSetComputeMarkedProc(fr_ElementButton3, theApp.MarkProc);

	FR_RibbonButton fr_Button4 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"TwoPageSign", (FS_LPCWSTR)L"TwoPageSign", -1);
	FR_RibbonElement fr_ElementButton4 = FRRibbonPanelGetElementByName(fr_Panel, "TwoPageSign");
	FRRibbonElementSetExecuteProc(fr_ElementButton4, theApp.TwoPageSignatureProc);
	FRRibbonElementSetComputeMarkedProc(fr_ElementButton4, theApp.MarkProc);*/

	FR_RibbonButton fr_Button5 = (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"SealSign", (FS_LPCWSTR)L"SealSign", -1);
	FR_RibbonElement fr_ElementButton5 = FRRibbonPanelGetElementByName(fr_Panel, "SealSign");
	FRRibbonElementSetExecuteProc(fr_ElementButton5, theApp.SealSignatureProc);
	FRRibbonElementSetComputeMarkedProc(fr_ElementButton5, theApp.MarkProc);
}

FS_BOOL CStarterApp::MarkProc(void* pData)
{
	return TRUE;
}

FS_BOOL CStarterApp::OnButtonEnableProc(void* clientDate)
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	if (frDocument == NULL)
	{
		return FALSE;
	}

	const FS_CHAR* name = (const FS_CHAR*)clientDate;

	if (_stricmp(name, "MultipyPageSign") == 0)
	{
		FR_Document frDoc = FRAppGetActiveDocOfPDDoc();
		FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
		FS_INT32 count = FPDDocGetPageCount(fpdDoc);
		if (count <= 1)
			return FALSE;
	}
	return TRUE;
}

void CStarterApp::OnFreeDataProc(void *clientData)
{

}

//设置好相关属性，直接调用签名接口
void CStarterApp::SignatureProc(void* pData)
{
	FR_Document frDoc = FRAppGetActiveDocOfPDDoc();
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);

	//在执行apply 签名的时候需要做以下流程：

	//第一步：可以先通过 FRCustomSignatureFoixtCreateSignatureF 创建签名表单相关的字典项，不调用也可以，也可以自己实现添加这个字典项的创建
	//FPD_Object pSignFDict = FRCustomSignatureFoixtCreateSignatureF(frDoc, true);
		//创建完表单字典项后，你可以自己修改或者往字典项中添加属性

	//第二步：通过 FRCustomSignatureGenerateSignInfo 创建签名
	   //先创建一个签名对象 到pdf文档中，这里只需要设置简单的签名信息即可
	   //在创建签名对象后，签名模块会通过FRCustomSignatureRegisterSignatureHandler注册进来的handler，来询问是否需要自己来签名摘要
	   // 如果FRSignatureHandlerSGVerifyCanSign返回FR_SIG_HANDLE_SUCCESS即表示需要自己实现签名摘要

	//设置证书的属性
	FR_SignatureInfo pSInfo;
	//这个属性目前没有用到
	pSInfo.bDocTimeStamp = FALSE;
	//签名图标类型
	//pSInfo.Icontype = FR_SG_ICONTYPE::FR_SGIT_GRAPHICS;
	pSInfo.Icontype = FR_SG_ICONTYPE::FR_SGIT_GRAPHICS;
	pSInfo.lShowFlag = 0;
	FS_WCHAR imagePath[260] = L"F:\\select.png";
	//图标路径
	memcpy(pSInfo.ImagePath, imagePath, 260 * sizeof(wchar_t));

	FR_SignatureExtendInfo extendInfo;
	extendInfo.lStructSize = sizeof(FR_SignatureExtendInfo);
	/*extendInfo.imageType = FS_IMAGE_TYPE::FS_IMAGE_PNG;
	std::ifstream is("F:\\select.png", std::ifstream::in | std::ios::binary);
	is.seekg(0, is.end);
	int length = is.tellg();
	is.seekg(0, is.beg);
	char* buffer = new char[length];
	is.read(buffer, length);
	extendInfo.imageData = (unsigned char*)buffer;
	extendInfo.imageLen = length;*/
	memcpy(extendInfo.wsSubFilter, L"testSubFilter", 128 * sizeof(wchar_t));

	//是否使用标准样式，类似protected-Sign&certify-place signatrue下的appearance type，
   //如果直接想通过这个接口添加证书显示外观，需要将该标识设置为true才能添加成功
	pSInfo.bUseStandAppearance = false;
	//设置签名后文档的权限
	pSInfo.PermissionType = FR_SG_PERMISSION::FR_APG_NONE;
	//签名信息显示的方式
	pSInfo.TextDir = FR_SG_TEXTDIR::FR_SGTD_AUTO;
	FS_WCHAR wsCN[64] = L"jingjing";
	//签名author
	memcpy(pSInfo.wsCN, wsCN, 64 * sizeof(wchar_t));
	FS_WCHAR wsC[64] = L"jing";
	//国家或地区信息
	memcpy(pSInfo.wsC, wsC, 64 * sizeof(wchar_t));
	FS_WCHAR wsOU[64] = L"jing";
	//组织单元信息
	memcpy(pSInfo.wsOU, wsOU, 64 * sizeof(wchar_t));
	FS_WCHAR wsE[64] = L"jingjing_tang@foxitsoftwrae.com";
	//邮箱信息
	memcpy(pSInfo.wsE, wsE, 128 * sizeof(wchar_t));
	FS_WCHAR wsO[64] = L"jing";
	//组织名称
	memcpy(pSInfo.wsO, wsO, 64 * sizeof(wchar_t));

	//签名密码信息
	memcpy(pSInfo.wsPassword, L"123456", 64 * sizeof(wchar_t));
	//外观样式名称
	memcpy(pSInfo.wsAppearance, L"", 64 * sizeof(wchar_t));

	//时间戳服务器URL
	memcpy(pSInfo.wsTimestampURL, L"", 256 * sizeof(wchar_t));

	//memcpy(pSInfo.wsCertFile, L"", MAX_PATH * sizeof(wchar_t));
	//签名自身的一些信息，类似创建签名那个界面需要填的信息
	FR_SignatureDictInfo dicInfo;
	memcpy(dicInfo.wsLocation, L"", 128 * sizeof(wchar_t));
	memcpy(dicInfo.wsName, L"jingjing", 128 * sizeof(wchar_t));
	memcpy(dicInfo.wsReason, L"I am the author of this document", 128 * sizeof(wchar_t));
	memcpy(dicInfo.wsFilter, L"", 128 * sizeof(wchar_t));
	memcpy(dicInfo.wsDate, L"2020/07/13", 128 * sizeof(wchar_t));
	pSInfo.SignDictInfo = dicInfo;

	// 三种方式传证书： 1、通过 wsCertFile 参数直接传证书路径；
	// 2、通过 FPDCertStoreGetCertsArray 获取已安装到系统的证书；
	// 3、通过 FPDCertStoreAddCert2 将证书添加到certsotre
	// 推荐用第一种方式，直接传证书路径，对于外部用户比较方便
	//方式一：
	//签名证书路径
	memcpy(pSInfo.wsCertFile, L"F:\\PhantomCode\\12.0Branch\\starship\\PluginSDK\\Samples\\SignatureDemo\\jingjing.pfx", MAX_PATH * sizeof(wchar_t));

	//FPD_CertStore certStore = FPDCertStoreNew();

	// 方式二：
	//如果已经提前将证书安装到系统，可以从系统中加载这个证书对象
	//FS_PtrArray arr = FSPtrArrayNew();
	//FPDCertStoreLoadMYSystemCerts(certStore, TRUE);
	//FPDCertStoreGetCertsArray(certStore, &arr);
	//FS_INT32 size = FSPtrArrayGetSize(arr);
	//for(int i=0; i< size; i++)
	//{
	//	PCCERT_CONTEXT cert = (PCCERT_CONTEXT)FSPtrArrayGetAt(arr, i);
	//	FS_ByteString cID = FSByteStringNew();
	//	FPDCertStoreGetCertSNID((FS_LPVOID)cert, &cID);
	//	FS_WideString wsID = FSWideStringNew();
	//	FSByteStringUTF8Decode(cID, &wsID);
	//	CString csCertID = (FS_LPCWSTR)FSWideStringCastToLPCWSTR(wsID);

	//	FS_WideString szCertName = FSWideStringNew();
	//	FPDCertMethodGetCertName((void*)cert, FALSE, &szCertName);
	//	CString csName = (FS_LPCWSTR)FSWideStringCastToLPCWSTR(szCertName);

	//	//设置签名对象PCCERT_CONTEXT，我上面是从系统签名列表中取了一个证书，也可以通过系统 API自己解析一个证书文件，对象为PCCERT_CONTEXT类型即可
	//	if (csName == L"jingjing")
	//	{
	//		pSInfo.pCertContext = (FS_LPVOID)cert;
	//		break;
	//	}
	//}

	// 方式三：
	//如果没有提前安装到系统，也可以通过FPDCertStoreAddCert2将证书添加到certstore，得到证书对象
	/*if (pSInfo.pCertContext == NULL)
	{
		pSInfo.pCertContext =
			FPDCertStoreAddCert2(certStore, L"F:\\PhantomCode\\TempBranch\\starship\\PluginSDK\\Samples\\SignatureDemo\\jingjing.pfx", L"123456");
	}*/


	//设置证书的位置及生成后保存的路径
	FR_SignaturePosInfo posInfo;
	posInfo.nPageIndex = 0;
	FS_WCHAR		FileSavePath[260] = L"E:\\PDFFile\\testfile\\signed.pdf";
	memcpy(posInfo.FileSavePath, FileSavePath, 260 * sizeof(wchar_t));
	posInfo.pPDFDoc = fpdDoc;

	FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
	FR_PageView frPageView = FRDocViewGetCurrentPageView(frDocView);
	FS_Rect rectPage = FRPageViewGetPageRect(frPageView);

	//传入设备坐标
	posInfo.nLeft = rectPage.left+ ((rectPage.right- rectPage.left)/2 - 100);
	posInfo.nRight = posInfo.nLeft + 64;
	posInfo.nTop = (rectPage.bottom - rectPage.top) /2;
	posInfo.nBottom = posInfo.nTop + 64;

	//FPD_Object pDict = theApp.CreateWidgetDict(frDoc);
	//posInfo.pFieldDict = pDict;

	//FS_BOOL bCreate = FRCustomSignatureGenerateSignInfo(&pSInfo, &posInfo);
	FS_BOOL bCreate = FRCustomSignatureGenerateSignInfo2(&pSInfo, &posInfo, true, NULL);
	//FPDCertStoreDestroy(certStore);
}

//自己构建field字典项,多页签章对应同一个V字典项
void CStarterApp::MultipyPageSignatureProc(void* pData)
{
	theApp.index = 0;
	FS_Int32Array pageArray = FSInt32ArrayNew();
	FSInt32ArrayAdd(pageArray, 0);
	FSInt32ArrayAdd(pageArray, 1);
	//theApp.subfilter = "subFilterSuccess";
	theApp.MultipySignature(pageArray);
	return;
}

//单页签章
void CStarterApp::OnePageSignatureProc(void* pData)
{
	theApp.index = 0;
	FS_Int32Array pageArray = FSInt32ArrayNew();
	FSInt32ArrayAdd(pageArray, 0);
	theApp.subfilter = "subFilterSignFail";
	theApp.MultipySignature(pageArray, "subFilterSignFail");
}

//多页签章
void CStarterApp::TwoPageSignatureProc(void* pData)
{
	theApp.index = 0;
	FS_Int32Array pageArray = FSInt32ArrayNew();
	FSInt32ArrayAdd(pageArray, 0);
	FSInt32ArrayAdd(pageArray, 1);
	//theApp.subfilter = "subFilterVerifyFail";
	theApp.MultipySignature(pageArray, "subFilterVerifyFail");
}

//自己设置好widget字典项，再调用签名接口
void CStarterApp::MultipySignature(FS_Int32Array pageArr, FS_LPCSTR subfilter)
{
	FR_Document frDoc = FRAppGetActiveDocOfPDDoc();
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FS_INT32 nCountDocViews = FRDocCountDocViews(frDoc);
	if (nCountDocViews == 0)
		return;
	FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
	if (!frDocView)
		return;
	FS_INT32 nPageViewCount = FRDocViewCountPageViews(frDocView);

	//构建 V 字典项
	FS_DWORD dwVObjNum = 0;
	FPD_Object vObj = FPDDictionaryNew();
	FS_ByteString byType = FSByteStringNew3("TestSignature", -1);
	FPDDictionarySetAtName(vObj, "Type", "Sig");
	FPDDictionarySetAtName(vObj, "ByteRange", "A123456789012345678901234567890123B");
	FPDDictionarySetAtName(vObj, "SubFilter", subfilter);
	FPDDictionarySetAtName(vObj, "Filter", "Adobe.PPKLite");
	//添加V字典项至文档间接引用对象
	dwVObjNum = FPDDocAddIndirectObject(fpdDoc, vObj);

	int count = FSInt32ArrayGetSize(pageArr);

	for (int i = 0; i < count; i++)
	{
		//sample只展示在两个页面添加签名
		if (i == 2)
			break;

		FR_SignatureInfo frSgInfo;
		//memset(&frSgInfo, 0, sizeof(frSgInfo));
		wcscpy(frSgInfo.wsCN, L"Foxit Software");
		wcscpy(frSgInfo.SignDictInfo.wsReason, L"Foxit Software");
		frSgInfo.Icontype = FR_SG_ICONTYPE::FR_SGIT_GRAPHICS;
		frSgInfo.lShowFlag = 0;
		FS_WCHAR imagePath[260] = L"F:\\select.png";
		frSgInfo.nImageOpt = 100;
		//图标路径
		memcpy(frSgInfo.ImagePath, imagePath, 260 * sizeof(wchar_t));
		frSgInfo.bUseStandAppearance = false;

		//设置签名后文档的权限
		frSgInfo.PermissionType = FR_SG_PERMISSION::FR_APG_NONE;

		//签名信息显示的方式
		frSgInfo.TextDir = FR_SG_TEXTDIR::FR_SGTD_AUTO;
		FS_WCHAR wsCN[64] = L"jingjing";
		//签名author
		memcpy(frSgInfo.wsCN, wsCN, 64 * sizeof(wchar_t));
		FS_WCHAR wsC[64] = L"jing";
		//国家或地区信息
		memcpy(frSgInfo.wsC, wsC, 64 * sizeof(wchar_t));
		FS_WCHAR wsOU[64] = L"jing";
		//组织单元信息
		memcpy(frSgInfo.wsOU, wsOU, 64 * sizeof(wchar_t));
		FS_WCHAR wsE[64] = L"jingjing_tang@foxitsoftwrae.com";
		//邮箱信息
		memcpy(frSgInfo.wsE, wsE, 128 * sizeof(wchar_t));
		FS_WCHAR wsO[64] = L"jing";
		//组织名称
		memcpy(frSgInfo.wsO, wsO, 64 * sizeof(wchar_t));

		//签名密码信息
		memcpy(frSgInfo.wsPassword, L"123456", 64 * sizeof(wchar_t));
		//外观样式名称
		memcpy(frSgInfo.wsAppearance, L"", 64 * sizeof(wchar_t));
		FR_SignatureDictInfo dicInfo;
		memcpy(dicInfo.wsLocation, L"", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsName, L"jingjing", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsReason, L"I am the author of this document", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsFilter, L"", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsDate, L"2020/07/13", 128 * sizeof(wchar_t));
		frSgInfo.SignDictInfo = dicInfo;
		memcpy(frSgInfo.wsCertFile, L"F:\\PhantomCode\\12.0Branch\\starship\\PluginSDK\\Samples\\SignatureDemo\\jingjing.pfx", MAX_PATH * sizeof(wchar_t));
		if (i == count-1)
			frSgInfo.bExecutesign = true;
		else
			frSgInfo.bExecutesign = false;
		//frSgInfo.bExecutesign = true;

		FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
		FR_PageView frPageView = FRDocViewGetCurrentPageView(frDocView);
		FS_INT32 pageIndex = FRPageViewGetPageIndex(frPageView);
		FS_Rect rectPage = FRPageViewGetPageRect(frPageView);
		FR_SignaturePosInfo frSgPosInfo;
		memset(&frSgPosInfo, 0, sizeof(FR_SignaturePosInfo));
		frSgPosInfo.pPDFDoc = fpdDoc;
		FS_WCHAR		FileSavePath[260] = L"E:\\PDFFile\\testfile\\signed.pdf";
		memcpy(frSgPosInfo.FileSavePath, FileSavePath, 260 * sizeof(wchar_t));
		frSgPosInfo.nPageIndex = i;
		frSgPosInfo.pFieldDict = theApp.CreateFieldDict(frDoc, i, dwVObjNum);

		//FRCustomSignatureGenerateSignInfo(&frSgInfo, &frSgPosInfo);
		FRCustomSignatureGenerateSignInfo2(&frSgInfo, &frSgPosInfo, true, NULL);
	}
}

FPD_Object CStarterApp::CreateFieldDict(FR_Document frDoc, FS_INT32 pageIndex, FS_DWORD dwVObjNum)
{
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
	FR_PageView frPageView = FRDocViewGetPageView(frDocView, pageIndex);

	//构建field字典
	FPD_Object pSignFDict = FRCustomSignatureFoixtCreateSignatureF(frDoc, false);
	if (!pSignFDict)
		return NULL;

	FPD_Object pDict = FPDDocGetPage(fpdDoc, pageIndex);
	FPD_Page fpdPage = FPDPageNew(); //initialized fpdfPage
	FPDPageLoad(fpdPage, fpdDoc, pDict, TRUE);
	FPDPageParseContent(fpdPage, NULL);
	FS_FloatRect pageRect = FPDPageGetPageBBox(fpdPage);

	FS_FloatRect signRect ;
	signRect.left = pageRect.left+ ((pageRect.right - pageRect.left) / 2 - 100);
	signRect.right = signRect.left + 64;
	signRect.top = (pageRect.top - pageRect.bottom) / 2;
	signRect.bottom = signRect.top - 64;

	//设置widget的BBOX，需要传入页面坐标
	FRCustomSignatureSetPosition(frPageView, pSignFDict, signRect);

	//将V字典项加入field字典项
	FPDDictionarySetAtReferenceToDoc(pSignFDict, "V", fpdDoc, dwVObjNum);

	/*auto pAPDict = FPDDictionaryGetDict(pSignFDict, "AP");
	if (pAPDict == nullptr)
	{
		pAPDict = FPDDictionarySetNewAt(pSignFDict, "AP", FPD_OBJ_DICTIONARY);
	}
	FS_DWORD dwApObjNum = 0;
	FS_DIBitmap pBitmap = FSDIBitmapLoadFromPNGIcon(L"F:\\select.png");
	FPD_Object pObj = CreateAppearanceDict(fpdDoc, pBitmap, 0, &dwApObjNum);
	FPDDictionarySetAtReferenceToDoc(pAPDict, "N", fpdDoc, dwApObjNum);*/

	return pSignFDict;
}


FPD_Object CStarterApp::CreateAppearanceDict(FPD_Document pdfDoc, FS_DIBitmap bitmap, int nRotate, FS_DWORD *pObjNum)
{
	if (!pdfDoc || !bitmap)
		return nullptr;
	FS_DWORD dwNewObjNumber = 0;
	FPD_Object apperenceDict = FPDDictionaryNew();

	FS_AffineMatrix streamMatrix;
	switch (nRotate)
	{
	case 0:
	{
		streamMatrix.a = 1.0f;
		streamMatrix.b = 0.0f;
		streamMatrix.c = 0.0f;
		streamMatrix.d = 1.0f;
	}
	break;
	case 90:
	{
		streamMatrix.a = 0.0f;
		streamMatrix.b = 1.0f;
		streamMatrix.c = -1.0f;
		streamMatrix.d = 0.0f;
	}
	break;
	case 180:
	{
		streamMatrix.a = -1.0f;
		streamMatrix.b = 0.0f;
		streamMatrix.c = 0.0f;
		streamMatrix.d = -1.0f;
	}
	break;
	case 270:
	{
		streamMatrix.a = 0.0f;
		streamMatrix.b = -1.0f;
		streamMatrix.c = 1.0f;
		streamMatrix.d = 0.0f;
	}
	break;
	default:
		break;
	}
	streamMatrix.e = 0;
	streamMatrix.f = 0;
	FPDDictionarySetAtMatrix(apperenceDict, "Matrix", streamMatrix);
	FPD_Object resourcesDict = FPDDictionaryNew();
	FPD_Object xObject = FPDDictionaryNew();

	FPD_Image image = FPDImageNew(pdfDoc);
	//ON_SCOPE_EXIT([&] {

	//});
	FPDImageSetImage(image, bitmap, FALSE, nullptr);
	FPD_Object imgStream = FPDImageGetStream(image);
	if (imgStream)
	{
		dwNewObjNumber = FPDDocAddIndirectObject(pdfDoc, imgStream);
		FPDDictionarySetAtReferenceToDoc(xObject, "ImgSignature", pdfDoc, dwNewObjNumber);
	}
	FPDDictionarySetAt(resourcesDict, "XObject", xObject, nullptr);

	FPD_Object extGStateDict = FPDDictionaryNew();
	FPDDictionarySetAtNumber(extGStateDict, "CA", 1);
	FPDDictionarySetAtNumber(extGStateDict, "ca", 1);
	FS_DWORD dwNewExtGStateObjNumber = FPDDocAddIndirectObject(pdfDoc, extGStateDict);
	FPD_Object extGStateDictRef = FPDDictionaryNew();
	FPDDictionarySetAtReferenceToDoc(extGStateDictRef, "FXE1", pdfDoc, dwNewExtGStateObjNumber);
	FPDDictionarySetAt(resourcesDict, "ExtGState", extGStateDictRef, nullptr);

	FPDDictionarySetAt(apperenceDict, "Resources", resourcesDict, nullptr);

	int bmpWidth = FSDIBitmapGetWidth(bitmap);
	int bmpHeight = FSDIBitmapGetHeight(bitmap);

	FPD_Object streamBBox = FPDArrayNew();
	FPDArrayAddInteger(streamBBox, 0);
	FPDArrayAddInteger(streamBBox, 0);
	FPDArrayAddInteger(streamBBox, bmpWidth);
	FPDArrayAddInteger(streamBBox, bmpHeight);

	FPDDictionarySetAt(apperenceDict, "BBox", streamBBox, nullptr);
	FPDDictionarySetAtName(apperenceDict, "Type", "XObject");
	FPDDictionarySetAtName(apperenceDict, "Subtype", "Form");

	FS_ByteString apStreamData = FSByteStringNew();

	FSByteStringFormat(apStreamData, "q\n/FXE1 gs\n%d 0 0 %d 0 0 cm\n/ImgSignature Do\nQ", bmpWidth, bmpHeight);

	auto pNewContents = FPDStreamNew();
	FPDStreamInitStream(pNewContents, NULL, 0, apperenceDict);
	FPDStreamSetData(pNewContents, (FS_LPBYTE)FSByteStringCastToLPCBYTE(apStreamData),
		FSByteStringGetLength(apStreamData), FALSE, FALSE);
	auto dwObjNum = FPDDocAddIndirectObject(pdfDoc, pNewContents);
	if (pObjNum)
		*pObjNum = dwObjNum;

	FSByteStringDestroy(apStreamData);
	FPDImageDestroy(image);
	return pNewContents;
}

void CStarterApp::SealSignatureProc(void* pData)
{
	FR_Document frDoc = FRAppGetActiveDocOfPDDoc();
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FS_INT32 nCountDocViews = FRDocCountDocViews(frDoc);
	if (nCountDocViews == 0)
		return;
	FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
	if (!frDocView)
		return;
	FS_INT32 nPageViewCount = FRDocViewCountPageViews(frDocView);
	//
	theApp.first_page_percent = 0.3;

	//构建 V 字典项
	FS_DWORD dwVObjNum = 0;
	FPD_Object vObj = FPDDictionaryNew();
	FS_ByteString byType = FSByteStringNew3("TestSignature", -1);
	FPDDictionarySetAtName(vObj, "Type", "Sig");
	FPDDictionarySetAtName(vObj, "ByteRange", "A123456789012345678901234567890123B");
	FPDDictionarySetAtName(vObj, "SubFilter", "adbe.pkcs7.detached");
	FPDDictionarySetAtName(vObj, "Filter", "Adobe.PPKLite");
	//添加V字典项至文档间接引用对象
	dwVObjNum = FPDDocAddIndirectObject(fpdDoc, vObj);

	int count = FPDDocGetPageCount(fpdDoc);
	FS_DIBitmap pBitmap = FSDIBitmapLoadFromPNGIcon(L"F:\\tuzhan.png");
	for (int i = 0; i < count; i++)
	{
		FR_SignatureInfo frSgInfo;
		//memset(&frSgInfo, 0, sizeof(frSgInfo));
		wcscpy(frSgInfo.wsCN, L"Foxit Software");
		wcscpy(frSgInfo.SignDictInfo.wsReason, L"Foxit Software");
		//frSgInfo.Icontype = FR_SG_ICONTYPE::FR_SGIT_GRAPHICS;
		frSgInfo.Icontype = FR_SG_ICONTYPE::FR_SGIT_NONE;
		frSgInfo.lShowFlag = 0;
		FS_WCHAR imagePath[260] = L"F:\\tuzhan.png";
		//图标路径
		memcpy(frSgInfo.ImagePath, imagePath, 260 * sizeof(wchar_t));
		frSgInfo.bUseStandAppearance = false;

		//设置签名后文档的权限
		frSgInfo.PermissionType = FR_SG_PERMISSION::FR_APG_NONE;

		//签名信息显示的方式
		frSgInfo.TextDir = FR_SG_TEXTDIR::FR_SGTD_AUTO;
		FS_WCHAR wsCN[64] = L"jingjing";
		//签名author
		memcpy(frSgInfo.wsCN, wsCN, 64 * sizeof(wchar_t));
		FS_WCHAR wsC[64] = L"jing";
		//国家或地区信息
		memcpy(frSgInfo.wsC, wsC, 64 * sizeof(wchar_t));
		FS_WCHAR wsOU[64] = L"jing";
		//组织单元信息
		memcpy(frSgInfo.wsOU, wsOU, 64 * sizeof(wchar_t));
		FS_WCHAR wsE[64] = L"jingjing_tang@foxitsoftwrae.com";
		//邮箱信息
		memcpy(frSgInfo.wsE, wsE, 128 * sizeof(wchar_t));
		FS_WCHAR wsO[64] = L"jing";
		//组织名称
		memcpy(frSgInfo.wsO, wsO, 64 * sizeof(wchar_t));

		//签名密码信息
		memcpy(frSgInfo.wsPassword, L"123456", 64 * sizeof(wchar_t));
		//外观样式名称
		memcpy(frSgInfo.wsAppearance, L"", 64 * sizeof(wchar_t));
		FR_SignatureDictInfo dicInfo;
		memcpy(dicInfo.wsLocation, L"", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsName, L"jingjing", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsReason, L"I am the author of this document", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsFilter, L"", 128 * sizeof(wchar_t));
		memcpy(dicInfo.wsDate, L"2020/07/13", 128 * sizeof(wchar_t));
		frSgInfo.SignDictInfo = dicInfo;
		memcpy(frSgInfo.wsCertFile, L"F:\\PhantomCode\\12.0Branch\\starship\\PluginSDK\\Samples\\SignatureDemo\\jingjing.pfx", MAX_PATH * sizeof(wchar_t));
		if (i == count - 1)
			frSgInfo.bExecutesign = true;
		else
			frSgInfo.bExecutesign = false;
		//frSgInfo.bExecutesign = true;

		FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
		FR_PageView frPageView = FRDocViewGetCurrentPageView(frDocView);
		FS_INT32 pageIndex = FRPageViewGetPageIndex(frPageView);
		FS_Rect rectPage = FRPageViewGetPageRect(frPageView);
		FR_SignaturePosInfo frSgPosInfo;
		memset(&frSgPosInfo, 0, sizeof(FR_SignaturePosInfo));
		frSgPosInfo.pPDFDoc = fpdDoc;
		FS_WCHAR		FileSavePath[260] = L"E:\\PDFFile\\testfile\\signed.pdf";
		memcpy(frSgPosInfo.FileSavePath, FileSavePath, 260 * sizeof(wchar_t));
		frSgPosInfo.nPageIndex = i;
		frSgPosInfo.pFieldDict = theApp.CreateSealFieldDict(frDoc, i, dwVObjNum, count, pBitmap);

		FR_SignatureExtendInfo extendInfo;
		extendInfo.lStructSize = sizeof(FR_SignatureExtendInfo);
		extendInfo.blendMode = 1;

		//FRCustomSignatureGenerateSignInfo(&frSgInfo, &frSgPosInfo);
		FRCustomSignatureGenerateSignInfo2(&frSgInfo, &frSgPosInfo, true, &extendInfo);
	}
}

FPD_Object CStarterApp::CreateSealFieldDict(FR_Document frDoc, FS_INT32 pageIndex, FS_DWORD dwVObjNum, FS_INT32 pagecount, FS_DIBitmap pBitmap)
{
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FR_DocView frDocView = FRDocGetCurrentDocView(frDoc);
	FR_PageView frPageView = FRDocViewGetPageView(frDocView, pageIndex);

	//构建field字典
	FPD_Object pSignFDict = FRCustomSignatureFoixtCreateSignatureF(frDoc, false);
	if (!pSignFDict)
		return NULL;

	FPD_Object pDict = FPDDocGetPage(fpdDoc, pageIndex);
	FPD_Page fpdPage = FPDPageNew(); //initialized fpdfPage
	FPDPageLoad(fpdPage, fpdDoc, pDict, TRUE);
	FPDPageParseContent(fpdPage, NULL);
	FS_FloatRect pageRect = FPDPageGetPageBBox(fpdPage);
	FS_FloatRect signRect;

	int bmpWidth = FSDIBitmapGetWidth(pBitmap);
	int bmpHeight = FSDIBitmapGetHeight(pBitmap);

	float other_page_percent = (1 - first_page_percent) / (pagecount - 1);
	//设置签章图片显示的位置
	if (pageIndex == 0)
	{
		signRect.left = pageRect.right - bmpWidth * first_page_percent;
		signRect.right = pageRect.right;
		signRect.top = (pageRect.top - pageRect.bottom) / 2 - (bmpHeight / 2);
		signRect.bottom = signRect.top + bmpHeight;
	}
	else
	{
		signRect.left = pageRect.right - bmpWidth * other_page_percent;
		signRect.right = pageRect.right;
		signRect.top = (pageRect.top - pageRect.bottom) / 2 - (bmpHeight / 2);
		signRect.bottom = signRect.top + bmpHeight;
	}


	//设置widget的BBOX，需要传入页面坐标
	FRCustomSignatureSetPosition(frPageView, pSignFDict, signRect);

	//将V字典项加入field字典项
	FPDDictionarySetAtReferenceToDoc(pSignFDict, "V", fpdDoc, dwVObjNum);

	auto pAPDict = FPDDictionaryGetDict(pSignFDict, "AP");
	if (pAPDict == nullptr)
	{
		pAPDict = FPDDictionarySetNewAt(pSignFDict, "AP", FPD_OBJ_DICTIONARY);
	}
	FS_DWORD dwApObjNum = 0;
	FPD_Object pObj = CreateSealAppearanceDict(fpdDoc, pBitmap, 0, &dwApObjNum, pageIndex, pagecount);
	FPDDictionarySetAtReferenceToDoc(pAPDict, "N", fpdDoc, dwApObjNum);

	return pSignFDict;
}

FPD_Object CStarterApp::CreateSealAppearanceDict(FPD_Document pdfDoc, FS_DIBitmap bitmap, int nRotate, FS_DWORD *pObjNum, int pageIndex, FS_INT32 pagecount)
{
	if (!pdfDoc || !bitmap)
		return nullptr;
	FS_DWORD dwNewObjNumber = 0;
	FPD_Object apperenceDict = FPDDictionaryNew();

	FS_AffineMatrix streamMatrix;
	switch (nRotate)
	{
	case 0:
	{
		streamMatrix.a = 1.0f;
		streamMatrix.b = 0.0f;
		streamMatrix.c = 0.0f;
		streamMatrix.d = 1.0f;
	}
	break;
	case 90:
	{
		streamMatrix.a = 0.0f;
		streamMatrix.b = 1.0f;
		streamMatrix.c = -1.0f;
		streamMatrix.d = 0.0f;
	}
	break;
	case 180:
	{
		streamMatrix.a = -1.0f;
		streamMatrix.b = 0.0f;
		streamMatrix.c = 0.0f;
		streamMatrix.d = -1.0f;
	}
	break;
	case 270:
	{
		streamMatrix.a = 0.0f;
		streamMatrix.b = -1.0f;
		streamMatrix.c = 1.0f;
		streamMatrix.d = 0.0f;
	}
	break;
	default:
		break;
	}
	streamMatrix.e = 0;
	streamMatrix.f = 0;
	FPDDictionarySetAtMatrix(apperenceDict, "Matrix", streamMatrix);
	FPD_Object resourcesDict = FPDDictionaryNew();
	FPD_Object xObject = FPDDictionaryNew();

	FPD_Image image = FPDImageNew(pdfDoc);
	//ON_SCOPE_EXIT([&] {

	//});
	FPDImageSetImage(image, bitmap, FALSE, nullptr);
	FPD_Object imgStream = FPDImageGetStream(image);
	if (imgStream)
	{
		dwNewObjNumber = FPDDocAddIndirectObject(pdfDoc, imgStream);
		FPDDictionarySetAtReferenceToDoc(xObject, "ImgSignature", pdfDoc, dwNewObjNumber);
	}
	FPDDictionarySetAt(resourcesDict, "XObject", xObject, nullptr);

	FPD_Object extGStateDict = FPDDictionaryNew();
	FPDDictionarySetAtNumber(extGStateDict, "CA", 1);
	FPDDictionarySetAtNumber(extGStateDict, "ca", 1);
	FS_DWORD dwNewExtGStateObjNumber = FPDDocAddIndirectObject(pdfDoc, extGStateDict);
	FPD_Object extGStateDictRef = FPDDictionaryNew();
	FPDDictionarySetAtReferenceToDoc(extGStateDictRef, "FXE1", pdfDoc, dwNewExtGStateObjNumber);
	FPDDictionarySetAt(resourcesDict, "ExtGState", extGStateDictRef, nullptr);

	FPDDictionarySetAt(apperenceDict, "Resources", resourcesDict, nullptr);

	int bmpWidth = FSDIBitmapGetWidth(bitmap);
	int bmpHeight = FSDIBitmapGetHeight(bitmap);

	float other_page_percent = (1 - first_page_percent) / (pagecount - 1);
	//通过BBox切割图片，streamBBox 依次填入的值为 left,bottom,right,top，是图片对应的margin值
	FPD_Object streamBBox = FPDArrayNew();
	if (pageIndex == 0)
	{
		FPDArrayAddInteger(streamBBox, 0);
		FPDArrayAddInteger(streamBBox, 0);
		FPDArrayAddInteger(streamBBox, bmpWidth * first_page_percent);
		FPDArrayAddInteger(streamBBox, bmpHeight);
	}
	else
	{
		float left = bmpWidth * first_page_percent + (bmpWidth * other_page_percent * (pageIndex - 1));
		float bottom = 0;
		float right = left + bmpWidth * other_page_percent;
		float top = bmpHeight;

		FPDArrayAddInteger(streamBBox, left);
		FPDArrayAddInteger(streamBBox, bottom);
		FPDArrayAddInteger(streamBBox, right);
		FPDArrayAddInteger(streamBBox, top);
	}

	/*else if (pageIndex == 1)
	{
		FPDArrayAddInteger(streamBBox, bmpWidth / 3);
		FPDArrayAddInteger(streamBBox, 0);
		FPDArrayAddInteger(streamBBox, bmpWidth / 3 + bmpWidth / 3);
		FPDArrayAddInteger(streamBBox, bmpHeight);
	}
	else if (pageIndex == 2)
	{
		FPDArrayAddInteger(streamBBox, (bmpWidth / 3)*2);
		FPDArrayAddInteger(streamBBox, 0);
		FPDArrayAddInteger(streamBBox, (bmpWidth / 3) * 2 + bmpWidth / 3);
		FPDArrayAddInteger(streamBBox, bmpHeight);
	}*/


	FPDDictionarySetAt(apperenceDict, "BBox", streamBBox, nullptr);
	FPDDictionarySetAtName(apperenceDict, "Type", "XObject");
	FPDDictionarySetAtName(apperenceDict, "Subtype", "Form");

	FS_ByteString apStreamData = FSByteStringNew();

	FSByteStringFormat(apStreamData, "q\n/FXE1 gs\n%d 0 0 %d 0 0 cm\n/ImgSignature Do\nQ", bmpWidth, bmpHeight);

	auto pNewContents = FPDStreamNew();
	FPDStreamInitStream(pNewContents, NULL, 0, apperenceDict);
	FPDStreamSetData(pNewContents, (FS_LPBYTE)FSByteStringCastToLPCBYTE(apStreamData),
		FSByteStringGetLength(apStreamData), FALSE, FALSE);
	auto dwObjNum = FPDDocAddIndirectObject(pdfDoc, pNewContents);
	if (pObjNum)
		*pObjNum = dwObjNum;

	FSByteStringDestroy(apStreamData);
	FPDImageDestroy(image);
	return pNewContents;
}


void CStarterApp::InitFoxitSignSignatureHandlerCallbacks()
{
	//return;

	if (NULL != m_pFRCustomSignHandlerCallbacks) return;
	m_pFRCustomSignHandlerCallbacks = std::make_unique<FR_SignatureHandlerCallbacksRec>();
	memset(m_pFRCustomSignHandlerCallbacks.get(), 0, sizeof(FR_SignatureHandlerCallbacksRec));
	m_pFRCustomSignHandlerCallbacks->lStructSize = sizeof(FR_SignatureHandlerCallbacksRec);
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerGetName = (char* (*)(FS_LPVOID))CFoxitSign3StandardHandler::SignatureHandlerGetName;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerSignData = CFoxitSign3StandardHandler::SignatureHandlerSignData;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerTimeStampDate = CFoxitSign3StandardHandler::SignatureHandlerTimeStampDate;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerVerifyData = CFoxitSign3StandardHandler::SignatureHandlerVerifyData;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerShowStateUI = CFoxitSign3StandardHandler::SignatureHandlerShowStateUI;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerCanClear = CFoxitSign3StandardHandler::SignatureHandlerCanClear;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerShowSignProperties = CFoxitSign3StandardHandler::SignatureHandlerShowSignProperties;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerRleaseSignData = CFoxitSign3StandardHandler::SignatureHandlerRleaseSignData;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerSGVerifyCanSign = CFoxitSign3StandardHandler::SignatureHandlerSGVerifyCanSign;
	m_pFRCustomSignHandlerCallbacks->FRSignatureHandlerSGVerifyCanVerify = CFoxitSign3StandardHandler::SignatureHandlerSGVerifyCanVerify;

	m_pCustomSignHandler = FRCustomSignatureCreateSignatureHandler(theApp.m_pFRCustomSignHandlerCallbacks.get());
	//FRCustomSignatureRegisterSignatureHandler(m_pCustomSignHandler);
}

void CStarterApp::ReleaseFoxitSignSignatureHandler()
{
	//FRCustomSignatureDestroySignatureHandler(m_pCustomSignHandler);
}

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

void PILoadRibbonUI(void* pParentWnd)
{
	theApp.CreateRibbonUI();
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
	theApp.InitFoxitSignSignatureHandlerCallbacks();
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
	theApp.ReleaseFoxitSignSignatureHandler();

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
	pData->PIHDRegisterPlugin(pData, "Sample_Signature", (FS_LPCWSTR)L"SignatureDemo");

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

