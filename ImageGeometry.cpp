//ͼ�񼸺α任

#include "stdafx.h"
#include "ImageGeometry.h"
#include "math.h"

IMPLEMENT_DYNAMIC(CImageGeometry, CImageProcess)
CImageGeometry::CImageGeometry()
{
	m_dwOperation = IMAGE_GEOMETRY_TRANSLATE;
	m_dwQuality = IMAGE_GEOMETRY_NEAREST_NEIGHBOR_INTERPOLATE;
}

CImageGeometry::~CImageGeometry()
{

}

#ifdef _DEBUG
void CImageGeometry::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImageGeometry::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif


void CImageGeometry::SetQuality(DWORD dwQuality)
{
	m_dwQuality = dwQuality;
}

///lpbySrcXY----����Դ����(x, y)�ĵ�ַ, 
//x, y, ��������任��õ��Ķ�Ӧ��ԭͼ�����������
//nScanWidth,  int nScanHeight, Դɨ���Ⱥ�ɨ��߶�
PIXELCOLORRGB CImageGeometry::Interpolate(LPBYTE lpbySrcXY,  int x,  int y,  float fu,  float fv,  int nScanWidth,  int nScanHeight)
{ // ��ֵ����
	PIXELCOLORRGB rgb;

	//���ֽ���, ���Խ�dwWidthBytes��Ϊ�������ݹ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	
	switch(m_dwQuality)
	{
		case IMAGE_GEOMETRY_NEAREST_NEIGHBOR_INTERPOLATE :
		{
			BYTE* pbySrc = lpbySrcXY;
			rgb.blue = *pbySrc++;
			rgb.green = *pbySrc++;
			rgb.red = *pbySrc++;
			break;
		}
		case IMAGE_GEOMETRY_BILINEAR_INTERPOLATE :
		{
			//���ڵ��ĸ����������½ǵ��x, y����ƫ����
			int nx = 1;
			int ny = 1;
			if((x + 1) > (nScanWidth - 1))
				nx = 0;
			if((y + 1) > (nScanHeight - 1))
				ny = 0;
			
			//�����ĸ����ص�����ֵ
			BYTE abyRed[2][2], abyGreen[2][2], abyBlue[2][2];
			
			//���ص�(x, y)������λ��
			BYTE* pbySrc = lpbySrcXY;
			//��ȡ������ֵ
			abyBlue[0][0] = *pbySrc++;
			abyGreen[0][0] = *pbySrc++;
			abyRed[0][0] = *pbySrc++;
			
			pbySrc = (lpbySrcXY + nx * 4);
			abyBlue[1][0] = *pbySrc++;
			abyGreen[1][0] = *pbySrc++;
			abyRed[1][0] = *pbySrc++;

			
			//ָ����һ������
			BYTE* pbySrcTemp = (lpbySrcXY + ny * dwWidthBytes);

			pbySrc = pbySrcTemp;
			abyBlue[0][1] = *pbySrc++;
			abyGreen[0][1] = *pbySrc++;
			abyRed[0][1] = *pbySrc++;

			pbySrc = pbySrcTemp + (4 * nx);
			abyBlue[1][1] = *pbySrc++;
			abyGreen[1][1] = *pbySrc++;
			abyRed[1][1] = *pbySrc++;
			
			rgb.red = (BYTE)(BOUND(((1 - fu) * (1 - fv) * ((float)abyRed[0][0]) + (1 - fu) * fv * ((float)abyRed[0][1]) + fu * (1 - fv) * ((float)abyRed[1][0]) + fu * fv * ((float)abyRed[1][1])), 0, 255));
			rgb.green = (BYTE)(BOUND(((1 - fu) * (1 - fv) * ((float)abyGreen[0][0]) + (1 - fu) * fv * ((float)abyGreen[0][1]) + fu * (1 - fv) * ((float)abyGreen[1][0]) + fu * fv * ((float)abyGreen[1][1])), 0, 255));
			rgb.blue = (BYTE)(BOUND(((1 - fu) * (1 - fv) * ((float)abyBlue[0][0]) + (1 - fu) * fv * ((float)abyBlue[0][1]) + fu * (1 - fv) * ((float)abyBlue[1][0]) + fu * fv * ((float)abyBlue[1][1])), 0, 255));
			break;
		}
		
		case IMAGE_GEOMETRY_THREE_ORDER_INTERPOLATE :
		{
			//��������
			int xx[4], yy[4];
			//�����ĸ����ص�����ֵ
			BYTE abyRed[4][4], abyGreen[4][4], abyBlue[4][4];

			xx[0] = -1;  xx[1] = 0; xx[2] = 1; xx[3] = 2;
			yy[0] = -1;  yy[1] = 0; yy[2] = 1; yy[3] = 2;

			//��֤�Ϸ�
			if((x - 1) < 0) 
				xx[0] = 0;
			if((x + 1) > (nScanWidth - 1)) 
				xx[2] = 0;
			if((x + 2) > (nScanWidth - 1)) 
				xx[3] = ((xx[2] == 0) ? 0 : 1);

			if((y - 1) < 0) 
				yy[0] = 0;
			if((y + 1) > (nScanHeight - 1)) 
				yy[2] = 0;
			if((y + 2) > (nScanHeight - 1)) 
				yy[3] = ((yy[2] == 0) ? 0 : 1);

			//���ص�(x, y)������λ��
			//��ȡ����
			int i;
			for(i = 0;i < 4;i++)
			{
				//���ص�(x, y)������λ��
				BYTE* pbySrcBase = lpbySrcXY + yy[i] * dwWidthBytes;

				for(int j = 0;j < 4;j++)
				{
					BYTE* pbySrc = pbySrcBase + 4 * xx[j];
					abyBlue[i][j] = *pbySrc++;
					abyGreen[i][j] = *pbySrc++;
					abyRed[i][j] = *pbySrc++;
				}
			}

			//u, v����
			float afu[4], afv[4];

			afu[0] = Sinxx(1.0f + fu);
			afu[1] = Sinxx(fu);
			afu[2] = Sinxx(1.0f - fu);
			afu[3] = Sinxx(2.0f - fu);

			afv[0] = Sinxx(1.0f + fv);
			afv[1] = Sinxx(fv);
			afv[2] = Sinxx(1.0f - fv);
			afv[3] = Sinxx(2.0f - fv);

			//������������м�ֵ
			float afRed[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			float afGreen[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			float afBlue[4] = {0.0f, 0.0f, 0.0f, 0.0f};

			for(i = 0;i < 4;i++)
			{
				for(int j = 0;j < 4;j++)
				{
					afRed[i] += afv[j] * abyRed[j][i];
					afGreen[i] += afv[j] * abyGreen[j][i];
					afBlue[i] += afv[j] * abyBlue[j][i];				
				}
			}
			rgb.red = (BYTE)(BOUND((afu[0] * afRed[0] + afu[1] * afRed[1] + afu[2] * afRed[2] + afu[3] * afRed[3]), 0, 255));
			rgb.green = (BYTE)(BOUND((afu[0] * afGreen[0] + afu[1] * afGreen[1] + afu[2] * afGreen[2] + afu[3] * afGreen[3]), 0, 255));
			rgb.blue = (BYTE)(BOUND((afu[0] * afBlue[0] + afu[1] * afBlue[1] + afu[2] * afBlue[2] + afu[3] * afBlue[3]), 0, 255));
			break;
		}
		default : break;
	}
	return rgb;
}

//�ú�������sin(x) / (x)
float CImageGeometry::Sinxx(float fx)
{
	float fabsx = ABS(fx);
	float fxx = fabsx * fabsx;
	float fxxx = fxx * fabsx;
	if(fabsx < 1.0f) 
		return (1.0f - 2.0f * fxx + fxxx);
	else if(fabsx < 2.0f) 
		return (4.0f - 8.0f * fabsx + 5.0f * fxx - fxxx); 
	else 
		return 0.0f;
}

//lpbyBitsSrc32----����Դ����ֵ, 
//x, y, nWidth,  nHeight, ��������Ŀ��
//nScanWidth,  nScanHeight, ɨ���Ⱥ�ɨ��߶�
//lpbyBitsDst32----���������ź�Ľ��
//nWidthImgDst, nHeightImgDst���������
BOOL CImageGeometry::Scale(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32, int nWidthImgDst, int nHeightImgDst)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);
	ASSERT(nWidthImgDst != 0);
	ASSERT(nHeightImgDst != 0);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;

	m_dwOperation = IMAGE_GEOMETRY_SCALE;
	//��Ч����Ŀ�Ⱥ͸߶�
	int w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);

	//ע������:
	//��һ:
	//���(w <  nWidth), ����(h <  nHeight)���ʾָ��������ܹ���Ч��ȡ���ݵ�����Ҫ��, 
	//��ʱ���򽫷Ŵ���, ʹ������ŵĽ�����ܴﵽ nWidthImgDst ��� nHeightImgDst ��
	//�ڶ�:	
	//fScalex, fScaley����ʾ�����ű�Ϊ��ʵ���űȵĵ���
	//֮������������������, ��һ�γ���, �ܱ�һ�γ˷�Ҫ��.

	//������ű�
	float fScalex = (float)w/(float)nWidthImgDst;
	float fScaley = (float)h/(float)nHeightImgDst;

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * dwWidthBytes + 4 * x;

	//ָ��Ŀ������
	BYTE* pbyDst = lpbyBitsDst32;
	//��ɱ任
	for(int i = 0; i < nHeightImgDst; i++)
	{
		//����任���õĸ���yֵ
		float fYInverse = i * fScaley;
		//ȡ��
		int yy = (int)fYInverse;
		//�����ֵ
		float fv = fYInverse - yy;
		//��Ӧ��ԭͼ���y����
		yy += y;
		BYTE* pbySrc = lpbyBitsSrc32 + yy * dwWidthBytes;

		for(int j = 0;j < nWidthImgDst;j++)
		{
			//����任���õĸ���xֵ
			float fXInverse = j * fScalex;
			//ȡ��
			int xx = (int)fXInverse;
			//�����ֵ
			float fu = fXInverse - xx;
			//��Ӧ��ԭͼ���y����
			xx += x;	
			//��ȡ����
			BYTE* pbyCurrent =  pbySrc + 4 * xx;
			PIXELCOLORRGB rgb = Interpolate(pbyCurrent, xx, yy, fu, fv, nScanWidth,  nScanHeight);
			
			*pbyDst++ = rgb.blue;
			*pbyDst++ = rgb.green;
			*pbyDst++ = rgb.red;
	
			//alpha�������ⲿ���.
			pbyDst++;	
		}
	}
	return TRUE;
}

//˳ʱ����ת90��
//���� n * 360 - 90 �� 
//x, y, nWidth,  nHeight-----����������
//lpbyBitsDst32----��СΪnWidth * nHeight * 4;
//nScanWidth-------Դͼ��ɨ����
//nScanHeight------Դͼ��ɨ��߶�
BOOL CImageGeometry::RotateCW90(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_CW_90;

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;		//No 10

	//ָ���������һ�е�һ�д������ݵ�ַ.
	BYTE* pbySrcBase = lpbyBitsSrc32 + (y + nHeight - 1) * dwWidthBytes + 4 * x;
	
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nWidth;i++)
	{
		//�ӵ����ϰ��л�ȡ����
		//�������ƶ�
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nHeight;j++)
		{
			
			BYTE* pbyCurrent = pbySrc;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			pbyDst++;
			pbySrc -= dwWidthBytes;
		}
		pbySrcBase += 4;
	}
	return TRUE;
}

//��ʱ����ת90��
//lpbyBitsDst32----��СΪnWidth * nHeight * 4;
BOOL CImageGeometry::RotateCCW90(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_CCW_90;

	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;


	//ָ�������һ�����һ�п�ʼ��ַ.
	BYTE* pbySrcBase = lpbyBitsSrc32 + y * dwWidthBytes + 4 * (x + nWidth - 1);
	
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nWidth;i++)
	{
		//�������°��л�ȡ����
		//�������ƶ�
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nHeight;j++)
		{
			
			BYTE* pbyCurrent = pbySrc;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			*pbyDst++ = *pbyCurrent++;
			pbyDst++;
			pbySrc += dwWidthBytes;
		}
		pbySrcBase -= 4;
	}
	return TRUE;
}

BOOL CImageGeometry::Rotate180(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_180;

	//ָ���������һ�����һ�п�ʼ��ַ.
	BYTE* pbySrcBase = lpbyBitsSrc32 + (y + nHeight - 1) * dwWidthBytes + 4 * (x + nWidth - 1);
	
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nHeight;i++)
	{
		//�������ƶ�
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nWidth;j++)
		{
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			pbyDst++;
			pbySrc -= 7;
		}
		pbySrcBase -= dwWidthBytes;
	}
	return TRUE;
}

//��ת..., -360, 0, 360, ...��
BOOL CImageGeometry::Rotate0(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE_0;
	//���ֽ���(Դͼ��)
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//���ֽ���(Ŀ����ͼ��)
	DWORD dwWidtgBytesDst = (DWORD)nWidth * 4;
	//ָ�������һ�е�һ�п�ʼ��ַ.
	BYTE* pbySrcBase = lpbyBitsSrc32 + y * dwWidthBytes + 4 * x;
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0; i < nHeight; i++)
	{
		::CopyMemory(pbyDst, pbySrcBase, dwWidtgBytesDst); 
		pbySrcBase += dwWidthBytes;
		pbyDst += dwWidtgBytesDst;
	}
	return TRUE;
}


//lpbyBitsSrc32----����Դ����ֵ, 
//x, y, nWidth,  nHeight, ��������Ŀ��
//nScanWidth,  nScanHeight, ɨ���Ⱥ�ɨ��߶�
//lpbyBitsDst32----���������ź�Ľ��
//fTheta ---- ��ת�Ƕ�
//nWidthImgDst, nHeightDst��ת��ĳߴ�, ��ʵ�����ǿ���ͨ��nWidth,  nHeight��fTheta�������.
BOOL CImageGeometry::Rotate(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32, float fTheta, int nWidthImgDst, int nHeightImgDst)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�.
	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_ROTATE;
	//���Ƕ�ת���ɻ���
	float fRadTheta = (float)(fTheta * PIE / 180.0);
	//���������Ǻ���ֵ
	float fSinTheta = (float)sin(fRadTheta);
	float fCosTheta = (float)cos(fRadTheta);

	//���ȿ����Ƿ�����ת����ĽǶ�.
	if((nWidthImgDst == nHeight) && (nHeightImgDst == nWidth))
	{
		// ...,  -270,  90,  450, ..., 
		if(fSinTheta > 0.99f)
			return RotateCCW90(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
		// ...,  -450,  -90,  270, ..., 
		else if(fSinTheta < -0.99f)
			return RotateCW90(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
	}
	if((nWidthImgDst == nWidth) && (nHeightImgDst == nHeight))
	{
		// ..., -540, -180, 180, 540, ..., 
		if(fCosTheta < -0.99f)
			return Rotate180(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
		// ..., -360, 0, 360, 
		else if (fCosTheta > 0.99f)
			return Rotate0(lpbyBitsSrc32, x, y, nWidth, nHeight, nScanWidth, nScanHeight, lpbyBitsDst32);
	}

	//���������Ƕ�

	//������ת����, �����ԭͼ������ϵͳ.
	int nAbsCenterX = x + nWidth / 2;
	int nAbsCenterY = y + nHeight / 2;
	//�����ת����, ���Ǿ�����ͬ��x, y����.�����Ŀ��ͼ��
	int nRelCenterX = nWidthImgDst / 2;
	int nRelCenterY = nHeightImgDst / 2;
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;

	//��ʼ����
	//���н���ɨ�����
	for(int i = 0;i < nHeightImgDst;i++)//y
	{
		//����ֱ������ϵ��, ����ֵΪ i ���������ʾ
		int yy = nRelCenterY - i;
		for(int j = 0; j < nWidthImgDst;j++)//x
		{
			//����ֱ������ϵ��, ����ֵΪ j �ĺ������ʾ
			int xx = j - nRelCenterX;
			//���з���任, ��ת�Ƕ�: -theta
			float fXInverse = xx * fCosTheta + yy * fSinTheta;
			float fYInverse = yy * fCosTheta - xx * fSinTheta;

			//�����������ϵ
			fYInverse = -fYInverse;
			fXInverse += nAbsCenterX;
			fYInverse += nAbsCenterY;

			//��������ֵ
			int nNearX = (int)fXInverse;
			int nNearY = (int)fYInverse;
			if((nNearX >= x) && (nNearX <= (x + nWidth - 1)) && (nNearY >= y) && (nNearY <= (y + nHeight - 1)))
			{
				//��ֵ����
				float fu = fXInverse - nNearX;
				float fv = fYInverse - nNearY;
				//���ڵ�ĵ�ַ
				BYTE* pbySrc = lpbyBitsSrc32 + nNearY * dwWidthBytes + 4 * nNearX;
				PIXELCOLORRGB rgb = Interpolate(pbySrc, nNearX, nNearY, fu, fv, nScanWidth,  nScanHeight);
				*pbyDst++ = rgb.blue;
				*pbyDst++ = rgb.green;
				*pbyDst++ = rgb.red;
				pbyDst++;
			}
			else
				pbyDst += 4;
		}
	}
	return TRUE;
}

//ˮƽ����,���Ǿ�����Ϊ Y ��
BOOL CImageGeometry::MirrorY(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�
	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}
	m_dwOperation = IMAGE_GEOMETRY_MIRROR_Y;
	//���ֽ���
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//ָ���������һ�����һ�п�ʼ��ַ.
	BYTE* pbySrcBase = lpbyBitsSrc32 + y * dwWidthBytes + 4 * (x + nWidth - 1);
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nHeight;i++)
	{
		//�������°��л�ȡ����
		//�������ƶ�
		BYTE* pbySrc = pbySrcBase;
		for(int j = 0;j < nWidth;j++)
		{
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			*pbyDst++ = *pbySrc++;
			pbyDst++;
			pbySrc -= 7;
		}
		pbySrcBase += dwWidthBytes;
	}
	return TRUE;
}

//��ֱ����,���Ǿ�����Ϊ X ��
BOOL CImageGeometry::MirrorX(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPBYTE lpbyBitsDst32)
{
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBitsSrc32);
	ASSERT(lpbyBitsDst32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) 
		return FALSE;
	
	//��Ч����Ŀ�Ⱥ͸߶�
	//��ע��, ��ʱ, ��Ⱥ͸߶Ȳ������ⳬ����Ч��Ⱥ͸߶�.

	if(nWidth > (nScanWidth - x)) 
	{
		::AfxMessageBox("The width of the area you defined is not obtained!");
		return FALSE;
	}
	if(nHeight > (nScanHeight - y)) 
	{
		::AfxMessageBox("The height of the area you defined is not obtained!");
		return FALSE;
	}

	m_dwOperation = IMAGE_GEOMETRY_MIRROR_X;

	//���ֽ���(Դͼ��)
	DWORD dwWidthBytes = (DWORD)nScanWidth * 4;
	//���ֽ���(Ŀ����ͼ��)
	DWORD dwWidtgBytesDst = (DWORD)nWidth * 4;
	//ָ���������һ�е�һ�п�ʼ��ַ.
	BYTE* pbySrcBase = lpbyBitsSrc32 + (y + nHeight - 1) * dwWidthBytes + 4 * x;
	//Ŀ�ĵص�ַ
	BYTE* pbyDst = lpbyBitsDst32;
	for(int i = 0;i < nHeight;i++)
	{
		//���������л�ȡ����
		::CopyMemory(pbyDst, pbySrcBase, dwWidtgBytesDst); 
		pbyDst += dwWidtgBytesDst;
		pbySrcBase -= dwWidthBytes;
	}
	return TRUE;
}
