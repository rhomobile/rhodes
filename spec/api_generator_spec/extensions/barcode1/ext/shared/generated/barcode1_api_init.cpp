#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_Barcode1();
extern "C" void Init_JSAPI_Barcode1();

extern "C" void Init_Barcode1_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_Barcode1();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_Barcode1();
#endif
}
