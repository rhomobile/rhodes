@echo off

echo ***************************
echo ***                     ***
echo ***  BUILDING NPAPI     ***
echo ***                     ***
echo ***************************
echo. 

	
set BUILDTYPE=%1
if not "%BUILDTYPE%"=="rebuild" (
	set BUILDTYPE=build
	echo BUILDTYPE is /build
	goto BUILDNPAPI
	)
echo BUILDTYPE is /rebuild

:BUILDNPAPI
echo.

REM Build the Plugins
echo BUILDING LEGACY NPAPI
echo. 
devenv /nologo /%BUILDTYPE% Release ../../Implementation/NPAPI/WTG_Legacy/wtg_legacy.sln /project wtg_legacy /out ../errorFiles/PB_Legacy_NPAPI.txt
if not exist "../../Implementation/Bin/npwtg_legacy.dll" goto BUILDFAILED
echo.


echo BUILDING JSOBJECTS NPAPI
echo.
devenv /nologo /%BUILDTYPE% Release ../../../NPAPI/wtg_jsobjects.sln /project wtg_jsobjects /out ../errorFiles/PB_JSObjects_NPAPI.txt
if not exist "../../Implementation/Bin/npwtg_jsobjects.dll" goto BUILDFAILED
echo.



GOTO BUILDSUCCEEDED

:BUILDFAILED

echo *******************************************
echo ***                                     ***
echo ***       NPAPI BUILD FAILED!!!!        ***
echo *** CHECK ERROR OUTPUT FOR LAST NPAPI   ***
echo ***                                     ***
echo *******************************************
pause
exit
GOTO ENDOFFILE

:BUILDSUCCEEDED

echo *******************************************
echo ***                                     ***
echo ***       NPAPI BUILD SUCCESS   :P      ***
echo ***                                     ***
echo *******************************************

:ENDOFFILE






