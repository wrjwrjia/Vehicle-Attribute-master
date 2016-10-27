// DIBTransform.h : main header file for the DIBTRANSFORM application
//

#if !defined(AFX_DIBTRANSFORM_H__31B77DC2_C6C7_4026_9825_0B45BA124D5C__INCLUDED_)
#define AFX_DIBTRANSFORM_H__31B77DC2_C6C7_4026_9825_0B45BA124D5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDIBTransformApp:
// See DIBTransform.cpp for the implementation of this class
//
#define WM_DOREALIZE   (WM_USER + 0)
class CDIBTransformApp : public CWinApp
{
public:
	CDIBTransformApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIBTransformApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDIBTransformApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBTRANSFORM_H__31B77DC2_C6C7_4026_9825_0B45BA124D5C__INCLUDED_)
