#include "StdAfx.h"
#include "../include/FDocInfo.h"

void CFDocInfo::SetContentSize(FS_DWORD dwPlayLoadSize) {
	m_dwDocSize = dwPlayLoadSize;
}
CFDocInfo::CFDocInfo() :
	filePath(NULL),
	dwOffsetSize(0),
	frDoc(NULL),
	m_pByteDoc(NULL),
	m_dwStartPos(0),
	m_dwEndPos(0),
	m_nWrapperType(FPD_WRAPPERTYPE_FOXIT),
	m_nWrapperVersion(0),
	m_pPlayLoadFile(NULL),
	m_bIsOpen(FALSE),
	m_dwDocSize(0),
	m_pFileForRead(NULL)

{
	filePath = FSWideStringNew();
	m_bNeedUpdatePath = FALSE;
	m_dwOffsetSizeBackup = 0;
	m_bNeedOpenFileWhenSave = TRUE;
	m_bCloseEverytime = FALSE;
}

CFDocInfo::~CFDocInfo()
{
	if (NULL != filePath)
		FSWideStringDestroy(filePath);

	if (NULL != m_pByteDoc)
		delete[] m_pByteDoc;

	if (m_pFileForRead != NULL)
	{
		m_pFileForRead->close();
		delete m_pFileForRead;
		m_pFileForRead = NULL;
	}



	if (m_nWrapperType == FPD_WRAPPERTYPE_PDF2 && !m_strPayLoadPath.empty())
	{
		::DeleteFile(m_strPayLoadPath.c_str());
	}
}



std::wstring CFDocInfo::GetWrapperPayLoadTempPath()
{
	return m_strPayLoadPath;
}

void CFDocInfo::SetWrapperType(int nType)
{
	m_nWrapperType = nType;
}

FS_DWORD CFDocInfo::GetContentSize()
{
		return m_dwDocSize;
}

FS_BOOL CFDocInfo::ReadContent(FR_Document frDoc, FS_DWORD pos, FS_LPBYTE pBuf, FS_DWORD size)
{
	FS_BOOL bResult = FALSE;
	FS_BOOL bOpen = TRUE;

	if (GetWrapperType() == FPD_WRAPPERTYPE_PDF2)
	{
		if (m_pFileForRead == NULL)
		{
			m_pFileForRead = new std::ifstream();
			std::string szPayLoadPath = theApp.to_byte_string(m_strPayLoadPath.c_str());
			m_pFileForRead->open(szPayLoadPath, std::ios::in | std::ios::binary);
		}
		if (m_pFileForRead->is_open())
		{
			m_pFileForRead->seekg(pos, m_pFileForRead->beg);
			m_pFileForRead->read((char*)pBuf, size);
			bResult = TRUE;
			if (m_bCloseEverytime)
			{
				CloseFile();
			}
		}
	
	}
	else
	{
		if (m_bNeedUpdatePath)
		{
			FRDocGetFilePath(frDoc, &filePath);
			if (m_pFileForRead != NULL)
			{
				m_pFileForRead->close();
				delete m_pFileForRead;
				m_pFileForRead = NULL;
			}

			m_bNeedUpdatePath = FALSE;
		}

		if (m_pFileForRead == NULL)
		{
			m_pFileForRead = new std::ifstream();
			FS_LPCWSTR lpwsFilePath = FSWideStringCastToLPCWSTR(filePath);
			std::string szfilepath = theApp.to_byte_string(lpwsFilePath);
			m_pFileForRead->open(szfilepath.c_str(), std::ios::in | std::ios::binary);
		}

		if (m_pFileForRead->is_open())
		{
			m_pFileForRead->seekg(pos, m_pFileForRead->beg);
			m_pFileForRead->read((char*)pBuf, size);
			bResult = TRUE;
			if (m_bCloseEverytime)
			{
				CloseFile();
			}
		}
	}
	return bResult;
}

void CFDocInfo::CloseFile()
{
	if (m_pFileForRead != NULL)
	{
		m_pFileForRead->close();
		delete m_pFileForRead;
		m_pFileForRead = NULL;
	}
	
}

void CFDocInfo::SetCloseFileEverytime()
{
	m_bCloseEverytime = TRUE;
}

void CFDocInfo::SetWrapperVersion(int nVersion)
{
	m_nWrapperVersion = nVersion;
}

int CFDocInfo::GetWrapperVersion()
{
	return m_nWrapperVersion;
}
