@echo off

setlocal
set _EL=0

cd /d "%~dp0"

call :build PocketBrowser
call :build PBNoSIP
call :build PBODAX

echo.
if not %_EL% == 0 echo --------------------------------------------------------------------------------&&echo Error: build failed with exit code %_EL%.
endlocal&&exit /b %_EL%
goto :eof

:build
if not %_EL% == 0 goto :eof
set _PWD=%~dp0%1
set _CMD=devenv /nologo /rebuild Release %1.sln
echo.
echo --------------------------------------------------------------------------------
echo PWD: %_PWD%
echo CMD: %_CMD%
setlocal
cd /d "%_PWD%"&&%_CMD%
endlocal&&set _EL=%ERRORLEVEL%
goto :eof
