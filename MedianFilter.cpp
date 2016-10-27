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

//�����ɰ�
void CMedianFilter::SetMask(const int *pnMask,  int nRows,  int nCols)
{
	//��֤Ϊ����
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


//���ٷ��෽��, �ݹ����
//nFirst, nLast��ʾ�������
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

//����߽�
PIXELCOLORRGB CMedianFilter::FilterPixelOnBorder(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//�洢һ�����������RGB������
	BYTE* pbyRed = new BYTE[m_nMaskBit];
	BYTE* pbyGreen = new BYTE[m_nMaskBit];
	BYTE* pbyBlue = new BYTE[m_nMaskBit];

	//�����е����Ͻǵ�.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//��������pnRed, pnGreen, pnBlue������ֵ
	int index = 0;

	//�������ɰ���Ѱ�ҷ�0��, �������
	int nOneByOne = 0;


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
			if(m_pnMask[nOneByOne] != 0)
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
			nOneByOne++;
		}//end j
	}//end i

	QuickSort(pbyRed, 0, (m_nMaskBit - 1));
	QuickSort(pbyGreen, 0, (m_nMaskBit - 1));
	QuickSort(pbyBlue, 0, (m_nMaskBit - 1));

	//RGB��ɫ�ṹ, �� IMG.H �ж���
	PIXELCOLORRGB rgb;

	rgb.red = pbyRed[m_nMaskBit / 2];
	rgb.green = pbyGreen[m_nMaskBit / 2];
	rgb.blue = pbyBlue[m_nMaskBit / 2];

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;

}

//�����ڲ�
PIXELCOLORRGB CMedianFilter::FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight)
{
	
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//�洢һ�����������RGB������
	BYTE* pbyRed = new BYTE[m_nMaskBit];
	BYTE* pbyGreen = new BYTE[m_nMaskBit];
	BYTE* pbyBlue = new BYTE[m_nMaskBit];

	//�����е����Ͻǵ�.
	int xx = x - ((m_nCols - 1) / 2);
	int yy = y - ((m_nRows - 1) / 2);

	//��������pnRed, pnGreen, pnBlue������ֵ
	int index = 0;

	//�������ɰ���Ѱ�ҷ�0��, �������
	int nOneByOne = 0;


	//��Ƕ��ѭ����ȡС��������.
	
	for(int i = 0;i < m_nRows;i++)
	{
		//y����
		int nY = yy + i;

		//ָ��, ָ��������
		BYTE* pbySrc = lpbyBitsSrc32 + ((DWORD)nY) * dwWidthBytes; 

		for(int j = 0;j < m_nCols;j++)
		{
			if(m_pnMask[nOneByOne] != 0)
			{
				//x����	
				int nX = xx + j;

				BYTE* pbyRaw = pbySrc + 4 * nX;
				
				//��¼��ɫ����
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

	//RGB��ɫ�ṹ, �� IMG.H �ж���
	PIXELCOLORRGB rgb;
	rgb.red = pbyRed[m_nMaskBit / 2];
	rgb.green = pbyGreen[m_nMaskBit / 2];
	rgb.blue = pbyBlue[m_nMaskBit / 2];

	delete[] pbyBlue;
	delete[] pbyGreen;
	delete[] pbyRed;
	return rgb;

}

//��ֵ����
BOOL CMedianFilter::Medianize(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	//��һ��, �����Ϸ��Լ��
	ASSERT(lpbyBits32);
	ASSERT(m_pnMask);
	if(m_nMaskBit == 0) return FALSE;

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//����һ�ݿ���
	BYTE* pbySrcCopy =  new BYTE[ (dwWidthBytes * nScanHeight) ];
	if(pbySrcCopy == NULL) return FALSE;
	::CopyMemory(pbySrcCopy, lpbyBits32, dwWidthBytes * nScanHeight);

	int i, j;

	//�������±߽�
	//���±߽�ĸ߶�:
	int nBorderH = (m_nRows - 1) / 2;
	for(i = 0;i < nBorderH;i++)
	{
		//��ǰ����y����
		int yy = y + i;

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

