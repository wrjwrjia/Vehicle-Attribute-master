#include "stdafx.h"
#include "ImageAreaProcess.h"

IMPLEMENT_DYNAMIC(CImageAreaProcess, CImageProcess)
CImageAreaProcess::CImageAreaProcess()
{
	m_nRows = 1;		
	m_nCols = 1;
}

CImageAreaProcess::~CImageAreaProcess()
{
	
}

#ifdef _DEBUG
void CImageAreaProcess::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImageAreaProcess::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif

//÷ÿ‘ÿ–Èƒ‚SetOperation()∫Ø ˝
void CImageAreaProcess::SetOperation(DWORD dwOperation)
{
	m_dwOperation = dwOperation;
}


    