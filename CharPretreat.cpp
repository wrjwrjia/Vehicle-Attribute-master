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


void CCharPretreat::SlopeAdjust(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight) //通过对图像左右半边平均高度的统计来进行倾斜的调整
{
	unsigned char*	lpSrc; // 指向源图像的指针
	int	i, j;
	long OneLineBytes = (long)nWidth * 4; // 图像每行的字节数
	double leftaver = 0.0; //图像左半边的平均高度
	double rightaver = 0.0; //图像右半边的平均高度
	double slope; //图像的倾斜度
	long counts = 0; //统计循环变量
	//扫描左半边的图像，求白色象素的平均高度
	for( i = 0; i < nHeight; i++ )
	{
		for( j = 0; j < nWidth/2; j++ )
		{
			lpSrc=(unsigned char*)lpbyBits32 + OneLineBytes *  i + 4 * j; //指向第i行第j个象素的指针	
			if(*lpSrc == 255)
			{
				counts = counts + nWidth/2 -j;
				leftaver = leftaver + i*(nWidth/2 -j);
			}
		}
	}
	leftaver = leftaver/counts; //计算平均高度

	counts =0;
	//扫描右半边的图像，求白色象素的平均高度
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
	rightaver = rightaver/counts; //计算右半边的平均高度

	slope = (leftaver - rightaver)/(nWidth/2); //计算斜率

	LPSTR lpNewDIBBits; //指向新的图像象素起始位置的指针
	LPSTR lpDst; //指向新图像的指针
	HLOCAL nNewDIBBits=LocalAlloc(LHND,OneLineBytes*nHeight); //新图像的句柄
	lpNewDIBBits=(char*)LocalLock(nNewDIBBits); //锁定内存
	lpDst=(char*)lpNewDIBBits; //指向新图像象素的指针
	memset(lpDst,(BYTE)0,OneLineBytes*nHeight); //为新图像赋初始值
	int gray; //象素点的灰度值
	int i_src; //位置映射值
	//根据斜率，把当前新图像的点映射到源图像的点
	for( i = 0; i < nHeight; i++ )
	{
   		for( j = 0; j < nWidth; j++ )
		{	
			i_src = int(i - (j-nWidth/2)*slope); //计算映射位置
			if( i_src <0 || i_src >= nHeight ) //如果点在图像外，象素置黑色
				gray = 0;
			else
			{	
				lpSrc=(unsigned char *)lpbyBits32 + OneLineBytes *  i_src + 4 * j;
				gray = *lpSrc;
			}			
			//把新图像的点用得到的象素值填充
			lpDst = (char *)lpNewDIBBits + OneLineBytes * i + 4 * j;
			*lpDst++ = gray;
			*lpDst++ = gray;
			*lpDst++ = gray;
		}
	}
	memcpy(lpbyBits32,lpNewDIBBits,OneLineBytes*nHeight); //将新的图像的内容拷贝到旧的图像中
}

CRectLink CCharPretreat::CharSegment(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight) //将二值化图像中待识别的字符逐个分离出来并返回存放各个字符的位置信息的链表
{
	//清空用来保存每个字符区域的链表
	CRectLink charRect1,charRect2;
	charRect1.clear();
	charRect2.clear();

	BYTE pixel; //像素值
	BYTE* lpSrc;//图像数据索引
	long	OneLineBytes = (long)nWidth * 4;//计算图像每行的字节数
	DWORD dwBaseIndex = y * OneLineBytes + 4 * x;//开始数据基索引
	int top,bottom;//定义上下边界两个变量
	int i, j;
	digicount = 0;//用来统计图像中字符个数的计数器

	//从上往下扫描，找到上边界
	dwBaseIndex = (nHeight - 1) * OneLineBytes;
	for( i = 0; i < nHeight; i++ )
	{
  		for( j = 0; j < nWidth; j++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * i + 4 * j;//指向图像第i行，第j个象素的指针
			pixel = (BYTE)*lpSrc; //获取该点的灰度值
			if( pixel == 255 )//看是否为白点
			{   
				
				top = i;//若为白点，把此点作为字符大致的最高点
				i = nHeight;
				break;
			}
		}
	}

    //从下往上扫描，找下边界
	dwBaseIndex = 0;
	for( i = nHeight - 1; i >= 0; i-- )
	{
		for( j = 0; j < nWidth; j++ )
		{
			lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes * i + 4 * j;
			pixel = (BYTE)*lpSrc;
			if( pixel == 255 )
			{
				bottom = i;//若为白点，把此点作为字符大致的最低点
				i = -1;
				break;
			}
		}
	}

	bool lab = false;//lab 用作是否进入一个字符分割的标志
	bool white = false; //表明扫描一列中是否发现白色点
	int	 white_num = 0;//表明扫描一列中有多少个白色点
	CRect rect;//存放位置信息的结构体
	digicount = 0;//计数器置零
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
		//由于牌照图像质量的关系，当white_num的值在0~3之间时认定该列没有白点
		if( white_num > 3 )
		{
			white = true;//如果发现白点，设置标志位
			if( lab == false )//如果还没有进入一个字符的分割
			{
				rect.left = i;//设置左侧边界
				lab = true;//字符分割开始
			}
		}
		//如果已经扫到了最右边那列，说明整副图像扫描完毕。退出
		if( i == (nWidth - 1) )
			break;
		//如果到此white仍为false，说明扫描了一列，都没有发现白点。表明当前字符分割结束
		if( lab == true && ( white_num <= 3 || i == (nWidth - 2) ) )
		{
			//将位置信息存入结构体中
			rect.right = i;//设置右边界
			rect.top = top;//设置上边界
			rect.bottom = bottom;//设置下边界
			int a = rect.top;
			int b = rect.bottom;
			int c = rect.left;
			int d = rect.right;
			rect.InflateRect(1,1);//将框外括一个象素，以免压到字符
			charRect1.push_back(rect);//将这个结构体插入存放位置信息的链表1的后面
			lab = false;//设置标志位，开始下一次的字符分割
			digicount++;//字符个数统计计数器加1
		}
	}
	//再将矩形轮廓矩形的top和bottom精确化
	charRect2 = charRect1;//将链表1赋值给链表2
    charRect2.clear();//将链表2的内容清空
	CRect rectnew;//建立一个新的存放位置信息的结构体
	while(!charRect1.empty())//对于链表1从头至尾逐个进行扫描
	{    
		rect = charRect1.front();//从链表1头上得到一个矩形
		charRect1.pop_front();//从链表1头上面删掉一个
		//计算更加精确的矩形区域
		if( rect.left - 1 >= 0 )//获得精确的左边界
			rectnew.left = rect.left - 1;
		else
			rectnew.left = 0;
		rectnew.right = rect.right + 1;//获得精确的右边界

		//通过获得的精确左右边界对上下边界重新进行精确定位
		if( rect.top < 0 )
			rect.top = 0;
		if( rect.left < 0 )
			rect.left = 0;
		for( i = rect.top; i < rect.bottom; i++ )//由上而下扫描计算上边界
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
		for( i = rect.bottom - 1; i >= rect.top; i-- )//由下而上扫描计算下边界
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
		charRect2.push_back(rectnew);//将得到的新的准确的位置信息从后面插到链表2的尾上
	}
	charRect1 = charRect2;//将链表2 传递给链表1
	return charRect1;
}

void CCharPretreat::StdDIBbyRect(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight,  CRectLink &m_charRect)//将经过分割的字符，进行缩放处理使他们的宽和高一直，以方便特征的提取
{
	BYTE* lpSrc;
	int tarWidth = 16;
	int	tarHeight = 32;
	int	i;
	int	j;
	long OneLineBytes = (long)nWidth * 4;
	double wscale,hscale;//宽度、高度方向上的缩放因子
	//开辟一块临时缓存区,来存放变化后的图像信息
	LPSTR lpNewDIBBits;
	LPSTR lpDst;
	HLOCAL nNewDIBBits = LocalAlloc(LHND, OneLineBytes*nHeight);//缓存区的大小和原图像的数据区大小一样
	lpNewDIBBits = (char*)LocalLock(nNewDIBBits);//指向缓存区开始位置的指针
	lpDst = (char*)lpNewDIBBits;//指向缓存内信息的指针
	memset(lpDst, 0, OneLineBytes*nHeight);//将缓存区的内容赋初始值
	int i_src,j_src;//进行映射操作的坐标变量
	//存放字符位置信息的结构体
	CRect rect;
	CRect rectnew;
	m_charRectCopy.clear();//先清空一个新的矩形区域链表以便存储标准化后的矩形区域链表
	while( !m_charRect.empty() )//从头到尾逐个扫描各个结点
	{
		rect = m_charRect.front();//从表头上得到一个矩形
		m_charRect.pop_front();//从链表头上面删掉一个
		int a = rect.Width();
		int b = rect.Height();
		if( rect.Width() >= 8 && rect.Height() >= 30 )//选择真正的字符
		{
			wscale = (double)tarWidth / rect.Width();//横坐标方向的缩放因子
			hscale = (double)tarHeight / rect.Height();//纵坐标方向的缩放因子
			rectnew.top = rect.top ;//上边界
			rectnew.bottom = rect.top + tarHeight;//下边界
			rectnew.left = rect.left;//左边界
			rectnew.right = rectnew.left + tarWidth;//右边界	
			for( i = rectnew.top; i < rectnew.bottom; i++ )//将原矩形框内的象素映射到新的矩形框内
			{
				for( j = rectnew.left; j < rectnew.right; j++ )
				{
					//计算映射坐标
					i_src = rectnew.top + int((i-rectnew.top) / hscale);
					j_src = rectnew.left + int((j-rectnew.left) / wscale);
					//将相对应的象素点进行映射操作
					lpSrc = (unsigned char*)lpbyBits32 + OneLineBytes *  i_src + 4 * j_src;
					lpDst = (char*)lpNewDIBBits + OneLineBytes * i + 4 * j;
					*lpDst++ = *lpSrc;
					*lpDst++ = *lpSrc;
					*lpDst++ = *lpSrc;
				}
			}
			m_charRectCopy.push_back(rectnew);//将标准化后的矩形区域插入新的链表
		}
	}
    m_charRect = m_charRectCopy;
	memcpy(lpbyBits32, lpNewDIBBits, OneLineBytes * nHeight);//将缓存区的内容拷贝到图像的数据区内
}

BOOL CCharPretreat::AutoAlign(LPBYTE lpbyBitsSrc32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32,  CRectLink &m_charRect) //将经过了标准化处理的字符进行规整的排列
{  
	BYTE* lpSrc;
	int w = m_charRect.front().Width();//获取标准化的宽度
	int h = m_charRect.front().Height();//获取标准化的高度
	BYTE* lpDst = lpbyBitsDst32;
	long OneLineBytes = (long)nWidth * 4;
	long OneLineBytesnew = (long)w * m_charRect.size() * 4;
	memset(lpDst, (BYTE)0, OneLineBytesnew * h);//将新的图像初始化为白色
	int i_src, j_src;//映射操作的坐标变量
	int i, j;
	int counts = 0;//统计字符个数的变量
	CRect rect,rectnew;//存放位置信息的结构体
	m_charRectCopy.clear ();//清空一个新的链表来存放新的字符位置信息
	while( !m_charRect.empty() )//从头至尾逐个扫描原链表的各个结点
	{
		rect = m_charRect.front ();//从表头上得到一个矩形框
		m_charRect.pop_front();//将这个矩形框从链表上删除
		rectnew.left = counts * w;//左边界
		rectnew.right = (counts+1) * w;//右边界
		rectnew.top = 0;//上边界
		rectnew.bottom = h;//下边界
		m_charRectCopy.push_back(rectnew);//将获得的新的矩形框插入到新的链表中
		for( i = 0; i < h; i++)//将原矩形框内的象素映射到新的矩形框中
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

BOOL CCharPretreat::ThiningDIB(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight, LPBYTE lpbyBitsDst32)//对图像进行细化运算
{
	LPSTR lpSrc;
	LPSTR lpDst;// 指向缓存图像的指针
	long OneLineBytes = (long)nWidth * 4;
	BOOL bModified;
	long i;
	long j;
	int  n;
	int  m;
	BOOL bCondition1;//四个条件
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;
	unsigned char nCount;//计数器
	unsigned char pixel;//像素值
	unsigned char neighbour[5][5];//5×5相邻区域像素值

	lpDst = (char *)lpbyBitsDst32;
	memset(lpDst, (BYTE)255, OneLineBytes*nHeight);
	bModified=TRUE;

	while(bModified)
	{
		bModified = FALSE;
		lpDst = (char *)lpbyBitsDst32;// 初始化新分配的内存，设定初始值为255
		memset(lpDst, (BYTE)255, OneLineBytes*nHeight);
		for( j = 2; j < nHeight-2; j++ )
		{
			for( i = 2; i < nWidth-2; i++ )
			{
				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;
				//由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素
				lpSrc = (char *)lpbyBits32 + OneLineBytes * j + 4 * i;// 指向源图像倒数第j行，第i个象素的指针
				lpDst = (char *)lpbyBitsDst32 + OneLineBytes * j + 4 * i;// 指向目标图像倒数第j行，第i个象素的指针
				pixel = (unsigned char)*lpSrc;//取得当前指针处的像素值
				if(pixel != 255 && *lpSrc != 0)
					continue;
				else if(pixel == 255)
					continue;
				
				for( m = 0; m < 5; m++ )//获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
				{
					for( n = 0; n < 5; n++)
						neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*OneLineBytes + 4 * (n - 2) )) / 255;
				}
				//判断2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;
				//判断Z0(P1)=1
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
				//判断P2*P4*P8=0 or Z0(p2)!=1
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
				//判断P2*P4*P6=0 or Z0(p4)!=1
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
	memcpy(lpbyBits32, lpbyBitsDst32, OneLineBytes * nHeight);// 复制腐蚀后的图像
	return TRUE;
}

void CCharPretreat::RemoveScatterNoise(LPBYTE lpbyBits32,  int x,  int y,  int nWidth,  int nHeight,  int nScanWidth,  int nScanHeight)//通过对连续点长度的统计来去除离散杂点
{
	
	int length=15;//设置判定噪声的长度阈值为15, 如果与考察点相连接的黑点的数目小于15则认为考察点是噪声点
	m_lianXuShu = 0;
	LONG i;
	LONG j;	
	LONG k;
	long OneLineBytes = (long)nWidth * 4;
	LPSTR lpSrc;
	LPBYTE lplab = new BYTE[nHeight * OneLineBytes];//开辟一块用来存放标志的内存数组
	bool *lpTemp = new bool[nHeight * OneLineBytes];//开辟一块用来保存离散判定结果的内存数组
	for( i = 0; i < nHeight * OneLineBytes; i++ )//初始化标志数组
		lplab[i] = false;
	CPoint lab[21];//用来存放离散点的坐标的数组
	k = 0;//为循环变量赋初始值
	for( i = 0; i < nHeight; i++ )
	{
		for( j = 0; j < nWidth; j++ )
		{
			for( k = 0; k < m_lianXuShu; k++ )
				lplab[lab[k].y * OneLineBytes + lab[k].x * 4] = false;
			m_lianXuShu = 0;//连续数置0
			lpTemp[i*OneLineBytes+j*4] = DeleteScaterJudge( lpbyBits32, OneLineBytes, lplab, nWidth, nHeight, j, i, lab, length);//进行离散性判断
		}
	}
	for( i = 0; i < nHeight; i++ )//扫描整个图像，把离散点填充成白色
	{
		for( j = 0; j < nWidth; j++ )
		{
			if(lpTemp[i*OneLineBytes+j*4] == false)//查看标志位,如果为非则将此点设为白点
			{	
				lpSrc = (char*)lpbyBits32 + OneLineBytes * i + 4 * j;
				//将此象素设为白点
				*lpSrc++ = BYTE(255);
				*lpSrc++ = BYTE(255);
				*lpSrc++ = BYTE(255);
			}
		}
	}
}

bool CCharPretreat::DeleteScaterJudge(LPBYTE lpbyBits32, long OneLineBytes, LPBYTE lplab, int nWidth, int nHeight, int x, int y, CPoint lab[], int lianXuShu)//利用递归算法统计连续点的个数，通过阈值来判定是否为离散点
{
	if( m_lianXuShu >= lianXuShu )//如果连续长度满足要求，说明不是离散点，返回
		return TRUE;
	m_lianXuShu++;
	lplab[OneLineBytes * y + 4 * x] = true;//设定访问标志
	//保存访问点坐标
	lab[m_lianXuShu-1].x = x;
	lab[m_lianXuShu-1].y = y;
	int gray;
	LPSTR lpSrc;
	
	if(m_lianXuShu>=lianXuShu)//长度判定, 如果连续长度满足要求，说明不是离散点，返回
		return TRUE;
	else//进入递归
	{	
		//考察上下左右以及左上、右上、左下、右下八个方向, 如果是黑色点，则调用函数自身进行递归
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y-1) + 4 * x;
		gray=*lpSrc;//传递灰度值
		if(y-1 >=0 && gray == 0 && lplab[(y-1)*OneLineBytes+x*4] == false)//如果点在图像内、颜色为黑色并且没有被访问过
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x,y-1,lab,lianXuShu);//进行递归处理
		if(m_lianXuShu>=lianXuShu)//判断长度, 如果连续长度满足要求，说明不是离散点，返回
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y-1) + 4 * (x-1);//左下点
		gray = *lpSrc;//传递灰度值
		if(y-1 >=0 &&  x-1 >=0 && gray== 0 && lplab[(y-1)*OneLineBytes+(x-1)*4] == false)//如果点在图像内、颜色为黑色并且没有被访问过	
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x-1,y-1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)//判断长度, 如果连续长度满足要求，说明不是离散点，返回
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * y + 4 * (x-1);//左边
		gray = *lpSrc;
		if(x-1 >=0 &&  gray== 0 && lplab[y*OneLineBytes+(x-1)*4] == false)//如果点在图像内、颜色为黑色并且没有被访问过
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x-1,y,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUEl
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y+1) + 4 * (x-1);//左上
		gray = *lpSrc;
		if(y+1 <nHeight && x-1 >= 0 && gray == 0 && lplab[(y+1)*OneLineBytes+(x-1)*4] == false)
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x-1,y+1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y+1) + 4 * x;//上面
		gray = *lpSrc;
		if(y+1 < nHeight && gray == 0 && lplab[(y+1)*OneLineBytes+x*4] == false)//如果点在图像内、颜色为黑色并且没有被访问过
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x,y+1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y+1) + 4 * (x+1);//右上
		gray=*lpSrc;
		if(y+1 <nHeight && x+1 <nWidth &&  gray == 0 && lplab[(y+1)*OneLineBytes+(x+1)*4] == false)
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x+1,y+1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * y + 4 * (x+1);//右边
		gray=*lpSrc;
		if(x+1 <nWidth && gray==0 && lplab[y*OneLineBytes+(x+1)*4] == false)	
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x+1,y,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		lpSrc=(char*)lpbyBits32 + OneLineBytes * (y-1) + 4 * (x+1);//右下
		gray = *lpSrc;
		if(y-1 >=0 && x+1 <nWidth && gray == 0 && lplab[(y-1)*OneLineBytes+(x+1)*4] == false)	
			DeleteScaterJudge(lpbyBits32,OneLineBytes,lplab,nWidth,nHeight,x+1,y-1,lab,lianXuShu);
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
	}
	return FALSE;
}