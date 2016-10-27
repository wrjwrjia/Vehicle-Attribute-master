/////////////////////////////////////////////////////////////////////////////////
//
//	ImageIncise.h: interface for the CImageIncise class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CImageIncise_H
#define _CImageIncise_H
#include "ImageProcess.h"

class CImageIncise : public CImageProcess
{
DECLARE_DYNAMIC(CImageIncise)

public:
	CImageIncise();
	virtual ~CImageIncise();

private:


public:
	BOOL ThresholdIncise(LPBYTE lpbyBits32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight);

public:
	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};
#endif
