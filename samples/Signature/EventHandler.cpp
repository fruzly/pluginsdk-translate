

#include "stdafx.h"
#include "EventHandler.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
/*CFoxitSign3StandardHandler*/
CFoxitSign3StandardHandler::CFoxitSign3StandardHandler()
{

}

//这个目前没有用到，可以不实现
const char* CFoxitSign3StandardHandler::SignatureHandlerGetName(FS_LPVOID clientData)
{
	return "";
}

//如果需要自己实现签名，需要在这个回调事件中去处理
//这个回调会在保存过程中，签名插件会向注册的插件取签名摘要的内容，如果自己实现签名，需要把签名的结果通过pPSignedData返回
//pData2BSigned、ulData2BSignedLen 表示当前pdf文档的数据buffer和长度
//pPSignedData、pulSignedDataLen 表示需要返回的签名结果的数据buffer和长度
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerSignData(FS_LPVOID clientData, const unsigned char* pData2BSigned, unsigned long ulData2BSignedLen,
	unsigned char** pPSignedData, FS_DWORD* pulSignedDataLen)
{
	CString strName = theApp.subfilter;
	if (strName == "subFilterSignFail")
	{
		return FR_SIG_HANDLE_FAIL;
	}
	return FR_SIG_HANDLE_NONE;
	//如果不需要自己实现签名
	// return FR_SIG_HANDLE_NONE;
	//如果签名失败
	//return FR_SIG_HANDLE_FAIL;
	//如果签名成功
	//return FR_SIG_HANDLE_SUCCESS;
}

//获取时间戳
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerTimeStampDate(FS_LPVOID clientData, FR_SignatureTimestamp* pSgTimeStampDate, FPD_Object pSignDict)
{
	//如果不需要返回自己的时间戳
	return FR_SIG_HANDLE_NONE;
	// 如果生成时间戳失败
	// return FR_SIG_HANDLE_FAIL;
	//如果生成时间戳成功
	//return FR_SIG_HANDLE_SUCCESS;
}

//实现自己的验签，在打开文档时会进行验签
//pSignedData、ulSignedDataLen 当前签名的数据和长度
//pData2BSigned、ulDataT2BSignedLen pdf  当前签名文档数据buffer和长度
//outVerifyState 返回验证的状态, 如 FR_SIG_VERIFY_VALID、FR_SIG_VERIFY_INVALID
//hCertContext 返回CERT_CONTEXT 证书数据
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerVerifyData(FS_LPVOID clientData, const unsigned char* pSignedData, FS_DWORD ulSignedDataLen,
	const unsigned char* pData2BSigned, unsigned long ulDataT2BSignedLen, FS_DWORD* outVerifyState, void** hCertContext)
{

	//PCCERT_CONTEXT pCertContext = NULL;

	//CRYPT_VERIFY_MESSAGE_PARA paraVerifyMsg;
	//::ZeroMemory(&paraVerifyMsg, sizeof(paraVerifyMsg));
	//paraVerifyMsg.cbSize = sizeof(CRYPT_VERIFY_MESSAGE_PARA);
	//paraVerifyMsg.dwMsgAndCertEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
	//paraVerifyMsg.hCryptProv = NULL;
	//paraVerifyMsg.pfnGetSignerCertificate = NULL;
	//paraVerifyMsg.pvGetArg = NULL;
	//
	////对数据部分进行验证
	//if (::CryptVerifyDetachedMessageSignature(&paraVerifyMsg, 0, pSignedData, ulSignedDataLen, 1, &pData2BSigned, &ulDataT2BSignedLen, &pCertContext))
	//{
	//	*hCertContext = (void*)pCertContext;
	//}
	CString strName = theApp.subfilter;
	if (strName == "subFilterVerifyFail")
	{
		*outVerifyState = FR_SIG_VERIFY_INVALID;
		return FR_SIG_HANDLE_FAIL;
	}
	return FR_SIG_HANDLE_NONE;

	//*outVerifyState = FR_SIG_VERIFY_VALID | FR_SIG_VERIFY_ISSUER_VALID;
	//return FR_SIG_HANDLE_SUCCESS;
}

//editor中signature插件在verify 的时候 , 本插件是否需要显示自己的状态UI
//nVerifyState  验证的状态，如 FR_SIG_VERIFY_VALID、FR_SIG_VERIFY_INVALID
//pWnd 父窗口句柄
//hCertContext CERT_CONTEXT 证书数据
//pSignDictInfo 签名字典信息
//pSignDict  签名字典
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerShowStateUI(FS_LPVOID clientData, const FS_DWORD nVerifyState, void* pWnd, void* hCertContext,
	FR_SignatureDictInfo* pSignDictInfo, FPD_Object pSignDict)
{
	//不需要处理UI
	return FR_SIG_HANDLE_NONE;
}

//是否可以删除这个签名
//在执行删除签名操作时，会询问是否可以删除
//hCertContext CERT_CONTEXT 证书数据
//bCanClear 需要返回是否可以删除
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerCanClear(FS_LPVOID clientData, void* hCertContext, FS_BOOL* bCanClear, FPD_Object pSignDict)
{
	//不需要去判断是否可以删除这个签名
	return FR_SIG_HANDLE_NONE;
	/**bCanClear = TRUE;
	return FR_SIG_HANDLE_SUCCESS;*/
	//判断成功或者失败
	//return FR_SIG_HANDLE_FAIL | FR_SIG_HANDLE_SUCCESS;
}

//是否需要显示自己的签名属性
//pSignedData、ulSignedDataLen 签名摘要的数据和长度
//pData2BSigned、ulDataT2BSignedLen pdf文档数据buffer和长度
//pWnd 父窗口句柄
//pSignDictInfo 签名字典信息
//hCertContext CERT_CONTEXT 证书数据
//pSignDict  签名字典
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerShowSignProperties(FS_LPVOID clientData, const unsigned char* pSignedData,
	unsigned long ulSignedDataLen,	const unsigned char* pData2BSigned, unsigned long ulDataT2BSignedLen,
	void* pWnd, FR_SignatureDictInfo* pSignDictInfo, void* hCertContext, FPD_Object pSignDict)
{
	//不需要显示
	return FR_SIG_HANDLE_NONE;
	//处理成功或失败
	//return FR_SIG_HANDLE_FAIL | FR_SIG_HANDLE_SUCCESS;
}

//判断是否可以签名，判断当前这个签名字典项是否是由该插件创建的，如果是，表明是你这个插件想要创建签名，即返回 FR_SIG_HANDLE_SUCCESS
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerSGVerifyCanSign(FS_LPVOID clientData, FPD_Object pSignDict)
{
	FS_ByteString bsSubFilter = FSByteStringNew();
	FPDDictionaryGetString(pSignDict, "SubFilter", &bsSubFilter);
	theApp.subfilter = FSByteStringCastToLPCSTR(bsSubFilter);

	CString strName = theApp.subfilter;
	if (strName == "subFilterSignFail")
	{
		return FR_SIG_HANDLE_SUCCESS;
	}
	//不需要自己实现签名摘要
	return FR_SIG_HANDLE_NONE;
	//需要自己实现签名摘要
	//return FR_SIG_HANDLE_SUCCESS;
}

//判断本插件是否可以签证这个签名，该签名字典项是否由该插件创建的，如果是，应该返回FR_SIG_HANDLE_SUCCESS
FR_SG_HANDLE CFoxitSign3StandardHandler::SignatureHandlerSGVerifyCanVerify(FS_LPVOID clientData, FPD_Object pSignDict)
{
	//return FR_SIG_HANDLE_SUCCESS;
	FS_ByteString bsSubFilter = FSByteStringNew();
	FPDDictionaryGetString(pSignDict, "SubFilter", &bsSubFilter);
	theApp.subfilter = FSByteStringCastToLPCSTR(bsSubFilter);
	CString strName = theApp.subfilter;
	if (strName == "subFilterVerifyFail")
	{
		return FR_SIG_HANDLE_SUCCESS;
	}
	//不需要自己签证签名
	return FR_SIG_HANDLE_NONE;
	//需要自己验证签名
	//return FR_SIG_HANDLE_SUCCESS;
}

//在处理了SignatureHandlerSignData回调后，需要释放签名数据，因为这个数据当时是由本插件申请的内存
void CFoxitSign3StandardHandler::SignatureHandlerRleaseSignData(FS_LPVOID clientData, unsigned char **pPSignedData, FS_DWORD *pulSignedDataLen)
{
	if (nullptr == pPSignedData)
		return;
	delete[] * pPSignedData;
	*pPSignedData = nullptr;
}

