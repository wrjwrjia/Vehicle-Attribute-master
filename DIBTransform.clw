; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDIBTransformView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DIBTransform.h"
LastPage=0

ClassCount=9
Class1=CDIBTransformApp
Class2=CDIBTransformDoc
Class3=CDIBTransformView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_CANVASSIZE
Resource2=IDR_MAINFRAME
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_DIBTRATYPE
Class7=CRotateDlg
Resource4=IDD_ABOUTBOX
Class8=CZoomDlg
Resource5=IDD_ROTATE
Class9=CanvasSizeDlg
Resource6=IDD_ZOOM

[CLS:CDIBTransformApp]
Type=0
HeaderFile=DIBTransform.h
ImplementationFile=DIBTransform.cpp
Filter=N
LastObject=CDIBTransformApp

[CLS:CDIBTransformDoc]
Type=0
HeaderFile=DIBTransformDoc.h
ImplementationFile=DIBTransformDoc.cpp
Filter=N
LastObject=CDIBTransformDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CDIBTransformView]
Type=0
HeaderFile=DIBTransformView.h
ImplementationFile=DIBTransformView.cpp
Filter=C
LastObject=IDM_COLOR_TO_GRAY
BaseClass=CScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=ID_CANVASSIZE


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=DIBTransform.cpp
ImplementationFile=DIBTransform.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_SELECT
Command8=ID_CROP
Command9=ID_ZOOM_OUT
Command10=ID_ZOOM_IN
Command11=ID_APP_ABOUT
CommandCount=11

[MNU:IDR_DIBTRATYPE]
Type=1
Class=CDIBTransformView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_TRANSFORM_ZOOM
Command16=IDM_COLOR_TO_GRAY
Command17=IDM_LOWPASS_NOISE_REMOVE_3
Command18=IDM_LOWPASS_NOISE_REMOVE_5
Command19=IDM_HIGHPASS_BASIC_SHARPEN
Command20=IDM_HIGHPASS_MODERATE_SHARPEN
Command21=IDM_MEDIAN_NOISE_REMOVE
Command22=IDM_LAPLACIAN_BASIC_DETECT
Command23=IDM_LAPLACIAN_MODERATE_DETECT
Command24=IDM_SOBEL_EDGE_DETECT
Command25=IDM_KIRSCH_EDGE_DETECT
Command26=IDM_GRADIENT_EAST_DETECT
Command27=IDM_DIFFERENCE_EAST_DETECT
Command28=IDM_THRESHOLD_INCISE
Command29=IDM_PRETREATMENT
Command30=IDM_JUMP_POINT
Command31=IDM_LOCATE_MARK
Command32=IDM_MARK_LOCATION
Command33=IDM_PLATE_INCISE
Command34=IDM_PLATE_ADJUST
Command35=IDM_CHAR_SEGMENT
Command36=IDM_CHAR_STD_ALIGN
Command37=IDM_CHAR_THINING
Command38=IDM_CHAR_PRETREAT
Command39=IDM_CHAR_RECOGNISE
Command40=IDM_PLATE_RECOGNISE
Command41=IDM_IMAGE_RESTORE
Command42=ID_APP_ABOUT
Command43=ID_VIEW_TOOLBAR
Command44=ID_VIEW_STATUS_BAR
Command45=ID_WINDOW_NEW
Command46=ID_WINDOW_CASCADE
Command47=ID_WINDOW_TILE_HORZ
Command48=ID_WINDOW_ARRANGE
CommandCount=48

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_CUT
Command14=ID_EDIT_REDO
Command15=ID_EDIT_UNDO
CommandCount=15

[CLS:CRotateDlg]
Type=0
HeaderFile=RotateDlg.h
ImplementationFile=RotateDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRotateDlg
VirtualFilter=dWC

[DLG:IDD_ZOOM]
Type=1
Class=CZoomDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308865
Control4=IDC_STATIC,static,1342308865
Control5=IDC_XRATIO,edit,1350631552
Control6=IDC_YRATIO,edit,1350631552

[CLS:CZoomDlg]
Type=0
HeaderFile=ZoomDlg.h
ImplementationFile=ZoomDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_ZOOM

[DLG:IDD_ROTATE]
Type=1
Class=CRotateDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO1,button,1342308361
Control4=IDC_RADIO2,button,1342177289
Control5=IDC_RADIO3,button,1342177289
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_CANVASSIZE]
Type=1
Class=CanvasSizeDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_HEIGHT,edit,1350631552
Control7=IDC_WIDTH,edit,1350631552

[CLS:CanvasSizeDlg]
Type=0
HeaderFile=anvasSizeDlg.h
ImplementationFile=anvasSizeDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CanvasSizeDlg

