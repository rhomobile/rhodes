#include "SQLite3Base.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SQLite3"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_SQLite3_Module();



static void string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement( szVal );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    rho_ruby_enum_strary_json(val, string_iter, &res);
}

static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::String, rho::String>& hash = *((rho::Hashtable<rho::String, rho::String>*)(par));
    hash.put( szName, szVal );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::String, rho::String>& res)
{
    rho_ruby_enum_strhash_json(val, hash_eachstr, &res);
}


static VALUE _api_generator_SQLite3_open(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    rho::String arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "string" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    rho::String arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
        {
            arg1 = getStringFromValue(argv[1]);

        }
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::database::ISQLite3::open, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->open( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_open(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_open(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_close(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::close,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->close(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_close(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_close(argc, argv, pObj);

    rho::database::CSQLite3FactoryBase::getInstance()->deleteModuleByID(szID);        
    rho_ruby_clear_object_id( obj );
    return res;
}




static VALUE _api_generator_SQLite3_startTransaction(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::startTransaction,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->startTransaction(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_startTransaction(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_startTransaction(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_commitTransaction(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::commitTransaction,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->commitTransaction(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_commitTransaction(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_commitTransaction(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_rollbackTransaction(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::rollbackTransaction,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->rollbackTransaction(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_rollbackTransaction(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_rollbackTransaction(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_lockDb(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::lockDb,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->lockDb(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_lockDb(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_lockDb(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_unlockDb(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::unlockDb,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->unlockDb(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_unlockDb(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_unlockDb(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_destroyTables(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::database::ISQLite3::destroyTables,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->destroyTables(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_destroyTables(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_destroyTables(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_isTableExist(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::String arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::database::ISQLite3::isTableExist, arg0,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->isTableExist( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_isTableExist(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_isTableExist(argc, argv, pObj);

    return res;
}




static VALUE _api_generator_SQLite3_isUiWaitForDb(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::String arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::database::ISQLite3::isUiWaitForDb, arg0,  oRes );

    if ( bUseCallback )
        rho::database::CSQLite3FactoryBase::getSQLite3SingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->isUiWaitForDb( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_SQLite3_isUiWaitForDb(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_SQLite3_isUiWaitForDb(argc, argv, pObj);

    return res;
}




VALUE rb_impl_SQLite3_execute(int argc, VALUE *argv, rho::database::ISQLite3* pObj);


VALUE rb_SQLite3_execute(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::database::ISQLite3* pObj =  rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(szID);

    res = rb_impl_SQLite3_execute(argc, argv, pObj);

    return res;
}





}