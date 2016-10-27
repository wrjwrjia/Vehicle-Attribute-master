/////////////////////////////////////////////////////////////////////////////////
//
//	GrayProcess.h: interface for the CGrayProcess class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _CGRAYPROCESS_H
#define _CGRAYPROCESS_H

#ifndef _CIMGPTPROCESS_H
#include "ImagePointProcess.h"
#endif

class CGrayProcess : public CImagePointProcess
{
DECLARE_DYNAMIC(CGrayProcess)  

public:
	CGrayProcess();
	virtual ~CGrayProcess();

private:
	
	int m_nPercentage;		//用于灰度线性化
	
	//区间[A, B]----->> 区间[C, D](映射)
	BYTE m_byA;
	BYTE m_byB;
	BYTE m_byC;
	BYTE m_byD;

	//用于直图规定化, 表示灰度频率
	int* m_pnMatchFreq;

	//用于灰度概率规定
	
	//原图像的灰度数学期望
	int m_nExpSrc;
	
	//目标图像的灰度数学期望
	int m_nExpDst;

private:
	

protected:
	int SearchBinary(const int *pnArray,  int nValue, int nStart, int nEnd);	
	BOOL MakeStatisticMatch(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	BOOL MakeHistogramMatch(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	BOOL MakeHistogramBalance(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	BOOL MakeLinearGray(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth, int nScanHeight);
	BOOL MakeHsvColorGray(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth, int nScanHeight);

public:
	BOOL MakeGray(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth, int nScanHeight);
	int GetGrayExpectation(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	void SetParam2i(int nExpDst, int nRatioVarPer);
	void SetParam1iv(const int* pnMatch);
	void SetParam4by(BYTE bya, BYTE byb, BYTE byc, BYTE byd);
	void SetParam1i(int nPercentage);

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};
#endif
