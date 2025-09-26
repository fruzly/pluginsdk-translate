#include "StdAfx.h"
#include "../../include/SecurityPolicy.h"
#include "../../include/FSecurityDocInfo.h"
#include "../../include/callbacks/FoxitSecurityCallbacks.h"
#include "../../include/FDocInfo.h"
#include "../../include/CustomStringPtr.h"


FS_LPVOID FoxitSecurityCallbacks::FRSecurityOnInit(FS_LPVOID clientData, FS_LPCWSTR filePath, FPD_Object encryptDict, FPD_Document fpdDoc)
{
	
 	FS_LPVOID pvoidResult = NULL;
  	CSecurityPolicy* pFIPCSecurityPolicy = NULL;
  	pFIPCSecurityPolicy = new CSecurityPolicy;
  	pvoidResult = pFIPCSecurityPolicy;

	return pvoidResult;
}

FS_BOOL FoxitSecurityCallbacks::FRSecurityIsProcessErrMsg(FS_LPVOID clientData)
{
	FS_BOOL bResult = TRUE;
	return bResult;
}

FS_DWORD FoxitSecurityCallbacks::FRSecurityGetPermissions(FS_LPVOID clientData, FS_LPVOID securityData)
{
	return 0;

}

FS_BOOL FoxitSecurityCallbacks::FRSecurityIsOwner(FS_LPVOID clientData, FS_LPVOID securityData)
{
	return TRUE;
}

FS_BOOL FoxitSecurityCallbacks::FRSecurityGetCryptInfo(FS_LPVOID clientData, FS_LPVOID securityData, int* outCipher, FS_LPVOID* outBuffer, FS_INT32* outKeylen)
{
	FS_BOOL bResult = TRUE;
	return bResult;
}

FS_BOOL FoxitSecurityCallbacks::FRSecurityIsMetadataEncrypted(FS_LPVOID clientData, FS_LPVOID securityData)
{
	FS_BOOL bResult = TRUE;

	return FALSE;
	return bResult;
}

void FoxitSecurityCallbacks::FRSecurityFinishHandler(FS_LPVOID clientData, FS_LPVOID securityData)
{
}

FS_LPVOID FoxitSecurityCallbacks::FRSecurityCreateCryptoHandler(FS_LPVOID clientData, FS_LPVOID securityData, FS_INT32* outType)
{
	FS_LPVOID pvoidResult = NULL;

	*outType = 0;
	if(NULL != securityData)
	{
		CSecurityPolicy* pSecurityPolicy = (CSecurityPolicy*)securityData;
		pvoidResult = pSecurityPolicy->pfrCryptoCallbacks;
	}

	return pvoidResult;
}