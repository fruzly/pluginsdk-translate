#include "StdAfx.h"
#include "../include/Callbacks/FoxitCryptoCallbacks.h"


CSecurityPolicy::CSecurityPolicy(BOOL bFileEncrypt) :
pfrCryptoCallbacks(NULL),
fpdCurDoc(NULL)
{
	if (!bFileEncrypt)
	{
		pfrCryptoCallbacks = new FR_CryptoCallbacksRec;
		InitialCryptoCallback();
	}
}

CSecurityPolicy::~CSecurityPolicy()
{
}

void CSecurityPolicy::InitialCryptoCallback()
{
	memset(pfrCryptoCallbacks, 0, sizeof(FR_CryptoCallbacksRec));
	pfrCryptoCallbacks->lStructSize = sizeof(FR_CryptoCallbacksRec);
	pfrCryptoCallbacks->FREnryptoCreateHandler = FoxitCryptoCallbacks::FREnryptoCreateHandler;
	pfrCryptoCallbacks->FREnryptoDecryptGetSize = FoxitCryptoCallbacks::FREnryptoDecryptGetSize;
	pfrCryptoCallbacks->FREnryptoDecryptStart = FoxitCryptoCallbacks::FREnryptoDecryptStart;
	pfrCryptoCallbacks->FREnryptoDecryptStream = FoxitCryptoCallbacks::FREnryptoDecryptStream;
	pfrCryptoCallbacks->FREnryptoDecryptFinish = FoxitCryptoCallbacks::FREnryptoDecryptFinish;
	pfrCryptoCallbacks->FREnryptoEncryptGetSize = FoxitCryptoCallbacks::FREnryptoEncryptGetSize;
	pfrCryptoCallbacks->FREnryptoEncryptContent = FoxitCryptoCallbacks::FREnryptoEncryptContent;
	pfrCryptoCallbacks->FREnryptoFinishHandler = FoxitCryptoCallbacks::FREnryptoFinishHandler;
	pfrCryptoCallbacks->FREnryptoProgressiveEncryptStart = FoxitCryptoCallbacks::FREnryptoProgressiveEncryptStart;
	pfrCryptoCallbacks->FREnryptoProgressiveEncryptContent = FoxitCryptoCallbacks::FREnryptoProgressiveEncryptContent;
	pfrCryptoCallbacks->FREnryptoProgressiveEncryptFinish = FoxitCryptoCallbacks::FREnryptoProgressiveEncryptFinish;
}
