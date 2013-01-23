#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_Barcode();
extern "C" void Init_JSAPI_Barcode();

void Init_Barcode_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_Barcode();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_Barcode();
#endif
}
