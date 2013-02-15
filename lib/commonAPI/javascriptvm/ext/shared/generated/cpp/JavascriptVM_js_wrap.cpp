#include "JavascriptVMBase.h"
#include "api_generator\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JavascriptVM"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;


rho::String js_s_JavascriptVM_executeScript(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toJSON();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = convertToStringW(argv[0].getString());

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toJSON();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toJSON();

        
        if ( !argv[nCallbackArg].isString() )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toJSON();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( argv[nCallbackArg].getString() );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !argv[nCallbackArg + 1].isString() )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toJSON();
            }

            oRes.setCallbackParam( argv[nCallbackArg + 1].getString() );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor2( rho::CJavascriptVMFactoryBase::getJavascriptVMSingletonS(), &rho::IJavascriptVMSingleton::executeScript, arg0,  oRes );




    if ( bUseCallback )
        rho::CJavascriptVMFactoryBase::getJavascriptVMSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        rho::CJavascriptVMFactoryBase::getJavascriptVMSingletonS()->executeScript( arg0,  oRes );


    }


    return oRes.toJSON();

}




