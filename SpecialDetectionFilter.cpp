#include "stdafx.h"
#include "SpecialDetectionFilter.h"

IMPLEMENT_DYNAMIC(CSpecialDetectionFilter, CImageAreaProcess)
CSpecialDetectionFilter::CSpecialDetectionFilter()
{
	m_dwOperation = IMAGE_SOBEL_EDGE_DETECT;
	m_nCols = 3;
	m_nRows = 3;
}

CSpecialDetectionFilter::~CSpecialDetectionFilter()
{
}

#ifdef _DEBUG
void CSpecialDetectionFilter::Dump(CDumpContext &dc) const
{
	CImageAreaProcess::Dump(dc);
}

void CSpecialDetectionFilter::AssertValid() const
{
	CImageAreaProcess::AssertValid();
}
#endif

PIXELCOLORRGB  CSpecialDetectionFilter::Sobel(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue, int nNum)
{
	int r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;

	r0 = ABS(( ( (int)pbyRed[0] + (int)pbyRed[1] + (int)pbyRed[2] ) - ( (int)pbyRed[6] + (int)pbyRed[7] + (int)pbyRed[8] ) ) / 3);
	g0 = ABS(( ( (int)pbyGreen[0] + (int)pbyGreen[1] + (int)pbyGreen[2] ) - ( (int)pbyGreen[6] + (int)pbyGreen[7] + (int)pbyGreen[8] ) ) / 3);
	b0 = ABS(( ( (int)pbyBlue[0] + (int)pbyBlue[1] + (int)pbyBlue[2] ) - ( (int)pbyBlue[6] + (int)pbyBlue[7] + (int)pbyBlue[8] ) ) / 3);
	
	r1 = ABS(( ( (int)pbyRed[0] + (int)pbyRed[3] + (int)pbyRed[6] ) - ( (int)pbyRed[2] + (int)pbyRed[5] + (int)pbyRed[8] ) ) / 3);
	g1 = ABS(( ( (int)pbyGreen[0] + (int)pbyGreen[3] + (int)pbyGreen[6] ) - ( (int)pbyGreen[2] + (int)pbyGreen[5] + (int)pbyGreen[8] ) ) / 3);
	b1 = ABS(( ( (int)pbyBlue[0] + (int)pbyBlue[3] + (int)pbyBlue[6] ) - ( (int)pbyBlue[2] + (int)pbyBlue[5] + (int)pbyBlue[8] ) ) / 3);

	r2 = ABS(( ( (int)pbyRed[0] + (int)pbyRed[1] + (int)pbyRed[3] ) - ( (int)pbyRed[5] + (int)pbyRed[7] + (int)pbyRed[8] ) ) / 3);
	g2 = ABS(( ( (int)pbyGreen[0] + (int)pbyGreen[1] + (int)pbyGreen[3] ) - ( (int)pbyGreen[5] + (int)pbyGreen[7] + (int)pbyGreen[8] ) ) / 3);
	b2 = ABS(( ( (int)pbyBlue[0] + (int)pbyBlue[1] + (int)pbyBlue[3] ) - ( (int)pbyBlue[5] + (int)pbyBlue[7] + (int)pbyBlue[8] ) ) / 3);

	r3 = ABS(( ( (int)pbyRed[1] + (int)pbyRed[2] + (int)pbyRed[5] ) - ( (int)pbyRed[3] + (int)pbyRed[6] + (int)pbyRed[7] ) ) / 3);
	g3 = ABS(( ( (int)pbyGreen[1] + (int)pbyGreen[2] + (int)pbyGreen[5] ) - ( (int)pbyGreen[3] + (int)pbyGreen[6] + (int)pbyGreen[7] ) ) / 3);
	b3 = ABS(( ( (int)pbyBlue[1] + (int)pbyBlue[2] + (int)pbyBlue[5] ) - ( (int)pbyBlue[3] + (int)pbyBlue[6] + (int)pbyBlue[7] ) ) / 3);
	
	PIXELCOLORRGB rgb;
	rgb.red = (BYTE)max(r0, max(r1,max(r2,r3)));
	rgb.green = (BYTE)max(g0, max(g1,max(g2,g3)));
	rgb.blue = (BYTE)max(b0, max(b1,max(b2,b3)));
	return rgb;
}

PIXELCOLORRGB  CSpecialDetectionFilter::Kirsch(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue, int nNum)
{
	int	nRed[8], nGreen[8], nBlue[8];

	nRed[0] = ( 5 * ((int)pbyRed[0] + (int)pbyRed[1] + (int)pbyRed[2]) - 3 * ((int)pbyRed[3] + (int)pbyRed[5] + (int)pbyRed[6] + (int)pbyRed[7] + (int)pbyRed[8]) );
	nGreen[0] = ( 5 * ((int)pbyGreen[0] + (int)pbyGreen[1] + (int)pbyGreen[2]) - 3 * ((int)pbyGreen[3] + (int)pbyGreen[5] + (int)pbyGreen[6] + (int)pbyGreen[7] + (int)pbyGreen[8]) );
	nBlue[0] = ( 5 * ((int)pbyBlue[0] + (int)pbyBlue[1] + (int)pbyBlue[2]) - 3 * ((int)pbyBlue[3] + (int)pbyBlue[5] + (int)pbyBlue[6] + (int)pbyBlue[7] + (int)pbyBlue[8]) );
	
	nRed[1] = ( 5 * ((int)pbyRed[1] + (int)pbyRed[2] + (int)pbyRed[5]) - 3 * ((int)pbyRed[0] + (int)pbyRed[3] + (int)pbyRed[6] + (int)pbyRed[7] + (int)pbyRed[8]) );
	nGreen[1] = ( 5 * ((int)pbyGreen[1] + (int)pbyGreen[2] + (int)pbyGreen[5]) - 3 * ((int)pbyGreen[0] + (int)pbyGreen[3] + (int)pbyGreen[6] + (int)pbyGreen[7] + (int)pbyGreen[8]) );
	nBlue[1] = ( 5 * ((int)pbyBlue[1] + (int)pbyBlue[2] + (int)pbyBlue[5]) - 3 * ((int)pbyBlue[0] + (int)pbyBlue[3] + (int)pbyBlue[6] + (int)pbyBlue[7] + (int)pbyBlue[8]) );

	nRed[2] = ( 5 * ((int)pbyRed[2] + (int)pbyRed[5] + (int)pbyRed[8]) - 3 * ((int)pbyRed[0] + (int)pbyRed[3] + (int)pbyRed[6] + (int)pbyRed[7] + (int)pbyRed[1]) );
	nGreen[2] = ( 5 * ((int)pbyGreen[2] + (int)pbyGreen[5] + (int)pbyGreen[8]) - 3 * ((int)pbyGreen[0] + (int)pbyGreen[3] + (int)pbyGreen[6] + (int)pbyGreen[7] + (int)pbyGreen[1]) );
	nBlue[2] = ( 5 * ((int)pbyBlue[2] + (int)pbyBlue[5] + (int)pbyBlue[8]) - 3 * ((int)pbyBlue[0] + (int)pbyBlue[3] + (int)pbyBlue[6] + (int)pbyBlue[7] + (int)pbyBlue[1]) );

	nRed[3] = ( 5 * ((int)pbyRed[7] + (int)pbyRed[5] + (int)pbyRed[8]) - 3 * ((int)pbyRed[0] + (int)pbyRed[3] + (int)pbyRed[6] + (int)pbyRed[2] + (int)pbyRed[1]) );
	nGreen[3] = ( 5 * ((int)pbyGreen[7] + (int)pbyGreen[5] + (int)pbyGreen[8]) - 3 * ((int)pbyGreen[0] + (int)pbyGreen[3] + (int)pbyGreen[6] + (int)pbyGreen[2] + (int)pbyGreen[1]) );
	nBlue[3] = ( 5 * ((int)pbyBlue[7] + (int)pbyBlue[5] + (int)pbyBlue[8]) - 3 * ((int)pbyBlue[0] + (int)pbyBlue[3] + (int)pbyBlue[6] + (int)pbyBlue[2] + (int)pbyBlue[1]) );

	nRed[4] = ( 5 * ((int)pbyRed[7] + (int)pbyRed[6] + (int)pbyRed[8]) - 3 * ((int)pbyRed[0] + (int)pbyRed[3] + (int)pbyRed[5] + (int)pbyRed[2] + (int)pbyRed[1]) );
	nGreen[4] = ( 5 * ((int)pbyGreen[7] + (int)pbyGreen[6] + (int)pbyGreen[8]) - 3 * ((int)pbyGreen[0] + (int)pbyGreen[3] + (int)pbyGreen[5] + (int)pbyGreen[2] + (int)pbyGreen[1]) );
	nBlue[4] = ( 5 * ((int)pbyBlue[7] + (int)pbyBlue[6] + (int)pbyBlue[8]) - 3 * ((int)pbyBlue[0] + (int)pbyBlue[3] + (int)pbyBlue[5] + (int)pbyBlue[2] + (int)pbyBlue[1]) );

	nRed[5] = ( 5 * ((int)pbyRed[7] + (int)pbyRed[6] + (int)pbyRed[3]) - 3 * ((int)pbyRed[0] + (int)pbyRed[8] + (int)pbyRed[5] + (int)pbyRed[2] + (int)pbyRed[1]) );
	nGreen[5] = ( 5 * ((int)pbyGreen[7] + (int)pbyGreen[6] + (int)pbyGreen[3]) - 3 * ((int)pbyGreen[0] + (int)pbyGreen[8] + (int)pbyGreen[5] + (int)pbyGreen[2] + (int)pbyGreen[1]) );
	nBlue[5] = ( 5 * ((int)pbyBlue[7] + (int)pbyBlue[6] + (int)pbyBlue[3]) - 3 * ((int)pbyBlue[0] + (int)pbyBlue[8] + (int)pbyBlue[5] + (int)pbyBlue[2] + (int)pbyBlue[1]) );

	nRed[6] = ( 5 * ((int)pbyRed[0] + (int)pbyRed[6] + (int)pbyRed[3]) - 3 * ((int)pbyRed[7] + (int)pbyRed[8] + (int)pbyRed[5] + (int)pbyRed[2] + (int)pbyRed[1]) );
	nGreen[6] = ( 5 * ((int)pbyGreen[0] + (int)pbyGreen[6] + (int)pbyGreen[3]) - 3 * ((int)pbyGreen[7] + (int)pbyGreen[8] + (int)pbyGreen[5] + (int)pbyGreen[2] + (int)pbyGreen[1]) );
	nBlue[6] = ( 5 * ((int)pbyBlue[0] + (int)pbyBlue[6] + (int)pbyBlue[3]) - 3 * ((int)pbyBlue[7] + (int)pbyBlue[8] + (int)pbyBlue[5] + (int)pbyBlue[2] + (int)pbyBlue[1]) );

	nRed[7] = ( 5 * ((int)pbyRed[0] + (int)pbyRed[1] + (int)pbyRed[3]) - 3 * ((int)pbyRed[7] + (int)pbyRed[8] + (int)pbyRed[5] + (int)pbyRed[2] + (int)pbyRed[6]) );
	nGreen[7] = ( 5 * ((int)pbyGreen[0] + (int)pbyGreen[1] + (int)pbyGreen[3]) - 3 * ((int)pbyGreen[7] + (int)pbyGreen[8] + (int)pbyGreen[5] + (int)pbyGreen[2] + (int)pbyGreen[6]) );
	nBlue[7] = ( 5 * ((int)pbyBlue[0] + (int)pbyBlue[1] + (int)pbyBlue[3]) - 3 * ((int)pbyBlue[7] + (int)pbyBlue[8] + (int)pbyBlue[5] + (int)pbyBlue[2] + (int)pbyBlue[6]) );

	for( int i = 1; i < 8; i++ )
	{
		if( nRed[i] > nRed[0] )
			nRed[0] = nRed[i];
		if( nGreen[i] > nGreen[0] )
			nGreen[0] = nGreen[i];
		if( nBlue[i] > nBlue[0] )
			nBlue[0] = nBlue[i]; 
	}

	PIXELCOLORRGB rgb;
	rgb.red = (BYTE)BOUND(nRed[0], 0, 255 );
	rgb.green = (BYTE)BOUND(nGreen[0], 0, 255 );
	rgb.blue = (BYTE)BOUND(nBlue[0], 0, 255 );

	return	rgb;
}

PIXELCOLORRGB  CSpecialDetectionFilter::Specialize(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue, int nNum)
{
	PIXELCOLORRGB rgb;
	switch ( m_dwOperation )
	{
		case IMAGE_SOBEL_EDGE_DETECT:
			rgb = Sobel(pbyRed, pbyGreen, pbyBlue, nNum);
			break;
		case IMAGE_KIRSCH_EDGE_DETECT:
			rgb = Kirsch(pbyRed, pbyGreen, pbyBlue, nNum);
			break;
		default: break;
	}
	return rgb;
}

//lpbyBitsSrc32----源像素值
//x, y当前像素的绝对地址, 其坐标是相对于(0, 0)点的.
//nScanWidth,  int nScanHeight, 扫描宽度和扫描高度
PIXELCOLORRGB CSpecialDetectionFilter::FilterPixelOnBorder(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	//卷积核元素的总个数;
	int nNum = m_nRows * m_nCols;

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//存储一个像素邻域的RGB三分量
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];

	//邻域中的左上角点.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//三个数组pnRed, pnGreen, pnBlue的索引值
	int index = 0;

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
	}
	//RGB颜色结构, 在 IMG.H 中定义
	//计算卷积
	PIXELCOLORRGB rgb = Specialize(pbyRed, pbyGreen, pbyBlue, nNum);

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}

//与FilterPixelOnBorder()函数相比, 该函数只是少了几个判断语句而已.
PIXELCOLORRGB CSpecialDetectionFilter::FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	//卷积核元素的总个数;
	int nNum = m_nRows * m_nCols;

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//存储一个像素邻域的RGB三分量
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];

	//邻域中的左上角点.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//三个数组pnRed, pnGreen, pnBlue的索引值
	int index = 0;

	//用嵌套循环获取小邻域数据.
	
	for(int i = 0;i < m_nRows;i++)
	{
		//y坐标
		int nY = yy + i;

		//指针, 指向行数据
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes + 4 * xx;

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
	PIXELCOLORRGB rgb = Specialize(pbyRed, pbyGreen, pbyBlue, nNum);

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}


//x, y, nWidth,  int nHeight, 定义区域的宽度
//nScanWidth,  int nScanHeight, 扫描宽度和扫描高度
//lpbyBits32----传递源像素值, 回载结果值
BOOL CSpecialDetectionFilter::Filter(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	//第一步, 参数合法性检测
	ASSERT(lpbyBits32);
	
	if ( m_dwOperation == IMAGE_SOBEL_EDGE_DETECT || m_dwOperation == IMAGE_KIRSCH_EDGE_DETECT )
		{
			m_nRows = 3;
			m_nCols = 3;
		}

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//建立一份拷贝
	BYTE* pbySrcCopy =  new BYTE[ (dwWidthBytes * nScanHeight) ];
	if(pbySrcCopy == NULL) 
		return FALSE;

	::CopyMemory(pbySrcCopy, lpbyBits32, dwWidthBytes * nScanHeight);

	int i, j;

	//处理上下边界
	//上下边界的高度:
	int nBorderH = (m_nRows - 1) / 2;
	for(i = 0;i < nBorderH;i++)
	{
		//当前像素y坐标
		int yy = y + i;

		//将改变源数据
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