#ifndef FIPCSECURITYPOLICY_H
#define FIPCSECURITYPOLICY_H

using namespace std;


struct CloseDocInfo
{
	BOOL m_bPromptToSave;
	BOOL m_bDelay;
	DWORD m_dwErrCode;
	CloseDocInfo():
		m_bPromptToSave(TRUE),
		m_bDelay(TRUE),
		m_dwErrCode(0)
	{
	}
};

class CSecurityPolicy{
public:
	CSecurityPolicy(BOOL bFileEncrypt = FALSE);
	~CSecurityPolicy();
	
private:

	void InitialCryptoCallback();
	bool m_bMetadataEncrypted;

public:
	
	FR_CryptoCallbacks pfrCryptoCallbacks;
	FPD_Document fpdCurDoc; // hold the doc which contains this security policy

};

#endif