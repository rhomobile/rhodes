#include "SQLite3Base.h"
#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SQLite3"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;


rho::String js_SQLite3_open(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::database::ISQLite3::open, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->open( arg0, arg1,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_close(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::close,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->close(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_startTransaction(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::startTransaction,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->startTransaction(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_commitTransaction(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::commitTransaction,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->commitTransaction(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_rollbackTransaction(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::rollbackTransaction,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->rollbackTransaction(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_lockDb(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::lockDb,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->lockDb(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_unlockDb(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::unlockDb,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->unlockDb(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_destroyTables(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);




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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::destroyTables,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->destroyTables(  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_isTableExist(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::database::ISQLite3::isTableExist, arg0,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->isTableExist( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_isUiWaitForDb(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::database::ISQLite3::isUiWaitForDb, arg0,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->isUiWaitForDb( arg0,  oRes );


    }


    return oRes.toJSON();

}


rho::String js_SQLite3_execute(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;

    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);



    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    










    rho::Vector<rho::String> arg2;
    if ( argc > 2 )
    {
        if ( argv[2].isArray() )
        {
            CJSONArray arParam(argv[2]);
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg2.addElement( arParam[i].getString() );
            }
        }
        else if (!argv[2].isNull())
        {
            oRes.setArgError("Type error: argument " "2" " should be " "array" );
            return oRes.toJSON();
        }
    }










    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &rho::database::ISQLite3::execute, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->execute( arg0, arg1, arg2,  oRes );


    }


    return oRes.toJSON();

}




