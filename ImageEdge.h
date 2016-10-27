/////////////////////////////////////////////////////////////////////////////////
//
//	ImageEdge.h: interface for the CImageEdge class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CImageEdge_H
#define _CImageEdge_H
#include "ImageProcess.h"

class CImageEdge : public CImageProcess
{
DECLARE_DYNAMIC(CImageEdge)

public:
	CImageEdge();
	virtual ~CImageEdge();

private:


public:
	BOOL FindJumpPoint(LPBYTE lpbyBits32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight);

public:
	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};
#endif
