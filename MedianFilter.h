/////////////////////////////////////////////////////////////////////////////////
//
// MedianFilter.h: interface for the CMedianFilter class.
//
////////////////////////////////////////////////////////////////////////////////
// ��Ȩ����(2002)
// Copyright(2002)
// ��д��: ������
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
	//�����ɰ�
	int* m_pnMask;
	//�ɰ��з�0Ԫ�صĸ���
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
