/////////////////////////////////////////////////////////////////////////////////
//
// ImageGeometry.h: interface for the CImageGeometry class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _CIMGGEOMETRY_H
#define _CIMGGEOMETRY_H

#ifndef _CIMAGEPROCESS_H
#include "ImageProcess.h"
#endif

class CImageGeometry : public  CImageProcess
{
DECLARE_DYNAMIC(CImageGeometry)
public:
	CImageGeometry();
	virtual ~CImageGeometry();

public:
	void SetQuality(DWORD dwQuality);

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif

public:
	
	BOOL MirrorX(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight, LPBYTE lpbyBitsDst32);
	BOOL MirrorY(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight, LPBYTE lpbyBitsDst32);

	BOOL Rotate(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight,LPBYTE lpbyBitsDst32,float fTheta,int nWidthImgDst,int nHeightImgDst);

	BOOL Rotate0(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight, LPBYTE lpbyBitsDst32);
	BOOL Rotate180(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight, LPBYTE lpbyBitsDst32);
	BOOL RotateCCW90(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight, LPBYTE lpbyBitsDst32);
	BOOL RotateCW90(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight,LPBYTE lpbyBitsDst32);

	BOOL Scale(LPBYTE lpbyBitsSrc32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight,LPBYTE lpbyBitsDst32,int nWidthImgDst,int nHeightImgDst);

protected:

	float Sinxx(float fx);
	PIXELCOLORRGB Interpolate(LPBYTE lpbySrcXY, int x, int y, float fu, float fv, int nScanWidth, int nScanHeight);

	DWORD m_dwQuality;
};

#endif 
