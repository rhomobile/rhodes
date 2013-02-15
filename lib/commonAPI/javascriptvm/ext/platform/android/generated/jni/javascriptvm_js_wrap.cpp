#include "JavascriptVM.h"

#include "MethodResultJni.h"
#include "api_generator/MethodResult.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JavascriptVMJS"

#include "rhodes/JNIRhoJS.h"

typedef rho::CJavascriptVMProxy<ArgumentsAdapter<rho::json::CJSONArray> > ObjectProxy;

using namespace rhoelements;



rho::String js_s_JavascriptVM_executeScript(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }


    int argc = argv.getSize();
    if((argc < 1) || (argc > 1))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toJson();
    }
    
    if(argc > 1)
    {
        if(argc > 2)
            result.setCallBack(argv[1], argv[2]);
        else
            result.setCallBack(argv[1]);
    
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toJson();
        }
    }

    ObjectProxy::executeScript(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
