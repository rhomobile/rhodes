#include "SimpleOnlyStaticModule.h"

#include "MethodResultJni.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleOnlyStaticModuleRUBY"

#include "ext/rho/rhoruby.h"
#include "rhodes/JNIRhoRuby.h"

typedef rho::examples::CSimpleOnlyStaticModuleProxy<ArgumentsAdapter<std::vector<VALUE> > > ObjectProxy;

using namespace rhoelements;
extern "C"
{

VALUE getRuby_SimpleOnlyStaticModule_Module();




VALUE rb_s_SimpleOnlyStaticModule_calcSumm(int argc, VALUE *argv)
{
    RAWTRACE(__FUNCTION__);
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toRuby();
    }


    if((argc < 2) || (argc > 4))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toRuby();
    }
    
    unsigned realParamCount = (argc < 2) ? argc : 2;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    if(argc > 2)
    {
        if (rho_ruby_is_proc(argv[2]) || rho_ruby_is_method(argv[2]))
        {
            result.setRubyProcCallBack(argv[2]);
        } else
        {
            if(argc > 3)
                result.setCallBack(argv[2], argv[3]);
            else
                result.setCallBack(argv[2]);
    
        }
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toRuby();
        }
    }

    ObjectProxy::calcSumm(argumentsAdapter(arguments), result); 
    VALUE res = 
        result.toRuby();

    RAWTRACE(__FUNCTION__);
    return res;
}

VALUE rb_s_SimpleOnlyStaticModule_joinStrings(int argc, VALUE *argv)
{
    RAWTRACE(__FUNCTION__);
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toRuby();
    }


    if((argc < 1) || (argc > 3))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toRuby();
    }
    
    unsigned realParamCount = (argc < 1) ? argc : 1;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    if(argc > 1)
    {
        if (rho_ruby_is_proc(argv[1]) || rho_ruby_is_method(argv[1]))
        {
            result.setRubyProcCallBack(argv[1]);
        } else
        {
            if(argc > 2)
                result.setCallBack(argv[1], argv[2]);
            else
                result.setCallBack(argv[1]);
    
        }
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toRuby();
        }
    }

    ObjectProxy::joinStrings(argumentsAdapter(arguments), result); 
    VALUE res = 
        result.toRuby();

    RAWTRACE(__FUNCTION__);
    return res;
}

VALUE rb_s_SimpleOnlyStaticModule_getPlatform(int argc, VALUE *argv)
{
    RAWTRACE(__FUNCTION__);
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toRuby();
    }


    if((argc < 0) || (argc > 0))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toRuby();
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
    
        }
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toRuby();
        }
    }

    ObjectProxy::getPlatform(argumentsAdapter(arguments), result); 
    VALUE res = 
        result.toRuby();

    RAWTRACE(__FUNCTION__);
    return res;
}

VALUE rb_s_SimpleOnlyStaticModule_showAlertFromUIThread(int argc, VALUE *argv)
{
    RAWTRACE(__FUNCTION__);
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toRuby();
    }


    if((argc < 0) || (argc > 0))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toRuby();
    }
    
    unsigned realParamCount = (argc < 0) ? argc : 0;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    if(argc > 0)
    {
        if (rho_ruby_is_proc(argv[0]) || rho_ruby_is_method(argv[0]))
        {
            result.setRubyProcCallBack(argv[0]);
        } else
        {
            if(argc > 1)
                result.setCallBack(argv[0], argv[1]);
            else
                result.setCallBack(argv[0]);
    
        }
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toRuby();
        }
    }

    ObjectProxy::showAlertFromUIThread(argumentsAdapter(arguments), result); 
    VALUE res = 
        result.toRuby();

    RAWTRACE(__FUNCTION__);
    return res;
}

}
