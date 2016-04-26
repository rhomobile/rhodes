@echo off

setlocal
set _EL=0

cd /d "%~dp0"

REM Build the NPAPI JS files from XML sources
setlocal
cd Generation\JSObjects
call :build NPAPIGenerator.sln NPAPIGenerator
call Generate_NPAPI.bat
endlocal

REM Build the NPAPI DLLs
setlocal
call :build HeliumForWindows\Implementation\NPAPI\WTG_Legacy\wtg_legacy.sln wtg_legacy
call :build NPAPI\wtg_jsobjects.sln wtg_jsobjects
call :build ..\..\npapi\webkit-bridge\bridge.sln bridge
endlocal
if not %_EL% == 0 goto :exit

REM it is not required to copy the files anywhere because their project settings automatically build them in the correct location, R2D1\Build\Windows\NPAPI
endlocal
goto :eof


:exit
echo.
if not %_EL% == 0 echo --------------------------------------------------------------------------------&&echo Error: build failed with exit code %_EL%.
endlocal&&exit /b %_EL%
goto :eof

:build
if not %_EL% == 0 goto :eof
set _CMD=devenv /nologo /rebuild Release %~nx1 /project %2
if not [%3] == [] set _CMD=%_CMD% /projectconfig %3
echo.
echo --------------------------------------------------------------------------------
echo PWD: %~dp1
echo CMD: %_CMD%
setlocal
cd /d "%~dp1"&&%_CMD%
endlocal&&set _EL=%ERRORLEVEL%
goto :eof

:cp
if not %_EL% == 0 goto :eof
echo %1
copy %1 "%_TARGET%" >nul
set _EL=%ERRORLEVEL%
goto :eof

:abs
set %1=%~f2
goto :eof

