#include "stdafx.h"
#include "methodsImpl.h"

void CMethodsDefine::Function1()
{
    FRSysShowMessageBox3(L"Function1 is invoked.", FR_MsgBtnType::FR_MBOK, FR_MsgType::FR_MBICONINFORMATION, L"", NULL, FRAppGetMainFrameWnd());
}

void CMethodsDefine::Function2(FS_INT32 iNum)
{
    FRSysShowMessageBox3(L"Function2 is invoked.", FR_MsgBtnType::FR_MBOK, FR_MsgType::FR_MBICONINFORMATION, L"", NULL, FRAppGetMainFrameWnd());
}
