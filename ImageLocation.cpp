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
{// 该函数处理的是32位图，lpbyBits32是指向32位图图像数据的指针
	//第一步, 进行参数合法性检测
	ASSERT(lpbyBits32);

	if((x > (nScanWidth - 1)) || (y > (nScanHeight - 1))) return FALSE;
	
	//循环变量
	long	i, j;

	//像素值
	BYTE	pixel;

	//图像数据索引
	BYTE*	lpSrc;

	//图像每行的字节数
	long	OneLineBytes = (long)nWidth * 4;
	
	//开始数据基索引
	DWORD dwBaseIndex = y * OneLineBytes + 4 * x;

	//有效区域的宽度和高度
	int	w = min(nWidth, nScanWidth - x);
	int h = min(nHeight, nScanHeight - y);
	if(w * h == 0)	return FALSE;

	//每行允许的最大跳变点间距(以车牌宽度为参考，略大)
	const int lmax = 250;

	//每行允许的最小跳变点间距(以车牌宽度为参考，略大)
	const int lmin = 120;

	//每行起始点和终止点之间允许的最小跳变点数
	//(应设为车牌区域每行左端到右端的跳变点数的平均数，有待探索)
	const int m = 13;

	//每行可能的起始点和终止点(假设图像高度不超过1000行, 每行都有)
	int	col_s[1000], col_e[1000];

	//每行的起始点和终止点之间的跳变点数
	int	r[1000];

	//找到每行起始点的标志符
	bool FirstColFlag;
	
	//当前点到起始点的距离
	int	l;
	
	//当前点到起始点的距离超过多少时开始比较比值
	const int l_max = 100;

	//当前点到起始点的距离与跳变点数量的比值
	float lr_proportion;

	//当前点到起始点的距离与跳变点数量的比值的范围
	const float	lr_p_min = (float)3.9;
	const float	lr_p_max = (float)13.0;

	//连续黑点的数目, 实际上是相邻两个跳变点的距离
	int	black = 0;

	//连续黑点的最大数目, 即相邻两个跳变点之间的最大距离
	const int black_max = 50;

	dwBaseIndex = 0;

	//第一次全图扫描的输出是每行可能的起始点和终止点, 以及起始点和终止点之间的跳变点数量
	//是从纵向来确定车牌区域可能处于的列范围, 并为下一步车牌区域行范围的确定提供依据

	for( i = 0; i < nHeight; i++ )//(逻辑上的)
	{
		lpSrc = lpbyBits32 + dwBaseIndex;
		FirstColFlag = false;
		r[i] = 0;
		col_s[i] = 0;
		for( j = 0; j < nWidth; j++ )//(逻辑上的)
		{
			//像素值
			pixel = (BYTE)*lpSrc;
			l = j - col_s[i];
			if( r[i] != 0 )
				lr_proportion = (float)l/(float)r[i];
			else
				lr_proportion = 65535;
			//如果该点是跳变点
			if( pixel == 255 )
			{
				//如果已找到该行的起始点
				if( FirstColFlag == true )
				{
					//如果当前点到起始点的距离小于lmax
					if( l <= lmax )
					{
						if( black > black_max )
							if( col_e[i] - col_s[i] > lmin )
							{
								break;	//换行
							}
							else
							{
								FirstColFlag = false;
								black = 0;
								continue;//换列
							}
							//如果当前点到起始点的距离超过l_max, 并且宽度和跳变点数的比例不满足要求, 就重新开始
							if( l > l_max && (r[i] == 0 || lr_proportion < lr_p_min || lr_proportion > lr_p_max ))
							{
								col_s[i] = j;
								col_e[i] = j;
								r[i] = 0;
							}
							//否则就将当前点设为终止点
							else
							{
								col_e[i] = j;
								r[i]++;
							}
					}
					//如果当前点到起始点的距离大于lmax, 但该行跳变点的数量还没超过
					//每行起始点和终止点之间允许的最小跳变点数, 有可能还没有找到车牌区域
					else if( l > lmax && r[i] <= m )
					{
						col_s[i] = j;
						col_e[i] = j;
						r[i] = 0;
					}
					//如果当前点到起始点的距离大于lmax, 而且该行跳变点的数量已经超过
					//每行起始点和终止点之间允许的最小跳变点数, 可能已经找到车牌区域
					else if( l > lmax && r[i] > m )
						break;	//换行
				}
				//如果该点是跳变点,并且还没有找到该行的起始点
				else
				{
					col_s[i] = j;
					col_e[i] = j;
					FirstColFlag = true;
				}
				black = 0;
			}
			//如果该点不是跳变点
			else
				black++;
			//事实上指针的横向移动
			*lpSrc++;
			*lpSrc++;
			*lpSrc++;
			*lpSrc++;
		}
		dwBaseIndex += OneLineBytes;
	}

	//第二次循环找出车牌区域的行范围

	//连续行中允许的不满足要求的最大行数
	const int B = 5;
	
	//满足要求的连续行的最小行数
	const int C = 20;

	//每行中满足要求的最大和最小跳变点数
	const	int	jmax = 40;
	const	int	jmin = 13;

	//车牌区域可能的起始行和终止行, 每个可能的区域都有, 假设这样的区域不超过10个
	int	row_s[10], row_e[10];
	int k = 0;

	//找到车牌区域起始行的标志符
	bool FirstRowFlag = false;

	//连续行中允许的不满足要求的连续行的行数
	int b = 0;
	//满足要求的连续行的行数
	int c = 0;

	for( i = 0; i < nHeight; i++ )//(逻辑上的)
	{
		if( FirstRowFlag == true )
		{
			if( r[i] >= jmin && r[i] <= jmax //如果该行的跳变点数量满足要求
				&& col_e[i] - col_s[i] >= lmin && col_e[i] - col_s[i] <= lmax ) //如果该行的起始点和终止点之间的距离满足要求
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
				//当前行到起始行的这个连续区域可能是车牌区域, 记下当前行为终止行
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
		//如果还没找到车牌区域可能的起始行, 同时当前行满足要求, 则设当前行为起始行
			if( r[i] >= jmin && r[i] <= jmax && col_e[i] - col_s[i] >= lmin && col_e[i] - col_s[i] <= lmax )
			{
				row_s[k] = i;
				FirstRowFlag = true;
			}
	}


	//根据车牌区域的长宽信息, 筛选可能的车牌区域, 找出真正的车牌区域

	//车牌区域的宽度和高度
	int	width, height;
	//车牌区域的宽度和高度范围
	const int width_min = 130;
	const int width_max = 235;
	const int height_min = 28;
	const int height_max = 60;

	//车牌区域的长宽比
	float wh_proportion;
	//车牌区域的长宽比的范围
	const float	wh_p_min = 3.5;
	const float	wh_p_max = 10.0;

	//车牌区域起始行和终止行
	int	start_row;
	int	end_row;

	//是否找到车牌区域
	bool FindFlag = false;

	for( i = 0; i < k; i++ )
	{
		width = min( col_e[row_s[i]], col_e[row_e[i]] ) - min( col_s[row_s[i]], col_s[row_e[i]] );
		height = row_e[i] - row_s[i];
		wh_proportion = (float)width / (float)height;
		//判断车牌区域的长宽范围和比值
		if( width > width_min && width < width_max && height > height_min && height < height_max && wh_proportion > wh_p_min && wh_proportion < wh_p_max )
		{
			start_row = row_s[i];
			end_row = row_e[i];
			FindFlag = true;
		}
	}

	//下面计算车牌区域的左右边界

	//统计所有出现的起始点和终止点的位置, 该数组大小为车牌区域最大宽度加1
	int	start_pos[width_max+1], end_pos[width_max+1];
	//统计记录起始点的出现频率
	int	start_count[width_max+1], end_count[width_max+1];
	//数组游标
	int	s = 0;
	int e = 0;
	//是否是新起始点和终止点的标志
	bool new_start;
	bool new_end;
	//起始点和终止点的游标
	int	s_l = 0;
	int	e_r = 0;

	if( FindFlag == true )
	{
		//该重循环完成对起始点和终止点位置的统计
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
			
		//设数组的最后一个单元值为数组中最大的元素值
		start_count[s] = start_count[0];
		for( i = 0; i < s; i++ )
		{
			if( start_count[i] >= start_count[s] )
			{
				start_count[s] = start_count[i];
				s_l = i;
			}
		}
		//设数组的最后一个单元值为数组中最大的元素值
		end_count[e] = end_count[j];
		for( j = 0; j < e; j++ )
		{
			if( end_count[j] >= end_count[e] )
			{
				end_count[e] = end_count[j];
				e_r = j;
			}
		}

		//对车牌区域进行微调
		p_markRect->left = start_pos[s_l] + 2;
		p_markRect->right = end_pos[e_r];
		p_markRect->top = end_row - 2;			//车牌的下边缘
		p_markRect->bottom = start_row - 3;	    //车牌的上边缘
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