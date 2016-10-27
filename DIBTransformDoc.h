// DIBTransformDoc.h : interface of the CDIBTransformDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBTRANSFORMDOC_H__C2876D67_4489_4260_BF5B_E01904504F2A__INCLUDED_)
#define AFX_DIBTRANSFORMDOC_H__C2876D67_4489_4260_BF5B_E01904504F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dib.h"

class CDIBTransformDoc : public CDocument
{
protected: // create from serialization only
	CDIBTransformDoc();
	DECLARE_DYNCREATE(CDIBTransformDoc)

// Attributes
public:
	CDib* m_pDib;
	CDib* m_pOldDib;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIBTransformDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDIBTransformDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDIBTransformDoc)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBTRANSFORMDOC_H__C2876D67_4489_4260_BF5B_E01904504F2A__INCLUDED_)
