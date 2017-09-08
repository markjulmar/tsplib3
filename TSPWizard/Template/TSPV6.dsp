# Microsoft Developer Studio Project File - Name="$$Safe_root$$" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=$$Safe_root$$ - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "$$Safe_root$$.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "$$Safe_root$$.mak" CFG="$$Safe_root$$ - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "$$Safe_root$$ - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "$$Safe_root$$ - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "$$Safe_root$$ - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "\TSPLIB3\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib splib32.lib /nologo /dll /pdb:none /map /machine:I386 /out:"Release/$$Safe_root$$.tsp" /libpath:"\TSPLIB3\LIB"

!ELSEIF  "$(CFG)" == "$$Safe_root$$ - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "\TSPLIB3\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /map /debug /machine:I386 /out:"Debug/$$Safe_root$$.tsp" /pdbtype:sept /libpath:"\TSPLIB3\LIB"

!ENDIF 

# Begin Target

# Name "$$Safe_root$$ - Win32 Release"
# Name "$$Safe_root$$ - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\device.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\$$Safe_root$$.cpp
# End Source File
# Begin Source File

SOURCE=.\$$Safe_root$$.def
# End Source File
# Begin Source File

SOURCE=.\unsolicited.cpp
# End Source File
$$IF(OVERRIDE_LINE)
# Begin Source File

SOURCE=.\$$LINE_CPP_NAME$$
# End Source File
$$IF(OVERRIDE_ADDRESS)
# Begin Source File

SOURCE=.\$$ADDRESS_CPP_NAME$$
# End Source File
$$ENDIF
$$IF(OVERRIDE_CALL)
# Begin Source File

SOURCE=.\$$CALL_CPP_NAME$$
# End Source File
$$ENDIF
$$IF(!HAS_UI)
# Begin Source File

SOURCE=.\install.cpp
# End Source File
$$ENDIF
$$IF(OVERRIDE_CONFCALL)
# Begin Source File

SOURCE=.\$$CONFCALL_CPP_NAME$$
# End Source File
$$ENDIF
# Begin Source File

SOURCE=.\dropcall.cpp
# End Source File
# Begin Source File

SOURCE=.\makecall.cpp
# End Source File
$$IF(LINEFEATURE_0)
# Begin Source File

SOURCE=.\accept.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_1)
# Begin Source File

SOURCE=.\conference.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_2)
# Begin Source File

SOURCE=.\answer.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_3 || LINEFEATURE_4)
# Begin Source File

SOURCE=.\transfer.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_20)
# Begin Source File

SOURCE=.\secure.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_25)
# Begin Source File

SOURCE=.\devconfig.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_5)
# Begin Source File

SOURCE=.\complete.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_35)
# Begin Source File

SOURCE=.\route.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_7)
# Begin Source File

SOURCE=.\agent.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_12 || LINEFEATURE_13)
# Begin Source File

SOURCE=.\generate.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_14)
# Begin Source File

SOURCE=.\hold.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_21)
# Begin Source File

SOURCE=.\uui.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_18)
# Begin Source File

SOURCE=.\park.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_19)
# Begin Source File

SOURCE=.\pickup.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_9)
# Begin Source File

SOURCE=.\forward.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_26)
# Begin Source File

SOURCE=.\qos.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_27)
# Begin Source File

SOURCE=.\calltreat.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_22)
# Begin Source File

SOURCE=.\mediactl.cpp
# End Source File
$$ENDIF
$$IF(LINEFEATURE_23)
# Begin Source File

SOURCE=.\terminal.cpp
# End Source File
$$ENDIF
$$ENDIF
$$IF(OVERRIDE_PHONE)
# Begin Source File

SOURCE=.\$$PHONE_CPP_NAME$$
# End Source File
$$IF(PHONEFEATURE_1 || PHONEFEATURE_2)
# Begin Source File

SOURCE=.\phndata.cpp
# End Source File
$$ENDIF
$$IF(PHONEFEATURE_3)
$$IF(DISPLAY_CANWRITE)
# Begin Source File

SOURCE=.\display.cpp
# End Source File
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_4)
$$IF(BUTTON_CANWRITE || LAMP_CANWRITE)
# Begin Source File

SOURCE=.\btnlamp.cpp
# End Source File
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_5)
$$IF(GAIN_CANWRITE)
# Begin Source File

SOURCE=.\gain.cpp
# End Source File
$$ENDIF
$$IF(HOOKSWITCH_CANWRITE)
# Begin Source File

SOURCE=.\hswitch.cpp
# End Source File
$$ENDIF
$$IF(VOLUME_CANWRITE)
# Begin Source File

SOURCE=.\volume.cpp
# End Source File
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_6)
$$IF(RINGER_CANWRITE)
# Begin Source File

SOURCE=.\ringer.cpp
# End Source File
$$ENDIF
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_8 || PHONEFEATURE_0)
# Begin Source File

SOURCE=.\devspecific.cpp
# End Source File
$$ENDIF
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\$$Safe_root$$.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\$$Safe_root$$.rc
# End Source File
# End Group
# End Target
# End Project
