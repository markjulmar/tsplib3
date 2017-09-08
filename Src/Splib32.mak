# Microsoft Developer Studio Generated NMAKE File, Based on Splib32.dsp
!IF "$(CFG)" == ""
CFG=SPLIB32 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SPLIB32 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SPLIB32 - Win32 Release" && "$(CFG)" != "SPLIB32 - Win32 Debug" && "$(CFG)" != "SPLIB32 - Win32 Debug Unicode" && "$(CFG)" != "SPLIB32 - Win32 Release Unicode"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Splib32.mak" CFG="SPLIB32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SPLIB32 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SPLIB32 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SPLIB32 - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "SPLIB32 - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\LIB\SPLIB32.lib"


CLEAN :
	-@erase "$(INTDIR)\Address.obj"
	-@erase "$(INTDIR)\Agent.obj"
	-@erase "$(INTDIR)\Call.obj"
	-@erase "$(INTDIR)\CallHub.obj"
	-@erase "$(INTDIR)\Confcall.obj"
	-@erase "$(INTDIR)\Conn.obj"
	-@erase "$(INTDIR)\CritSec.obj"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\Device.obj"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Lineconn.obj"
	-@erase "$(INTDIR)\Misc.obj"
	-@erase "$(INTDIR)\Phonecon.obj"
	-@erase "$(INTDIR)\PoolMgr.obj"
	-@erase "$(INTDIR)\Request.obj"
	-@erase "$(INTDIR)\Serialize.obj"
	-@erase "$(INTDIR)\Sp.obj"
	-@erase "$(INTDIR)\Spdll.obj"
	-@erase "$(INTDIR)\Splib32.pch"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\TSPLayer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\LIB\SPLIB32.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /YX"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Splib32.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\LIB\SPLIB32.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Address.obj" \
	"$(INTDIR)\Agent.obj" \
	"$(INTDIR)\Call.obj" \
	"$(INTDIR)\CallHub.obj" \
	"$(INTDIR)\Confcall.obj" \
	"$(INTDIR)\Conn.obj" \
	"$(INTDIR)\CritSec.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\Device.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\Lineconn.obj" \
	"$(INTDIR)\Misc.obj" \
	"$(INTDIR)\Phonecon.obj" \
	"$(INTDIR)\PoolMgr.obj" \
	"$(INTDIR)\Request.obj" \
	"$(INTDIR)\Serialize.obj" \
	"$(INTDIR)\Sp.obj" \
	"$(INTDIR)\Spdll.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\TSPLayer.obj"

"..\LIB\SPLIB32.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\LIB\SPLIB32D.lib" "$(OUTDIR)\Splib32.bsc"


CLEAN :
	-@erase "$(INTDIR)\Address.obj"
	-@erase "$(INTDIR)\Address.sbr"
	-@erase "$(INTDIR)\Agent.obj"
	-@erase "$(INTDIR)\Agent.sbr"
	-@erase "$(INTDIR)\Call.obj"
	-@erase "$(INTDIR)\Call.sbr"
	-@erase "$(INTDIR)\CallHub.obj"
	-@erase "$(INTDIR)\CallHub.sbr"
	-@erase "$(INTDIR)\Confcall.obj"
	-@erase "$(INTDIR)\Confcall.sbr"
	-@erase "$(INTDIR)\Conn.obj"
	-@erase "$(INTDIR)\Conn.sbr"
	-@erase "$(INTDIR)\CritSec.obj"
	-@erase "$(INTDIR)\CritSec.sbr"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\Debug.sbr"
	-@erase "$(INTDIR)\Device.obj"
	-@erase "$(INTDIR)\Device.sbr"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Display.sbr"
	-@erase "$(INTDIR)\Lineconn.obj"
	-@erase "$(INTDIR)\Lineconn.sbr"
	-@erase "$(INTDIR)\Misc.obj"
	-@erase "$(INTDIR)\Misc.sbr"
	-@erase "$(INTDIR)\Phonecon.obj"
	-@erase "$(INTDIR)\Phonecon.sbr"
	-@erase "$(INTDIR)\PoolMgr.obj"
	-@erase "$(INTDIR)\PoolMgr.sbr"
	-@erase "$(INTDIR)\Request.obj"
	-@erase "$(INTDIR)\Request.sbr"
	-@erase "$(INTDIR)\Serialize.obj"
	-@erase "$(INTDIR)\Serialize.sbr"
	-@erase "$(INTDIR)\Sp.obj"
	-@erase "$(INTDIR)\Sp.sbr"
	-@erase "$(INTDIR)\Spdll.obj"
	-@erase "$(INTDIR)\Spdll.sbr"
	-@erase "$(INTDIR)\Splib32.pch"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\TSPLayer.obj"
	-@erase "$(INTDIR)\TSPLayer.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Splib32.bsc"
	-@erase "..\LIB\SPLIB32D.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /YX"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Splib32.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Address.sbr" \
	"$(INTDIR)\Call.sbr" \
	"$(INTDIR)\CallHub.sbr" \
	"$(INTDIR)\Confcall.sbr" \
	"$(INTDIR)\Conn.sbr" \
	"$(INTDIR)\CritSec.sbr" \
	"$(INTDIR)\Debug.sbr" \
	"$(INTDIR)\Device.sbr" \
	"$(INTDIR)\Display.sbr" \
	"$(INTDIR)\Lineconn.sbr" \
	"$(INTDIR)\Misc.sbr" \
	"$(INTDIR)\Phonecon.sbr" \
	"$(INTDIR)\PoolMgr.sbr" \
	"$(INTDIR)\Request.sbr" \
	"$(INTDIR)\Serialize.sbr" \
	"$(INTDIR)\Sp.sbr" \
	"$(INTDIR)\Spdll.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\TSPLayer.sbr"

"$(OUTDIR)\Splib32.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\LIB\SPLIB32D.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Address.obj" \
	"$(INTDIR)\Agent.obj" \
	"$(INTDIR)\Call.obj" \
	"$(INTDIR)\CallHub.obj" \
	"$(INTDIR)\Confcall.obj" \
	"$(INTDIR)\Conn.obj" \
	"$(INTDIR)\CritSec.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\Device.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\Lineconn.obj" \
	"$(INTDIR)\Misc.obj" \
	"$(INTDIR)\Phonecon.obj" \
	"$(INTDIR)\PoolMgr.obj" \
	"$(INTDIR)\Request.obj" \
	"$(INTDIR)\Serialize.obj" \
	"$(INTDIR)\Sp.obj" \
	"$(INTDIR)\Spdll.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\TSPLayer.obj"

"..\LIB\SPLIB32D.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

OUTDIR=.\DebugUnicode
INTDIR=.\DebugUnicode
# Begin Custom Macros
OutDir=.\DebugUnicode
# End Custom Macros

ALL : "..\LIB\SPLIB32UD.LIB" "$(OUTDIR)\Splib32.bsc"


CLEAN :
	-@erase "$(INTDIR)\Address.obj"
	-@erase "$(INTDIR)\Address.sbr"
	-@erase "$(INTDIR)\Agent.obj"
	-@erase "$(INTDIR)\Agent.sbr"
	-@erase "$(INTDIR)\Call.obj"
	-@erase "$(INTDIR)\Call.sbr"
	-@erase "$(INTDIR)\CallHub.obj"
	-@erase "$(INTDIR)\CallHub.sbr"
	-@erase "$(INTDIR)\Confcall.obj"
	-@erase "$(INTDIR)\Confcall.sbr"
	-@erase "$(INTDIR)\Conn.obj"
	-@erase "$(INTDIR)\Conn.sbr"
	-@erase "$(INTDIR)\CritSec.obj"
	-@erase "$(INTDIR)\CritSec.sbr"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\Debug.sbr"
	-@erase "$(INTDIR)\Device.obj"
	-@erase "$(INTDIR)\Device.sbr"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Display.sbr"
	-@erase "$(INTDIR)\Lineconn.obj"
	-@erase "$(INTDIR)\Lineconn.sbr"
	-@erase "$(INTDIR)\Misc.obj"
	-@erase "$(INTDIR)\Misc.sbr"
	-@erase "$(INTDIR)\Phonecon.obj"
	-@erase "$(INTDIR)\Phonecon.sbr"
	-@erase "$(INTDIR)\PoolMgr.obj"
	-@erase "$(INTDIR)\PoolMgr.sbr"
	-@erase "$(INTDIR)\Request.obj"
	-@erase "$(INTDIR)\Request.sbr"
	-@erase "$(INTDIR)\Serialize.obj"
	-@erase "$(INTDIR)\Serialize.sbr"
	-@erase "$(INTDIR)\Sp.obj"
	-@erase "$(INTDIR)\Sp.sbr"
	-@erase "$(INTDIR)\Spdll.obj"
	-@erase "$(INTDIR)\Spdll.sbr"
	-@erase "$(INTDIR)\Splib32.pch"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\TSPLayer.obj"
	-@erase "$(INTDIR)\TSPLayer.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Splib32.bsc"
	-@erase "..\LIB\SPLIB32UD.LIB"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /YX"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Splib32.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Address.sbr" \
	"$(INTDIR)\Call.sbr" \
	"$(INTDIR)\CallHub.sbr" \
	"$(INTDIR)\Confcall.sbr" \
	"$(INTDIR)\Conn.sbr" \
	"$(INTDIR)\CritSec.sbr" \
	"$(INTDIR)\Debug.sbr" \
	"$(INTDIR)\Device.sbr" \
	"$(INTDIR)\Display.sbr" \
	"$(INTDIR)\Lineconn.sbr" \
	"$(INTDIR)\Misc.sbr" \
	"$(INTDIR)\Phonecon.sbr" \
	"$(INTDIR)\PoolMgr.sbr" \
	"$(INTDIR)\Request.sbr" \
	"$(INTDIR)\Serialize.sbr" \
	"$(INTDIR)\Sp.sbr" \
	"$(INTDIR)\Spdll.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\TSPLayer.sbr"

"$(OUTDIR)\Splib32.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\LIB\SPLIB32UD.LIB" 
LIB32_OBJS= \
	"$(INTDIR)\Address.obj" \
	"$(INTDIR)\Agent.obj" \
	"$(INTDIR)\Call.obj" \
	"$(INTDIR)\CallHub.obj" \
	"$(INTDIR)\Confcall.obj" \
	"$(INTDIR)\Conn.obj" \
	"$(INTDIR)\CritSec.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\Device.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\Lineconn.obj" \
	"$(INTDIR)\Misc.obj" \
	"$(INTDIR)\Phonecon.obj" \
	"$(INTDIR)\PoolMgr.obj" \
	"$(INTDIR)\Request.obj" \
	"$(INTDIR)\Serialize.obj" \
	"$(INTDIR)\Sp.obj" \
	"$(INTDIR)\Spdll.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\TSPLayer.obj"

"..\LIB\SPLIB32UD.LIB" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

OUTDIR=.\ReleaseUnicode
INTDIR=.\ReleaseUnicode

ALL : "..\LIB\SPLIB32U.lib"


CLEAN :
	-@erase "$(INTDIR)\Address.obj"
	-@erase "$(INTDIR)\Agent.obj"
	-@erase "$(INTDIR)\Call.obj"
	-@erase "$(INTDIR)\CallHub.obj"
	-@erase "$(INTDIR)\Confcall.obj"
	-@erase "$(INTDIR)\Conn.obj"
	-@erase "$(INTDIR)\CritSec.obj"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\Device.obj"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Lineconn.obj"
	-@erase "$(INTDIR)\Misc.obj"
	-@erase "$(INTDIR)\Phonecon.obj"
	-@erase "$(INTDIR)\PoolMgr.obj"
	-@erase "$(INTDIR)\Request.obj"
	-@erase "$(INTDIR)\Serialize.obj"
	-@erase "$(INTDIR)\Sp.obj"
	-@erase "$(INTDIR)\Spdll.obj"
	-@erase "$(INTDIR)\Splib32.pch"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\TSPLayer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\LIB\SPLIB32U.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /YX"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Splib32.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\LIB\SPLIB32U.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Address.obj" \
	"$(INTDIR)\Agent.obj" \
	"$(INTDIR)\Call.obj" \
	"$(INTDIR)\CallHub.obj" \
	"$(INTDIR)\Confcall.obj" \
	"$(INTDIR)\Conn.obj" \
	"$(INTDIR)\CritSec.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\Device.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\Lineconn.obj" \
	"$(INTDIR)\Misc.obj" \
	"$(INTDIR)\Phonecon.obj" \
	"$(INTDIR)\PoolMgr.obj" \
	"$(INTDIR)\Request.obj" \
	"$(INTDIR)\Serialize.obj" \
	"$(INTDIR)\Sp.obj" \
	"$(INTDIR)\Spdll.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\TSPLayer.obj"

"..\LIB\SPLIB32U.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Splib32.dep")
!INCLUDE "Splib32.dep"
!ELSE 
!MESSAGE Warning: cannot find "Splib32.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SPLIB32 - Win32 Release" || "$(CFG)" == "SPLIB32 - Win32 Debug" || "$(CFG)" == "SPLIB32 - Win32 Debug Unicode" || "$(CFG)" == "SPLIB32 - Win32 Release Unicode"
SOURCE=.\Address.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Address.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Address.obj"	"$(INTDIR)\Address.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Address.obj"	"$(INTDIR)\Address.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Address.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Agent.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Agent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Agent.obj"	"$(INTDIR)\Agent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Agent.obj"	"$(INTDIR)\Agent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Agent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 
SOURCE=.\Call.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Call.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Call.obj"	"$(INTDIR)\Call.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Call.obj"	"$(INTDIR)\Call.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Call.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CallHub.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\CallHub.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\CallHub.obj"	"$(INTDIR)\CallHub.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\CallHub.obj"	"$(INTDIR)\CallHub.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\CallHub.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Confcall.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Confcall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Confcall.obj"	"$(INTDIR)\Confcall.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Confcall.obj"	"$(INTDIR)\Confcall.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Confcall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Conn.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Conn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Conn.obj"	"$(INTDIR)\Conn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Conn.obj"	"$(INTDIR)\Conn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Conn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CritSec.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\CritSec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\CritSec.obj"	"$(INTDIR)\CritSec.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\CritSec.obj"	"$(INTDIR)\CritSec.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\CritSec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Debug.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Debug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Debug.obj"	"$(INTDIR)\Debug.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Debug.obj"	"$(INTDIR)\Debug.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Debug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Device.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Device.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Device.obj"	"$(INTDIR)\Device.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Device.obj"	"$(INTDIR)\Device.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Device.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Display.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Display.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Display.obj"	"$(INTDIR)\Display.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Display.obj"	"$(INTDIR)\Display.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Display.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Lineconn.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Lineconn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Lineconn.obj"	"$(INTDIR)\Lineconn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Lineconn.obj"	"$(INTDIR)\Lineconn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Lineconn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Misc.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Misc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Misc.obj"	"$(INTDIR)\Misc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Misc.obj"	"$(INTDIR)\Misc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Misc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Phonecon.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Phonecon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Phonecon.obj"	"$(INTDIR)\Phonecon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Phonecon.obj"	"$(INTDIR)\Phonecon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Phonecon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\PoolMgr.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PoolMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\PoolMgr.obj"	"$(INTDIR)\PoolMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\PoolMgr.obj"	"$(INTDIR)\PoolMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PoolMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Request.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Request.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Request.obj"	"$(INTDIR)\Request.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Request.obj"	"$(INTDIR)\Request.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Request.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Serialize.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Serialize.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Serialize.obj"	"$(INTDIR)\Serialize.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Serialize.obj"	"$(INTDIR)\Serialize.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Serialize.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Sp.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sp.obj"	"$(INTDIR)\Sp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sp.obj"	"$(INTDIR)\Sp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /WX /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Spdll.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Spdll.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Spdll.obj"	"$(INTDIR)\Spdll.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Spdll.obj"	"$(INTDIR)\Spdll.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Spdll.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yc"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\Splib32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yc"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\stdafx.sbr"	"$(INTDIR)\Splib32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yc"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\stdafx.sbr"	"$(INTDIR)\Splib32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yc"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\Splib32.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TSPLayer.cpp

!IF  "$(CFG)" == "SPLIB32 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TSPLayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Ob1 /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /Fa"$(INTDIR)\\" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\TSPLayer.obj"	"$(INTDIR)\TSPLayer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W4 /WX /Gi /GR /GX /Zi /Od /Gf /Gy /I "..\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\TSPLayer.obj"	"$(INTDIR)\TSPLayer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SPLIB32 - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W4 /GR /GX /O1 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Splib32.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TSPLayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Splib32.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

