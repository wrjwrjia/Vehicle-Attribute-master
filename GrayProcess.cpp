#include "stdafx.h"
#include "GrayProcess.h"

IMPLEMENT_DYNAMIC(CGrayProcess, CImagePointProcess)
CGrayProcess::CGrayProcess()
{
	//缺省操作
	m_dwOperation = IMAGE_GRAY_COLOR_GRAYED_OUT;
	
	//灰度比例化百分比
	m_nPercentage = 100;

	//灰度线性变换的四个区间参数
	m_byA = 0;
	m_byB = 255;
	m_byC = 0;
	m_byD = 255;

	//给定与之匹配的灰度频数
	m_pnMatchFreq = NULL;

	//原图像的灰度数学期望(expectation)
	m_nExpSrc = 128;
	
	//目标图像的灰度数学期望
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

//设置一个int
void CGrayProcess::SetParam1i(int nPercentage)
{
	m_nPercentage = nPercentage;
}

//设置4个BYTE
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

//设置目标直方图,
//保证 pnMatch 的长度不小于 256
void CGrayProcess::SetParam1iv(const int* pnMatch)
{
	if(m_pnMatchFreq) delete[] m_pnMatchFreq;
	else
		m_pnMatchFreq = new int[256];

	for(int i = 0; i < 256; i++)
		m_pnMatchFreq[i] = pnMatch[i];
}

//nExpDst目标期望
//nRatioVarPer方差比值(百分值)
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
	//第一步, 进行参数合法性检测
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//区间长度
	BYTE b_a = m_byB - m_byA;	// b - a
	BYTE d_c = m_byD - m_byC;	// d - c

	if(b_a == 0)b_a = 1;
	if(d_c == 0)d_c = 1;

	//第二步, 灰度修改
	for(int i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(int j = 0;j < w;j++)
		{
			//获取指针
			BYTE* pbyBlue = pbyRsc++;
			BYTE* pbyGreen = pbyRsc++;
			BYTE* pbyRed = pbyRsc++;
			pbyRsc++;

			//获取值;
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			
			float sum=r+g+b;
			float R=r,G=g,B=b;
			R=R/sum;
			G=G/sum;
			B=B/sum;
			//RBG转为HSV
			float lmax=max(R,G);
			lmax=max(B,lmax);
			float lmin=min(R,G);
			lmin=min(B,lmin);
			float H,S,V;
			V=lmax*255;
			S=(lmax-lmin)/lmax*255;
			//HSV颜色空间模型
			if(R == lmax)
				H =(G-B)/(lmax-lmin)* 60;
			if(G == lmax)
				H = 120+(B-R)/(lmax-lmin)* 60;
			if(B == lmax)
				H = 240 +(R-G)/(lmax-lmin)* 60;
			if(H < 0)
				H = H+ 360;
			H = H/360*255;
			//红色相近
			if(V>25 && S>=50 && ((H>=0 && H<20) || (H>230 && H<=255)))
			{
				*pbyBlue = 255;  //B
				*pbyGreen = 255;  //G
				*pbyRed = 255;  //R     
			}
			//黄色相近
			else if(H>21 && H<64 && S>70 && V>102)
			{
				*pbyBlue = 255;  //B
				*pbyGreen = 255;  //G
				*pbyRed = 255;  //R
			}
			//蓝色相近
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
	//第一步, 进行参数合法性检测
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//区间长度
	BYTE b_a = m_byB - m_byA;	// b - a
	BYTE d_c = m_byD - m_byC;	// d - c

	if(b_a == 0)b_a = 1;
	if(d_c == 0)d_c = 1;

	//第二步, 灰度修改
	for(int i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(int j = 0;j < w;j++)
		{
			//第一小步, 获取指针
			BYTE* pbyBlue = pbyRsc++;
			BYTE* pbyGreen = pbyRsc++;
			BYTE* pbyRed = pbyRsc++;
			pbyRsc++;

			//第二小步, 获取值;
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			

			BYTE gray = (BYTE)(((WORD)r * 30 + (WORD)g * 59 + (WORD)b * 11) / 100);
			//第三小步, 处理单个像素值
			switch(m_dwOperation)
			{
				//将彩色转换为灰度图像
				case IMAGE_GRAY_COLOR_GRAYED_OUT:
				{
					*pbyBlue = gray;
					*pbyGreen = gray;
					*pbyRed = gray;	
					break;
				}
				//灰度比例化
				case IMAGE_GRAY_PERCENTAGE:
				{
					int nScale = ((int)gray * m_nPercentage) / 100;
					BYTE byResult = (BYTE)BOUND(nScale, 0, 255);
					*pbyBlue = byResult;
					*pbyGreen = byResult;
					*pbyRed = byResult;	
					break;
				}
				//灰度线性化:
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

				//灰度线性截断
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

				//灰度取反
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
	//第一步, 进行参数合法性检测
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//第二步, 灰度统计
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//子区域像素个数
	DWORD dwPixelSize = w * h;
	int i = 0;
	int j = 0;

	//开辟一个内存区, 记录指定区域的灰度值

	//存放被处理的子区域的灰度
	BYTE* pbyGraySubArea =  new BYTE[dwPixelSize];
	if(pbyGraySubArea == NULL) return FALSE;

	//定义一个数组, 大小为256, 记录256级灰度出现的频数
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
	
	//完成频率统计工作

	//将频数(Frequencies)进行累加
	DWORD adwAccumuSrc[256];
	adwAccumuSrc[0] = adwFreqSrc[0];
	for(i = 1; i < 256; i++)
		adwAccumuSrc[i] = adwAccumuSrc[i - 1] + adwFreqSrc[i];
	
	//最后记录均衡后的灰度, 明显地, 其等级少于256或灰度得到提高
	for(i = 0; i < 256; i++)
		adwAccumuSrc[i] = ((adwAccumuSrc[i] * 255) / dwPixelSize);
	

	//第三步, 灰度修改
	//重新开始
	dwBaseIndex = y * dwWidthBytes + 4 * x;
	pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			//原像素的灰度
			BYTE byGraySrc = *pbyGrayCopy++;
			//原像素的最终灰度
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

//直方图规定化, 应用步骤:
//第一, 给定灰度出现的频数, 采用成员函数设定灰度频数, 灰度级必须为256级
//第二, 进行灰度直方图规定化
BOOL CGrayProcess::MakeHistogramMatch(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//第一步, 进行参数合法性检测
	if(m_pnMatchFreq == NULL) return FALSE;
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//第二步, 对原图像灰度进行统计
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//子区域像素个数
	DWORD dwPixelSize = w * h;
	int i = 0;
	int j = 0;

	//开辟一个内存区, 记录指定区域的灰度值
	
	//记录子区域的灰度
	BYTE* pbyGraySubArea =  new BYTE[dwPixelSize];
	if(pbyGraySubArea == NULL) return FALSE;

	//定义一个数组, 大小为256, 记录256级灰度出现的频数--源
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
	
	//完成频率统计工作

	//将频数进行累加
	DWORD adwAccumSrc[256];
	adwAccumSrc[0] = adwFreqSrc[0];
	for(i = 1; i < 256; i++)
		adwAccumSrc[i] = adwAccumSrc[i - 1] + adwFreqSrc[i];
	
	//最后记录均衡后的灰度, 明显地, 其等级少于256或灰度得到提高
	for(i = 0; i < 256; i++)
		adwAccumSrc[i] = ((adwAccumSrc[i] * 255) / dwPixelSize);
	
	//第三步, 建立映射.

	//首先, 累积规定的频数, 从而获得规定的直方图
	DWORD adwAccumDst[256];
	adwAccumDst[0] = (DWORD)m_pnMatchFreq[0];
	for(i = 1; i < 256; i++)
		adwAccumDst[i] = adwAccumDst[i - 1] + (DWORD)m_pnMatchFreq[i];
	if(adwAccumDst[255] == 0)adwAccumDst[255] = 1;
	for(i = 0; i < 255; i++)
		adwAccumDst[i] = ((adwAccumDst[i] * 255) / adwAccumDst[255]);
	adwAccumDst[255] = 255;

	//建立映射:----一个逆过程
	adwAccumSrc[0] = SearchBinary((int*)adwAccumDst, adwAccumSrc[0], 0, 255);
	
	//优化的快速查找方法
	for(i = 1;i < 256;i++)
		adwAccumSrc[i] = SearchBinary((int*)adwAccumDst, adwAccumSrc[i], adwAccumSrc[i - 1], 255);
		
	//第四步, 灰度修改
	//重新开始
	dwBaseIndex = y * dwWidthBytes + 4 * x;
	pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			//原像素的灰度
			BYTE byGraySrc = *pbyGrayCopy++;
			//原像素的最终灰度
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

//邻近二分查找, 返回最近索引
//nValue----待查找的值
//nStart----开始序号
//nEnd-----结束序号
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
		//区间右端点
		int nRight = nEnd;
		//区间左端点
		int nLeft = nStart;		

	
		//小于等于左端点的值
		if(nValue <= pnArray[nLeft]) return nLeft;
		//大于等于右端点的值
		if(nValue >= pnArray[nRight]) return nRight;
		
		nRight--;
		nLeft++;

		//区间中点
		int nMiddle = (nRight + nLeft) / 2;

		//是否发现适度的区间
		BOOL bFound = FALSE;
		
		//返回值
		int nReturn = -1;

		//待查找之数一定在[nLeft - 1, nRight + 1]区间内
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
				//找到了
				if(nValue >= pnArray[nMiddle - 1])
				{
					bFound = TRUE;
					//比较
					if((nValue - pnArray[nMiddle - 1]) < (pnArray[nMiddle] - nValue))
						nReturn = nMiddle - 1;
					else
						nReturn = nMiddle;
				}
				//没有找到, 不在该区间
				else
					nRight = nMiddle - 1;
			}

			//nValue > pnArray[middle]
			else
			{
				//找到了
				if(nValue < pnArray[nMiddle + 1])
				{
					bFound = TRUE;
					//比较
					if((nValue - pnArray[nMiddle]) < (pnArray[nMiddle + 1] - nValue))
						nReturn = nMiddle;
					else
						nReturn = (nMiddle + 1);
				}
				
				//没有找到, 不在该区间
				else
					nLeft = nMiddle + 1;
			}//nValue >= pnArray[middle]
			nMiddle = (nRight + nLeft) / 2;
		
		}//end -- while
		
		return nReturn;
	}// end -- else -- nLength >= 3
}

//按Wallis and Jong-Sen Lee的方法计算数学期望
int CGrayProcess::GetGrayExpectation(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//第一步, 进行参数合法性检测
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//第二步, 计算灰度数学期望
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//数学期望累积
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

//进行统计匹配
BOOL CGrayProcess::MakeStatisticMatch(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)
{
	ASSERT(lpbyBits32);
	//第一步, 进行参数合法性检测
	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	//有效区域的宽度和高度
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)return FALSE;

	//第二步, 灰度统计
	//行字节数
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//开始数据基索引
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//子区域像素个数
	DWORD dwPixelSize = w * h;
	int i = 0;
	int j = 0;

	//开辟一个内存区, 记录指定区域的灰度值
	BYTE* pbyGraySubArea =  new BYTE[dwPixelSize];
	if(pbyGraySubArea == NULL) return FALSE;

	//记录灰度数学期望(平均值, 按Wallis and Jong-Sen Lee的方法)
	int nExpGray = 0;

	//求出灰度和数学期望(即, 平均值)
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
	
	//最后数学期望:
	nExpGray /= dwPixelSize;	

	//第三步, 灰度修改
	//重新开始
	dwBaseIndex = y * dwWidthBytes + 4 * x;
	pbyGrayCopy = pbyGraySubArea;
	for(i = 0;i < h;i++)
	{
		BYTE* pbyRsc = lpbyBits32 + dwBaseIndex;

		for(j = 0;j < w;j++)
		{
			//原像素的灰度
			BYTE byGraySrc = *pbyGrayCopy++;
			
			//原像素的最终灰度
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


