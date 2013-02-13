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



extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::StringW>& ar = *((rho::Vector<rho::StringW>*)(par));
    ar.addElement( convertToStringW(szVal) );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::StringW>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

extern "C" static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::StringW, rho::StringW>& hash = *((rho::Hashtable<rho::StringW, rho::StringW>*)(par));
    hash.put( convertToStringW(szName), convertToStringW(szVal) );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::StringW, rho::StringW>& res)
{
    rho_ruby_enum_strhash(val, hash_eachstr, &res);
}



VALUE rb_s_JavascriptVM_executeScript(int argc, VALUE *argv)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
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