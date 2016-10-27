/////////////////////////////////////////////////////////////////////////////////
//
//	CharPretreat.h: interface for the CCharPretreat class.
//
////////////////////////////////////////////////////////////////////////////////
#define HDIB HANDLE

#include <iostream>
#include <deque>
using namespace std;

typedef deque<CRect> CRectLink;
typedef deque<HDIB>  HDIBLink;

#ifndef _CCharPretreat_H
#define _CCharPretreat_H
#include "ImageProcess.h"

class CCharPretreat : public CImageProcess
{
DECLARE_DYNAMIC(CCharPretreat)

public:
	CCharPretreat();
	virtual ~CCharPretreat();

private:
	CRectLink m_charRectCopy;
	int	digicount;
	int m_lianXuShu;

public:
	void SlopeAdjust(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	CRectLink CharSegment(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	void StdDIBbyRect(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  CRectLink &m_charRect);
	BOOL AutoAlign(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32,  CRectLink &m_charRect);
	BOOL ThiningDIB(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32);
	void RemoveScatterNoise(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight);
	bool DeleteScaterJudge(LPBYTE lpbyBits32, long OneLineBytes, LPBYTE lplab, int nWidth, int nHeight, int x, int y, CPoint lab[], int lianXuShu);

public:
	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};
#endif