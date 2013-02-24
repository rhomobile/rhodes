#include "MegamoduleBase.h"
#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Megamodule"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;


rho::String js_Megamodule_getStringProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getStringProperty,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getStringProperty(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_getIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getIntegerProperty,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getIntegerProperty(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_setIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setIntegerProperty, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setIntegerProperty( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_typesTest(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
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
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    






    bool arg1;
    if ( argc > 1 )
    {
        if ( argv[1].isString() )
            arg1 = argv[1].getInt() ? true : false;
        else if (!argv[1].isNull())
        {
            oRes.setArgError("Type error: argument " "1" " should be " "boolean" );
            return oRes.toJSON();
        }
    }













    nCallbackArg = 3;

    
    if ( argc == 2 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    




    int arg2;
    if ( argc > 2 )
    {
        if ( argv[2].isString() )
            arg2 = argv[2].getInt();
        else if (!argv[2].isNull())
        {
            oRes.setArgError("Type error: argument " "2" " should be " "integer" );
            return oRes.toJSON();
        }
    }















    nCallbackArg = 4;

    
    if ( argc == 3 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    








    double arg3;
    if ( argc > 3 )
    {
        if ( argv[3].isString() )
            arg3 = argv[3].getDouble();
        else if (!argv[3].isNull())
        {
            oRes.setArgError("Type error: argument " "3" " should be " "float" );
            return oRes.toJSON();
        }
    }











    nCallbackArg = 5;

    
    if ( argc == 4 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    










    rho::Vector<rho::String> arg4;
    if ( argc > 4 )
    {
        if ( argv[4].isArray() )
        {
            CJSONArray arParam(argv[4]);
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg4.addElement( arParam[i].getString() );
            }
        }
        else if (!argv[4].isNull())
        {
            oRes.setArgError("Type error: argument " "4" " should be " "array" );
            return oRes.toJSON();
        }
    }









    nCallbackArg = 6;

    
    if ( argc == 5 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    












    rho::Hashtable<rho::String, rho::String> arg5;
    if ( argc > 5 )
    {
        if ( argv[5].isObject() )
        {
            CJSONStructIterator objIter(argv[5]);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg5[objIter.getCurKey()] = objIter.getCurString();
            }
        }
        else if (!argv[5].isNull())
        {
            oRes.setArgError("Type error: argument " "5" " should be " "hash" );
            return oRes.toJSON();
        }
    }








    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
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


    pFunctor = rho_makeInstanceClassFunctor7( pObj, &rho::examples::IMegamodule::typesTest, arg0, arg1, arg2, arg3, arg4, arg5,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->typesTest( arg0, arg1, arg2, arg3, arg4, arg5,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_produceArray(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceArray,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->produceArray(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_produceHash(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceHash,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->produceHash(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_produceComplicatedResult(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceComplicatedResult,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->produceComplicatedResult(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_s_Megamodule_getObjectsCount(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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


    pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::getObjectsCount,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getObjectsCount(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_s_Megamodule_getObjectByIndex(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;



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


    pFunctor = rho_makeInstanceClassFunctor2( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::getObjectByIndex, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getObjectByIndex( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::showAlertFromUIThread, arg0,  oRes );



    rho_wm_impl_performOnUiThread( pFunctor );


    return oRes.toJSON();

}


rho::String js_Megamodule_setPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    




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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setPeriodicallyCallback, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPeriodicallyCallback( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_isPeriodicallyCallbackSetted(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::isPeriodicallyCallbackSetted,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->isPeriodicallyCallbackSetted(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_stopPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::stopPeriodicallyCallback,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->stopPeriodicallyCallback(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_complicatedTypesTest1(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::complicatedTypesTest1, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->complicatedTypesTest1( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::getProperty, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getProperty( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::getProperties, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getProperties( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getAllProperties,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getAllProperties(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::examples::IMegamodule::setProperty, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setProperty( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setProperties, arg0,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setProperties( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_Megamodule_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::clearAllProperties,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->clearAllProperties(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_s_Megamodule_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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


    pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::enumerate,  oRes );




    if ( bUseCallback )
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->enumerate(  oRes );


    }


    return oRes.toJSON();

}




rho::String js_s_Megamodule_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    oRes.set(strDefaultID);

    return oRes.toJSON();
}

rho::String js_s_Megamodule_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->setDefaultID(strObjID);

    return oRes.toJSON();
}


