#include "StdAfx.h"
#include "../../include/FDocInfo.h"
#include "../../include/FSecurityDocInfo.h"
#include "../../include/Callbacks/FoxitContentProviderCallbacks.h"
#include "../../include/CustomStringPtr.h"

const PSTR g_szWRAPPER_VERSION = "Version";
const PSTR g_szENCRYPTDICT_FILTER = "Filter";
FS_BOOL g_BIsFileClosed = FALSE;


FS_BOOL InputPasswordProc(FR_Document frDoc, FS_ByteString bsPassword, FS_BOOL* bReset)
{
	if (theApp.m_TryPasswordCount <= 3)
	{
		FSByteStringFill(bsPassword, theApp.m_defpassword.c_str());
		*bReset = TRUE;
		theApp.m_TryPasswordCount++;
	}
	else
	{
		theApp.m_TryPasswordCount = 0;
		*bReset = FALSE;
		return FALSE;
	}
	return TRUE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnFileOpen(FS_LPVOID clientData, FR_Document frDoc, FS_LPCWSTR lpwszSource, FS_BOOL bIsAttachment)
{
	FS_BOOL bResult = FALSE;
	FPD_Parser fpdParser = NULL;
	fpdParser = FPDParserNew();
	FS_DWORD parseResult = FPDParserStartParseW(fpdParser, lpwszSource, FALSE);
	switch (parseResult)
	{
	case FPD_PARSE_ERROR_SUCCESS:
	{
		return FALSE;
	}
	case FPD_PARSE_ERROR_HANDLER:
	{
		theApp.m_bsecurity = true;
		FPD_Object encryptDict = NULL;
		encryptDict = FPDParserGetEncryptDict(fpdParser);
		FS_DWORD wrapperVersion = FPDDictionaryGetInteger(encryptDict, g_szWRAPPER_VERSION);
		FS_ByteString fsbsTemp = FSByteStringNew();
		FPDDictionaryGetString(encryptDict, g_szENCRYPTDICT_FILTER, &fsbsTemp);
		if (NULL != fpdParser)
		{
			FPDParserDestroy(fpdParser);
			fpdParser = NULL;
		}
		std::ifstream fileTemp;
		std::wstring wssr=std::wstring(lpwszSource);
		std::string szSource = theApp.to_byte_string(lpwszSource);
		fileTemp.open(szSource.c_str(), std::ios::in);
		if(fileTemp.is_open())
		{
			FS_DWORD dwOffset = 0;
			CFDocInfo* pFDocInfo = NULL;
			pFDocInfo = new CFDocInfo;
			FSWideStringFill(pFDocInfo->filePath, lpwszSource);
			pFDocInfo->frDoc = frDoc;

			fileTemp.seekg(0, fileTemp.end);
			FS_DWORD fileSize = fileTemp.tellg();
			fileTemp.seekg(0, fileTemp.beg);


			pFDocInfo->SetContentSize(fileSize);

			DWORD dd = pFDocInfo->GetContentSize();
			pFDocInfo->SetWrapperType(FPD_WRAPPERTYPE_NO);
			theApp.AddDocInfo(pFDocInfo);
			bResult = TRUE;
			fileTemp.close();
		}
		FSByteStringDestroy(fsbsTemp);
	}
	break;
	case FPD_PARSE_ERROR_PASSWORD:
		FPDParserDestroy(fpdParser);
		FRDocSetInputPasswordProc(frDoc, InputPasswordProc);
		return FALSE;
	default:
		break;
	}

	if (NULL != fpdParser)
		FPDParserDestroy(fpdParser);
	
	return bResult;
}

FS_DWORD FoxitContentProviderCallbacks::FRConProviderOnGetPermissions(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD  pdfselfPermissions)
{
	return pdfselfPermissions;
	
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnGetContentSize(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD* pTotalSize)
{

	FS_BOOL bResult = FALSE;

	CFDocInfo* pDocInfo = NULL;
	HRESULT hr = S_OK;
	hr = theApp.GetDocInfo(frDoc, &pDocInfo);
	if(S_OK == hr)
	{
		*pTotalSize = pDocInfo->GetContentSize();
		bResult = TRUE;
	}

	return bResult;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnReadContent(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD pos, FS_LPBYTE pBuf, FS_DWORD size)
{

	FS_BOOL bResult = FALSE;
	HRESULT hr = S_OK;
	CFDocInfo* pDocInfo = NULL;
	hr = theApp.GetDocInfo(frDoc, &pDocInfo);
	if(S_OK == hr)
	{
		bResult = pDocInfo->ReadContent(frDoc, pos, pBuf, size);
	}
	return bResult;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnWriteContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD dwSize, FS_LPCWSTR lpSaveFilePath)
{
	CFSecurityDocInfo* pSecurityDocInfo = NULL;
	if(S_FALSE == theApp.GetSecurityDocInfo(frDoc, &pSecurityDocInfo))
		return FALSE;

	CFDocInfo* pDocInfo = NULL;
	if(S_FALSE == theApp.GetDocInfo(frDoc, &pDocInfo))
		return FALSE;

	if (!pSecurityDocInfo->m_bCreateFileForSave)
	{
		std::string savefilepath = theApp.to_byte_string(lpSaveFilePath);
		pSecurityDocInfo->m_fileForSave.open(savefilepath.c_str(), std::ios::out | std::ios::binary);
		if(pSecurityDocInfo->m_fileForSave.is_open())
		{
			pDocInfo->m_dwOffsetSizeBackup = 0;
			pSecurityDocInfo->m_bCreateFileForSave = TRUE;
		}
	}

	if (pSecurityDocInfo->m_bCreateFileForSave)
	{
		pSecurityDocInfo->m_fileForSave.write((const char*)pBuf, dwSize);
	}
	else
	{
		return FALSE;
	}

	pDocInfo->m_dwOffsetSizeBackup += dwSize;

	return TRUE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnBackFillContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD dwSize, FS_LPCWSTR lpSaveFilePath)
{
	return TRUE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderIsWriteContentProgressive(FS_LPVOID clientData, FR_Document doc, FS_LPCWSTR lpSaveFilePath)
{
	return TRUE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderWriteContentProgressiveFinish(FS_LPVOID clientData, FR_Document doc, FS_LPCWSTR lpSaveFilePath, FS_BOOL bResult)
{
	CFSecurityDocInfo* pSecurityDocInfo = NULL;
	if(S_FALSE == theApp.GetSecurityDocInfo(doc, &pSecurityDocInfo))
		return FALSE;

	CFDocInfo* pDocInfo = NULL;
	if(S_FALSE == theApp.GetDocInfo(doc, &pDocInfo))
		return FALSE;

	if (pSecurityDocInfo->m_bCreateFileForSave)
	{
		if (bResult)
		{
			pDocInfo->dwOffsetSize = pDocInfo->m_dwOffsetSizeBackup;
			if (pDocInfo->GetWrapperType() == FPD_WRAPPERTYPE_NO)
			{
				pDocInfo->SetContentSize(pDocInfo->m_dwOffsetSizeBackup);
				pDocInfo->CloseFile();
			}
			pDocInfo->m_bNeedUpdatePath = TRUE;
		}		

		pSecurityDocInfo->m_fileForSave.close();
		pSecurityDocInfo->m_bCreateFileForSave = FALSE;
		pDocInfo->m_dwOffsetSizeBackup = 0;
	}
	else
	{
		return FALSE;
	}

	if(pSecurityDocInfo->bSecurityRemoved)
		return TRUE;
	

	return TRUE;
}

void FoxitContentProviderCallbacks::FRConProviderOnFileClose(FS_LPVOID clientData, FR_Document frDoc)
{

	CFSecurityDocInfo* pSecurityDocInfo = NULL;
	if(S_OK == theApp.GetSecurityDocInfo(frDoc, &pSecurityDocInfo))
	{
		//log to server when file close
		WideString wsFilePath;
	}

}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnWriteAttachmentContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD size, FS_LPCWSTR lpAttchmntPath)
{
	return -1;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnGetAttachmentSize(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD* pTotalSize, FS_LPCWSTR lpAttchmntPath)
{
	return -1;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderOnReadAttachmentContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD size, FS_LPCWSTR lpAttchmntPath)
{
		return TRUE;
}
FS_BOOL FoxitContentProviderCallbacks::FRConProviderIsPageAvail(FS_LPVOID clientData, FR_Document frDoc, FS_INT32 iPage)
{
	return TRUE;
}
FS_BOOL FoxitContentProviderCallbacks::FRConProviderGetPageSize(FS_LPVOID clientData, FR_Document frDoc, FS_INT32 iPage, FS_FLOAT* outWidth, FS_FLOAT* outHeight)
{
	return FALSE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderDrawOwnerPage(FS_LPVOID clientData, FR_Document frDoc, FS_INT32 iPage, FS_DIBitmap bitmap, FS_INT32 start_x, FS_INT32 start_y, FS_INT32 width, FS_INT32 height)
{
	return FALSE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderNeedReopenDoc(FS_LPVOID clientData, FR_Document frDoc)
{
	CFSecurityDocInfo* pSecurityDocInfo = NULL;
	if(S_FALSE == theApp.GetSecurityDocInfo(frDoc, &pSecurityDocInfo))
		return FALSE;
	CFDocInfo* pDocInfo = NULL;
	if(S_FALSE == theApp.GetDocInfo(frDoc, &pDocInfo))
		return FALSE;

	if (pDocInfo->GetWrapperType() == FPD_WRAPPERTYPE_PDF2 &&
		pSecurityDocInfo->bSecurityRemoved)
	{
		return TRUE;
	}
	return FALSE;
}

FS_BOOL FoxitContentProviderCallbacks::FRConProviderCanBeSaved(FS_LPVOID clientData, FR_Document frDoc)
{
	return TRUE;
}

FS_BOOL FoxitContentProviderCallbacks::FRContentProviderIsProcessErrMsg(FS_LPVOID clientData)
{
	return FALSE;
}


