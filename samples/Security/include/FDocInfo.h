#ifndef FDOCINFO_H
#define FDOCINFO_H
//#include "FR_LockBytesMem.h"
#include <fstream>
#include <string>

#define	  MAX_USE_MEMORY_SIZE	(1024*1024*100)

class CFSecurityDocInfo;

class CFDocInfo{
public:
	CFDocInfo();
	~CFDocInfo();

	FS_WideString filePath;
	FS_DWORD dwOffsetSize;
	FS_DWORD m_dwOffsetSizeBackup;
	FR_Document frDoc;
	FS_BOOL m_bNeedUpdatePath;

	FS_BOOL m_bFileEncrypt;

	CFSecurityDocInfo* pFileSecurityDocInfo;//紧保存个指针方便获取不需要释放
	FS_BOOL m_bNeedReOpen;

	
	FS_LPBYTE m_pByteDoc;
	FS_DWORD m_dwStartPos;
	FS_DWORD m_dwEndPos;

	FS_BOOL m_bNeedOpenFileWhenSave;
	
	std::wstring GetWrapperPayLoadTempPath();
	void SetWrapperType(int nType);
	int GetWrapperType(){return m_nWrapperType;}

	void SetContentSize(FS_DWORD dwPlayLoadSize);
	FS_DWORD GetContentSize();

	FS_BOOL ReadContent(FR_Document frDoc, FS_DWORD pos, FS_LPBYTE pBuf, FS_DWORD size);

	void CloseFile();
	void SetCloseFileEverytime();


	void SetWrapperVersion(int nVersion);
	int GetWrapperVersion();
	//for wrapper2.0
	
	std::fstream* m_pPlayLoadFile;
	
	FS_BOOL m_bIsOpen;
protected:
	int m_nWrapperType;
	int m_nWrapperVersion;
	std::wstring m_strPayLoadPath;
	FS_DWORD m_dwDocSize;

	std::ifstream* m_pFileForRead;
	FS_BOOL m_bCloseEverytime;
	
};

#endif