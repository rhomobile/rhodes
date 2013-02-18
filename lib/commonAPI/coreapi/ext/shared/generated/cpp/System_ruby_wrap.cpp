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



extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement( szVal );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

extern "C" static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::String, rho::String>& hash = *((rho::Hashtable<rho::String, rho::String>*)(par));
    hash.put( szName, szVal );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::String, rho::String>& res)
{
    rho_ruby_enum_strhash(val, hash_eachstr, &res);
}


VALUE rb_s_System_getPlatform(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getPlatform,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getPlatform(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasCamera(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasCamera,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasCamera(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getScreenWidth(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getScreenWidth,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getScreenWidth(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getScreenHeight(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getScreenHeight,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getScreenHeight(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getRealScreenWidth(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getRealScreenWidth,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getRealScreenWidth(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getRealScreenHeight(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getRealScreenHeight,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getRealScreenHeight(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getScreenOrientation(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getScreenOrientation,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getScreenOrientation(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getPpiX(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getPpiX,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getPpiX(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getPpiY(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getPpiY,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getPpiY(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getPhoneNumber(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getPhoneNumber,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getPhoneNumber(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getDeviceOwnerEmail(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getDeviceOwnerEmail,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getDeviceOwnerEmail(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getDeviceOwnerName(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getDeviceOwnerName,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getDeviceOwnerName(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getDevicePushId(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getDevicePushId,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getDevicePushId(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getPhoneId(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getPhoneId,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getPhoneId(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getDeviceName(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getDeviceName,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getDeviceName(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getOsVersion(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getOsVersion,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getOsVersion(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getLocale(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getLocale,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getLocale(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getCountry(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getCountry,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getCountry(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getIsEmulator(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getIsEmulator,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getIsEmulator(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasCalendar(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasCalendar,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasCalendar(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getIsMotorolaDevice(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getIsMotorolaDevice,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getIsMotorolaDevice(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getOemInfo(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getOemInfo,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getOemInfo(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getUuid(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getUuid,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getUuid(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getApplicationIconBadge(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getApplicationIconBadge,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getApplicationIconBadge(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setApplicationIconBadge(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setApplicationIconBadge, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setApplicationIconBadge( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHttpProxyURI(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHttpProxyURI,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHttpProxyURI(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setHttpProxyURI(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setHttpProxyURI, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setHttpProxyURI( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getLockWindowSize(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getLockWindowSize,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getLockWindowSize(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setLockWindowSize(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setLockWindowSize, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setLockWindowSize( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getShowKeyboard(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getShowKeyboard,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getShowKeyboard(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setShowKeyboard(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setShowKeyboard, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setShowKeyboard( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getFullScreen(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getFullScreen,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getFullScreen(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setFullScreen(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setFullScreen, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setFullScreen( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getLocalServerPort(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getLocalServerPort,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getLocalServerPort(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setLocalServerPort(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setLocalServerPort, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setLocalServerPort( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getFreeServerPort(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getFreeServerPort,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getFreeServerPort(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getScreenAutoRotate(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getScreenAutoRotate,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getScreenAutoRotate(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setScreenAutoRotate(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setScreenAutoRotate, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setScreenAutoRotate( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasTouchscreen(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasTouchscreen,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasTouchscreen(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getSecurityTokenNotPassed(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getSecurityTokenNotPassed,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getSecurityTokenNotPassed(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getWebviewFramework(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getWebviewFramework,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getWebviewFramework(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getScreenSleeping(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getScreenSleeping,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getScreenSleeping(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setScreenSleeping(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setScreenSleeping, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setScreenSleeping( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasNetwork(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasNetwork,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasNetwork(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasWifiNetwork(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasWifiNetwork,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasWifiNetwork(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasCellNetwork(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasCellNetwork,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasCellNetwork(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getHasSqlite(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getHasSqlite,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getHasSqlite(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_applicationInstall(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::applicationInstall, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->applicationInstall( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_isApplicationInstalled(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::isApplicationInstalled, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->isApplicationInstalled( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_applicationUninstall(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::applicationUninstall, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->applicationUninstall( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getStartParams(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getStartParams,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getStartParams(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_openUrl(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::openUrl, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->openUrl( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_unzipFile(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::unzipFile, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->unzipFile( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_zipFile(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 3;
    rho::String arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = getStringFromValue(argv[2]);

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor4( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::zipFile, arg0, arg1, arg2,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->zipFile( arg0, arg1, arg2,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_zipFiles(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
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
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
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

    nCallbackArg = 3;
    if ( argc == 2 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
        return oRes.toRuby();
    }
    rho::Vector<rho::String> arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_array(argv[2]) )
            getStringArrayFromValue(argv[2], arg2);
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "array" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 4;
    rho::String arg3;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_string(argv[3]) )
        {
            arg3 = getStringFromValue(argv[3]);

        }
        else if (!rho_ruby_is_NIL(argv[3]))
        {
            oRes.setArgError("Type error: argument " "3" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor5( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::zipFiles, arg0, arg1, arg2, arg3,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->zipFiles( arg0, arg1, arg2, arg3,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setRegistrySetting(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(5) );
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

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(5) );
        return oRes.toRuby();
    }
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 3;
    if ( argc == 2 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(5) );
        return oRes.toRuby();
    }
    rho::String arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = getStringFromValue(argv[2]);

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "string" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 4;
    if ( argc == 3 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(5) );
        return oRes.toRuby();
    }
    rho::String arg3;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_string(argv[3]) )
        {
            arg3 = getStringFromValue(argv[3]);

        }
        else if (!rho_ruby_is_NIL(argv[3]))
        {
            oRes.setArgError("Type error: argument " "3" " should be " "string" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 5;
    if ( argc == 4 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(5) );
        return oRes.toRuby();
    }
    rho::String arg4;
    if ( argc > 4 )
    {
        if ( rho_ruby_is_string(argv[4]) )
        {
            arg4 = getStringFromValue(argv[4]);

        }
        else if (!rho_ruby_is_NIL(argv[4]))
        {
            oRes.setArgError("Type error: argument " "4" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(5) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor6( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setRegistrySetting, arg0, arg1, arg2, arg3, arg4,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setRegistrySetting( arg0, arg1, arg2, arg3, arg4,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getRegistrySetting(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 3;
    if ( argc == 2 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    rho::String arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = getStringFromValue(argv[2]);

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor4( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getRegistrySetting, arg0, arg1, arg2,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getRegistrySetting( arg0, arg1, arg2,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setWindowFrame(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
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

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
        return oRes.toRuby();
    }
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 3;
    if ( argc == 2 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
        return oRes.toRuby();
    }
    int arg2;
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
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
        return oRes.toRuby();
    }
    int arg3;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_integer(argv[3]) )
            arg3 = rho_ruby_get_int(argv[3]);
        else if (!rho_ruby_is_NIL(argv[3]))
        {
            oRes.setArgError("Type error: argument " "3" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(4) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor5( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setWindowFrame, arg0, arg1, arg2, arg3,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setWindowFrame( arg0, arg1, arg2, arg3,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setWindowPosition(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
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

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setWindowPosition, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setWindowPosition( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setWindowSize(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
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

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setWindowSize, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setWindowSize( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_bringToFront(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::bringToFront,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->bringToFront(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_replaceCurrentBundle(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    rho::Hashtable<rho::String, rho::String> arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_hash(argv[1]) )
            getStringHashFromValue(argv[1], arg1);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "hash" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::replaceCurrentBundle, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->replaceCurrentBundle( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_deleteFolder(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::deleteFolder, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->deleteFolder( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setDoNotBackupAttribute(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setDoNotBackupAttribute, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setDoNotBackupAttribute( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_isBlobAttr(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    nCallbackArg = 3;
    if ( argc == 2 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    rho::String arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = getStringFromValue(argv[2]);

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor4( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::isBlobAttr, arg0, arg1, arg2,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->isBlobAttr( arg0, arg1, arg2,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_updateBlobAttribs(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::updateBlobAttribs, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->updateBlobAttribs( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_get_locale(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::get_locale,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->get_locale(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_set_locale(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::set_locale, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->set_locale( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setPushNotification(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 3;
    rho::String arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = getStringFromValue(argv[2]);

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor4( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setPushNotification, arg0, arg1, arg2,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setPushNotification( arg0, arg1, arg2,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setScreenRotationNotification(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setScreenRotationNotification, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setScreenRotationNotification( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_exit(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::exit,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->exit(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_set_sleeping(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::set_sleeping, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->set_sleeping( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_startTimer(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 2;
    if ( argc == 1 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 3;
    rho::String arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
        {
            arg2 = getStringFromValue(argv[2]);

        }
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "string" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor4( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::startTimer, arg0, arg1, arg2,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->startTimer( arg0, arg1, arg2,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_stopTimer(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::stopTimer, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->stopTimer( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setNetworkStatusNotify(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_integer(argv[1]) )
            arg1 = rho_ruby_get_int(argv[1]);
        else if (!rho_ruby_is_NIL(argv[1]))
        {
            oRes.setArgError("Type error: argument " "1" " should be " "integer" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(2) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setNetworkStatusNotify, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setNetworkStatusNotify( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_clearNetworkStatusNotify(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::clearNetworkStatusNotify,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->clearNetworkStatusNotify(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_set_http_proxy_url(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::set_http_proxy_url, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->set_http_proxy_url( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_unset_http_proxy(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::unset_http_proxy,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->unset_http_proxy(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_set_application_icon_badge(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::set_application_icon_badge, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->set_application_icon_badge( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_runApplication(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    nCallbackArg = 1;
    if ( argc == 0 )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
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

    nCallbackArg = 3;
    bool arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_boolean(argv[2]) )
            arg2 = rho_ruby_get_bool(argv[2]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError("Type error: argument " "2" " should be " "boolean" );
            return oRes.toRuby();
        }
    }

    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(3) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor4( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::runApplication, arg0, arg1, arg2,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->runApplication( arg0, arg1, arg2,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getProperty(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getProperty, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getProperty( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getProperties(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getProperties, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getProperties( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_getAllProperties(int argc, VALUE *argv)
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
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::getAllProperties,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->getAllProperties(  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setProperty(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor3( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setProperty, arg0, arg1,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setProperty( arg0, arg1,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_setProperties(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
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
    pFunctor = rho_makeInstanceClassFunctor2( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::setProperties, arg0,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->setProperties( arg0,  oRes );

    }
    return oRes.toRuby();
}





VALUE rb_s_System_clearAllProperties(int argc, VALUE *argv)
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;
    if ( argc > nCallbackArg )
    {

        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(0) );
        return oRes.toRuby();
    }
    pFunctor = rho_makeInstanceClassFunctor1( rho::CSystemFactoryBase::getSystemSingletonS(), &rho::ISystemSingleton::clearAllProperties,  oRes );

    if ( bUseCallback )
        rho::CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else 
    {
        delete pFunctor;


        rho::CSystemFactoryBase::getSystemSingletonS()->clearAllProperties(  oRes );

    }
    return oRes.toRuby();
}






}