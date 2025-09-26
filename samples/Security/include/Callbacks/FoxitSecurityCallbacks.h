#ifndef FOXITSECURITYCALLBACKS_H
#define FOXITSECURITYCALLBACKS_H

struct FoxitSecurityCallbacks{
	static FS_LPVOID FRSecurityOnInit(FS_LPVOID clientData, FS_LPCWSTR filePath, FPD_Object encryptDict, FPD_Document doc);
	static FS_BOOL FRSecurityIsProcessErrMsg(FS_LPVOID clientData);
	static FS_DWORD FRSecurityGetPermissions(FS_LPVOID clientData, FS_LPVOID securityData);
	static FS_BOOL FRSecurityIsOwner(FS_LPVOID clientData, FS_LPVOID securityData);
	static FS_BOOL FRSecurityGetCryptInfo(FS_LPVOID clientData, FS_LPVOID securityData, int* outCipher, FS_LPVOID* outBuffer, FS_INT32* outKeylen);
	static FS_BOOL FRSecurityIsMetadataEncrypted(FS_LPVOID clientData, FS_LPVOID securityData);
	static void FRSecurityFinishHandler(FS_LPVOID clientData, FS_LPVOID securityData);
	static FS_LPVOID FRSecurityCreateCryptoHandler(FS_LPVOID clientData, FS_LPVOID securityData, FS_INT32* outType);
};

#endif