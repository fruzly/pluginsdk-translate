#ifndef RIBBONBAR_OPERATION_H
#define RIBBONBAR_OPERATION_H

#include "stdafx.h"

class Ribbonbar_Operation 
{

public:
    Ribbonbar_Operation();

    void InitRibbonBar();
    FR_RibbonCategory AddNewRibbonCategory(FR_RibbonBar hRibbonBar);
    FR_RibbonPanel AddNewRibbonPanel(FR_RibbonCategory hRibbonCategory);
    void AddNewRibbonButton(FR_RibbonPanel hRibbonPanel);
    void AddNewRibbonDropButton(FR_RibbonPanel hRibbonPanel);
    void AddNewRibbonDropAction(FR_RibbonPanel hRibbonPanel);
    void AddNewRibbonCheckBox(FR_RibbonPanel hRibbonPanel);

private:
    FS_LPSTR m_RibbonCategoryName;
    FS_LPSTR m_RibbonPanelName;
    FS_LPSTR m_ControlButtonName;
    FS_LPSTR m_ControlDropButtonName;
    FS_LPSTR m_ControlDropActionName;
    FS_LPSTR m_ControlCheckBoxName;
};

#endif // RIBBONBAR_OPERATION_H
