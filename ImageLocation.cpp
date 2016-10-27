/////////////////////////////////////////////////////////////////////////////////
//
// ImageLocation.cpp: implementation of the CImageLocation class.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageLocation.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CImageLocation, CImageProcess)
CImageLocation::CImageLocation()
{
}

CImageLocation::~CImageLocation()
{
}

#ifdef _DEBUG
void CImageLocation::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CImageLocation::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif

bool CImageLocation::LocateMark(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  LPRECT  p_markRect)
{// �ú����������32λͼ��lpbyBits32��ָ��32λͼͼ�����ݵ�ָ��
	//��һ��, ���в����Ϸ��Լ��
	ASSERT(lpbyBits32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	
	//ѭ������
	long	i, j;

	//����ֵ
	BYTE	pixel;

	//ͼ����������
	BYTE*	lpSrc;

	//ͼ��ÿ�е��ֽ���
	long	OneLineBytes = (long)nWidth * 4;
	
	//��ʼ���ݻ�����
	DWORD dwBaseIndex = y * OneLineBytes + 4 * x;

	//��Ч����Ŀ�Ⱥ͸߶�
	int	w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)	return FALSE;

	//ÿ�����������������(�Գ��ƿ��Ϊ�ο����Դ�)
	const int lmax = 250;

	//ÿ���������С�������(�Գ��ƿ��Ϊ�ο����Դ�)
	const int lmin = 120;

	//ÿ����ʼ�����ֹ��֮���������С�������
	//(Ӧ��Ϊ��������ÿ����˵��Ҷ˵����������ƽ�������д�̽��)
	const int m = 13;

	//ÿ�п��ܵ���ʼ�����ֹ��(����ͼ��߶Ȳ�����1000��, ÿ�ж���)
	int	col_s[1000], col_e[1000];

	//ÿ�е���ʼ�����ֹ��֮����������
	int	r[1000];

	//�ҵ�ÿ����ʼ��ı�־��
	bool FirstColFlag;
	
	//��ǰ�㵽��ʼ��ľ���
	int	l;
	
	//��ǰ�㵽��ʼ��ľ��볬������ʱ��ʼ�Ƚϱ�ֵ
	const int l_max = 100;

	//��ǰ�㵽��ʼ��ľ���������������ı�ֵ
	float lr_proportion;

	//��ǰ�㵽��ʼ��ľ���������������ı�ֵ�ķ�Χ
	const float	lr_p_min = (float)3.9;
	const float	lr_p_max = (float)13.0;

	//�����ڵ����Ŀ, ʵ�������������������ľ���
	int	black = 0;

	//�����ڵ�������Ŀ, ���������������֮���������
	const int black_max = 50;

	dwBaseIndex = 0;

	//��һ��ȫͼɨ��������ÿ�п��ܵ���ʼ�����ֹ��, �Լ���ʼ�����ֹ��֮������������
	//�Ǵ�������ȷ������������ܴ��ڵ��з�Χ, ��Ϊ��һ�����������з�Χ��ȷ���ṩ����

	for( i = 0; i < nHeight; i++ )//(�߼��ϵ�)
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		FirstColFlag = false;
		r[i] = 0;
		col_s[i] = 0;
		for( j = 0; j < nWidth; j++ )//(�߼��ϵ�)
		{
			//����ֵ
			pixel = (BYTE)*lpSrc;
			l = j - col_s[i];
			if( r[i] != 0 )
				lr_proportion = (float)l/(float)r[i];
			else
				lr_proportion = 65535;
			//����õ��������
			if( pixel == 255 )
			{
				//������ҵ����е���ʼ��
				if( FirstColFlag == true )
				{
					//�����ǰ�㵽��ʼ��ľ���С��lmax
					if( l <= lmax )
					{
						if( black > black_max )
							if( col_e[i] - col_s[i] > lmin )
							{
								break;	//����
							}
							else
							{
								FirstColFlag = false;
								black = 0;
								continue;//����
							}
							//�����ǰ�㵽��ʼ��ľ��볬��l_max, ���ҿ�Ⱥ���������ı���������Ҫ��, �����¿�ʼ
							if( l > l_max && (r[i] == 0 || lr_proportion < lr_p_min || lr_proportion > lr_p_max ))
							{
								col_s[i] = j;
								col_e[i] = j;
								r[i] = 0;
							}
							//����ͽ���ǰ����Ϊ��ֹ��
							else
							{
								col_e[i] = j;
								r[i]++;
							}
					}
					//�����ǰ�㵽��ʼ��ľ������lmax, ������������������û����
					//ÿ����ʼ�����ֹ��֮���������С�������, �п��ܻ�û���ҵ���������
					else if( l > lmax && r[i] <= m )
					{
						col_s[i] = j;
						col_e[i] = j;
						r[i] = 0;
					}
					//�����ǰ�㵽��ʼ��ľ������lmax, ���Ҹ��������������Ѿ�����
					//ÿ����ʼ�����ֹ��֮���������С�������, �����Ѿ��ҵ���������
					else if( l > lmax && r[i] > m )
						break;	//����
				}
				//����õ��������,���һ�û���ҵ����е���ʼ��
				else
				{
					col_s[i] = j;
					col_e[i] = j;
					FirstColFlag = true;
				}
				black = 0;
			}
			//����õ㲻�������
			else
				black++;
			//��ʵ��ָ��ĺ����ƶ�
			*lpSrc++;
			*lpSrc++;
			*lpSrc++;
			*lpSrc++;
		}
		dwBaseIndex += OneLineBytes;
	}

	//�ڶ���ѭ���ҳ�����������з�Χ

	//������������Ĳ�����Ҫ����������
	const int B = 5;
	
	//����Ҫ��������е���С����
	const int C = 20;

	//ÿ��������Ҫ���������С�������
	const	int	jmax = 40;
	const	int	jmin = 13;

	//����������ܵ���ʼ�к���ֹ��, ÿ�����ܵ�������, �������������򲻳���10��
	int	row_s[10], row_e[10];
	int k = 0;

	//�ҵ�����������ʼ�еı�־��
	bool FirstRowFlag = false;

	//������������Ĳ�����Ҫ��������е�����
	int b = 0;
	//����Ҫ��������е�����
	int c = 0;

	for( i = 0; i < nHeight; i++ )//(�߼��ϵ�)
	{
		if( FirstRowFlag == true )
		{
			if( r[i] >= jmin && r[i] <= jmax //������е��������������Ҫ��
				&& col_e[i] - col_s[i] >= lmin && col_e[i] - col_s[i] <= lmax ) //������е���ʼ�����ֹ��֮��ľ�������Ҫ��
			{
				c++;
				b = 0;
			}
			else
			{
				b++;
				if( b >= B && c < C )
				{
					FirstRowFlag = false;
					b = 0;
					c = 0;
				}
				//��ǰ�е���ʼ�е����������������ǳ�������, ���µ�ǰ��Ϊ��ֹ��
				else if( c >= C && b >= B )
				{
					row_e[k] = i;
					k++;
					FirstRowFlag = false;
					b = 0;
					c = 0;
 				}
			}
		}
		else 
		//�����û�ҵ�����������ܵ���ʼ��, ͬʱ��ǰ������Ҫ��, ���赱ǰ��Ϊ��ʼ��
			if( r[i] >= jmin && r[i] <= jmax && col_e[i] - col_s[i] >= lmin && col_e[i] - col_s[i] <= lmax )
			{
				row_s[k] = i;
				FirstRowFlag = true;
			}
	}


	//���ݳ�������ĳ�����Ϣ, ɸѡ���ܵĳ�������, �ҳ������ĳ�������

	//��������Ŀ�Ⱥ͸߶�
	int	width, height;
	//��������Ŀ�Ⱥ͸߶ȷ�Χ
	const int width_min = 130;
	const int width_max = 235;
	const int height_min = 28;
	const int height_max = 60;

	//��������ĳ����
	float wh_proportion;
	//��������ĳ���ȵķ�Χ
	const float	wh_p_min = 3.5;
	const float	wh_p_max = 10.0;

	//����������ʼ�к���ֹ��
	int	start_row;
	int	end_row;

	//�Ƿ��ҵ���������
	bool FindFlag = false;

	for( i = 0; i < k; i++ )
	{
		width = min( col_e[row_s[i]], col_e[row_e[i]] ) - min( col_s[row_s[i]], col_s[row_e[i]] );
		height = row_e[i] - row_s[i];
		wh_proportion = (float)width / (float)height;
		//�жϳ�������ĳ���Χ�ͱ�ֵ
		if( width > width_min && width < width_max && height > height_min && height < height_max && wh_proportion > wh_p_min && wh_proportion < wh_p_max )
		{
			start_row = row_s[i];
			end_row = row_e[i];
			FindFlag = true;
		}
	}

	//������㳵����������ұ߽�

	//ͳ�����г��ֵ���ʼ�����ֹ���λ��, �������СΪ������������ȼ�1
	int	start_pos[width_max+1], end_pos[width_max+1];
	//ͳ�Ƽ�¼��ʼ��ĳ���Ƶ��
	int	start_count[width_max+1], end_count[width_max+1];
	//�����α�
	int	s = 0;
	int e = 0;
	//�Ƿ�������ʼ�����ֹ��ı�־
	bool new_start;
	bool new_end;
	//��ʼ�����ֹ����α�
	int	s_l = 0;
	int	e_r = 0;

	if( FindFlag == true )
	{
		//����ѭ����ɶ���ʼ�����ֹ��λ�õ�ͳ��
		for( i = start_row; i <= end_row; i++ )
		{
			new_start = true;
			new_end = true;
			for( j = 0; j < s; j++ )
				if( col_s[i] == start_pos[j] )
				{
					start_count[j]++;
					new_start = false;
				}
				for( j = 0; j < e; j++ )
					if( col_e[i] == end_pos[j] )
					{
						end_count[j]++;
						new_end = false;
					}
					if( new_start == true )
					{
						start_pos[s] = col_s[i];
						start_count[s] = 1;
						s++;
					}
					if( new_end == true )
					{
						end_pos[e] = col_e[i];
						end_count[e] = 1;
						e++;
					}
		}
			
		//����������һ����ԪֵΪ����������Ԫ��ֵ
		start_count[s] = start_count[0];
		for( i = 0; i < s; i++ )
		{
			if( start_count[i] >= start_count[s] )
			{
				start_count[s] = start_count[i];
				s_l = i;
			}
		}
		//����������һ����ԪֵΪ����������Ԫ��ֵ
		end_count[e] = end_count[j];
		for( j = 0; j < e; j++ )
		{
			if( end_count[j] >= end_count[e] )
			{
				end_count[e] = end_count[j];
				e_r = j;
			}
		}

		//�Գ����������΢��
		p_markRect->left = start_pos[s_l] + 2;
		p_markRect->right = end_pos[e_r];
		p_markRect->top = end_row - 2;			//���Ƶ��±�Ե
		p_markRect->bottom = start_row - 3;	    //���Ƶ��ϱ�Ե
	}
	else
	{
		p_markRect->left = 0;
		p_markRect->right = nWidth;
		p_markRect->top = nHeight;
		p_markRect->bottom = 0;
	}

	return true;
}