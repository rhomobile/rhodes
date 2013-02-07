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


rho::String js_System_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPlatform,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPlatform(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getHasCamera(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getHasCamera,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHasCamera(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getScreenWidth,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenWidth(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getScreenHeight,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenHeight(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getRealScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getRealScreenWidth,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getRealScreenWidth(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getRealScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getRealScreenHeight,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getRealScreenHeight(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getScreenOrientation(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getScreenOrientation,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenOrientation(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getPpiX(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPpiX,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPpiX(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getPpiY(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPpiY,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPpiY(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getPhoneNumber(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPhoneNumber,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPhoneNumber(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getDeviceOwnerEmail(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceOwnerEmail,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceOwnerEmail(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getDeviceOwnerName(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceOwnerName,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceOwnerName(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getDeviceId(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceId,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceId(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getPhoneId(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPhoneId,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPhoneId(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getDeviceName(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceName,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceName(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getOsVersion(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getOsVersion,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getOsVersion(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getLocale(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getLocale,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getLocale(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getCountry(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getCountry,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getCountry(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getIsEmulator(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getIsEmulator,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getIsEmulator(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getHasCalendar(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getHasCalendar,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHasCalendar(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getIsMotorolaDevice(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getIsMotorolaDevice,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getIsMotorolaDevice(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getOemInfo(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getOemInfo,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getOemInfo(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getUuid(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getUuid,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getUuid(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getApplicationIconBadge,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getApplicationIconBadge(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    




    int arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setApplicationIconBadge, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setApplicationIconBadge( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getHttpProxyURI,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHttpProxyURI(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setHttpProxyURI, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setHttpProxyURI( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getLockWindowSize,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getLockWindowSize(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt() ? true : false;
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setLockWindowSize, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setLockWindowSize( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getShowKeyboard,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getShowKeyboard(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt() ? true : false;
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setShowKeyboard, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setShowKeyboard( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::applicationInstall, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->applicationInstall( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::isApplicationInstalled, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->isApplicationInstalled( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::applicationUninstall, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->applicationUninstall( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_openUrl(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::openUrl, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->openUrl( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_runApplication(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
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











        



    nCallbackArg = 3;

    






    bool arg2;
    if ( argc > 2 )
    {
        if ( argv[2].isString() )
            arg2 = argv[2].getInt() ? true : false;
        else if (!argv[2].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"boolean" );
            return oRes.toJSON();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
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


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &ISystem::runApplication, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->runApplication( arg0, arg1, arg2,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_unzipFile(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::unzipFile, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->unzipFile( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_zipFile(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
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
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
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











        



    nCallbackArg = 3;

    


    rho::StringW arg2;
    if ( argc > 2 )
    {
        if ( argv[2].isString() )
        {
            arg2 = convertToStringW(argv[2].getString());

        }
        else if (!argv[2].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"string" );
            return oRes.toJSON();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
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


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &ISystem::zipFile, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->zipFile( arg0, arg1, arg2,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_zipFiles(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
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
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
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











        



    nCallbackArg = 3;

    
    if ( argc == 2 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toJSON();
    }
    










    rho::Vector<rho::StringW> arg2;
    if ( argc > 2 )
    {
        if ( argv[2].isArray() )
        {
            CJSONArray arParam(argv[2]);
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg2.addElement( convertToStringW(arParam[i].getString()) );
            }
        }
        else if (!argv[2].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"array" );
            return oRes.toJSON();
        }
    }



        



    nCallbackArg = 4;

    


    rho::StringW arg3;
    if ( argc > 3 )
    {
        if ( argv[3].isString() )
        {
            arg3 = convertToStringW(argv[3].getString());

        }
        else if (!argv[3].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"3" L" should be " L"string" );
            return oRes.toJSON();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
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


    pFunctor = rho_makeInstanceClassFunctor5( pObj, &ISystem::zipFiles, arg0, arg1, arg2, arg3,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->zipFiles( arg0, arg1, arg2, arg3,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &ISystem::setRegistrySetting, arg0, arg1,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setRegistrySetting( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_getRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID)
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::getRegistrySetting, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getRegistrySetting( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setWindowFrame(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toJSON();
    }
    




    int arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toJSON();
        }
    }









        



    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toJSON();
    }
    




    int arg1;
    if ( argc > 1 )
    {
        if ( argv[1].isString() )
            arg1 = argv[1].getInt();
        else if (!argv[1].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
            return oRes.toJSON();
        }
    }









        



    nCallbackArg = 3;

    
    if ( argc == 2 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toJSON();
    }
    




    int arg2;
    if ( argc > 2 )
    {
        if ( argv[2].isString() )
            arg2 = argv[2].getInt();
        else if (!argv[2].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"integer" );
            return oRes.toJSON();
        }
    }









        



    nCallbackArg = 4;

    
    if ( argc == 3 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toJSON();
    }
    




    int arg3;
    if ( argc > 3 )
    {
        if ( argv[3].isString() )
            arg3 = argv[3].getInt();
        else if (!argv[3].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"3" L" should be " L"integer" );
            return oRes.toJSON();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
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


    pFunctor = rho_makeInstanceClassFunctor5( pObj, &ISystem::setWindowFrame, arg0, arg1, arg2, arg3,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setWindowFrame( arg0, arg1, arg2, arg3,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_System_setWindowPosition(rho::json::CJSONArray& argv, const rho::String& strObjID)
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
    




    int arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toJSON();
        }
    }









        



    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toJSON();
    }
    




    int arg1;
    if ( argc > 1 )
    {
        if ( argv[1].isString() )
            arg1 = argv[1].getInt();
        else if (!argv[1].isNull())
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &ISystem::setWindowPosition, arg0, arg1,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setWindowPosition( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}


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


