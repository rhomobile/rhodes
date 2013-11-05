@echo off
setlocal

cd /d "%~dp0"

call "%VS110COMNTOOLS%..\..\VC\vcvarsall.bat" x86
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

"%QTDIR%\bin\qmake" -o Makefile -r -spec win32-msvc2012 "CONFIG-=debug" "CONFIG+=release" "%1" RhoSimulator.pro
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

nmake clean
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

nmake all
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

endlocal
