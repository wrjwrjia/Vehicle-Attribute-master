#include "stdafx.h"
#include "CharPretreat.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCharPretreat, CImageProcess)
CCharPretreat::CCharPretreat()
{
	digicount = 0;
	m_lianXuShu = 0;
}

CCharPretreat::~CCharPretreat()
{
}

#ifdef _DEBUG
void CCharPretreat::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CCharPretreat::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif


void CCharPretreat::SlopeAdjust(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight) //ͨ����ͼ�����Ұ��ƽ���߶ȵ�ͳ����������б�ĵ���
{
	unsigned char*	lpSrc; // ָ��Դͼ���ָ��
	int	i, j;
	long OneLineBytes = (long)nWidth * 4; // ͼ��ÿ�е��ֽ���
	double leftaver = 0.0; //ͼ�����ߵ�ƽ���߶�
	double rightaver = 0.0; //ͼ���Ұ�ߵ�ƽ���߶�
	double slope; //ͼ�����б��
	long counts = 0; //ͳ��ѭ������
	//ɨ�����ߵ�ͼ�����ɫ���ص�ƽ���߶�
	for( i = 0; i < nHeight; i++ )
	{
		for( j = 0; j < nWidth/2; j++ )
		{
			lpSrc=(unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j; //ָ���i�е�j�����ص�ָ��	
			if(*lpSrc == 255)
			{
				counts = counts + nWidth/2 -j;
				leftaver = leftaver + i*(nWidth/2 -j);
			}
		}
	}
	leftaver = leftaver/counts; //����ƽ���߶�

	counts =0;
	//ɨ���Ұ�ߵ�ͼ�����ɫ���ص�ƽ���߶�
	for( i = 0; i < nHeight; i++ )
	{
		for( j = nWidth/2; j < nWidth; j++ )
		{
			lpSrc=(unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
			if(*lpSrc == 255)
			{
				counts = counts + nWidth -j;
				rightaver = rightaver + i*(nWidth -j);
			}
		}
	}
	rightaver = rightaver/counts; //�����Ұ�ߵ�ƽ���߶�

	slope = (leftaver - rightaver)/(nWidth/2); //����б��

	LPSTR lpNewDIBBits; //ָ���µ�ͼ��������ʼλ�õ�ָ��
	LPSTR lpDst; //ָ����ͼ���ָ��
	HLOCAL nNewDIBBits=LocalAlloc(LHND,OneLineBytes*nHeight); //��ͼ��ľ��
	lpNewDIBBits=(char*)LocalLock(nNewDIBBits); //�����ڴ�
	lpDst=(char*)lpNewDIBBits; //ָ����ͼ�����ص�ָ��
	memset(lpDst,(BYTE)0,OneLineBytes*nHeight); //Ϊ��ͼ�񸳳�ʼֵ
	int gray; //���ص�ĻҶ�ֵ
	int i_src; //λ��ӳ��ֵ
	//����б�ʣ��ѵ�ǰ��ͼ��ĵ�ӳ�䵽Դͼ��ĵ�
	for( i = 0; i < nHeight; i++ )
	{
   		for( j = 0; j < nWidth; j++ )
		{	
			i_src = int(i - (j-nWidth/2)*slope); //����ӳ��λ��
			if( i_src <0 || i_src >= nHeight ) //�������ͼ���⣬�����ú�ɫ
				gray = 0;
			else
			{	
				lpSrc=(unsigned char *)lpbyBits32 + OneLineBytes *  i_src + 4 * j;
				gray = *lpSrc;
			}			
			//����ͼ��ĵ��õõ�������ֵ���
			lpDst = (char *)lpNewDIBBits + OneLineBytes * i + 4 * j;
			*lpDst++ = gray;
			*lpDst++ = gray;
			*lpDst++ = gray;
		}
	}
	memcpy(lpbyBits32,lpNewDIBBits,OneLineBytes*nHeight); //���µ�ͼ������ݿ������ɵ�ͼ����
}

CRectLink CCharPretreat::CharSegment(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight) //����ֵ��ͼ���д�ʶ����ַ����������������ش�Ÿ����ַ���λ����Ϣ������
{
	//�����������ÿ���ַ����������
	CRectLink charRect1,charRect2;
	charRect1.clear();
	charRect2.clear();

	BYTE pixel; //����ֵ
	BYTE* lpSrc;//ͼ����������
	long	OneLineBytes = (long)nWidth * 4;//����ͼ��ÿ�е��ֽ���
	DWORD dwBaseIndex = y * OneLineBytes + 4 * x;//��ʼ���ݻ�����
	int top,bottom;//�������±߽���������
	int i, j;
	digicount = 0;//����ͳ��ͼ�����ַ������ļ�����

	//��������ɨ�裬�ҵ��ϱ߽�
	dwBaseIndex = (nHeight - 1) * OneLineBytes;
	for( i = 0; i < nHeight; i++ )
	{
  		for( j = 0; j < nWidth; j++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * i + 4 * j;//ָ��ͼ���i�У���j�����ص�ָ��
			pixel = (BYTE)*lpSrc; //��ȡ�õ�ĻҶ�ֵ
			if( pixel == 255 )//���Ƿ�Ϊ�׵�
			{   
				
				top = i;//��Ϊ�׵㣬�Ѵ˵���Ϊ�ַ����µ���ߵ�
				i = nHeight;
				break;
			}
		}
	}

    //��������ɨ�裬���±߽�
	dwBaseIndex = 0;
	for( i = nHeight - 1; i >= 0; i-- )
	{
		for( j = 0; j < nWidth; j++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * i + 4 * j;
			pixel = (BYTE)*lpSrc;
			if( pixel == 255 )
			{
				bottom = i;//��Ϊ�׵㣬�Ѵ˵���Ϊ�ַ����µ���͵�
				i = -1;
				break;
			}
		}
	}

	bool lab = false;//lab �����Ƿ����һ���ַ��ָ�ı�־
	bool white = false; //����ɨ��һ�����Ƿ��ְ�ɫ��
	int	 white_num = 0;//����ɨ��һ�����ж��ٸ���ɫ��
	CRect rect;//���λ����Ϣ�Ľṹ��
	digicount = 0;//����������
	for( i = 0; i < nWidth; i++ )
	{
		white = false;
		white_num = 0;
		for( j = 0; j < nHeight; j++ )
		{	
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * j + 4 * i;
			pixel = (BYTE)*lpSrc;
			if ( pixel == 255 )
				white_num++;
		}
		//��������ͼ�������Ĺ�ϵ����white_num��ֵ��0~3֮��ʱ�϶�����û�а׵�
		if( white_num > 3 )
		{
			white = true;//������ְ׵㣬���ñ�־λ
			if( lab == false )//�����û�н���һ���ַ��ķָ�
			{
				rect.left = i;//�������߽�
				lab = true;//�ַ��ָʼ
			}
		}
		//����Ѿ�ɨ�������ұ����У�˵������ͼ��ɨ����ϡ��˳�
		if( i == (nWidth - 1) )
			break;
		//�������white��Ϊfalse��˵��ɨ����һ�У���û�з��ְ׵㡣������ǰ�ַ��ָ����
		if( lab == true && ( white_num <= 3 || i == (nWidth - 2) ) )
		{
			//��λ����Ϣ����ṹ����
			rect.right = i;//�����ұ߽�
			rect.top = top;//�����ϱ߽�
			rect.bottom = bottom;//�����±߽�
			int a = rect.top;
			int b = rect.bottom;
			int c = rect.left;
			int d = rect.right;
			rect.InflateRect(1,1);//��������һ�����أ�����ѹ���ַ�
			charRect1.push_back(rect);//������ṹ�������λ����Ϣ������1�ĺ���
			lab = false;//���ñ�־λ����ʼ��һ�ε��ַ��ָ�
			digicount++;//�ַ�����ͳ�Ƽ�������1
		}
	}
	//�ٽ������������ε�top��bottom��ȷ��
	charRect2 = charRect1;//������1��ֵ������2
    charRect2.clear();//������2���������
	CRect rectnew;//����һ���µĴ��λ����Ϣ�Ľṹ��
	while(!charRect1.empty())//��������1��ͷ��β�������ɨ��
	{    
		rect = charRect1.front();//������1ͷ�ϵõ�һ������
		charRect1.pop_front();//������1ͷ����ɾ��һ��
		//������Ӿ�ȷ�ľ�������
		if( rect.left - 1 >= 0 )//��þ�ȷ����߽�
			rectnew.left = rect.left - 1;
		else
			rectnew.left = 0;
		rectnew.right = rect.right + 1;//��þ�ȷ���ұ߽�

		//ͨ����õľ�ȷ���ұ߽�����±߽����½��о�ȷ��λ
		if( rect.top < 0 )
			rect.top = 0;
		if( rect.left < 0 )
			rect.left = 0;
		for( i = rect.top; i < rect.bottom; i++ )//���϶���ɨ������ϱ߽�
		{
			for( j = rect.left; j < rect.right; j++ )
			{
				lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * i + 4 * j;
				pixel = (BYTE)*lpSrc;
				if( *lpSrc == 255 )
				{
					if( i - 2 >= 0 )
						rectnew.top = i - 2;
					else
						rectnew.top = 0;
					i = rect.bottom;
					break;
				}
			}
		}
		for( i = rect.bottom - 1; i >= rect.top; i-- )//���¶���ɨ������±߽�
		{
			for( j = rect.left; j < rect.right; j++ )
			{
				lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * i + 4 * j;
				pixel = (BYTE)*lpSrc;
				if( pixel == 255 )
				{
					rectnew.bottom = i + 2;
					i = -1;
					break;
				}
			}
		}
		charRect2.push_back(rectnew);//���õ����µ�׼ȷ��λ����Ϣ�Ӻ���嵽����2��β��
	}
	charRect1 = charRect2;//������2 ���ݸ�����1
	return charRect1;
}

void CCharPretreat::StdDIBbyRect(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  CRectLink &m_charRect)//�������ָ���ַ����������Ŵ���ʹ���ǵĿ�͸�һֱ���Է�����������ȡ
{
	BYTE* lpSrc;
	int tarWidth = 16;
	int	tarHeight = 32;
	int	i;
	int	j;
	long OneLineBytes = (long)nWidth * 4;
	double wscale,hscale;//��ȡ��߶ȷ����ϵ���������
	//����һ����ʱ������,����ű仯���ͼ����Ϣ
	LPSTR lpNewDIBBits;
	LPSTR lpDst;
	HLOCAL nNewDIBBits = LocalAlloc(LHND, OneLineBytes*nHeight);//�������Ĵ�С��ԭͼ�����������Сһ��
	lpNewDIBBits = (char*)LocalLock(nNewDIBBits);//ָ�򻺴�����ʼλ�õ�ָ��
	lpDst = (char*)lpNewDIBBits;//ָ�򻺴�����Ϣ��ָ��
	memset(lpDst, 0, OneLineBytes*nHeight);//�������������ݸ���ʼֵ
	int i_src,j_src;//����ӳ��������������
	//����ַ�λ����Ϣ�Ľṹ��
	CRect rect;
	CRect rectnew;
	m_charRectCopy.clear();//�����һ���µľ������������Ա�洢��׼����ľ�����������
	while( !m_charRect.empty() )//��ͷ��β���ɨ��������
	{
		rect = m_charRect.front();//�ӱ�ͷ�ϵõ�һ������
		m_charRect.pop_front();//������ͷ����ɾ��һ��
		int a = rect.Width();
		int b = rect.Height();
		if( rect.Width() >= 8 && rect.Height() >= 30 )//ѡ���������ַ�
		{
			wscale = (double)tarWidth / rect.Width();//�����귽�����������
			hscale = (double)tarHeight / rect.Height();//�����귽�����������
			rectnew.top = rect.top ;//�ϱ߽�
			rectnew.bottom = rect.top + tarHeight;//�±߽�
			rectnew.left = rect.left;//��߽�
			rectnew.right = rectnew.left + tarWidth;//�ұ߽�	
			for( i = rectnew.top; i < rectnew.bottom; i++ )//��ԭ���ο��ڵ�����ӳ�䵽�µľ��ο���
			{
				for( j = rectnew.left; j < rectnew.right; j++ )
				{
					//����ӳ������
					i_src = rectnew.top + int((i-rectnew.top) / hscale);
					j_src = rectnew.left + int((j-rectnew.left) / wscale);
					//�����Ӧ�����ص����ӳ�����
					lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i_src + 4 * j_src;
					lpDst = (char*)lpNewDIBBits + OneLineBytes * i + 4 * j;
					*lpDst++ = *lpSrc;
					*lpDst++ = *lpSrc;
					*lpDst++ = *lpSrc;
				}
			}
			m_charRectCopy.push_back(rectnew);//����׼����ľ�����������µ�����
		}
	}
    m_charRect = m_charRectCopy;
	memcpy(lpbyBits32, lpNewDIBBits, OneLineBytes * nHeight);//�������������ݿ�����ͼ�����������
}

BOOL CCharPretreat::AutoAlign(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32,  CRectLink &m_charRect) //�������˱�׼��������ַ����й���������
{  
	BYTE* lpSrc;
	int w = m_charRect.front().Width();//��ȡ��׼���Ŀ��
	int h = m_charRect.front().Height();//��ȡ��׼���ĸ߶�
	BYTE* lpDst = lpbyBitsDst32;
	long OneLineBytes = (long)nWidth * 4;
	long OneLineBytesnew = (long)w * m_charRect.size() * 4;
	memset(lpDst, (BYTE)0, OneLineBytesnew * h);//���µ�ͼ���ʼ��Ϊ��ɫ
	int i_src, j_src;//ӳ��������������
	int i, j;
	int counts = 0;//ͳ���ַ������ı���
	CRect rect,rectnew;//���λ����Ϣ�Ľṹ��
	m_charRectCopy.clear ();//���һ���µ�����������µ��ַ�λ����Ϣ
	while( !m_charRect.empty() )//��ͷ��β���ɨ��ԭ����ĸ������
	{
		rect = m_charRect.front ();//�ӱ�ͷ�ϵõ�һ�����ο�
		m_charRect.pop_front();//��������ο��������ɾ��
		rectnew.left = counts * w;//��߽�
		rectnew.right = (counts+1) * w;//�ұ߽�
		rectnew.top = 0;//�ϱ߽�
		rectnew.bottom = h;//�±߽�
		m_charRectCopy.push_back(rectnew);//����õ��µľ��ο���뵽�µ�������
		for( i = 0; i < h; i++)//��ԭ���ο��ڵ�����ӳ�䵽�µľ��ο���
		{
			for( j = counts*w; j < (counts+1)*w; j++)
			{
	          	i_src = rect.top + i;
				j_src = rect.left + j - counts * w;
				lpSrc = (BYTE*)lpbyBitsSrc32 + OneLineBytes *  i_src + 4 * j_src;
				lpDst = (BYTE*)lpbyBitsDst32 + OneLineBytesnew * i + 4 * j;
				*lpDst++ = *lpSrc;
				*lpDst++ = *lpSrc;
				*lpDst++ = *lpSrc;
			}
		}
		counts++;
	}
	m_charRect = m_charRectCopy;
	return true;
}

BOOL CCharPretreat::ThiningDIB(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32)//��ͼ�����ϸ������
{
	LPSTR lpSrc;
	LPSTR lpDst;// ָ�򻺴�ͼ���ָ��
	long OneLineBytes = (long)nWidth * 4;
	BOOL bModified;
	long i;
	long j;
	int  n;
	int  m;
	BOOL bCondition1;//�ĸ�����
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;
	unsigned char nCount;//������
	unsigned char pixel;//����ֵ
	unsigned char neighbour[5][5];//5��5������������ֵ

	lpDst = (char *)lpbyBitsDst32;
	memset(lpDst, (BYTE)255, OneLineBytes*nHeight);
	bModified=TRUE;

	while(bModified)
	{
		bModified = FALSE;
		lpDst = (char *)lpbyBitsDst32;// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
		memset(lpDst, (BYTE)255, OneLineBytes*nHeight);
		for( j = 2; j < nHeight-2; j++ )
		{
			for( i = 2; i < nWidth-2; i++ )
			{
				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;
				//����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������
				lpSrc = (char *)lpbyBits32 + OneLineBytes * j + 4 * i;// ָ��Դͼ������j�У���i�����ص�ָ��
				lpDst = (char *)lpbyBitsDst32 + OneLineBytes * j + 4 * i;// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
				pixel = (unsigned char)*lpSrc;//ȡ�õ�ǰָ�봦������ֵ
				if(pixel != 255 && *lpSrc != 0)
					continue;
				else if(pixel == 255)
					continue;
				
				for( m = 0; m < 5; m++ )//��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				{
					for( n = 0; n < 5; n++)
						neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*OneLineBytes + 4 * (n - 2) )) / 255;
				}
				//�ж�2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;
				//�ж�Z0(P1)=1
				nCount = 0;
				if(neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if(neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if(neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if(neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if(neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if(neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if(neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if(neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if(nCount == 1)
					bCondition2 = TRUE;
				//�ж�P2*P4*P8=0 or Z0(p2)!=1
				if(neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if(neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if(neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if(neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if(neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if(neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if(neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if(neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if(neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if(nCount != 1)
						bCondition3 = TRUE;
				}
				//�ж�P2*P4*P6=0 or Z0(p4)!=1
				if(neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if(neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if(neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if(neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if(neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if(neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if(neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if(neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if(neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if(nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst++ = (unsigned char)255;
					*lpDst++ = (unsigned char)255;
					*lpDst++ = (unsigned char)255;
					bModified = TRUE;
				}
				else
				{
					*lpDst++ = (unsigned char)0;
					*lpDst++ = (unsigned char)0;
					*lpDst++ = (unsigned char)0;
				}
			}
		}
		memcpy(lpbyBits32, lpbyBitsDst32, OneLineBytes*nHeight);
	}
	memcpy(lpbyBits32, lpbyBitsDst32, OneLineBytes * nHeight);// ���Ƹ�ʴ���ͼ��
	return TRUE;
}

void CCharPretreat::RemoveScatterNoise(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)//ͨ���������㳤�ȵ�ͳ����ȥ����ɢ�ӵ�
{
	
	int length=15;//�����ж������ĳ�����ֵΪ15, ����뿼��������ӵĺڵ����ĿС��15����Ϊ�������������
	m_lianXuShu = 0;
	LONG i;
	LONG j;	
	LONG k;
	long OneLineBytes = (long)nWidth * 4;
	LPSTR lpSrc;
	LPBYTE lplab = new BYTE[nHeight * OneLineBytes];//����һ��������ű�־���ڴ�����
	bool *lpTemp = new bool[nHeight * OneLineBytes];//����һ������������ɢ�ж�������ڴ�����
	for( i = 0; i < nHeight * OneLineBytes; i++ )//��ʼ����־����
		lplab[i] = false;
	CPoint lab[21];//���������ɢ������������
	k = 0;//Ϊѭ����������ʼֵ
	for( i = 0; i < nHeight; i++ )
	{
		for( j = 0; j < nWidth; j++ )
		{
			for( k = 0; k < m_lianXuShu; k++ )
				lplab[lab[k].y * OneLineBytes + lab[k].x * 4] = false;
			m_lianXuShu = 0;//��������0
			lpTemp[i*OneLineBytes+j*4] = DeleteScaterJudge( lpbyBits32, OneLineBytes, lplab, nWidth, nHeight, j, i, lab, length);//������ɢ���ж�
		}
	}
	for( i = 0; i < nHeight; i++ )//ɨ������ͼ�񣬰���ɢ�����ɰ�ɫ
	{
		for( j = 0; j < nWidth; j++ )
		{
			if(lpTemp[i*OneLineBytes+j*4] == false)//�鿴��־λ,���Ϊ���򽫴˵���Ϊ�׵�
			{	
				lpSrc = (char*)lpbyBits32 + OneLineBytes * i + 4 * j;
				//����������Ϊ�׵�
				*lpSrc++ = BYTE(255);
				*lpSrc++ = BYTE(255);
				*lpSrc++ = BYTE(255);
			}
		}
	}
}

bool CCharPretreat::DeleteScaterJudge(LPBYTE lpbyBits32, long OneLineBytes, LPBYTE lplab, int nWidth, int nHeight, int x, int y, CPoint lab[], int lianXuShu)//���õݹ��㷨ͳ��������ĸ�����ͨ����ֵ���ж��Ƿ�Ϊ��ɢ��
{
	if( m_lianXuShu >= lianXuShu )//���������������Ҫ��˵��������ɢ�㣬����
		return TRUE;
	m_lianXuShu++;
	lplab[OneLineBytes * y + 4 * x] = true;//�趨���ʱ�־
	//������ʵ�����
	lab[m_lianXuShu-1].x = x;
	lab[m_lianXuShu-1].y = y;
	int gray;
	LPSTR lpSrc;
	
	if(m_lianXuShu>=lianXuShu)//�����ж�, ���������������Ҫ��˵��������ɢ�㣬����
		return TRUE;
	else//����ݹ�
	{	
		//�������������Լ����ϡ����ϡ����¡����°˸�����, ����Ǻ�ɫ�㣬����ú���������еݹ�
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y-1) + 4 * x;
		gray=*lpSrc;//���ݻҶ�ֵ
		if(y-1 >=0 && gray == 0 && lplab[(y-1)*OneLineBytes+x*4] == false)//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x,y-1,lab,lianXuShu);//���еݹ鴦��
		if(m_lianXuShu>=lianXuShu)//�жϳ���, ���������������Ҫ��˵��������ɢ�㣬����
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y-1) + 4 * (x-1);//���µ�
		gray = *lpSrc;//���ݻҶ�ֵ
		if(y-1 >=0 &&  x-1 >=0 && gray== 0 && lplab[(y-1)*OneLineBytes+(x-1)*4] == false)//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�	
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x-1,y-1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)//�жϳ���, ���������������Ҫ��˵��������ɢ�㣬����
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * y + 4 * (x-1);//���
		gray = *lpSrc;
		if(x-1 >=0 &&  gray== 0 && lplab[y*OneLineBytes+(x-1)*4] == false)//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x-1,y,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUEl
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y+1) + 4 * (x-1);//����
		gray = *lpSrc;
		if(y+1 <nHeight && x-1 >= 0 && gray == 0 && lplab[(y+1)*OneLineBytes+(x-1)*4] == false)
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x-1,y+1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y+1) + 4 * x;//����
		gray = *lpSrc;
		if(y+1 < nHeight && gray == 0 && lplab[(y+1)*OneLineBytes+x*4] == false)//�������ͼ���ڡ���ɫΪ��ɫ����û�б����ʹ�
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x,y+1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y+1) + 4 * (x+1);//����
		gray=*lpSrc;
		if(y+1 <nHeight && x+1 <nWidth &&  gray == 0 && lplab[(y+1)*OneLineBytes+(x+1)*4] == false)
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x+1,y+1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * y + 4 * (x+1);//�ұ�
		gray=*lpSrc;
		if(x+1 <nWidth && gray==0 && lplab[y*OneLineBytes+(x+1)*4] == false)	
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x+1,y,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y-1) + 4 * (x+1);//����
		gray = *lpSrc;
		if(y-1 >=0 && x+1 <nWidth && gray == 0 && lplab[(y-1)*OneLineBytes+(x+1)*4] == false)	
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x+1,y-1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
	}
	return FALSE;
}