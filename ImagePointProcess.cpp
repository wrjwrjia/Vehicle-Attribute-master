#include "stdafx.h"
#include "ImagePointProcess.h"

IMPLEMENT_DYNAMIC(CImagePointProcess,CImageProcess)
CImagePointProcess::CImagePointProcess()
{}

CImagePointProcess::~CImagePointProcess()
{}

#ifdef _DEBUG
void CImagePointProcess::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImagePointProcess::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif

