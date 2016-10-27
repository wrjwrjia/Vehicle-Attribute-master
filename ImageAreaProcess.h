/////////////////////////////////////////////////////////////////////////////////
//
//	ImageAreaProcess.h: interface for the CImageAreaProcess class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CIMGAREAPROCESS_H
#define _CIMGAREAPROCESS_H

#ifndef _CIMAGEPROCESS_H
#include "ImageProcess.h"
#endif

class CImageAreaProcess : public CImageProcess 
{
DECLARE_DYNAMIC(CImageAreaProcess)
public:
	CImageAreaProcess();
	virtual ~CImageAreaProcess();

protected:
	int m_nCols;	//过滤单个像素的邻域区域的宽度		
	int m_nRows;   //过滤单个像素的邻域区域的高度

public:
	virtual void SetOperation(DWORD dwType);

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif

};

#endif 
