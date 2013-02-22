#include "SimpleOnlyStaticModule.h"

#include "MethodResultJni.h"
#include "api_generator/MethodResult.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleOnlyStaticModuleJS"

#include "rhodes/JNIRhoJS.h"

typedef rho::examples::CSimpleOnlyStaticModuleProxy<ArgumentsAdapter<rho::json::CJSONArray> > ObjectProxy;

using namespace rhoelements;



rho::String js_s_SimpleOnlyStaticModule_calcSumm(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    if((argc < 2) || (argc > 4))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toJson();
    }
    
    if(argc > 2)
    {
        if(argc > 3)
            result.setCallBack(argv[2], argv[3]);
        else
            result.setCallBack(argv[2]);
    
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toJson();
        }
    }

    ObjectProxy::calcSumm(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_s_SimpleOnlyStaticModule_joinStrings(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    if((argc < 1) || (argc > 3))
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

    ObjectProxy::joinStrings(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_s_SimpleOnlyStaticModule_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    if((argc < 0) || (argc > 0))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toJson();
    }
    
    if(argc > 0)
    {
        if(argc > 1)
            result.setCallBack(argv[0], argv[1]);
        else
            result.setCallBack(argv[0]);
    
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toJson();
        }
    }

    ObjectProxy::getPlatform(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_s_SimpleOnlyStaticModule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    if((argc < 0) || (argc > 0))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toJson();
    }
    
    if(argc > 0)
    {
        if(argc > 1)
            result.setCallBack(argv[0], argv[1]);
        else
            result.setCallBack(argv[0]);
    
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toJson();
        }
    }

    ObjectProxy::showAlertFromUIThread(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
