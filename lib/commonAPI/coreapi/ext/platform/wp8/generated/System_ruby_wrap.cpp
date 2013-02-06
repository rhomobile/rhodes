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



static VALUE _api_generator_System_getplatform(int argc, VALUE *argv, ISystem* pObj)

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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getplatform,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getplatform(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_getplatform(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getplatform(argc, argv, pObj);
}



VALUE rb_s_System_def_getplatform(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getplatform(argc, argv, pObj);
}



static VALUE _api_generator_System_setplatform(int argc, VALUE *argv, ISystem* pObj)

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


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &ISystem::setplatform, arg0,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setplatform( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_System_setplatform(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_setplatform(argc, argv, pObj);
}



VALUE rb_s_System_def_setplatform(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_setplatform(argc, argv, pObj);
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



static VALUE _api_generator_System_getStartParams(int argc, VALUE *argv, ISystem* pObj)

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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &ISystem::getStartParams,  oRes );




    if ( bUseCallback )
        CSystemFactoryBase::getSystemSingletonS()->addCommandToQueue( pFunctor );
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
    ISystem* pObj =  CSystemFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_System_getStartParams(argc, argv, pObj);
}



VALUE rb_s_System_def_getStartParams(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CSystemFactoryBase::getSystemSingletonS()->getDefaultID();
    ISystem* pObj = CSystemFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_System_getStartParams(argc, argv, pObj);
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