
if "%RHO_PLATFORM%" == "android" (

cd platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

call %RHO_VSCMNTOOLS%
cd platform\qt
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd platform\bb
rake --trace

)

if "%RHO_PLATFORM%" == "wp8" (

cd platform\wp8
rake --trace

)

if "%RHO_PLATFORM%" == "uwp" (

cd platform\uwp
rake --trace

)

