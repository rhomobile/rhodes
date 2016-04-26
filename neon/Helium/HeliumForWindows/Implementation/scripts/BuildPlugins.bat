@echo off

setlocal
set _EL=0


cd /d "%~dp0.."
call :build PlgLibrary\PBPlugin.sln PBPlugin

cd Plugins
call :build PB_SmartCradle_PLG\PB_SmartCradle_PLG.sln PB_SmartCradle_PLG
call :build PB_Multimedia_PLG\PB_Multimedia_PLG.sln PB_Multimedia_PLG
call :build PB_Controls\PBControls.sln ImageWrapper
call :build PB_Controls\PBControls.sln PlugIn
call :build PB_CardReader_PLG\PB_CardReader_PLG.sln PB_CardReader_PLG.vcproj
call :build PB_Comm_PLG\PB_Comm_PLG.sln PB_Comm_PLG
call :build PB_Cradle_PLG\PB_CRADLE_PLG.sln PB_Cradle_PLG
call :build PB_DeviceApplication_PLG\PB_DeviceApplication_PLG.sln PB_DeviceApplication_PLG.vcproj
call :build PB_EMMP_PLG\PB_EMMP_PLG.sln PB_EMMP_PLG
call :build PB_FileTransfer_PLG\PB_FileTransfer_PLG.sln PB_FileTransfer_PLG
call :build PB_ImageCapture_PLG\PB_ImageCapture_PLG.sln PB_ImageCapture_PLG "Release|Windows Mobile 5.0 Pocket PC SDK (ARMV4I)"
call :build PB_Sensors_PLG\PB_Sensors_PLG.sln PB_Sensors_PLG
call :build PB_Signature_PLG\PB_Signature_PLG.sln PB_Signature_PLG
call :build WTG_Scanner_PLG\WTG_Scanner_PLG.sln WTG_Scanner_PLG
call :build PB_Configuration_PLG\PB_Configuration_PLG.sln PlugIn
call :build PB_Gesture_PLG\PB_Gesture_PLG.sln PB_Gesture_PLG
call :build PB_KeyCapture_PLG\PB_KeyCapture_PLG.sln PB_KeyCapture_PLG
call :build PB_Push_PLG\PB_Push_PLG.sln PB_Push_PLG
call :build PB_Network_PLG\PB_Network_PLG.sln PB_Network_PLG
if not %_EL% == 0 goto :exit


cd "%~dp0..\bin"

call :abs _TARGET %~dp0..\..\..\..\R2D1\Build\Windows\Plugins
if not exist %_TARGET% mkdir %_TARGET%

echo.
echo --------------------------------------------------------------------------------
echo FROM: %CD%
echo TO  : %_TARGET%
echo copying files ...
call :cp WTG_CardReader_PLG.dll
call :cp WTG_Comm_PLG.dll
call :cp WTG_Config_PLG.dll
call :cp WTG_Controls_PLG.dll
call :cp WTG_DeviceApplication_PLG.dll
call :cp WTG_EMMP_PLG.dll
call :cp WTG_FileTransfer_PLG.dll
call :cp WTG_Gesture_PLG.dll
call :cp WTG_ImageCapture_PLG.dll
call :cp WTG_KeyCapture_PLG.dll
call :cp WTG_Network_PLG.dll
call :cp WTG_Push_PLG.dll
call :cp WTG_Scanner_PLG.dll
call :cp WTG_SHWrapper.dll
call :cp WTG_Signature_PLG.dll
call :cp WTG_Sensors_PLG.dll
call :cp WTG_Multimedia_PLG.dll
call :cp WTG_Cradle_PLG.dll
call :cp WTG_SmartCradle_PLG.dll


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
