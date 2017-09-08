@echo off
rem build TSP++ librares.
rem NOTE: YOU MUST RUN THIS SCRIPT ONLY AFTER setting up VC environment!

if /"%DevEnvDir%"/ == /""/ goto usage

if not exist lib mkdir lib

cd SRC
nmake /a /f splib32.mak CFG="SPLIB32 - Win32 Release"
nmake /a /f splib32.mak CFG="SPLIB32 - Win32 Debug"
nmake /a /f splib32.mak CFG="SPLIB32 - Win32 Release Unicode"
nmake /a /f splib32.mak CFG="SPLIB32 - Win32 Debug Unicode"
cd ..

cd UISRC
nmake /a /f splui.mak CFG="Release"
nmake /a /f splui.mak CFG="Debug"
nmake /a /f splui.mak CFG="ReleaseUnicode"
nmake /a /f splui.mak CFG="DebugUnicode"
cd ..
:quit
goto done

:usage
echo ERROR: you must setup VS.NET for command line usage before building libraries
echo You can do this using the VSVARS32.BAT in your Visual Studio COMMON7\TOOLS directory.

:done
echo Finished.
pause

