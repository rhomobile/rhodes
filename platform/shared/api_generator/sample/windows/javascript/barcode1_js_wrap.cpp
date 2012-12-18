#include "..\Barcode1.h"
#include "..\..\..\Common\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

using namespace rho::json;

rho::String js_barcode1_enumerate(const rho::String& strID, CJSONEntry& oParams)
{
    return "{}";
}

rho::String js_barcode1_getProps(const rho::String& strID, CJSONEntry& oParams)
{
    return "{}";
}
