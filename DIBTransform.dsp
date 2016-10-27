# Microsoft Developer Studio Project File - Name="DIBTransform" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DIBTransform - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DIBTransform.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DIBTransform.mak" CFG="DIBTransform - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DIBTransform - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SkinPPWTL.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Target

# Name "DIBTransform - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CharPretreat.cpp
# End Source File
# Begin Source File

SOURCE=.\CharRecognise.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvolutionFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBTransform.rc
# End Source File
# Begin Source File

SOURCE=.\DIBTransformDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBTransformView.cpp
# End Source File
# Begin Source File

SOURCE=.\GrayProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageAreaProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageIncise.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\ImagePointProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MedianFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecialDetectionFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CharPretreat.h
# End Source File
# Begin Source File

SOURCE=.\CharRecognise.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ConvolutionFilter.h
# End Source File
# Begin Source File

SOURCE=.\Dib.h
# End Source File
# Begin Source File

SOURCE=.\DIBTransform.h
# End Source File
# Begin Source File

SOURCE=.\DIBTransformDoc.h
# End Source File
# Begin Source File

SOURCE=.\DIBTransformView.h
# End Source File
# Begin Source File

SOURCE=.\GrayProcess.h
# End Source File
# Begin Source File

SOURCE=.\ImageAreaProcess.h
# End Source File
# Begin Source File

SOURCE=.\ImageEdge.h
# End Source File
# Begin Source File

SOURCE=.\ImageGeometry.h
# End Source File
# Begin Source File

SOURCE=.\ImageIncise.h
# End Source File
# Begin Source File

SOURCE=.\ImageLocation.h
# End Source File
# Begin Source File

SOURCE=.\ImagePointProcess.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcess.h
# End Source File
# Begin Source File

SOURCE=.\Img.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MedianFilter.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SpecialDetectionFilter.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DIBTransform.ico
# End Source File
# Begin Source File

SOURCE=.\res\DIBTransform.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DIBTransformDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\lcross.cur
# End Source File
# Begin Source File

SOURCE=.\res\MAGNIFY.CUR
# End Source File
# Begin Source File

SOURCE=.\res\magnify_1.cur
# End Source File
# Begin Source File

SOURCE=.\res\magnify_2.cur
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
