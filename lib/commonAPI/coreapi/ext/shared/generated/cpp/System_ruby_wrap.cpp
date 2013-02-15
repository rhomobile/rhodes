#include "SystemBase.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_System_Module();


VALUE rb_System_s_default(VALUE klass)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, convertToStringA(strDefaultID).c_str() );
}

VALUE rb_System_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    rho::CSystemFactoryBase::getSystemSingletonS()->setDefaultID(convertToStringW(szID));

    return rho_ruby_get_NIL();
}


extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::StringW>& ar = *((rho::Vector<rho::StringW>*)(par));
    ar.addElement( convertToStringW(szVal) );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::StringW>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

extern "C" static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::StringW, rho::StringW>& hash = *((rho::Hashtable<rho::StringW, rho::StringW>*)(par));
    hash.put( convertToStringW(szName), convertToStringW(szVal) );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::StringW, rho::StringW>& res)
{
    rho_ruby_enum_strhash(val, hash_eachstr, &res);
}



static VALUE _api_generator_System_getPlatform(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getPlatform,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPlatform(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getPlatform(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPlatform(argc, argv, pObj);
}



VALUE rb_s_System_def_getPlatform(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPlatform(argc, argv, pObj);
}



static VALUE _api_generator_System_getHasCamera(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getHasCamera,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHasCamera(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getHasCamera(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHasCamera(argc, argv, pObj);
}



VALUE rb_s_System_def_getHasCamera(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHasCamera(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenWidth(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getScreenWidth,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenWidth(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getScreenWidth(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenWidth(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenWidth(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenWidth(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenHeight(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getScreenHeight,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenHeight(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getScreenHeight(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenHeight(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenHeight(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenHeight(argc, argv, pObj);
}



static VALUE _api_generator_System_getRealScreenWidth(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getRealScreenWidth,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getRealScreenWidth(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getRealScreenWidth(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getRealScreenWidth(argc, argv, pObj);
}



VALUE rb_s_System_def_getRealScreenWidth(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getRealScreenWidth(argc, argv, pObj);
}



static VALUE _api_generator_System_getRealScreenHeight(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getRealScreenHeight,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getRealScreenHeight(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getRealScreenHeight(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getRealScreenHeight(argc, argv, pObj);
}



VALUE rb_s_System_def_getRealScreenHeight(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getRealScreenHeight(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenOrientation(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getScreenOrientation,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenOrientation(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getScreenOrientation(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenOrientation(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenOrientation(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenOrientation(argc, argv, pObj);
}



static VALUE _api_generator_System_getPpiX(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getPpiX,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPpiX(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getPpiX(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPpiX(argc, argv, pObj);
}



VALUE rb_s_System_def_getPpiX(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPpiX(argc, argv, pObj);
}



static VALUE _api_generator_System_getPpiY(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getPpiY,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPpiY(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getPpiY(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPpiY(argc, argv, pObj);
}



VALUE rb_s_System_def_getPpiY(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPpiY(argc, argv, pObj);
}



static VALUE _api_generator_System_getPhoneNumber(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getPhoneNumber,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPhoneNumber(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getPhoneNumber(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPhoneNumber(argc, argv, pObj);
}



VALUE rb_s_System_def_getPhoneNumber(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPhoneNumber(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceOwnerEmail(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getDeviceOwnerEmail,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceOwnerEmail(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getDeviceOwnerEmail(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceOwnerEmail(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceOwnerEmail(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceOwnerEmail(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceOwnerName(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getDeviceOwnerName,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceOwnerName(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getDeviceOwnerName(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceOwnerName(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceOwnerName(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceOwnerName(argc, argv, pObj);
}



static VALUE _api_generator_System_getDevicePushId(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getDevicePushId,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDevicePushId(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getDevicePushId(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDevicePushId(argc, argv, pObj);
}



VALUE rb_s_System_def_getDevicePushId(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDevicePushId(argc, argv, pObj);
}



static VALUE _api_generator_System_getPhoneId(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getPhoneId,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getPhoneId(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getPhoneId(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPhoneId(argc, argv, pObj);
}



VALUE rb_s_System_def_getPhoneId(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPhoneId(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceName(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getDeviceName,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceName(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getDeviceName(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceName(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceName(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceName(argc, argv, pObj);
}



static VALUE _api_generator_System_getOsVersion(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getOsVersion,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getOsVersion(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getOsVersion(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getOsVersion(argc, argv, pObj);
}



VALUE rb_s_System_def_getOsVersion(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getOsVersion(argc, argv, pObj);
}



static VALUE _api_generator_System_getLocale(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getLocale,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getLocale(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getLocale(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getLocale(argc, argv, pObj);
}



VALUE rb_s_System_def_getLocale(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getLocale(argc, argv, pObj);
}



static VALUE _api_generator_System_getCountry(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getCountry,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getCountry(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getCountry(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getCountry(argc, argv, pObj);
}



VALUE rb_s_System_def_getCountry(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getCountry(argc, argv, pObj);
}



static VALUE _api_generator_System_getIsEmulator(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getIsEmulator,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getIsEmulator(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getIsEmulator(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getIsEmulator(argc, argv, pObj);
}



VALUE rb_s_System_def_getIsEmulator(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getIsEmulator(argc, argv, pObj);
}



static VALUE _api_generator_System_getHasCalendar(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getHasCalendar,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHasCalendar(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getHasCalendar(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHasCalendar(argc, argv, pObj);
}



VALUE rb_s_System_def_getHasCalendar(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHasCalendar(argc, argv, pObj);
}



static VALUE _api_generator_System_getIsMotorolaDevice(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getIsMotorolaDevice,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getIsMotorolaDevice(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getIsMotorolaDevice(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getIsMotorolaDevice(argc, argv, pObj);
}



VALUE rb_s_System_def_getIsMotorolaDevice(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getIsMotorolaDevice(argc, argv, pObj);
}



static VALUE _api_generator_System_getOemInfo(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getOemInfo,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getOemInfo(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getOemInfo(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getOemInfo(argc, argv, pObj);
}



VALUE rb_s_System_def_getOemInfo(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getOemInfo(argc, argv, pObj);
}



static VALUE _api_generator_System_getUuid(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getUuid,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getUuid(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getUuid(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getUuid(argc, argv, pObj);
}



VALUE rb_s_System_def_getUuid(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getUuid(argc, argv, pObj);
}



static VALUE _api_generator_System_getApplicationIconBadge(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getApplicationIconBadge,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getApplicationIconBadge(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getApplicationIconBadge(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getApplicationIconBadge(argc, argv, pObj);
}



VALUE rb_s_System_def_getApplicationIconBadge(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getApplicationIconBadge(argc, argv, pObj);
}



static VALUE _api_generator_System_setApplicationIconBadge(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    




    int64 arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setApplicationIconBadge, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setApplicationIconBadge( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setApplicationIconBadge(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setApplicationIconBadge(argc, argv, pObj);
}



VALUE rb_s_System_def_setApplicationIconBadge(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setApplicationIconBadge(argc, argv, pObj);
}



static VALUE _api_generator_System_getHttpProxyURI(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getHttpProxyURI,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHttpProxyURI(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getHttpProxyURI(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHttpProxyURI(argc, argv, pObj);
}



VALUE rb_s_System_def_getHttpProxyURI(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHttpProxyURI(argc, argv, pObj);
}



static VALUE _api_generator_System_setHttpProxyURI(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setHttpProxyURI, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setHttpProxyURI( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setHttpProxyURI(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setHttpProxyURI(argc, argv, pObj);
}



VALUE rb_s_System_def_setHttpProxyURI(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setHttpProxyURI(argc, argv, pObj);
}



static VALUE _api_generator_System_getLockWindowSize(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getLockWindowSize,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getLockWindowSize(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getLockWindowSize(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getLockWindowSize(argc, argv, pObj);
}



VALUE rb_s_System_def_getLockWindowSize(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getLockWindowSize(argc, argv, pObj);
}



static VALUE _api_generator_System_setLockWindowSize(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
            return oRes.toRuby();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setLockWindowSize, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setLockWindowSize( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setLockWindowSize(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setLockWindowSize(argc, argv, pObj);
}



VALUE rb_s_System_def_setLockWindowSize(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setLockWindowSize(argc, argv, pObj);
}



static VALUE _api_generator_System_getShowKeyboard(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getShowKeyboard,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getShowKeyboard(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getShowKeyboard(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getShowKeyboard(argc, argv, pObj);
}



VALUE rb_s_System_def_getShowKeyboard(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getShowKeyboard(argc, argv, pObj);
}



static VALUE _api_generator_System_setShowKeyboard(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
            return oRes.toRuby();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setShowKeyboard, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setShowKeyboard( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setShowKeyboard(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setShowKeyboard(argc, argv, pObj);
}



VALUE rb_s_System_def_setShowKeyboard(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setShowKeyboard(argc, argv, pObj);
}



static VALUE _api_generator_System_getFullScreen(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getFullScreen,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getFullScreen(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getFullScreen(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getFullScreen(argc, argv, pObj);
}



VALUE rb_s_System_def_getFullScreen(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getFullScreen(argc, argv, pObj);
}



static VALUE _api_generator_System_setFullScreen(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
            return oRes.toRuby();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setFullScreen, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setFullScreen( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setFullScreen(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setFullScreen(argc, argv, pObj);
}



VALUE rb_s_System_def_setFullScreen(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setFullScreen(argc, argv, pObj);
}



static VALUE _api_generator_System_getLocalServerPort(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getLocalServerPort,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getLocalServerPort(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getLocalServerPort(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getLocalServerPort(argc, argv, pObj);
}



VALUE rb_s_System_def_getLocalServerPort(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getLocalServerPort(argc, argv, pObj);
}



static VALUE _api_generator_System_setLocalServerPort(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    




    int64 arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setLocalServerPort, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setLocalServerPort( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setLocalServerPort(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setLocalServerPort(argc, argv, pObj);
}



VALUE rb_s_System_def_setLocalServerPort(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setLocalServerPort(argc, argv, pObj);
}



static VALUE _api_generator_System_getFreeServerPort(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getFreeServerPort,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getFreeServerPort(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getFreeServerPort(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getFreeServerPort(argc, argv, pObj);
}



VALUE rb_s_System_def_getFreeServerPort(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getFreeServerPort(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenAutoRotate(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getScreenAutoRotate,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenAutoRotate(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getScreenAutoRotate(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenAutoRotate(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenAutoRotate(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenAutoRotate(argc, argv, pObj);
}



static VALUE _api_generator_System_setScreenAutoRotate(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
            return oRes.toRuby();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setScreenAutoRotate, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setScreenAutoRotate( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setScreenAutoRotate(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setScreenAutoRotate(argc, argv, pObj);
}



VALUE rb_s_System_def_setScreenAutoRotate(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setScreenAutoRotate(argc, argv, pObj);
}



static VALUE _api_generator_System_getHasTouchscreen(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getHasTouchscreen,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getHasTouchscreen(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getHasTouchscreen(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHasTouchscreen(argc, argv, pObj);
}



VALUE rb_s_System_def_getHasTouchscreen(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHasTouchscreen(argc, argv, pObj);
}



static VALUE _api_generator_System_getSecurityTokenNotPassed(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getSecurityTokenNotPassed,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getSecurityTokenNotPassed(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getSecurityTokenNotPassed(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getSecurityTokenNotPassed(argc, argv, pObj);
}



VALUE rb_s_System_def_getSecurityTokenNotPassed(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getSecurityTokenNotPassed(argc, argv, pObj);
}



static VALUE _api_generator_System_getWebviewFramework(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getWebviewFramework,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getWebviewFramework(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getWebviewFramework(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getWebviewFramework(argc, argv, pObj);
}



VALUE rb_s_System_def_getWebviewFramework(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getWebviewFramework(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenSleeping(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getScreenSleeping,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getScreenSleeping(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getScreenSleeping(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenSleeping(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenSleeping(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenSleeping(argc, argv, pObj);
}



static VALUE _api_generator_System_setScreenSleeping(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    






    bool arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_boolean(argv[0]) )
            arg0 = rho_ruby_get_bool(argv[0]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"boolean" );
            return oRes.toRuby();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setScreenSleeping, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setScreenSleeping( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setScreenSleeping(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setScreenSleeping(argc, argv, pObj);
}



VALUE rb_s_System_def_setScreenSleeping(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setScreenSleeping(argc, argv, pObj);
}



static VALUE _api_generator_System_applicationInstall(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::applicationInstall, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->applicationInstall( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_applicationInstall(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_applicationInstall(argc, argv, pObj);
}



VALUE rb_s_System_def_applicationInstall(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_applicationInstall(argc, argv, pObj);
}



static VALUE _api_generator_System_isApplicationInstalled(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::isApplicationInstalled, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->isApplicationInstalled( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_isApplicationInstalled(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_isApplicationInstalled(argc, argv, pObj);
}



VALUE rb_s_System_def_isApplicationInstalled(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_isApplicationInstalled(argc, argv, pObj);
}



static VALUE _api_generator_System_applicationUninstall(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::applicationUninstall, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->applicationUninstall( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_applicationUninstall(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_applicationUninstall(argc, argv, pObj);
}



VALUE rb_s_System_def_applicationUninstall(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_applicationUninstall(argc, argv, pObj);
}



static VALUE _api_generator_System_getStartParams(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getStartParams,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getStartParams(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getStartParams(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getStartParams(argc, argv, pObj);
}



VALUE rb_s_System_def_getStartParams(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getStartParams(argc, argv, pObj);
}



static VALUE _api_generator_System_openUrl(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::openUrl, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->openUrl( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_openUrl(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_openUrl(argc, argv, pObj);
}



VALUE rb_s_System_def_openUrl(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_openUrl(argc, argv, pObj);
}



static VALUE _api_generator_System_unzipFile(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    


    rho::StringW arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
        {
            arg1 = convertToStringW(getStringFromValue(argv[1]));

        }
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ISystem::unzipFile, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->unzipFile( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_unzipFile(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_unzipFile(argc, argv, pObj);
}



VALUE rb_s_System_def_unzipFile(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_unzipFile(argc, argv, pObj);
}



static VALUE _api_generator_System_zipFile(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();
    }
    


    rho::StringW arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
        {
            arg1 = convertToStringW(getStringFromValue(argv[1]));

        }
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 3;

    


    rho::StringW arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = convertToStringW(getStringFromValue(argv[2]));

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &rho::ISystem::zipFile, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->zipFile( arg0, arg1, arg2,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_zipFile(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_zipFile(argc, argv, pObj);
}



VALUE rb_s_System_def_zipFile(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_zipFile(argc, argv, pObj);
}



static VALUE _api_generator_System_zipFiles(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    


    rho::StringW arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
        {
            arg1 = convertToStringW(getStringFromValue(argv[1]));

        }
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 3;

    
    if ( argc == 2 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    










    rho::Vector<rho::StringW> arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_array(argv[2]) )
            getStringArrayFromValue(argv[2], arg2);
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"array" );
            return oRes.toRuby();
        }
    }



        




    nCallbackArg = 4;

    


    rho::StringW arg3;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_string(argv[3]) )
        {
            arg3 = convertToStringW(getStringFromValue(argv[3]));

        }
        else if (!rho_ruby_is_NIL(argv[3]))
        {
            oRes.setArgError(L"Type error: argument " L"3" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor5( pObj, &rho::ISystem::zipFiles, arg0, arg1, arg2, arg3,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->zipFiles( arg0, arg1, arg2, arg3,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_zipFiles(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_zipFiles(argc, argv, pObj);
}



VALUE rb_s_System_def_zipFiles(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_zipFiles(argc, argv, pObj);
}



static VALUE _api_generator_System_setRegistrySetting(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    


    rho::StringW arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
        {
            arg1 = convertToStringW(getStringFromValue(argv[1]));

        }
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ISystem::setRegistrySetting, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setRegistrySetting( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setRegistrySetting(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setRegistrySetting(argc, argv, pObj);
}



VALUE rb_s_System_def_setRegistrySetting(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setRegistrySetting(argc, argv, pObj);
}



static VALUE _api_generator_System_getRegistrySetting(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::getRegistrySetting, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getRegistrySetting( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getRegistrySetting(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getRegistrySetting(argc, argv, pObj);
}



VALUE rb_s_System_def_getRegistrySetting(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getRegistrySetting(argc, argv, pObj);
}



static VALUE _api_generator_System_setWindowFrame(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    




    int64 arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    




    int64 arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    nCallbackArg = 3;

    
    if ( argc == 2 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    




    int64 arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_integer(argv[2]) )
            arg2 = rho_ruby_get_int(argv[2]);
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    nCallbackArg = 4;

    
    if ( argc == 3 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    




    int64 arg3;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_integer(argv[3]) )
            arg3 = rho_ruby_get_int(argv[3]);
        else if (!rho_ruby_is_NIL(argv[3]))
        {
            oRes.setArgError(L"Type error: argument " L"3" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor5( pObj, &rho::ISystem::setWindowFrame, arg0, arg1, arg2, arg3,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setWindowFrame( arg0, arg1, arg2, arg3,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setWindowFrame(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setWindowFrame(argc, argv, pObj);
}



VALUE rb_s_System_def_setWindowFrame(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setWindowFrame(argc, argv, pObj);
}



static VALUE _api_generator_System_setWindowPosition(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    




    int64 arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    




    int64 arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ISystem::setWindowPosition, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setWindowPosition( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setWindowPosition(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setWindowPosition(argc, argv, pObj);
}



VALUE rb_s_System_def_setWindowPosition(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setWindowPosition(argc, argv, pObj);
}



static VALUE _api_generator_System_setWindowSize(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    




    int64 arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_integer(argv[0]) )
            arg0 = rho_ruby_get_int(argv[0]);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    




    int64 arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ISystem::setWindowSize, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setWindowSize( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setWindowSize(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setWindowSize(argc, argv, pObj);
}



VALUE rb_s_System_def_setWindowSize(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setWindowSize(argc, argv, pObj);
}



static VALUE _api_generator_System_bringToFront(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::bringToFront,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->bringToFront(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_bringToFront(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_bringToFront(argc, argv, pObj);
}



VALUE rb_s_System_def_bringToFront(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_bringToFront(argc, argv, pObj);
}



static VALUE _api_generator_System_replaceCurrentBundle(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::replaceCurrentBundle, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->replaceCurrentBundle( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_replaceCurrentBundle(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_replaceCurrentBundle(argc, argv, pObj);
}



VALUE rb_s_System_def_replaceCurrentBundle(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_replaceCurrentBundle(argc, argv, pObj);
}



static VALUE _api_generator_System_deleteFolder(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::deleteFolder, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->deleteFolder( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_deleteFolder(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_deleteFolder(argc, argv, pObj);
}



VALUE rb_s_System_def_deleteFolder(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_deleteFolder(argc, argv, pObj);
}



static VALUE _api_generator_System_setDoNotBackupAttribute(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setDoNotBackupAttribute, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setDoNotBackupAttribute( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setDoNotBackupAttribute(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setDoNotBackupAttribute(argc, argv, pObj);
}



VALUE rb_s_System_def_setDoNotBackupAttribute(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setDoNotBackupAttribute(argc, argv, pObj);
}



static VALUE _api_generator_System_isBlobAttr(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();
    }
    




    int64 arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    nCallbackArg = 3;

    
    if ( argc == 2 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();
    }
    


    rho::StringW arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = convertToStringW(getStringFromValue(argv[2]));

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &rho::ISystem::isBlobAttr, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->isBlobAttr( arg0, arg1, arg2,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_isBlobAttr(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_isBlobAttr(argc, argv, pObj);
}



VALUE rb_s_System_def_isBlobAttr(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_isBlobAttr(argc, argv, pObj);
}



static VALUE _api_generator_System_updateBlobAttribs(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    




    int64 arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"integer" );
            return oRes.toRuby();
        }
    }









        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ISystem::updateBlobAttribs, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->updateBlobAttribs( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_updateBlobAttribs(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_updateBlobAttribs(argc, argv, pObj);
}



VALUE rb_s_System_def_updateBlobAttribs(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_updateBlobAttribs(argc, argv, pObj);
}



static VALUE _api_generator_System_getProperty(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
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
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::getProperty, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProperty( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getProperty(argc, argv, pObj);
}



VALUE rb_s_System_def_getProperty(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getProperty(argc, argv, pObj);
}



static VALUE _api_generator_System_getProperties(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    










    rho::Vector<rho::StringW> arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_array(argv[0]) )
            getStringArrayFromValue(argv[0], arg0);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"array" );
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
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::getProperties, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProperties( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_getProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getProperties(argc, argv, pObj);
}



static VALUE _api_generator_System_getAllProperties(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
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
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::getAllProperties,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getAllProperties(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getAllProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_getAllProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getAllProperties(argc, argv, pObj);
}



static VALUE _api_generator_System_setProperty(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    


    rho::StringW arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            arg0 = convertToStringW(getStringFromValue(argv[0]));

            oRes.setStringParam(getStringFromValue(argv[0]));

        }
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    nCallbackArg = 2;

    
    if ( argc == 1 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    


    rho::StringW arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
        {
            arg1 = convertToStringW(getStringFromValue(argv[1]));

        }
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError(L"Type error: argument " L"1" L" should be " L"string" );
            return oRes.toRuby();
        }
    }











        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &rho::ISystem::setProperty, arg0, arg1,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setProperty( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setProperty(argc, argv, pObj);
}



VALUE rb_s_System_def_setProperty(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setProperty(argc, argv, pObj);
}



static VALUE _api_generator_System_setProperties(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    












    rho::Hashtable<rho::StringW, rho::StringW> arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_hash(argv[0]) )
            getStringHashFromValue(argv[0], arg0);
        else if (!rho_ruby_is_NIL(argv[0]))
        {
            oRes.setArgError(L"Type error: argument " L"0" L" should be " L"hash" );
            return oRes.toRuby();
        }
    }

        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &rho::ISystem::setProperties, arg0,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setProperties( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_setProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setProperties(argc, argv, pObj);
}



static VALUE _api_generator_System_clearAllProperties(int argc, VALUE *argv, rho::ISystem* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::ISystem::clearAllProperties,  oRes );




    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->clearAllProperties(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_clearAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::ISystem* pObj =  rho::CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_clearAllProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_clearAllProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = rho::CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    rho::ISystem* pObj = rho::CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_clearAllProperties(argc, argv, pObj);
}



}