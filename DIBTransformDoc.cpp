// DIBTransformDoc.cpp : implementation of the CDIBTransformDoc class
//

#include "stdafx.h"
#include "DIBTransform.h"

#include "DIBTransformDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformDoc

IMPLEMENT_DYNCREATE(CDIBTransformDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIBTransformDoc, CDocument)
	//{{AFX_MSG_MAP(CDIBTransformDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformDoc construction/destruction

CDIBTransformDoc::CDIBTransformDoc()
{
	m_pDib = new CDib;
	m_pOldDib = NULL;
}

CDIBTransformDoc::~CDIBTransformDoc()
{
	if (m_pDib)
		delete m_pDib;
	if (m_pOldDib)
		delete m_pOldDib;
}

BOOL CDIBTransformDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDIBTransformDoc serialization

void CDIBTransformDoc::Serialize(CArchive& ar)
{
	m_pDib->Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformDoc diagnostics

#ifdef _DEBUG
void CDIBTransformDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIBTransformDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformDoc commands

void CDIBTransformDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog fileOpenDlg(TRUE,"bmp","*.bmp",NULL,"Bitmap Files(*.bmp)",NULL);
	int nRespond = fileOpenDlg.DoModal();
	if(nRespond == IDOK)
	{
		CString filePath = fileOpenDlg.GetPathName();
		m_pDib = new CDib(filePath);
		if(m_pDib->IsValid())
			SetTitle(filePath);
		else	
			DeleteContents();
	}
	UpdateAllViews(0);
}

BOOL CDIBTransformDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 失败
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 返回FALSE
		return FALSE;
	}
	
	DeleteContents();
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 尝试调用ReadDIBFile()读取图像
	TRY
	{
		//m_hDIB = ::ReadDIBFile(file);
		m_pDib = new CDib(lpszPathName);
	}
	CATCH (CFileException, eLoad)
	{
		// 读取失败
		file.Abort();
		
		// 恢复光标形状
		EndWaitCursor();
		
		// 报告失败
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 设置DIB为空
		m_pDib = NULL;
		
		// 返回FALSE
		return FALSE;
	}
	END_CATCH
	
	// 初始化DIB
	//InitDIBData();
	
	// 恢复光标形状
	EndWaitCursor();
	
	// 设置文件名称
	SetPathName(lpszPathName);
	
	// 初始化胀标记为FALSE
	SetModifiedFlag(FALSE);
	
	// 返回TRUE
	return TRUE;
}