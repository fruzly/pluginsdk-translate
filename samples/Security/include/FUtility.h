#ifndef FUTILITY_H
#define FUTILITY_H

#include <ObjSel.h>
#include <winnt.h>
//#include "../include/OutlookHeader.h"
#include <vector>
#include <list>
#include "CustomStringPtr.h"




class FUtility{
	FUtility();

public:
	static BOOL GetEmailFromList(const CString& EmailList, int nIndex, CString& email);
	static BOOL ValidateEmail(const CString& email);

	static BOOL ValidateEmailList(const CString& EmailList, CString& invalidEmail);
	static HRESULT SelectUsers(HWND hParent, PCWSTR wszTargetComputer, CString& selUser);	
	static HRESULT InitObjectPicker(IDsObjectPicker* pDsObjectPicker, PCWSTR wszTargetComputer);
	static HRESULT ProcessSelectedObjects(IDataObject* pdo, CString& selUser);

	//static Outlook::_ApplicationPtr CreateOutLookApp();
// 	static HRESULT SelectUsersByOutlookModel(Outlook::_ApplicationPtr spOutlookApp, int nBtnType,LPCWSTR lpwsFirstLabel, LPCWSTR lpwsSecondLabel,
// 		CString& wsUserRead, CString& wsUserChange, BOOL& bCanUse);
	static HRESULT CheckUserByOutlookModel(CString& wsAddressToCheck);


	static BOOL IsThemeEnabled();
	static LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR app, LPCWSTR idlist);


	static BOOL IsDirectoryOrFileExists(LPCWSTR wszPath);

	static BOOL IsPluginHadBeenDisabled(LPCWSTR pluginName);

	/*
	param:
			wszDirectoryPath: pattern = x:\xxx\aaa   or pattern = x:\xxx\aaa\
	return -1: not a valid directory
			0: valid directory but not contains pdf file
			1:valid directory and contains pdf file
	*/
	static int IsDirectoryContainsPDFFile(LPCWSTR wszDirectoryPath, BOOL bSubDirectoryIncluding);
	static int IsFileExists(LPCWSTR wszFilePath);
	static void GetDocFileName(FR_Document frDoc, FS_WideString *pFileName);
	static void GetFileName(const LPCWSTR pFilePath, FS_WideString *pFileName);


	/*
	param:
		strDirectory: the unofficial templates directory
	return:
		TRUE: success  FALSE: failed
	remark:
		......
	*/
	static BOOL GetUnofficialTemplatesDirectory(CString& strDirectory);
	static CString GetRMSExtensionConfigInfoFilePath();


	static BOOL SendEmail(LPCWSTR wszEmailAddress);



	static BOOL UTCSysTime2LocalSysTime(LPSYSTEMTIME pstUTC, LPSYSTEMTIME pstLocal);


	static BOOL GetGuidString(FS_ByteString bsGuid);

	static BOOL CenterWindowAccordingParentWnd(HWND hParentWnd, HWND hChildWnd, int nAdjustTopPos = 0);


	// for load FS_DIBitmap capability
	typedef struct _png_clientdata_
	{
		BYTE* pData;
		DWORD dwSize;
	}PNGCLIENTDATA;

	static FS_BOOL OnFileReadBlock(FS_LPVOID clientData, void* buffer, FS_DWORD offset, FS_DWORD size);
	static FS_DWORD OnFileGetSize(FS_LPVOID clientData);
	static void OnFileRelease(FS_LPVOID clientData);
	static FS_FileRead fsFileReader;
	static FS_DIBitmap LoadFSDIBitmapFromPNG(UINT uID);



#ifdef FXNET//#17101-FXNET-HCM 领鲜版区分产品
	enum HostAppType { emFOXIT_READER, emFOXIT_PHANTOMPDF, emFOXIT_READER_PLUS };
#else
	enum HostAppType {emFOXIT_READER, emFOXIT_PHANTOMPDF};
#endif
	static HostAppType GetHostAppType();

	static void GetGPOSubKey(FS_WideString wszGPOSubKey);

	static void GetLogOrPrinterLimitSubKey(CString& wsSubKey, BOOL bPrinter = FALSE);

	static void Trim(CString& name)
	{
		name.TrimLeft();
		name.TrimRight();
	}
	//parse input string
	static void ParseInput(CString& csIuput, std::list<CString>& lstNames);
	//resolve input string against Address Book locally or on exchange server.
//	static inline bool ResolveName(Outlook::_NameSpacePtr spNS, CString& strInput );

	typedef struct _NameResolve_Result
	{
		//input to resolve or resolved
		CString csName;
		//resolved or not resolved
		bool bResolved;
	}NAMERESOLVE_RESULT;
	typedef std::vector<NAMERESOLVE_RESULT> V_RESOLVE_RESULTS, *PV_RESOLVE_RESULTS;

// 	static bool ResolveAllNames(Outlook::_ApplicationPtr spOutlookApp, 
// 		std::list<CString>& lstNames, V_RESOLVE_RESULTS& resolveResults, HWND parentHwnd = NULL);

	static SRWLOCK s_srwlock;
	static CONDITION_VARIABLE s_cvReadyToPrint;
	//static CONDITION_VARIABLE s_cvStopResolving;
	static V_RESOLVE_RESULTS s_nameResolveResults;
	static LONG s_LResolutionFailed;	//0: Resolution succeed; 1:Resolution failed

	static void MaintainScrollBarVisible(HWND hWnd);
	static HWND GetWindowHandleByPID(DWORD dwProcessID);

	static void GetANSIBufferFromUnicode(char** pDes, const CString strSrc);

	static void GetLocaleIPAddress(CString& strIP);
	
	static BOOL IsWestEURLanguage();
	/*a. bInput = TRUE: 不区分语言,将字串中的 ’ , ’ 或 ’ . ’ 统一成 ’ . ’。
	  b. bInput = FALSE: 根据语言, 将字串中的 ’ , ’ 或 ’ . ’ 统一成 西欧语言‘ , ’ / 非西欧语言’ . ’。
	  */
	static void GetFormatSymbolString(LPWSTR str, BOOL bInput);

	//0066402: r167369,OCR/Bates Numbering操作，弹出RMS相应验证提示后直接关闭选择文件对话框及主程序，会停止工作
	static HWND GetParentWnd();

	static CString GetFileVersion(const CString &strFilePath);
	static CString GenerateTempWrapperFilePath();
};

#endif

