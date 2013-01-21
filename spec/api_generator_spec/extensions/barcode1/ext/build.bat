
if "%RHO_PLATFORM%" == "android" (

cd barcode1\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd barcode1\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd barcode1\platform\bb
rake --trace

)

