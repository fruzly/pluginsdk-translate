#ifndef FOXITSECURITYMETHODCALLBACKS_H
#define FOXITSECURITYMETHODCALLBACKS_H


typedef struct _securitymethod_clientdata_
{
	FS_WideString wsMethodTitle;
	FR_Document pReaderDoc;
}SECURITY_CLIENTDATA;


struct FoxitSecurityMethodCallbacks{
	static FS_LPWSTR FRSecurityMethodGetName(FS_LPVOID clientData);
	static FS_LPWSTR FRSecurityMethodGetTitle(FS_LPVOID clientData);
	static FS_BOOL FRSecurityMethodIsMyMethod(FS_LPVOID clientData, FR_Document frDoc);
	static FS_BOOL FRSecurityMethodCheckModuleLicense(FS_LPVOID clientData);
	static FS_BOOL FRSecurityMethodCanBeModified(FS_LPVOID clientData);
	static void FRSecurityMethodDoSetting(FS_LPVOID clientData, FS_HWND hParent, FS_BOOL* bSuc);
	static FS_BOOL FRSecurityMethodRemoveSecurityInfo(FS_LPVOID clientData);
	static FS_HWND FRSecurityMethodCreatePermSubDlg(FS_LPVOID clientData, FS_HWND hParent);
	static void FRSecurityMethodDestroyPermSubDlg(FS_LPVOID clientData, FS_HWND hWnd);
};

#endif