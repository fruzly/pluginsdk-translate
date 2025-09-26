#include "StdAfx.h"

 #include "../../include/Callbacks/FoxitSecurityMethodCallbacks.h"

// for multilanguage reason, comment the follow line
const LPWSTR	wszSecurityMethodName = L"Foit Encryption";

FS_LPWSTR FoxitSecurityMethodCallbacks::FRSecurityMethodGetName(FS_LPVOID clientData)
{
	if(NULL == clientData)
		return NULL;
	return (FS_LPWSTR)wszSecurityMethodName;

}

FS_LPWSTR FoxitSecurityMethodCallbacks::FRSecurityMethodGetTitle(FS_LPVOID clientData)
{
	if(NULL == clientData)
		return NULL;
	SECURITY_CLIENTDATA* psecurityClientData = (SECURITY_CLIENTDATA*)clientData;
	return (FS_LPWSTR)FSWideStringCastToLPCWSTR(psecurityClientData->wsMethodTitle);
}

FS_BOOL FoxitSecurityMethodCallbacks::FRSecurityMethodIsMyMethod(FS_LPVOID clientData, FR_Document frDoc)
{
	return theApp.m_bsecurity;
}

FS_BOOL FoxitSecurityMethodCallbacks::FRSecurityMethodCheckModuleLicense(FS_LPVOID clientData)
{
	return TRUE;
}

FS_BOOL FoxitSecurityMethodCallbacks::FRSecurityMethodCanBeModified(FS_LPVOID clientData)
{
	
	FR_Document frDoc = FRAppGetActiveDocOfPDDoc();

	CFSecurityDocInfo* pSecurityDocInfo = NULL;
	if(S_FALSE == theApp.GetSecurityDocInfo(frDoc, &pSecurityDocInfo))
		return TRUE;

	if(FRDocIsInProtectedViewMode(frDoc))
	{
		return FALSE;
	}

	return FALSE;
}

void FoxitSecurityMethodCallbacks::FRSecurityMethodDoSetting(FS_LPVOID clientData, HWND hParent, FS_BOOL* bSuc)
{	
	FR_Document frDoc = NULL;
	frDoc = FRAppGetActiveDocOfPDDoc();
	if(NULL == frDoc)
		return;

}

FS_BOOL FoxitSecurityMethodCallbacks::FRSecurityMethodRemoveSecurityInfo(FS_LPVOID clientData)
{

	return TRUE;
}

HWND FoxitSecurityMethodCallbacks::FRSecurityMethodCreatePermSubDlg(FS_LPVOID clientData, HWND hParent)
{

	return NULL;

}

void FoxitSecurityMethodCallbacks::FRSecurityMethodDestroyPermSubDlg(FS_LPVOID clientData, HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SECURITY_CLIENTDATA* psecurityClientData = (SECURITY_CLIENTDATA*)clientData;

	
}