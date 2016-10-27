// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DIBTransform.h"
#include "Dib.h"
#include "Mmsystem.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDib,  CObject)

CDib::CDib()
{
	Init();
}

CDib::CDib(const char* pszDibFileName)
{
	Init();
    LoadFile(pszDibFileName);
}

CDib::~CDib()
{
	ClearMemory();
}

void CDib::ClearMemory()
{
	if(m_hDib != NULL)
		::GlobalFree(m_hDib);
	m_hDib = NULL;		
	m_pszFileName = NULL;
	m_pBitmapInfoHeader = NULL;
    m_pBitmapInfo = NULL;

	m_pRGBQuad = NULL;
	m_pBitmapFileHeader = NULL;
	
	if ( m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
}

#ifdef _DEBUG
void CDib::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

void CDib::AssertValid() const
{
	CObject::AssertValid();
	ASSERT(m_pszFileName != NULL);
	ASSERT(m_hDib != NULL);
}
#endif


void CDib::Init()
{
	m_pRGBQuad = NULL;		//RGBQUAD 表项
	m_nWidth = 0;			//宽度
	m_nHeight = 0;			//高度
	m_nBitCount = 0;		//每个像素占有的位数
    m_nTotalColors = 0;		//颜色总数
    m_pBitmapFileHeader = NULL;
    m_pBitmapInfoHeader = NULL;
    m_pBitmapInfo = NULL;
	m_hDib = NULL;
	m_pszFileName = NULL;
	m_pPalette = NULL;
}

void CDib::LoadFile(const char* pszDibFileName)
{
	ASSERT(pszDibFileName);

	//如果存在就释放内存
	if(m_pBitmapInfo) ::GlobalFree(m_hDib);
	//再次初始化, 用于多次动态重用
	Init();
	m_pszFileName = (char*)pszDibFileName;

	BITMAPFILEHEADER bitmapFileHeader;

	CFile dibFile(pszDibFileName, CFile::modeRead | CFile::shareDenyWrite);
	try
	{
		//读入位图文件头信息
		UINT nBFHsize = dibFile.Read((void*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
		if(nBFHsize != sizeof(BITMAPFILEHEADER))
		{
			m_bValid = FALSE;
			return;
		}
	}
	catch(CFileException* e)
	{
		e->Delete();
	}

	//如果为位图就进行下一步的处理
    if (bitmapFileHeader.bfType == 0x4d42)
    {
		AfxGetApp()->BeginWaitCursor();
		
		//文件长度
        DWORD dwFileLength = dibFile.GetLength();
		
		//位图信息及位图数据的大小(字节)
        DWORD dwSize = dwFileLength - sizeof(BITMAPFILEHEADER);
 

		//为Dib分配全局内存
		m_hDib = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE,  dwSize);
		if (m_hDib == NULL)	m_bValid = FALSE;

		//指向Dib实际像素数据的指针, 
		BYTE* pbyDib = (BYTE*)::GlobalLock(m_hDib);
		if (pbyDib == NULL)	m_bValid = FALSE;
	 
		try
		{
			//读入内存
			DWORD dwReadSize = dibFile.Read((void*)pbyDib,  dwSize);
			dibFile.Close();
			if(dwReadSize != dwSize)
			{
				m_bValid = FALSE;
				::GlobalUnlock(m_hDib);
				::GlobalFree(m_hDib);
				Init();
				return;
			}
		}
		catch(CFileException* e)
		{
			e->Delete();
		}

		//获取位图文件头指针和位图文件数据信息指针
        m_pBitmapInfo = (BITMAPINFO*) pbyDib;
		m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pbyDib;
		
		//宽度和高度
		m_nWidth = m_pBitmapInfoHeader->biWidth;
		m_nHeight = m_pBitmapInfoHeader->biHeight;
		//每像素的颜色位数
		m_nBitCount = (BYTE)m_pBitmapInfoHeader->biBitCount;
      
		//颜色数
		m_nTotalColors = GetColorNumber();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed = m_nTotalColors;

		//指向位图颜色索引表项的指针, 如果没有颜色表项, 则该值为NULL
        m_pRGBQuad = (RGBQUAD*)(pbyDib + m_pBitmapInfoHeader->biSize);
  
		//颜色索引表的大小(字节)
		dwColorTableSize = m_nTotalColors * sizeof(RGBQUAD);
        
		//指向位图数据的指针
		m_pDibData = pbyDib + m_pBitmapInfoHeader->biSize + dwColorTableSize;
		
		//如果没有颜色索引表, 则该值为NULL
		if (m_pRGBQuad == (RGBQUAD*)m_pDibData) m_pRGBQuad = NULL;
       
		//位图的大小(面积像素单位)
		m_pBitmapInfoHeader->biSizeImage = GetSize();
		::GlobalUnlock(m_hDib);
		AfxGetApp()->EndWaitCursor();
		m_bValid = TRUE;
    }    
    else
    {
        AfxMessageBox("This is not a bitmap file!");
		m_bValid = FALSE;
    }
}

LONG CDib::GetBITMAPWidth() const 
{
    return m_nWidth;
}
        
LONG CDib::GetBITMAPHeight() const
{
    return m_nHeight;
}

// 获得位图的宽，该函数为CDib的内部接口，由GetWidth函数调用
LONG CDib::DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  
	LPBITMAPCOREHEADER lpbmc;  

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 返回Win30位图的宽 
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  // 返回OS/2位图的宽 
		return (LONG)lpbmc->bcWidth;
}

// 获得位图的宽
LONG CDib::GetWidth()
{
	if(m_hDib == NULL)
		return 0;
	LPSTR lpDIB = (LPSTR)GlobalLock(m_hDib);
	return DIBWidth(lpDIB);
}	

// 获得位图的高，该函数为CDib的内部接口，由GetHeight函数调用
LONG CDib::DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  
	LPBITMAPCOREHEADER lpbmc;  

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 返回Win30位图的长 
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  // 返回OS/2位图的长
		return (LONG)lpbmc->bcHeight;
}

// 获得位图的高
LONG CDib::GetHeight()
{	
	if(m_hDib == NULL)
		return 0;
	LPSTR lpDIB = (LPSTR)GlobalLock(m_hDib);
	return DIBHeight(lpDIB);
}

//获取数据缓冲区的大小       
DWORD CDib::GetSize()
{
    if (m_pBitmapInfoHeader->biSizeImage != 0)
        return (m_pBitmapInfoHeader->biSizeImage);
	else 
		return (DWORD)GetDibWidthBytes(m_nWidth, 24) * (DWORD)m_nHeight;
}

BYTE CDib::GetBitCount() const
{
	return m_nBitCount;
}

UINT CDib::GetColorNumber() const
{
	UINT nColors = 0;

    if ((m_pBitmapInfoHeader->biClrUsed == 0) &&
          (m_pBitmapInfoHeader->biBitCount < 9))
	{
		switch (m_pBitmapInfoHeader->biBitCount)
		{
		    case 1: nColors = 2; break;
		    case 4: nColors = 16; break;
		    case 8: nColors = 256;break;
		}
	}
    else
		nColors = (UINT) m_pBitmapInfoHeader->biClrUsed;

    return nColors;
}

char* CDib::GetFileName() const
{
	return m_pszFileName;
}
    
BYTE* CDib::GetDibData() const
{
    return m_pDibData;
}

RGBQUAD* CDib::GetRGBQuad() const
{
    return m_pRGBQuad;
}

BITMAPINFO* CDib::GetBmpInfo() const
{
    return m_pBitmapInfo;
}

BOOL CDib::IsValid() const
{
	return m_bValid;
}

//每行的字节宽度
DWORD CDib::GetDibWidthBytes(int nWidth,  BYTE byBitCount)
{
	DWORD dwWidthBytes = (DWORD)nWidth;	//8-bits
	if(byBitCount == 1) dwWidthBytes = (nWidth + 7) / 8;
	else if(byBitCount == 4) dwWidthBytes = (nWidth + 1) / 2;
	else if(byBitCount == 24) dwWidthBytes = 3 * nWidth ;
	else if(byBitCount == 32) dwWidthBytes = 4 * nWidth ;
	
	while((dwWidthBytes & 3) != 0) dwWidthBytes++;

	return dwWidthBytes;
}

// 得到位图数据的指针
LPSTR CDib::FindDIBBits(LPSTR lpbi)
{
	LPSTR	    m_pDibData;

	m_pBitmapInfoHeader = (BITMAPINFOHEADER*) lpbi;
	dwColorTableSize = GetColorNumber() * sizeof(RGBQUAD);
	m_pDibData = lpbi + m_pBitmapInfoHeader->biSize + dwColorTableSize;
	return m_pDibData;
	//return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}

// 在屏幕上显示位图
BOOL CDib::PaintDIB(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect)
{
	LPSTR    lpDIBHdr;
	LPSTR    lpDIBBits;
	HPALETTE hPal;
	BOOL     bSuccess  =FALSE;
	HPALETTE hOldPal = NULL;

	if (m_hDib == NULL)
		return FALSE;
	// 获得指向位图数据的指针
	lpDIBHdr  = (LPSTR) GlobalLock(m_hDib);
	lpDIBBits = FindDIBBits(lpDIBHdr);

	// 获得位图的调色板并把它选入设备上下文
	if (m_pPalette != NULL)
	{
		hPal = (HPALETTE) m_pPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}
	::SetStretchBltMode(hDC, COLORONCOLOR);

	//决定是调用StretchDIBits还是SetDIBitsToDevice函数显示位图
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    
								   lpDCRect->left,             
								   lpDCRect->top,              
								   RECTWIDTH(lpDCRect),        
								   RECTHEIGHT(lpDCRect),       
								   lpDIBRect->left,            
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   
								   0,                          
								   (WORD)DIBHeight(lpDIBHdr),  
								   lpDIBBits,                  
								   (LPBITMAPINFO)lpDIBHdr,     
								   DIB_RGB_COLORS);            
   else
	  bSuccess = ::StretchDIBits(hDC,                          
							   lpDCRect->left,                 
							   lpDCRect->top,                  
							   RECTWIDTH(lpDCRect),            
							   RECTHEIGHT(lpDCRect),           
							   lpDIBRect->left,                
							   lpDIBRect->top,                 
							   RECTWIDTH(lpDIBRect),           
							   RECTHEIGHT(lpDIBRect),          
							   lpDIBBits,                      
							   (LPBITMAPINFO)lpDIBHdr,         
							   DIB_RGB_COLORS,                 
							   SRCCOPY);                       

   GlobalUnlock(m_hDib);

	// 重新选择旧的调色板
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

HDIB CDib::CropDIB(HDIB hDIB, LPRECT lpRect)
{
	LPBITMAPINFO lpbmi = NULL;
    LPSTR        lpSourceBits, lpTargetBits, lpResult;
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize; 
	int			 nWidth, nHeight;
	HDIB		 hNewDIB;
	DWORD		 dwSize;
	DWORD		 dwColorTableSize;

	// 获得指向DIB的指针
	if (! hDIB)
		return NULL;
	
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (! lpSrcDIB)
		return NULL;
	
	m_pBitmapInfoHeader = (BITMAPINFOHEADER*) lpSrcDIB;
	dwColorTableSize = GetColorNumber() * sizeof(RGBQUAD);

    // 为新的DIB申请内存并填充其BITMAPINFO结构
	dwTargetHeaderSize = sizeof( BITMAPINFOHEADER ) + dwColorTableSize; 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
	memcpy(lpbmi, lpSrcDIB, dwTargetHeaderSize);
	nWidth = RECTWIDTH(lpRect);
	nHeight = RECTHEIGHT(lpRect);
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
 
	// 利用DIBSections和BitBlt函数进行格式转换
   	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC );

	dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * GetDibWidthBytes(lpSrcDIB->bmiHeader.biWidth,  m_nBitCount);
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  m_nBitCount);
	memcpy( lpSourceBits, FindDIBBits((LPSTR)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
 
    // 把DIBSections选入设备上下文 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap );
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap );
 
	// 清除设备上下文 
    BitBlt(hTargetDC, 0, 0, nWidth, nHeight, hSourceDC, lpRect->left, lpRect->top, SRCCOPY);
 
    SelectObject( hSourceDC, hOldSourceBitmap );
    SelectObject( hTargetDC, hOldTargetBitmap );
    DeleteDC( hSourceDC );
    DeleteDC( hTargetDC );
    ReleaseDC( NULL, hDC );
    
    GdiFlush();
 	
	// 拷贝位数据
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPSTR)GlobalLock(hNewDIB);
	memcpy( (LPSTR)lpResult, lpbmi, dwTargetHeaderSize );
	memcpy( FindDIBBits( (LPSTR)lpResult ), lpTargetBits, dwTargetBitsSize );

    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi );
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
    return hNewDIB;
}

HDIB CDib::CropDIB(LPRECT lpRect)
{
	return CropDIB(m_hDib,lpRect);
}

//将所有格式的数据转换成DDB--24位数据, 存放于lpbyDdb24
//返回获取数据的总数.
DWORD  CDib::GetDdbData24(LPBYTE lpbyDdb24)
{
	ASSERT(lpbyDdb24);

	//计算DIB位图中一行的字节量
	DWORD dwDibWidthBytes = GetDibWidthBytes(m_nWidth,  m_nBitCount);

	//转换成24位DDB后每行的大小, 注意每行的字节数只需要是偶数即可
	DWORD dwDdbWidthBytes =  ((m_nWidth * 24 + 15) / 16) * 2;

	if(m_nBitCount == 1)
	{
		//每行实际字节数, 8个像素对应一个字节.
		LONG nLineBytes = (m_nWidth + 7) / 8;
		BYTE abyBitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
		
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
	
			//指向 DDB 24位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//8个像素数据的索引值
				BYTE byBit8 =  *(pbyDibRaw++);
				for(int n = 0; n < 8;n++)
				{
					BYTE byBitSingle = (byBit8 & abyBitMask[n]) >> (7 - n);
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbBlue;	//蓝色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbGreen;	//绿色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbRed;	//红色
				}
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 1)
	else if(m_nBitCount == 4)
	{
		//每行实际字节数
		LONG nLineBytes = (m_nWidth + 1) / 2;

		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			
			//指向 DDB 24位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//两个像素数据的索引值, 通过高四位和低四位
				BYTE byBit8 =  *(pbyDibRaw++);
				BYTE byBitHigh = (byBit8 & 0xF0) >> 4;
				BYTE byBitLow = (byBit8 & 0x0F);

				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbRed;		//红色

				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbGreen;		//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbRed;		//红色

			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 4)
	else if(m_nBitCount == 8)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < m_nHeight;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//指向 DDB 24位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < m_nWidth;j++)
			{
				//一个像素数据的索引值
				BYTE byIndex = *(pbyDibRaw++);
	
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbBlue;	//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbRed;	//红色
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 8)
	else if(m_nBitCount == 24)
	{
	
		DWORD dwLength = m_nWidth * 3;
		//指向DIB行数据的指针
		BYTE* pbyDibRaw = m_pDibData + (m_nHeight - 1) * dwDibWidthBytes;;
		//指向 DDB 24位位图的数据的指针
		BYTE* pbyDdbRaw = lpbyDdb24;

		//蓝色--绿色--红色
		for(LONG i = 0;i < m_nHeight;i++)
		{
			::CopyMemory(pbyDdbRaw, pbyDibRaw, dwLength);
			pbyDibRaw -= dwDibWidthBytes;		//from 
			pbyDdbRaw += dwDdbWidthBytes;		//to
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 24)

	return (m_nWidth * m_nHeight);
}

//将所有格式的数据转换成DDB--32位数据, 存放于lpbyDdb32
//返回获取数据的总数.
//32位DDB数据按字节顺序:蓝色--绿色--红色--保留字节--
DWORD  CDib::GetDdbData32(LPBYTE lpbyDdb32,HDIB hDIB)
{
	ASSERT(lpbyDdb32);

	if (!hDIB)
		return NULL;

	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (!lpSrcDIB)
		return NULL;

	long width = lpSrcDIB->bmiHeader.biWidth;
	long height = lpSrcDIB->bmiHeader.biHeight;

	//计算DIB位图中一行的字节量
	DWORD dwDibWidthBytes = GetDibWidthBytes(width,  m_nBitCount);

	//转换成32位DDB后每行的大小, 注意每位4字节
	DWORD dwDdbWidthBytes =  width * 4;

	if(m_nBitCount == 1)
	{
		//每行实际字节数, 8个像素对应一个字节.
		LONG nLineBytes = (width + 7) / 8;
		BYTE abyBitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
		
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < height;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
	
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//8个像素数据的索引值
				BYTE byBit8 =  *(pbyDibRaw++);
				for(int n = 0; n < 8;n++)
				{
					BYTE byBitSingle = (byBit8 & abyBitMask[n]) >> (7 - n);
					
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbBlue;	//蓝色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbGreen;	//绿色
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbRed;	//红色
					pbyDdbRaw++;										//保留**
				}
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 1)
	else if(m_nBitCount == 4)
	{
		//每行实际字节数
		LONG nLineBytes = (width + 1) / 2;

		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < height;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//两个像素数据的索引值, 通过高四位和低四位
				BYTE byBit8 =  *(pbyDibRaw++);
				BYTE byBitHigh = (byBit8 & 0xF0) >> 4;
				BYTE byBitLow = (byBit8 & 0x0F);
				
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbRed;		//红色
				pbyDdbRaw++;

				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbBlue;		//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbGreen;		//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbRed;		//红色
				pbyDdbRaw++;

			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 4)
	else if(m_nBitCount == 8)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < height;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < width;j++)
			{
				//一个像素数据的索引值
				BYTE byIndex = *(pbyDibRaw++);
	
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbBlue;	//蓝色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbGreen;	//绿色
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbRed;	//红色
				pbyDdbRaw++;
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 8)
	else if(m_nBitCount == 24)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < height;i++)
		{
			//指向DIB行数据的指针
			//BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			BYTE* pbyDibRaw = (BYTE*)FindDIBBits((LPSTR)lpSrcDIB) + dwDibBaseIndex;
		
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < width;j++)
			{
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//蓝色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//绿色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//红色
				pbyDdbRaw++;						
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		//return (dwDdbWidthBytes * m_nHeight);
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 24)
	else if(m_nBitCount == 32)
	{
		//DIB位图数据的行头
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32位位图数据的行头
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < height;i++)
		{
			//指向DIB行数据的指针
			BYTE* pbyDibRaw = (BYTE*)FindDIBBits((LPSTR)lpSrcDIB) + dwDibBaseIndex;
		
			//指向 DDB 32位位图的数据的指针
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < width;j++)
			{
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//蓝色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//绿色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//红色
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//保留字节
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		//return (dwDdbWidthBytes * m_nHeight);
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 32)

	//return (m_nWidth * m_nHeight);
	return (width * height);
}

void CDib::Format32DDBtoDiB(LPBYTE lpbySrcBits32,  int nScanWidth,  int nScanHeight,  LPBYTE lpTargetBits,  int m_nBitCount)
{// 将32位DDB数据转换32位DIB数据
	ASSERT(lpbySrcBits32);
	ASSERT(lpTargetBits);

	//32位DIB数据
	DWORD dwddbWidthBytes32 = (DWORD)nScanWidth * 4;
	DWORD dwdibWidthBytes = GetDibWidthBytes(nScanWidth, m_nBitCount);

	//对于32位DDB数据, 其扫描字节宽度必然为其宽度的2倍

	//指向32每行数据的指针	
	DWORD dwddbBaseIndex32 = (nScanHeight-1) * dwddbWidthBytes32;
	DWORD dwdibBaseIndex = 0;

	if ( m_nBitCount != 32 )
	{
		for(long i = 0; i < nScanHeight; i++)
		{
			BYTE* ddbpbyBits32 = lpbySrcBits32 + dwddbBaseIndex32;
			BYTE* dibpbyBits = lpTargetBits + dwdibBaseIndex;
			
			for(long j = 0; j < nScanWidth; j++)
			{
				*(dibpbyBits++) = *(ddbpbyBits32++);	//蓝色	
				*(dibpbyBits++) = *(ddbpbyBits32++);	//绿色
				*(dibpbyBits++) = *(ddbpbyBits32++);	//红色
				ddbpbyBits32++;
			}
			dwddbBaseIndex32 -= dwddbWidthBytes32;
			dwdibBaseIndex += dwdibWidthBytes;
		}
	}
	else if ( m_nBitCount == 32 )
	{
		for(long i = 0; i < nScanHeight; i++)
		{
			BYTE* ddbpbyBits32 = lpbySrcBits32 + dwddbBaseIndex32;
			BYTE* dibpbyBits = lpTargetBits + dwdibBaseIndex;

			for(long j = 0; j < nScanWidth; j++)
			{
				*(dibpbyBits++) = *(ddbpbyBits32++);	//蓝色	
				*(dibpbyBits++) = *(ddbpbyBits32++);	//绿色
				*(dibpbyBits++) = *(ddbpbyBits32++);	//红色
				*(dibpbyBits++) = *(ddbpbyBits32++);	//保留字节
			}
			dwddbBaseIndex32 -= dwddbWidthBytes32;
			dwdibBaseIndex += dwdibWidthBytes;
		}
	}
}

void CDib::Save(const char *pszDibFileName)
{
	BITMAPFILEHEADER bmfHdr;	// 位图文件头
	LPBITMAPINFOHEADER lpBI;    // 位图信息头指针
	DWORD dwDIBSize;

	if (m_hDib == NULL)
		return;
	// 获得存放DIB数据的内存块指针，该内存块的起始部分为BITMAPINFO结构
	lpBI = (LPBITMAPINFOHEADER)GlobalLock((HGLOBAL)m_hDib);
	if (lpBI == NULL)
		return;
	// 确认正在处理的DIB是否是一个OS/2 DIB，如果是，由于该函数不支持
	// 该格式的位图的保存，所以退出
	if (!IS_WIN30_DIB(lpBI))
	{
		GlobalUnlock(m_hDib);
		return;
	}
	// 填充文件头信息，其中标识文件类型的字段必须为“BM”以表示该文件为
	// 位图文件
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"
	// 计算DIB的尺寸，最简单的实现方法是对位图内存句柄hDIB调用GlobalSize()
	// 但是由于内存中可能被填充进其他一些字节，这样做的话将会向文件写入较多
	//的字节，而对有些应用程序这样往往会产生问题，因此下面手工对DIB的文件尺
	// 寸进行计算。由于BITMAPINFOHEADER和BITMAPCOREHEADER结构的开
	// 头双字节包含了这两个结构的尺寸信息，因此可用下面语句计算文件头尺寸与
	// 颜色表尺寸

	dwDIBSize = *(LPDWORD)lpBI + dwColorTableSize;  // Partial Calculation

	// 计算图像的尺寸
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// 该位图为一RLE位图，无法手工计算其尺寸，因此只好利用biSizeImage
		// 字段得到图像的尺寸
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// 该位图不是一个RLE位图, 因此图像尺寸为：高乘于宽，即
		// dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*
		// ((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		DWORD dwBmBitsSize;  
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		dwDIBSize += dwBmBitsSize;
		// 把通过计算得到的位图尺寸填充biSizeImage 项
		lpBI->biSizeImage = dwBmBitsSize;
	}
	// 通过把DIB的尺寸与sizeof(BITMAPFILEHEADER)相加得到文件尺寸
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	// 计算由文件头到位图数据的偏移量
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + dwColorTableSize;
	TRY
	{
		//	请创建自己的目录或获取当前目录：
		CFile file((LPCTSTR )pszDibFileName, CFile::modeCreate |
			CFile::modeReadWrite | CFile::shareExclusive); 
		
		// 写文件头
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		// 写DIB数据
		file.WriteHuge(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		::GlobalUnlock((HGLOBAL)m_hDib);
		THROW_LAST();
	}
	END_CATCH

	GlobalUnlock(m_hDib);
	return;
}
