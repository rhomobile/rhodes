@echo off

REM %1 is Major Version Number
REM %2 is Minor Version Number 1
REM %3 is Minor Version Number 2
REM %4 is Minor Version Number 3
REM %5 is the Hot Fix Version

echo **********************************
echo ***                            ***
echo ***  BUILDING RESOURCE FILES   ***
echo ***                            ***
echo **********************************

REM Controls and Buttons
Call GenerateRCFile.bat "ImageWrapper for AYGShell" ImageWrapper resource.h WTG_SHWrapper.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Controls\ImageWrapper\ImageWrapper.rc
Call GenerateRCFile.bat "RhoElements v1 Controls Plugin" WTG_Controls_PLG resource.h WTG_Controls_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Controls\PlugIn\PB_Controls_PLG.rc

REM Card Reader
Call GenerateRCFile.bat "RhoElements v1 CardReader Plugin" WTG_CardReader_PLG resource.h WTG_CardReader_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_CardReader_PLG\PB_CardReader_PLG.rc

REM COMM Port
Call GenerateRCFile.bat "RhoElements v1 Communications Port Plugin" WTG_Comm_PLG resource2.h WTG_Comm_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Comm_PLG\PB_Comm_PLG\PB_Comm_PLG.rc

REM Device & Application
Call GenerateRCFile.bat "RhoElements v1 Device and Application Plugin" WTG_DeviceApplication_PLG resource3.h WTG_DeviceApplication_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_DeviceApplication_PLG\PB_DeviceApplication_PLG1.rc

REM EMML Profiles
Call GenerateRCFile.bat "RhoElements v1 EMML Profiles Plugin" WTG_EMMP_PLG resource.h WTG_EMMP_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_EMMP_PLG\PB_EMMP_PLG\PB_EMMP_PLG.rc

REM File Transfer
Call GenerateRCFile.bat "RhoElements v1 File Transfer Plugin" WTG_FileTransfer_PLG resource.h WTG_FileTransfer_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_FileTransfer_PLG\PB_FileTransfer_PLG.rc

REM Image Capture
Call GenerateRCFile.bat "RhoElements v1 Image Capture Plugin" WTG_ImageCapture_PLG resource.h WTG_ImageCapture_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_ImageCapture_PLG\PB_ImageCapture_PLG.rc

REM Signature Capture
Call GenerateRCFile.bat "RhoElements v1 Signature Capture Plugin" WTG_Signature_PLG resource.h WTG_Signature_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Signature_PLG\PB_Signature_PLG.rc

REM Scanner
Call GenerateRCFile.bat "RhoElements v1 Scanner Plugin" WTG_Scanner_PLG resource.h WTG_Scanner_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\WTG_Scanner_PLG\WTG_Scanner_PLG.rc

REM Configuration
Call GenerateRCFile.bat "RhoElements v1 Configuration Plugin" WTG_Config_PLG resource.h WTG_Config_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Configuration_PLG\PlugIn\resource.rc

REM Gestures
Call GenerateRCFile.bat "RhoElements v1 Gesture Plugin" WTG_Gesture_PLG resource.h WTG_Gesture_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Gesture_PLG\PlugIn\PlugIn.rc

REM Key Capture
Call GenerateRCFile.bat "RhoElements v1 Key Capture Plugin" WTG_KeyCapture_PLG resource.h WTG_KeyCapture_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_KeyCapture_PLG\PlugIn\PlugIn.rc

REM Push Server
Call GenerateRCFile.bat "RhoElements v1 Push Plugin" WTG_Push_PLG resource.h WTG_Push_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Push_PLG\PlugIn\PlugIn.rc

REM Network
Call GenerateRCFile.bat "RhoElements v1 Network Plugin" WTG_Network_PLG resource.h WTG_Network_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Network_PLG\PB_Network_PLG.rc

REM AudioCapture
Call GenerateRCFile.bat "RhoElements v1 AudioCapture Plugin" WTG_Multimedia_PLG resource.h WTG_Multimedia_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Multimedia_PLG\PB_Multimedia_PLG.rc

REM Cradle
Call GenerateRCFile.bat "RhoElements v1 Cradle Plugin" WTG_Cradle_PLG resource.h WTG_Cradle_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Cradle_PLG\PB_Cradle_PLG.rc

REM SmartCradle
Call GenerateRCFile.bat "RhoElements v1 Smart Cradle Plugin" WTG_SmartCradle_PLG resource.h WTG_SmartCradle_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_SmartCradle_PLG\PB_SmartCradle_PLG\PB_SmartCradle_PLG.rc

REM Sensors
Call GenerateRCFile.bat "RhoElements v1 Sensors Plugin" WTG_Sensors_PLG resource.h WTG_Sensors_PLG.dll %1 %2 %3 %4 %5 ..\..\Implementation\Plugins\PB_Sensors_PLG\PB_Sensors_PLG.rc

REM ActiveX NoSIP
Call GenerateRCFileActiveX.bat "RhoElements v1 No SIP Edit Control" NoSIP resourceppc.h NoSIP.dll %1 %2 %3 %4 %5 ..\..\Implementation\ActiveX\PBNoSIP\PBNoSIPppc.rc

REM ActiveX CeODAX
Call GenerateRCFileActiveX.bat "RhoElements v1 CeODAX Control" CeODAX resourceppc.h CeODAX.dll %1 %2 %3 %4 %5 ..\..\Implementation\ActiveX\PBODAX\CeODAX\CeODAXppc.rc

REM ActiveX RhoElements v1 (Generic)
Call GenerateRCFileActiveX.bat "RhoElements v1 Generic Control" PocketBrowser resourceppc.h PocketBrowser.dll %1 %2 %3 %4 %5 ..\..\Implementation\ActiveX\PocketBrowser\PocketBrowserppc.rc

REM NPAPI Legacy
Call GenerateRCFileNPAPI.bat "Symbol Web Technologies Group Compatibility Plugin" npwtg_legacy.dll resource.h npwtg_legacy.dll %1 %2 %3 %4 %5 ..\..\Implementation\NPAPI\WTG_Legacy\wtg_legacy.rc "application/x-wtg-legacy-activex|application/x-wtg-legacy-generic|application/x-wtg-legacy-odax|application/x-wtg-legacy-nosip|application/x-wtg-legacy-airbeam|application/x-wtg-legacy-apd"

REM NPAPI JSObjects
Call GenerateRCFileNPAPI.bat "Symbol Web Technologies Group JSObjects Plugin" npwtg_jsobjects.dll resource.h npwtg_jsobjects.dll %1 %2 %3 %4 %5 ..\..\..\NPAPI\wtg_jsobjects.rc "application/x-wtg-jsobjects"

REM CE Engine
Call GenerateRCFile.bat "RhoElements v1 CE Engine" PBEngine_COM resource.h PBEngine_COM.dll %1 %2 %3 %4 %5 ..\..\Implementation\Engine\IEEngine\IEEngineCE\IEEngineCE.rc

REM WM Engine
Call GenerateRCFile.bat "RhoElements v1 WM Engine" PBEngine_CTRL resource.h PBEngine_CTRL.dll %1 %2 %3 %4 %5 ..\..\Implementation\Engine\IEEngine\IEEngineMobile\IEMobileEngine.rc

REM Core
REM LOTS OF CHANGES REQUIRED HERE!!!!
Call GenerateRCFile.bat "Symbol RhoElements v1" RhoElements resource.h RhoElements.exe %1 %2 %3 %4 %5 ..\..\Implementation\PBCore\PBCore\PBCore.rc









