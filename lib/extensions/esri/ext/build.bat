
if "%RHO_PLATFORM%" == "android" (

cd esri\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd esri\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd esri\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd esri\platform\bb
rake --trace

)

