#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode"


rho::String js_Barcode_enable(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_start(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_stop(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_disable(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_take(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_clearProps(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID);


rho::String js_Barcode_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_Barcode_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);



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


    js_define_method("Rho:Barcode:getDefaultID", js_Barcode_getDefaultID);
    js_define_method("Rho:Barcode:setDefaultID", js_Barcode_setDefaultID);

}

