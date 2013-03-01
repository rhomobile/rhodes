#include "SimpleOnlyStaticModuleBase.h"
#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleOnlyStaticModule"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;


rho::String js_s_SimpleOnlyStaticModule_calcSumm(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toJSON();
    }
    




    int arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toJSON();
        }
    }















    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toJSON();
    }
    




    int arg1;
    if ( argc > 1 )
    {
        if ( argv[1].isString() )
            arg1 = argv[1].getInt();
        else if (!argv[1].isNull())
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toJSON();
        }
    }
















    if ( argc > nCallbackArg )
    {


        if ( !argv[nCallbackArg].isString() )
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toJSON();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( argv[nCallbackArg].getString() );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !argv[nCallbackArg + 1].isString() )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toJSON();
            }

            oRes.setCallbackParam( argv[nCallbackArg + 1].getString() );
        }

    }


    pFunctor = rho_makeInstanceClassFunctor3( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::calcSumm, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->calcSumm( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_s_SimpleOnlyStaticModule_joinStrings(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    










    rho::Vector<rho::String> arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isArray() )
        {
            CJSONArray arParam(argv[0]);
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg0.addElement( arParam[i].getString() );
            }
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "array" );
            return oRes.toJSON();
        }
    }










    if ( argc > nCallbackArg )
    {


        if ( !argv[nCallbackArg].isString() )
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toJSON();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( argv[nCallbackArg].getString() );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !argv[nCallbackArg + 1].isString() )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toJSON();
            }

            oRes.setCallbackParam( argv[nCallbackArg + 1].getString() );
        }

    }


    pFunctor = rho_makeInstanceClassFunctor2( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::joinStrings, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->joinStrings( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_s_SimpleOnlyStaticModule_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toJSON();


        if ( !argv[nCallbackArg].isString() )
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toJSON();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( argv[nCallbackArg].getString() );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !argv[nCallbackArg + 1].isString() )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toJSON();
            }

            oRes.setCallbackParam( argv[nCallbackArg + 1].getString() );
        }

    }


    pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::getPlatform,  oRes );




    if ( bUseCallback )
        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS()->getPlatform(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_s_SimpleOnlyStaticModule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toJSON();


        if ( !argv[nCallbackArg].isString() )
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toJSON();
        }

        oRes.setCallInUIThread(true);
        oRes.setRubyCallback( argv[nCallbackArg].getString() );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !argv[nCallbackArg + 1].isString() )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toJSON();
            }

            oRes.setCallbackParam( argv[nCallbackArg + 1].getString() );
        }

    }


    pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CSimpleOnlyStaticModuleFactoryBase::getSimpleOnlyStaticModuleSingletonS(), &rho::examples::ISimpleOnlyStaticModuleSingleton::showAlertFromUIThread,  oRes );



    rho_wm_impl_performOnUiThread( pFunctor );


    return oRes.toJSON();

}




