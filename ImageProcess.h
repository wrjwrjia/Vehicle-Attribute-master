
/////////////////////////////////////////////////////////////////////////////////
//
//	ImageProcess.h: interface for the CImageProcess class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _CIMAGEPROCESS_H
#define _CIMAGEPROCESS_H
#include "Img.h"

typedef	struct
{
	int	left;
	int	right;
	int	bottom;
	int top;
}Rect;

class CImageProcess : public CObject 
{
DECLARE_DYNAMIC(CImageProcess)
public:
	CImageProcess();
	virtual ~CImageProcess();

protected:
	DWORD m_dwOperation;	//图像点处理操作类型
	DWORD m_dwColorModel;	//颜色模型

public:
	HBITMAP CreateDdb(HDC hDC, int nWidth,  int nHeight, const LPBYTE lpbyBits32);
	DWORD PutRGB(BYTE byRed,  BYTE byGreen,  BYTE byBlue);
	DWORD PutRGBA(BYTE byRed, BYTE byGreen, BYTE byBlue, BYTE byAlpha);
	BYTE GetAlpha(DWORD dwRGBA);
	BYTE GetBlue(DWORD dwRGBA);
	BYTE GetGreen(DWORD dwRGBA);
	BYTE GetRed(DWORD dwRGBA);
	
	DWORD GetColorModel() const;
	DWORD GetOperation() const;
	void SetColorModel(DWORD dwColorModel);
	virtual void SetOperation(DWORD dwOperation);

public:

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

};

#endif 
