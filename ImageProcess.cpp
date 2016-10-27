#include "stdafx.h"
#include "ImageProcess.h"

IMPLEMENT_DYNAMIC(CImageProcess,  CObject)
CImageProcess::CImageProcess()
{
	m_dwOperation = 0L;	//缺省
	m_dwColorModel = IMAGE_COLOR_RGBA;
}

CImageProcess::~CImageProcess()
{

}

#ifdef _DEBUG
void CImageProcess::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

void CImageProcess::AssertValid() const
{
	CObject::AssertValid();
}
#endif

//设置图像处理操作(虚拟)
void CImageProcess::SetOperation(DWORD dwOperation)
{
	m_dwOperation = dwOperation;
}


//设置颜色模型
//注意, 我们将只处理RGBA颜色模型
void CImageProcess::SetColorModel(DWORD dwColorModel)
{
	m_dwColorModel = dwColorModel;
}

DWORD CImageProcess::GetColorModel() const
{
	return m_dwColorModel;
}

DWORD CImageProcess::GetOperation() const
{
	return m_dwOperation;
}



//从一个32位无符号整数中获取颜色分量
BYTE CImageProcess::GetAlpha(DWORD dwRGBA)
{
	//四字节，顺序为A--B--G--R.
	//如果不是这一顺序, 请按重载该函数
	return (BYTE)((dwRGBA & 0xFF000000) >> 24); 	
}

BYTE CImageProcess::GetBlue(DWORD dwRGBA)
{
	return (BYTE)((dwRGBA & 0x000000FF) >> 16);
}

BYTE CImageProcess::GetGreen(DWORD dwRGBA)
{
	return (BYTE)((dwRGBA & 0x0000FF00) >> 8);
}
	
BYTE CImageProcess::GetRed(DWORD dwRGBA)
{
	return (BYTE)(dwRGBA & 0x00FF0000);
}

DWORD CImageProcess::PutRGBA(BYTE byRed,  BYTE byGreen,  BYTE byBlue,  BYTE byAlpha)
{
	return ((((DWORD)byAlpha) << 24) | 
		   (((DWORD)byRed) << 16)	|
		   (((DWORD)byGreen << 8)) |
		   ((DWORD)byBlue)) ;
}

DWORD CImageProcess::PutRGB(BYTE byRed,  BYTE byGreen,  BYTE byBlue)
{
	//不透明处理
	return ( 0xFF000000 | 
		   (((DWORD)byRed) << 16)	|
		   (((DWORD)byGreen) << 8) |
		   ((DWORD)byBlue)) ;
}

//从32位DDB数据直接创建一个DDB对象, 即HBITMAP对象
HBITMAP CImageProcess::CreateDdb(HDC hDC,  int nWidth,  int nHeight,  const LPBYTE lpbyBits32)
{
	//第一步, 创建一个BITMAPV4HEADER结构

	//分配内存:
	BYTE* pbyBits = new BYTE[sizeof(BITMAPV4HEADER)];
	if(pbyBits == NULL) return NULL;
	//指向Dib实际像素数据的指针, 
	BITMAPV4HEADER* pbmih = (BITMAPV4HEADER*)pbyBits;

	pbmih->bV4Size = sizeof(BITMAPV4HEADER);	//108
	pbmih->bV4Width = (LONG)nWidth;
	pbmih->bV4Height = (LONG)(-nHeight);
	pbmih->bV4Planes = 1;
	pbmih->bV4BitCount = 32;

    pbmih->bV4V4Compression = BI_RGB;    
	pbmih->bV4SizeImage = 0;
	pbmih->bV4XPelsPerMeter = 0;    
	pbmih->bV4YPelsPerMeter = 0;
	pbmih->bV4ClrUsed = 0;    
	pbmih->bV4ClrImportant = 0;
 
	//第二步, 创建一个DDB
	HBITMAP hBitmap = ::CreateDIBitmap(hDC, (BITMAPINFOHEADER*)pbmih, CBM_INIT, lpbyBits32, (BITMAPINFO*)pbmih, DIB_RGB_COLORS);

	delete[] pbyBits;
	return hBitmap;
}


