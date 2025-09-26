#ifndef FOXITCRYPTOCALLBACKS_H
#define FOXITCRYPTOCALLBACKS_H
#include "../SecurityPolicy.h"

struct FoxitCryptoCallbacks{
	static FS_LPVOID FREnryptoCreateHandler(FS_LPVOID clientData, FPD_Object encryptDict, FS_LPVOID securityData);
	static FS_DWORD FREnryptoDecryptGetSize(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD src_size);
	static FS_LPVOID FREnryptoDecryptStart(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD objnum,  FS_DWORD gennum);
	static FS_BOOL FREnryptoDecryptStream(FS_LPVOID clientData, FS_LPVOID cryptoContext, FS_LPCBYTE src_buf, FS_DWORD src_size, FS_BinaryBuf dest_buf);
	static FS_BOOL FREnryptoDecryptFinish(FS_LPVOID clientData, FS_LPVOID cryptoContext, FS_BinaryBuf dest_buf);
	
	static FS_DWORD FREnryptoEncryptGetSize(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD objnum, FS_DWORD version, FS_LPCBYTE src_buf, FS_DWORD src_size);
	static FS_BOOL FREnryptoEncryptContent(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_INT32 objnum, unsigned long version, FS_LPCBYTE src_buf, 
											FS_DWORD src_size, FS_LPBYTE dest_buf, FS_DWORD* outDestsize);
	static void FREnryptoFinishHandler(FS_LPVOID clientData, FS_LPVOID cryptoHandler);
	static FS_BOOL FREnryptoProgressiveEncryptStart(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_DWORD objnum, unsigned long version, FS_DWORD raw_size, FS_BOOL bFlateEncode);
	static FS_BOOL FREnryptoProgressiveEncryptContent(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_INT32 objnum, 
		unsigned long version, FS_LPCBYTE src_buf, FS_DWORD src_size, FS_BinaryBuf dest_buf);
	static FS_BOOL FREnryptoProgressiveEncryptFinish(FS_LPVOID clientData, FS_LPVOID cryptoHandler, FS_BinaryBuf dest_buf);

};

#endif