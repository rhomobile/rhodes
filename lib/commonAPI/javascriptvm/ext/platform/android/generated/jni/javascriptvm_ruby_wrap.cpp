#include "JavascriptVM.h"

#include "MethodResultJni.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JavascriptVMRUBY"

#include "ext/rho/rhoruby.h"
#include "rhodes/JNIRhoRuby.h"

typedef rho::CJavascriptVMProxy<ArgumentsAdapter<std::vector<VALUE> > > ObjectProxy;

using namespace rhoelements;
extern "C"
{

VALUE getRuby_JavascriptVM_Module();




VALUE rb_s_JavascriptVM_executeScript(int argc, VALUE *argv)
{
    RAWTRACE(__FUNCTION__);
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toRuby();
    }


    if((argc < 1) || (argc > 1))
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

    ObjectProxy::executeScript(argumentsAdapter(arguments), result); 
    VALUE res = 
        result.toRuby();

    RAWTRACE(__FUNCTION__);
    return res;
}

}
