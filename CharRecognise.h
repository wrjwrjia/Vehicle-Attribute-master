/////////////////////////////////////////////////////////////////////////////////
//
//	CharRecognize.h: interface for the CCharRecognize class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CCharRecognise_H
#define _CCharRecognise_H
#include "ImageProcess.h"

class CCharRecognise : public CImageProcess
{
DECLARE_DYNAMIC(CCharRecognise)

public:
	CCharRecognise();
	virtual ~CCharRecognise();

private:


public:
	/*** 申请1维双精度实数数组 ***/
	float *alloc_1d_dbl(int n)
	{
		float *new1;
	
		new1 = (float *) malloc ((unsigned) (n * sizeof (float)));
		if (new1 == NULL) 
			{
				printf("ALLOC_1D_DBL: Couldn't allocate array of doubles\n");
				return (NULL);
			}
		return (new1);
	}

	/*** 申请2维双精度实数数组 ***/
	float **alloc_2d_dbl(int m, int n)
	{
		int i;
		float **new1;
	
		new1 = (float **) malloc ((unsigned) (m * sizeof (float *)));
		if (new1 == NULL)
			{
				//	printf("ALLOC_2D_DBL: Couldn't allocate array of dbl ptrs\n");
				return (NULL);
			}
	
		for ( i = 0; i < m; i++ ) 
			{
				new1[i] = alloc_1d_dbl(n);
			}
	
		return (new1);
	}

	float * * TZTQ_13(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  int num,  int dim);
	void CodeRecognize( float * * feature, int num );

public:
	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
	#endif
};
#endif