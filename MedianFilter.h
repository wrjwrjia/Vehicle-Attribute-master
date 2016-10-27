/////////////////////////////////////////////////////////////////////////////////
//
// MedianFilter.h: interface for the CMedianFilter class.
//
////////////////////////////////////////////////////////////////////////////////
// 版权所有(2002)
// Copyright(2002)
// 编写者: 向世明
// Author: Xiang Shiming


#ifndef _CMEDIANFILTER_H
#define _CMEDIANFILTER_H

#ifndef _CIMGAREAPROCESS_H
#include "ImageAreaProcess.h"
#endif
	 
class CMedianFilter  : public CImageAreaProcess
{
DECLARE_DYNAMIC(CMedianFilter)
public:
	CMedianFilter();
	virtual ~CMedianFilter();

protected:
	PIXELCOLORRGB FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight);
	PIXELCOLORRGB FilterPixelOnBorder(LPBYTE lpbyBitsSrc32, int x, int y, int nScanWidth, int nScanHeight);
	void QuickSort(BYTE* pbyArray, int nFirst, int nLast);

protected:
	//矩阵蒙板
	int* m_pnMask;
	//蒙板中非0元素的个数
	int m_nMaskBit;

public:
	BOOL Medianize(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	void SetMask(const int* pnMask, int nRows, int nCols);

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};

#endif 
