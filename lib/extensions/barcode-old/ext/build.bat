
if "%RHO_PLATFORM%" == "android" (

cd barcode\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd barcode\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd barcode\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd barcode\platform\bb
rake --trace

)

