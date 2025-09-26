#if !defined(AFX_DISPLAYPDFDLG_H__A8056FAB_BF8C_4DDC_A575_B34EBD1C6B95__INCLUDED_)
#define AFX_DISPLAYPDFDLG_H__A8056FAB_BF8C_4DDC_A575_B34EBD1C6B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayPDFDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayPDFDlg dialog
#include "Resource.h"
#include "StaticDisplayPDF.h"
class CDisplayPDFDlg : public CDialog
{
virtual ULONG GetGestureStatus(CPoint ptTouch) { return 0;}
// Construction
public:
	CDisplayPDFDlg(CWnd* pParent = NULL);   // standard constructor
	~CDisplayPDFDlg();

// Dialog Data
	//{{AFX_DATA(CDisplayPDFDlg)
	enum { IDD = IDD_DLG_DISPLAY };
	CStaticDisplayPDF	m_stcDisplayPDF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayPDFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	FPD_Document m_pdfDoc;

// Implementation
public:
	void UpdateLastNextPageState();
	void UpdateZoomState();

	BOOL RenderPDFToBitmap(CRect rcDisplay, int nCurrentPage, int nRotate, float fScale);
	void SaveAsBmp(FS_DIBitmap bitmap, CString strPath);

	void SetPDFDoc(FPD_Document pdfDoc){m_pdfDoc = pdfDoc;}
	
private:

protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplayPDFDlg)
	afx_msg void OnButtonPreviouspage();
	afx_msg void OnButtonNextpage();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClockwiserotate();
	afx_msg void OnButtonCounterclockwiserotate();
	afx_msg void OnButtonZoomin();
	afx_msg void OnButtonZoomout();
	afx_msg void OnButtonRenderToBMP();
	afx_msg void OnButtonExtractPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYPDFDLG_H__A8056FAB_BF8C_4DDC_A575_B34EBD1C6B95__INCLUDED_)
