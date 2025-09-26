#include "stdafx.h"
#include "RibbonBar_Operation.h"
#include "resource.h"
#include "RibbonBar.h"
#include <atlbase.h>

extern CRibbonBarApp theApp;
/*Get the icon from file.*/
FS_DIBitmap GetBmpFromRes(UINT uID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return FSDIBitmapLoadFromPNGIcon3(theApp.GetInstanceHandle(), (FS_LPCWSTR)MAKEINTRESOURCE(uID));
}

FR_RibbonCategory GetRibbonCategory(FS_LPSTR nameCategory)
{
    //Get FR_RibbonBar handle
    FR_RibbonBar hRibbonBar = FRAppGetRibbonBar(NULL);

    //Get FR_RibbonCategory handle By name
    FR_RibbonCategory hRibbonCategory = FRRibbonBarGetCategoryByName(hRibbonBar,  nameCategory);
    return hRibbonCategory;
}

FR_RibbonPanel GetRibbonPanel(FR_RibbonCategory hRibbonCategory, FS_LPSTR namePanel)
{
    //Get FR_RibbonPanel handle By name
    FR_RibbonPanel hRibbonPanel = FRRibbonCategoryGetPanelByName(hRibbonCategory,  namePanel);
    return hRibbonPanel;
}

FR_CommonControl GetRibbonControl(FR_RibbonPanel hRibbonPanel, FS_LPSTR nameControl)
{
    //Get FR_CommonControl handle By name
    FR_CommonControl hCommonControl = FRRibbonPanelGetControlByName(hRibbonPanel,  nameControl);
    return hCommonControl;
}


//Output a line of debugging information when the bound button is clicked
void btnExcute(void *data)
{
	TRACE(L"button <RibbonButton> was clicked.\n");
}

//Set the bound button enable state through the check state of the RibbonCheckBox
FS_BOOL btnComputeEnabled(void *data)
{
    //Get FR_RibbonCategory handle By name
    FR_RibbonCategory hRibbonCategory = GetRibbonCategory("RibbonCategory");
    if(hRibbonCategory != NULL)
    {
        //Get FR_RibbonPanel handle By name
        FR_RibbonPanel hRibbonPanel = GetRibbonPanel(hRibbonCategory,  "RibbonPanel");
        if(hRibbonPanel != NULL)
        {
            //Get FR_CommonControl handle By name
            FR_CommonControl hCommonControl = GetRibbonControl(hRibbonPanel,  "RibbonCheckBox");
            if(hCommonControl != NULL)
            {
                //Get FR_CommonControl check status
                FS_BOOLEAN isEnbled = FRCommonControlIsChecked(hCommonControl);
                if(isEnbled)
                {
					TRACE(L"button <RibbonButton> current will be enable.\n");
                }
                else
                {
					TRACE(L"button <RibbonButton> current will be disable.\n");
                }
                return isEnbled;
            }
        }
    }

    return FALSE;
}

//Set the visibility of the RibbonButton according to the check state of the RibbonDropButton subitem TestDropButtonSubItem
void DropButtonItemExcute(void *data)
{
    //Get FR_RibbonCategory handle By name
    FR_RibbonCategory hRibbonCategory = GetRibbonCategory("RibbonCategory");
    if(hRibbonCategory != NULL)
    {
        //Get FR_RibbonPanel handle By name
        FR_RibbonPanel hRibbonPanel = GetRibbonPanel(hRibbonCategory,  "RibbonPanel");
        if(hRibbonPanel != NULL)
        {
            //Get FR_CommonControl handle By name
            FR_CommonControl hDropButtonControl = GetRibbonControl(hRibbonPanel,  "RibbonDropButton");
            if(hDropButtonControl != NULL)
            {
                //Get FR_CommonControl handle of sub item By name
                FR_CommonControl hSubItem = FRCommonControlGetMenuItemByName(hDropButtonControl, "TestDropButtonSubItem");
                if(hSubItem != NULL)
                {
                    //Get FR_CommonControl check status
                    FS_BOOLEAN isChecked = FRCommonControlIsChecked(hSubItem);
                    //Get FR_CommonControl handle By name
                    FR_CommonControl hButtonControl = GetRibbonControl(hRibbonPanel,  "RibbonButton");
                    if(hButtonControl != NULL)
                    {
                        //Set the visile of FR_CommonControl
                        FRCommonControlSetVisible(hButtonControl, !isChecked);
						//Update Ribbon category layout
						FRRibbonCategoryRecalcLayout(hRibbonCategory, TRUE);
                    }
                }
            }
        }
    }
}

//Output debugging information, display all category information in the Ribbonbar
void DropActionItem1Excute(void *data)
{
	USES_CONVERSION;
    //Get FR_RibbonBar handle
    FR_RibbonBar hRibbonBar = FRAppGetRibbonBar(NULL);

    //Get the number of categories of ribbonbar
    FS_INT32 iCategoryCount = FRRibbonBarGetCategoryCount(hRibbonBar);

    FS_ByteString ribbonCategoryName = FSByteStringNew();
    FR_RibbonCategory hRibbonCategory = NULL;
    for(FS_INT32 i=0; i<iCategoryCount; i++)
    {
        //Get FR_RibbonCategory handle By index
        hRibbonCategory = FRRibbonBarGetCategoryByIndex(hRibbonBar, i);
        if(hRibbonCategory !=  NULL)
        {
            //Get the name of the FR_RibbonCategory
            FRRibbonCategoryGetName(hRibbonCategory, &ribbonCategoryName);
			TRACE(L"RibbonCategory index<%d> object name<%s>\n", i, A2W(FSByteStringCastToLPCSTR(ribbonCategoryName)));
        }
    }
	FSByteStringDestroy(ribbonCategoryName);
}

//Output debugging information, display all Panel information under the first category currently in the Ribbonbar
void DropActionItem2Excute(void *data)
{
	USES_CONVERSION;

    //Get FR_RibbonBar handle
    FR_RibbonBar hRibbonBar = FRAppGetRibbonBar(NULL);

    //Get first FR_RibbonCategory handle
    FR_RibbonCategory hRibbonCategory = FRRibbonBarGetCategoryByIndex(hRibbonBar, 0);
    if(hRibbonCategory != NULL)
    {
        FS_ByteString ribbonCategoryName = FSByteStringNew();
        //Get the name of the FR_RibbonCategory
        FRRibbonCategoryGetName(hRibbonCategory, &ribbonCategoryName);
		TRACE(L"Show the RibbonPanel name from the RibbonCategory<%s>:\n", A2W(FSByteStringCastToLPCSTR(ribbonCategoryName)));
        FSByteStringDestroy(ribbonCategoryName);

        FR_RibbonPanel hRibbonPanel = NULL;
        FS_ByteString ribbonPanelName = FSByteStringNew();
        //Get the number of panels of the FR_RibbonCategory
        FS_INT32 iPanelCount = FRRibbonCategoryGetPanelCount(hRibbonCategory);
        for(FS_INT32 i=0; i<iPanelCount; i++)
        {
            //Get FR_RibbonPanel handle By index
            hRibbonPanel = FRRibbonCategoryGetPanelByIndex(hRibbonCategory, i);
            if(hRibbonPanel !=  NULL)
            {
                //Get the name of the FR_RibbonPanel
                FRRibbonPanelGetName(hRibbonPanel, &ribbonPanelName);
				TRACE(L"RibbonPanel index<%d> object name<%s>\n", i, A2W(FSByteStringCastToLPCSTR(ribbonPanelName)));
            }
        }
        FSByteStringDestroy(ribbonPanelName);
    }
}

void CheckboxExcute(void *data)
{

}

Ribbonbar_Operation::Ribbonbar_Operation()
{
    m_RibbonCategoryName = "RibbonCategory";
    m_RibbonPanelName = "RibbonPanel";
    m_ControlButtonName = "RibbonButton";
    m_ControlDropButtonName = "RibbonDropButton";
    m_ControlDropActionName = "RibbonDropAction";
    m_ControlCheckBoxName = "RibbonCheckBox";
}

void Ribbonbar_Operation::InitRibbonBar()
{
    //Get FR_RibbonBar handle
    FR_RibbonBar hRibbonBar = FRAppGetRibbonBar(NULL);

    //Add new FR_RibbonCategory
    FR_RibbonCategory hRibbonCategory  = AddNewRibbonCategory(hRibbonBar);

    //Add new FR_RibbonPanel
    FR_RibbonPanel hRibbonPanel = AddNewRibbonPanel(hRibbonCategory);

    //Add new RibbonButton
    AddNewRibbonButton(hRibbonPanel);

    //Add new RibbonDropButton
    AddNewRibbonDropButton(hRibbonPanel);

    //Add new RibbonDropAction
    AddNewRibbonDropAction(hRibbonPanel);

    //Add new RibbonCheckBox
    AddNewRibbonCheckBox(hRibbonPanel);
}

FR_RibbonCategory Ribbonbar_Operation::AddNewRibbonCategory(FR_RibbonBar hRibbonBar)
{
    //Check if there is a category with the same name, add it if it does not exist
    FR_RibbonCategory hRibbonCategory = GetRibbonCategory(m_RibbonCategoryName);
    if(hRibbonCategory == NULL)
    {
        hRibbonCategory = FRRibbonBarAddCategory(hRibbonBar,  m_RibbonCategoryName, L"TestCategory");
    }

    return hRibbonCategory;
}

FR_RibbonPanel Ribbonbar_Operation::AddNewRibbonPanel(FR_RibbonCategory hRibbonCategory)
{
    //Check if there is a panel with the same name, add it if it does not exist
    FR_RibbonPanel hRibbonPanel = GetRibbonPanel(hRibbonCategory,  m_RibbonPanelName);
    if(hRibbonPanel == NULL)
    {
         hRibbonPanel = FRRibbonCategoryAddPanel(hRibbonCategory,  m_RibbonPanelName, L"TestPanel", NULL);
    }

    return hRibbonPanel;
}

void Ribbonbar_Operation::AddNewRibbonButton(FR_RibbonPanel hRibbonPanel)
{
    //Check if there is a button with the same name, add it if it does not exist
    FR_CommonControl hCommonControl = GetRibbonControl(hRibbonPanel,  m_ControlButtonName);
    if(hCommonControl == NULL)
    {
        hCommonControl = FRRibbonPanelAddControl(hRibbonPanel,  FR_CommonControl_BUTTON, m_ControlButtonName, L"TestButton");
    }

	//create a FS_DIBitmap object
	FS_DIBitmap DIBitmap_Large = GetBmpFromRes(IDB_PNG_BUTTON_LARGE);
	FS_DIBitmap DIBitmap_Small = GetBmpFromRes(IDB_PNG_BUTTON_SMALL);
	//Set image of the FR_CommonControl
    FRCommonControlSetImage(hCommonControl, DIBitmap_Large, DIBitmap_Small);
    FSDIBitmapDestroy(DIBitmap_Large);
	FSDIBitmapDestroy(DIBitmap_Small);

    //Set the tooltip of the bound button
    FRCommonControlSetTooltip(hCommonControl, L"This is a ordinary button");
    //The callback function when the bound button is clicked
    FRCommonControlSetExecuteProc(hCommonControl, &btnExcute);
    //The callback function to set enable for the bound button
    FRCommonControlSetComputeEnabledProc(hCommonControl, &btnComputeEnabled);}

void Ribbonbar_Operation::AddNewRibbonDropButton(FR_RibbonPanel hRibbonPanel)
{
    //Check if there is a button with the same name, add it if it does not exist
    FR_CommonControl hCommonControl = FRRibbonPanelGetControlByName(hRibbonPanel,  m_ControlDropButtonName);
    if(hCommonControl == NULL)
    {
        hCommonControl = FRRibbonPanelAddControl(hRibbonPanel,  FR_CommonControl_DROPDOWNBUTTON, m_ControlDropButtonName, L"TestDropButton");
    }

	//create a FS_DIBitmap object
	FS_DIBitmap DIBitmap_Large = GetBmpFromRes(IDB_PNG_VISIBLE_LARGE);
	FS_DIBitmap DIBitmap_Small = GetBmpFromRes(IDB_PNG_VISIBLE_SMALL);
	//Set image of the FR_CommonControl
	FRCommonControlSetImage(hCommonControl, DIBitmap_Large, DIBitmap_Small);
	FSDIBitmapDestroy(DIBitmap_Large);
	FSDIBitmapDestroy(DIBitmap_Small);
	
    //Add sub item to the FR_CommonControl
    FR_CommonControl subControlItem = FRCommonControlAddMenuItem(hCommonControl, FR_CommonControl_CHECKBOX, "TestDropButtonSubItem", L"set <TestButton> visible", 0, FALSE, FALSE);
    //Initialize the check state of the current subitem
    FRCommonControlSetCheck(subControlItem, TRUE);
    //The callback function when the bound button is clicked
    FRCommonControlSetExecuteProc(subControlItem, &DropButtonItemExcute);
}

void Ribbonbar_Operation::AddNewRibbonDropAction(FR_RibbonPanel hRibbonPanel)
{
    //Check if there is a button with the same name, add it if it does not exist
    FR_CommonControl hCommonControl = FRRibbonPanelGetControlByName(hRibbonPanel,  m_ControlDropActionName);
    if(hCommonControl == NULL)
    {
        hCommonControl = FRRibbonPanelAddControl(hRibbonPanel,  FR_CommonControl_DROPDOWNACTION, m_ControlDropActionName, L"TestDropAction");
    }

    //create a FS_DIBitmap object
	FS_DIBitmap DIBitmap_Print_Large = GetBmpFromRes(IDB_PNG_PRINT_LARGE);
	FS_DIBitmap DIBitmap_Print_Small = GetBmpFromRes(IDB_PNG_PRINT_SMALL);
	
    //Set image of the FR_CommonControl
	FRCommonControlSetImage(hCommonControl, DIBitmap_Print_Large, DIBitmap_Print_Small);
	FSDIBitmapDestroy(DIBitmap_Print_Large);
	FSDIBitmapDestroy(DIBitmap_Print_Small);

	FS_DIBitmap DIBitmap_PrintInfo_Large = GetBmpFromRes(IDB_PNG_PRINTINFO_LARGE);
	FS_DIBitmap DIBitmap_PrintInfo_Small = GetBmpFromRes(IDB_PNG_PRINTINFO_SMALL);
    //Add sub item to the FR_CommonControl
    FR_CommonControl subDropActionItem1 = FRCommonControlAddMenuItem(hCommonControl, FR_CommonControl_BUTTON, "TestDropActionSubItem1", L"Print RibbonCategory Name", 0, FALSE, FALSE);

    //Set image of the sub item
    FRCommonControlSetImage(subDropActionItem1, DIBitmap_PrintInfo_Large, DIBitmap_PrintInfo_Small);

    //The callback function when the bound button is clicked
    FRCommonControlSetExecuteProc(subDropActionItem1, &DropActionItem1Excute);

    //Add sub item to the FR_CommonControl
    FR_CommonControl subDropActionItem2 = FRCommonControlAddMenuItem(hCommonControl, FR_CommonControl_BUTTON, "TestDropActionSubItem2", L"Print RibbonPanel Name", 1, FALSE, FALSE);

    //Set image of the sub item
    FRCommonControlSetImage(subDropActionItem2, DIBitmap_PrintInfo_Large, DIBitmap_PrintInfo_Small);

    //The callback function when the bound button is clicked
    FRCommonControlSetExecuteProc(subDropActionItem2, &DropActionItem2Excute);

    FSDIBitmapDestroy(DIBitmap_PrintInfo_Large);
	FSDIBitmapDestroy(DIBitmap_PrintInfo_Small);
}

void Ribbonbar_Operation::AddNewRibbonCheckBox(FR_RibbonPanel hRibbonPanel)
{
    //Check if there is a button with the same name, add it if it does not exist
    FR_CommonControl hCommonControl = FRRibbonPanelGetControlByName(hRibbonPanel,  m_ControlCheckBoxName);
    if(hCommonControl == NULL)
    {
        hCommonControl = FRRibbonPanelAddControl(hRibbonPanel,  FR_CommonControl_CHECKBOX, m_ControlCheckBoxName, L"TestCheckBox");
    }

    //The callback function when the bound button is clicked
    FRCommonControlSetExecuteProc(hCommonControl, &CheckboxExcute);
}




