
if "%RHO_PLATFORM%" == "android" (

cd platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

call "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" x86
cd platform\osx
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd platform\bb
rake --trace

)

