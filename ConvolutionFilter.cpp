#include "stdafx.h"
#include "ConvolutionFilter.h"

 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CConvolutionFilter, CImageAreaProcess)
CConvolutionFilter::CConvolutionFilter()
{
	//ȱʡ���Ϊ�Զ�������
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

void CConvolutionFilter::SetOperation(DWORD dwOperation)//��������SetOperation()����
{
	m_dwOperation = dwOperation;
	switch(m_dwOperation)
	{
		//��������ƽ��ģ��:
		case IMAGE_BLUR_MEAN_SMOOTH_3:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 9;
			m_pnKernel = new int[9];
			for(int i = 0;i < 9;i++)m_pnKernel[i] = 1;
			break;
		}
		//�������ƽ��ģ��:
		case IMAGE_BLUR_MEAN_SMOOTH_5:
		{
			m_nRows = m_nCols = 5;
			m_nKernelWeight = 25;
			m_pnKernel = new int[25];
			for(int i = 0;i < 25;i++)m_pnKernel[i] = 1;
			break;
		}
		//�������׵�ͨ�˲��⻬����
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
		//������׵�ͨ�˲��⻬����, ע��, ��ֵ��������ר�Ŵ���.
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
		//��ͨ�񻯴���, ������
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
		//�е�ǿ��
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
		//��ǿ��
		case IMAGE_HIGHPASS_EXCESSIVE_SHARPEN:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 1;
			m_pnKernel = new int[9];
			for(int i = 0;i < 9;i++)m_pnKernel[i] = -1;
			m_pnKernel[4] = 9;
			break;
		}
		//��ͨ����񻯴���
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
		//Laplacian ����, ����, ��ǿ��������
		//������
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
		//�жȵ�
		case IMAGE_LAPLACIAN_MODERATE_DETECT:
		{
			m_nRows = m_nCols = 3;
			m_nKernelWeight = 0;
			m_pnKernel = new int[9];
			for(int i = 0;i < 9;i++)m_pnKernel[i] = -1;
			m_pnKernel[4] = 8;
			break;
		}
		
		//���ȵ�, �߶ȵ�
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
		//�ݶȼ��, ��
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
		//�ݶȼ��, ��
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
		//�ݶȼ��, ��
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
		//�ݶȼ��, ��
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
		//�ݶȼ��, ����
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
		//�ݶȼ��, ����
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
		//�ݶȼ��, ����
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
		//�ݶȼ��, ����
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
		
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ��
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ��
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ��
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ��
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ����
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ����
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ����
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
		//ƽ�ƺͲ�ֱ�Ե��ǿ, ����
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
		//Prewitt��Ե��ǿ, ��
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
		//Prewitt��Ե��ǿ, ��
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
		//Prewitt��Ե��ǿ, ��
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
		//Prewitt��Ե��ǿ, ��
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
		//Prewitt��Ե��ǿ, ����
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
		//Prewitt��Ե��ǿ, ����
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
		//Prewitt��Ե��ǿ, ����
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
		//Prewitt��Ե��ǿ, ����
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
		//ˮƽ�߼��
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
		//��ֱ�߼��
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
		//��Խ��߼��,  45��, б��Ϊ-1 
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
		//�ҶԽ��߼��,  45��, б��Ϊ1
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

void CConvolutionFilter::SetKernel(const int *pnKernel,  int nRows,  int nCols)//���þ����
{
	m_dwOperation = IMAGE_GENERAL_CONVOLUTION_FILTER;
	//��֤Ϊ����
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

//��ʱ��, ����Ծ����Լ�����һ��Ȩֵ�������, ����������ṩ���������
//����, ���������SetKernel()��SetOperation()֮������.
void CConvolutionFilter::SetKernelWeight(int nKernelWeight)
{
	m_nKernelWeight = nKernelWeight;
}

//x, y, nWidth,  int nHeight, ��������Ŀ��
//nScanWidth,  int nScanHeight, ɨ���Ⱥ�ɨ��߶�
//lpbyBits32----����Դ����ֵ, ���ؽ��ֵ
BOOL CConvolutionFilter::Filter(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	//��һ��, �����Ϸ��Լ��
	ASSERT(lpbyBits32);
	ASSERT(m_pnKernel);
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	BYTE* pbySrcCopy =  new BYTE[ (dwWidthBytes * nScanHeight) ];
	if(pbySrcCopy == NULL) return FALSE;

	::CopyMemory(pbySrcCopy, lpbyBits32, dwWidthBytes * nScanHeight);

	int i, j;

	//�������±߽�
	//���±߽�ĸ߶�:
	int nBorderH = (m_nRows - 1) / 2;
	for(i = 0;i < nBorderH;i++)
	{
		int yy = y + i;//��ǰ����y����
		//���ı�Դ����
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < w;j++)
		{
			int xx = x + j;//��ǰ����x����
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);//���о������
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nYBottom = (y + h - 1);//�±߽�Y����
	//�����±߽�
	for(i = 0;i < nBorderH;i++)
	{
		int yy = nYBottom - i;//��ǰ����y����
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;//ָ��Ŀ�ĵ�ַ
		for(j = 0;j < w;j++)
		{
			int xx = x + j;//��ǰ����x����
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);//���о������
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nBorderW = (m_nCols - 1) / 2;//��ֱ�߽���
	int nRemnantH = (h - nBorderH);//ʣ��߶� + 1 + nBorderH: 
	//������߽�
	for(i = nBorderH; i < nRemnantH;i++)
	{
		int yy = y + i;//��ǰ����y����
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;//ָ��Ŀ�ĵ�ַ
		for(j = 0;j < nBorderW;j++)
		{
			int xx = x + j;//��ǰ����x����
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nXRight = x + w - nBorderW; //�ұ߽�x�������
	//�����ұ߽�
	for(i = nBorderH; i < nRemnantH;i++)
	{
		int yy = y + i;//��ǰ����y����
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * nXRight;//ָ��Ŀ�ĵ�ַ
		for(j = 0;j < nBorderW;j++)
		{
			int xx = nXRight + j;//��ǰ����x����
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	int nInnerW = w - nBorderW;	//�ڲ���� - nBorderW + 1
	//�����ڲ�
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//��ǰ����y����
		int yy = y + i;
		//ָ��Ŀ�ĵ�ַ
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < nInnerW;j++)
		{
			int xx = x + j;//��ǰ����x����
			PIXELCOLORRGB rgb = FilterPixelInner(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);//���о������
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	delete[] pbySrcCopy;
	return TRUE;
}

//60%�ļ���ʱ�䶼���������������:
//ʱ�俪����Ϊ������:
//��һ, ���㺯����;���, ���øú����ĵ��ȿ���
//�����Ա����
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

//lpbyBitsSrc32----Դ����ֵ
//x, y��ǰ���صľ��Ե�ַ, �������������(0, 0)���.
//nScanWidth,  int nScanHeight, ɨ���Ⱥ�ɨ��߶�
PIXELCOLORRGB CConvolutionFilter::FilterPixelOnBorder(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	int nNum = m_nRows * m_nCols;//�����Ԫ�ص��ܸ���;
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;//���ֽ���
	//�洢һ�����������RGB������
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];
	//�����е����Ͻǵ�.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);
	int index = 0;//��������pnRed, pnGreen, pnBlue������ֵ
	//��Ƕ��ѭ����ȡС��������.
	for(int i = 0;i < m_nRows;i++)
	{
		int nY = yy + i;//y����
		nY = (nY < 0) ? 0 : ((nY > (nScanHeight - 1)) ? (nScanHeight - 1) : nY);//���Ʊ߽�
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes; //ָ��, ָ��������
		for(int j = 0;j < m_nCols;j++)
		{
			int nX = xx + j;//x����	
			nX = (nX < 0) ? 0 : ((nX > (nScanWidth - 1)) ? (nScanWidth - 1) : nX);//���Ʊ߽�
			BYTE* pbyRaw = pbySrc + 4 * nX;
			//��¼��ɫ����
			pbyBlue[index] = *pbyRaw++; 
			pbyGreen[index] = *pbyRaw++;
			pbyRed[index] = *pbyRaw++;
			index++;
		}
	}
	//RGB��ɫ�ṹ, �� IMG.H �ж���
	//������
	PIXELCOLORRGB rgb = Convolute(pbyRed, pbyGreen, pbyBlue, nNum);
	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}

//��FilterPixelOnBorder()�������, �ú���ֻ�����˼����ж�������.
PIXELCOLORRGB CConvolutionFilter::FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	int nNum = m_nRows * m_nCols;//�����Ԫ�ص��ܸ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//�洢һ�����������RGB������
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];
	//�����е����Ͻǵ�.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);
	int index = 0;//��������pnRed, pnGreen, pnBlue������ֵ
	//��Ƕ��ѭ����ȡС��������.
	for(int i = 0;i < m_nRows;i++)
	{
		int nY = yy + i;//y����
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes + 4 * xx;//ָ��, ָ��������
		for(int j = 0;j < m_nCols;j++)
		{
			//x����	
			//��¼��ɫ����
			pbyBlue[index] = *pbySrc++; 
			pbyGreen[index] = *pbySrc++;
			pbyRed[index] = *pbySrc++;
			pbySrc++;
			index++;
		}
	}
	//������
	PIXELCOLORRGB rgb = Convolute(pbyRed, pbyGreen, pbyBlue, nNum);
	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}


