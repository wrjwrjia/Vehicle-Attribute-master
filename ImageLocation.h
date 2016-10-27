/////////////////////////////////////////////////////////////////////////////////
//
//	ImageLocation.h: interface for the CImageLocation class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CImageLocation_H
#define _CImageLocation_H
#include "ImageProcess.h"

class CImageLocation : public CImageProcess
{
DECLARE_DYNAMIC(CImageLocation)

public:
	CImageLocation();
	virtual ~CImageLocation();

private:


public:
	bool LocateMark(LPBYTE lpbyBits32, int x, int y, int nWidth, int nHeight, int nScanWidth, int nScanHeight, LPRECT  p_markRect);

public:
	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};
#endif
