
if "%RHO_PLATFORM%" == "android" (

cd rawsensors\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd rawsensors\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd rawsensors\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

cd rawsensors\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd rawsensors\platform\bb
rake --trace

)

