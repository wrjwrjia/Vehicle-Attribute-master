#include "stdafx.h"
#include "GrayProcess.h"

IMPLEMENT_DYNAMIC(CGrayProcess, CImagePointProcess)
CGrayProcess::CGrayProcess()
{
	//ȱʡ����
	m_dwOperation = IMAGE_GRAY_COLOR_GRAYED_OUT;
	
	//�Ҷȱ������ٷֱ�
	m_nPercentage = 100;

	//�Ҷ����Ա任���ĸ��������
	m_byA = 0;
	m_byB = 255;
	m_byC = 0;
	m_byD = 255;

	//������֮ƥ��ĻҶ�Ƶ��
	m_pnMatchFreq = NULL;

	//ԭͼ��ĻҶ���ѧ����(expectation)
	m_nExpSrc = 128;
	
	//Ŀ��ͼ��ĻҶ���ѧ����
	m_nExpDst = 128;

}

CGrayProcess::~CGrayProcess()
{
}

#ifdef _DEBUG
void CGrayProcess::Dump(CDumpContext& dc) const
{
	CImagePointProcess::Dump(dc);
}

void CGrayProcess::AssertValid() const
{
	CImagePointProcess::AssertValid();
}
#endif

//����һ��int
void CGrayProcess::SetParam1i(int nPercentage)
{
	m_nPercentage = nPercentage;
}

//����4��BYTE
void CGrayProcess::SetParam4by(BYTE bya,  BYTE byb,  BYTE byc,  BYTE byd)
{
	m_byA = bya;
	m_byB = byb;
	m_byC = byc;
	m_byD = byd;

	if(m_byA > m_byB)
	{
		BYTE byTemp = m_byA;
		m_byA = m_byB;
		m_byB = byTemp;
	}
	
	if(m_byC > m_byD)
	{
		BYTE byTemp = m_byC;
		m_byC = m_byD;
		m_byD = byTemp;
	}
}

//����Ŀ��ֱ��ͼ,
//��֤ pnMatch �ĳ��Ȳ�С�� 256
void CGrayProcess::SetParam1iv(const int* pnMatch)
{
	if(m_pnMatchFreq) delete[] m_pnMatchFreq;
	else
		m_pnMatchFreq = new int[256];

	for(int i = 0; i < 256; i++)
		m_pnMatchFreq[i] = pnMatch[i];
}

//nExpDstĿ������
//nRatioVarPer�����ֵ(�ٷ�ֵ)
//nRatioVarPer = (int)((varSrc / varDst) * 100);
void CGrayProcess::SetParam2i(int nExpDst,  int nRatioVarPer)
{
	m_nExpDst = nExpDst;
	m_nPercentage = nRatioVarPer;
}


BOOL CGrayProcess::MakeGray(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	BOOL bSucessfully = TRUE;
	switch(m_dwOperation)
	{
		case IMAGE_GRAY_COLOR_GRAYED_OUT:
		{
			bSucessfully = MakeHsvColorGray(lpbyBits32, x, y, nWidth, nHeight, nScanWidth, nScanHeight);
			break;
		}
		case IMAGE_GRAY_PERCENTAGE:		
		case IMAGE_GRAY_LINEARIZE:		
		case IMAGE_GRAY_LINEAR_ROUND_OFF:
		case IMAGE_GRAY_REVERSE:
		{
			bSucessfully = MakeLinearGray(lpbyBits32, x, y, nWidth, nHeight, nScanWidth, nScanHeight);
			break;
		}
		case IMAGE_GRAY_HISTOGRAM_BALANCE:
		{
			bSucessfully = 	MakeHistogramBalance(lpbyBits32, x, y, nWidth, nHeight, nScanWidth, nScanHeight);
			break;

		}
		case IMAGE_GRAY_HISTOGRAM_MATCH:
		{
			bSucessfully = 	MakeHistogramMatch(lpbyBits32, x, y, nWidth, nHeight, nScanWidth, nScanHeight);
			break;
		}
		case IMAGE_GRAY_STATISTIC_MATCH:
		{
			bSucessfully = 	MakeStatisticMatch(lpbyBits32, x, y, nWidth, nHeight, nScanWidth, nScanHeight);
			break;
		}
		default : break;
	}
	return bSucessfully;
}

BOOL CGrayProcess::MakeHsvColorGray(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//��һ��, ���в����Ϸ��Լ��
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//���䳤��
	BYTE b_a = m_byB - m_byA;	// b - a
	BYTE d_c = m_byD - m_byC;	// d - c

	if(b_a == 0)b_a = 1;
	if(d_c == 0)d_c = 1;

	//�ڶ���, �Ҷ��޸�
	for(int i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(int j = 0;j < w;j++)
		{
			//��ȡָ��
			BYTE* pbyBlue = pbyRsc++;
			BYTE* pbyGreen = pbyRsc++;
			BYTE* pbyRed = pbyRsc++;
			pbyRsc++;

			//��ȡֵ;
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			
			float sum=r+g+b;
			float R=r,G=g,B=b;
			R=R/sum;
			G=G/sum;
			B=B/sum;
			//RBGתΪHSV
			float lmax=max(R,G);
			lmax=max(B,lmax);
			float lmin=min(R,G);
			lmin=min(B,lmin);
			float H,S,V;
			V=lmax*255;
			S=(lmax-lmin)/lmax*255;
			//HSV��ɫ�ռ�ģ��
			if(R == lmax)
				H =(G-B)/(lmax-lmin)* 60;
			if(G == lmax)
				H = 120+(B-R)/(lmax-lmin)* 60;
			if(B == lmax)
				H = 240 +(R-G)/(lmax-lmin)* 60;
			if(H < 0)
				H = H+ 360;
			H = H/360*255;
			//��ɫ���
			if(V>25 && S>=50 && ((H>=0 && H<20) || (H>230 && H<=255)))
			{
				*pbyBlue = 255;  //B
				*pbyGreen = 255;  //G
				*pbyRed = 255;  //R     
			}
			//��ɫ���
			else if(H>21 && H<64 && S>70 && V>102)
			{
				*pbyBlue = 255;  //B
				*pbyGreen = 255;  //G
				*pbyRed = 255;  //R
			}
			//��ɫ���
			else if(H>140 && H<188 && S>50)
			{
				*pbyBlue = 255;  //B
				*pbyGreen = 255;  //G
				*pbyRed = 255;  //R     
			} 
			else
			{
				*pbyBlue = 0;  //B
				*pbyGreen = 0;  //G
				*pbyRed = 0;  //R
			}
		}
		dwBaseIndex += dwWidthBytes;
	}
	return TRUE;
}

BOOL CGrayProcess::MakeLinearGray(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//��һ��, ���в����Ϸ��Լ��
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//���䳤��
	BYTE b_a = m_byB - m_byA;	// b - a
	BYTE d_c = m_byD - m_byC;	// d - c

	if(b_a == 0)b_a = 1;
	if(d_c == 0)d_c = 1;

	//�ڶ���, �Ҷ��޸�
	for(int i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(int j = 0;j < w;j++)
		{
			//��һС��, ��ȡָ��
			BYTE* pbyBlue = pbyRsc++;
			BYTE* pbyGreen = pbyRsc++;
			BYTE* pbyRed = pbyRsc++;
			pbyRsc++;

			//�ڶ�С��, ��ȡֵ;
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			

			BYTE gray = (BYTE)(((WORD)r * 30 + (WORD)g * 59 + (WORD)b * 11) / 100);
			//����С��, ����������ֵ
			switch(m_dwOperation)
			{
				//����ɫת��Ϊ�Ҷ�ͼ��
				case IMAGE_GRAY_COLOR_GRAYED_OUT:
				{
					*pbyBlue = gray;
					*pbyGreen = gray;
					*pbyRed = gray;	
					break;
				}
				//�Ҷȱ�����
				case IMAGE_GRAY_PERCENTAGE:
				{
					int nScale = ((int)gray * m_nPercentage) / 100;
					BYTE byResult = (BYTE)BOUND(nScale, 0, 255);
					*pbyBlue = byResult;
					*pbyGreen = byResult;
					*pbyRed = byResult;	
					break;
				}
				//�Ҷ����Ի�:
				case IMAGE_GRAY_LINEARIZE:
				{
					BYTE byResult = gray;
					if((gray >= m_byA) && (gray <= m_byB))
						byResult = (BYTE)((((WORD)d_c * (WORD)(gray - m_byA)) / (WORD)b_a) + m_byC);
					
					*pbyBlue = byResult;
					*pbyGreen = byResult;
					*pbyRed = byResult;	
					break;
				}

				//�Ҷ����Խض�
				case IMAGE_GRAY_LINEAR_ROUND_OFF:
				{
					BYTE byResult = gray;
					if(gray <= m_byA)byResult = m_byC;
					else if(gray >= m_byB)byResult = m_byD;
					else
						byResult = (BYTE)((((WORD)d_c * (WORD)(gray - m_byA)) / (WORD)b_a) + m_byC);
					
					*pbyBlue = byResult;
					*pbyGreen = byResult;
					*pbyRed = byResult;	
					break;
				}

				//�Ҷ�ȡ��
				case IMAGE_GRAY_REVERSE:
				{
					BYTE byResult = 255 - gray;
					*pbyBlue = byResult;
					*pbyGreen = byResult;
					*pbyRed = byResult;	
					break;	
				}
				default:break;
			}
		}
		dwBaseIndex += dwWidthBytes;
	}
	return TRUE;
}

BOOL CGrayProcess::MakeHistogramBalance(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//��һ��, ���в����Ϸ��Լ��
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//�ڶ���, �Ҷ�ͳ��
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//���������ظ���
	DWORD dwPixelSize = w * h;
	int i = 0;
	int j = 0;

	//����һ���ڴ���, ��¼ָ������ĻҶ�ֵ

	//��ű������������ĻҶ�
	BYTE* pbyGraySubArea =  new BYTE[dwPixelSize];
	if(pbyGraySubArea == NULL) return FALSE;

	//����һ������, ��СΪ256, ��¼256���Ҷȳ��ֵ�Ƶ��
	DWORD adwFreqSrc[256];
	for(i = 0;i < 256;i++)adwFreqSrc[i] = 0;

	BYTE* pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			BYTE b = *pbyRsc++;
			BYTE g = *pbyRsc++;
			BYTE r = *pbyRsc++;
			pbyRsc++;

			BYTE gray = (BYTE)(((WORD)r * 30 + (WORD)g * 59 + (WORD)b * 11) / 100);
			adwFreqSrc[gray]++;
			*pbyGrayCopy++ = gray;
		}
		dwBaseIndex += dwWidthBytes;
	}
	
	//���Ƶ��ͳ�ƹ���

	//��Ƶ��(Frequencies)�����ۼ�
	DWORD adwAccumuSrc[256];
	adwAccumuSrc[0] = adwFreqSrc[0];
	for(i = 1; i < 256; i++)
		adwAccumuSrc[i] = adwAccumuSrc[i - 1] + adwFreqSrc[i];
	
	//����¼�����ĻҶ�, ���Ե�, ��ȼ�����256��Ҷȵõ����
	for(i = 0; i < 256; i++)
		adwAccumuSrc[i] = ((adwAccumuSrc[i] * 255) / dwPixelSize);
	

	//������, �Ҷ��޸�
	//���¿�ʼ
	dwBaseIndex = y * dwWidthBytes + 4 * x;
	pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			//ԭ���صĻҶ�
			BYTE byGraySrc = *pbyGrayCopy++;
			//ԭ���ص����ջҶ�
			BYTE byGrayDst = (BYTE)adwAccumuSrc[byGraySrc];

			*pbyRsc++ = byGrayDst;
			*pbyRsc++ = byGrayDst;
			*pbyRsc++ = byGrayDst;
			pbyRsc++;
		}
		dwBaseIndex += dwWidthBytes;
	}

	delete[] pbyGraySubArea;
	return TRUE;
}

//ֱ��ͼ�涨��, Ӧ�ò���:
//��һ, �����Ҷȳ��ֵ�Ƶ��, ���ó�Ա�����趨�Ҷ�Ƶ��, �Ҷȼ�����Ϊ256��
//�ڶ�, ���лҶ�ֱ��ͼ�涨��
BOOL CGrayProcess::MakeHistogramMatch(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//��һ��, ���в����Ϸ��Լ��
	if(m_pnMatchFreq == NULL) return FALSE;
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//�ڶ���, ��ԭͼ��ҶȽ���ͳ��
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//���������ظ���
	DWORD dwPixelSize = w * h;
	int i = 0;
	int j = 0;

	//����һ���ڴ���, ��¼ָ������ĻҶ�ֵ
	
	//��¼������ĻҶ�
	BYTE* pbyGraySubArea =  new BYTE[dwPixelSize];
	if(pbyGraySubArea == NULL) return FALSE;

	//����һ������, ��СΪ256, ��¼256���Ҷȳ��ֵ�Ƶ��--Դ
	DWORD adwFreqSrc[256];
	for(i = 0;i < 256;i++)adwFreqSrc[i] = 0;

	BYTE* pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			BYTE b = *pbyRsc++;
			BYTE g = *pbyRsc++;
			BYTE r = *pbyRsc++;
			pbyRsc++;

			BYTE gray = (BYTE)(((WORD)r * 30 + (WORD)g * 59 + (WORD)b * 11) / 100);
			adwFreqSrc[gray]++;
			*pbyGrayCopy++ = gray;
		}
		dwBaseIndex += dwWidthBytes;
	}
	
	//���Ƶ��ͳ�ƹ���

	//��Ƶ�������ۼ�
	DWORD adwAccumSrc[256];
	adwAccumSrc[0] = adwFreqSrc[0];
	for(i = 1; i < 256; i++)
		adwAccumSrc[i] = adwAccumSrc[i - 1] + adwFreqSrc[i];
	
	//����¼�����ĻҶ�, ���Ե�, ��ȼ�����256��Ҷȵõ����
	for(i = 0; i < 256; i++)
		adwAccumSrc[i] = ((adwAccumSrc[i] * 255) / dwPixelSize);
	
	//������, ����ӳ��.

	//����, �ۻ��涨��Ƶ��, �Ӷ���ù涨��ֱ��ͼ
	DWORD adwAccumDst[256];
	adwAccumDst[0] = (DWORD)m_pnMatchFreq[0];
	for(i = 1; i < 256; i++)
		adwAccumDst[i] = adwAccumDst[i - 1] + (DWORD)m_pnMatchFreq[i];
	if(adwAccumDst[255] == 0)adwAccumDst[255] = 1;
	for(i = 0; i < 255; i++)
		adwAccumDst[i] = ((adwAccumDst[i] * 255) / adwAccumDst[255]);
	adwAccumDst[255] = 255;

	//����ӳ��:----һ�������
	adwAccumSrc[0] = SearchBinary((int*)adwAccumDst, adwAccumSrc[0], 0, 255);
	
	//�Ż��Ŀ��ٲ��ҷ���
	for(i = 1;i < 256;i++)
		adwAccumSrc[i] = SearchBinary((int*)adwAccumDst, adwAccumSrc[i], adwAccumSrc[i - 1], 255);
		
	//���Ĳ�, �Ҷ��޸�
	//���¿�ʼ
	dwBaseIndex = y * dwWidthBytes + 4 * x;
	pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			//ԭ���صĻҶ�
			BYTE byGraySrc = *pbyGrayCopy++;
			//ԭ���ص����ջҶ�
			BYTE byGrayDst = (BYTE)adwAccumSrc[byGraySrc];

			*pbyRsc++ = byGrayDst;
			*pbyRsc++ = byGrayDst;
			*pbyRsc++ = byGrayDst;
			pbyRsc++;
		}
		dwBaseIndex += dwWidthBytes;
	}

	delete[] pbyGraySubArea;

	return TRUE;
}

//�ڽ����ֲ���, �����������
//nValue----�����ҵ�ֵ
//nStart----��ʼ���
//nEnd-----�������
int CGrayProcess::SearchBinary(const int* pnArray,  int nValue, int nStart, int nEnd)
{
	ASSERT(pnArray);

	int nLength = nEnd - nStart + 1;
	if(nLength == 1)return nStart;
	else if(nLength == 2)
	{
		if(nValue <= pnArray[nStart]) return nStart;
		else if(nValue >= pnArray[nStart + 1])return (nStart + 1);
		else
		{
			if((nValue - pnArray[nStart]) < (pnArray[nStart + 1] - nValue)) 
				return nStart;
			else 
				return (nStart + 1);
		}
	}
	//nLength >= 3
	else	
	{
		//�����Ҷ˵�
		int nRight = nEnd;
		//������˵�
		int nLeft = nStart;		

	
		//С�ڵ�����˵��ֵ
		if(nValue <= pnArray[nLeft]) return nLeft;
		//���ڵ����Ҷ˵��ֵ
		if(nValue >= pnArray[nRight]) return nRight;
		
		nRight--;
		nLeft++;

		//�����е�
		int nMiddle = (nRight + nLeft) / 2;

		//�Ƿ����ʶȵ�����
		BOOL bFound = FALSE;
		
		//����ֵ
		int nReturn = -1;

		//������֮��һ����[nLeft - 1, nRight + 1]������
		while(!bFound && (nRight >= nLeft))
		{
			//equal to the middle
			if(nValue == pnArray[nMiddle])
			{
				bFound = TRUE;
				nReturn = nMiddle;
			}
			else if(nValue < pnArray[nMiddle])
			{
				//�ҵ���
				if(nValue >= pnArray[nMiddle - 1])
				{
					bFound = TRUE;
					//�Ƚ�
					if((nValue - pnArray[nMiddle - 1]) < (pnArray[nMiddle] - nValue))
						nReturn = nMiddle - 1;
					else
						nReturn = nMiddle;
				}
				//û���ҵ�, ���ڸ�����
				else
					nRight = nMiddle - 1;
			}

			//nValue > pnArray[middle]
			else
			{
				//�ҵ���
				if(nValue < pnArray[nMiddle + 1])
				{
					bFound = TRUE;
					//�Ƚ�
					if((nValue - pnArray[nMiddle]) < (pnArray[nMiddle + 1] - nValue))
						nReturn = nMiddle;
					else
						nReturn = (nMiddle + 1);
				}
				
				//û���ҵ�, ���ڸ�����
				else
					nLeft = nMiddle + 1;
			}//nValue >= pnArray[middle]
			nMiddle = (nRight + nLeft) / 2;
		
		}//end -- while
		
		return nReturn;
	}// end -- else -- nLength >= 3
}

//��Wallis and Jong-Sen Lee�ķ���������ѧ����
int CGrayProcess::GetGrayExpectation(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//��һ��, ���в����Ϸ��Լ��
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//�ڶ���, ����Ҷ���ѧ����
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//��ѧ�����ۻ�
	int nGrayExpSum = 0;
	for(int i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(int j = 0;j < w;j++)
		{
			BYTE b = *pbyRsc++;
			BYTE g = *pbyRsc++;
			BYTE r = *pbyRsc++;
			pbyRsc++;

			BYTE gray = (BYTE)(((WORD)r * 30 + (WORD)g * 59 + (WORD)b * 11) / 100);
			
			nGrayExpSum += gray;
		}
		dwBaseIndex += dwWidthBytes;
	}
	m_nExpSrc = (nGrayExpSum / (w * h));
	return m_nExpSrc;
}

//����ͳ��ƥ��
BOOL CGrayProcess::MakeStatisticMatch(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//��һ��, ���в����Ϸ��Լ��
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//�ڶ���, �Ҷ�ͳ��
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//���������ظ���
	DWORD dwPixelSize = w * h;
	int i = 0;
	int j = 0;

	//����һ���ڴ���, ��¼ָ������ĻҶ�ֵ
	BYTE* pbyGraySubArea =  new BYTE[dwPixelSize];
	if(pbyGraySubArea == NULL) return FALSE;

	//��¼�Ҷ���ѧ����(ƽ��ֵ, ��Wallis and Jong-Sen Lee�ķ���)
	int nExpGray = 0;

	//����ҶȺ���ѧ����(��, ƽ��ֵ)
	BYTE* pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			BYTE b = *pbyRsc++;
			BYTE g = *pbyRsc++;
			BYTE r = *pbyRsc++;
			pbyRsc++;

			BYTE gray = (BYTE)(((WORD)r * 30 + (WORD)g * 59 + (WORD)b * 11) / 100);
			*pbyGrayCopy++ = gray;
			nExpGray += gray;
		}
		dwBaseIndex += dwWidthBytes;
	}
	
	//�����ѧ����:
	nExpGray /= dwPixelSize;	

	//������, �Ҷ��޸�
	//���¿�ʼ
	dwBaseIndex = y * dwWidthBytes + 4 * x;
	pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			//ԭ���صĻҶ�
			BYTE byGraySrc = *pbyGrayCopy++;
			
			//ԭ���ص����ջҶ�
			 int nGrayDst = (m_nPercentage * ((int)byGraySrc - nExpGray)) / 100 + m_nExpDst;
			 BYTE byGrayDst = (BYTE)(BOUND(nGrayDst, 0, 255));

			*pbyRsc++ = byGrayDst;
			*pbyRsc++ = byGrayDst;
			*pbyRsc++ = byGrayDst;
			pbyRsc++;
		}
		dwBaseIndex += dwWidthBytes;
	}

	delete[] pbyGraySubArea;
	return TRUE;
}


