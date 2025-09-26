#include "StdAfx.h"
#include "Tool.h"
#include <vector>
#include "CustomTool.h"

using namespace std;


void FinishDrawing(FS_LPVOID clientData)
{
	CCustomToolApp* pApp = (CCustomToolApp*)clientData;
	assert(pApp != NULL);
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	FPD_Document fpdDocument = FRDocGetPDDoc(frDocument);
	FR_DocView frDocView = FRDocGetCurrentDocView(frDocument);
	FR_PageView frPageView = FRDocViewGetCurrentPageView(frDocView);
	FPD_Page pPDFPage = FRPageViewGetPDPage(frPageView);
	FPD_Object fpdObject = FPDDictionaryNew();
	FS_ByteString strtype = FSByteStringNew3("Annot", -1);
	FPD_Object fpdfstringAnnot = FPDStringNew(strtype, 0);
	FPDDictionaryAddValue(fpdObject, "Type", fpdfstringAnnot);
	FS_ByteString strSubtype = FSByteStringNew3("Line", -1);
	FPD_Object fpdfstringSubtype = FPDStringNew(strSubtype, 0);
	FPDDictionaryAddValue(fpdObject, "Subtype", fpdfstringSubtype);
	FPDDictionaryAddValue(fpdObject, "Subj", FPDStringNew(FSByteStringNew3("Line", -1), 0));

	FS_FloatPoint fx;
	FS_FloatPoint fy;
	FRPageViewDevicePointToPage(pApp->m_page, pApp->m_begin.x, pApp->m_begin.y, &fx);
	FRPageViewDevicePointToPage(pApp->m_page, pApp->m_end.x, pApp->m_end.y, &fy);

	FPD_Object line = FPDArrayNew(); 

 	FPDArrayAddNumber(line, fx.m_PointX); 
	FPDArrayAddNumber(line, fx.m_PointY);
	FPDArrayAddNumber(line, fy.m_PointX);
	FPDArrayAddNumber(line, fy.m_PointY);
 	FPDDictionaryAddValue(fpdObject, "L", line);

	FPD_Object quadColor = FPDArrayNew();
    FS_COLORREF color;

    color = pApp->m_map[pApp->m_keyinfo];
	FPDArrayAddNumber(quadColor, (FS_FLOAT)FSARGB_R(color) / 255.0f); //1 means RGB Color
	FPDArrayAddNumber(quadColor, (FS_FLOAT)FSARGB_G(color) / 255.0f); //Defines the color
	FPDArrayAddNumber(quadColor, (FS_FLOAT)FSARGB_B(color) / 255.0f); //Defines the color
	FPDDictionaryAddValue(fpdObject, "C", quadColor);

	FPD_Object pPageDic = FPDPageGetDict(pPDFPage);
	FPDDictionarySetAt(fpdObject, "P", pPageDic, fpdDocument);
	
	FR_Annot frAnnot = FRPageViewAddAnnot(frPageView, fpdObject, 0); //Adds the highlight annotation

	FR_MarkupPanel panel = FRMarkupPanelGetMarkupPanel();
	FRMarkupPanelAddAnnot(panel, frAnnot, TRUE, TRUE);
	FRDocReloadPage(frDocument, 0, FALSE);

}
FS_BOOL CustomToolOnInit(FS_LPVOID clientData)
{
	return TRUE;
}

FS_BOOL CustomToolDestroy(FS_LPVOID clientData)
{
	return TRUE;
}


void CustomToolOnActivate(FS_LPVOID clientData, FS_BOOL bPersistent)
{
}

void CustomToolOnDeactivate(FS_LPVOID clientData)
{
}


FS_BOOL CustomToolOnKeyDown(FS_LPVOID clientData, FS_UINT nKeyCode, FS_UINT nFlags)
{
	CCustomToolApp* pApp = (CCustomToolApp*)clientData;
    assert(pApp != NULL);
	return TRUE;
}

FS_BOOL CustomToolOnKeyUp(FS_LPVOID clientData, FS_UINT nKeyCode, FS_UINT nFlags)
{
	return FALSE;
}

FS_BOOL CustomToolOnChar(FS_LPVOID clientData, FS_UINT nChar, FS_UINT nFlags)
{
    CCustomToolApp* pApp = (CCustomToolApp*)clientData;
    assert(pApp != NULL);
	return TRUE;
}

void CustomToolOnLeavePage(FS_LPVOID clientData, FR_PageView pageview)
{
    CCustomToolApp* pApp = (CCustomToolApp*)clientData;
}

FS_BOOL CustomToolIsEnabled(FS_LPVOID clientData)
{
	FR_Document frDocument = FRAppGetActiveDocOfPDDoc();
	if (frDocument == NULL)
	{
		return TRUE;
	}

	if (FRDocGetDocumentType(frDocument) == FR_DOCTYPE_DYNIMIC_XFA)
		return FALSE;
	
	if (!FRDocGetMergedPermissions(frDocument, FR_PERM_MODIFY_CONTENT))
		return FALSE;

	return TRUE;
}

FS_BOOL CustomToolOnLButtonDown(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{

	CCustomToolApp* pApp = (CCustomToolApp*)clientData;
	int x = MK_LBUTTON | MK_CONTROL;
	if (nFlags & MK_CONTROL)
	{
		pApp->m_keyinfo = CCustomToolApp::CTRL;
	}
	else if (nFlags & MK_SHIFT)
	{
		pApp->m_keyinfo = CCustomToolApp::SHIFT;
	}
	assert(pApp != NULL);
	pApp->m_begin = point;
	pApp->m_page = pageview;
	pApp->m_bDraw = true;

	return TRUE;
}

FS_BOOL CustomToolOnLButtonUp(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{
	CCustomToolApp* pApp = (CCustomToolApp*)clientData;
	assert(pApp != NULL);
	pApp->m_bDraw = false;
	if (pApp->m_begin.x == pApp->m_end.x &&pApp->m_end.y == pApp->m_begin.y)
	{
		return FALSE;
	}
	pApp->m_end = point;
	FinishDrawing(clientData);
	pApp->m_begin = point;
    pApp->m_keyinfo = CCustomToolApp::DEFAULT;
	return TRUE;
}

FS_BOOL CustomToolOnLButtonDblClk(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{
    CCustomToolApp* pApp = (CCustomToolApp*)clientData;
    assert(pApp != NULL);
	return TRUE;
}


FS_BOOL CustomToolOnMouseMove(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{
	CCustomToolApp* pApp = (CCustomToolApp*)clientData;
	assert(pApp != NULL);
	pApp->m_end = point;
	if (pApp->m_bDraw)
	{
		FR_Document pDocument = FRPageViewGetDocument(pageview);

		if (FR_DocView pDocView = FRDocGetDocView(pDocument, 0))
		{
			FRDocViewDrawNow(pDocView);
		}
	}

	return TRUE;
}

FS_BOOL CustomToolOnRButtonDown(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{
    return FALSE;

}

FS_BOOL CustomToolOnRButtonUp(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{
    CCustomToolApp* pApp = (CCustomToolApp*)clientData;
    assert(pApp != NULL);

    return FALSE;
}

FS_BOOL CustomToolOnRButtonDblClk(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point)
{
	return FALSE;
}

FS_BOOL CustomToolOnMouseWheel(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_SHORT zDelta, FS_DevicePoint point)
{
	return FALSE;
}

FS_BOOL CustomToolOnDraw(FS_LPVOID clientData, FR_WinPort winPort, FS_DWORD dwFlags)
{
	CCustomToolApp* pApp = (CCustomToolApp*)clientData;
	assert(pApp != NULL);
	if (pApp->m_bDraw)
	{
		FS_FLOAT scale = FRPageViewGetPageScale(pApp->m_page);
		
		FPD_RenderDevice pRD = FPDWindowsDeviceNew(winPort->hDC);
		assert(pRD != NULL);
		pApp->m_pPathFill = FPDPathNew();
		FPDPathSetPointCount(pApp->m_pPathFill, 2);
 		FPDPathSetPoint(pApp->m_pPathFill, 0, pApp->m_begin.x, pApp->m_begin.y, FPDPT_MOVETO);
 		FPDPathSetPoint(pApp->m_pPathFill, 1, pApp->m_end.x, pApp->m_end.y, FPDPT_LINETO);

		if (FPDPathGetPointCount(pApp->m_pPathFill) > 0)
		{
			FS_AffineMatrix mt;
			mt.a = 1;
			mt.b = 0;
			mt.c = 0;
			mt.d = 1;
			mt.e = 0;
			mt.f = 0;
			FPD_GraphState pGSD = FPDGraphStateNew();
			FPDGraphStateSetLineWidth(pGSD, 1);
            FS_ARGB color = pApp->m_map[pApp->m_keyinfo];
			FPDRenderDeviceDrawPath(pRD, pApp->m_pPathFill,
				mt, pGSD, color, 0, WINDING, FALSE, 0, NULL);
			FPDGraphStateDestroy(pGSD);
		}
	}
	return TRUE;
}

FS_BOOL	CustomToolIsProcessing(FS_LPVOID clientData)
{
    return FALSE;
}

FS_BOOL	CustomToolOnMouseHover(FS_LPVOID clientData, FR_PageView pageview, FS_DevicePoint point)
{
	return FALSE;
}

FS_BOOL	CustomToolIsWndCapturing(FS_LPVOID clientData)
{
	return FALSE;
}
