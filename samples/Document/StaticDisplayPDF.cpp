// StaticDisplayPDF.cpp : implementation file
//

#include "stdafx.h"
#include "StaticDisplayPDF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticDisplayPDF

CStaticDisplayPDF::CStaticDisplayPDF()
{
	m_nCurrentPage = 0;
	m_iRotate = 0;
	m_fScale = 1.0;
}

CStaticDisplayPDF::~CStaticDisplayPDF()
{
}


BEGIN_MESSAGE_MAP(CStaticDisplayPDF, CStatic)
	//{{AFX_MSG_MAP(CStaticDisplayPDF)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_PAINT()
		ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CStaticDisplayPDF::OnPaint()
{
	CPaintDC dc(this);	
	
	CRect rcDisplay;
	GetWindowRect(&rcDisplay);
	DisplayPDFDocument( dc.m_hDC, rcDisplay, m_nCurrentPage, m_iRotate, m_fScale );
	
}
BOOL CStaticDisplayPDF::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	this->GetClientRect(&rect);
	pDC->FillSolidRect(rect, RGB(255,255,255));	
	
	return CStatic::OnEraseBkgnd(pDC);
}

void CStaticDisplayPDF::SetCurrentPage( int nPageIndex )
{
	m_nCurrentPage = nPageIndex;
}

void CStaticDisplayPDF::NextPage()
{
	m_nCurrentPage++;
	this->Invalidate();
}

void CStaticDisplayPDF::PreviousPage()
{
	m_nCurrentPage--;
	this->Invalidate();
}

int CStaticDisplayPDF::GetCurrentPage()
{
	return m_nCurrentPage;
}

void CStaticDisplayPDF::ClockWiseRotate()
{
	m_iRotate = (m_iRotate + 45) % 180;
	this->Invalidate();
}

void CStaticDisplayPDF::CounterClockWiseRotate()
{
	if (m_iRotate == 0)
	{
		m_iRotate = 180;
	}
	m_iRotate = (m_iRotate - 45) % 180;
	this->Invalidate();
}

int CStaticDisplayPDF::GetRotate()
{
	return m_iRotate;
}

void CStaticDisplayPDF::ZoomIn()
{
	if (m_fScale <= 0.75)
	{
		m_fScale = m_fScale + 0.25;
		this->Invalidate();
	}
}

void CStaticDisplayPDF::ZoomOut()
{
	if (m_fScale > 0.25)
	{
		m_fScale = m_fScale - 0.25;
		this->Invalidate();
	}
}

float CStaticDisplayPDF::GetScale()
{
	return m_fScale;
}

BOOL CStaticDisplayPDF::DisplayPDFDocument( HDC hDC, CRect rcDisplay, int nCurrentPage, int nRotate, float fScale )
{
	if (m_pdfDoc == NULL)
	{
		return FALSE;
	}

	/*Create a new Windows device to render the PDF contents.*/
	FPD_RenderDevice device = NULL;
	device = FPDWindowsDeviceNew((void*)hDC);
	
	/*Definition used.*/
	FPD_Object dict = NULL;
	FPD_Page page = NULL;
	FPD_AnnotList annotList = NULL;
	FS_AffineMatrix aMatrix;
	FPD_RenderContext context = NULL;
	
	if (nCurrentPage >= 0 && nCurrentPage < FPDDocGetPageCount(m_pdfDoc))
	{
		/*Get the PDF page dictionary that is used to construct the PDF page object.*/
		dict = FPDDocGetPage(m_pdfDoc, nCurrentPage);
		page = FPDPageNew();
		FPDPageLoad(page, m_pdfDoc, dict, TRUE);	
		
		/*Parse all the contents of the PDF page.*/
		FPDPageParseContent(page, NULL);
		
		/*Build a matrix from PDF user space to targeted device space, according to metrics info
	     *(top-left position and widht-height size) provided in device space. 
		 *Set the rotation here. 0 means no ratation. 45 means rotating to left. 90 means rotating
		 *to the bottom.
		 */
		aMatrix = FPDPageGetDisplayMatrix(page, 0, 0, rcDisplay.Width(), rcDisplay.Height(), nRotate);	
		
		/*Set the scale value.*/
		aMatrix = FSAffineMatrixScale(aMatrix, fScale, fScale);

		/*Create a context for rendering a PDF page.*/
		context = FPDRenderContextNew(page, TRUE);

		/*Append the PDF page content to the context.*/
		FPDRenderContextAppendPage(context, page, aMatrix);

		/*Do the real rendering. 
	     *Optionally, rendering matrix can be modified by the last matrix.
		 */
		FPDRenderContextRender(context, device, NULL, NULL);

		/*Create the annotation list from the PDF page.*/
		annotList = FPDAnnotListNew(page);

		/*Display all the annotations to the device.*/
		FPDAnnotListDisplayAnnots(annotList, page, device, aMatrix, TRUE, NULL);
	}					
	
	/*Release all the object created.*/
	if (NULL != context)
	{
		FPDRenderContextDestroy(context);
		context = NULL;
	}	
	if (NULL != annotList)
	{
		FPDAnnotListDestroy(annotList);
		annotList = NULL;
	}
	if (NULL != page)
	{
		FPDPageDestroy(page);
		page = NULL;
	}
	if (NULL != device)
	{		
		FPDWindowsDeviceDestroy(device);					
		device = NULL;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CStaticDisplayPDF message handlers
