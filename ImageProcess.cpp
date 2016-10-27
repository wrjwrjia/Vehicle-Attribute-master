#include "stdafx.h"
#include "ImageProcess.h"

IMPLEMENT_DYNAMIC(CImageProcess,  CObject)
CImageProcess::CImageProcess()
{
	m_dwOperation = 0L;	//ȱʡ
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

//����ͼ�������(����)
void CImageProcess::SetOperation(DWORD dwOperation)
{
	m_dwOperation = dwOperation;
}


//������ɫģ��
//ע��, ���ǽ�ֻ����RGBA��ɫģ��
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



//��һ��32λ�޷��������л�ȡ��ɫ����
BYTE CImageProcess::GetAlpha(DWORD dwRGBA)
{
	//���ֽڣ�˳��ΪA--B--G--R.
	//���������һ˳��, �밴���ظú���
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
	//��͸������
	return ( 0xFF000000 | 
		   (((DWORD)byRed) << 16)	|
		   (((DWORD)byGreen) << 8) |
		   ((DWORD)byBlue)) ;
}

//��32λDDB����ֱ�Ӵ���һ��DDB����, ��HBITMAP����
HBITMAP CImageProcess::CreateDdb(HDC hDC,  int nWidth,  int nHeight,  const LPBYTE lpbyBits32)
{
	//��һ��, ����һ��BITMAPV4HEADER�ṹ

	//�����ڴ�:
	BYTE* pbyBits = new BYTE[sizeof(BITMAPV4HEADER)];
	if(pbyBits == NULL) return NULL;
	//ָ��Dibʵ���������ݵ�ָ��, 
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
 
	//�ڶ���, ����һ��DDB
	HBITMAP hBitmap = ::CreateDIBitmap(hDC, (BITMAPINFOHEADER*)pbmih, CBM_INIT, lpbyBits32, (BITMAPINFO*)pbmih, DIB_RGB_COLORS);

	delete[] pbyBits;
	return hBitmap;
}


