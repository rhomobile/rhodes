@echo off
for /f "tokens=*" %%F in ('get-rhodes-info.bat --rhodes-path') do set get-rhodes-path=%%F
cd /D %get-rhodes-path%

set PATH=c:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE;%PATH%
set PATH=%get-rhodes-path%\res\build-tools;%PATH%

set VERSION_STR=4.0.0.0.0
set RESOURCE_VER=%VERSION_STR:.=,%

call bin\ms_include.bat

call neon\Helium\HeliumForWindows\Implementation\ActiveX\BuildRCFiles.bat %RESOURCE_VER%
call neon\Helium\HeliumForWindows\Implementation\ActiveX\Build_ActiveX.bat
call neon\Helium\HeliumForWindows\Implementation\BuildPlugins %RESOURCE_VER% 

echo Fill rhodes\libs\data directory
setlocal
cd /D %get-rhodes-path%
call rake build:set_neon[%CD%\neon] --trace
call rake wm_gem:build:wm:copy_webkit --trace
call rake wm_gem:build:printing_service --trace
endlocal
