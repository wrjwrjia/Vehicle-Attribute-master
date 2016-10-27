#include "stdafx.h"
#include "ConvolutionFilter.h"

 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CConvolutionFilter, CImageAreaProcess)
CConvolutionFilter::CConvolutionFilter()
{
	//缺省情况为自定义卷积核
	m_dwOperation = IMAGE_GENERAL_CONVOLUTION_FILTER;
	m_nKernelWeight = 0;
	m_pnKernel = NULL;
}

CConvolutionFilter::~CConvolutionFilter()
{
	if(m_pnKernel)delete[] m_pnKernel;
	m_pnKernel = NULL;
}

#ifdef _DEBUG
void CConvolutionFilter::Dump(CDumpContext& dc) const
{
	CImageAreaProcess::Dump(dc);
}

void CConvolutionFilter::AssertValid() const
{
	CImageAreaProcess::AssertValid();
}
#endif

void CConvolutionFilter::SetOperation(DWORD dwOperation)//重载虚拟SetOperation()函数
{
	m_dwOperation = dwOperation;
	switch(m_dwOperation)
	{
		//三阶算术平均模糊:
		case IMAGE_BLUR_MEAN_SMOOTH_3:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 9;
			m_pnKernel = new int[9];
			for(int i = 0;i < 9;i++)m_pnKernel[i] = 1;
			break;
		}
		//五阶算术平均模糊:
		case IMAGE_BLUR_MEAN_SMOOTH_5:
		{
			m_nRows = m_nCols = 5;
			m_nKernelWeight = 25;
			m_pnKernel = new int[25];
			for(int i = 0;i < 25;i++)m_pnKernel[i] = 1;
			break;
		}
		//基本三阶低通滤波光滑处理
		case IMAGE_LOWPASS_NOISE_REMOVE_3:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 16;
	 		m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = m_pnKernel[6] = m_pnKernel[8] = 1;
			m_pnKernel[1] = m_pnKernel[3] = m_pnKernel[5] = m_pnKernel[7] = 2;
			m_pnKernel[4] = 4;
			break;
		}
		//基本五阶低通滤波光滑处理, 注意, 中值过滤另外专门处理.
		case IMAGE_LOWPASS_NOISE_REMOVE_5:
		{
			m_nRows = m_nCols = 5;
			m_nKernelWeight = 60;
			m_pnKernel = new int[25];
			for(int i = 0;i < 25;i++)m_pnKernel[i] = 1;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 4;
			m_pnKernel[16] = m_pnKernel[17] = m_pnKernel[18] = 4;
			m_pnKernel[11] = m_pnKernel[13] = 4;
			m_pnKernel[12] = 12;
			break;
		}
		//高通锐化处理, 基本锐化
		case IMAGE_HIGHPASS_BASIC_SHARPEN:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 7;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = m_pnKernel[6] = m_pnKernel[8] = -1;
			m_pnKernel[1] = m_pnKernel[3] = m_pnKernel[5] = m_pnKernel[7] = -2;
			m_pnKernel[4] = 19;
			break;
		}
		//中等强度
		case IMAGE_HIGHPASS_MODERATE_SHARPEN:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 1;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = m_pnKernel[6] = m_pnKernel[8] = 0;
			m_pnKernel[1] = m_pnKernel[3] = m_pnKernel[5] = m_pnKernel[7] = -1;
			m_pnKernel[4] = 5;
			break;
		}
		//高强度
		case IMAGE_HIGHPASS_EXCESSIVE_SHARPEN:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 1;
			m_pnKernel = new int[9];
			for(int i = 0;i < 9;i++)m_pnKernel[i] = -1;
			m_pnKernel[4] = 9;
			break;
		}
		//高通五阶锐化处理
		case IMAGE_HIGHPASS_OUTLINE_SHARPEN:
		{
			m_nRows = m_nCols = 5;
			m_nKernelWeight = -7;
			m_pnKernel = new int[25];
			m_pnKernel[0] = m_pnKernel[4] = m_pnKernel[20] = m_pnKernel[24] = 0;
			m_pnKernel[1] = m_pnKernel[2] = m_pnKernel[3] = -1;
			m_pnKernel[5] = m_pnKernel[10] = m_pnKernel[15] = -1;
			m_pnKernel[9] = m_pnKernel[14] = m_pnKernel[19] = -1;
			m_pnKernel[21] = m_pnKernel[22] = m_pnKernel[23] = -1;
			m_pnKernel[6] = m_pnKernel[8] = m_pnKernel[16] = m_pnKernel[18] = 2;
			m_pnKernel[7] = m_pnKernel[11] = m_pnKernel[13] = m_pnKernel[17] = -4;
			m_pnKernel[12] = 13;
			break;
		}
		//Laplacian 过滤, 三阶, 增强不连续性
		//基本的
		case IMAGE_LAPLACIAN_BASIC_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = m_pnKernel[6] = m_pnKernel[8] = 0;
			m_pnKernel[1] = m_pnKernel[3] = m_pnKernel[5] = m_pnKernel[7] = -1;
			m_pnKernel[4] = 4;
			break;
		}
		//中度的
		case IMAGE_LAPLACIAN_MODERATE_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			for(int i = 0;i < 9;i++)m_pnKernel[i] = -1;
			m_pnKernel[4] = 8;
			break;
		}
		
		//过度的, 高度的
		case IMAGE_LAPLACIAN_EXCESSIVE_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = m_pnKernel[6] = m_pnKernel[8] = 1;
			m_pnKernel[1] = m_pnKernel[3] = m_pnKernel[5] = m_pnKernel[7] = -2;
			m_pnKernel[4] = 4;
			break;
		}
		//梯度检测, 南
		case IMAGE_GRADIENT_SOUTH_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = -1;  m_pnKernel[1] = -2;
			m_pnKernel[3] = m_pnKernel[4] = m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[8] = 1;   m_pnKernel[7] = 2;
			break;		
		}
		//梯度检测, 东
		case IMAGE_GRADIENT_EAST_DETECT:
		{
		    m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[6] = -1;  m_pnKernel[3] = -2;
			m_pnKernel[1] = m_pnKernel[4] = m_pnKernel[7] = 0;
			m_pnKernel[2] = m_pnKernel[8] = 1;   m_pnKernel[5] = 2;
			break;
		}
		//梯度检测, 西
		case IMAGE_GRADIENT_WEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[6] = 1;  m_pnKernel[3] = 2;
			m_pnKernel[1] = m_pnKernel[4] = m_pnKernel[7] = 0;
			m_pnKernel[2] = m_pnKernel[8] = -1;   m_pnKernel[5] = -2;
			break;		
		}
		//梯度检测, 北
		case IMAGE_GRADIENT_NORTH_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = 1;  m_pnKernel[1] = 2;
			m_pnKernel[3] = m_pnKernel[4] = m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[8] = -1;   m_pnKernel[7] = -2;
			break;		
		}
		//梯度检测, 西南
		case IMAGE_GRADIENT_SOUTHWEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[1] = m_pnKernel[5] = -1;  m_pnKernel[2] = -2;
			m_pnKernel[0] = m_pnKernel[4] = m_pnKernel[8] = 0;
			m_pnKernel[3] = m_pnKernel[7] = 1;   m_pnKernel[6] = 2;
			break;		
		}
		//梯度检测, 东北
		case IMAGE_GRADIENT_NORTHEAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[1] = m_pnKernel[5] = 1;    m_pnKernel[2] = 2;
			m_pnKernel[0] = m_pnKernel[4] = m_pnKernel[8] = 0;
			m_pnKernel[3] = m_pnKernel[7] = -1;   m_pnKernel[6] = -2;
			break;			
		}
		//梯度检测, 东南
		case IMAGE_GRADIENT_SOUTHEAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = -2;  m_pnKernel[1] = m_pnKernel[3] = -1;  
			m_pnKernel[2] = m_pnKernel[4] = m_pnKernel[6] = 0;
			m_pnKernel[5] = m_pnKernel[7] = 1;   m_pnKernel[8] = 2;
			break;			
		}
		//梯度检测, 西北
		case IMAGE_GRADIENT_NORTHWEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = 2;  m_pnKernel[1] = m_pnKernel[3] = 1;  
			m_pnKernel[2] = m_pnKernel[4] = m_pnKernel[6] = 0;
			m_pnKernel[5] = m_pnKernel[7] = -1;   m_pnKernel[8] = -2;
			break;			
		}
		
		//平移和差分边缘增强, 西
		case IMAGE_DIFFERENCE_WEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 0;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = -1;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 0;
			break;					
		}
		//平移和差分边缘增强, 北
		case IMAGE_DIFFERENCE_NORTH_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 0;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[8] = 0; m_pnKernel[7] = -1;
			break;					
		}
		//平移和差分边缘增强, 东
		case IMAGE_DIFFERENCE_EAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 0;
			m_pnKernel[3] = -1; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 0;
			break;					
		}
		//平移和差分边缘增强, 南
		case IMAGE_DIFFERENCE_SOUTH_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[2] = 0; m_pnKernel[1] = -1;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 0;
			break;					
		}
		//平移和差分边缘增强, 西南
		case IMAGE_DIFFERENCE_SOUTHWEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = 0; m_pnKernel[2] = -1;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 0;
			break;					
		}
		//平移和差分边缘增强, 东北
		case IMAGE_DIFFERENCE_NORTHEAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 0;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = -1; m_pnKernel[7] = m_pnKernel[8] = 0;
			break;					
		}
		//平移和差分边缘增强, 东南
		case IMAGE_DIFFERENCE_SOUTHEAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = -1; m_pnKernel[1] = m_pnKernel[2] = 0;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 0;
			break;					
		}
		//平移和差分边缘增强, 西北
		case IMAGE_DIFFERENCE_NORTHWEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 0;
			m_pnKernel[3] = 0; m_pnKernel[4] = 1; m_pnKernel[5] = 0;
			m_pnKernel[6] = m_pnKernel[7] = 0; m_pnKernel[8] = -1;
			break;					
		}
		//Prewitt边缘增强, 西
		case IMAGE_PREWITT_WEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = 1; m_pnKernel[2] = -1;
			m_pnKernel[3] = 1; m_pnKernel[4] = -2; m_pnKernel[5] = -1;
			m_pnKernel[6] = m_pnKernel[7] = 1; m_pnKernel[8] = -1;
			break;					
		}
		//Prewitt边缘增强, 北
		case IMAGE_PREWITT_NORTH_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 1;
			m_pnKernel[3] = 1; m_pnKernel[4] = -2; m_pnKernel[5] = 1;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = -1;
			break;				
		}
		//Prewitt边缘增强, 东
		case IMAGE_PREWITT_EAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = -1; m_pnKernel[1] = m_pnKernel[2] = 1;
			m_pnKernel[3] = -1; m_pnKernel[4] = -2; m_pnKernel[5] = 1;
			m_pnKernel[6] = -1; m_pnKernel[7] = m_pnKernel[8] = 1;
			break;				
		}
		//Prewitt边缘增强, 南
		case IMAGE_PREWITT_SOUTH_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = -1;
			m_pnKernel[3] = 1; m_pnKernel[4] = -2; m_pnKernel[5] = 1;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 1;
			break;					
		}
		//Prewitt边缘增强, 西北
		case IMAGE_PREWITT_SOUTHWEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = 1; m_pnKernel[1] = m_pnKernel[2] = -1;
			m_pnKernel[3] = 1; m_pnKernel[4] = -2; m_pnKernel[5] = -1;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 1;
			break;					
		}
		//Prewitt边缘增强, 东北
		case IMAGE_PREWITT_NORTHEAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 1;
			m_pnKernel[3] = -1; m_pnKernel[4] = -2; m_pnKernel[5] = 1;
			m_pnKernel[6] = m_pnKernel[7] = -1; m_pnKernel[8] = 1;
			break;					
		}
		//Prewitt边缘增强, 东南
		case IMAGE_PREWITT_SOUTHEAST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = -1; m_pnKernel[2] = 1;
			m_pnKernel[3] = -1; m_pnKernel[4] = -2; m_pnKernel[5] = 1;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = 1;
			break;					
		}
		//Prewitt边缘增强, 西北
		case IMAGE_PREWITT_NORTHWEST_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = 1;
			m_pnKernel[3] = 1; m_pnKernel[4] = -2; m_pnKernel[5] = -1;
			m_pnKernel[6] = 1; m_pnKernel[7] = m_pnKernel[8] = -1;
			break;					
		}
		//水平线检测
		case IMAGE_LINE_HORIZONTAL_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[2] = -1;
			m_pnKernel[3] = m_pnKernel[4] = m_pnKernel[5] = 2;
			m_pnKernel[6] = m_pnKernel[7] = m_pnKernel[8] = -1;
			break;			
		}
		//垂直线检测
		case IMAGE_LINE_VERTICAL_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[3] = m_pnKernel[6] = -1;
			m_pnKernel[1] = m_pnKernel[4] = m_pnKernel[7] = 2;
			m_pnKernel[2] = m_pnKernel[5] = m_pnKernel[8] = -1;
			break;			
		}
		//左对角线检测,  45度, 斜率为-1 
		case IMAGE_LINE_LEFT_DIAGONAL_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[1] = m_pnKernel[2] = m_pnKernel[3] = -1;
			m_pnKernel[0] = m_pnKernel[4] = m_pnKernel[8] = 2;
			m_pnKernel[5] = m_pnKernel[6] = m_pnKernel[7] = -1;
			break;		
		}
		//右对角线检测,  45度, 斜率为1
		case IMAGE_LINE_RIGHT_DIAGONAL_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			m_pnKernel[0] = m_pnKernel[1] = m_pnKernel[3] = -1;
			m_pnKernel[2] = m_pnKernel[4] = m_pnKernel[6] = 2;
			m_pnKernel[5] = m_pnKernel[7] = m_pnKernel[8] = -1;
			break;		
		}
		default : break;
	}
}

void CConvolutionFilter::SetKernel(const int *pnKernel,  int nRows,  int nCols)//设置卷积核
{
	m_dwOperation = IMAGE_GENERAL_CONVOLUTION_FILTER;
	//保证为奇数
	m_nRows = 2 * (nRows / 2) + 1;
	m_nCols = 2 * (nCols / 2) + 1;

	if(m_pnKernel)delete[] m_pnKernel;
	m_pnKernel = new int[m_nRows * m_nCols];
	int i;
	for( i = 0;i < m_nRows * m_nCols;i++)m_pnKernel[i] = 0;
	
	m_nKernelWeight = 0;
	for(i = 0;i < nRows;i++)
	{
		for(int j = 0; j < nCols;j++)
		{
			int index = m_nCols * i + j;
			m_pnKernel[index] = pnKernel[nCols * i + j];
			m_nKernelWeight += m_pnKernel[index];
		}
	}
}

//有时候, 你可以觉得自己设置一个权值更加灵活, 这个函数就提供了这个功能
//但是, 你必须是在SetKernel()和SetOperation()之后用它.
void CConvolutionFilter::SetKernelWeight(int nKernelWeight)
{
	m_nKernelWeight = nKernelWeight;
}

//x, y, nWidth,  int nHeight, 定义区域的宽度
//nScanWidth,  int nScanHeight, 扫描宽度和扫描高度
//lpbyBits32----传递源像素值, 回载结果值
BOOL CConvolutionFilter::Filter(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	//第一步, 参数合法性检测
	ASSERT(lpbyBits32);
	ASSERT(m_pnKernel);
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	BYTE* pbySrcCopy =  new BYTE[ (dwWidthBytes * nScanHeight) ];
	if(pbySrcCopy == NULL) return FALSE;

	::CopyMemory(pbySrcCopy, lpbyBits32, dwWidthBytes * nScanHeight);

	int i, j;

	//处理上下边界
	//上下边界的高度:
	int nBorderH = (m_nRows - 1) / 2;
	for(i = 0;i < nBorderH;i++)
	{
		int yy = y + i;//当前像素y坐标
		//将改变源数据
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < w;j++)
		{
			int xx = x + j;//当前像素x坐标
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);//进行卷积操作
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nYBottom = (y + h - 1);//下边界Y坐标
	//处理下边界
	for(i = 0;i < nBorderH;i++)
	{
		int yy = nYBottom - i;//当前像素y坐标
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;//指向目的地址
		for(j = 0;j < w;j++)
		{
			int xx = x + j;//当前像素x坐标
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);//进行卷积操作
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nBorderW = (m_nCols - 1) / 2;//垂直边界宽度
	int nRemnantH = (h - nBorderH);//剩余高度 + 1 + nBorderH: 
	//处理左边界
	for(i = nBorderH; i < nRemnantH;i++)
	{
		int yy = y + i;//当前像素y坐标
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;//指向目的地址
		for(j = 0;j < nBorderW;j++)
		{
			int xx = x + j;//当前像素x坐标
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nXRight = x + w - nBorderW; //右边界x坐标起点
	//处理右边界
	for(i = nBorderH; i < nRemnantH;i++)
	{
		int yy = y + i;//当前像素y坐标
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * nXRight;//指向目的地址
		for(j = 0;j < nBorderW;j++)
		{
			int xx = nXRight + j;//当前像素x坐标
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nInnerW = w - nBorderW;	//内部宽度 - nBorderW + 1
	//处理内部
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//当前像素y坐标
		int yy = y + i;
		//指向目的地址
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < nInnerW;j++)
		{
			int xx = x + j;//当前像素x坐标
			PIXELCOLORRGB rgb = FilterPixelInner(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);//进行卷积操作
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	delete[] pbySrcCopy;
	return TRUE;
}

//60%的计算时间都花在这个函数上面:
//时间开销分为两部分:
//其一, 运算函数体;其二, 调用该函数的调度开销
//虚拟成员函数
PIXELCOLORRGB CConvolutionFilter::Convolute(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue,  int nNum)
{
	int i, nSumRed, nSumGreen, nSumBlue;
	nSumRed = nSumGreen = nSumBlue = 0;
	for(i = 0 ; i < nNum;i++)
	{
		nSumRed += pbyRed[i] * m_pnKernel[i];
		nSumGreen += pbyGreen[i] * m_pnKernel[i];
		nSumBlue += pbyBlue[i] * m_pnKernel[i];
	}
	if((m_nKernelWeight != 0) && (m_nKernelWeight != 1))
	{
		nSumRed /= m_nKernelWeight;
		nSumGreen /= m_nKernelWeight;
		nSumBlue /= m_nKernelWeight;
	}
	PIXELCOLORRGB rgb;
	rgb.red = (BYTE)(BOUND(nSumRed, 0, 255));
	rgb.green = (BYTE)(BOUND(nSumGreen, 0, 255));
	rgb.blue = (BYTE)(BOUND(nSumBlue, 0, 255));
	return rgb;
}

//lpbyBitsSrc32----源像素值
//x, y当前像素的绝对地址, 其坐标是相对于(0, 0)点的.
//nScanWidth,  int nScanHeight, 扫描宽度和扫描高度
PIXELCOLORRGB CConvolutionFilter::FilterPixelOnBorder(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	int nNum = m_nRows * m_nCols;//卷积核元素的总个数;
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;//行字节数
	//存储一个像素邻域的RGB三分量
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];
	//邻域中的左上角点.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);
	int index = 0;//三个数组pnRed, pnGreen, pnBlue的索引值
	//用嵌套循环获取小邻域数据.
	for(int i = 0;i < m_nRows;i++)
	{
		int nY = yy + i;//y坐标
		nY = (nY < 0) ? 0 : ((nY > (nScanHeight - 1)) ? (nScanHeight - 1) : nY);//复制边界
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes; //指针, 指向行数据
		for(int j = 0;j < m_nCols;j++)
		{
			int nX = xx + j;//x坐标	
			nX = (nX < 0) ? 0 : ((nX > (nScanWidth - 1)) ? (nScanWidth - 1) : nX);//复制边界
			BYTE* pbyRaw = pbySrc + 4 * nX;
			//记录颜色分量
			pbyBlue[index] = *pbyRaw++; 
			pbyGreen[index] = *pbyRaw++;
			pbyRed[index] = *pbyRaw++;
			index++;
		}
	}
	//RGB颜色结构, 在 IMG.H 中定义
	//计算卷积
	PIXELCOLORRGB rgb = Convolute(pbyRed, pbyGreen, pbyBlue, nNum);
	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}

//与FilterPixelOnBorder()函数相比, 该函数只是少了几个判断语句而已.
PIXELCOLORRGB CConvolutionFilter::FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	int nNum = m_nRows * m_nCols;//卷积核元素的总个数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//存储一个像素邻域的RGB三分量
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];
	//邻域中的左上角点.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);
	int index = 0;//三个数组pnRed, pnGreen, pnBlue的索引值
	//用嵌套循环获取小邻域数据.
	for(int i = 0;i < m_nRows;i++)
	{
		int nY = yy + i;//y坐标
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes + 4 * xx;//指针, 指向行数据
		for(int j = 0;j < m_nCols;j++)
		{
			//x坐标	
			//记录颜色分量
			pbyBlue[index] = *pbySrc++; 
			pbyGreen[index] = *pbySrc++;
			pbyRed[index] = *pbySrc++;
			pbySrc++;
			index++;
		}
	}
	//计算卷积
	PIXELCOLORRGB rgb = Convolute(pbyRed, pbyGreen, pbyBlue, nNum);
	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}


