#ifndef BOOKMARKPANELVIEW_H
#define BOOKMARKPANELVIEW_H

#include "StdAfx.h"
#include "NavPage.h"

class CBookmarkPanelView
{
public:
    CBookmarkPanelView(FPD_Document pPDFDoc);

public:
    FS_HWND                 GetHwnd();
    void                    OnPanelActive();

    void                    Init(FPD_Document pDoc,FS_HWND hParent);
    FS_BOOL                 Create(FS_HWND hParent);

    FPD_Document            GetPDFDoc();
    FS_HWND                 GetParentWnd();
    CNavPage*               GetBookmarkPanel(){return m_pBookmarkPanel;}
    void                    StParentWnd(FS_HWND hParent){m_hOwner = hParent;}
    FS_BOOL                 GetIsFirstActive(){return m_bFirstActive;}

protected:
    CNavPage*                   m_pBookmarkPanel;
    FPD_Document				m_pPDFDoc;
    FS_HWND						m_hOwner;
    FS_BOOL						m_bFirstActive;
};

#endif // BOOKMARKPANELVIEW_H
