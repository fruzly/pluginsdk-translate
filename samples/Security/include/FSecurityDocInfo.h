#ifndef FSECURITYDOCINFO_H
#define FSECURITYDOCINFO_H
#include <fstream>
class CFIPCSecurityPolicy;
class CRMSWatermarkDraw;
class CFSecurityDocInfo{
public:
	CFSecurityDocInfo(BOOL bFileEncryptPPDF = FALSE);
	~CFSecurityDocInfo();

	FR_Document frDoc;//for PPDF

	FS_WideString filePath;
	FPD_Document fpdDoc;
	FPD_Object objEncryptDict;
	CFIPCSecurityPolicy* pFIPCSecurityPolicy;
	BOOL bSecurityRemoved;

	//CFile m_fileForSave;
	std::ofstream m_fileForSave;
	BOOL m_bCreateFileForSave;

	BOOL m_bSaveAs;//只有PPDF时用


};

#endif