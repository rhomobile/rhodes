#include "Megamodule.h"

#include "MethodResultJni.h"
#include "api_generator/MethodResult.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MegamoduleJS"

#include "rhodes/JNIRhoJS.h"

typedef rho::examples::CMegamoduleProxy<ArgumentsAdapter<rho::json::CJSONArray> > ObjectProxy;

using namespace rhoelements;

rho::String js_s_Megamodule_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE(__FUNCTION__);
    
    rho::apiGenerator::CMethodResult result;
    result.set(ObjectProxy::getDefaultID());
    return result.toJSON();
}

rho::String js_s_Megamodule_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE(__FUNCTION__);

    rho::apiGenerator::CMethodResult result;
    ObjectProxy::setDefaultID(strObjID);
    return result.toJSON();
} 


rho::String js_Megamodule_getStringProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.getStringProperty(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_getIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.getIntegerProperty(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_setIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.setIntegerProperty(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_typesTest(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

    int argc = argv.getSize();
    if((argc < 6) || (argc > 6))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toJson();
    }
    
    if(argc > 6)
    {
        if(argc > 7)
            result.setCallBack(argv[6], argv[7]);
        else
            result.setCallBack(argv[6]);
    
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toJson();
        }
    }

    megamodule.typesTest(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_produceArray(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.produceArray(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_produceHash(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.produceHash(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_produceComplicatedResult(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.produceComplicatedResult(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_s_Megamodule_getObjectsCount(rho::json::CJSONArray& argv, const rho::String& strObjID)
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

    ObjectProxy::getObjectsCount(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_s_Megamodule_getObjectByIndex(rho::json::CJSONArray& argv, const rho::String& strObjID)
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

    ObjectProxy::getObjectByIndex(argumentsAdapter(argv), result); 
    rho::String res = 
        result.enumerateJSObjects();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.showAlertFromUIThread(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_setPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

    int argc = argv.getSize();
    if((argc < 2) || (argc > 3))
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

    megamodule.setPeriodicallyCallback(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_isPeriodicallyCallbackSetted(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

    int argc = argv.getSize();
    if((argc < 0) || (argc > 2))
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

    megamodule.isPeriodicallyCallbackSetted(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_stopPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.stopPeriodicallyCallback(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_complicatedTypesTest1(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.complicatedTypesTest1(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.getProperty(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.getProperties(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

    int argc = argv.getSize();
    if((argc < 0) || (argc > 2))
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

    megamodule.getAllProperties(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

    int argc = argv.getSize();
    if((argc < 2) || (argc > 2))
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

    megamodule.setProperty(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.setProperties(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_Megamodule_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }

    ObjectProxy megamodule(strObjID);

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

    megamodule.clearAllProperties(argumentsAdapter(argv), result); 
    rho::String res = 
        result.toJson();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
rho::String js_s_Megamodule_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    if((argc < 0) || (argc > 2))
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

    ObjectProxy::enumerate(argumentsAdapter(argv), result); 
    rho::String res = 
        result.enumerateJSObjects();

    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}
