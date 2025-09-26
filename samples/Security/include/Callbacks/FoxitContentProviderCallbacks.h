#ifndef FOXITCONTENTPROVIDERCALLBACKS_H
#define FOXITCONTENTPROVIDERCALLBACKS_H

struct FoxitContentProviderCallbacks{

	static FS_BOOL FRConProviderOnFileOpen(FS_LPVOID clientData, FR_Document frDoc, FS_LPCWSTR lpwszSource, FS_BOOL bIsAttachment);
	static FS_DWORD FRConProviderOnGetPermissions(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD  pdfselfPermissions);
	static FS_BOOL FRConProviderOnGetContentSize(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD* pTotalSize);
	static FS_BOOL FRConProviderOnReadContent(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD pos, FS_LPBYTE pBuf, FS_DWORD size);
	static FS_BOOL FRConProviderOnWriteContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD size, FS_LPCWSTR lpSaveFilePath);
	static FS_BOOL FRConProviderOnBackFillContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD size, FS_LPCWSTR lpSaveFilePath);
	static FS_BOOL FRConProviderIsWriteContentProgressive(FS_LPVOID clientData, FR_Document doc, FS_LPCWSTR lpSaveFilePath);
	static FS_BOOL FRConProviderWriteContentProgressiveFinish(FS_LPVOID clientData, FR_Document doc, FS_LPCWSTR lpSaveFilePath, FS_BOOL bResult);
	static void FRConProviderOnFileClose(FS_LPVOID clientData, FR_Document frDoc);
	static FS_BOOL FRConProviderOnWriteAttachmentContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD size, FS_LPCWSTR lpAttchmntPath);
	static FS_BOOL FRConProviderOnGetAttachmentSize(FS_LPVOID clientData, FR_Document frDoc, FS_DWORD* pTotalSize, FS_LPCWSTR lpAttchmntPath);
	static FS_BOOL FRConProviderOnReadAttachmentContent(FS_LPVOID clientData, FR_Document frDoc, FS_LPBYTE pBuf, FS_DWORD dwSize, FS_LPCWSTR lpAttchmntPath);
	static FS_BOOL FRConProviderIsPageAvail(FS_LPVOID clientData, FR_Document frDoc, FS_INT32 iPage);
	static FS_BOOL FRConProviderGetPageSize(FS_LPVOID clientData, FR_Document frDoc, FS_INT32 iPage, FS_FLOAT* outWidth, FS_FLOAT* outHeight);
	static FS_BOOL FRConProviderDrawOwnerPage(FS_LPVOID clientData, FR_Document frDoc, FS_INT32 iPage, FS_DIBitmap bitmap, FS_INT32 start_x, FS_INT32 start_y, FS_INT32 width, FS_INT32 height);
	static FS_BOOL FRConProviderNeedReopenDoc(FS_LPVOID clientData, FR_Document frDoc);
	static FS_BOOL FRConProviderCanBeSaved(FS_LPVOID clientData, FR_Document frDoc);
	static FS_BOOL FRContentProviderIsProcessErrMsg(FS_LPVOID clientData);

};


#endif