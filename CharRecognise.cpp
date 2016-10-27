#include "stdafx.h"
#include "CharRecognise.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int template_num = 26;	//模板库中有多少模板
const float	template_code [template_num][13] = {
												{8, 8, 8, 8, 8, 8, 8, 8, 64, 2, 2, 3, 2},	 //0
												{0, 3, 0, 8, 0, 8, 0, 3, 22, 1, 1, 0, 1},	 //1
												{6, 9, 0, 11, 6, 6, 9, 6, 53, 1, 1, 5, 4},	 //2
												{4, 7, 0, 11, 0, 9, 7, 9, 47, 1, 1, 2, 6},	 //3
												{0, 5, 4, 13, 11, 8, 4, 9, 54, 2, 2, 4, 2},	 //4
												{6, 4, 10, 5, 0, 9, 5, 8, 47, 1, 1, 4, 4},	 //5
												{4, 2, 10, 4, 8, 8, 7, 9, 52, 1, 2, 8, 2},	 //6
												{5, 9, 0, 10, 0, 8, 0, 6, 38, 1, 1, 1, 5},	 //7
												{7, 7, 10, 10, 10, 10, 8, 8, 71, 2, 2, 5, 5},//8
												{8, 8, 10, 9, 3, 13, 0, 5, 56, 2, 1, 3, 4},	 //9
												{0, 4, 9, 9, 9, 9, 9, 9, 54, 1, 2, 6, 4},	   //A(10)
												{8, 8, 11, 12, 10, 10, 8, 9, 71, 2, 2, 3, 3},  //B(11)
												{7, 8, 8, 0, 8, 2, 6, 10, 48, 1, 1, 2, 2},	   //C(12)
												{8, 6, 11, 5, 11, 0, 9, 6, 53, 1, 1, 3, 3},	   //E(13)
												{7, 6, 11, 4, 11, 0, 4, 0, 40, 1, 1, 2, 2},	   //F(14)
												{6, 8, 8, 5, 8, 8, 7, 9, 63, 1, 2, 3, 3},	   //G(15)
												{4, 5, 11, 10, 9, 9, 6, 5, 60, 2, 2, 3, 5},	   //K(16)
												{5, 5, 6, 6, 6, 6, 5, 5, 48, 2, 2, 2, 2},	   //X(17)
												{9, 9, 9, 2, 13, 11, 8, 7, 66, 0, 3, 8, 4},		 //苏(18)
												{10, 12, 9, 18, 10, 15, 9, 8, 98, 3, 1, 6, 6},	 //黑(19)
												{6, 6, 9, 9, 8, 14, 7, 10, 69, 5, 2, 3, 3},		 //京(20)
												{8, 9, 10, 24, 14, 17, 4, 9, 94, 3, 0, 4, 7},	 //粤(21)
												{10, 7, 17, 16, 14, 20, 5, 6, 100, 3, 2, 13, 7}, //鲁(22)
												{9, 9, 14, 11, 11, 14, 8, 8, 84, 3, 2, 14, 13},	 //晋(23)
												{5, 10, 20, 25, 16, 16, 7, 9, 97, 7, 3, 3, 9},	 //闽(24)
												{0, 10, 6, 8, 8, 8, 10, 5, 57, 1, 2, 16, 18},	 //辽(25)
												};
const CString code[template_num] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "E", "F", "G","K", "X", "苏", "黑", "京", "粤", "鲁", "晋", "闽", "辽"};

IMPLEMENT_DYNAMIC(CCharRecognise, CImageProcess)

CCharRecognise::CCharRecognise( )
{
}

CCharRecognise::~CCharRecognise()
{
}

#ifdef _DEBUG
void CCharRecognise::Dump(CDumpContext& dc) const
{
	CImageProcess::Dump(dc);
}

void CCharRecognise::AssertValid() const
{
	CImageProcess::AssertValid();
}
#endif

float * * CCharRecognise::TZTQ_13(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  int num,  int dim) //图像分为8块，作为8个特征；象素总数作为一个特征；水平切割过去两条线，得*到两个特征；垂直的两个，总共得到13个特征
{	
	int i,j,k,m;
	float * * feature = alloc_2d_dbl(num,dim);//分配一个内存空间并得到二维指针
	BYTE* lpSrc;
	long width = nWidth/num;
	long	OneLineBytes = (long)nWidth * 4;
	int b;
	float * tz = new float[dim];//存储临时的特征
	for( k = 0; k < num; k++ )
	{
		for( i = 0; i < dim; i++ )
			tz[i]=0;
		for( m = 0; m < 8; m++ )//提取前8个特征
		{	
			for( i = int(m/2)*8; i < (int(m/2)+1)*8; i++ )
				for( j = m%2*8+k*width; j < (m%2+1)*8+k*width; j++ )
				{	
					lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
					b = (*lpSrc==255)?0:1;
					tz[m] += b;
				}
		}
		//提取第9个特征-总象素值
		for( i = 0; i < nHeight; i++ )
			for( j = k*width; j < (k+1)*width; j++ )
			{
				lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
				b = (*lpSrc==255)?0:1;
				tz[8] += b;
			}
		//提取第10、11个特征-水平扫描切割
		i = int(nHeight*1/3);
		for( j = k*width; j < (k+1)*width; j++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
			b = (*lpSrc==255)?0:1;
			tz[9] += b;
		}
		i = int(nHeight*2/3);
		for( j = k*width; j < (k+1)*width; j++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
			b = (*lpSrc==255)?0:1;
			tz[10] += b;
		}
		//提取第12、13个特征-垂直扫描切割
		j = int(k*width+width*1/3);
		for( i = 0; i < nHeight; i++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
			b = (*lpSrc==255)?0:1;
			tz[11] += b;
		}
		j = int(k*width+width*2/3);
		for( i = 0; i < nHeight; i++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j;
			b = (*lpSrc==255)?0:1;
			tz[12] += b;
		}
		//存储特征
		for( i = 0; i < dim; i++ )
			feature[k][i] = tz[i];
	}
	return feature;
}

void CCharRecognise::CodeRecognize( float * * feature, int num )//读入输入样本的特征相量并根据训练所得的权值进行识别，将识别的结果写入result.txt 
{
	int i, j, k;
	CString recognize[7];
	float match_value[template_num];//匹配值数组的大小为模板的多少
	float min_match_value;//匹配的最小值，最小即最吻合
	int	match_template;//匹配的模板序号
	int	start_tnum, end_tnum;//与模板库比较的起始序号和终结序号
	float a = 0;
	for( i = 0; i < num; i++ )//牌照上第i个字符
	{
		min_match_value = 65535;
		match_template = 0;
		//如果当前字符不是牌照上的第一个也不是最后一个字符，就一定不是汉字, 如果当前字符是第一个字符,只匹配汉字
		if( i == 0 )
		{
			start_tnum = 18;
			end_tnum = template_num - 1;
		}
		//如果当前字符是第二个字符,只匹配英文
		else if( i == 1 )
		{
			start_tnum = 10;
			end_tnum = 17;
		}
		//如果是最后三个字符，只匹配数字
		else if( i == num - 3 || i == num - 2 || i == num - 1 )
		{
			start_tnum = 0;
			end_tnum = 9;
		}
		//其他匹配英文和数字
		else
		{
			start_tnum = 0;
			end_tnum = 17;
		}
		for( j = start_tnum; j <= end_tnum; j++ )//与模板库中第j个模板相比较
		{
			match_value[j] = 0;
			for( k = 0; k < 2; k++ )//比较第k个特征
			{
				if( template_code[j][k] == 0 )
					match_value[j] += (float)(fabs( feature[i][k] - template_code[j][k] ));
				else
					match_value[j] += (float)(fabs( feature[i][k] - template_code[j][k] ));
			}
			for( k = 2; k < 6; k++ )
			{
				if( template_code[j][k] == 0 )
					match_value[j] += (float)(2 * fabs( feature[i][k] - template_code[j][k] ));
				else
					match_value[j] += (float)(2 * fabs( feature[i][k] - template_code[j][k] ));
			}
			for( k = 6; k < 8; k++ )
			{
				if( template_code[j][k] == 0 )
					match_value[j] += (float)(0.5 * fabs( feature[i][k] - template_code[j][k] ));
				else
					match_value[j] += (float)(0.5 * fabs( feature[i][k] - template_code[j][k] ));
			}
			for( k = 8; k < 9; k++ )
			{
				if( template_code[j][k] == 0 )
					match_value[j] += (float)(fabs( feature[i][k] - template_code[j][k] ));
				else
					match_value[j] += (float)(fabs( feature[i][k] - template_code[j][k] ));
			}
			for( k = 9; k < 13; k++ )
			{
				if( template_code[j][k] == 0 )
					match_value[j] += (float)(fabs( feature[i][k] - template_code[j][k] ));
				else
					match_value[j] += (float)(fabs( feature[i][k] - template_code[j][k] ));
			}
			for( k = 11; k < 13; k++ )
			{
				if( template_code[j][k] == 0 )
					match_value[j] += (float)(2 * fabs( feature[i][k] - template_code[j][k] ));
				else
					match_value[j] += (float)(2 * fabs( feature[i][k] - template_code[j][k] ));
			}
			a = match_value[j];
		}			
		for( j = start_tnum; j <= end_tnum; j++ )
		{
			if( match_value[j] < min_match_value )
			{
				min_match_value = match_value[j];
				match_template = j;
			}
		}
		if( code[match_template] == "0" )//对相似字符进行再次判断
		{
			if( feature[i][11] >= 6 )
				match_template = 8;
		}
		if( code[match_template] == "K" )
		{
			if( feature[i][2] <= 10 && feature[i][12] < 4 )
				match_template = 0;
		}
		if( code[match_template] == "8" || code[match_template] == "B" )
		{
			if( feature[i][11] >= 6 || i >= num - 3 )
				match_template = 8;
			else
				match_template = 11;
		}	
		if( code[match_template] == "京" )
		{
			if( feature[i][3] <= 6 && feature[i][9] <= 1 )
				match_template = 18;
		}
		recognize[i].Format("%s", code[match_template]);
	}
    CString str,str1;
	for ( i = 0; i < num; i++ )
	{
		str.Format("%s", recognize[i]);
		str1 += str;
	}
	::MessageBox(NULL,str1,"识别结果",NULL);
}