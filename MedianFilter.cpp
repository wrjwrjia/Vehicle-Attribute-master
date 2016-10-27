#include "stdafx.h"
#include "MedianFilter.h"

IMPLEMENT_DYNAMIC(CMedianFilter, CImageAreaProcess)
CMedianFilter::CMedianFilter()
{
	m_dwOperation = IMAGE_BLUR_MEDIAN_FILTER;
	m_pnMask = NULL;
	m_nMaskBit = 0;
}

CMedianFilter::~CMedianFilter()
{
	if(m_pnMask)delete[] m_pnMask;
	m_pnMask = NULL;
}

#ifdef _DEBUG
void CMedianFilter::Dump(CDumpContext& dc) const
{
	CImageAreaProcess::Dump(dc);
}

void CMedianFilter::AssertValid() const
{
	CImageAreaProcess::AssertValid();
}
#endif

//设置蒙板
void CMedianFilter::SetMask(const int *pnMask,  int nRows,  int nCols)
{
	//保证为奇数
	m_nRows = 2 * (nRows / 2) + 1;
	m_nCols = 2 * (nCols / 2) + 1;

	if(m_pnMask)delete[] m_pnMask;
	m_pnMask = new int[m_nRows * m_nCols];
	int i;
	m_nMaskBit = 0;
	for( i = 0;i < m_nRows * m_nCols;i++)
		m_pnMask[i] = 0;
	
	for(i = 0;i < nRows;i++)
	{
		for(int j = 0; j < nCols;j++)
		{
			int index = m_nCols * i + j;
			m_pnMask[index] = pnMask[nCols * i + j];
			if(m_pnMask[index] != 0) 
				m_nMaskBit++;
		}
	}

}


//快速分类方法, 递归调用
//nFirst, nLast表示数组序号
void CMedianFilter::QuickSort(BYTE* pbyArray,  int nFirst,  int nLast)
{
	int nLeft = nFirst;
	int nRight = nLast;
	if(nFirst >= nLast) return;

	BYTE byMiddle = pbyArray[(nFirst + nLast) / 2];
	do
	{
		while(pbyArray[nLeft] < byMiddle)nLeft++;
		while(pbyArray[nRight] > byMiddle)nRight--;
		if(nLeft <= nRight)
		{
			int nTemp = pbyArray[nLeft];
			pbyArray[nLeft++] = pbyArray[nRight];
			pbyArray[nRight--] = nTemp;
		}
	
	}while(nLeft <= nRight);

	QuickSort(pbyArray, nFirst, nRight);
	QuickSort(pbyArray, nLeft, nLast);
}

//处理边界
PIXELCOLORRGB CMedianFilter::FilterPixelOnBorder(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//存储一个像素邻域的RGB三分量
	BYTE* pbyRed = new BYTE[m_nMaskBit];
	BYTE* pbyGreen = new BYTE[m_nMaskBit];
	BYTE* pbyBlue = new BYTE[m_nMaskBit];

	//邻域中的左上角点.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//三个数组pnRed, pnGreen, pnBlue的索引值
	int index = 0;

	//用于在蒙板中寻找非0数, 逐个进行
	int nOneByOne = 0;


	//用嵌套循环获取小邻域数据.
	
	for(int i = 0;i < m_nRows;i++)
	{
		//y坐标
		int nY = yy + i;

		//复制边界
		nY = (nY < 0) ? 0 : ((nY > (nScanHeight - 1)) ? (nScanHeight - 1) : nY);

		//指针, 指向行数据
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes; 

		for(int j = 0;j < m_nCols;j++)
		{
			if(m_pnMask[nOneByOne] != 0)
			{
				//x坐标	
				int nX = xx + j;

				//复制边界
				nX = (nX < 0) ? 0 : ((nX > (nScanWidth - 1)) ? (nScanWidth - 1) : nX);
				
				BYTE* pbyRaw = pbySrc + 4 * nX;
				
				//记录颜色分量
				pbyBlue[index] = *pbyRaw++; 
				pbyGreen[index] = *pbyRaw++;
				pbyRed[index] = *pbyRaw++;
				index++;
			}
			nOneByOne++;
		}//end j
	}//end i

	QuickSort(pbyRed, 0, (m_nMaskBit - 1));
	QuickSort(pbyGreen, 0, (m_nMaskBit - 1));
	QuickSort(pbyBlue, 0, (m_nMaskBit - 1));

	//RGB颜色结构, 在 IMG.H 中定义
	PIXELCOLORRGB rgb;

	rgb.red = pbyRed[m_nMaskBit / 2];
	rgb.green = pbyGreen[m_nMaskBit / 2];
	rgb.blue = pbyBlue[m_nMaskBit / 2];

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;

}

//过滤内部
PIXELCOLORRGB CMedianFilter::FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//存储一个像素邻域的RGB三分量
	BYTE* pbyRed = new BYTE[m_nMaskBit];
	BYTE* pbyGreen = new BYTE[m_nMaskBit];
	BYTE* pbyBlue = new BYTE[m_nMaskBit];

	//邻域中的左上角点.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//三个数组pnRed, pnGreen, pnBlue的索引值
	int index = 0;

	//用于在蒙板中寻找非0数, 逐个进行
	int nOneByOne = 0;


	//用嵌套循环获取小邻域数据.
	
	for(int i = 0;i < m_nRows;i++)
	{
		//y坐标
		int nY = yy + i;

		//指针, 指向行数据
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes; 

		for(int j = 0;j < m_nCols;j++)
		{
			if(m_pnMask[nOneByOne] != 0)
			{
				//x坐标	
				int nX = xx + j;

				BYTE* pbyRaw = pbySrc + 4 * nX;
				
				//记录颜色分量
				pbyBlue[index] = *pbyRaw++; 
				pbyGreen[index] = *pbyRaw++;
				pbyRed[index] = *pbyRaw++;
				index++;
			}
			nOneByOne++;
		}//end j
	}//end i

	QuickSort(pbyRed, 0, (m_nMaskBit - 1));
	QuickSort(pbyGreen, 0, (m_nMaskBit - 1));
	QuickSort(pbyBlue, 0, (m_nMaskBit - 1));

	//RGB颜色结构, 在 IMG.H 中定义
	PIXELCOLORRGB rgb;
	rgb.red = pbyRed[m_nMaskBit / 2];
	rgb.green = pbyGreen[m_nMaskBit / 2];
	rgb.blue = pbyBlue[m_nMaskBit / 2];

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;

}

//中值过滤
BOOL CMedianFilter::Medianize(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	//第一步, 参数合法性检测
	ASSERT(lpbyBits32);
	ASSERT(m_pnMask);
	if(m_nMaskBit == 0) return FALSE;

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//建立一份拷贝
	BYTE* pbySrcCopy =  new BYTE[ (dwWidthBytes * nScanHeight) ];
	if(pbySrcCopy == NULL) return FALSE;
	::CopyMemory(pbySrcCopy, lpbyBits32, dwWidthBytes * nScanHeight);

	int i, j;

	//处理上下边界
	//上下边界的高度:
	int nBorderH = (m_nRows - 1) / 2;
	for(i = 0;i < nBorderH;i++)
	{
		//当前像素y坐标
		int yy = y + i;

		//指向目的地址
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < w;j++)
		{
			//当前像素x坐标
			int xx = x + j;

			//进行卷积操作
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	
	//下边界Y坐标
	int nYBottom = (y + h - 1);
	//处理下边界
	for(i = 0;i < nBorderH;i++)
	{
		//当前像素y坐标
		int yy = nYBottom - i;

		//指向目的地址
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < w;j++)
		{
			//当前像素x坐标
			int xx = x + j;

			//进行卷积操作
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}

	//垂直边界宽度
	int nBorderW = (m_nCols - 1) / 2;

	
	//剩余高度 + 1 + nBorderH: 
	int nRemnantH = (h - nBorderH);
	
	//处理左边界
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//当前像素y坐标
		int yy = y + i;
		//指向目的地址
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;

		for(j = 0;j < nBorderW;j++)
		{
			//当前像素x坐标
			int xx = x + j;
			//进行卷积操作
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}

	//右边界x坐标起点
	int nXRight = x + w - nBorderW; 

	//处理右边界
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//当前像素y坐标
		int yy = y + i;
		//指向目的地址
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * nXRight;

		for(j = 0;j < nBorderW;j++)
		{
			//当前像素x坐标
			int xx = nXRight + j;
			//进行卷积操作
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//写向目的数据缓冲区
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}

	//内部宽度 - nBorderW + 1
	int nInnerW = w - nBorderW;
	
	//处理内部
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//当前像素y坐标
		int yy = y + i;
		//指向目的地址
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;

		for(j = 0;j < nInnerW;j++)
		{
			//当前像素x坐标
			int xx = x + j;

			//进行卷积操作
			PIXELCOLORRGB rgb = FilterPixelInner(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
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

