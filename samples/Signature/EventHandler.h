#ifndef EventHandler_h__
#define EventHandler_h__


//用于处理FoxitSign签名域
class CFoxitSign3StandardHandler
{
public:
	CFoxitSign3StandardHandler();
	virtual ~CFoxitSign3StandardHandler() {}

	static const char* SignatureHandlerGetName(FS_LPVOID clientData);

	static FR_SG_HANDLE SignatureHandlerSignData(FS_LPVOID clientData, const unsigned char* pData2BSigned, unsigned long ulData2BSignedLen, unsigned char** pPSignedData, FS_DWORD* pulSignedDataLen);
	static FR_SG_HANDLE SignatureHandlerTimeStampDate(FS_LPVOID clientData, FR_SignatureTimestamp* pSgTimeStampDate, FPD_Object pSignDict);

	static FR_SG_HANDLE SignatureHandlerVerifyData(FS_LPVOID clientData, const unsigned char* pSignedData, FS_DWORD ulSignedDataLen,
		const unsigned char* pData2BSigned, unsigned long ulDataT2BSignedLen, FS_DWORD* outVerifyState, void** hCertContext);

	static FR_SG_HANDLE SignatureHandlerShowStateUI(FS_LPVOID clientData, const FS_DWORD nVerifyState, void* pWnd, void* hCertContext, FR_SignatureDictInfo* pSignDictInfo, FPD_Object pSignDict);

	static FR_SG_HANDLE SignatureHandlerCanClear(FS_LPVOID clientData, void* hCertContext, FS_BOOL* bCanClear, FPD_Object pSignDict);

	static FR_SG_HANDLE SignatureHandlerShowSignProperties(FS_LPVOID clientData, const unsigned char* pSignedData, unsigned long ulSignedDataLen,
		const unsigned char* pData2BSigned, unsigned long ulDataT2BSignedLen, void* pWnd, FR_SignatureDictInfo* pSignDictInfo, void* hCertContext, FPD_Object pSignDict);

	static FR_SG_HANDLE SignatureHandlerSGVerifyCanSign(FS_LPVOID clientData, FPD_Object pSignDict);
	static FR_SG_HANDLE SignatureHandlerSGVerifyCanVerify(FS_LPVOID clientData, FPD_Object pSignDict);

	static void SignatureHandlerRleaseSignData(FS_LPVOID clientData, unsigned char **pPSignedData, FS_DWORD *pulSignedDataLen);

};


#endif

