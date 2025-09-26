#pragma once
#include <string>
//#include "WebPluginManager.h"
enum EleActionType
{
	ExecuteFucntion = 0,
	ShowTaskpane = 1,
	DoModelDialog = 2
};

typedef struct htmlDialog_options_
{
	std::string sPluginId;
	std::wstring wsDialogTitle;
	std::string sUrl;
	int nSizeX;
	int nSizeY;

	htmlDialog_options_()
	{
		nSizeX = 300;
		nSizeY = 400;
		sUrl = "";
		wsDialogTitle = L"";
		sPluginId = "";
	}
}HtmlDialog_Options;

class CRibbonUIManager
{
public:
	CRibbonUIManager() {}
	virtual ~CRibbonUIManager() {}
	static CRibbonUIManager*		Create();
	
	//RibbonElement Action ClientData
	typedef struct ribbon_element_clientdata_
	{
		std::string sPluginId;
		std::string sControlId;
		std::wstring wsTitle;
		std::string sUrl;
		EleActionType nActionType;
		std::string sExecFuncName;
		FR_RibbonElement ribbonElement;
		bool bDefaultEnabled;
		int nSizeX;
		int nSizeY;

		ribbon_element_clientdata_()
		{
			sPluginId = "";
			sControlId = "";
			sExecFuncName = "";
			nActionType = ShowTaskpane;
			sUrl = "";
			ribbonElement = NULL;
			bDefaultEnabled = true;
			wsTitle = L"";
			nSizeX = nSizeY = 0;
		}
	}ElemActionData;


	// RibbonElement动态加载的图标信息
	struct RIBBON_IMAGE_INFO
	{
		std::wstring wsPNG16Path;
		std::wstring wsPNG32Path;
		RIBBON_IMAGE_INFO()
		{ 
			wsPNG16Path = L"";
			wsPNG16Path = L"";
		}
		RIBBON_IMAGE_INFO(std::wstring ws16Path, std::wstring  ws32Path)
			:wsPNG16Path(ws16Path), wsPNG32Path(ws32Path)
		{	}
	};

	typedef std::map<FR_RibbonElement, RIBBON_IMAGE_INFO> MAP_RIBELE_IMAGES;
	
	static MAP_RIBELE_IMAGES s_mapRibEleImages;
	
	//FS_BOOL AddRibbonButton(std::string pluginName, CWebPluginRibbonCustomTab* pTab, CWebPluginRibbonGroup* pGroup, CWebPluginControl* pControl);
	//void AddPanelView(std::string pluginName, std::wstring path, CWebPluginPanel* pPanel);

	static void CustomerEncryptExecProc(void* pData);
	static void CustomerDecryptExecProc(void* pData);
	static FS_BOOL CustomerEnabledProc(void* pData);

	static void StandardDecryptExecProc(void* pData);
	static void StandardEncryptExecProc(void* pData);
	

protected:
	static void RibbonElementImageInitProc(FR_RibbonElement ribbonElement, void* clientdata);
	static void RibbonExecuteProc(void *clientData);
	static FS_BOOL RibbonComputeEnabledProc(void *clientData);
	static void	RibbonFreeDataProc(void *clientData);
};
