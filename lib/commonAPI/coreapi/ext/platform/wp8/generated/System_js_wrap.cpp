#include "ISystem.h"
#include "api_generator\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;


rho::String js_System_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(strObjID));



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::getProperty, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProperty( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(strObjID));



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toJSON();
    }
    




    rho::Vector<rho::StringW> arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isArray() )
        {
            CJSONArray arParam(argv[0]);
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg0.addElement( convertToStringW(arParam[i].getString()) );
            }
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"array" );
            return oRes.toJSON();
        }
    }



        




    if ( argc > nCallbackArg )
    {

        
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::getProperties, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProperties( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(strObjID));




    if ( argc > nCallbackArg )
    {

        
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getAllProperties,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getAllProperties(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(strObjID));



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
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





        



    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toJSON();
    }
    


    rho::StringW arg1;
    if ( argc > 1 )
    {
        if ( argv[1].isString() )
        {
            arg1 = convertToStringW(argv[1].getString());

        }
        else if (!argv[1].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"string" );
            return oRes.toJSON();
        }
    }





        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &ISystem::setProperty, arg0, arg1,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setProperty( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(strObjID));



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toJSON();
    }
    






    rho::Hashtable<rho::StringW, rho::StringW> arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isObject() )
        {
            CJSONStructIterator objIter(argv[0]);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg0[convertToStringW(objIter.getCurKey())] = convertToStringW(objIter.getCurString());
            }
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"hash" );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setProperties, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setProperties( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(strObjID));




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::clearAllProperties,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->clearAllProperties(  oRes );


    }


    return oRes.toJSON();

}




rho::String js_s_System_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    oRes.set(strDefaultID);

    return oRes.toJSON();
}

rho::String js_s_System_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    CMethodResult oRes;
    CSystemFactoryBase::getSystemSingletonS()->setDefaultID(convertToStringW(strObjID));

    return oRes.toJSON();
}


