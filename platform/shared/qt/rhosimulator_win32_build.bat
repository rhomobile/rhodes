@echo off
setlocal

cd /d "%~dp0"

call "%RHO_VSCMNTOOLS%..\..\VC\vcvarsall.bat" x86
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

rem "%QTDIR%\bin\qmake" -o Makefile -r -spec %RHO_QMAKE_SPEC% "CONFIG+=debug" "CONFIG-=release" "%1" RhoSimulator.pro
"%QTDIR%\bin\qmake" -o Makefile -r -spec %RHO_QMAKE_SPEC% "CONFIG-=debug" "CONFIG+=release" "%1" RhoSimulator.pro
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

rem nmake clean
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

nmake all
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

endlocal
