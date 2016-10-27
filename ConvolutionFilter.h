/////////////////////////////////////////////////////////////////////////////////
//
//	ConvolutionFilter.h: interface for the CConvolutionFilter class.
//
////////////////////////////////////////////////////////////////////////////////

//ͳһ�ľ��������

#ifndef _CCNVLTFILTER_H
#define _CCNVLTFILTER_H

#ifndef _CIMGAREAPROCESS_H
#include "ImageAreaProcess.h"
#endif
	 
class CConvolutionFilter  : public  CImageAreaProcess
{
DECLARE_DYNAMIC(CConvolutionFilter)
public:
	CConvolutionFilter();
	virtual ~CConvolutionFilter();

protected:

	//���ϵ��Ȩ��;
	int m_nKernelWeight;

	////�����, һ�� m_nRows ��, m_nCols �о���, ��һά�����ʾ
	int* m_pnKernel;	

protected:
	PIXELCOLORRGB FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight);
	PIXELCOLORRGB FilterPixelOnBorder(LPBYTE lpbyBitsSrc32, int x, int y, int nScanWidth, int nScanHeight);
	virtual PIXELCOLORRGB Convolute(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue,  int nNum);
	
public:
	void SetKernelWeight(int nKernelWeight);
	

	BOOL Filter(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	
	void SetKernel(const int* pnKernel, int nRows, int nCols);
	virtual void SetOperation(DWORD dwOperation);

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};

#endif 