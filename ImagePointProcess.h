////////////////////////////////////////////////////////////////////////////////
//
// ImagePointProcess.h: interface for the CImagePointProcess class.
//
//////////////////////////////////////////////////////////////////////


#ifndef _CIMGPTPROCESS_H
#define _CIMGPTPROCESS_H

#ifndef _CIMAGEPROCESS_H
#include "ImageProcess.h"
#endif

class CImagePointProcess  : public CImageProcess  
{
DECLARE_DYNAMIC(CImagePointProcess)
public:
	CImagePointProcess();
	virtual ~CImagePointProcess();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif
};

#endif 
