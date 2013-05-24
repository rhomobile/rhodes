

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
using namespace rho::apiGenerator;


rho::String js_Megamodule_getStringProperty(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getStringProperty(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eString);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getStringProperty,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getStringProperty(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_getIntegerProperty(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getIntegerProperty(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eInt);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getIntegerProperty,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getIntegerProperty(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_setIntegerProperty(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("setIntegerProperty(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    




    int arg0 = 12345;
    if ( argc > 0 )
    {
        if ( argv[0].isInteger() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toJSON();
        }
    }



















    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setIntegerProperty, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setIntegerProperty( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_typesTest(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("typesTest(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eBool);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    


    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }

















    
    if ( argc == 1 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    






    bool arg1 = false;
    if ( argc > 1 )
    {
        if ( argv[1].isBoolean() )
            arg1 = argv[1].getBoolean();
        else if (!argv[1].isNull())
        {
            oRes.setArgError("Type error: argument " "1" " should be " "boolean" );
            return oRes.toJSON();
        }
    }













    
    if ( argc == 2 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    




    int arg2 = 0;
    if ( argc > 2 )
    {
        if ( argv[2].isInteger() )
            arg2 = argv[2].getInt();
        else if (!argv[2].isNull())
        {
            oRes.setArgError("Type error: argument " "2" " should be " "integer" );
            return oRes.toJSON();
        }
    }















    
    if ( argc == 3 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toJSON();
    }
    








    double arg3 = 0;
    if ( argc > 3 )
    {
        if ( argv[3].isFloat() )
            arg3 = argv[3].getDouble();
        else if ( argv[3].isInteger() )
            arg3 = argv[3].getInt();
        else if (!argv[3].isNull())
        {
            oRes.setArgError("Type error: argument " "3" " should be " "float" );
            return oRes.toJSON();
        }
    }











    
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











    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor7( pObj, &rho::examples::IMegamodule::typesTest, arg0, arg1, arg2, arg3, arg4, arg5,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->typesTest( arg0, arg1, arg2, arg3, arg4, arg5,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_produceArray(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("produceArray(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setParamName( "result" );
    oRes.setRequestedType(CMethodResult::eStringArray);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceArray,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->produceArray(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_produceHash(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("produceHash(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eStringHash);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceHash,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->produceHash(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_produceComplicatedResult(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("produceComplicatedResult(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setParamName( "result" );
    oRes.setRequestedType(CMethodResult::eStringArray);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceComplicatedResult,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->produceComplicatedResult(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_s_Megamodule_getObjectsCount(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getObjectsCount(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eInt);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::getObjectsCount,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getObjectsCount(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_s_Megamodule_getObjectByIndex(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getObjectByIndex(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setJSObjectClassPath( "Rho.Examples.Megamodule" );
    oRes.setRequestedType(CMethodResult::eNone);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();



    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    




    int arg0 = 0;
    if ( argc > 0 )
    {
        if ( argv[0].isInteger() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toJSON();
        }
    }



















    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::getObjectByIndex, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getObjectByIndex( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_showAlertFromUIThread(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("showAlertFromUIThread(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }





















    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::showAlertFromUIThread, arg0,  oRes );
    rho_wm_impl_performOnUiThread( pFunctor );

    return oRes.toJSON();

}

rho::String js_Megamodule_setPeriodicallyCallback(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("setPeriodicallyCallback(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    




    int arg0 = 0;
    if ( argc > 0 )
    {
        if ( argv[0].isInteger() )
            arg0 = argv[0].getInt();
        else if (!argv[0].isNull())
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toJSON();
        }
    }

















    oRes.setCallInUIThread(false);
    oRes.setJSCallback( strCallbackID );
    oRes.setCallbackParam( strCallbackParam );



    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setPeriodicallyCallback, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setPeriodicallyCallback( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_isPeriodicallyCallbackSetted(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("isPeriodicallyCallbackSetted(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eBool);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);





    oRes.setCallInUIThread(false);
    oRes.setJSCallback( strCallbackID );
    oRes.setCallbackParam( strCallbackParam );



    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::isPeriodicallyCallbackSetted,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->isPeriodicallyCallbackSetted(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_stopPeriodicallyCallback(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("stopPeriodicallyCallback(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::stopPeriodicallyCallback,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->stopPeriodicallyCallback(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_complicatedTypesTest1(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("complicatedTypesTest1(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eBool);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
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













    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::complicatedTypesTest1, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->complicatedTypesTest1( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_getProperty(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getProperty(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eString);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toJSON();
    }
    


    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }



















    oRes.setCallInUIThread(false);
    oRes.setJSCallback( strCallbackID );
    oRes.setCallbackParam( strCallbackParam );



    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::getProperty, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getProperty( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_getProperties(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getProperties(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eStringHash);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
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











    oRes.setCallInUIThread(false);
    oRes.setJSCallback( strCallbackID );
    oRes.setCallbackParam( strCallbackParam );



    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::getProperties, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getProperties( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_getAllProperties(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("getAllProperties(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setRequestedType(CMethodResult::eStringHash);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);





    oRes.setCallInUIThread(false);
    oRes.setJSCallback( strCallbackID );
    oRes.setCallbackParam( strCallbackParam );



    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getAllProperties,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getAllProperties(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_setProperty(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("setProperty(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
    if ( argc == 0 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toJSON();
    }
    


    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( argv[0].isString() )
        {
            arg0 = argv[0].getString();
        }
        else if (!argv[0].isNull())
        {
            oRes.setArgError( "Type error: argument " "0" " should be " "string" );
            return oRes.toJSON();
        }
    }

















    
    if ( argc == 1 )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toJSON();
    }
    


    rho::String arg1 = "";
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





















    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::examples::IMegamodule::setProperty, arg0, arg1,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setProperty( arg0, arg1,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_setProperties(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("setProperties(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);



    
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











    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setProperties, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setProperties( arg0,  oRes );
    }

    return oRes.toJSON();

}

rho::String js_Megamodule_clearAllProperties(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("clearAllProperties(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;



    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();

    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strObjID);







    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::clearAllProperties,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->clearAllProperties(  oRes );
    }

    return oRes.toJSON();

}

rho::String js_s_Megamodule_enumerate(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    RAWTRACE3("enumerate(strObjID = %s, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), strCallbackID.c_str(), strJsVmID.c_str());

    rho::apiGenerator::CMethodResult oRes;


    oRes.setParamName( "result" );
    oRes.setJSObjectClassPath( "Rho.Examples.Megamodule" );
    oRes.setRequestedType(CMethodResult::eStringArray);

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    int argc = argv.getSize();





    oRes.setCallInUIThread(false);
    oRes.setJSCallback( strCallbackID );
    oRes.setCallbackParam( strCallbackParam );



    if ( oRes.hasCallback() )
    {
        pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::enumerate,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->enumerate(  oRes );
    }

    return oRes.toJSON();

}



rho::String js_s_Megamodule_getDefaultID(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    oRes.set(strDefaultID);

    return oRes.toJSON();
}

rho::String js_s_Megamodule_getDefault(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    oRes.set(strDefaultID);
    oRes.setJSObjectClassPath("Rho.Examples.Megamodule");

    return oRes.toJSON();
}

rho::String js_s_Megamodule_setDefaultID(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    rho::apiGenerator::CMethodResult oRes;
    rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->setDefaultID(strObjID);

    return oRes.toJSON();
}


