#include "stdafx.h"
#include "ImageIncise.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CImageIncise, CImageProcess)
CImageIncise::CImageIncise()
{
}

CImageIncise::~CImageIncise()
{
}

#ifdef _DEBUG
void CImageIncise::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImageIncise::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif

BOOL CImageIncise::ThresholdIncise(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	// �ú����������32λͼ��lpbyBits32��ָ��32λͼͼ�����ݵ�ָ��
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBits32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//ָ�򻺴�DIBͼ���ָ��
	//ѭ������
	long	i, j;

	//����ֵ
	BYTE	pixel;

	//ֱ��ͼ����
	long	Histogram[256];

	//��ֵ
	BYTE	Threshold, NewThreshold;

	//���Ҷ�ֵ����С�Ҷ�ֵ,���������ƽ���Ҷ�ֵ
	BYTE	MaxGrayValue, MinGrayValue, MeanGrayValue1, MeanGrayValue2;

	//���ڼ�������Ҷ�ƽ��ֵ���м����
	long	Temp1, Temp2, Temp3, Temp4;

	//��������
	int		Times;
	
	//ͼ����������
	BYTE* lpSrc;

	//ͼ��ÿ�е��ֽ���
	long	OneLineBytes = (long)nScanWidth * 4;

	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * OneLineBytes + 4 * x;

	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;
	
	//��ʼ��ֱ��ͼ����Ϊ0
	for( i = 0; i < 256; i++ )
	{
		Histogram[i] = 0;
	}
	
	//���ֱ��ͼ
	//��ʼ�������С�Ҷ�ֵ
	MaxGrayValue = 0;
	MinGrayValue = 255;

	//ÿ��
	for( i = 0; i < nHeight; i++ )
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		//ÿ��
		for( j = 0; j < nWidth; j++ )
		{
			//����ֵ
			lpSrc++;
			lpSrc++;
			pixel = (BYTE)*lpSrc;
			lpSrc++;
			//ͳ�Ƹ����ص���ֵĴ���
			Histogram[pixel]++;

			//�������С�Ҷ�ֵ
			if( MinGrayValue > pixel )
				MinGrayValue = pixel;
			if( MaxGrayValue < pixel )
				MaxGrayValue = pixel;
			lpSrc++;
		}
		dwBaseIndex += OneLineBytes;
	}

	//�����������ֵ
	NewThreshold = (MinGrayValue + MaxGrayValue) / 2;
	Threshold = 0;

	//����������Ϊ50�Σ����Ը����Լ�����Ҫ�趨
	for( Times = 0; Threshold != NewThreshold && Times < 50; Times++ )
	{
		Threshold = NewThreshold;
		Temp1 = 0;
		Temp2 = 0;
		Temp3 = 0;
		Temp4 = 0;

		//���1������ĻҶȼ�Ȩƽ��ֵ
		for( i = MinGrayValue; i < Threshold; i++ )
		{
			Temp1 += Histogram[i] * i;
			Temp3 += Histogram[i];
		}
		MeanGrayValue1 = (BYTE)(Temp1 / Temp3);

		//��ڶ�������ĻҶȼ�Ȩƽ��ֵ
		for( i = Threshold; i <= MaxGrayValue; i++ )
		{
			Temp2 += Histogram[i] * i;
			Temp4 += Histogram[i];
		}
		MeanGrayValue2 = (BYTE)(Temp2 / Temp4);

		//����ֵ�޸�Ϊ����õ���ֵ
		NewThreshold = (MeanGrayValue1 + MeanGrayValue2) / 2;
	}

	dwBaseIndex = y * OneLineBytes + 4 * x;

	//������ֵ��ͼ���ֵ��
	//ÿ��
	for( i = 0; i < nHeight; i++ )
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		//ÿ��
		for( j = 0; j < nWidth; j++ )
		{
			//����ֵ
			pixel = (BYTE)*lpSrc;
			if( pixel <= Threshold )
			{
				*lpSrc++ = (BYTE)0;
				*lpSrc++ = (BYTE)0;
				*lpSrc++ = (BYTE)0;
				*lpSrc++;
			}
			else
			{
				*lpSrc++ = (BYTE)255;
				*lpSrc++ = (BYTE)255;
				*lpSrc++ = (BYTE)255;
				*lpSrc++;
			}
		}
		dwBaseIndex += OneLineBytes;
	}
	return true;
}