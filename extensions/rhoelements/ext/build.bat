
if "%RHO_PLATFORM%" == "android" (

cd android
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd wm
rake --trace

)
