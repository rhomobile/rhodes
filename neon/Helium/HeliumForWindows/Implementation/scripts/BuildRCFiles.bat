@echo off
setlocal

set _VERSION=%1 %2 %3 %4 %5

cd /d "%~dp0..\Plugins"

echo --------------------------------------------------------------------------------
echo PWD: %CD%
echo generating resource files ...

call :rc "ImageWrapper for AYGShell" ImageWrapper resource.h WTG_SHWrapper.dll PB_Controls\ImageWrapper\ImageWrapper.rc
call :rc "SmartCradle" WTG_SmartCradle_PLG resource.h WTG_SmartCradle_PLG.dll PB_SmartCradle_PLG\PB_SmartCradle_PLG\PB_SmartCradle_PLG.rc
call :rc "RhoElements v1 Controls Plugin" WTG_Controls_PLG resource.h WTG_Controls_PLG.dll PB_Controls\PlugIn\PB_Controls_PLG.rc
call :rc "RhoElements v1 CardReader Plugin" WTG_CardReader_PLG resource.h WTG_CardReader_PLG.dll PB_CardReader_PLG\PB_CardReader_PLG.rc
call :rc "RhoElements v1 Communications Port Plugin" WTG_Comm_PLG resource2.h WTG_Comm_PLG.dll PB_Comm_PLG\PB_Comm_PLG\PB_Comm_PLG.rc
call :rc "RhoElements v1 Device and Application Plugin" WTG_DeviceApplication_PLG resource3.h WTG_DeviceApplication_PLG.dll PB_DeviceApplication_PLG\PB_DeviceApplication_PLG1.rc
call :rc "RhoElements v1 EMML Profiles Plugin" WTG_EMMP_PLG resource.h WTG_EMMP_PLG.dll PB_EMMP_PLG\PB_EMMP_PLG\PB_EMMP_PLG.rc
call :rc "RhoElements v1 File Transfer Plugin" WTG_FileTransfer_PLG resource.h WTG_FileTransfer_PLG.dll PB_FileTransfer_PLG\PB_FileTransfer_PLG.rc
call :rc "RhoElements v1 Image Capture Plugin" WTG_ImageCapture_PLG resource.h WTG_ImageCapture_PLG.dll PB_ImageCapture_PLG\PB_ImageCapture_PLG.rc
call :rc "RhoElements v1 Signature Capture Plugin" WTG_Signature_PLG resource.h WTG_Signature_PLG.dll PB_Signature_PLG\PB_Signature_PLG.rc
call :rc "RhoElements v1 Scanner Plugin" WTG_Scanner_PLG resource.h WTG_Scanner_PLG.dll WTG_Scanner_PLG\WTG_Scanner_PLG.rc
call :rc "RhoElements v1 Configuration Plugin" WTG_Config_PLG resource.h WTG_Config_PLG.dll PB_Configuration_PLG\PlugIn\resource.rc
call :rc "RhoElements v1 Gesture Plugin" WTG_Gesture_PLG resource.h WTG_Gesture_PLG.dll PB_Gesture_PLG\PlugIn\PlugIn.rc
call :rc "RhoElements v1 Key Capture Plugin" WTG_KeyCapture_PLG resource.h WTG_KeyCapture_PLG.dll PB_KeyCapture_PLG\PlugIn\PlugIn.rc
call :rc "RhoElements v1 Push Plugin" WTG_Push_PLG resource.h WTG_Push_PLG.dll PB_Push_PLG\PlugIn\PlugIn.rc
call :rc "RhoElements v1 Network Plugin" WTG_Network_PLG resource.h WTG_Network_PLG.dll PB_Network_PLG\PB_Network_PLG.rc
call :rc "RhoElements v1 AudioCapture Plugin" WTG_Multimedia_PLG resource.h WTG_Multimedia_PLG.dll PB_Multimedia_PLG\PB_Multimedia_PLG.rc
call :rc "RhoElements v1 Cradle Plugin" WTG_Cradle_PLG resource.h WTG_Cradle_PLG.dll PB_Cradle_PLG\PB_Cradle_PLG.rc
call :rc "RhoElements v1 Sensors Plugin" WTG_Sensors_PLG resource.h WTG_Sensors_PLG.dll PB_Sensors_PLG\PB_Sensors_PLG.rc
call :rc "Symbol RhoElements v1" RhoElements resource.h RhoElements.exe ..\PBCore\PBCore\PBCore.rc

endlocal
goto :eof

:rc
echo %5
call "%~dp0GenerateRCFile.bat" %1 %2 %3 %4 %_VERSION% >%5
goto :eof
