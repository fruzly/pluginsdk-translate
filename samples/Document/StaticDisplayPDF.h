#if !defined(AFX_STATICDISPLAYPDF_H__10F371CD_D9FC_4FE1_88C1_7C1DD118B570__INCLUDED_)
#define AFX_STATICDISPLAYPDF_H__10F371CD_D9FC_4FE1_88C1_7C1DD118B570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticDisplayPDF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticDisplayPDF window

class CStaticDisplayPDF : public CStatic
{
virtual ULONG GetGestureStatus(CPoint ptTouch) { return 0;}
// Construction
public:
	CStaticDisplayPDF();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticDisplayPDF)
	//}}AFX_VIRTUAL
private:
	FPD_Document m_pdfDoc;

// Implementation
public:
	void SetCurrentPage(int nPageIndex);
	int GetCurrentPage();
	void NextPage();
	void PreviousPage();
	void ClockWiseRotate();
	void CounterClockWiseRotate();
	int GetRotate();
	void ZoomIn();
	void ZoomOut();
	float GetScale();
	BOOL DisplayPDFDocument(HDC hDC, CRect rcDisplay, int nCurrentPage, int nRotate, float fScale);

	void SetPDFDoc(FPD_Document pdfDoc){m_pdfDoc = pdfDoc;}

private:
	int m_nCurrentPage;
	int m_iRotate;
	float m_fScale;

public:
	virtual ~CStaticDisplayPDF();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticDisplayPDF)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICDISPLAYPDF_H__10F371CD_D9FC_4FE1_88C1_7C1DD118B570__INCLUDED_)
