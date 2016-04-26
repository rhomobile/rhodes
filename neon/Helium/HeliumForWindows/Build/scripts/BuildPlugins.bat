REM %1 is Rebuild or Build
REM %2 is Major Version Number
REM %3 is Minor Version Number 1
REM %4 is Minor Version Number 2
REM %5 is Minor Version Number 3

@echo off

echo ***************************
echo ***                     ***
echo ***  BUILDING PLUGINS   ***
echo ***                     ***
echo ***************************
echo. 

	
set BUILDTYPE=%1
if not "%BUILDTYPE%"=="rebuild" (
	set BUILDTYPE=build
	echo BUILDTYPE is /build
	goto BUILDPLUGINS
	)
echo BUILDTYPE is /rebuild

:BUILDPLUGINS
echo.

REM Build the Plugin Dependencies
echo BUILDING PLUGIN.LIB
echo.
devenv /nologo /%BUILDTYPE% Release ../../Implementation/PlgLibrary/PBPlugin.sln /project PBPlugin /out ../errorFiles/PBPlugin.lib.txt
if not exist "../../Implementation/Common/Public/PBPlugin.lib" goto BUILDFAILED
echo.

REM Build the Plugins
echo BUILDING MULTIMEDIA CAPTURE
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Multimedia_PLG/PB_Multimedia_PLG.sln /project PB_Multimedia_PLG /out ../errorFiles/PB_Multimedia_PLG.txt
if not exist "../../Implementation/Bin/WTG_Multimedia_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING CONTROLS (BUTTONS AND PB_SHWrapper)
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Controls/PBControls.sln /project ImageWrapper /out ../errorFiles/WTG_ImageWrapper_PLG.txt
if not exist "../../Implementation/Bin/WTG_SHWrapper.dll" goto BUILDFAILED
echo.
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Controls/PBControls.sln /project PlugIn /out ../errorFiles/WTG_Controls_PLG.txt
if not exist "../../Implementation/Bin/WTG_Controls_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING CARD READER
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_CardReader_PLG/PB_CardReader_PLG.sln /project PB_CardReader_PLG.vcproj /out ../errorFiles/WTG_CardReader_PLG.txt
if not exist "../../Implementation/Bin/WTG_CardReader_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING COMM PORT
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Comm_PLG/PB_Comm_PLG.sln /project PB_Comm_PLG /out ../errorFiles/WTG_Comm_PLG.txt
if not exist "../../Implementation/Bin/WTG_Comm_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING CRADLE
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Cradle_PLG/PB_CRADLE_PLG.sln /project PB_Cradle_PLG /out ../errorFiles/WTG_Cradle_PLG.txt
if not exist "../../Implementation/Bin/WTG_Cradle_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING SMART CRADLE
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_SmartCradle_PLG/PB_SmartCradle_PLG.sln /project PB_SmartCradle_PLG /out ../errorFiles/WTG_SmartCradle_PLG.txt
if not exist "../../Implementation/Bin/WTG_SmartCradle_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING DEVICE AND APPLICATION
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_DeviceApplication_PLG/PB_DeviceApplication_PLG.sln /project PB_DeviceApplication_PLG.vcproj /out ../errorFiles/WTG_DeviceApplication_PLG.txt
if not exist "../../Implementation/Bin/WTG_DeviceApplication_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING EMML PROFILES
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_EMMP_PLG/PB_EMMP_PLG.sln /project PB_EMMP_PLG /out ../errorFiles/WTG_EMMLP_PLG.txt
if not exist "../../Implementation/Bin/WTG_EMMP_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING FILE TRANSFER
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_FileTransfer_PLG/PB_FileTransfer_PLG.sln /project PB_FileTransfer_PLG /out ../errorFiles/WTG_FileTransfer_PLG.txt
if not exist "../../Implementation/Bin/WTG_FileTransfer_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING IMAGE CAPTURE
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_ImageCapture_PLG/PB_ImageCapture_PLG.sln /project PB_ImageCapture_PLG /projectconfig "Release|Windows Mobile 5.0 Pocket PC SDK (ARMV4I)" /out ../errorFiles/WTG_ImageCapture_PLG.txt
if not exist "../../Implementation/Bin/WTG_ImageCapture_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING SENSORS
echo.
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Sensors_PLG/PB_Sensors_PLG.sln /project PB_Sensors_PLG /out ../errorFiles/PB_Sensors_PLG.txt
if not exist "../../Implementation/Bin/WTG_Sensors_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING SIGNATURE CAPTURE
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Signature_PLG/PB_Signature_PLG.sln /project PB_Signature_PLG /out ../errorFiles/WTG_SignatureCapture_PLG.txt
if not exist "../../Implementation/Bin/WTG_Signature_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING SCANNER
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/WTG_Scanner_PLG/WTG_Scanner_PLG.sln /project WTG_Scanner_PLG /out ../errorFiles/WTG_Scanner_PLG.txt
if not exist "../../Implementation/Bin/WTG_Scanner_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING CONFIGURATION
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Configuration_PLG/PB_Configuration_PLG.sln /project PlugIn /out ../errorFiles/WTG_Configuration_PLG.txt
if not exist "../../Implementation/Bin/WTG_Config_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING GESTURES
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Gesture_PLG/PB_Gesture_PLG.sln /project PB_Gesture_PLG /out ../errorFiles/WTG_Gesture_PLG.txt
if not exist "../../Implementation/Bin/WTG_Gesture_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING KEY CAPTURE
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_KeyCapture_PLG/PB_KeyCapture_PLG.sln /project PB_KeyCapture_PLG /out ../errorFiles/WTG_KeyCapture_PLG.txt
if not exist "../../Implementation/Bin/WTG_KeyCapture_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING PUSH SERVER
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Push_PLG/PB_Push_PLG.sln /project PB_Push_PLG /out ../errorFiles/WTG_Push_PLG.txt
if not exist "../../Implementation/Bin/WTG_Push_PLG.dll" goto BUILDFAILED
echo.

echo BUILDING NETWORK
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/Plugins/PB_Network_PLG/PB_Network_PLG.sln /project PB_Network_PLG /out ../errorFiles/WTG_Network_PLG.txt
if not exist "../../Implementation/Bin/WTG_Network_PLG.dll" goto BUILDFAILED
echo.

GOTO BUILDSUCCEEDED

:BUILDFAILED

echo *******************************************
echo ***                                     ***
echo ***           BUILD FAILED!!!!          ***
echo *** CHECK ERROR OUTPUT FOR LAST PLUG-IN ***
echo ***                                     ***
echo *******************************************
pause
exit
GOTO ENDOFFILE

:BUILDSUCCEEDED

echo *******************************************
echo ***                                     ***
echo ***      PLUGINS BUILD SUCCESS   =)      ***
echo ***                                     ***
echo *******************************************

:ENDOFFILE






