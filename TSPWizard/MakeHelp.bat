@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TSPWizard.HPJ. >"hlp\TSPWizard.hm"
echo. >>"hlp\TSPWizard.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\TSPWizard.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\TSPWizard.hm"
echo. >>"hlp\TSPWizard.hm"
echo // Prompts (IDP_*) >>"hlp\TSPWizard.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\TSPWizard.hm"
echo. >>"hlp\TSPWizard.hm"
echo // Resources (IDR_*) >>"hlp\TSPWizard.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\TSPWizard.hm"
echo. >>"hlp\TSPWizard.hm"
echo // Dialogs (IDD_*) >>"hlp\TSPWizard.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\TSPWizard.hm"
echo. >>"hlp\TSPWizard.hm"
echo // Frame Controls (IDW_*) >>"hlp\TSPWizard.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\TSPWizard.hm"
REM -- Make help for Project TSPWizard

echo Building Win32 Help files
cd .\hlp
start /wait hcw /C /E /M "TSPWizard.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\TSPWizard.hlp" goto :Error
rem if not exist "hlp\TSPWizard.cnt" goto :Error
cd ..
echo.
if exist Debug\nul copy "hlp\TSPWizard.hlp" Debug
rem if exist Debug\nul copy "hlp\TSPWizard.cnt" Debug
if exist Release\nul copy "hlp\TSPWizard.hlp" Release
rem if exist Release\nul copy "hlp\TSPWizard.cnt" Release
echo.
goto :done

:Error
cd..
echo hlp\TSPWizard.hpj(1) : error: Problem encountered creating help file

:done
echo.
