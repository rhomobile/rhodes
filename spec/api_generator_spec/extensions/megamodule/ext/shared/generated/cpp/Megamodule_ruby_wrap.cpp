
#include "MegamoduleBase.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Megamodule"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;
using namespace rho::apiGenerator;

extern "C"
{
void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_Megamodule_Module();


VALUE rb_Megamodule_s_default(VALUE klass)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, strDefaultID.c_str() );
}

VALUE rb_Megamodule_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->setDefaultID(szID);

    return rho_ruby_get_NIL();
}


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


static VALUE _api_generator_Megamodule_getStringProperty(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eString);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getStringProperty,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getStringProperty(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_getStringProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_getStringProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_getStringProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_getStringProperty(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_getIntegerProperty(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eInt);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getIntegerProperty,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getIntegerProperty(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_getIntegerProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_getIntegerProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_getIntegerProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_getIntegerProperty(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_setIntegerProperty(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
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
    int arg0 = 12345;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setIntegerProperty, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setIntegerProperty( arg0,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_setIntegerProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_setIntegerProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_setIntegerProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_setIntegerProperty(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_typesTest(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eBool);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);
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
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    bool arg1 = false;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_boolean(argv[1]) )
            arg1 = rho_ruby_get_bool(argv[1]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "boolean" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 3;
    if ( argc == 2 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    int arg2 = 0;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_integer(argv[2]) )
            arg2 = rho_ruby_get_int(argv[2]);
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 4;
    if ( argc == 3 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    double arg3 = 0;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_double(argv[3]) )
            arg3 = rho_ruby_get_double(argv[3]);
        else if (!rho_ruby_is_NIL(argv[3]))
        {
            oRes.setArgError("Type error: argument " "3" " should be " "float" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 5;
    if ( argc == 4 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    rho::Vector<rho::String> arg4;
    if ( argc > 4 )
    {
        if ( rho_ruby_is_array(argv[4]) )
            getStringArrayFromValue(argv[4], arg4);
        else if (!rho_ruby_is_NIL(argv[4]))
        {
            oRes.setArgError("Type error: argument " "4" " should be " "array" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 6;
    if ( argc == 5 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    rho::Hashtable<rho::String, rho::String> arg5;
    if ( argc > 5 )
    {
        if ( rho_ruby_is_hash(argv[5]) )
            getStringHashFromValue(argv[5], arg5);
        else if (!rho_ruby_is_NIL(argv[5]))
        {
            oRes.setArgError("Type error: argument " "5" " should be " "hash" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(6) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor7( pObj, &rho::examples::IMegamodule::typesTest, arg0, arg1, arg2, arg3, arg4, arg5,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->typesTest( arg0, arg1, arg2, arg3, arg4, arg5,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_typesTest(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_typesTest(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_typesTest(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_typesTest(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_produceArray(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setParamName( "result" );
    oRes.setRequestedType(CMethodResult::eStringArray);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceArray,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->produceArray(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_produceArray(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_produceArray(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_produceArray(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_produceArray(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_produceHash(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eStringHash);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceHash,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->produceHash(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_produceHash(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_produceHash(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_produceHash(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_produceHash(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_produceComplicatedResult(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setParamName( "result" );
    oRes.setRequestedType(CMethodResult::eStringArray);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::produceComplicatedResult,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->produceComplicatedResult(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_produceComplicatedResult(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_produceComplicatedResult(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_produceComplicatedResult(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_produceComplicatedResult(argc, argv, pObj);
}


VALUE rb_s_Megamodule_getObjectsCount(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eInt);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::getObjectsCount,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getObjectsCount(  oRes );
    }
    
    return oRes.toRuby();
}






VALUE rb_s_Megamodule_getObjectByIndex(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRubyObjectClass( getRuby_Megamodule_Module() );
    oRes.setRequestedType(CMethodResult::eNone);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    int arg0 = 0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::getObjectByIndex, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getObjectByIndex( arg0,  oRes );
    }
    
    return oRes.toRuby();
}






static VALUE _api_generator_Megamodule_showAlertFromUIThread(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
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
    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::showAlertFromUIThread, arg0,  oRes );
    rho_wm_impl_performOnUiThread( pFunctor );
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_showAlertFromUIThread(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_showAlertFromUIThread(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_showAlertFromUIThread(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_showAlertFromUIThread(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_setPeriodicallyCallback(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    int arg0 = 0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    else
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) + ".Mandatory Callback parameter is mised." );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setPeriodicallyCallback, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setPeriodicallyCallback( arg0,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_setPeriodicallyCallback(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_setPeriodicallyCallback(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_setPeriodicallyCallback(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_setPeriodicallyCallback(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_isPeriodicallyCallbackSetted(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eBool);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::isPeriodicallyCallbackSetted,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->isPeriodicallyCallbackSetted(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_isPeriodicallyCallbackSetted(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_isPeriodicallyCallbackSetted(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_isPeriodicallyCallbackSetted(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_isPeriodicallyCallbackSetted(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_stopPeriodicallyCallback(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
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
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::stopPeriodicallyCallback,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->stopPeriodicallyCallback(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_stopPeriodicallyCallback(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_stopPeriodicallyCallback(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_stopPeriodicallyCallback(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_stopPeriodicallyCallback(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_complicatedTypesTest1(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eBool);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::Vector<rho::String> arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_array(argv[0]) )
            getStringArrayFromValue(argv[0], arg0);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "array" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::complicatedTypesTest1, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->complicatedTypesTest1( arg0,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_complicatedTypesTest1(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_complicatedTypesTest1(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_complicatedTypesTest1(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_complicatedTypesTest1(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_getProperty(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eString);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);
        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::getProperty, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getProperty( arg0,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_getProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_getProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_getProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_getProperty(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_getProperties(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eStringHash);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    rho::Vector<rho::String> arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_array(argv[0]) )
            getStringArrayFromValue(argv[0], arg0);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "array" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::getProperties, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getProperties( arg0,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_getProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_getProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_getProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_getProperties(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_getAllProperties(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRequestedType(CMethodResult::eStringHash);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::getAllProperties,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->getAllProperties(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_getAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_getAllProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_getAllProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_getAllProperties(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_setProperty(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
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
    rho::String arg0 = "";
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = getStringFromValue(argv[0]);
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
    rho::String arg1 = "";
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
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::examples::IMegamodule::setProperty, arg0, arg1,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setProperty( arg0, arg1,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_setProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_setProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_setProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_setProperty(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_setProperties(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
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
    rho::Hashtable<rho::String, rho::String> arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_hash(argv[0]) )
            getStringHashFromValue(argv[0], arg0);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "hash" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::examples::IMegamodule::setProperties, arg0,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->setProperties( arg0,  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_setProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_setProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_setProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_setProperties(argc, argv, pObj);
}


static VALUE _api_generator_Megamodule_clearAllProperties(int argc, VALUE *argv, rho::examples::IMegamodule* pObj)
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
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::examples::IMegamodule::clearAllProperties,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        pObj->clearAllProperties(  oRes );
    }
    
    return oRes.toRuby();
}



VALUE rb_Megamodule_clearAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::examples::IMegamodule* pObj =  rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Megamodule_clearAllProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Megamodule_def_clearAllProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->getDefaultID();
    rho::examples::IMegamodule* pObj = rho::examples::CMegamoduleFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Megamodule_clearAllProperties(argc, argv, pObj);
}


VALUE rb_s_Megamodule_enumerate(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setParamName( "result" );
    oRes.setRubyObjectClass( getRuby_Megamodule_Module() );
    oRes.setRequestedType(CMethodResult::eStringArray);
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
    }
    if ( bUseCallback )
    {
        pFunctor = rho_makeInstanceClassFunctor1( rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS(), &rho::examples::IMegamoduleSingleton::enumerate,  oRes );
        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->addCommandToQueue( pFunctor );
    }
    else 
    {

        rho::examples::CMegamoduleFactoryBase::getMegamoduleSingletonS()->enumerate(  oRes );
    }
    
    return oRes.toRuby();
}







}