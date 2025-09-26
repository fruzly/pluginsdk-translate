#include "StdAfx.h"
#include "bookmarkpanelview.h"

CBookmarkPanelView::CBookmarkPanelView(FPD_Document pPDFDoc)
{
    m_pPDFDoc = pPDFDoc;
    m_pBookmarkPanel = nullptr;
    m_hOwner = NULL;
    m_bFirstActive = FALSE;
}

FS_HWND CBookmarkPanelView::GetHwnd()
{
    if(nullptr == m_pBookmarkPanel)
    {
        this->Create(m_hOwner);
    }

    FS_BOOL bResult = IsWindow(m_pBookmarkPanel->GetSafeHwnd());
    if (!bResult)
    {
        return NULL;
    }
    return m_pBookmarkPanel->GetSafeHwnd();
}

void CBookmarkPanelView::OnPanelActive()
{
    if (FALSE == m_bFirstActive)
    {
        ASSERT(m_hOwner);
        if(nullptr == m_pBookmarkPanel)
        {
            this->Create(m_hOwner);
        }
        m_bFirstActive = TRUE;
    }
}

void CBookmarkPanelView::Init(FPD_Document pDoc, FS_HWND hParent)
{
    if(!pDoc)
        return;
    m_pPDFDoc = pDoc;
    m_hOwner = hParent;
}

FS_BOOL CBookmarkPanelView::Create(FS_HWND hParent)
{
    if(!m_pPDFDoc) return FALSE;
    int nCount = FRAppCountDocsOfPDDoc();
    FR_Document pTempDoc = NULL;
    for(int i = 0; i < nCount; i++)
    {
        pTempDoc = FRAppGetDocOfPDDoc(i);
        if(FRDocGetPDDoc(pTempDoc) == m_pPDFDoc)
        {
            break;
        }
    }
    if(!pTempDoc)
    {
        return FALSE;
    }

    m_pBookmarkPanel = new CNavPage(FRAppGetActiveDocOfPDDoc());
    if (!m_pBookmarkPanel->Create(NULL,
        L"Bookmark",
        FWS_ADDTOTITLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        CRect(0, 0, 500, 500), CWnd::FromHandle(hParent),
        ID_PAGE))
    {
        delete m_pBookmarkPanel;
        m_pBookmarkPanel = NULL;
        return FALSE;
    }


    m_hOwner = hParent;
    return TRUE;
}

FPD_Document CBookmarkPanelView::GetPDFDoc()
{
    return m_pPDFDoc;
}

FS_HWND CBookmarkPanelView::GetParentWnd()
{
    return m_hOwner;
}
