#include "Megamodule.h"

#include "MethodResultJni.h"

#include "ext/rho/rhoruby.h"
#include "rhodes/JNIRhoRuby.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MegamoduleRUBY"


typedef rho::examples::CMegamoduleProxy<ArgumentsAdapter<std::vector<VALUE> > > ObjectProxy;

using namespace rho::apiGenerator;
extern "C"
{

VALUE getRuby_Megamodule_Module();


VALUE rb_Megamodule_s_default(VALUE klass)
{
    RAWTRACE("rb_barcode1_s_default");
    return rho_ruby_create_object_with_id(klass, rho::examples::CMegamoduleBase::getDefaultID().c_str());
}

VALUE rb_Megamodule_s_setDefault(VALUE klass, VALUE valObj)
{
    RAWTRACE("rb_barcode1_s_set_default");

    const char* szID = rho_ruby_get_object_id(valObj);
    rho::examples::CMegamoduleBase::setDefaultID(szID);

    return rho_ruby_get_NIL();
} 


static VALUE Megamodule_getStringProperty(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.getStringProperty(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_getStringProperty(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_getStringProperty(argc, argv, id);
}

VALUE rb_s_Megamodule_def_getStringProperty(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_getStringProperty(argc, argv, id);
}
static VALUE Megamodule_getIntegerProperty(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.getIntegerProperty(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_getIntegerProperty(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_getIntegerProperty(argc, argv, id);
}

VALUE rb_s_Megamodule_def_getIntegerProperty(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_getIntegerProperty(argc, argv, id);
}
static VALUE Megamodule_setIntegerProperty(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 1))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.setIntegerProperty(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_setIntegerProperty(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_setIntegerProperty(argc, argv, id);
}

VALUE rb_s_Megamodule_def_setIntegerProperty(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_setIntegerProperty(argc, argv, id);
}
static VALUE Megamodule_typesTest(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 6) || (argc > 6))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 6) ? argc : 6;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 6)
    {
        if (rho_ruby_is_proc(argv[6]) || rho_ruby_is_method(argv[6]))
        {
            result.setRubyProcCallBack(argv[6]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 7)
                result.setCallBack(argv[6], argv[7]);
            else
                result.setCallBack(argv[6]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.typesTest(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_typesTest(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_typesTest(argc, argv, id);
}

VALUE rb_s_Megamodule_def_typesTest(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_typesTest(argc, argv, id);
}
static VALUE Megamodule_produceArray(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.produceArray(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_produceArray(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_produceArray(argc, argv, id);
}

VALUE rb_s_Megamodule_def_produceArray(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_produceArray(argc, argv, id);
}
static VALUE Megamodule_produceHash(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.produceHash(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_produceHash(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_produceHash(argc, argv, id);
}

VALUE rb_s_Megamodule_def_produceHash(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_produceHash(argc, argv, id);
}
static VALUE Megamodule_produceComplicatedResult(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.produceComplicatedResult(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_produceComplicatedResult(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_produceComplicatedResult(argc, argv, id);
}

VALUE rb_s_Megamodule_def_produceComplicatedResult(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_produceComplicatedResult(argc, argv, id);
}
VALUE rb_s_Megamodule_getObjectsCount(int argc, VALUE *argv)
{
    RAWTRACE2("%s - argc: %d", __FUNCTION__, argc);
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");


    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    ObjectProxy::getObjectsCount(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}

VALUE rb_s_Megamodule_getObjectByIndex(int argc, VALUE *argv)
{
    RAWTRACE2("%s - argc: %d", __FUNCTION__, argc);
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");


    if((argc < 1) || (argc > 1))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    result.setObjectClassPath("Rho::Examples::Megamodule");
    RAWTRACE("Object class path is set");

    ObjectProxy::getObjectByIndex(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}

static VALUE Megamodule_showAlertFromUIThread(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 1) || (argc > 1))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.showAlertFromUIThread(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_showAlertFromUIThread(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_showAlertFromUIThread(argc, argv, id);
}

VALUE rb_s_Megamodule_def_showAlertFromUIThread(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_showAlertFromUIThread(argc, argv, id);
}
static VALUE Megamodule_setPeriodicallyCallback(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 2) || (argc > 3))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.setPeriodicallyCallback(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_setPeriodicallyCallback(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_setPeriodicallyCallback(argc, argv, id);
}

VALUE rb_s_Megamodule_def_setPeriodicallyCallback(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_setPeriodicallyCallback(argc, argv, id);
}
static VALUE Megamodule_isPeriodicallyCallbackSetted(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 2))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.isPeriodicallyCallbackSetted(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_isPeriodicallyCallbackSetted(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_isPeriodicallyCallbackSetted(argc, argv, id);
}

VALUE rb_s_Megamodule_def_isPeriodicallyCallbackSetted(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_isPeriodicallyCallbackSetted(argc, argv, id);
}
static VALUE Megamodule_stopPeriodicallyCallback(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.stopPeriodicallyCallback(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_stopPeriodicallyCallback(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_stopPeriodicallyCallback(argc, argv, id);
}

VALUE rb_s_Megamodule_def_stopPeriodicallyCallback(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_stopPeriodicallyCallback(argc, argv, id);
}
static VALUE Megamodule_complicatedTypesTest1(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 1) || (argc > 1))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.complicatedTypesTest1(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_complicatedTypesTest1(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_complicatedTypesTest1(argc, argv, id);
}

VALUE rb_s_Megamodule_def_complicatedTypesTest1(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_complicatedTypesTest1(argc, argv, id);
}
static VALUE Megamodule_getProperty(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 1) || (argc > 3))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.getProperty(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_getProperty(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_getProperty(argc, argv, id);
}

VALUE rb_s_Megamodule_def_getProperty(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_getProperty(argc, argv, id);
}
static VALUE Megamodule_getProperties(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 1) || (argc > 3))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.getProperties(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_getProperties(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_getProperties(argc, argv, id);
}

VALUE rb_s_Megamodule_def_getProperties(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_getProperties(argc, argv, id);
}
static VALUE Megamodule_getAllProperties(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 2))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.getAllProperties(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_getAllProperties(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_getAllProperties(argc, argv, id);
}

VALUE rb_s_Megamodule_def_getAllProperties(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_getAllProperties(argc, argv, id);
}
static VALUE Megamodule_setProperty(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 2) || (argc > 2))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 2) ? argc : 2;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 2)
    {
        if (rho_ruby_is_proc(argv[2]) || rho_ruby_is_method(argv[2]))
        {
            result.setRubyProcCallBack(argv[2]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 3)
                result.setCallBack(argv[2], argv[3]);
            else
                result.setCallBack(argv[2]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.setProperty(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_setProperty(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_setProperty(argc, argv, id);
}

VALUE rb_s_Megamodule_def_setProperty(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_setProperty(argc, argv, id);
}
static VALUE Megamodule_setProperties(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 1) || (argc > 1))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.setProperties(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_setProperties(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_setProperties(argc, argv, id);
}

VALUE rb_s_Megamodule_def_setProperties(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_setProperties(argc, argv, id);
}
static VALUE Megamodule_clearAllProperties(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");

    ObjectProxy megamodule(id);

    if((argc < 0) || (argc > 0))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    megamodule.clearAllProperties(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}
VALUE rb_Megamodule_clearAllProperties(int argc, VALUE *argv, VALUE obj)
{
    rho::String id = rho_ruby_get_object_id(obj);
    return Megamodule_clearAllProperties(argc, argv, id);
}

VALUE rb_s_Megamodule_def_clearAllProperties(int argc, VALUE *argv)
{
    rho::String id = ObjectProxy::getDefaultID();
    return Megamodule_clearAllProperties(argc, argv, id);
}
VALUE rb_s_Megamodule_enumerate(int argc, VALUE *argv)
{
    RAWTRACE2("%s - argc: %d", __FUNCTION__, argc);
    MethodResultJni result(true);
    if(!result)
    {
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        result.setError("JNI error: failed to initialize MethodResult java object");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    RAWTRACE("MethodResultJni instance is created");


    if((argc < 0) || (argc > 2))
    {
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        result.setArgError("Wrong number of arguments");
        return CMethodResultConvertor().toRuby(result, false);
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    
    RAWTRACE1("Count of passed arguments: %d", arguments.size());
    
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
            RAWTRACE("Ruby proc callback is set");
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
            
            RAWTRACE("Callback URL is set");
        }
        if(!result.hasCallback())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return CMethodResultConvertor().toRuby(result, false);
        }
    }

    result.setObjectClassPath("Rho::Examples::Megamodule");
    RAWTRACE("Object class path is set");

    ObjectProxy::enumerate(argumentsAdapter(arguments), result); 
    RAWTRACE("Native metod has invoked, converting result");
    VALUE res = CMethodResultConvertor().toRuby(result, false);
    RAWTRACE(__FUNCTION__);
    return res;
}

}
