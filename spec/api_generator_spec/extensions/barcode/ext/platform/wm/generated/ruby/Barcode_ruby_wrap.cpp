#include "..\IBarcode.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_Barcode_Module();


VALUE rb_Barcode_s_default(VALUE klass)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, convertToStringA(strDefaultID).c_str() );
}

VALUE rb_Barcode_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    CBarcodeFactoryBase::getBarcodeSingletonS()->setDefaultID(convertToStringW(szID));

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



static VALUE _api_generator_Barcode_enable(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    






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

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &IBarcode::enable, arg0,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->enable( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_enable(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_enable(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_enable(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_enable(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_start(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &IBarcode::start,  oRes );



    CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );


    return oRes.toRuby();
}


VALUE rb_Barcode_start(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_start(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_start(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_start(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_stop(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &IBarcode::stop,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->stop(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_stop(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_stop(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_stop(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_stop(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_disable(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &IBarcode::disable,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->disable(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_disable(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_disable(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_disable(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_disable(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_take(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    nCallbackArg = 1;

    






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

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &IBarcode::take, arg0,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->take( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_take(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_take(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_take(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_take(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_getProperty(int argc, VALUE *argv, IBarcode* pObj)

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

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &IBarcode::getProperty, arg0,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProperty( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_getProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_getProperty(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_getProperty(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_getProperty(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_getProperties(int argc, VALUE *argv, IBarcode* pObj)

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

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &IBarcode::getProperties, arg0,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getProperties( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_getProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_getProperties(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_getProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_getProperties(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_getAllProperties(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &IBarcode::getAllProperties,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->getAllProperties(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_getAllProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_getAllProperties(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_getAllProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_getAllProperties(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_setProperty(int argc, VALUE *argv, IBarcode* pObj)

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

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor3( pObj, &IBarcode::setProperty, arg0, arg1,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setProperty( arg0, arg1,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_setProperty(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_setProperty(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_setProperty(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_setProperty(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_setProperties(int argc, VALUE *argv, IBarcode* pObj)

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

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor2( pObj, &IBarcode::setProperties, arg0,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->setProperties( arg0,  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_setProperties(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_setProperties(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_setProperties(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_setProperties(argc, argv, pObj);
}



static VALUE _api_generator_Barcode_clearProps(int argc, VALUE *argv, IBarcode* pObj)

{
    CMethodResult oRes;



    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(0) );
        return oRes.toRuby();

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor1( pObj, &IBarcode::clearProps,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        pObj->clearProps(  oRes );


    }


    return oRes.toRuby();
}


VALUE rb_Barcode_clearProps(int argc, VALUE *argv, VALUE obj)
{
    const char* szID = rho_ruby_get_object_id( obj );
    IBarcode* pObj =  CBarcodeFactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_Barcode_clearProps(argc, argv, pObj);
}



VALUE rb_s_Barcode_def_clearProps(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcodeFactoryBase::getBarcodeSingletonS()->getDefaultID();
    IBarcode* pObj = CBarcodeFactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_Barcode_clearProps(argc, argv, pObj);
}



VALUE rb_s_Barcode_enumerate(int argc, VALUE *argv)

{
    CMethodResult oRes;


    oRes.setRubyObjectClass(getRuby_Barcode_Module());


    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;




    if ( argc > nCallbackArg )
    {

        
        if ( !rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(false);
        oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
    }


    pFunctor = rho_makeInstanceClassFunctor1( CBarcodeFactoryBase::getBarcodeSingletonS(), &IBarcodeSingleton::enumerate,  oRes );




    if ( bUseCallback )
        CBarcodeFactoryBase::getBarcodeSingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;


        CBarcodeFactoryBase::getBarcodeSingletonS()->enumerate(  oRes );


    }


    return oRes.toRuby();
}






}