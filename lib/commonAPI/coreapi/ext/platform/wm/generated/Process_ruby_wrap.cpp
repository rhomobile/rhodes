#include "IProcess.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Process"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_Process_Module();



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



VALUE rb_s_Process_runApplication(int argc, VALUE *argv)

{
    rho::apiGenerator::CMethodResult oRes;


    oRes.setRubyObjectClass(getRuby_Process_Module());


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
        if ( rho_ruby_is_boolean(argv[2]) )
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


    pFunctor = rho_makeInstanceClassFunctor4( rho::system::CProcessFactoryBase::getProcessSingletonS(), &rho::system::IProcessSingleton::runApplication, arg0, arg1, arg2,  oRes );




    if ( bUseCallback )
        rho::system::CProcessFactoryBase::getProcessSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        rho::system::CProcessFactoryBase::getProcessSingletonS()->runApplication( arg0, arg1, arg2,  oRes );


    }


    return oRes.toRuby();
}






static VALUE _api_generator_Process_waitForApplication(int argc, VALUE *argv, rho::system::IProcess* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::system::IProcess::waitForApplication,  oRes );




    if ( bUseCallback )
        rho::system::CProcessFactoryBase::getProcessSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->waitForApplication(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Process_waitForApplication(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::system::IProcess* pObj =  rho::system::CProcessFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Process_waitForApplication(argc, argv, pObj);
}





static VALUE _api_generator_Process_closeHandle(int argc, VALUE *argv, rho::system::IProcess* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::system::IProcess::closeHandle,  oRes );




    if ( bUseCallback )
        rho::system::CProcessFactoryBase::getProcessSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->closeHandle(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Process_closeHandle(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::system::IProcess* pObj =  rho::system::CProcessFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Process_closeHandle(argc, argv, pObj);
}





static VALUE _api_generator_Process_getProcessExitCode(int argc, VALUE *argv, rho::system::IProcess* pObj)

{
    rho::apiGenerator::CMethodResult oRes;



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


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &rho::system::IProcess::getProcessExitCode,  oRes );




    if ( bUseCallback )
        rho::system::CProcessFactoryBase::getProcessSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProcessExitCode(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Process_getProcessExitCode(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    rho::system::IProcess* pObj =  rho::system::CProcessFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Process_getProcessExitCode(argc, argv, pObj);
}





}