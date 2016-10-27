// DIBTransformView.h : interface of the CDIBTransformView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBTRANSFORMVIEW_H__3D1FBCD0_50A2_40A2_9742_292DEC953DA8__INCLUDED_)
#define AFX_DIBTRANSFORMVIEW_H__3D1FBCD0_50A2_40A2_9742_292DEC953DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define OT_SELECT 1;
#define OT_ZOOM_OUT 2;
#define OT_ZOOM_IN  3;
//#define BOUND(x, mn, mx)	((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x)))

#include <iostream>
#include <deque>
using namespace std;

typedef deque<CRect> CRectLink;

class CDIBTransformView : public CScrollView
{
protected: // create from serialization only
	CDIBTransformView();
	DECLARE_DYNCREATE(CDIBTransformView)

// Attributes(变量)
public:
	CDIBTransformDoc* GetDocument();
	CPoint	m_ptStart;
	CRect	m_rcRubber;
	BOOL	m_bDrawingRubber;
	DWORD	m_nOperationType;
	CRect	markRect;
	LPRECT	p_markRect;
	CRectLink m_charRect;
	
private:
	int  m_nWidth;
	int  m_nHeight;
	bool cropflag;
	bool First;		// 为了消除一个bug而设
	
	//插值质量
	int m_nQuality;
	
	//缩放比例
	float m_fsx;
	float m_fsy;
	
	// 指向原图像
	LPBITMAPINFO lpRestoreDIB;
// Operations(函数)
public:

private:
	void Scale(CDC *pDC, CDib *pDib, int x, int y);
	void MakeGray(CDC *pDC, CDib* pDib, int x, int y);
	void ConvolutionFilter(CDC *pDC, CDib *pDib, int x, int y);
	void MedianFilter(CDC *pDC, CDib *pDib, int x, int y);
	void ThresholdIncise(CDC *pDC, CDib *pDib, int x, int y);
	void Detect(CDC *pDC, CDib* pDib, int x, int y);
	void FindJumpPoint(CDC *pDC, CDib* pDib, int x, int y);
	void LocateMark(CDC *pDC, CDib* pDib, int x, int y);
	void SlopeAdjust(CDC *pDC,  CDib *pDib,  int x,  int y);
	void CharSegment(CDC *pDC,  CDib *pDib,  int x,  int y);
	void CharStandard(CDC *pDC,  CDib *pDib,  int x,  int y);
	void Align(CDC *pDC, CDib *pDib, int x, int y);
	void DrawFrame(CDC* pDC, CDib *pDib, CRectLink charRect, unsigned int linewidth, COLORREF color);
	void Thining(CDC *pDC,  CDib *pDib,  int x,  int y);
	void CharRecognise(CDC *pDC,  CDib *pDib,  int x,  int y);
	void CharPretreat(CDC *pDC, CDib *pDib, int x, int y);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIBTransformView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void StopDrawRubber();
	void DrawRubber(CPoint point);
	bool Black_Back(CDC *pDC,  CDib *pDib,  int x,  int y);
	//重载DrawRubber, 用于车牌区域的切割
	void DrawRubber(CRect  markRect);
	virtual ~CDIBTransformView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDIBTransformView)
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelect();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnCrop();
	afx_msg void OnTransformZoom();
	afx_msg void OnZoomOut();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnZoomIn();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnColorToGray();
	afx_msg void OnUpdateColorToGray(CCmdUI* pCmdUI);
	afx_msg void OnGrayLinearize();
	afx_msg void OnUpdateGrayLinearize(CCmdUI* pCmdUI);
	afx_msg void OnGrayReverse();
	afx_msg void OnUpdateGrayReverse(CCmdUI* pCmdUI);
	afx_msg void OnGrayRoundOff();
	afx_msg void OnUpdateGrayRoundOff(CCmdUI* pCmdUI);
	afx_msg void OnGrayScale();
	afx_msg void OnUpdateGrayScale(CCmdUI* pCmdUI);
	afx_msg void OnHistogramBalance();
	afx_msg void OnUpdateHistogramBalance(CCmdUI* pCmdUI);
	afx_msg void OnHistogramMatch();
	afx_msg void OnUpdateHistogramMatch(CCmdUI* pCmdUI);
	afx_msg void OnImageRestore();
	afx_msg void OnUpdateImageRestore(CCmdUI* pCmdUI);
	afx_msg void OnProbabilityMatch();
	afx_msg void OnUpdateProbabilityMatch(CCmdUI* pCmdUI);
	afx_msg void OnLowpassNoiseRemove3();
	afx_msg void OnUpdateLowpassNoiseRemove3(CCmdUI* pCmdUI);
	afx_msg void OnLowpassNoiseRemove5();
	afx_msg void OnUpdateLowpassNoiseRemove5(CCmdUI* pCmdUI);
	afx_msg void OnMedianNoiseRemove();
	afx_msg void OnUpdateMedianNoiseRemove(CCmdUI* pCmdUI);
	afx_msg void OnLaplacianBasicDetect();
	afx_msg void OnUpdateLaplacianBasicDetect(CCmdUI* pCmdUI);
	afx_msg void OnLaplacianModerateDetect();
	afx_msg void OnUpdateLaplacianModerateDetect(CCmdUI* pCmdUI);
	afx_msg void OnThresholdIncise();
	afx_msg void OnHighpassBasicSharpen();
	afx_msg void OnUpdateHighpassBasicSharpen(CCmdUI* pCmdUI);
	afx_msg void OnHighpassModerateSharpen();
	afx_msg void OnUpdateHighpassModerateSharpen(CCmdUI* pCmdUI);
	afx_msg void OnSobelEdgeDetect();
	afx_msg void OnUpdateSobelEdgeDetect(CCmdUI* pCmdUI);
	afx_msg void OnKirschEdgeDetect();
	afx_msg void OnUpdateKirschEdgeDetect(CCmdUI* pCmdUI);
	afx_msg void OnGradientEastDetect();
	afx_msg void OnUpdateGradientEastDetect(CCmdUI* pCmdUI);
	afx_msg void OnDifferenceEastDetect();
	afx_msg void OnUpdateDifferenceEastDetect(CCmdUI* pCmdUI);
	afx_msg void OnJumpPoint();
	afx_msg void OnUpdateJumpPoint(CCmdUI* pCmdUI);
	afx_msg void OnCharPretreat();
	afx_msg void OnCharRecognise();
	afx_msg void OnPlateIncise();
	afx_msg void OnPlateAdjust();
	afx_msg void OnCharsegment();
	afx_msg void OnCharStdAlign();
	afx_msg void OnPretreatment();
	afx_msg void OnLocateMark();
	afx_msg void OnMarkLocation();
	afx_msg void OnPlateRecognise();
	afx_msg void OnCharThining();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DIBTransformView.cpp
inline CDIBTransformDoc* CDIBTransformView::GetDocument()
   { return (CDIBTransformDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBTRANSFORMVIEW_H__3D1FBCD0_50A2_40A2_9742_292DEC953DA8__INCLUDED_)
