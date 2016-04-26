if "%RHO_PLATFORM%" == "wm" (

cd platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "android" (

cd platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd platform\bb
rake --trace

)

