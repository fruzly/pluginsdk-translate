// DisplayPDFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DisplayPDFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayPDFDlg dialog

CDisplayPDFDlg::CDisplayPDFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayPDFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplayPDFDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDisplayPDFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayPDFDlg)
	DDX_Control(pDX, IDC_STATIC_DISPLAYPDF, m_stcDisplayPDF);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplayPDFDlg, CDialog)
	//{{AFX_MSG_MAP(CDisplayPDFDlg)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUSPAGE, &CDisplayPDFDlg::OnButtonPreviouspage)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPAGE, &CDisplayPDFDlg::OnButtonNextpage)
	ON_BN_CLICKED(IDC_BUTTON_CLOCKWISEROTATE, &CDisplayPDFDlg::OnButtonClockwiserotate)
	ON_BN_CLICKED(IDC_BUTTON_COUNTERCLOCKWISEROTATE, &CDisplayPDFDlg::OnButtonCounterclockwiserotate)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CDisplayPDFDlg::OnButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CDisplayPDFDlg::OnButtonZoomout)
	ON_BN_CLICKED(IDC_BUTTON_RENDERTOBMP, &CDisplayPDFDlg::OnButtonRenderToBMP)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACTPAGE, &CDisplayPDFDlg::OnButtonExtractPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDisplayPDFDlg::OnButtonPreviouspage()
{
	// TODO: Add your control notification handler code here
	m_stcDisplayPDF.PreviousPage();
	UpdateLastNextPageState();
	
}

void CDisplayPDFDlg::OnButtonNextpage() 
{
	// TODO: Add your control notification handler code here
	m_stcDisplayPDF.NextPage();
	UpdateLastNextPageState();
	
}

BOOL CDisplayPDFDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateLastNextPageState();
	UpdateZoomState();

	m_stcDisplayPDF.SetPDFDoc(m_pdfDoc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDisplayPDFDlg::UpdateLastNextPageState()
{	
	if (m_stcDisplayPDF.GetCurrentPage() == 0)
	{
		(GetDlgItem(IDC_BUTTON_PREVIOUSPAGE))->EnableWindow(FALSE);
	}
	else
	{
		(GetDlgItem(IDC_BUTTON_PREVIOUSPAGE))->EnableWindow(TRUE);
	}
	if (m_stcDisplayPDF.GetCurrentPage() == FPDDocGetPageCount(m_pdfDoc) - 1)
	{
		(GetDlgItem(IDC_BUTTON_NEXTPAGE))->EnableWindow(FALSE);
	}
	else
	{
		(GetDlgItem(IDC_BUTTON_NEXTPAGE))->EnableWindow(TRUE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CDisplayPDFDlg message handlers

void CDisplayPDFDlg::OnButtonClockwiserotate() 
{
	// TODO: Add your control notification handler code here
	m_stcDisplayPDF.ClockWiseRotate();
}

void CDisplayPDFDlg::OnButtonCounterclockwiserotate() 
{
	// TODO: Add your control notification handler code here
	m_stcDisplayPDF.CounterClockWiseRotate();
}

void CDisplayPDFDlg::OnButtonZoomin() 
{
	// TODO: Add your control notification handler code here
	m_stcDisplayPDF.ZoomIn();
	UpdateZoomState();
}

void CDisplayPDFDlg::OnButtonZoomout() 
{
	// TODO: Add your control notification handler code here
	m_stcDisplayPDF.ZoomOut();
	UpdateZoomState();
}

void CDisplayPDFDlg::UpdateZoomState()
{
	if (m_stcDisplayPDF.GetScale() >= 1.0)
	{
		(GetDlgItem(IDC_BUTTON_ZOOMIN))->EnableWindow(FALSE);
	}
	else
	{
		(GetDlgItem(IDC_BUTTON_ZOOMIN))->EnableWindow(TRUE);
	}

	if (m_stcDisplayPDF.GetScale() <= 0.25)
	{
		(GetDlgItem(IDC_BUTTON_ZOOMOUT))->EnableWindow(FALSE);
	}
	else
	{
		(GetDlgItem(IDC_BUTTON_ZOOMOUT))->EnableWindow(TRUE);
	}
}

CDisplayPDFDlg::~CDisplayPDFDlg()
{

}

void CDisplayPDFDlg::OnButtonRenderToBMP()
{	
	CRect rcDisplay;
	m_stcDisplayPDF.GetWindowRect(&rcDisplay);
	RenderPDFToBitmap(rcDisplay, m_stcDisplayPDF.GetCurrentPage(), m_stcDisplayPDF.GetRotate(), m_stcDisplayPDF.GetScale());	
}

void CDisplayPDFDlg::OnButtonExtractPage()
{
	
}

BOOL CDisplayPDFDlg::RenderPDFToBitmap(CRect rcDisplay, int nCurrentPage, int nRotate, float fScale)
{
	if (m_pdfDoc == NULL)
	{
		return FALSE;
	}
	
	/*Create a new GE device to render the PDF contents.
	 *And create a bitmap attached to the device.
	*/
	FPD_RenderDevice device = NULL;
	FS_DIBitmap bitmap = NULL;

	device = FPDFxgeDeviceNew();
	bitmap = FSDIBitmapNew();
	FSDIBitmapCreate(bitmap, rcDisplay.Width(), rcDisplay.Height(), FS_DIB_Argb, NULL, 0);
	FS_ARGB argb = (100 << 24) | (255 << 16) | (255 << 8) | 255;
	FSDIBitmapClear(bitmap, argb);
	FPDFxgeDeviceAttach(device, bitmap, 0);		
			
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
		FPDPageParseContent(page,NULL);
		
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
		
		CString csInfo;
		CString csPath;
		csPath.Format(L"f:/page%d.bmp",nCurrentPage + 1 );
		csInfo.Format(L"Page%d is rendered to %s.", nCurrentPage + 1, csPath);

		/*Save the PDF page to bitmap file.*/
		SaveAsBmp(bitmap, csPath);
		AfxMessageBox(csInfo);		
	}					
	
	/*Release all the object created.*/
	if (NULL != bitmap)
	{
		FSDIBitmapDestroy(bitmap);
		bitmap = NULL;
	}	
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
		FPDFxgeDeviceDestroy(device);				
		device = NULL;
	}
	return TRUE;
}

void CDisplayPDFDlg::SaveAsBmp( FS_DIBitmap bitmap, CString strPath )
{
	int width = FSDIBitmapGetWidth(bitmap);
	int height = FSDIBitmapGetHeight(bitmap);
	DWORD dwSize =width*height*4;

	/*Get the bitmap buffer.*/
	BYTE *pBuffer = (BYTE *)(FSDIBitmapGetBuffer(bitmap));
	
	/*Write the bitmap file with bitmap info.*/
	CFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeReadWrite))
	{
		BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER bmih;			
		
		bmfh.bfType = 0x4d42;
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSize;
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		file.Write(&bmfh, sizeof(BITMAPFILEHEADER));
		
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth =  width;
		bmih.biHeight = -height;
		bmih.biBitCount = 32;
		bmih.biPlanes = 1;
		bmih.biCompression=BI_RGB;
		bmih.biSizeImage=dwSize;
		bmih.biXPelsPerMeter=0;
		bmih.biYPelsPerMeter=0;
		bmih.biClrUsed=0;
		bmih.biClrImportant=0;
		file.Write(&bmih, sizeof(BITMAPINFOHEADER));
		file.Write(pBuffer, dwSize);
		file.Close();	
	}
}