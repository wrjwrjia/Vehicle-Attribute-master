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
{// �ú����������32λͼ��lpbyBits32��ָ��32λͼͼ�����ݵ�ָ��
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBits32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;

	//ѭ������
	long	i, j;

	//ǰһ��(�󷽵�)���ص�
	BYTE	prepixel;

	//��ǰ���ص�
	BYTE	nowpixel;
	
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

	//ÿ��
	for( i = 0; i < nHeight; i++ )
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		prepixel = (BYTE)255;
		nowpixel = (BYTE)255;

		//ÿ��
		for( j = 0; j < nWidth; j++ )
		{
			//����ֵ
			nowpixel = (BYTE)*lpSrc;
			//��������ص��Ǻ�ɫ������
			if( nowpixel == 0 )
			{
				*lpSrc++;
				*lpSrc++;
				*lpSrc++;
				*lpSrc++;
				prepixel = (BYTE)0;
			}
			//��������ص��ǰ�ɫ
			else if( nowpixel == 255 )
				//��������ص��Ǻ�ɫ������
				if( prepixel == 0 )
				{
					*lpSrc++;
					*lpSrc++;
					*lpSrc++;
					*lpSrc++;
					prepixel = (BYTE)255;
				}
				 //��������ص��ǰ�ɫ����ǰ���ص���Ϊ��ɫ
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