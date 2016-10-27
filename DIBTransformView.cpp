// 操作流程：放大 - 灰度局部统计 - 低通滤波3*3 - sobel边缘检测 - 水平梯度检测 - 二值化 - 跳变图

#include "stdafx.h"
#include "DIBTransform.h"
#include "MainFrm.h"
#include "DIBTransformDoc.h"
#include "DIBTransformView.h"
#include "ZoomDlg.h"
#include "ImageGeometry.h"
#include "GrayProcess.h"
#include "ConvolutionFilter.h"
#include "MedianFilter.h"
#include "ImageIncise.h"
#include "SpecialDetectionFilter.h"
#include "ImageEdge.h"
#include "ImageLocation.h"
#include "CharPretreat.h"
#include "CharRecognise.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDIBTransformView, CScrollView)

BEGIN_MESSAGE_MAP(CDIBTransformView, CScrollView)
	//{{AFX_MSG_MAP(CDIBTransformView)
	ON_MESSAGE(WM_DOREALIZE, OnDoRealize)
	ON_COMMAND(ID_SELECT, OnSelect)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_CROP, OnCrop)
	ON_COMMAND(ID_TRANSFORM_ZOOM, OnTransformZoom)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_WM_CREATE()
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(IDM_COLOR_TO_GRAY, OnColorToGray)
	ON_UPDATE_COMMAND_UI(IDM_COLOR_TO_GRAY, OnUpdateColorToGray)
	ON_COMMAND(IDM_GRAY_LINEARIZE, OnGrayLinearize)
	ON_UPDATE_COMMAND_UI(IDM_GRAY_LINEARIZE, OnUpdateGrayLinearize)
	ON_COMMAND(IDM_GRAY_REVERSE, OnGrayReverse)
	ON_UPDATE_COMMAND_UI(IDM_GRAY_REVERSE, OnUpdateGrayReverse)
	ON_COMMAND(IDM_GRAY_ROUND_OFF, OnGrayRoundOff)
	ON_UPDATE_COMMAND_UI(IDM_GRAY_ROUND_OFF, OnUpdateGrayRoundOff)
	ON_COMMAND(IDM_GRAY_SCALE, OnGrayScale)
	ON_UPDATE_COMMAND_UI(IDM_GRAY_SCALE, OnUpdateGrayScale)
	ON_COMMAND(IDM_HISTOGRAM_BALANCE, OnHistogramBalance)
	ON_UPDATE_COMMAND_UI(IDM_HISTOGRAM_BALANCE, OnUpdateHistogramBalance)
	ON_COMMAND(IDM_HISTOGRAM_MATCH, OnHistogramMatch)
	ON_UPDATE_COMMAND_UI(IDM_HISTOGRAM_MATCH, OnUpdateHistogramMatch)
	ON_COMMAND(IDM_IMAGE_RESTORE, OnImageRestore)
	ON_UPDATE_COMMAND_UI(IDM_IMAGE_RESTORE, OnUpdateImageRestore)
	ON_COMMAND(IDM_PROBABILITY_MATCH, OnProbabilityMatch)
	ON_UPDATE_COMMAND_UI(IDM_PROBABILITY_MATCH, OnUpdateProbabilityMatch)
	ON_COMMAND(IDM_LOWPASS_NOISE_REMOVE_3, OnLowpassNoiseRemove3)
	ON_UPDATE_COMMAND_UI(IDM_LOWPASS_NOISE_REMOVE_3, OnUpdateLowpassNoiseRemove3)
	ON_COMMAND(IDM_LOWPASS_NOISE_REMOVE_5, OnLowpassNoiseRemove5)
	ON_UPDATE_COMMAND_UI(IDM_LOWPASS_NOISE_REMOVE_5, OnUpdateLowpassNoiseRemove5)
	ON_COMMAND(IDM_MEDIAN_NOISE_REMOVE, OnMedianNoiseRemove)
	ON_UPDATE_COMMAND_UI(IDM_MEDIAN_NOISE_REMOVE, OnUpdateMedianNoiseRemove)
	ON_COMMAND(IDM_LAPLACIAN_BASIC_DETECT, OnLaplacianBasicDetect)
	ON_UPDATE_COMMAND_UI(IDM_LAPLACIAN_BASIC_DETECT, OnUpdateLaplacianBasicDetect)
	ON_COMMAND(IDM_LAPLACIAN_MODERATE_DETECT, OnLaplacianModerateDetect)
	ON_UPDATE_COMMAND_UI(IDM_LAPLACIAN_MODERATE_DETECT, OnUpdateLaplacianModerateDetect)
	ON_COMMAND(IDM_THRESHOLD_INCISE, OnThresholdIncise)
	ON_COMMAND(IDM_HIGHPASS_BASIC_SHARPEN, OnHighpassBasicSharpen)
	ON_UPDATE_COMMAND_UI(IDM_HIGHPASS_BASIC_SHARPEN, OnUpdateHighpassBasicSharpen)
	ON_COMMAND(IDM_HIGHPASS_MODERATE_SHARPEN, OnHighpassModerateSharpen)
	ON_UPDATE_COMMAND_UI(IDM_HIGHPASS_MODERATE_SHARPEN, OnUpdateHighpassModerateSharpen)
	ON_COMMAND(IDM_SOBEL_EDGE_DETECT, OnSobelEdgeDetect)
	ON_UPDATE_COMMAND_UI(IDM_SOBEL_EDGE_DETECT, OnUpdateSobelEdgeDetect)
	ON_COMMAND(IDM_KIRSCH_EDGE_DETECT, OnKirschEdgeDetect)
	ON_UPDATE_COMMAND_UI(IDM_KIRSCH_EDGE_DETECT, OnUpdateKirschEdgeDetect)
	ON_COMMAND(IDM_GRADIENT_EAST_DETECT, OnGradientEastDetect)
	ON_UPDATE_COMMAND_UI(IDM_GRADIENT_EAST_DETECT, OnUpdateGradientEastDetect)
	ON_COMMAND(IDM_DIFFERENCE_EAST_DETECT, OnDifferenceEastDetect)
	ON_UPDATE_COMMAND_UI(IDM_DIFFERENCE_EAST_DETECT, OnUpdateDifferenceEastDetect)
	ON_COMMAND(IDM_JUMP_POINT, OnJumpPoint)
	ON_UPDATE_COMMAND_UI(IDM_JUMP_POINT, OnUpdateJumpPoint)
	ON_COMMAND(IDM_CHAR_PRETREAT, OnCharPretreat)
	ON_COMMAND(IDM_CHAR_RECOGNISE, OnCharRecognise)
	ON_COMMAND(IDM_PLATE_INCISE, OnPlateIncise)
	ON_COMMAND(IDM_PLATE_ADJUST, OnPlateAdjust)
	ON_COMMAND(IDM_CHAR_SEGMENT, OnCharsegment)
	ON_COMMAND(IDM_CHAR_STD_ALIGN, OnCharStdAlign)
	ON_COMMAND(IDM_PRETREATMENT, OnPretreatment)
	ON_COMMAND(IDM_LOCATE_MARK, OnLocateMark)
	ON_COMMAND(IDM_MARK_LOCATION, OnMarkLocation)
	ON_COMMAND(IDM_PLATE_RECOGNISE, OnPlateRecognise)
	ON_COMMAND(IDM_CHAR_THINING, OnCharThining)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformView construction/destruction

CDIBTransformView::CDIBTransformView()
{
	m_nOperationType = 0L;	// 缺省
	m_bDrawingRubber = FALSE;
	m_fsx = m_fsy = 1.0f;
	First = true;

	p_markRect = &markRect;
}

CDIBTransformView::~CDIBTransformView()
{
}

BOOL CDIBTransformView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformView drawing

void CDIBTransformView::OnDraw(CDC* pDC)
{
	CDIBTransformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDib* pDib = pDoc->m_pDib;
	if(pDib->m_hDib != NULL)
	{
		m_nWidth = (int) pDib->GetBITMAPWidth();
		m_nHeight = (int) pDib->GetBITMAPHeight();
		int cxDIB = (int) pDib->GetWidth();         // Size of DIB - x
		int cyDIB = (int) pDib->GetHeight();        // Size of DIB - y
		GlobalUnlock( pDib->m_hDib );
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		rcDest = rcDIB;
		if( First )
		{
			lpRestoreDIB = (LPBITMAPINFO)(pDib->m_hDib);
			First = false;
		}
		if( m_nOperationType == 0L || cropflag == true )
			pDib->PaintDIB(pDC->m_hDC, &rcDest,	&rcDIB);
		else if( m_nOperationType == IMAGE_RESTORE )
		{
			pDib->m_hDib = lpRestoreDIB;
			pDib->PaintDIB(pDC->m_hDC, &rcDest,&rcDIB);
			m_nOperationType = 0L;
		}
		//图像预处理
		else if( m_nOperationType == IMAGE_PRETREATMENT )
		{
			if( cxDIB != 640 )
			{
				m_fsx = (float)640 / (float)cxDIB;
				m_fsy = m_fsx;
				Scale(pDC, pDib, 0, 0);
				lpRestoreDIB = (LPBITMAPINFO)(pDib->m_hDib);
			}
			//将彩色车辆图像转为灰度图
			m_nOperationType = IMAGE_GRAY_COLOR_GRAYED_OUT;
			MakeGray(pDC, pDib, 0, 0);
			//低通滤波，减少噪声
			m_nOperationType = IMAGE_LOWPASS_NOISE_REMOVE_3;
			ConvolutionFilter(pDC, pDib, 0, 0);
			//SOBEL边缘检测
			m_nOperationType = IMAGE_SOBEL_EDGE_DETECT;
			Detect(pDC, pDib, 0, 0);
			//水平方向梯度边缘检测
			m_nOperationType = IMAGE_GRADIENT_EAST_DETECT;
			ConvolutionFilter(pDC, pDib, 0, 0);
			//车辆图像二值化
			m_nOperationType = IMAGE_THRESHOLD_INCISE;
			ThresholdIncise(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_GEOMETRY_MAGNIFY || m_nOperationType == IMAGE_GEOMETRY_MINIFY )
		{
			Scale(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_GRAY_COLOR_GRAYED_OUT || m_nOperationType == IMAGE_GRAY_PERCENTAGE || m_nOperationType == IMAGE_GRAY_LINEARIZE || m_nOperationType == IMAGE_GRAY_LINEAR_ROUND_OFF
				|| m_nOperationType == IMAGE_GRAY_REVERSE || m_nOperationType == IMAGE_GRAY_HISTOGRAM_BALANCE || m_nOperationType == IMAGE_GRAY_HISTOGRAM_MATCH || m_nOperationType == IMAGE_GRAY_STATISTIC_MATCH )
		{
			MakeGray(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_LOWPASS_NOISE_REMOVE_3 || m_nOperationType == IMAGE_LOWPASS_NOISE_REMOVE_5 || m_nOperationType == IMAGE_LAPLACIAN_BASIC_DETECT || m_nOperationType == IMAGE_LAPLACIAN_MODERATE_DETECT 
				|| m_nOperationType == IMAGE_HIGHPASS_BASIC_SHARPEN || m_nOperationType == IMAGE_HIGHPASS_MODERATE_SHARPEN || m_nOperationType == IMAGE_GRADIENT_EAST_DETECT || m_nOperationType == IMAGE_DIFFERENCE_EAST_DETECT )
		{
			ConvolutionFilter(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_MEDIAN_NOISE_REMOVE )
		{
			MedianFilter(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_SOBEL_EDGE_DETECT || m_nOperationType == IMAGE_KIRSCH_EDGE_DETECT )
		{
			Detect(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_THRESHOLD_INCISE )
		{
			ThresholdIncise(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		//车牌定位
		else if( m_nOperationType == IMAGE_MARK_LOCATION )
		{
			//形成跳变点图
			m_nOperationType = IMAGE_JUMP_POINT;
			FindJumpPoint(pDC, pDib, 0, 0);
			//车牌定位，切割出车牌图像
			LocateMark(pDC, pDib, 0, 0);
			DrawRubber(markRect);
			pDib->m_hDib = lpRestoreDIB;
			OnCrop();
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_JUMP_POINT )
		{
			FindJumpPoint(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_LOCATE_MARK )
		{
			//车牌定位，切割出车牌图像
			LocateMark(pDC, pDib, 0, 0);
			DrawRubber(markRect);
			pDib->m_hDib = lpRestoreDIB;
			OnCrop();
			m_nOperationType = 0L;
		}
		//字符预处理
		else if( m_nOperationType == IMAGE_CHAR_PRETREAT )
		{
			if ( cxDIB != 168 )
			{
				m_fsx = (float)168 / (float)cxDIB;
				m_fsy = m_fsx;
				Scale(pDC, pDib, 0, 0);
			}

			//将彩色车牌图像转为灰度图
			m_nOperationType = IMAGE_GRAY_STATISTIC_MATCH;
			MakeGray(pDC, pDib, 0, 0);
			//将灰度车牌图像二值化
			ThresholdIncise(pDC, pDib, 0, 0);
			if( Black_Back(pDC, pDib, 0, 0) )
			{
				//将二值车牌图像反色
				m_nOperationType = IMAGE_GRAY_REVERSE;
				MakeGray(pDC, pDib, 0, 0);
			}
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			//预处理，将车牌上的字符提取出来
			CharPretreat(pDC, pDib, 0, 0);
			Align(pDC, pDib, 0, 0);
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			//将车牌图像去除离散噪点，并将字符细化
			Thining(pDC, pDib, 0, 0);
			//画框
			DrawFrame(pDC, pDib, m_charRect, 2, RGB(252,115,27));
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_PLATE_INCISE )
		{
			if( cxDIB != 168 )
			{
				m_fsx = (float)168 / (float)cxDIB;
				m_fsy = m_fsx;
				Scale(pDC, pDib, 0, 0);
			}
			//将彩色车牌图像转为灰度图
			m_nOperationType = IMAGE_GRAY_STATISTIC_MATCH;
			MakeGray(pDC, pDib, 0, 0);
			//将灰度车牌图像二值化
			ThresholdIncise(pDC, pDib, 0, 0);
			if( Black_Back(pDC, pDib, 0, 0) )
			{
				//将二值车牌图像反色
				m_nOperationType = IMAGE_GRAY_REVERSE;
				MakeGray(pDC, pDib, 0, 0);
			}
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_PLATE_ADJUST )
		{
			//将二值车牌图像反色
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			SlopeAdjust(pDC, pDib, 0, 0);
			//将二值车牌图像反色
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_CHAR_SEGMENT )
		{
			//将二值车牌图像反色
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			CharSegment(pDC, pDib, 0, 0);
			//将二值车牌图像反色
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			DrawFrame(pDC, pDib, m_charRect, 2, RGB(252,115,27));
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_CHAR_STD_ALIGN )
		{
			//将二值车牌图像反色
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			CharStandard(pDC, pDib, 0, 0);
			Align(pDC, pDib, 0, 0);
			//将二值车牌图像反色
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			DrawFrame(pDC, pDib, m_charRect, 2, RGB(252,115,27));
			m_nOperationType = 0L;
		}
		else if( m_nOperationType == IMAGE_CHAR_THINING )
		{
			//将车牌图像去除离散噪点，并将字符细化
			Thining(pDC, pDib, 0, 0);
			//画框
			DrawFrame(pDC, pDib, m_charRect, 2, RGB(252,115,27));
			m_nOperationType = 0L;
		}
		//字符识别
		else if( m_nOperationType == IMAGE_CHAR_RECOGNISE )
		{
			CharRecognise(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
		//总实现
		else if( m_nOperationType == IMAGE_PLATE_RECOGNISE )
		{
			if( cxDIB != 640 )
			{
				m_fsx = (float)640 / (float)cxDIB;
				m_fsy = m_fsx;
				Scale(pDC, pDib, 0, 0);
				lpRestoreDIB = (LPBITMAPINFO)(pDib->m_hDib);
			}
			//将彩色车辆图像转为灰度图
			m_nOperationType = IMAGE_GRAY_STATISTIC_MATCH;
			MakeGray(pDC, pDib, 0, 0);
			//低通滤波，减少噪声
			m_nOperationType = IMAGE_LOWPASS_NOISE_REMOVE_3;
			ConvolutionFilter(pDC, pDib, 0, 0);
			//SOBEL边缘检测
			m_nOperationType = IMAGE_SOBEL_EDGE_DETECT;
			Detect(pDC, pDib, 0, 0);
			//水平方向梯度边缘检测
			m_nOperationType = IMAGE_GRADIENT_EAST_DETECT;
			ConvolutionFilter(pDC, pDib, 0, 0);
			//车辆图像二值化
			m_nOperationType = IMAGE_THRESHOLD_INCISE;
			ThresholdIncise(pDC, pDib, 0, 0);
			//形成跳变点图
			m_nOperationType = IMAGE_JUMP_POINT;
			FindJumpPoint(pDC, pDib, 0, 0);
			//车牌定位，切割出车牌图像
			LocateMark(pDC, pDib, 0, 0);
			DrawRubber(markRect);
			pDib->m_hDib = lpRestoreDIB;
			OnCrop();
			pDib = pDoc->m_pDib;
			cxDIB = pDib->GetWidth();
			if( cxDIB != 168 )
			{
				m_fsx = (float)168 / (float)cxDIB;
				m_fsy = m_fsx;
				Scale(pDC, pDib, 0, 0);
			}
			//将彩色车牌图像转为灰度图
			m_nOperationType = IMAGE_GRAY_STATISTIC_MATCH;
			MakeGray(pDC, pDib, 0, 0);
			//将灰度车牌图像二值化
			ThresholdIncise(pDC, pDib, 0, 0);
			if( Black_Back(pDC, pDib, 0, 0) )
			{
				//将二值车牌图像反色
				m_nOperationType = IMAGE_GRAY_REVERSE;
				MakeGray(pDC, pDib, 0, 0);
			}
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			//预处理，将车牌上的字符提取出来
			//CharPretreat(pDC, pDib, 0, 0);
			SlopeAdjust(pDC, pDib, 0, 0);
			CharSegment(pDC, pDib, 0, 0);
			CharStandard(pDC, pDib, 0, 0);
			Align(pDC, pDib, 0, 0);
			m_nOperationType = IMAGE_GRAY_REVERSE;
			MakeGray(pDC, pDib, 0, 0);
			//将车牌图像去除离散噪点，并将字符细化
			Thining(pDC, pDib, 0, 0);
			//画框
			DrawFrame(pDC, pDib, m_charRect, 2, RGB(252,115,27));
			CharRecognise(pDC, pDib, 0, 0);
			m_nOperationType = 0L;
		}
	}
}

void CDIBTransformView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = GetDocument()->m_pDib->GetWidth();
	sizeTotal.cy = GetDocument()->m_pDib->GetHeight();
	SetScrollSizes(MM_TEXT, sizeTotal);		   // 设置滚动视图的映射模式和大小
}

BOOL CDIBTransformView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDIBTransformView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDIBTransformView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformView diagnostics

#ifdef _DEBUG
void CDIBTransformView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIBTransformView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIBTransformDoc* CDIBTransformView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIBTransformDoc)));
	return (CDIBTransformDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformView message handlers
LRESULT CDIBTransformView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	CDIBTransformDoc* pDoc = GetDocument();
	if(pDoc->m_pDib == NULL)
		return 0L;  // must be a new document
	
	CPalette* pPal = pDoc->m_pDib->m_pPalette;
	if(pPal != NULL)
	{
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		
		CClientDC appDC(pAppFrame);
		
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		
		if(oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tSelectPalette failed in CDibView::OnPaletteChanged\n");
		}
	}
	
	return 0L;
}

void CDIBTransformView::OnSelect() 
{	
	StopDrawRubber();
	m_nOperationType = IMAGE_CUT;
	SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_CROSS));
		SetCapture();
}

void CDIBTransformView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_nOperationType == IMAGE_CUT )
	{
		StopDrawRubber();
		m_bDrawingRubber = TRUE;
		m_ptStart = point;
		m_rcRubber.SetRectEmpty();
	}
	if( m_nOperationType == IMAGE_GEOMETRY_MAGNIFY )
	{
		// 显示放大的图像
		SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM));
		CDIBTransformDoc* pDoc = GetDocument();
		m_fsx = (float)1.1;
		m_fsy = (float)1.1;
		Invalidate();
	}
	if( m_nOperationType == IMAGE_GEOMETRY_MINIFY )
	{
		// 显示缩小的图像
		SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM));
		CDIBTransformDoc* pDoc = GetDocument();
		m_fsx = (float)0.9;
		m_fsy = (float)0.9;
		Invalidate();
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CDIBTransformView::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bDrawingRubber )
	{
		DrawRubber(point);
	}	
	CScrollView::OnMouseMove(nFlags, point);
}

void CDIBTransformView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_nOperationType == IMAGE_CUT )
	{
		m_bDrawingRubber = FALSE;
		ReleaseCapture();
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CDIBTransformView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	CDIBTransformDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->m_pOldDib != NULL);
}

void CDIBTransformView::OnCrop()
{
	if(m_rcRubber.IsRectEmpty())
		return;
	cropflag = true;
	CDIBTransformDoc* pDoc = GetDocument();

	pDoc->m_pDib->m_hDib = pDoc->m_pDib->CropDIB(m_rcRubber);
	CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
	StopDrawRubber();
}

void CDIBTransformView::OnTransformZoom()
{
	CDIBTransformDoc* pDoc = GetDocument();
	CZoomDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_nOperationType = IMAGE_GEOMETRY_MAGNIFY;
		m_fsx = dlg.m_fXRatio;
		m_fsy = dlg.m_fYRatio;
		CSize sizeTotal(pDoc->m_pDib->GetWidth(),pDoc->m_pDib->GetHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);	
	}
	cropflag = false;	// 表示截图结束，使得该功能可以实现
}

void CDIBTransformView::OnZoomOut()
{
	m_nOperationType = IMAGE_GEOMETRY_MAGNIFY;
	cropflag = false;
}

void CDIBTransformView::OnZoomIn()
{
	m_nOperationType = IMAGE_GEOMETRY_MINIFY;
	cropflag = false;
}

void CDIBTransformView::DrawRubber(CPoint point)
{
	// 获得设备上下文并设置光栅操作方式
	CClientDC dc(this);

	CPen pen(PS_DOT, 1, RGB(0,0,0));
	CPen* pOldPen = dc.SelectObject(&pen);
	int nOldRop = dc.SetROP2(R2_NOTXORPEN);
	// 若已经画了矩形则清除它
	dc.Rectangle(&m_rcRubber);

	// 调整选择的坐标系
	if(m_nOperationType == IMAGE_CUT)
	{
		// 获得当前滚动条的位置
		int nScrollX = GetScrollPos(SB_HORZ);
		int nScrollY = GetScrollPos(SB_VERT);
		// 计算并设置新的滚动条位置
		CRect rcClient;
		GetClientRect(&rcClient);
		int nMinX, nMaxX, nMinY, nMaxY;
		GetScrollRange(SB_HORZ, &nMinX, &nMaxX);
		GetScrollRange(SB_VERT, &nMinY, &nMaxY);
		BOOL bNeedRedraw = FALSE;
		if((rcClient.Width() <(int)(GetDocument()->m_pDib->GetWidth())) && (point.x < 0 || point.x > rcClient.right))
		{
			nScrollX += point.x;
			nScrollX = BOUND(nScrollX, nMinX, nMaxX);
			SetScrollPos(SB_HORZ, nScrollX);
			bNeedRedraw = TRUE;
		}
		if((rcClient.Height() < (int)(GetDocument()->m_pDib->GetHeight())) && (point.y < 0 || point.y > rcClient.bottom))
		{
			nScrollY += point.y;
			nScrollY = BOUND(nScrollY, nMinY, nMaxY);
			SetScrollPos(SB_VERT, nScrollY);
			bNeedRedraw = TRUE;
		}
		if(bNeedRedraw)
		{
			// 刷新
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
	CPoint ptStart(m_ptStart);
	if(point.x < ptStart.x)
	{
		m_rcRubber.left = point.x;
		m_rcRubber.right = ptStart.x;
	}	
	else
	{
		m_rcRubber.left = ptStart.x;
		m_rcRubber.right = point.x;
	}
	if(point.y < ptStart.y)
	{
		m_rcRubber.top = point.y;
		m_rcRubber.bottom = ptStart.y;
	}
	else
	{
		m_rcRubber.top = ptStart.y;
		m_rcRubber.bottom = point.y;
	}
	m_rcRubber.NormalizeRect();
	// 画新的矩形
	dc.Rectangle(m_rcRubber);
	dc.SelectObject(pOldPen);
	dc.SetROP2(nOldRop);
}

//重载DrawRubber, 用于车牌区域的切割
void CDIBTransformView::DrawRubber(CRect  markRect)
{
	// 获得设备上下文并设置光栅操作方式
	CClientDC dc(this);
	CPen pen(PS_DOT, 1, RGB(0,0,0));
	CPen* pOldPen = dc.SelectObject(&pen);
	int nOldRop = dc.SetROP2(R2_NOTXORPEN);
	// 若已经画了矩形则清除它
	dc.Rectangle(&m_rcRubber);
	// 调整选择的坐标系
	if(m_nOperationType == IMAGE_CUT)
	{
		// 获得当前滚动条的位置
		int nScrollX = GetScrollPos(SB_HORZ);
		int nScrollY = GetScrollPos(SB_VERT);
		// 计算并设置新的滚动条位置
		CRect rcClient;
		GetClientRect(&rcClient);
		int nMinX, nMaxX, nMinY, nMaxY;
		GetScrollRange(SB_HORZ, &nMinX, &nMaxX);
		GetScrollRange(SB_VERT, &nMinY, &nMaxY);
		BOOL bNeedRedraw = FALSE;
		if(bNeedRedraw)
		{
			// 刷新
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
	CPoint ptStart(m_ptStart);
	m_rcRubber.left = markRect.left;
	m_rcRubber.right = markRect.right;
	m_rcRubber.top = markRect.top;
	m_rcRubber.bottom = markRect.bottom;
	m_rcRubber.NormalizeRect();
	// 画新的矩形
	dc.Rectangle(m_rcRubber);
	dc.SelectObject(pOldPen);
	dc.SetROP2(nOldRop);
}

void CDIBTransformView::StopDrawRubber()
{
	if( m_nOperationType == IMAGE_CUT)
	{
		// 若已经有橡皮筋矩形，清除它
		if(! m_rcRubber.IsRectEmpty())
		{
			// 获得设备上下文并设置其光栅操作方式
			CClientDC dc(this);
			int OldRop = dc.SetROP2(R2_NOTXORPEN);
			CPen pen(PS_DOT, 1, RGB(0,0,0));
			CPen* pOldPen = dc.SelectObject(&pen);
			dc.Rectangle(&m_rcRubber);
			dc.SetROP2(OldRop);
			dc.SelectObject(pOldPen);
			m_rcRubber.SetRectEmpty();
		}
	}
}

int CDIBTransformView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CDIBTransformView::OnFileSave() 
{
	// TODO: Add your command handler code here
	CFileDialog fileSaveDlg(FALSE, "*.bmp", ".bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, "Bitmap Files(*.bmp)|*.bmp|", NULL);
	int nRespond = fileSaveDlg.DoModal();
	if(nRespond == IDOK)
	{
		CString filePath = fileSaveDlg.GetPathName();
		CDIBTransformDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CDib* pDib = pDoc->m_pDib;
		pDib->Save(filePath);
		pDib->ClearMemory();
	}
}

void CDIBTransformView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	CFileDialog fileSaveDlg(FALSE, "*.bmp", "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files(*.bmp)", NULL);
	int nRespond = fileSaveDlg.DoModal();
	if(nRespond == IDOK)
	{
		CString filePath = fileSaveDlg.GetPathName();
		CDIBTransformDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CDib* pDib = pDoc->m_pDib;
		pDib->Save(filePath);
		pDib->ClearMemory();
	}
}

//彩色转换为灰阶表示
void CDIBTransformView::OnColorToGray()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_COLOR_GRAYED_OUT;
	Invalidate();
	cropflag = false;	// 表示截图结束，使得该功能可以实现
}

void CDIBTransformView::OnUpdateColorToGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_COLOR_GRAYED_OUT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度比例化
void CDIBTransformView::OnGrayScale()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_PERCENTAGE;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateGrayScale(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_PERCENTAGE)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度线性化
void CDIBTransformView::OnGrayLinearize()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_LINEARIZE;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateGrayLinearize(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_LINEARIZE)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度一次线性截断
void CDIBTransformView::OnGrayRoundOff()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_LINEAR_ROUND_OFF;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateGrayRoundOff(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_LINEAR_ROUND_OFF)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度取反
void CDIBTransformView::OnGrayReverse()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_REVERSE;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateGrayReverse(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_REVERSE)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度直方图均衡化
void CDIBTransformView::OnHistogramBalance()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_HISTOGRAM_BALANCE;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateHistogramBalance(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_HISTOGRAM_BALANCE)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度直方图规定化
void CDIBTransformView::OnHistogramMatch()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_HISTOGRAM_MATCH;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateHistogramMatch(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_HISTOGRAM_MATCH)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//灰度概率匹配
void CDIBTransformView::OnProbabilityMatch()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRAY_STATISTIC_MATCH;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateProbabilityMatch(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRAY_STATISTIC_MATCH)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//低通过滤3x3
void CDIBTransformView::OnLowpassNoiseRemove3()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_LOWPASS_NOISE_REMOVE_3;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateLowpassNoiseRemove3(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_LOWPASS_NOISE_REMOVE_3)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//低通过滤5x5
void CDIBTransformView::OnLowpassNoiseRemove5()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_LOWPASS_NOISE_REMOVE_5;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateLowpassNoiseRemove5(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_LOWPASS_NOISE_REMOVE_5)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//高通滤波基本锐化
void CDIBTransformView::OnHighpassBasicSharpen()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_HIGHPASS_BASIC_SHARPEN;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateHighpassBasicSharpen(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_HIGHPASS_BASIC_SHARPEN)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//高通滤波适度锐化
void CDIBTransformView::OnHighpassModerateSharpen()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_HIGHPASS_MODERATE_SHARPEN;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateHighpassModerateSharpen(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_HIGHPASS_MODERATE_SHARPEN)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//中值滤波
void CDIBTransformView::OnMedianNoiseRemove()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_MEDIAN_NOISE_REMOVE;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateMedianNoiseRemove(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_MEDIAN_NOISE_REMOVE)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//水平方向梯度检测
void CDIBTransformView::OnGradientEastDetect()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_GRADIENT_EAST_DETECT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateGradientEastDetect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_GRADIENT_EAST_DETECT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//水平方向平移差分
void CDIBTransformView::OnDifferenceEastDetect()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_DIFFERENCE_EAST_DETECT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateDifferenceEastDetect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_DIFFERENCE_EAST_DETECT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//拉普拉斯基本边缘检测
void CDIBTransformView::OnLaplacianBasicDetect()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_LAPLACIAN_BASIC_DETECT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateLaplacianBasicDetect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_LAPLACIAN_BASIC_DETECT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//拉普拉斯适度边缘检测
void CDIBTransformView::OnLaplacianModerateDetect()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_LAPLACIAN_MODERATE_DETECT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateLaplacianModerateDetect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_LAPLACIAN_MODERATE_DETECT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//Sobel边缘检测
void CDIBTransformView::OnSobelEdgeDetect()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_SOBEL_EDGE_DETECT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateSobelEdgeDetect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_SOBEL_EDGE_DETECT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//Kirsch边缘检测
void CDIBTransformView::OnKirschEdgeDetect()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_KIRSCH_EDGE_DETECT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateKirschEdgeDetect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_KIRSCH_EDGE_DETECT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//基于阈值的图像区域分割
void CDIBTransformView::OnThresholdIncise()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_THRESHOLD_INCISE;
	Invalidate();
	cropflag = false;
}

//图像预处理(组合)
void CDIBTransformView::OnPretreatment() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_PRETREATMENT;
	Invalidate();
	cropflag = false;
}

//找出跳变点
void CDIBTransformView::OnJumpPoint()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_JUMP_POINT;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateJumpPoint(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_JUMP_POINT)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

//车牌区域定位
void CDIBTransformView::OnLocateMark() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_LOCATE_MARK;
	Invalidate();
	cropflag = false;
}

//车牌区域定位(组合)
void CDIBTransformView::OnMarkLocation() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_MARK_LOCATION;
	Invalidate();
	cropflag = false;
}

//车牌二值化
void CDIBTransformView::OnPlateIncise() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_PLATE_INCISE;
	Invalidate();
	cropflag = false;
}

//车牌倾斜度调整
void CDIBTransformView::OnPlateAdjust() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_PLATE_ADJUST;
	Invalidate();
	cropflag = false;
}

//字符分割
void CDIBTransformView::OnCharsegment() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_CHAR_SEGMENT;
	Invalidate();
	cropflag = false;
}

//字符归一化并紧缩重排
void CDIBTransformView::OnCharStdAlign() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_CHAR_STD_ALIGN;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnCharThining() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_CHAR_THINING;
	Invalidate();
	cropflag = false;
}

//字符预处理(组合)
void CDIBTransformView::OnCharPretreat() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_CHAR_PRETREAT;
	Invalidate();
	cropflag = false;
}

//字符识别
void CDIBTransformView::OnCharRecognise() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_CHAR_RECOGNISE;
	Invalidate();
	cropflag = false;
}

//总实现
void CDIBTransformView::OnPlateRecognise() 
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_PLATE_RECOGNISE;
	Invalidate();
	cropflag = false;
}

//恢复图像
void CDIBTransformView::OnImageRestore()
{
	// TODO: Add your command handler code here
	m_nOperationType = IMAGE_RESTORE;
	Invalidate();
	cropflag = false;
}

void CDIBTransformView::OnUpdateImageRestore(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_nOperationType == IMAGE_RESTORE)
		pCmdUI ->SetCheck(TRUE);
	else
		pCmdUI ->SetCheck(FALSE);
	cropflag = false;
}

void CDIBTransformView::Scale(CDC *pDC,  CDib *pDib,  int x,  int y)//放缩图像
{// 参数说明：nWidth和nHeight表示源图像的长宽
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) 
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据:输入源
	DWORD dwSizeSrc = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 =  new BYTE[dwSizeSrc];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32,  0 ,  dwSizeSrc);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	//目的图的有效宽度和有效高度
	int nWidthDst = (int)(m_fsx * width + 0.5f);
	int nHeightDst = (int)(m_fsy * height + 0.5f);
	DWORD dwSizeDst = ((DWORD)(nWidthDst * nHeightDst)) * 4;
	BYTE* pbyBitsDst32 =  new BYTE[dwSizeDst];
	if(pbyBitsDst32 == NULL) return;
	memset(pbyBitsDst32,  0,  dwSizeDst);
	CImageGeometry* ig = new CImageGeometry();
	//设置插值质量
	ig->SetQuality( IMAGE_GEOMETRY_THREE_ORDER_INTERPOLATE );
	ig->Scale(pbyBitsSrc32, x, y, w, h, width, height, pbyBitsDst32, nWidthDst, nHeightDst);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = ig->CreateDdb(pDC->m_hDC, nWidthDst, nHeightDst, pbyBitsDst32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, nWidthDst, nHeightDst, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	DWORD dwSize;
	BOOL flag = true;
	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	// 为新的DIB申请内存并填充其BITMAPINFO结构 
	// dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = nWidthDst;
	lpbmi->bmiHeader.biHeight = nHeightDst;
	
	// 对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsDst32, nWidthDst, nHeightDst, lpTargetBits, pDib->m_nBitCount);
	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete ig;
	delete[] pbyBitsDst32;
	if( flag )
		delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//灰度化
void CDIBTransformView::MakeGray(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if(!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if(!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) return ;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	//对比度处理
	CGrayProcess* gp = new CGrayProcess();
	gp->SetOperation(m_nOperationType);
	//该数组只对直方图规定化起作用:
	//注意, 灰度等级是256级, 必须保证数组有256个元数
	int anFrequencies[256];
	switch(m_nOperationType)
	{
		//灰度比例化
		case IMAGE_GRAY_PERCENTAGE:
		{
			gp->SetParam1i(150);
			break;
		}
		//灰度线性化:
		case IMAGE_GRAY_LINEARIZE:
		{
			gp->SetParam4by(64, 202, 96, 255);
			break;
		}
		//灰度线性截断
		case IMAGE_GRAY_LINEAR_ROUND_OFF:
		{
			gp->SetParam4by(64, 202, 96, 255);
			break;
		}
		//灰度直方图规定化(即灰度直方图匹配)
		case IMAGE_GRAY_HISTOGRAM_MATCH:
		{
			//灰度分布频数, 可以通过频率乘以总样本数得到
			int i = 0;
			for(i = 0;i < 256;i++)
				anFrequencies[i] = 0;
			//提供5级灰度
			//如果两个整数相等, 无论它们为0或100, 其频率都是相等的
			anFrequencies[0] = 1;
			for(i = 63;i < 256;i += 64)
				anFrequencies[i] = 1;
			gp->SetParam1iv(anFrequencies);
			break;
		}
		case IMAGE_GRAY_STATISTIC_MATCH:
		{
			gp->SetParam2i(100, 192);
			break;
		}
		default:break;
	}
	//获取数据后, 其基本参考点变为(0, 0)
	gp->MakeGray(pbyBitsSrc32, x, y, w, h, width, height);
	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = gp->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	BOOL flag = true;

	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	// 为新的DIB申请内存并填充其BITMAPINFO结构 
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	// 对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete gp;
	if ( flag )
		delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//可完成多项功能
void CDIBTransformView::ConvolutionFilter(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) return ;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);

	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	//图像区域处理
	CConvolutionFilter* cf = new CConvolutionFilter();
	//获取数据后, 其基本参考点变为(0, 0).
	cf->SetOperation(m_nOperationType);
	if(m_nOperationType == IMAGE_GENERAL_CONVOLUTION_FILTER)
	{
		int* pnKernel = new int[49];
		pnKernel[0] = pnKernel[6] = pnKernel[42] = pnKernel[48] = 0;
		
		pnKernel[1] = pnKernel[5] = pnKernel[7] = pnKernel[13] = 0;
		pnKernel[35] = pnKernel[41] = pnKernel[43] = pnKernel[47] = 0;

		pnKernel[2] = pnKernel[4] = pnKernel[8] = pnKernel[12] = 4;
		pnKernel[14] = pnKernel[20] = pnKernel[28] = pnKernel[34] = 4;
		pnKernel[36] = pnKernel[40] = pnKernel[44] = pnKernel[46] = 4;
		
		pnKernel[3] = pnKernel[9] = pnKernel[15] = pnKernel[21] = 0;
		pnKernel[27] = pnKernel[33] = pnKernel[39] = pnKernel[45] = 0;
		pnKernel[11] = pnKernel[19] = pnKernel[29] = pnKernel[37] = 0;

		pnKernel[10] = pnKernel[16] = pnKernel[22] = pnKernel[18] = 0;
		pnKernel[26] = pnKernel[32] = pnKernel[38] = pnKernel[30] = 0;

		pnKernel[17] = pnKernel[23] = pnKernel[25] = pnKernel[31] = -1;
		pnKernel[24] = 5;		

		cf->SetKernel(pnKernel, 7, 7);
		//可以释放, 因为CConvolutionFilter类重新建立了卷积核版本
		delete[] pnKernel;
	}
	cf->Filter(pbyBitsSrc32, x, y, w, h, width, height);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cf->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	BOOL flag = true;
	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	// 为新的DIB申请内存并填充其BITMAPINFO结构 
	// dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	// 对于非32位位图全部作为24位位图处理
	if ( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )	
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth, 24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);

	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cf;
	if( flag )
		delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//中值滤波
void CDIBTransformView::MedianFilter(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if(!pDib->m_hDib)
		return;

	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if(!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);

	// 32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CMedianFilter* mf = new CMedianFilter();
	int* pnMask = new int[25];
	// 十字形
	for(int i = 0;i < 25;i++)
		pnMask[i] = 0;
	pnMask[2] = pnMask[7] = pnMask[12] = pnMask[17] = pnMask[22] = 1;
	pnMask[10] = pnMask[11] = pnMask[13] = pnMask[14] = 1;

	mf->SetMask(pnMask, 5, 5);
	mf->Medianize(pbyBitsSrc32, x, y, w, h, width, height);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = mf->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	BOOL flag = true;

	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;

	// 为新的DIB申请内存并填充其BITMAPINFO结构 
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	// 对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);

	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete mf;
	if ( flag )
		delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//二值化
void CDIBTransformView::ThresholdIncise(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) 
		return ;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);

	CImageIncise* Ii = new CImageIncise();
	Ii->ThresholdIncise(pbyBitsSrc32, x, y, w, h, width, height);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = Ii->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	BOOL flag = true;

	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;

	// 为新的DIB申请内存并填充其BITMAPINFO结构 
	// dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	//lpbmi->bmiHeader.biWidth = m_nWidth;
	//lpbmi->bmiHeader.biHeight = m_nHeight;
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	// 对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);

	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete Ii;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//Sobel边缘检测
void CDIBTransformView::Detect(CDC *pDC, CDib* pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))
		return ;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	//对比度处理
	CSpecialDetectionFilter* sdf = new CSpecialDetectionFilter();
	sdf->SetOperation(m_nOperationType);
	//获取数据后, 其基本参考点变为(0, 0).
	sdf->Filter(pbyBitsSrc32, x, y, w, h, width, height);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = sdf->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);
	
	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	BOOL flag = true;

	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	// 为新的DIB申请内存并填充其BITMAPINFO结构
	// dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	// 对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete sdf;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//生成边缘点图
void CDIBTransformView::FindJumpPoint(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if(!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) 
		return ;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) 
		return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CImageEdge* ie = new CImageEdge();
	ie->FindJumpPoint(pbyBitsSrc32, x, y, w, h, width, height);
	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = ie->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  //位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  //指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  //DIB句柄
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	//对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete ie;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//车牌定位
void CDIBTransformView::LocateMark(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))		
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CImageLocation* il = new CImageLocation();
	il->LocateMark(pbyBitsSrc32, x, y, w, h, width, height, p_markRect);
	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = il->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  //位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  //指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  //DIB句柄
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	//对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);

	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete il;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//倾斜度调整
void CDIBTransformView::SlopeAdjust(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))		
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CCharPretreat* cp = new CCharPretreat();
	cp->SlopeAdjust(pbyBitsSrc32, x, y, w, h, width, height);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cp->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);
	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  //位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  //指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  //DIB句柄
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	//对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )	
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cp;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//字符分割
void CDIBTransformView::CharSegment(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))		
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CCharPretreat* cp = new CCharPretreat();
	m_charRect = cp->CharSegment(pbyBitsSrc32, x, y, w, h, width, height);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cp->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  //位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  //指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  //DIB句柄
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	//对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cp;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

//字符归一化并紧缩重排
void CDIBTransformView::CharStandard(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))		
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CCharPretreat* cp = new CCharPretreat();
	cp->StdDIBbyRect(pbyBitsSrc32, x, y, w, h, width, height, m_charRect);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cp->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  //位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  //指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  //DIB句柄
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	//对于非32位位图全部作为24位位图处理
	if ( pDib->m_nBitCount != 32 )
	{
		if ( pDib->m_nBitCount != 24 )	
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cp;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

void CDIBTransformView::Align(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据:输入源
	DWORD dwSizeSrc = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 =  new BYTE[dwSizeSrc];
	if(pbyBitsSrc32 == NULL) return;
	memset(pbyBitsSrc32,  0 ,  dwSizeSrc);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32, pDib->m_hDib);
	//目的图的有效宽度和有效高度
	int nWidthDst = m_charRect.front().Width() * m_charRect.size();
	int nHeightDst = m_charRect.front().Height();
	DWORD dwSizeDst = ((DWORD)(nWidthDst * nHeightDst)) * 4;
	BYTE* pbyBitsDst32 =  new BYTE[dwSizeDst];
	if(pbyBitsDst32 == NULL) return;
	memset(pbyBitsDst32,  0,  dwSizeDst);
	CCharPretreat* cp = new CCharPretreat();

	cp->AutoAlign(pbyBitsSrc32, x, y, w, h, width, height, pbyBitsDst32, m_charRect);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cp->CreateDdb(pDC->m_hDC, nWidthDst, nHeightDst, pbyBitsDst32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, nWidthDst, nHeightDst, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	DWORD dwSize;
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = nWidthDst;
	lpbmi->bmiHeader.biHeight = nHeightDst;
	
	//对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth, 24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsDst32, nWidthDst, nHeightDst, lpTargetBits, pDib->m_nBitCount);
	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cp;
	delete[] pbyBitsDst32;
	if ( flag )
		delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

void CDIBTransformView::Thining(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1))) 
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	// 32位颜色数据:输入源
	DWORD dwSizeSrc = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 =  new BYTE[dwSizeSrc];
	if(pbyBitsSrc32 == NULL) 
		return;
	memset(pbyBitsSrc32,  0 ,  dwSizeSrc);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	//目的图的有效宽度和有效高度
	int nWidthDst = width;
	int nHeightDst = height;
	DWORD dwSizeDst = ((DWORD)(nWidthDst * nHeightDst)) * 4;
	BYTE* pbyBitsDst32 =  new BYTE[dwSizeDst];
	if(pbyBitsDst32 == NULL) 
		return;
	memset(pbyBitsDst32,  0,  dwSizeDst);
	
	CCharPretreat* cp = new CCharPretreat();
	cp->ThiningDIB(pbyBitsSrc32, x, y, w, h, width, height, pbyBitsDst32);
	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cp->CreateDdb(pDC->m_hDC, nWidthDst, nHeightDst, pbyBitsDst32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, nWidthDst, nHeightDst, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	// 将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  // 位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  // 指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  // DIB句柄
	DWORD dwSize;
	BOOL flag = true;

	// 因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	// 为新的DIB申请内存并填充其BITMAPINFO结构 
	// dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = nWidthDst;
	lpbmi->bmiHeader.biHeight = nHeightDst;
	
	// 对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
	
	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsDst32, nWidthDst, nHeightDst, lpTargetBits, pDib->m_nBitCount);

	// 拷贝位数据
	// dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cp;
	delete[] pbyBitsDst32;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

void CDIBTransformView::CharRecognise(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))		
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) 
		return;
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CCharRecognise* cr = new CCharRecognise();
	int i = 0;
	float * * f;
	f = cr->TZTQ_13(pbyBitsSrc32, x, y, w, h, width, height, m_charRect.size(), 13);
	cr->CodeRecognize( f, m_charRect.size() );

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cr->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);
	delete cr;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

/*****************绘制数字字符外面的矩形框*******************/
void CDIBTransformView::DrawFrame(CDC* pDC, CDib *pDib, CRectLink charRect, unsigned int linewidth, COLORREF color)
{
	CPen pen;
	pen.CreatePen (PS_SOLID,linewidth,color);
	pDC->SelectObject (&pen);
	::SelectObject (*pDC,GetStockObject(NULL_BRUSH));
	CRect rect,rect2;
	LPBITMAPINFO lpDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	while(!charRect.empty())
		{
			//从表头上得到一个矩形
			rect2 = rect = charRect.front();
			//从链表头上面删掉一个
			charRect.pop_front();
			pDC->Rectangle(&rect);
		}
	//::GlobalUnlock ((HGLOBAL)hDIB);
}

void CDIBTransformView::CharPretreat(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);	
	if (!pDib->m_hDib)
		return;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (!lpSrcDIB)
		return;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//限制处理的区域----防止任意填写数据而使用访问无效
	if((x > (width - 1)) || (y > (height - 1)))		
		return;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	if(pbyBitsSrc32 == NULL) 
		return;
	memset(pbyBitsSrc32, 0, dwSize);

	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	CCharPretreat* cp = new CCharPretreat();
	cp->SlopeAdjust(pbyBitsSrc32, x, y, w, h, width, height);
	m_charRect = cp->CharSegment(pbyBitsSrc32, x, y, w, h, width, height);
	cp->StdDIBbyRect(pbyBitsSrc32, x, y, w, h, width, height, m_charRect);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = cp->CreateDdb(pDC->m_hDC, width, height, pbyBitsSrc32);
	HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
	::DeleteObject(hBitmap);
	memDC.SelectObject(hOldBitmap);

	//将所有非32位位图转换为24位位图存储
	LPBITMAPINFO lpbmi = NULL;						  //位图数据BITMAPINFO结构地址(指针)
	LPSTR lpResult;							  //指向位图数据的指针
	DWORD dwTargetBitsSize,dwTargetHeaderSize;
	HDIB hNewDIB;							  //DIB句柄
	BOOL flag = true;

	//因为要将非32位位图转换为24位位图，所以调色板大小一律为0
	DWORD dwColorTableSize = 0;
	//为新的DIB申请内存并填充其BITMAPINFO结构 
	//dwTargetHeaderSize表示放缩后的DIB图象的BITMAPINFO结构的大小
	dwTargetHeaderSize = sizeof(BITMAPINFOHEADER) + dwColorTableSize;
	lpbmi = (LPBITMAPINFO)malloc(dwTargetHeaderSize);
	memcpy(lpbmi,lpSrcDIB,dwTargetHeaderSize);
	lpbmi->bmiHeader.biWidth = width;
	lpbmi->bmiHeader.biHeight = height;
	
	//对于非32位位图全部作为24位位图处理
	if( pDib->m_nBitCount != 32 )
	{
		if( pDib->m_nBitCount != 24 )		
			flag = false;	// 这句是为了掩盖一个小Bug
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  24);
		lpbmi->bmiHeader.biBitCount = 24;
		pDib->m_nBitCount = 24;
		lpbmi->bmiHeader.biClrUsed = 0;
	}
	else
		dwTargetBitsSize = lpbmi->bmiHeader.biHeight * pDib->GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  pDib->m_nBitCount);
	lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;

	BYTE* lpTargetBits =  new BYTE[dwTargetBitsSize];
	pDib->Format32DDBtoDiB(pbyBitsSrc32, width, height, lpTargetBits, pDib->m_nBitCount);
	//拷贝位数据
	//dwSize表示BITMAPINFO结构加上数据区的大小
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND,dwSize);
	lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy(lpResult,lpbmi,dwTargetHeaderSize);
	memcpy(pDib->FindDIBBits((LPSTR)lpResult),lpTargetBits,dwTargetBitsSize);
	free(lpbmi);
	GlobalUnlock(pDib->m_hDib);
	pDib->m_hDib = hNewDIB;
	delete cp;
	delete[] pbyBitsSrc32;
	ReleaseDC(&dc);
}

bool CDIBTransformView::Black_Back(CDC *pDC,  CDib *pDib,  int x,  int y)
{
	ASSERT(pDib);
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(pDib->m_hDib);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;
	//实际处理的宽度和高度
	int w = min(width, width - x);
	int h = min(height, height - y);
	//32位颜色数据
	DWORD dwSize = width * height * 4;
	//分配全局内存 32位源数据
	BYTE* pbyBitsSrc32 = new BYTE[dwSize];
	memset(pbyBitsSrc32, 0, dwSize);
	//CDib类只提供了一个全部数据的函数
	pDib->GetDdbData32(pbyBitsSrc32,pDib->m_hDib);
	int i, j;
	int black = 0;
	BYTE* lpSrc;
	long OneLineBytes = (long)w * 4;//图像每行的字节数
	long total = w * h;//总的像素点

	for( i = 0; i < h; i++ )
		for( j = 0; j < w; j++ )
		{
			lpSrc = pbyBitsSrc32 + OneLineBytes *  i + 4 * j;
			if( *lpSrc == 0 )
				black++;
			if( black >= (float)total / 2 )
				return true;
			}
	return false;
}
