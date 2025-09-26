//#include "Internal_include.h"
#include "StdAfx.h"
#include "UIExtention.h"
#include <string>
using namespace std;


CRibbonUIManager::MAP_RIBELE_IMAGES CRibbonUIManager::s_mapRibEleImages;
CRibbonUIManager*	CRibbonUIManager::Create()
{
	return new CRibbonUIManager();
}

std::wstring OpenPdffile()
{
#ifdef _WIN32
	CString FilePathName;
	CFileDialog dlg(TRUE, 
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("PDF Files (*.pdf)|*.pdf|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
		return std::wstring(FilePathName.GetBuffer(FilePathName.GetLength()));
	}
	else
	{
		return L"";

	}
#else

    QString fileName = QFileDialog::getOpenFileName(
            NULL,
            "open a file.",
            "",
            "pdf(*.pdf);;All files(*.*)");
        if (fileName.isEmpty()) {
//            QMessageBox::warning(this, "Warning!", "Failed to open the video!");
        }
        else {
            return fileName.toStdWString();

        }


#endif

}

std::wstring GetSavePdffile()
{
#ifdef _WIN32
	CString FilePathName;
	CFileDialog dlg(FALSE, 
		L"*.pdf",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("PDF Files (*.pdf)|*.pdf|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
		
		return std::wstring(FilePathName.GetBuffer(FilePathName.GetLength()));
	}
	else
	{
		return L"";

	}
#else
    QString filePath = QFileDialog::getSaveFileName(NULL,
                                                    QString::fromLocal8Bit("save pdf"),
                                                    "/tmp/",
                                                    "pdf (*.pdf);");

    return filePath.toStdWString();
#endif
}


void CRibbonUIManager::CustomerEncryptExecProc(void * pData)
{
	theApp.m_bsecurity = true;
	FPD_Object objEncryptDict = NULL;
	std::wstring pdfpath = OpenPdffile();
	if (pdfpath.empty())
	{
		return;
	}
	FR_Document frDoc = FRDocOpenFromFile(pdfpath.c_str(), (FS_LPCSTR)L"", TRUE, TRUE);
	objEncryptDict = FPDDictionaryNew(); // need not to destroy
	BOOL bMetaData = FALSE;

	FS_WideString fswsFilePath = FSWideStringNew();
	
	std:wstring resave_path = GetSavePdffile();
	FSWideStringFill(fswsFilePath, (FS_LPCWSTR)resave_path.c_str());

	FPD_Creator fpdCreator = FRDocGetPDFCreator(frDoc);
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FPD_Parser fpdParser = FPDDocGetParser(fpdDoc);


	FS_DWORD objNum = 0;
	objNum = FPDDocAddIndirectObject(fpdDoc, objEncryptDict);

	FS_ByteString bs = FSByteStringNew();
	FSByteStringFill(bs, "filter");
	FPDDictionarySetAtName(objEncryptDict, "Filter", "filter");
	
 	FPD_Object trailerDict = FPDParserGetTrailer(fpdParser);
 	FPDDictionarySetAtReferenceToDoc(trailerDict, "Encrypt", fpdDoc, objNum);

	void* pSecurityData = FPDCreatorSetCustomSecurity(fpdCreator, objEncryptDict, theApp.m_pFRCryptoCallbacks.get(), bMetaData);

	FS_BOOL bChanged = FRDocGetChangeMark(frDoc);
	FRDocSetChangeMark(frDoc);
	FS_BOOL bSaveAsResult = FALSE;
	bSaveAsResult = FRDocDoSaveAs2(
		frDoc,
		FSWideStringCastToLPCWSTR(fswsFilePath),
		NULL,
		NULL,
		FALSE,
		FALSE);

	if (pSecurityData != NULL)
	{
		FPDCreatorReleaseCustomSecurityData(fpdCreator, pSecurityData);
	}

	if (!bSaveAsResult)
	{
		if (!bChanged)
			FRDocClearChangeMark(frDoc);
	}
	FSWideStringDestroy(fswsFilePath);
	theApp.m_bsecurity = false;
}

void CRibbonUIManager::CustomerDecryptExecProc(void * pData)
{
	std::wstring pdfpath = OpenPdffile();
	if (pdfpath.empty())
	{
#ifdef _WIN32
        AfxMessageBox(L"please open a customer pdf file");
#else
        QMessageBox::information(NULL, "Warnint", "please open a customer pdf file",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
#endif
		return;
	}
	FR_Document m_pdfDoc = FRDocOpenFromFile(pdfpath.c_str(), (FS_LPCSTR)L"", TRUE, TRUE);

}

FS_BOOL CRibbonUIManager::CustomerEnabledProc(void * pData)
{
	return TRUE;
}

void CRibbonUIManager::StandardDecryptExecProc(void * pData)
{
	std::wstring pdfpath = OpenPdffile();
	if (pdfpath.empty())
	{
#ifdef _WIN32
        AfxMessageBox(L"please open a password pdf file");
#else
        QMessageBox::information(NULL, "Warning", "please open a password pdf file",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
#endif
		return ;
	}
	FR_Document m_pdfDoc = FRDocOpenFromFile(pdfpath.c_str(), (FS_LPCSTR)L"", TRUE, TRUE);
	return ;
}

void CRibbonUIManager::StandardEncryptExecProc(void * pData)
{
	theApp.m_bsecurity = true;
	std::wstring pdfpath = OpenPdffile();
	if (pdfpath.empty())
	{
		return;
	}
	FR_Document frDoc = FRDocOpenFromFile(pdfpath.c_str(), (FS_LPCSTR)L"", TRUE, TRUE);
 	FPD_Object objEncryptDict = NULL;
	//frDoc = FRAppGetActiveDocOfPDDoc();
	objEncryptDict = FPDDictionaryNew(); // need not to destroy
	BOOL bMetaData = FALSE;

 	FS_WideString fswsFilePath = FSWideStringNew();
	std:wstring resave_path = GetSavePdffile();
	FSWideStringFill(fswsFilePath, (FS_LPCWSTR)resave_path.c_str());
	FPD_Creator fpdCreator = FRDocGetPDFCreator(frDoc);
	FPD_Document fpdDoc = FRDocGetPDDoc(frDoc);
	FPD_Parser fpdParser = FPDDocGetParser(fpdDoc);

	FS_DWORD objNum = 0;
	objNum = FPDDocAddIndirectObject(fpdDoc, objEncryptDict);


	FS_ByteString userPwd = FSByteStringNew3(theApp.m_defpassword.c_str(), theApp.m_defpassword.size());
	FS_ByteString ownerPwd = FSByteStringNew3(theApp.m_defpassword.c_str(), theApp.m_defpassword.size());
	int userpwdLen = FSByteStringGetLength(userPwd);
	int ownerPwdLen = FSByteStringGetLength(ownerPwd);
	FS_LPCBYTE cbUserPwd = FSByteStringCastToLPCBYTE(userPwd);
	FS_LPCBYTE cbOwnerPwd = FSByteStringCastToLPCBYTE(ownerPwd);
	FPDCreatorSetStandardSecurity(fpdCreator, 0x0004, cbUserPwd, userpwdLen, cbOwnerPwd, ownerPwdLen, FRCIPHER_AES,
		16, FALSE);


	FS_BOOL bChanged = FRDocGetChangeMark(frDoc);
	FRDocSetChangeMark(frDoc);
	FS_BOOL bSaveAsResult = FALSE;
	bSaveAsResult = FRDocDoSaveAs2(
		frDoc,
		FSWideStringCastToLPCWSTR(fswsFilePath),
		NULL,
		NULL,
		FALSE,
		FALSE);

	if (!bSaveAsResult)
	{
		if (!bChanged)
			FRDocClearChangeMark(frDoc);
	}
	FSWideStringDestroy(fswsFilePath);
	theApp.m_bsecurity = false;
}

void CRibbonUIManager::RibbonElementImageInitProc(FR_RibbonElement ribbonElement, void* clientdata)
{
	MAP_RIBELE_IMAGES::const_iterator citer = CRibbonUIManager::s_mapRibEleImages.find(ribbonElement);
	if (CRibbonUIManager::s_mapRibEleImages.cend() == citer)
		return;
	FS_DIBitmap fsDIBitmap32ID = FSDIBitmapLoadFromPNGIcon(citer->second.wsPNG32Path.c_str());
	FS_DIBitmap fsDIBitmap16ID = FSDIBitmapLoadFromPNGIcon(citer->second.wsPNG16Path.c_str());

	FRRibbonElementSetImage(ribbonElement,
		citer->second.wsPNG32Path.empty() ? (FS_DIBitmap)NULL : fsDIBitmap32ID,
		citer->second.wsPNG16Path.empty() ? (FS_DIBitmap)NULL : fsDIBitmap16ID);

	CRibbonUIManager::s_mapRibEleImages.erase(citer);

	FSDIBitmapDestroy(fsDIBitmap32ID);
	FSDIBitmapDestroy(fsDIBitmap16ID);
}



void CRibbonUIManager::RibbonExecuteProc(void *clientData)
{

}

FS_BOOL CRibbonUIManager::RibbonComputeEnabledProc(void *clientData)
{
	ElemActionData* pClientData = (ElemActionData*)clientData;
	if (!pClientData) return FALSE;
	return pClientData->bDefaultEnabled;
}

void CRibbonUIManager::RibbonFreeDataProc(void *clientData)
{
	ElemActionData* pClientData = (ElemActionData*)clientData;
	if (pClientData != NULL)
	{
		delete pClientData;
		pClientData = NULL;
	}
}


