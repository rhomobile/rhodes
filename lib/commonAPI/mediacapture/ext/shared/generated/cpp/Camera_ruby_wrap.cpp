#include "CameraBase.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Camera"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_Camera_Module();


VALUE rb_Camera_s_default(VALUE klass)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, strDefaultID.c_str() );
}

VALUE rb_Camera_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    rho::CCameraFactoryBase::getCameraSingletonS()->setDefaultID(szID);

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


static VALUE _api_generator_Camera_getDesiredWidth(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getDesiredWidth,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getDesiredWidth(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getDesiredWidth(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getDesiredWidth(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getDesiredWidth(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getDesiredWidth(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setDesiredWidth(int argc, VALUE *argv, rho::ICamera* pObj)
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
    int arg0;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setDesiredWidth, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setDesiredWidth( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setDesiredWidth(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setDesiredWidth(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setDesiredWidth(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setDesiredWidth(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getDesiredHeight(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getDesiredHeight,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getDesiredHeight(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getDesiredHeight(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getDesiredHeight(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getDesiredHeight(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getDesiredHeight(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setDesiredHeight(int argc, VALUE *argv, rho::ICamera* pObj)
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
    int arg0;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setDesiredHeight, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setDesiredHeight( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setDesiredHeight(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setDesiredHeight(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setDesiredHeight(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setDesiredHeight(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getCompressionFormat(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getCompressionFormat,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getCompressionFormat(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getCompressionFormat(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getCompressionFormat(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getCompressionFormat(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getCompressionFormat(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setCompressionFormat(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setCompressionFormat, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setCompressionFormat( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setCompressionFormat(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setCompressionFormat(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setCompressionFormat(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setCompressionFormat(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getOutputFormat(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getOutputFormat,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getOutputFormat(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getOutputFormat(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getOutputFormat(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getOutputFormat(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getOutputFormat(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setOutputFormat(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setOutputFormat, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setOutputFormat( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setOutputFormat(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setOutputFormat(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setOutputFormat(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setOutputFormat(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getColorModel(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getColorModel,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getColorModel(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getColorModel(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getColorModel(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getColorModel(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getColorModel(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setColorModel(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setColorModel, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setColorModel( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setColorModel(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setColorModel(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setColorModel(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setColorModel(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getEnableEditing(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getEnableEditing,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getEnableEditing(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getEnableEditing(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getEnableEditing(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getEnableEditing(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getEnableEditing(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setEnableEditing(int argc, VALUE *argv, rho::ICamera* pObj)
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
    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "boolean" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setEnableEditing, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setEnableEditing( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setEnableEditing(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setEnableEditing(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setEnableEditing(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setEnableEditing(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getFlashMode(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getFlashMode,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getFlashMode(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getFlashMode(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getFlashMode(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getFlashMode(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getFlashMode(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setFlashMode(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setFlashMode, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setFlashMode( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setFlashMode(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setFlashMode(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setFlashMode(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setFlashMode(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getSaveToSharedGallery(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getSaveToSharedGallery,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getSaveToSharedGallery(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getSaveToSharedGallery(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getSaveToSharedGallery(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getSaveToSharedGallery(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getSaveToSharedGallery(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setSaveToSharedGallery(int argc, VALUE *argv, rho::ICamera* pObj)
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
    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError("Type error: argument " "0" " should be " "boolean" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setSaveToSharedGallery, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setSaveToSharedGallery( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setSaveToSharedGallery(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setSaveToSharedGallery(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setSaveToSharedGallery(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setSaveToSharedGallery(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getCaptureSound(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getCaptureSound,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getCaptureSound(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getCaptureSound(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getCaptureSound(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getCaptureSound(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getCaptureSound(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setCaptureSound(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setCaptureSound, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setCaptureSound( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setCaptureSound(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setCaptureSound(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setCaptureSound(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setCaptureSound(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getPreviewLeft(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewLeft,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewLeft(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getPreviewLeft(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getPreviewLeft(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getPreviewLeft(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getPreviewLeft(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setPreviewLeft(int argc, VALUE *argv, rho::ICamera* pObj)
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
    int arg0;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewLeft, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewLeft( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setPreviewLeft(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setPreviewLeft(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setPreviewLeft(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setPreviewLeft(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getPreviewTop(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewTop,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewTop(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getPreviewTop(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getPreviewTop(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getPreviewTop(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getPreviewTop(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setPreviewTop(int argc, VALUE *argv, rho::ICamera* pObj)
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
    int arg0;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewTop, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewTop( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setPreviewTop(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setPreviewTop(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setPreviewTop(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setPreviewTop(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getPreviewWidth(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewWidth,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewWidth(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getPreviewWidth(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getPreviewWidth(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getPreviewWidth(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getPreviewWidth(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setPreviewWidth(int argc, VALUE *argv, rho::ICamera* pObj)
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
    int arg0;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewWidth, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewWidth( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setPreviewWidth(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setPreviewWidth(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setPreviewWidth(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setPreviewWidth(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getPreviewHeight(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getPreviewHeight,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getPreviewHeight(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getPreviewHeight(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getPreviewHeight(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getPreviewHeight(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getPreviewHeight(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setPreviewHeight(int argc, VALUE *argv, rho::ICamera* pObj)
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
    int arg0;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setPreviewHeight, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setPreviewHeight( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setPreviewHeight(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setPreviewHeight(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setPreviewHeight(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setPreviewHeight(argc, argv, pObj);
}


static VALUE _api_generator_Camera_takePicture(int argc, VALUE *argv, rho::ICamera* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::takePicture, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->takePicture( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_takePicture(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_takePicture(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_takePicture(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_takePicture(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getCameraInfo(int argc, VALUE *argv, rho::ICamera* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

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
    else
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(1) + ".Mandatory Callback parameter is mised." );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getCameraInfo,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getCameraInfo(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getCameraInfo(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getCameraInfo(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getCameraInfo(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getCameraInfo(argc, argv, pObj);
}


static VALUE _api_generator_Camera_choosePicture(int argc, VALUE *argv, rho::ICamera* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::choosePicture, arg0,  oRes );
    rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    return oRes.toRuby();
}



VALUE rb_Camera_choosePicture(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_choosePicture(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_choosePicture(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_choosePicture(argc, argv, pObj);
}


static VALUE _api_generator_Camera_saveImageToDeviceGallery(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::saveImageToDeviceGallery,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->saveImageToDeviceGallery(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_saveImageToDeviceGallery(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_saveImageToDeviceGallery(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_saveImageToDeviceGallery(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_saveImageToDeviceGallery(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getProperty(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::getProperty, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getProperty( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getProperty(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getProperties(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::getProperties, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getProperties( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getProperties(argc, argv, pObj);
}


static VALUE _api_generator_Camera_getAllProperties(int argc, VALUE *argv, rho::ICamera* pObj)
{
    rho::apiGenerator::CMethodResult oRes;

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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::getAllProperties,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->getAllProperties(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_getAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_getAllProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_getAllProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_getAllProperties(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setProperty(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ICamera::setProperty, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setProperty( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setProperty(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setProperty(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setProperty(argc, argv, pObj);
}


static VALUE _api_generator_Camera_setProperties(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ICamera::setProperties, arg0,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->setProperties( arg0,  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_setProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_setProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_setProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_setProperties(argc, argv, pObj);
}


static VALUE _api_generator_Camera_clearAllProperties(int argc, VALUE *argv, rho::ICamera* pObj)
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
    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ICamera::clearAllProperties,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        pObj->clearAllProperties(  oRes );

    }
    return oRes.toRuby();
}



VALUE rb_Camera_clearAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    rho::ICamera* pObj =  rho::CCameraFactoryBase::getInstance()->getModuleByID(szID);

    res = _api_generator_Camera_clearAllProperties(argc, argv, pObj);

    return res;
}



VALUE rb_s_Camera_def_clearAllProperties(int argc, VALUE *argv)
{
    rho::String strDefaultID = rho::CCameraFactoryBase::getCameraSingletonS()->getDefaultID();
    rho::ICamera* pObj = rho::CCameraFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Camera_clearAllProperties(argc, argv, pObj);
}


VALUE rb_s_Camera_enumerate(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    oRes.setRubyObjectClass( getRuby_Camera_Module() );
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
    pFunctor = rho_makeInstanceClassFunctor1( rho::CCameraFactoryBase::getCameraSingletonS(), &rho::ICameraSingleton::enumerate,  oRes );

    if ( bUseCallback )
        rho::CCameraFactoryBase::getCameraSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CCameraFactoryBase::getCameraSingletonS()->enumerate(  oRes );

    }
    return oRes.toRuby();
}







}