/////////////////////////////////////////////////////////////////////////////////
//
//	SpecialDetectionFilter.h: interface for the CSpecialDetectionFilter class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CSOBELKRISCHFLT_H
#define _CSOBELKRISCHFLT_H
#ifndef _CIMGAREAPROCESS_H
#include "ImageAreaProcess.h"
#endif

class CSpecialDetectionFilter: public CImageAreaProcess
{
DECLARE_DYNAMIC(CSpecialDetectionFilter)

public:
	CSpecialDetectionFilter();
	virtual ~CSpecialDetectionFilter();
	BOOL Filter(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);

protected:
	PIXELCOLORRGB  Sobel(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue, int nNum);
	PIXELCOLORRGB  Kirsch(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue, int nNum);
	PIXELCOLORRGB  Specialize(BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue, int nNum);
	PIXELCOLORRGB  FilterPixelOnBorder(LPBYTE lpbyBitsSrc32, int x, int y, int nScanWidth, int nScanHeight);
	PIXELCOLORRGB  FilterPixelInner(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nScanWidth,  int nScanHeight);

public:
	#ifdef _DEBUG
	virtual	void Dump(CDumpContext &dc)	const;
	virtual void AssertValid() const;
	#endif
};
#endif
