#include "StdAfx.h"
#include "../../include/SecurityPolicy.h"
#include "../../include/Callbacks/FoxitCryptoCallbacks.h"
#include <stack>

using namespace std;

FS_LPVOID FoxitCryptoCallbacks::FREnryptoCreateHandler(FS_LPVOID clientData, FPD_Object encryptDict, FS_LPVOID securityData)
{
	return securityData;
}

FS_DWORD FoxitCryptoCallbacks::FREnryptoDecryptGetSize(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD src_size)
{

	return 0;
}

FS_LPVOID FoxitCryptoCallbacks::FREnryptoDecryptStart(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD objnum,  FS_DWORD gennum)
{
	return cryptoHandler;
}



FS_BOOL FoxitCryptoCallbacks::FREnryptoDecryptStream(FS_LPVOID clientData, FS_LPVOID cryptoContext, FS_LPCBYTE src_buf, FS_DWORD src_size, FS_BinaryBuf dest_buf)
{
	if(NULL == src_buf || src_size <= 0)
		return FALSE;

	FS_BOOL bResult = FALSE;
	CSecurityPolicy* pSecurityPolicy = (CSecurityPolicy*)cryptoContext;

	FS_DWORD i = 0;
	while(i< src_size)
	{
	//	dec_data[i] = ;
		FSBinaryBufAppendByte(dest_buf, ((FS_BYTE*)src_buf)[i] + 1);
		i++;
	}
	return TRUE;
}


FS_BOOL FoxitCryptoCallbacks::FREnryptoDecryptFinish(FS_LPVOID clientData, FS_LPVOID cryptoContext, FS_BinaryBuf dest_buf)
{
	CSecurityPolicy* pSecurityPolicy = (CSecurityPolicy*)cryptoContext;

	
 	FS_BOOL bResult = FALSE;

	return TRUE;
}

FS_DWORD FoxitCryptoCallbacks::FREnryptoEncryptGetSize(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD objnum, FS_DWORD version, FS_LPCBYTE src_buf, FS_DWORD src_size)
{

	FS_DWORD dwEncryptSize = src_size;

	return dwEncryptSize;
}

FS_BOOL FoxitCryptoCallbacks::FREnryptoEncryptContent(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_INT32 objnum, unsigned long version,
														FS_LPCBYTE src_buf, FS_DWORD src_size, FS_LPBYTE dest_buf, FS_DWORD* outDestsize)
{

 	if(0 == src_size || NULL == src_buf)
 	{
 		*outDestsize = 0;
 		return S_OK;
 	}

	FS_DWORD i = 0;
	while (i < src_size)
	{
		((FS_BYTE*)dest_buf)[i] = ((FS_BYTE*)src_buf)[i] - 1;
		i++;
	}
	*outDestsize = src_size;
	return TRUE;
 	FS_BOOL bResult = FALSE;

	return bResult;
}

FS_BOOL FoxitCryptoCallbacks::FREnryptoProgressiveEncryptStart(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD objnum, unsigned long version, FS_DWORD raw_size, FS_BOOL bFlateEncode)
{
	return FALSE;
}

FS_BOOL FoxitCryptoCallbacks::FREnryptoProgressiveEncryptContent(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_INT32 objnum, unsigned long version, FS_LPCBYTE src_buf, FS_DWORD src_size, FS_BinaryBuf dest_buf)
{
	if(0 == src_size || NULL == src_buf)
	{
		return TRUE;
	}

	return FALSE;
}

FS_BOOL FoxitCryptoCallbacks::FREnryptoProgressiveEncryptFinish(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_BinaryBuf dest_buf)
{
	return FALSE;
}

void FoxitCryptoCallbacks::FREnryptoFinishHandler(FS_LPVOID clientData, FS_LPVOID cryptoHandler)
{
}

