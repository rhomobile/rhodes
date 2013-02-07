#include "ISystem.h"

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
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, convertToStringA(strDefaultID).c_str() );
}

VALUE rb_System_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    CSystemFactoryBase::getSystemSingletonS()->setDefaultID(convertToStringW(szID));

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



static VALUE _api_generator_System_getPlatform(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPlatform,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPlatform(argc, argv, pObj);
}



VALUE rb_s_System_def_getPlatform(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPlatform(argc, argv, pObj);
}



static VALUE _api_generator_System_getHasCamera(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getHasCamera,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHasCamera(argc, argv, pObj);
}



VALUE rb_s_System_def_getHasCamera(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHasCamera(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenWidth(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getScreenWidth,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenWidth(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenWidth(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenWidth(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenHeight(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getScreenHeight,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenHeight(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenHeight(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenHeight(argc, argv, pObj);
}



static VALUE _api_generator_System_getRealScreenWidth(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getRealScreenWidth,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getRealScreenWidth(argc, argv, pObj);
}



VALUE rb_s_System_def_getRealScreenWidth(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getRealScreenWidth(argc, argv, pObj);
}



static VALUE _api_generator_System_getRealScreenHeight(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getRealScreenHeight,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getRealScreenHeight(argc, argv, pObj);
}



VALUE rb_s_System_def_getRealScreenHeight(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getRealScreenHeight(argc, argv, pObj);
}



static VALUE _api_generator_System_getScreenOrientation(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getScreenOrientation,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getScreenOrientation(argc, argv, pObj);
}



VALUE rb_s_System_def_getScreenOrientation(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getScreenOrientation(argc, argv, pObj);
}



static VALUE _api_generator_System_getPpiX(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPpiX,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPpiX(argc, argv, pObj);
}



VALUE rb_s_System_def_getPpiX(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPpiX(argc, argv, pObj);
}



static VALUE _api_generator_System_getPpiY(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPpiY,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPpiY(argc, argv, pObj);
}



VALUE rb_s_System_def_getPpiY(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPpiY(argc, argv, pObj);
}



static VALUE _api_generator_System_getPhoneNumber(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPhoneNumber,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPhoneNumber(argc, argv, pObj);
}



VALUE rb_s_System_def_getPhoneNumber(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPhoneNumber(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceOwnerEmail(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceOwnerEmail,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceOwnerEmail(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceOwnerEmail(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceOwnerEmail(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceOwnerName(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceOwnerName,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceOwnerName(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceOwnerName(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceOwnerName(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceId(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceId,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getDeviceId(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getDeviceId(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceId(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceId(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceId(argc, argv, pObj);
}



static VALUE _api_generator_System_getPhoneId(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getPhoneId,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getPhoneId(argc, argv, pObj);
}



VALUE rb_s_System_def_getPhoneId(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getPhoneId(argc, argv, pObj);
}



static VALUE _api_generator_System_getDeviceName(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getDeviceName,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getDeviceName(argc, argv, pObj);
}



VALUE rb_s_System_def_getDeviceName(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getDeviceName(argc, argv, pObj);
}



static VALUE _api_generator_System_getOsVersion(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getOsVersion,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getOsVersion(argc, argv, pObj);
}



VALUE rb_s_System_def_getOsVersion(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getOsVersion(argc, argv, pObj);
}



static VALUE _api_generator_System_getLocale(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getLocale,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getLocale(argc, argv, pObj);
}



VALUE rb_s_System_def_getLocale(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getLocale(argc, argv, pObj);
}



static VALUE _api_generator_System_getCountry(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getCountry,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getCountry(argc, argv, pObj);
}



VALUE rb_s_System_def_getCountry(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getCountry(argc, argv, pObj);
}



static VALUE _api_generator_System_getIsEmulator(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getIsEmulator,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getIsEmulator(argc, argv, pObj);
}



VALUE rb_s_System_def_getIsEmulator(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getIsEmulator(argc, argv, pObj);
}



static VALUE _api_generator_System_getHasCalendar(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getHasCalendar,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHasCalendar(argc, argv, pObj);
}



VALUE rb_s_System_def_getHasCalendar(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHasCalendar(argc, argv, pObj);
}



static VALUE _api_generator_System_getIsMotorolaDevice(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getIsMotorolaDevice,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getIsMotorolaDevice(argc, argv, pObj);
}



VALUE rb_s_System_def_getIsMotorolaDevice(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getIsMotorolaDevice(argc, argv, pObj);
}



static VALUE _api_generator_System_getOemInfo(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getOemInfo,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getOemInfo(argc, argv, pObj);
}



VALUE rb_s_System_def_getOemInfo(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getOemInfo(argc, argv, pObj);
}



static VALUE _api_generator_System_getUuid(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getUuid,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getUuid(argc, argv, pObj);
}



VALUE rb_s_System_def_getUuid(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getUuid(argc, argv, pObj);
}



static VALUE _api_generator_System_getApplicationIconBadge(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getApplicationIconBadge,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getApplicationIconBadge(argc, argv, pObj);
}



VALUE rb_s_System_def_getApplicationIconBadge(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getApplicationIconBadge(argc, argv, pObj);
}



static VALUE _api_generator_System_setApplicationIconBadge(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(1) );
        return oRes.toRuby();
    }
    




    int arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setApplicationIconBadge, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setApplicationIconBadge(argc, argv, pObj);
}



VALUE rb_s_System_def_setApplicationIconBadge(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setApplicationIconBadge(argc, argv, pObj);
}



static VALUE _api_generator_System_getHttpProxyURI(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getHttpProxyURI,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getHttpProxyURI(argc, argv, pObj);
}



VALUE rb_s_System_def_getHttpProxyURI(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getHttpProxyURI(argc, argv, pObj);
}



static VALUE _api_generator_System_setHttpProxyURI(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setHttpProxyURI, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setHttpProxyURI(argc, argv, pObj);
}



VALUE rb_s_System_def_setHttpProxyURI(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setHttpProxyURI(argc, argv, pObj);
}



static VALUE _api_generator_System_getLockWindowSize(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getLockWindowSize,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getLockWindowSize(argc, argv, pObj);
}



VALUE rb_s_System_def_getLockWindowSize(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getLockWindowSize(argc, argv, pObj);
}



static VALUE _api_generator_System_setLockWindowSize(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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
        if ( rho_ruby_is_string(argv[0]) )
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setLockWindowSize, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setLockWindowSize(argc, argv, pObj);
}



VALUE rb_s_System_def_setLockWindowSize(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setLockWindowSize(argc, argv, pObj);
}



static VALUE _api_generator_System_getShowKeyboard(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getShowKeyboard,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getShowKeyboard(argc, argv, pObj);
}



VALUE rb_s_System_def_getShowKeyboard(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getShowKeyboard(argc, argv, pObj);
}



static VALUE _api_generator_System_setShowKeyboard(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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
        if ( rho_ruby_is_string(argv[0]) )
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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setShowKeyboard, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setShowKeyboard(argc, argv, pObj);
}



VALUE rb_s_System_def_setShowKeyboard(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setShowKeyboard(argc, argv, pObj);
}



static VALUE _api_generator_System_applicationInstall(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::applicationInstall, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_applicationInstall(argc, argv, pObj);
}



VALUE rb_s_System_def_applicationInstall(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_applicationInstall(argc, argv, pObj);
}



static VALUE _api_generator_System_isApplicationInstalled(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::isApplicationInstalled, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_isApplicationInstalled(argc, argv, pObj);
}



VALUE rb_s_System_def_isApplicationInstalled(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_isApplicationInstalled(argc, argv, pObj);
}



static VALUE _api_generator_System_applicationUninstall(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::applicationUninstall, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_applicationUninstall(argc, argv, pObj);
}



VALUE rb_s_System_def_applicationUninstall(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_applicationUninstall(argc, argv, pObj);
}



static VALUE _api_generator_System_openUrl(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::openUrl, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_openUrl(argc, argv, pObj);
}



VALUE rb_s_System_def_openUrl(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_openUrl(argc, argv, pObj);
}



static VALUE _api_generator_System_runApplication(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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

    






    bool arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
            arg2 = rho_ruby_get_bool(argv[2]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[2]))
        {
            oRes.setArgError(L"Type error: argument " L"2" L" should be " L"boolean" );
            return oRes.toRuby();
        }
    }







        




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(3) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &ISystem::runApplication, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->runApplication( arg0, arg1, arg2,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_runApplication(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_runApplication(argc, argv, pObj);
}



VALUE rb_s_System_def_runApplication(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_runApplication(argc, argv, pObj);
}



static VALUE _api_generator_System_unzipFile(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::unzipFile, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->unzipFile( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_unzipFile(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_unzipFile(argc, argv, pObj);
}



VALUE rb_s_System_def_unzipFile(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_unzipFile(argc, argv, pObj);
}



static VALUE _api_generator_System_zipFile(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor4( pObj, &ISystem::zipFile, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_zipFile(argc, argv, pObj);
}



VALUE rb_s_System_def_zipFile(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_zipFile(argc, argv, pObj);
}



static VALUE _api_generator_System_zipFiles(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor5( pObj, &ISystem::zipFiles, arg0, arg1, arg2, arg3,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_zipFiles(argc, argv, pObj);
}



VALUE rb_s_System_def_zipFiles(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_zipFiles(argc, argv, pObj);
}



static VALUE _api_generator_System_setRegistrySetting(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &ISystem::setRegistrySetting, arg0, arg1,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setRegistrySetting(argc, argv, pObj);
}



VALUE rb_s_System_def_setRegistrySetting(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setRegistrySetting(argc, argv, pObj);
}



static VALUE _api_generator_System_getRegistrySetting(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::getRegistrySetting, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getRegistrySetting(argc, argv, pObj);
}



VALUE rb_s_System_def_getRegistrySetting(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getRegistrySetting(argc, argv, pObj);
}



static VALUE _api_generator_System_setWindowFrame(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(4) );
        return oRes.toRuby();
    }
    




    int arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
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
    




    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
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
    




    int arg2;
    if ( argc > 2 )
    {
        if ( rho_ruby_is_string(argv[2]) )
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
    




    int arg3;
    if ( argc > 3 )
    {
        if ( rho_ruby_is_string(argv[3]) )
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


    pFunctor = rho_makeInstanceClassFunctor5( pObj, &ISystem::setWindowFrame, arg0, arg1, arg2, arg3,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setWindowFrame(argc, argv, pObj);
}



VALUE rb_s_System_def_setWindowFrame(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setWindowFrame(argc, argv, pObj);
}



static VALUE _api_generator_System_setWindowPosition(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    
    if ( argc == 0 )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(2) );
        return oRes.toRuby();
    }
    




    int arg0;
    if ( argc > 0 )
    {
        if ( rho_ruby_is_string(argv[0]) )
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
    




    int arg1;
    if ( argc > 1 )
    {
        if ( rho_ruby_is_string(argv[1]) )
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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &ISystem::setWindowPosition, arg0, arg1,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setWindowPosition(argc, argv, pObj);
}



VALUE rb_s_System_def_setWindowPosition(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setWindowPosition(argc, argv, pObj);
}



static VALUE _api_generator_System_getProperty(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::getProperty, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getProperty(argc, argv, pObj);
}



VALUE rb_s_System_def_getProperty(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getProperty(argc, argv, pObj);
}



static VALUE _api_generator_System_getProperties(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::getProperties, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_getProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getProperties(argc, argv, pObj);
}



static VALUE _api_generator_System_getAllProperties(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getAllProperties,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getAllProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_getAllProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getAllProperties(argc, argv, pObj);
}



static VALUE _api_generator_System_setProperty(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &ISystem::setProperty, arg0, arg1,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setProperty(argc, argv, pObj);
}



VALUE rb_s_System_def_setProperty(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setProperty(argc, argv, pObj);
}



static VALUE _api_generator_System_setProperties(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setProperties, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_setProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setProperties(argc, argv, pObj);
}



static VALUE _api_generator_System_clearAllProperties(int argc, VALUE *argv, ISystem* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();


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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::clearAllProperties,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_clearAllProperties(argc, argv, pObj);
}



VALUE rb_s_System_def_clearAllProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_clearAllProperties(argc, argv, pObj);
}



}