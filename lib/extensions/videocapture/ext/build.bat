
if "%RHO_PLATFORM%" == "android" (

cd videocapture\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd videocapture\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd videocapture\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

cd videocapture\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd videocapture\platform\bb
rake --trace

)

