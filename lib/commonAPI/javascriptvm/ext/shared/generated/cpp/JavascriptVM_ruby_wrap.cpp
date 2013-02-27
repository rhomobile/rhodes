#include "JavascriptVMBase.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JavascriptVM"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_JavascriptVM_Module();



static void string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement( szVal );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    rho_ruby_enum_strary_json(val, string_iter, &res);
}

static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::String, rho::String>& hash = *((rho::Hashtable<rho::String, rho::String>*)(par));
    hash.put( szName, szVal );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::String, rho::String>& res)
{
    rho_ruby_enum_strhash_json(val, hash_eachstr, &res);
}


VALUE rb_s_JavascriptVM_executeScript(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::String arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor2( rho::CJavascriptVMFactoryBase::getJavascriptVMSingletonS(), &rho::IJavascriptVMSingleton::executeScript, arg0,  oRes );

    if ( bUseCallback )
        rho::CJavascriptVMFactoryBase::getJavascriptVMSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CJavascriptVMFactoryBase::getJavascriptVMSingletonS()->executeScript( arg0,  oRes );

    }
    return oRes.toRuby();
}






}