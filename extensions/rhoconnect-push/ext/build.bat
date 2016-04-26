
if "%RHO_PLATFORM%" == "android" (

cd rhoconnect-push\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd rhoconnect-push\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd rhoconnect-push\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

call "%RHO_VSCMNTOOLS%..\..\VC\vcvarsall.bat" x86
cd rhoconnect-push\platform\qt
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd rhoconnect-push\platform\bb
rake --trace

)

