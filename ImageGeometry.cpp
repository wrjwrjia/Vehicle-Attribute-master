//图像几何变换

#include "stdafx.h"
#include "ImageGeometry.h"
#include "math.h"

IMPLEMENT_DYNAMIC(CImageGeometry, CImageProcess)
CImageGeometry::CImageGeometry()
{
	m_dwOperation = IMAGE_GEOMETRY_TRANSLATE;
	m_dwQuality = IMAGE_GEOMETRY_NEAREST_NEIGHBOR_INTERPOLATE;
}

CImageGeometry::~CImageGeometry()
{

}

#ifdef _DEBUG
void CImageGeometry::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImageGeometry::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif


void CImageGeometry::SetQuality(DWORD dwQuality)
{
	m_dwQuality = dwQuality;
}

///lpbySrcXY----传递源像素(x, y)的地址, 
//x, y, 经过反向变换后得到的对应于原图像的象点的坐标
//nScanWidth,  int nScanHeight, 源扫描宽度和扫描高度
PIXELCOLORRGB CImageGeometry::Interpolate(LPBYTE lpbySrcXY,  int x,  int y,  float fu,  float fv,  int nScanWidth,  int nScanHeight)
{ // 插值函数
	PIXELCOLORRGB rgb;

	//行字节数, 可以将dwWidthBytes作为参数传递过来
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	
	switch(m_dwQuality)
	{
		case IMAGE_GEOMETRY_NEAREST_NEIGHBOR_INTERPOLATE :
		{
			BYTE* pbySrc = lpbySrcXY;
			rgb.blue = *pbySrc++;
			rgb.green = *pbySrc++;
			rgb.red = *pbySrc++;
			break;
		}
		case IMAGE_GEOMETRY_BILINEAR_INTERPOLATE :
		{
			//相邻的四个像素最右下角点的x, y坐标偏移量
			int nx = 1;
			int ny = 1;
			if((x + 1) > (nScanWidth - 1))
				nx = 0;
			if((y + 1) > (nScanHeight - 1))
				ny = 0;
			
			//相邻四个像素的像素值
			BYTE abyRed[2][2], abyGreen[2][2], abyBlue[2][2];
			
			//像素点(x, y)的数据位置
			BYTE* pbySrc = lpbySrcXY;
			//获取像素数值
			abyBlue[0][0] = *pbySrc++;
			abyGreen[0][0] = *pbySrc++;
			abyRed[0][0] = *pbySrc++;
			
			pbySrc = (lpbySrcXY + nx * 4);
			abyBlue[1][0] = *pbySrc++;
			abyGreen[1][0] = *pbySrc++;
			abyRed[1][0] = *pbySrc++;

			
			//指向下一行数据
			BYTE* pbySrcTemp = (lpbySrcXY + ny * dwWidthBytes);

			pbySrc = pbySrcTemp;
			abyBlue[0][1] = *pbySrc++;
			abyGreen[0][1] = *pbySrc++;
			abyRed[0][1] = *pbySrc++;

			pbySrc = pbySrcTemp + (4 * nx);
			abyBlue[1][1] = *pbySrc++;
			abyGreen[1][1] = *pbySrc++;
			abyRed[1][1] = *pbySrc++;
			
			rgb.red = (BYTE)(BOUND(((1 - fu) * (1 - fv) * ((float)abyRed[0][0]) + (1 - fu) * fv * ((float)abyRed[0][1]) + fu * (1 - fv) * ((float)abyRed[1][0]) + fu * fv * ((float)abyRed[1][1])), 0, 255));
			rgb.green = (BYTE)(BOUND(((1 - fu) * (1 - fv) * ((float)abyGreen[0][0]) + (1 - fu) * fv * ((float)abyGreen[0][1]) + fu * (1 - fv) * ((float)abyGreen[1][0]) + fu * fv * ((float)abyGreen[1][1])), 0, 255));
			rgb.blue = (BYTE)(BOUND(((1 - fu) * (1 - fv) * ((float)abyBlue[0][0]) + (1 - fu) * fv * ((float)abyBlue[0][1]) + fu * (1 - fv) * ((float)abyBlue[1][0]) + fu * fv * ((float)abyBlue[1][1])), 0, 255));
			break;
		}
		
		case IMAGE_GEOMETRY_THREE_ORDER_INTERPOLATE :
		{
			//像素坐标
			int xx[4], yy[4];
			//相邻四个像素的像素值
			BYTE abyRed[4][4], abyGreen[4][4], abyBlue[4][4];

			xx[0] = -1;  xx[1] = 0; xx[2] = 1; xx[3] = 2;
			yy[0] = -1;  yy[1] = 0; yy[2] = 1; yy[3] = 2;

			//保证合法
			if((x - 1) < 0) 
				xx[0] = 0;
			if((x + 1) > (nScanWidth - 1)) 
				xx[2] = 0;
			if((x + 2) > (nScanWidth - 1)) 
				xx[3] = ((xx[2] == 0) ? 0 : 1);

			if((y - 1) < 0) 
				yy[0] = 0;
			if((y + 1) > (nScanHeight - 1)) 
				yy[2] = 0;
			if((y + 2) > (nScanHeight - 1)) 
				yy[3] = ((yy[2] == 0) ? 0 : 1);

			//像素点(x, y)的数据位置
			//获取数据
			int i;
			for(i = 0;i < 4;i++)
			{
				//像素点(x, y)的数据位置
				BYTE* pbySrcBase = lpbySrcXY + yy[i] * dwWidthBytes;

				for(int j = 0;j < 4;j++)
				{
					BYTE* pbySrc = pbySrcBase + 4 * xx[j];
					abyBlue[i][j] = *pbySrc++;
					abyGreen[i][j] = *pbySrc++;
					abyRed[i][j] = *pbySrc++;
				}
			}

			//u, v向量
			float afu[4], afv[4];

			afu[0] = Sinxx(1.0f + fu);
			afu[1] = Sinxx(fu);
			afu[2] = Sinxx(1.0f - fu);
			afu[3] = Sinxx(2.0f - fu);

			afv[0] = Sinxx(1.0f + fv);
			afv[1] = Sinxx(fv);
			afv[2] = Sinxx(1.0f - fv);
			afv[3] = Sinxx(2.0f - fv);

			//矩阵乘向量的中间值
			float afRed[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			float afGreen[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			float afBlue[4] = {0.0f, 0.0f, 0.0f, 0.0f};

			for(i = 0;i < 4;i++)
			{
				for(int j = 0;j < 4;j++)
				{
					afRed[i] += afv[j] * abyRed[j][i];
					afGreen[i] += afv[j] * abyGreen[j][i];
					afBlue[i] += afv[j] * abyBlue[j][i];				
				}
			}
			rgb.red = (BYTE)(BOUND((afu[0] * afRed[0] + afu[1] * afRed[1] + afu[2] * afRed[2] + afu[3] * afRed[3]), 0, 255));
			rgb.green = (BYTE)(BOUND((afu[0] * afGreen[0] + afu[1] * afGreen[1] + afu[2] * afGreen[2] + afu[3] * afGreen[3]), 0, 255));
			rgb.blue = (BYTE)(BOUND((afu[0] * afBlue[0] + afu[1] * afBlue[1] + afu[2] * afBlue[2] + afu[3] * afBlue[3]), 0, 255));
			break;
		}
		default : break;
	}
	return rgb;
}

//该函数计算sin(x) / (x)
float CImageGeometry::Sinxx(float fx)
{
	float fabsx = ABS(fx);
	float fxx = fabsx * fabsx;
	float fxxx = fxx * fabsx;
	if(fabsx < 1.0f) 
		return (1.0f - 2.0f * fxx + fxxx);
	else if(fabsx < 2.0f) 
		return (4.0f - 8.0f * fabsx + 5.0f * fxx - fxxx); 
	else 
		return 0.0f;
}

//lpbyBitsSrc32----传递源像素值, 
//x, y, nWidth,  nHeight, 定义区域的宽度
//nScanWidth,  nScanHeight, 扫描宽度和扫描高度
//lpbyBitsDst32----存放最后缩放后的结果
//nWidthImgDst, nHeightImgDst缩放至多大
BOOL CImageGeometry::Scale(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32, int nWidthImgDst, int nHeightImgDst)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);
	ASSERT(nWidthImgDst != 0);
	ASSERT(nHeightImgDst != 0);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;

	m_dwOperation = IMAGE_GEOMETRY_SCALE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//注意事项:
	//第一:
	//如果(w <  nWidth), 或者(h <  nHeight)则表示指的区域比能够有效获取数据的区域要大, 
	//这时程序将放大倍数, 使最后缩放的结果总能达到 nWidthImgDst 宽和 nHeightImgDst 高
	//第二:	
	//fScalex, fScaley所表示的缩放比为真实缩放比的倒数
	//之所以这样处理是由于, 作一次除法, 总比一次乘法要慢.

	//宽度缩放比
	float fScalex = (float)w/(float)nWidthImgDst;
	float fScaley = (float)h/(float)nHeightImgDst;

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//指向目标数据
	BYTE* pbyDst = lpbyBitsDst32;
	//完成变换
	for(int i = 0; i < nHeightImgDst; i++)
	{
		//反向变换后获得的浮点y值
		float fYInverse = i * fScaley;
		//取整
		int yy = (int)fYInverse;
		//坐标差值
		float fv = fYInverse - yy;
		//对应于原图像的y坐标
		yy += y;
		BYTE* pbySrc = lpbyBitsSrc32 + yy * dwWidthBytes;

		for(int j = 0;j < nWidthImgDst;j++)
		{
			//反向变换后获得的浮点x值
			float fXInverse = j * fScalex;
			//取整
			int xx = (int)fXInverse;
			//坐标差值
			float fu = fXInverse - xx;
			//对应于原图像的y坐标
			xx += x;	
			//获取数据
			BYTE* pbyCurrent =  pbySrc + 4 * xx;
			PIXELCOLORRGB rgb = Interpolate(pbyCurrent, xx, yy, fu, fv, nScanWidth,  nScanHeight);
			
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
	
			//alpha数据由外部填充.
			pbyDst++;	
		}
	}
	return TRUE;
}

//顺时针旋转90度
//包括 n * 360 - 90 度 
//x, y, nWidth,  nHeight-----定义子区域
//lpbyBitsDst32----大小为nWidth * nHeight * 4;
//nScanWidth-------源图像扫描宽度
//nScanHeight------源图像扫描高度
BOOL CImageGeometry::RotateCW90(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_CW_90;

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;		//No 10

	//指定区域最后一行第一列处的数据地址.
	BYTE* pbySrcBase = lpbyBitsSrc32 + (y + nHeight - 1) * dwWidthBytes + 4 * x;
	
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nWidth;i++)
	{
		//从底向上按列获取数据
		//逐渐向右移动
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nHeight;j++)
		{
			
			BYTE* pbyCurrent = pbySrc;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			pbyDst++;
			pbySrc -= dwWidthBytes;
		}
		pbySrcBase += 4;
	}
	return TRUE;
}

//逆时针旋转90度
//lpbyBitsDst32----大小为nWidth * nHeight * 4;
BOOL CImageGeometry::RotateCCW90(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_CCW_90;

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;


	//指定区域第一行最后一列开始地址.
	BYTE* pbySrcBase = lpbyBitsSrc32 + y * dwWidthBytes + 4 * (x + nWidth - 1);
	
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nWidth;i++)
	{
		//从上向下按列获取数据
		//逐渐向左移动
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nHeight;j++)
		{
			
			BYTE* pbyCurrent = pbySrc;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			pbyDst++;
			pbySrc += dwWidthBytes;
		}
		pbySrcBase -= 4;
	}
	return TRUE;
}

BOOL CImageGeometry::Rotate180(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_180;

	//指定区域最后一行最后一列开始地址.
	BYTE* pbySrcBase = lpbyBitsSrc32 + (y + nHeight - 1) * dwWidthBytes + 4 * (x + nWidth - 1);
	
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nHeight;i++)
	{
		//逐渐向左移动
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nWidth;j++)
		{
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			pbyDst++;
			pbySrc -= 7;
		}
		pbySrcBase -= dwWidthBytes;
	}
	return TRUE;
}

//旋转..., -360, 0, 360, ...度
BOOL CImageGeometry::Rotate0(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_0;
	//行字节数(源图像)
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//行字节数(目标子图像)
	DWORD dwWidtgBytesDst = (DWORD)nWidth * 4;
	//指定区域第一行第一列开始地址.
	BYTE* pbySrcBase = lpbyBitsSrc32 + y * dwWidthBytes + 4 * x;
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0; i < nHeight; i++)
	{
		::CopyMemory(pbyDst, pbySrcBase, dwWidtgBytesDst); 
		pbySrcBase += dwWidthBytes;
		pbyDst += dwWidtgBytesDst;
	}
	return TRUE;
}


//lpbyBitsSrc32----传递源像素值, 
//x, y, nWidth,  nHeight, 定义区域的宽度
//nScanWidth,  nScanHeight, 扫描宽度和扫描高度
//lpbyBitsDst32----存放最后缩放后的结果
//fTheta ---- 旋转角度
//nWidthImgDst, nHeightDst旋转后的尺寸, 事实上它们可以通过nWidth,  nHeight和fTheta计算出来.
BOOL CImageGeometry::Rotate(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32, float fTheta, int nWidthImgDst, int nHeightImgDst)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度.
	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE;
	//将角度转化成弧度
	float fRadTheta = (float)(fTheta * PIE / 180.0);
	//计算其三角函数值
	float fSinTheta = (float)sin(fRadTheta);
	float fCosTheta = (float)cos(fRadTheta);

	//首先考查是否是旋转特殊的角度.
	if((nWidthImgDst == nHeight) && (nHeightImgDst == nWidth))
	{
		// ...,  -270,  90,  450, ..., 
		if(fSinTheta > 0.99f)
			return RotateCCW90(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
		// ...,  -450,  -90,  270, ..., 
		else if(fSinTheta < -0.99f)
			return RotateCW90(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
	}
	if((nWidthImgDst == nWidth) && (nHeightImgDst == nHeight))
	{
		// ..., -540, -180, 180, 540, ..., 
		if(fCosTheta < -0.99f)
			return Rotate180(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
		// ..., -360, 0, 360, 
		else if (fCosTheta > 0.99f)
			return Rotate0(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
	}

	//处理非特殊角度

	//绝对旋转中心, 相对于原图像坐标系统.
	int nAbsCenterX = x + nWidth / 2;
	int nAbsCenterY = y + nHeight / 2;
	//相对旋转中心, 它们具有相同的x, y坐标.相对于目标图像
	int nRelCenterX = nWidthImgDst / 2;
	int nRelCenterY = nHeightImgDst / 2;
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;

	//开始计算
	//按行进行扫描计算
	for(int i = 0;i < nHeightImgDst;i++)//y
	{
		//右手直角坐标系下, 坐标值为 i 的纵坐标表示
		int yy = nRelCenterY - i;
		for(int j = 0; j < nWidthImgDst;j++)//x
		{
			//右手直角坐标系下, 坐标值为 j 的横坐标表示
			int xx = j - nRelCenterX;
			//进行反向变换, 旋转角度: -theta
			float fXInverse = xx * fCosTheta + yy * fSinTheta;
			float fYInverse = yy * fCosTheta - xx * fSinTheta;

			//变成左手坐标系
			fYInverse = -fYInverse;
			fXInverse += nAbsCenterX;
			fYInverse += nAbsCenterY;

			//近邻坐标值
			int nNearX = (int)fXInverse;
			int nNearY = (int)fYInverse;
			if((nNearX >= x) && (nNearX <= (x + nWidth - 1)) && (nNearY >= y) && (nNearY <= (y + nHeight - 1)))
			{
				//插值参数
				float fu = fXInverse - nNearX;
				float fv = fYInverse - nNearY;
				//近邻点的地址
				BYTE* pbySrc = lpbyBitsSrc32 + nNearY * dwWidthBytes + 4 * nNearX;
				PIXELCOLORRGB rgb = Interpolate(pbySrc, nNearX, nNearY, fu, fv, nScanWidth,  nScanHeight);
				*pbyDst++ = rgb.blue;
				*pbyDst++ = rgb.green;
				*pbyDst++ = rgb.red;
				pbyDst++;
			}
			else
				pbyDst += 4;
		}
	}
	return TRUE;
}

//水平镜象,但是镜象轴为 Y 轴
BOOL CImageGeometry::MirrorY(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度
	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}
	m_dwOperation = IMAGE_GEOMETRY_MIRROR_Y;
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//指定区域最后一行最后一列开始地址.
	BYTE* pbySrcBase = lpbyBitsSrc32 + y * dwWidthBytes + 4 * (x + nWidth - 1);
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nHeight;i++)
	{
		//从上向下按列获取数据
		//逐渐向左移动
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nWidth;j++)
		{
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			pbyDst++;
			pbySrc -= 7;
		}
		pbySrcBase += dwWidthBytes;
	}
	return TRUE;
}

//垂直镜象,但是镜象轴为 X 轴
BOOL CImageGeometry::MirrorX(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//有效区域的宽度和高度
	//请注意, 这时, 宽度和高度不得随意超过有效宽度和高度.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_MIRROR_X;

	//行字节数(源图像)
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//行字节数(目标子图像)
	DWORD dwWidtgBytesDst = (DWORD)nWidth * 4;
	//指定区域最后一行第一列开始地址.
	BYTE* pbySrcBase = lpbyBitsSrc32 + (y + nHeight - 1) * dwWidthBytes + 4 * x;
	//目的地地址
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nHeight;i++)
	{
		//从下向向按行获取数据
		::CopyMemory(pbyDst, pbySrcBase, dwWidtgBytesDst); 
		pbyDst += dwWidtgBytesDst;
		pbySrcBase -= dwWidthBytes;
	}
	return TRUE;
}
