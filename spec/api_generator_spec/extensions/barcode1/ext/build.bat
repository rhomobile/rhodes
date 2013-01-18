
if "%RHO_PLATFORM%" == "android" (

cd barcode1\platform\android
rake --trace

)

if "%RHO_PLATFORM%" == "iphone" (

cd barcode1\platform\phone
rake --trace

)

if "%RHO_PLATFORM%" == "wm" (

cd barcode1\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "win32" (

cd barcode1\platform\wm
rake --trace

)

if "%RHO_PLATFORM%" == "wp8" (

cd barcode1\platform\wp8
rake --trace

)

if "%RHO_PLATFORM%" == "bb" (

cd barcode1\platform\bb
rake --trace

)

