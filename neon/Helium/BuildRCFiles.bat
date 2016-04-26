@echo off
setlocal

set _VERSION=%1 %2 %3 %4 %5

REM change to the Helium Directory
cd /d "%~dp0"

echo --------------------------------------------------------------------------------
echo PWD: %CD%
echo generating resource files ...

call :rc "Symbol Web Technologies Group Compatibility Plugin" npwtg_legacy.dll resource.h npwtg_legacy.dll "application/x-wtg-legacy-activex|application/x-wtg-legacy-generic|application/x-wtg-legacy-odax|application/x-wtg-legacy-nosip|application/x-wtg-legacy-airbeam|application/x-wtg-legacy-apd" HeliumForWindows\Implementation\NPAPI\WTG_Legacy\wtg_legacy.rc
call :rc "Symbol Web Technologies Group JSObjects Plugin" npwtg_jsobjects.dll resource.h npwtg_jsobjects.dll "application/x-wtg-jsobjects" NPAPI\wtg_jsobjects.rc 

endlocal
goto :eof

:rc
echo %6
call "%~dp0GenerateRCFile.bat" %1 %2 %3 %4 %_VERSION% " " %5 >%6
goto :eof

