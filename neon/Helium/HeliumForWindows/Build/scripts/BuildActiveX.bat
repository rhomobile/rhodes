@echo off

echo ***************************
echo ***                     ***
echo ***  BUILDING ACTIVEX   ***
echo ***                     ***
echo ***************************
echo. 

	
set BUILDTYPE=%1
if not "%BUILDTYPE%"=="rebuild" (
	set BUILDTYPE=build
	echo BUILDTYPE is /build
	goto BUILDACTIVEX
	)
echo BUILDTYPE is /rebuild

:BUILDACTIVEX
echo.

REM Build the Plugins
echo BUILDING RhoElements ACTIVEX
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/ActiveX/PocketBrowser/PocketBrowser.sln /project PocketBrowser /out ../errorFiles/PB_PocketBrowser_AX.txt
if not exist "../../Implementation/Bin/PocketBrowser.dll" goto BUILDFAILED
echo.

echo BUILDING NOSIP ACTIVEX
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/ActiveX/PBNoSIP/PBNoSIP.sln /project PBNoSIP.vcproj /out ../errorFiles/PB_NoSIP_AX.txt
if not exist "../../Implementation/Bin/NoSIP.dll" goto BUILDFAILED
echo.

echo BUILDING CEODAX ACTIVEX
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/ActiveX/PBODAX/PBODAX.sln /project CeODAX /out ../errorFiles/PB_CEODAX_AX.txt
if not exist "../../Implementation/Bin/CeODAX.dll" goto BUILDFAILED
echo.


GOTO BUILDSUCCEEDED

:BUILDFAILED

echo *******************************************
echo ***                                     ***
echo ***      ACTIVEX BUILD FAILED!!!!       ***
echo *** CHECK ERROR OUTPUT FOR LAST PLUG-IN ***
echo ***                                     ***
echo *******************************************
pause
exit
GOTO ENDOFFILE

:BUILDSUCCEEDED

echo *******************************************
echo ***                                     ***
echo ***      ACTIVEX BUILD SUCCESS   =)     ***
echo ***                                     ***
echo *******************************************

:ENDOFFILE






