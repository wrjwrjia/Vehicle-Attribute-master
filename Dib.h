// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__6138FFBC_1C8D_4F78_9DA0_1120DFFCA860__INCLUDED_)
#define AFX_DIB_H__6138FFBC_1C8D_4F78_9DA0_1120DFFCA860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define HDIB HANDLE
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define   IS_WIN30_DIB(lpbi) \
		((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define PALVERSION   0x300
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)


class CDib : public CObject  
{
	DECLARE_DYNAMIC(CDib)

public:

	BOOL m_bValid;			//�Ƿ���Ч
    RGBQUAD* m_pRGBQuad;	//RGBQUAD ����
    BYTE* m_pDibData;		//λͼ��������
	LONG m_nWidth;			//���
	LONG m_nHeight;			//�߶�
	BYTE m_nBitCount;		//ÿ������ռ�е�λ��
    UINT m_nTotalColors;	//��ɫ����
	DWORD dwColorTableSize;
	char* m_pszFileName;
    BITMAPFILEHEADER* m_pBitmapFileHeader;
    BITMAPINFOHEADER* m_pBitmapInfoHeader;
    BITMAPINFO* m_pBitmapInfo;
	CPalette* m_pPalette;
	HDIB      m_hDib;
	HBITMAP   m_hBitmap;
	CBitmap*  m_pBitmap;

public:

	CDib();
    CDib(const char* pszDibFileName);
    ~CDib();

// ���Ժ���
public:
	LPSTR FindDIBBits(LPSTR lpbi);	
	LONG GetHeight();
	LONG GetWidth();

// ��������
	void Save(const char *pszDibFileName);

// ��ʾλͼ
	BOOL PaintDIB(HDC hDC,LPRECT  lpDCRect,LPRECT  lpDIBRect);
private:
	LONG DIBHeight(LPSTR lpDIB);
	LONG DIBWidth(LPSTR lpDIB);
	HDIB CropDIB(HDIB hDIB,LPRECT lpRect);

public:
	HDIB CropDIB(LPRECT lpRect);
    
	DWORD GetDdbData32(LPBYTE lpbyDdb32,HDIB hDIB);
	DWORD GetDdbData24(LPBYTE lpbyDdb24);
	void Format32DDBtoDiB(LPBYTE lpbySrcBits32,  int nScanWidth,  int nScanHeight,  LPBYTE lpTargetBits,  int m_nBitCount);

	DWORD GetDibWidthBytes(int nWidth,  BYTE byBitCount);
	BYTE GetBitCount() const;
    DWORD GetSize();
    LONG GetBITMAPWidth() const;
    LONG GetBITMAPHeight() const;
    UINT GetColorNumber() const;
	char* GetFileName() const;
	BOOL IsValid() const ;
    RGBQUAD* GetRGBQuad() const ;
    BYTE* GetDibData() const ;
    BITMAPINFO* GetBmpInfo() const;

public:
	void ClearMemory();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

protected:
	void LoadFile(const char* pszDibFileName);
private:
	void Init();
};

#endif // !defined(AFX_DIB_H__6138FFBC_1C8D_4F78_9DA0_1120DFFCA860__INCLUDED_)
