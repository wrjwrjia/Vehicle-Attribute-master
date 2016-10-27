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

//lpbyBitsSrc32----Դ����ֵ
//x, y��ǰ���صľ��Ե�ַ, �������������(0, 0)���.
//nScanWidth,  int nScanHeight, ɨ���Ⱥ�ɨ��߶�
PIXELCOLORRGB CSpecialDetectionFilter::FilterPixelOnBorder(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	//�����Ԫ�ص��ܸ���;
	int nNum = m_nRows * m_nCols;

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//�洢һ�����������RGB������
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];

	//�����е����Ͻǵ�.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//��������pnRed, pnGreen, pnBlue������ֵ
	int index = 0;

	//��Ƕ��ѭ����ȡС��������.
	
	for(int i = 0;i < m_nRows;i++)
	{
		//y����
		int nY = yy + i;

		//���Ʊ߽�
		nY = (nY < 0) ? 0 : ((nY > (nScanHeight - 1)) ? (nScanHeight - 1) : nY);

		//ָ��, ָ��������
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes; 

		for(int j = 0;j < m_nCols;j++)
		{
			//x����	
			int nX = xx + j;

			//���Ʊ߽�
			nX = (nX < 0) ? 0 : ((nX > (nScanWidth - 1)) ? (nScanWidth - 1) : nX);
			
			BYTE* pbyRaw = pbySrc + 4 * nX;
			
			//��¼��ɫ����
			pbyBlue[index] = *pbyRaw++; 
			pbyGreen[index] = *pbyRaw++;
			pbyRed[index] = *pbyRaw++;
			index++;
		}
	}
	//RGB��ɫ�ṹ, �� IMG.H �ж���
	//������
	PIXELCOLORRGB rgb = Specialize(pbyRed, pbyGreen, pbyBlue, nNum);

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}

//��FilterPixelOnBorder()�������, �ú���ֻ�����˼����ж�������.
PIXELCOLORRGB CSpecialDetectionFilter::FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	//�����Ԫ�ص��ܸ���;
	int nNum = m_nRows * m_nCols;

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//�洢һ�����������RGB������
	BYTE* pbyRed = new BYTE[nNum];
	BYTE* pbyGreen = new BYTE[nNum];
	BYTE* pbyBlue = new BYTE[nNum];

	//�����е����Ͻǵ�.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//��������pnRed, pnGreen, pnBlue������ֵ
	int index = 0;

	//��Ƕ��ѭ����ȡС��������.
	
	for(int i = 0;i < m_nRows;i++)
	{
		//y����
		int nY = yy + i;

		//ָ��, ָ��������
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes + 4 * xx;

		for(int j = 0;j < m_nCols;j++)
		{
			//x����	
			//��¼��ɫ����
			pbyBlue[index] = *pbySrc++; 
			pbyGreen[index] = *pbySrc++;
			pbyRed[index] = *pbySrc++;
			pbySrc++;
			index++;
		}
	}

	//������
	PIXELCOLORRGB rgb = Specialize(pbyRed, pbyGreen, pbyBlue, nNum);

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;
}


//x, y, nWidth,  int nHeight, ��������Ŀ��
//nScanWidth,  int nScanHeight, ɨ���Ⱥ�ɨ��߶�
//lpbyBits32----����Դ����ֵ, ���ؽ��ֵ
BOOL CSpecialDetectionFilter::Filter(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	//��һ��, �����Ϸ��Լ��
	ASSERT(lpbyBits32);
	
	if ( m_dwOperation == IMAGE_SOBEL_EDGE_DETECT || m_dwOperation == IMAGE_KIRSCH_EDGE_DETECT )
		{
			m_nRows = 3;
			m_nCols = 3;
		}

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//����һ�ݿ���
	BYTE* pbySrcCopy =  new BYTE[ (dwWidthBytes * nScanHeight) ];
	if(pbySrcCopy == NULL) 
		return FALSE;

	::CopyMemory(pbySrcCopy, lpbyBits32, dwWidthBytes * nScanHeight);

	int i, j;

	//�������±߽�
	//���±߽�ĸ߶�:
	int nBorderH = (m_nRows - 1) / 2;
	for(i = 0;i < nBorderH;i++)
	{
		//��ǰ����y����
		int yy = y + i;

		//���ı�Դ����
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < w;j++)
		{
			//��ǰ����x����
			int xx = x + j;

			//���о������
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	
	
	//�±߽�Y����
	int nYBottom = (y + h - 1);
	//�����±߽�
	for(i = 0;i < nBorderH;i++)
	{
		//��ǰ����y����
		int yy = nYBottom - i;

		//ָ��Ŀ�ĵ�ַ
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;
		for(j = 0;j < w;j++)
		{
			//��ǰ����x����
			int xx = x + j;

			//���о������
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}

	//��ֱ�߽���
	int nBorderW = (m_nCols - 1) / 2;

	//ʣ��߶� + 1 + nBorderH: 
	int nRemnantH = (h - nBorderH);
	
	//������߽�
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//��ǰ����y����
		int yy = y + i;
		//ָ��Ŀ�ĵ�ַ
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;

		for(j = 0;j < nBorderW;j++)
		{
			//��ǰ����x����
			int xx = x + j;
			//���о������
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}

	//�ұ߽�x�������
	int nXRight = x + w - nBorderW; 

	//�����ұ߽�
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//��ǰ����y����
		int yy = y + i;
		//ָ��Ŀ�ĵ�ַ
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * nXRight;

		for(j = 0;j < nBorderW;j++)
		{
			//��ǰ����x����
			int xx = nXRight + j;
			//���о������
			PIXELCOLORRGB rgb = FilterPixelOnBorder(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}

	//�ڲ���� - nBorderW + 1
	int nInnerW = w - nBorderW;
	
	//�����ڲ�
	for(i = nBorderH; i < nRemnantH;i++)
	{
		//��ǰ����y����
		int yy = y + i;
		//ָ��Ŀ�ĵ�ַ
		BYTE* pbyDst = lpbyBits32 + yy * dwWidthBytes + 4 * x;

		for(j = 0;j < nInnerW;j++)
		{
			//��ǰ����x����
			int xx = x + j;
			//���о������
			PIXELCOLORRGB rgb = FilterPixelInner(pbySrcCopy, xx, yy, nScanWidth, nScanHeight);
	
			//д��Ŀ�����ݻ�����
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
			pbyDst++;
		}
	}
	
	delete[] pbySrcCopy;
	return TRUE;
}