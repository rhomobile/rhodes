#include "Barcode1.h"
#include "MethodResultJni.h"
#include "js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1JS"

#include "common/StringConverter.h"

#include "ext/rho/rhoruby.h"
#include "rhodes/JNIRhoRuby.h"

using namespace rho;
using namespace rho::json;
using namespace rho::common;
using namespace rhoelements;

rho::String js_barcode1_enumerate(const rho::String& strID, CJSONArrayIterator& oParams)
{
    RAWTRACE("js_barcode1_enumerate");

    MethodResultJni result;
    CBarcode1::enumerate(result);
    return result.enumerateJSObjects();
}

rho::String js_barcode1_getProps(const rho::String& strID, CJSONArrayIterator& oParams)
{
    rho::String id = strID;
    if (id.length() == 0)
         id = CBarcode1::getDefaultID();

    MethodResultJni result;
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return result.toJson();
    }

    CBarcode1 barcode(id);

    if(oParams.isEnd())
    {
        barcode.getProps(result);
        return result.toJson();
    }

//    CJSONEntry oParam1 = oParams.getCurItem();
//    oParams.next();
//
//    if(oParams.isEnd())
//    {
//        if(oParam1.isNull())
//        {
//            barcode.getProps(result);
//        }
//        else if(oParam1.isString())
//        {
//            jhstring jhName = rho_cast<jstring>(oParam1.getString());
//            barcode.getProps(jhName.get(), result);
//        }
//        else if(oParam1.isArray())
//        {
//            rho::Vector<rho::String> ar;
//            CJSONArrayIterator arParam(oParam1);
//            for( ; !arParam.isEnd(); arParam.next())
//            {
//                ar.addElement(arParam.getCurItem().getString());
//            }
//
//            jhobject jhNames = rho_cast<jobject>(ar);
//            barcode.getProps(jhNames.get(), result);
//        }else
//        {
//            result.setArgError(L"Type error: argument 1 should be nil, String or Array"); //see SWIG Ruby_Format_TypeError
//        }
//    }else
//    {
//        CJSONEntry oParam2 = oParams.getCurItem();
//
//        oParams.next();
//        if ( !oParams.isEnd() )
//        {
//            CJSONEntry oParam3 = oParams.getCurItem();
//            if ( !oParam3.isString() )
//            {
//                result.setArgError(L"Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
//                return oRes.toJSON();
//            }
//            result.setCallBack(oParam2.getString(), oParam3.getString());
//        }
//        else
//        {
//            result.setCallBack(oParam2.getString(), 0);
//        }
//        if(oParam1.isNull())
//        {
//            barcode.getProps(result);
//        }
//        else if(oParam1.isString())
//        {
//            jhstring jhName = rho_cast<jstring>(oParam1.getString());
//            barcode.getProps(jhName.get(), result);
//        }
//        else if(oParam1.isArray())
//        {
//            rho::Vector<rho::String> ar;
//            CJSONArrayIterator arParam(oParam1);
//            for( ; !arParam.isEnd(); arParam.next())
//            {
//                ar.addElement(arParam.getCurItem().getString());
//            }
//
//            jhobject jhNames = rho_cast<jobject>(ar);
//            barcode.getProps(jhNames.get(), result);
//        }else
//        {
//            result.setArgError(L"Type error: argument 1 should be nil, String or Array"); //see SWIG Ruby_Format_TypeError
//        }
//    }

    return result.toJson();
}
