#include "SimpleOnlyStaticModuleBase.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleOnlyStaticModule"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_SimpleOnlyStaticModule_Module();



extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement( szVal );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

extern "C" static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::String, rho::String>& hash = *((rho::Hashtable<rho::String, rho::String>*)(par));
    hash.put( szName, szVal );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::String, rho::String>& res)
{
    rho_ruby_enum_strhash(val, hash_eachstr, &res);
}


VALUE rb_s_SimpleOnlyStaticModule_calcSumm(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    int arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    pFunctor = rho_makeInstanceClassFunctor3( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::calcSumm, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->calcSumm( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_SimpleOnlyStaticModule_joinStrings(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::Vector<rho::String> arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_array(argv[0]) )
            getStringArrayFromValue(argv[0], arg0);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "array" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    pFunctor = rho_makeInstanceClassFunctor2( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::joinStrings, arg0,  oRes );

    if ( bUseCallback )
        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->joinStrings( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_SimpleOnlyStaticModule_getPlatform(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::getPlatform,  oRes );

    if ( bUseCallback )
        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->getPlatform(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_SimpleOnlyStaticModule_showAlertFromUIThread(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::showAlertFromUIThread,  oRes );
    rho_wm_impl_performOnUiThread( pFunctor );
    return oRes.toRuby();
}






}