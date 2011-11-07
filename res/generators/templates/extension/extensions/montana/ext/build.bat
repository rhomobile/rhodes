
if "%RHO_PLATFORM%" == "android" (

cd <%= name.downcase %>\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd <%= name.downcase %>\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd <%= name.downcase %>\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

cd <%= name.downcase %>\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd <%= name.downcase %>\platform\bb
rake --trace

)

