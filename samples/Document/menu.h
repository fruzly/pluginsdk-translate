#ifndef _DOCUMENT_MENU_H_
#define _DOCUMENT_MENU_H_

#define DOCUMENT_CATEGORY_NAME "DocumentDemo"
#define DOCUMENT_CATEGORY_TITLE L"DocumentDemo"
#define DOCUMENT_PANEL_NAME "DocumentDemo"
#define DOCUMENT_PANEL_TITLE L"DocumentDemo"

#define DOCUMENT_BUTTON_NAME_NEXTPAGE "Next Page"
#define DOCUMENT_BUTTON_TITLE_NEXTPAGE L"Next Page"
#define DOCUMENT_BUTTON_TOOLTIP_NEXTPAGE L"Next Page"

#define DOCUMENT_BUTTON_NAME_PREVIOUSPAGE "Previous Page"
#define DOCUMENT_BUTTON_TITLE_PREVIOUSPAGE L"Previous Page"
#define DOCUMENT_BUTTON_TOOLTIP_PREVIOUSPAGE L"Previous Page"

#define DOCUMENT_BUTTON_NAME_EXTERNALWINDOW "External Window"
#define DOCUMENT_BUTTON_TITLE_EXTERNALWINDOW L"External Window"
#define DOCUMENT_BUTTON_TOOLTIP_EXTERNALWINDOW L"External Window"

#define DOCUMENT_BUTTON_NAME_OPENDOCUMENT "Open Document"
#define DOCUMENT_BUTTON_TITLE_OPENDOCUMENT L"Open Document"
#define DOCUMENT_BUTTON_TOOLTIP_OPENDOCUMENT L"Open Document"

#define DOCUMENT_BUTTON_NAME_CLOSEDOCUMENT "Close Document"
#define DOCUMENT_BUTTON_TITLE_CLOSEDOCUMENT L"Close Document"
#define DOCUMENT_BUTTON_TOOLTIP_CLOSEDOCUMENT L"Close Document"

#define DOCUMENT_BUTTON_NAME_SAVEDOCUMENT "Save Document"
#define DOCUMENT_BUTTON_TITLE_SAVEDOCUMENT L"Save Document"
#define DOCUMENT_BUTTON_TOOLTIP_SAVEDOCUMENT L"Save Document"

#define DOCUMENT_BUTTON_NAME_PRINTDOCUMENT "Print Document"
#define DOCUMENT_BUTTON_TITLE_PRINTDOCUMENT L"Print Document"
#define DOCUMENT_BUTTON_TOOLTIP_PRINTDOCUMENT L"Print Document"

#define DOCUMENT_BUTTON_NAME_PERMISSIONDOCUMENT "Permission Document"
#define DOCUMENT_BUTTON_TITLE_PERMISSIONDOCUMENT L"Permission Document"
#define DOCUMENT_BUTTON_TOOLTIP_PERMISSIONDOCUMENT L"Permission Document"

#define DOCUMENT_BUTTON_NAME_REPLACEPAGES "Replace Pages"
#define DOCUMENT_BUTTON_TITLE_REPLACEPAGES L"Replace Pages"
#define DOCUMENT_BUTTON_TOOLTIP_REPLACEPAGES L"Replace Pages"

#define DOCUMENT_BUTTON_NAME_INSERTPAGES "Insert Pages"
#define DOCUMENT_BUTTON_TITLE_INSERTPAGES L"Insert Pages"
#define DOCUMENT_BUTTON_TOOLTIP_INSERTPAGES L"Insert Pages"

#define DOCUMENT_BUTTON_NAME_EXTRACTPAGES "Extract Pages"
#define DOCUMENT_BUTTON_TITLE_EXTRACTPAGES L"Extract Pages"
#define DOCUMENT_BUTTON_TOOLTIP_EXTRACTPAGES L"Extract Pages"

#define DOCUMENT_BUTTON_NAME_CONVERTDOCUMENT "Convert Document"
#define DOCUMENT_BUTTON_TITLE_CONVERTDOCUMENT L"Convert Document"
#define DOCUMENT_BUTTON_TOOLTIP_CONVERTDOCUMENT L"Convert Document"

#define DOCUMENT_BUTTON_NAME_CREATEFORMCONTROL "Create Form"
#define DOCUMENT_BUTTON_TITLE_CREATEFORMCONTROL L"Create Form"
#define DOCUMENT_BUTTON_TOOLTIP_CREATEFORMCONTROL L"Create Form"

#define DOCUMENT_BUTTON_NAME_DOCUMENT "Document"
#define DOCUMENT_BUTTON_TITLE_DOCUMENT L"Document"
#define DOCUMENT_BUTTON_TOOLTIP_DOCUMENT L"Document"

class CRibbonHelper
{
public:

	//ribbon
	static void OnButtonExecuteProc(void* clientDate);
	static FS_BOOL OnButtonMarkedProc(void* clientDate);
	static FS_BOOL OnButtonEnableProc(void* clientDate);
	static void OnFreeDataProc(void *clientData);

	void CreateRibbonButton(FR_RibbonPanel fr_Panel, const FS_CHAR* name, FS_LPCWSTR title, FS_LPCWSTR tooltip);
	void CreateButtonToNewCategory();
	void CreateConvertSubButton(FR_RibbonPanel fr_Panel);
	void CreateDocumentSubButton();
	void CreatePagesButton();
	static CString GetOpenDocumentPath();
	static CString CRibbonHelper::GetSaveAsFilePath(CString suggestFileName);

	static void OnSubButtonExecuteProc(void* clientDate);

	static void OpenDocument();

	static void GetSetPermission();
	static void ReplacePages();
	static void ExtractPages();
	static void InsertPages();
	static void CreateFormControl();

	static void OnSubDocButtonExecuteProc(void* clientDate);
};

#endif	