@echo off
setlocal

set _VERSION=%1 %2 %3 %4 %5

REM change to ActiveX folder
cd /d "%~dp0"

echo --------------------------------------------------------------------------------
echo PWD: %CD%
echo generating resource files ...

call :rc "RhoElements v1 No SIP Edit Control" NoSIP resourceppc.h NoSIP.dll PBNoSIP\PBNoSIPppc.rc
call :rc "RhoElements v1 CeODAX Control" CeODAX resourceppc.h CeODAX.dll PBODAX\CeODAX\CeODAXppc.rc
call :rc "RhoElements v1 Generic Control" PocketBrowser resourceppc.h PocketBrowser.dll PocketBrowser\PocketBrowserppc.rc

endlocal
goto :eof

:rc
echo %5
call "%~dp0GenerateRCFile.bat" %1 %2 %3 %4 %_VERSION% >%5
goto :eof
