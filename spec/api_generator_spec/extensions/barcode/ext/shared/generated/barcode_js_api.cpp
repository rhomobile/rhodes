#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode"


rho::String js_Barcode_enable(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_start(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_stop(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_disable(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_take(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_getProperty(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_getProperties(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_getAllProperties(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_setProperty(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_setProperties(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_clearProps(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);
rho::String js_Barcode_enumerate(const rho::String& strID, rho::json::CJSONArrayIterator& oParams);

extern "C" void Init_JSAPI_Barcode(void)
{

    js_define_method("Rho:Barcode:enable", js_Barcode_enable);
    js_define_method("Rho:Barcode:start", js_Barcode_start);
    js_define_method("Rho:Barcode:stop", js_Barcode_stop);
    js_define_method("Rho:Barcode:disable", js_Barcode_disable);
    js_define_method("Rho:Barcode:take", js_Barcode_take);
    js_define_method("Rho:Barcode:getProperty", js_Barcode_getProperty);
    js_define_method("Rho:Barcode:getProperties", js_Barcode_getProperties);
    js_define_method("Rho:Barcode:getAllProperties", js_Barcode_getAllProperties);
    js_define_method("Rho:Barcode:setProperty", js_Barcode_setProperty);
    js_define_method("Rho:Barcode:setProperties", js_Barcode_setProperties);
    js_define_method("Rho:Barcode:clearProps", js_Barcode_clearProps);
    js_define_method("Rho:Barcode:enumerate", js_Barcode_enumerate);
}

