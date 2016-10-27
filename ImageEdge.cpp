#include "stdafx.h"
#include "ImageEdge.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CImageEdge, CImageProcess)
CImageEdge::CImageEdge()
{
}

CImageEdge::~CImageEdge()
{
}

#ifdef _DEBUG
void CImageEdge::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImageEdge::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif

BOOL CImageEdge::FindJumpPoint(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{// 该函数处理的是32位图，lpbyBits32是指向32位图图像数据的指针
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBits32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;

	//循环变量
	long	i, j;

	//前一个(左方的)像素点
	BYTE	prepixel;

	//当前像素点
	BYTE	nowpixel;
	
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

	//每行
	for( i = 0; i < nHeight; i++ )
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		prepixel = (BYTE)255;
		nowpixel = (BYTE)255;

		//每列
		for( j = 0; j < nWidth; j++ )
		{
			//像素值
			nowpixel = (BYTE)*lpSrc;
			//如果该像素点是黑色，保留
			if( nowpixel == 0 )
			{
				*lpSrc++;
				*lpSrc++;
				*lpSrc++;
				*lpSrc++;
				prepixel = (BYTE)0;
			}
			//如果该像素点是白色
			else if( nowpixel == 255 )
				//如果左方像素点是黑色，保留
				if( prepixel == 0 )
				{
					*lpSrc++;
					*lpSrc++;
					*lpSrc++;
					*lpSrc++;
					prepixel = (BYTE)255;
				}
				 //如果左方像素点是白色，当前像素点设为黑色
				else if( prepixel == 255 )
				{
					*lpSrc++ = (BYTE)0;
					*lpSrc++ = (BYTE)0;
					*lpSrc++ = (BYTE)0;
					*lpSrc++;
					prepixel = (BYTE)255;
				}
		}
		dwBaseIndex += OneLineBytes;
	}
	return true;
}