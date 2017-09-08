# Microsoft Developer Studio Project File - Name="TestUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TestUI - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Testui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Testui.mak" CFG="TestUI - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestUI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TestUI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "TestUI - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE "TestUI - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TestUI - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "\TSPLIB3\INCLUDE" /I "S:\JULMAR ENTERTAINMENT\TSPLIB3\INCLUDE" /D "STRICT" /D "WIN32" /D "_MT" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /version:4.0 /subsystem:windows /dll /pdb:none /machine:I386 /libpath:"\TSPLIB3\LIB" /libpath:"S:\JULMAR ENTERTAINMENT\TSPLIB3\LIB"

!ELSEIF  "$(CFG)" == "TestUI - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GR /GX /ZI /Od /I "\TSPLIB3\INCLUDE" /I "S:\JULMAR ENTERTAINMENT\TSPLIB3\INCLUDE" /D "STRICT" /D "WIN32" /D "_MT" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x19600000" /version:4.0 /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /pdbtype:sept /libpath:"S:\JULMAR ENTERTAINMENT\TSPLIB3\LIB"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TestUI - Win32 Debug Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestUI___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "TestUI___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DebugUnicode"
# PROP Intermediate_Dir ".\DebugUnicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GR /GX /ZI /Od /I "\TSPLIB3\Include" /D "STRICT" /D "WIN32" /D "_MT" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GR /GX /ZI /Od /I "\TSPLIB3\INCLUDE" /I "S:\JULMAR ENTERTAINMENT\TSPLIB3\INCLUDE" /D "STRICT" /D "WIN32" /D "_MT" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x19600000" /version:4.0 /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"DebugUnicode\TestUI.DLL" /pdbtype:sept /libpath:"\TSPLIB3\LIB"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /base:"0x19600000" /version:4.0 /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"DebugUnicode\TestUI.dll" /pdbtype:sept /libpath:"\TSPLIB3\LIB" /libpath:"S:\JULMAR ENTERTAINMENT\TSPLIB3\LIB"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TestUI - Win32 Release Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TestUI___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "TestUI___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ReleaseUnicode"
# PROP Intermediate_Dir ".\ReleaseUnicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /WX /GX /O2 /I "\TSPLIB3\INCLUDE" /I "\TSPLIB3\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "S:\JULMAR ENTERTAINMENT\TSPLIB3\INCLUDE" /D "STRICT" /D "WIN32" /D "_MT" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"ReleaseUnicode\TestUI.DLL" /libpath:"\TSPLIB3\LIB"
# ADD LINK32 /nologo /version:4.0 /subsystem:windows /dll /pdb:none /machine:I386 /out:"ReleaseUnicode\TestUI.dll" /libpath:"S:\JULMAR ENTERTAINMENT\TSPLIB3\LIB"

!ENDIF 

# Begin Target

# Name "TestUI - Win32 Release"
# Name "TestUI - Win32 Debug"
# Name "TestUI - Win32 Debug Unicode"
# Name "TestUI - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TestUI.CPP
# End Source File
# Begin Source File

SOURCE=.\TestUI.def
# End Source File
# Begin Source File

SOURCE=.\TestUI.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestUI.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\TestUI.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
