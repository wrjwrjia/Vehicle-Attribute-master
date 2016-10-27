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
	m_pRGBQuad = NULL;		//RGBQUAD ����
	m_nWidth = 0;			//���
	m_nHeight = 0;			//�߶�
	m_nBitCount = 0;		//ÿ������ռ�е�λ��
    m_nTotalColors = 0;		//��ɫ����
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

	//������ھ��ͷ��ڴ�
	if(m_pBitmapInfo) ::GlobalFree(m_hDib);
	//�ٴγ�ʼ��, ���ڶ�ζ�̬����
	Init();
	m_pszFileName = (char*)pszDibFileName;

	BITMAPFILEHEADER bitmapFileHeader;

	CFile dibFile(pszDibFileName, CFile::modeRead | CFile::shareDenyWrite);
	try
	{
		//����λͼ�ļ�ͷ��Ϣ
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

	//���Ϊλͼ�ͽ�����һ���Ĵ���
    if (bitmapFileHeader.bfType == 0x4d42)
    {
		AfxGetApp()->BeginWaitCursor();
		
		//�ļ�����
        DWORD dwFileLength = dibFile.GetLength();
		
		//λͼ��Ϣ��λͼ���ݵĴ�С(�ֽ�)
        DWORD dwSize = dwFileLength - sizeof(BITMAPFILEHEADER);
 

		//ΪDib����ȫ���ڴ�
		m_hDib = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE,  dwSize);
		if (m_hDib == NULL)	m_bValid = FALSE;

		//ָ��Dibʵ���������ݵ�ָ��, 
		BYTE* pbyDib = (BYTE*)::GlobalLock(m_hDib);
		if (pbyDib == NULL)	m_bValid = FALSE;
	 
		try
		{
			//�����ڴ�
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

		//��ȡλͼ�ļ�ͷָ���λͼ�ļ�������Ϣָ��
        m_pBitmapInfo = (BITMAPINFO*) pbyDib;
		m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pbyDib;
		
		//��Ⱥ͸߶�
		m_nWidth = m_pBitmapInfoHeader->biWidth;
		m_nHeight = m_pBitmapInfoHeader->biHeight;
		//ÿ���ص���ɫλ��
		m_nBitCount = (BYTE)m_pBitmapInfoHeader->biBitCount;
      
		//��ɫ��
		m_nTotalColors = GetColorNumber();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed = m_nTotalColors;

		//ָ��λͼ��ɫ���������ָ��, ���û����ɫ����, ���ֵΪNULL
        m_pRGBQuad = (RGBQUAD*)(pbyDib + m_pBitmapInfoHeader->biSize);
  
		//��ɫ������Ĵ�С(�ֽ�)
		dwColorTableSize = m_nTotalColors * sizeof(RGBQUAD);
        
		//ָ��λͼ���ݵ�ָ��
		m_pDibData = pbyDib + m_pBitmapInfoHeader->biSize + dwColorTableSize;
		
		//���û����ɫ������, ���ֵΪNULL
		if (m_pRGBQuad == (RGBQUAD*)m_pDibData) m_pRGBQuad = NULL;
       
		//λͼ�Ĵ�С(������ص�λ)
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

// ���λͼ�Ŀ��ú���ΪCDib���ڲ��ӿڣ���GetWidth��������
LONG CDib::DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  
	LPBITMAPCOREHEADER lpbmc;  

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����Win30λͼ�Ŀ� 
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  // ����OS/2λͼ�Ŀ� 
		return (LONG)lpbmc->bcWidth;
}

// ���λͼ�Ŀ�
LONG CDib::GetWidth()
{
	if(m_hDib == NULL)
		return 0;
	LPSTR lpDIB = (LPSTR)GlobalLock(m_hDib);
	return DIBWidth(lpDIB);
}	

// ���λͼ�ĸߣ��ú���ΪCDib���ڲ��ӿڣ���GetHeight��������
LONG CDib::DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  
	LPBITMAPCOREHEADER lpbmc;  

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����Win30λͼ�ĳ� 
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  // ����OS/2λͼ�ĳ�
		return (LONG)lpbmc->bcHeight;
}

// ���λͼ�ĸ�
LONG CDib::GetHeight()
{	
	if(m_hDib == NULL)
		return 0;
	LPSTR lpDIB = (LPSTR)GlobalLock(m_hDib);
	return DIBHeight(lpDIB);
}

//��ȡ���ݻ������Ĵ�С       
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

//ÿ�е��ֽڿ��
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

// �õ�λͼ���ݵ�ָ��
LPSTR CDib::FindDIBBits(LPSTR lpbi)
{
	LPSTR	    m_pDibData;

	m_pBitmapInfoHeader = (BITMAPINFOHEADER*) lpbi;
	dwColorTableSize = GetColorNumber() * sizeof(RGBQUAD);
	m_pDibData = lpbi + m_pBitmapInfoHeader->biSize + dwColorTableSize;
	return m_pDibData;
	//return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}

// ����Ļ����ʾλͼ
BOOL CDib::PaintDIB(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect)
{
	LPSTR    lpDIBHdr;
	LPSTR    lpDIBBits;
	HPALETTE hPal;
	BOOL     bSuccess  =FALSE;
	HPALETTE hOldPal = NULL;

	if (m_hDib == NULL)
		return FALSE;
	// ���ָ��λͼ���ݵ�ָ��
	lpDIBHdr  = (LPSTR) GlobalLock(m_hDib);
	lpDIBBits = FindDIBBits(lpDIBHdr);

	// ���λͼ�ĵ�ɫ�岢����ѡ���豸������
	if (m_pPalette != NULL)
	{
		hPal = (HPALETTE) m_pPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}
	::SetStretchBltMode(hDC, COLORONCOLOR);

	//�����ǵ���StretchDIBits����SetDIBitsToDevice������ʾλͼ
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

	// ����ѡ��ɵĵ�ɫ��
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

	// ���ָ��DIB��ָ��
	if (! hDIB)
		return NULL;
	
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	lpSrcDIB->bmiHeader.biSizeImage = 0;
	if (! lpSrcDIB)
		return NULL;
	
	m_pBitmapInfoHeader = (BITMAPINFOHEADER*) lpSrcDIB;
	dwColorTableSize = GetColorNumber() * sizeof(RGBQUAD);

    // Ϊ�µ�DIB�����ڴ沢�����BITMAPINFO�ṹ
	dwTargetHeaderSize = sizeof( BITMAPINFOHEADER ) + dwColorTableSize; 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
	memcpy(lpbmi, lpSrcDIB, dwTargetHeaderSize);
	nWidth = RECTWIDTH(lpRect);
	nHeight = RECTHEIGHT(lpRect);
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
 
	// ����DIBSections��BitBlt�������и�ʽת��
   	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC );

	dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * GetDibWidthBytes(lpSrcDIB->bmiHeader.biWidth,  m_nBitCount);
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * GetDibWidthBytes(lpbmi->bmiHeader.biWidth,  m_nBitCount);
	memcpy( lpSourceBits, FindDIBBits((LPSTR)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize;
 
    // ��DIBSectionsѡ���豸������ 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap );
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap );
 
	// ����豸������ 
    BitBlt(hTargetDC, 0, 0, nWidth, nHeight, hSourceDC, lpRect->left, lpRect->top, SRCCOPY);
 
    SelectObject( hSourceDC, hOldSourceBitmap );
    SelectObject( hTargetDC, hOldTargetBitmap );
    DeleteDC( hSourceDC );
    DeleteDC( hTargetDC );
    ReleaseDC( NULL, hDC );
    
    GdiFlush();
 	
	// ����λ����
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

//�����и�ʽ������ת����DDB--24λ����, �����lpbyDdb24
//���ػ�ȡ���ݵ�����.
DWORD  CDib::GetDdbData24(LPBYTE lpbyDdb24)
{
	ASSERT(lpbyDdb24);

	//����DIBλͼ��һ�е��ֽ���
	DWORD dwDibWidthBytes = GetDibWidthBytes(m_nWidth,  m_nBitCount);

	//ת����24λDDB��ÿ�еĴ�С, ע��ÿ�е��ֽ���ֻ��Ҫ��ż������
	DWORD dwDdbWidthBytes =  ((m_nWidth * 24 + 15) / 16) * 2;

	if(m_nBitCount == 1)
	{
		//ÿ��ʵ���ֽ���, 8�����ض�Ӧһ���ֽ�.
		LONG nLineBytes = (m_nWidth + 7) / 8;
		BYTE abyBitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
		
		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
	
			//ָ�� DDB 24λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//8���������ݵ�����ֵ
				BYTE byBit8 =  *(pbyDibRaw++);
				for(int n = 0; n < 8;n++)
				{
					BYTE byBitSingle = (byBit8 & abyBitMask[n]) >> (7 - n);
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbBlue;	//��ɫ
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbGreen;	//��ɫ
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbRed;	//��ɫ
				}
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 1)
	else if(m_nBitCount == 4)
	{
		//ÿ��ʵ���ֽ���
		LONG nLineBytes = (m_nWidth + 1) / 2;

		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < m_nHeight;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			
			//ָ�� DDB 24λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//�����������ݵ�����ֵ, ͨ������λ�͵���λ
				BYTE byBit8 =  *(pbyDibRaw++);
				BYTE byBitHigh = (byBit8 & 0xF0) >> 4;
				BYTE byBitLow = (byBit8 & 0x0F);

				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbBlue;		//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbGreen;	//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbRed;		//��ɫ

				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbBlue;		//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbGreen;		//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbRed;		//��ɫ

			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 4)
	else if(m_nBitCount == 8)
	{
		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (m_nHeight - 1) * dwDibWidthBytes;
		
		//DDB 24λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < m_nHeight;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//ָ�� DDB 24λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb24 + dwDdbBaseIndex;
			for(LONG j = 0;j < m_nWidth;j++)
			{
				//һ���������ݵ�����ֵ
				BYTE byIndex = *(pbyDibRaw++);
	
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbBlue;	//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbGreen;	//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbRed;	//��ɫ
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * m_nHeight);
	}// end if(m_nBitCount == 8)
	else if(m_nBitCount == 24)
	{
	
		DWORD dwLength = m_nWidth * 3;
		//ָ��DIB�����ݵ�ָ��
		BYTE* pbyDibRaw = m_pDibData + (m_nHeight - 1) * dwDibWidthBytes;;
		//ָ�� DDB 24λλͼ�����ݵ�ָ��
		BYTE* pbyDdbRaw = lpbyDdb24;

		//��ɫ--��ɫ--��ɫ
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

//�����и�ʽ������ת����DDB--32λ����, �����lpbyDdb32
//���ػ�ȡ���ݵ�����.
//32λDDB���ݰ��ֽ�˳��:��ɫ--��ɫ--��ɫ--�����ֽ�--
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

	//����DIBλͼ��һ�е��ֽ���
	DWORD dwDibWidthBytes = GetDibWidthBytes(width,  m_nBitCount);

	//ת����32λDDB��ÿ�еĴ�С, ע��ÿλ4�ֽ�
	DWORD dwDdbWidthBytes =  width * 4;

	if(m_nBitCount == 1)
	{
		//ÿ��ʵ���ֽ���, 8�����ض�Ӧһ���ֽ�.
		LONG nLineBytes = (width + 7) / 8;
		BYTE abyBitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
		
		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < height;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
	
			//ָ�� DDB 32λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//8���������ݵ�����ֵ
				BYTE byBit8 =  *(pbyDibRaw++);
				for(int n = 0; n < 8;n++)
				{
					BYTE byBitSingle = (byBit8 & abyBitMask[n]) >> (7 - n);
					
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbBlue;	//��ɫ
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbGreen;	//��ɫ
					*(pbyDdbRaw++) = m_pRGBQuad[byBitSingle].rgbRed;	//��ɫ
					pbyDdbRaw++;										//����**
				}
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 1)
	else if(m_nBitCount == 4)
	{
		//ÿ��ʵ���ֽ���
		LONG nLineBytes = (width + 1) / 2;

		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;

		for(LONG i = 0;i < height;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			
			//ָ�� DDB 32λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < nLineBytes;j++)
			{
				//�����������ݵ�����ֵ, ͨ������λ�͵���λ
				BYTE byBit8 =  *(pbyDibRaw++);
				BYTE byBitHigh = (byBit8 & 0xF0) >> 4;
				BYTE byBitLow = (byBit8 & 0x0F);
				
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbBlue;		//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbGreen;	//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitHigh].rgbRed;		//��ɫ
				pbyDdbRaw++;

				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbBlue;		//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbGreen;		//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byBitLow].rgbRed;		//��ɫ
				pbyDdbRaw++;

			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 4)
	else if(m_nBitCount == 8)
	{
		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < height;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
		
			//ָ�� DDB 32λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < width;j++)
			{
				//һ���������ݵ�����ֵ
				BYTE byIndex = *(pbyDibRaw++);
	
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbBlue;	//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbGreen;	//��ɫ
				*(pbyDdbRaw++) = m_pRGBQuad[byIndex].rgbRed;	//��ɫ
				pbyDdbRaw++;
			}//end j
			dwDibBaseIndex -= dwDibWidthBytes;
			dwDdbBaseIndex += dwDdbWidthBytes;
		}//end i
		return (dwDdbWidthBytes * height);
	}// end if(m_nBitCount == 8)
	else if(m_nBitCount == 24)
	{
		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < height;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			//BYTE* pbyDibRaw = m_pDibData + dwDibBaseIndex;
			BYTE* pbyDibRaw = (BYTE*)FindDIBBits((LPSTR)lpSrcDIB) + dwDibBaseIndex;
		
			//ָ�� DDB 32λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < width;j++)
			{
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//��ɫ
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//��ɫ
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//��ɫ
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
		//DIBλͼ���ݵ���ͷ
		DWORD dwDibBaseIndex = (height - 1) * dwDibWidthBytes;
		
		//DDB 32λλͼ���ݵ���ͷ
		DWORD dwDdbBaseIndex = 0;
		for(LONG i = 0;i < height;i++)
		{
			//ָ��DIB�����ݵ�ָ��
			BYTE* pbyDibRaw = (BYTE*)FindDIBBits((LPSTR)lpSrcDIB) + dwDibBaseIndex;
		
			//ָ�� DDB 32λλͼ�����ݵ�ָ��
			BYTE* pbyDdbRaw = lpbyDdb32 + dwDdbBaseIndex;
			for(LONG j = 0;j < width;j++)
			{
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//��ɫ
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//��ɫ
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//��ɫ
				*(pbyDdbRaw++) = *(pbyDibRaw++);	//�����ֽ�
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
{// ��32λDDB����ת��32λDIB����
	ASSERT(lpbySrcBits32);
	ASSERT(lpTargetBits);

	//32λDIB����
	DWORD dwddbWidthBytes32 = (DWORD)nScanWidth * 4;
	DWORD dwdibWidthBytes = GetDibWidthBytes(nScanWidth, m_nBitCount);

	//����32λDDB����, ��ɨ���ֽڿ�ȱ�ȻΪ���ȵ�2��

	//ָ��32ÿ�����ݵ�ָ��	
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
				*(dibpbyBits++) = *(ddbpbyBits32++);	//��ɫ	
				*(dibpbyBits++) = *(ddbpbyBits32++);	//��ɫ
				*(dibpbyBits++) = *(ddbpbyBits32++);	//��ɫ
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
				*(dibpbyBits++) = *(ddbpbyBits32++);	//��ɫ	
				*(dibpbyBits++) = *(ddbpbyBits32++);	//��ɫ
				*(dibpbyBits++) = *(ddbpbyBits32++);	//��ɫ
				*(dibpbyBits++) = *(ddbpbyBits32++);	//�����ֽ�
			}
			dwddbBaseIndex32 -= dwddbWidthBytes32;
			dwdibBaseIndex += dwdibWidthBytes;
		}
	}
}

void CDib::Save(const char *pszDibFileName)
{
	BITMAPFILEHEADER bmfHdr;	// λͼ�ļ�ͷ
	LPBITMAPINFOHEADER lpBI;    // λͼ��Ϣͷָ��
	DWORD dwDIBSize;

	if (m_hDib == NULL)
		return;
	// ��ô��DIB���ݵ��ڴ��ָ�룬���ڴ�����ʼ����ΪBITMAPINFO�ṹ
	lpBI = (LPBITMAPINFOHEADER)GlobalLock((HGLOBAL)m_hDib);
	if (lpBI == NULL)
		return;
	// ȷ�����ڴ����DIB�Ƿ���һ��OS/2 DIB������ǣ����ڸú�����֧��
	// �ø�ʽ��λͼ�ı��棬�����˳�
	if (!IS_WIN30_DIB(lpBI))
	{
		GlobalUnlock(m_hDib);
		return;
	}
	// ����ļ�ͷ��Ϣ�����б�ʶ�ļ����͵��ֶα���Ϊ��BM���Ա�ʾ���ļ�Ϊ
	// λͼ�ļ�
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"
	// ����DIB�ĳߴ磬��򵥵�ʵ�ַ����Ƕ�λͼ�ڴ���hDIB����GlobalSize()
	// ���������ڴ��п��ܱ���������һЩ�ֽڣ��������Ļ��������ļ�д��϶�
	//���ֽڣ�������ЩӦ�ó�������������������⣬��������ֹ���DIB���ļ���
	// ����м��㡣����BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�Ŀ�
	// ͷ˫�ֽڰ������������ṹ�ĳߴ���Ϣ����˿��������������ļ�ͷ�ߴ���
	// ��ɫ��ߴ�

	dwDIBSize = *(LPDWORD)lpBI + dwColorTableSize;  // Partial Calculation

	// ����ͼ��ĳߴ�
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// ��λͼΪһRLEλͼ���޷��ֹ�������ߴ磬���ֻ������biSizeImage
		// �ֶεõ�ͼ��ĳߴ�
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// ��λͼ����һ��RLEλͼ, ���ͼ��ߴ�Ϊ���߳��ڿ���
		// dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*
		// ((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		DWORD dwBmBitsSize;  
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		dwDIBSize += dwBmBitsSize;
		// ��ͨ������õ���λͼ�ߴ����biSizeImage ��
		lpBI->biSizeImage = dwBmBitsSize;
	}
	// ͨ����DIB�ĳߴ���sizeof(BITMAPFILEHEADER)��ӵõ��ļ��ߴ�
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	// �������ļ�ͷ��λͼ���ݵ�ƫ����
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + dwColorTableSize;
	TRY
	{
		//	�봴���Լ���Ŀ¼���ȡ��ǰĿ¼��
		CFile file((LPCTSTR )pszDibFileName, CFile::modeCreate |
			CFile::modeReadWrite | CFile::shareExclusive); 
		
		// д�ļ�ͷ
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		// дDIB����
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
