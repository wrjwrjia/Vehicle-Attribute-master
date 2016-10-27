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
	// 该函数处理的是32位图，lpbyBits32是指向32位图图像数据的指针
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBits32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//指向缓存DIB图像的指针
	//循环变量
	long	i, j;

	//像素值
	BYTE	pixel;

	//直方图数组
	long	Histogram[256];

	//阈值
	BYTE	Threshold, NewThreshold;

	//最大灰度值与最小灰度值,两个区域的平均灰度值
	BYTE	MaxGrayValue, MinGrayValue, MeanGrayValue1, MeanGrayValue2;

	//用于计算区域灰度平均值的中间变量
	long	Temp1, Temp2, Temp3, Temp4;

	//迭代次数
	int		Times;
	
	//图像数据索引
	BYTE* lpSrc;

	//图像每行的字节数
	long	OneLineBytes = (long)nScanWidth * 4;

	//开始数据基索引
	DWORD dwBaseIndex = y * OneLineBytes + 4 * x;

	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;
	
	//初始化直方图数组为0
	for( i = 0; i < 256; i++ )
	{
		Histogram[i] = 0;
	}
	
	//获得直方图
	//初始化最大、最小灰度值
	MaxGrayValue = 0;
	MinGrayValue = 255;

	//每行
	for( i = 0; i < nHeight; i++ )
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		//每列
		for( j = 0; j < nWidth; j++ )
		{
			//像素值
			lpSrc++;
			lpSrc++;
			pixel = (BYTE)*lpSrc;
			lpSrc++;
			//统计各像素点出现的次数
			Histogram[pixel]++;

			//求最大最小灰度值
			if( MinGrayValue > pixel )
				MinGrayValue = pixel;
			if( MaxGrayValue < pixel )
				MaxGrayValue = pixel;
			lpSrc++;
		}
		dwBaseIndex += OneLineBytes;
	}

	//迭代求最佳阈值
	NewThreshold = (MinGrayValue + MaxGrayValue) / 2;
	Threshold = 0;

	//最大迭代次数为50次，可以根据自己的需要设定
	for( Times = 0; Threshold != NewThreshold && Times < 50; Times++ )
	{
		Threshold = NewThreshold;
		Temp1 = 0;
		Temp2 = 0;
		Temp3 = 0;
		Temp4 = 0;

		//求第1个区域的灰度加权平均值
		for( i = MinGrayValue; i < Threshold; i++ )
		{
			Temp1 += Histogram[i] * i;
			Temp3 += Histogram[i];
		}
		MeanGrayValue1 = (BYTE)(Temp1 / Temp3);

		//求第二个区域的灰度加权平均值
		for( i = Threshold; i <= MaxGrayValue; i++ )
		{
			Temp2 += Histogram[i] * i;
			Temp4 += Histogram[i];
		}
		MeanGrayValue2 = (BYTE)(Temp2 / Temp4);

		//将阈值修改为新求得的阈值
		NewThreshold = (MeanGrayValue1 + MeanGrayValue2) / 2;
	}

	dwBaseIndex = y * OneLineBytes + 4 * x;

	//根据阈值将图像二值化
	//每行
	for( i = 0; i < nHeight; i++ )
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		//每列
		for( j = 0; j < nWidth; j++ )
		{
			//像素值
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