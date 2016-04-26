@echo off
echo *************************************************************
echo ***                                                       ***
echo ***          Copy Dependent Header Files From             ***
echo ***            1.Windows Mobile 6 SDK PATH                ***
echo ***            2.Windows Mobile 6.5.3 DTK PATH            ***
echo ***                                                       ***
echo *************************************************************

for /f "tokens=*" %%F in ('get-rhodes-info.bat --rhodes-path') do set get-rhodes-path=%%F
cd /D %get-rhodes-path%

set wm6sdkPath=C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i
set /P confirm=Confirm Windows Mobile 6 SDK Include Armv4i Path i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i? (Y/N):

if "%confirm%"=="n" set /P wm6sdkPath=Enter Windows Mobile 6 SDK Include Armv4i Path  : 
if "%confirm%"=="N" set /P wm6sdkPath=Enter Windows Mobile 6 SDK Include Armv4i Path  :
if "%confirm%"=="no" set /P wm6sdkPath=Enter Windows Mobile 6 SDK Include Armv4i Path  :
if "%confirm%"=="No" set /P wm6sdkPath=Enter Windows Mobile 6 SDK Include Armv4i Path  :
if "%confirm%"=="NO" set /P wm6sdkPath=Enter Windows Mobile 6 SDK Include Armv4i Path  :

echo Windows Mobile 6 SDK Include Armv4i Path is: %wm6sdkPath%

set wm653dtkPath=C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i
set /P confirm=Confirm Windows Mobile 6.5.3 DTK Include Armv4i Path i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i? (Y/N):

if "%confirm%"=="n" set /P wm653dtkPath=Enter Windows Mobile 6.5.3 DTK Include Armv4i Path  : 
if "%confirm%"=="N" set /P wm653dtkPath=Enter Windows Mobile 6.5.3 DTK Include Armv4i Path  :
if "%confirm%"=="no" set /P wm653dtkPath=Enter Windows Mobile 6.5.3 DTK Include Armv4i Path  :
if "%confirm%"=="No" set /P wm653dtkPath=Enter Windows Mobile 6.5.3 DTK Include Armv4i Path  :
if "%confirm%"=="NO" set /P wm653dtkPath=Enter Windows Mobile 6.5.3 DTK Include Armv4i Path  :

echo Windows Mobile 6.5.3 DTK Include Armv4i Path is: %wm653dtkPath%

rem echo 1. gpsapi.h
rem		a) - rhodes\platform\wm\rhodes\stubs\
rem			Copy gpsapi.h  to the above location from Windows Mobile 6.5.3 DTK 
rem		b) - rhodes\neon\Helium\HeliumForWindows\Implementation\PBCore\PBCore\gpswrapper\
rem			Copy gpsapi.h  to the above location from Windows Mobile 6 SDK 
xcopy /s /y "%wm6sdkPath%\gpsapi.h" "neon\Helium\HeliumForWindows\Implementation\PBCore\PBCore\gpswrapper"
xcopy /s /y "%wm653dtkPath%\gpsapi.h" "platform\wm\rhodes\stubs\"

rem 2.  getdeviceuniqueid.h - copy getdeviceuniqueid.h  to the below location from Windows Mobile 6 SDK (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm6sdkPath%\getdeviceuniqueid.h" "platform\wm\rhodes\stubs"

rem 3. 	connmgr.h - copy connmgr.h  to the below location from Windows Mobile 6 SDK (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm6sdkPath%\connmgr.h" "platform\wm\rhodes\stubs\"
		
rem 4. 	cfgmgrapi.h - copy cfgmgrapi.h  to the below location from Windows Mobile 6 SDK (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm6sdkPath%\cfgmgrapi.h" "platform\wm\rhodes\stubs\"

rem 5. 	bthutil.h - copy bthutil.h  to the below location from Windows Mobile 6 SDK (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm6sdkPath%\bthutil.h" "platform\wm\rhodes\stubs\"

rem 6. 	soundfile.h - copy soundfile.h  to the below location from Windows Mobile 6.5.3 DTK (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm653dtkPath%\soundfile.h" "platform\wm\rhodes\stubs\"
		
rem 7. 	snapi.h - copy snapi.h  to the below location from Windows Mobile 6.5.3 DTK (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm653dtkPath%\snapi.h" "platform\wm\rhodes\stubs\"

rem 8. 	simmgr.h - copy simmgr.h  to the below location from Windows Mobile 6.5.3 DTK (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm653dtkPath%\simmgr.h" "platform\wm\rhodes\stubs\"

rem 9. 	regext.h - copy regext.h  to the below location from Windows Mobile 6 SDK (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm6sdkPath%\regext.h" "platform\wm\rhodes\stubs\"
			
rem 10. bthapi.h - copy bthapi.h  to the below location from Windows Mobile 6.5.3 DTK (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm653dtkPath%\bthapi.h" "platform\wm\rhodes\stubs\"

rem 11. htmlctrl.h - copy htmlctrl.h  to the below location from Windows Mobile 6 SDK (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
xcopy /s /y "%wm6sdkPath%\htmlctrl.h" "platform\wm\rhodes\browser\"