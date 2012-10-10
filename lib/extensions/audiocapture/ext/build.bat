
if "%RHO_PLATFORM%" == "android" (

cd audiocapture\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd audiocapture\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd audiocapture\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

cd audiocapture\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd audiocapture\platform\bb
rake --trace

)

