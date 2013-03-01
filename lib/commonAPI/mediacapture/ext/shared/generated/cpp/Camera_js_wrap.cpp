#include "CameraBase.h"
#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Camera"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;


rho::String js_Camera_getDesiredWidth(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getDesiredWidth,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getDesiredWidth(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setDesiredWidth(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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
















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setDesiredWidth, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setDesiredWidth( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getDesiredHeight(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getDesiredHeight,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getDesiredHeight(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setDesiredHeight(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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
















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setDesiredHeight, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setDesiredHeight( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getCompressionFormat(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getCompressionFormat,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getCompressionFormat(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setCompressionFormat(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }


















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setCompressionFormat, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setCompressionFormat( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getOutputFormat(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getOutputFormat,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getOutputFormat(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setOutputFormat(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }


















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setOutputFormat, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setOutputFormat( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getColorModel(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getColorModel,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getColorModel(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setColorModel(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }


















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setColorModel, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setColorModel( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getEnableEditing(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getEnableEditing,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getEnableEditing(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setEnableEditing(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt() ? true : false;
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "boolean" );
            return oRes.toJSON();
        }
    }














    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setEnableEditing, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setEnableEditing( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getFlashMode(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getFlashMode,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getFlashMode(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setFlashMode(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }


















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setFlashMode, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setFlashMode( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getSaveToSharedGallery(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getSaveToSharedGallery,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getSaveToSharedGallery(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setSaveToSharedGallery(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
            arg0 = argv[0].getInt() ? true : false;
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "boolean" );
            return oRes.toJSON();
        }
    }














    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setSaveToSharedGallery, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setSaveToSharedGallery( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getCaptureSound(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getCaptureSound,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getCaptureSound(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setCaptureSound(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }


















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setCaptureSound, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setCaptureSound( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getPreviewLeft(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewLeft,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewLeft(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setPreviewLeft(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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
















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewLeft, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewLeft( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getPreviewTop(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewTop,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewTop(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setPreviewTop(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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
















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewTop, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewTop( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getPreviewWidth(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewWidth,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewWidth(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setPreviewWidth(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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
















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewWidth, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewWidth( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getPreviewHeight(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewHeight,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewHeight(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setPreviewHeight(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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
















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewHeight, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewHeight( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_takePicture(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    












    rho::Hashtable<rho::String, rho::String> arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isObject() )
        {
            CJSONStructIterator objIter(argv[0]);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg0[objIter.getCurKey()] = objIter.getCurString();
            }
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "hash" );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::takePicture, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->takePicture( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getCameraInfo(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getCameraInfo,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getCameraInfo(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_choosePicture(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    












    rho::Hashtable<rho::String, rho::String> arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isObject() )
        {
            CJSONStructIterator objIter(argv[0]);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg0[objIter.getCurKey()] = objIter.getCurString();
            }
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "hash" );
            return oRes.toJSON();
        }
    }








    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::choosePicture, arg0,  oRes );



    rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );


    return oRes.toJSON();

}

rho::String js_Camera_saveImageToDeviceGallery(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::saveImageToDeviceGallery,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->saveImageToDeviceGallery(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::getProperty, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getProperty( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::getProperties, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getProperties( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getAllProperties,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getAllProperties(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toJSON();
    }
    


    rho::String arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();

            oRes.setStringParam(argv[0].getString());

        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }

















    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toJSON();
    }
    


    rho::String arg1;
    if ( argc > 1 )
    {
        if ( argv[1].isString() )
        {
            arg1 = argv[1].getString();

        }
        else if (!argv[1].isNull())
        {
            oRes.setArgError( "Type error: argument " "1" " should be " "string" );
            return oRes.toJSON();
        }
    }


















    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ICamera::setProperty, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setProperty( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    












    rho::Hashtable<rho::String, rho::String> arg0;
    if ( argc > 0 )
    {
        if ( argv[0].isObject() )
        {
            CJSONStructIterator objIter(argv[0]);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg0[objIter.getCurKey()] = objIter.getCurString();
            }
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "hash" );
            return oRes.toJSON();
        }
    }








    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setProperties, arg0,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setProperties( arg0,  oRes );


    }


    return oRes.toJSON();

}

rho::String js_Camera_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::clearAllProperties,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->clearAllProperties(  oRes );


    }


    return oRes.toJSON();

}

rho::String js_s_Camera_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;




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


    pFunctor = rho_makeInstanceClassFunctor1( rho::CCameraFactoryBase::getCameraSingletonS(), &rho::ICameraSingleton::enumerate,  oRes );




    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CCameraFactoryBase::getCameraSingletonS()->enumerate(  oRes );


    }


    return oRes.toJSON();

}



rho::String js_s_Camera_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    oRes.set(strDefaultID);

    return oRes.toJSON();
}

rho::String js_s_Camera_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::CCameraFactoryBase::getCameraSingletonS()->setDefaultID(strObjID);

    return oRes.toJSON();
}


