#include "StdAfx.h"
#include "bookmarktoolhander.h"
#include "resource.h"

FS_DIBitmap      CBookmarkToolHander::m_pPanelIcon = NULL;

std::vector<CBookmarkPanelView*> CBookmarkToolHander::m_PanelViewArray;

/*Get the icon from file.*/
FS_DIBitmap GetBmpFromRes(UINT uID)
{
    HINSTANCE hInstOld = AfxGetResourceHandle();
    AfxSetResourceHandle(AfxGetInstanceHandle());
    HRSRC hRes = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(uID), _T("pdf"));
    if (hRes == NULL) return NULL;
    DWORD dwLength = ::SizeofResource(AfxGetInstanceHandle(), hRes);
    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRes);
    if (hGlobal == NULL) return NULL;
    LPVOID pData = ::LockResource(hGlobal);
    if (pData == NULL) return NULL;
    AfxSetResourceHandle(hInstOld);

    //create bmp
    FS_DIBitmap pBmp = FSDIBitmapNew();
    FSDIBitmapCreate(pBmp, 24, 24, FS_DIB_Argb, NULL, 0);
    FSDIBitmapClear(pBmp, 0xffffff);
    FPD_RenderDevice pDC = FPDFxgeDeviceNew();
    FPDFxgeDeviceAttach(pDC, pBmp, 0);
    FPD_Document pPDFDoc = FPDDocOpenMemDocument(pData, dwLength, NULL);
    FPD_Object pDict = FPDDocGetPage(pPDFDoc, 0);
    FPD_Page pPage = FPDPageNew();
    FPDPageLoad(pPage, pPDFDoc, pDict, TRUE);
    FPDPageParseContent(pPage, NULL);
    FS_AffineMatrix matrix = FPDPageGetDisplayMatrix(pPage, 0, 0, 24, 24, 0);
    FPD_RenderContext pContext = FPDRenderContextNew(pPage, TRUE);
    FPDRenderContextAppendPage(pContext, pPage, matrix);
    FPDRenderContextRender(pContext, pDC, NULL, NULL);
    FPDRenderContextDestroy(pContext);
    FPDPageDestroy(pPage);
    FPDDocClose(pPDFDoc);
    FPDFxgeDeviceDestroy(pDC);

    return pBmp;
}

CBookmarkToolHander::CBookmarkToolHander()
{
}

CBookmarkToolHander::~CBookmarkToolHander()
{
    if(m_pPanelIcon)
        FSDIBitmapDestroy(m_pPanelIcon);
}

FS_LPSTR CBookmarkToolHander::Tool_GetName(FS_LPVOID clientData)
{
    return "Bookmark_TestName";
}

FS_LPCWSTR CBookmarkToolHander::Tool_GetTitle(FS_LPVOID clientData)
{
    return L"Bookmark_TestTitle";
}

void CBookmarkToolHander::Tool_InitNewView(FS_LPVOID clientData, FPD_Document pDocument, FS_HWND hParent)
{
    CBookmarkPanelView* pView = new CBookmarkPanelView(pDocument);
    if (pView)
    {
        pView->Init(pDocument, hParent);
        m_PanelViewArray.push_back(pView);
    }
    return;
}

void CBookmarkToolHander::Tool_CloseView(FS_LPVOID clientData, FS_HWND window, FPD_Document doc)
{
    int nCount = m_PanelViewArray.size();
    for (int i=0; i<nCount; i++)
    {
        CBookmarkPanelView* pPanelView = m_PanelViewArray.at(i);
        if(pPanelView->GetPDFDoc() == doc)
        {
            m_PanelViewArray.erase(m_PanelViewArray.begin() + i);
            return;
        }
    }
}

void CBookmarkToolHander::Tool_SetActiveView(FS_LPVOID clientData, FPD_Document doc, FS_HWND window)
{

}

void CBookmarkToolHander::Tool_GetButtonTip(FS_LPVOID clientData, FS_WideString *csOutTip)
{
    FSWideStringFill(*csOutTip, L"Bookmark_TestButtonTip");
}

FS_DIBitmap CBookmarkToolHander::Tool_PanelViewGetButtonIconUnion(FS_LPVOID clientData, FS_APPMODE mode, FS_BTNSTATE state, FS_ICONSIZE size)
{
    if (m_pPanelIcon == NULL)
    {
        m_pPanelIcon = GetBmpFromRes(IDR_ICON_BOOKMARK);
    }

    return m_pPanelIcon;
}

FS_HWND CBookmarkToolHander::Tool_PanelViewOnGetHwnd(FS_LPVOID clientData, FPD_Document doc, FS_HWND parent)
{
    CBookmarkPanelView* pView = Tool_GetView(doc, parent);
    if(pView)
      return  pView->GetHwnd();
    return NULL;
}

FS_HWND CBookmarkToolHander::Tool_PanelViewOnPanelActive(FS_LPVOID clientData, FPD_Document doc, FS_HWND parent)
{
    CBookmarkPanelView* pView = Tool_GetView(doc, parent);
    if(pView)
    {
       pView->OnPanelActive();
       return  pView->GetHwnd();
    }
    return NULL;
}

CBookmarkPanelView *CBookmarkToolHander::Tool_GetView(FPD_Document pDocument, FS_HWND hParent)
{
    CBookmarkPanelView* pView = NULL;
    int count = m_PanelViewArray.size();
    for(int i = 0; i < count; i ++)
    {
        CBookmarkPanelView* BookmarkPanelView = m_PanelViewArray.at(i);
        if( BookmarkPanelView->GetPDFDoc() == pDocument && BookmarkPanelView->GetParentWnd()  == hParent)
        {
            pView = BookmarkPanelView;
            break;
        }
    }
    return pView;
}

