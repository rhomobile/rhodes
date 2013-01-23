#include "..\Barcode1.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

VALUE rb_barcode1_s_enumerate(VALUE klass)
{
    CMethodResult oRes;
    CBarcode1FactoryBase::getBarcode1SingletonS()->enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();

    CHoldRubyValue valArray(rho_ruby_create_array());
    for( int i = 0; i < (int)arIDs.size(); i++ )
    {
        VALUE valObj = rho_ruby_create_object_with_id( klass, convertToStringA(arIDs[i]).c_str() );
        rho_ruby_add_to_array( valArray, valObj );
    }

    return valArray;
}

VALUE rb_barcode1_s_default(VALUE klass)
{
    rho::StringW strDefaultID = CBarcode1FactoryBase::getBarcode1SingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, convertToStringA(strDefaultID).c_str() );
}

VALUE rb_barcode1_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    CBarcode1FactoryBase::getBarcode1SingletonS()->setDefaultID(convertToStringW(szID));

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

static VALUE barcode1_getprops(int argc, VALUE *argv, IBarcode1* pObj)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    bool bCallInUIThread = false;
    bool bCallInSeparateThread = true;

    CMethodResult oRes;
    if ( argc == 0 )
    {
        pObj->getProps(oRes);
    }else if ( argc == 1 )
    {
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pObj->getProps(oRes);
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            pObj->getPropsWithString( convertToStringW(getStringFromValue(argv[0])), oRes);
        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::StringW> ar;
            getStringArrayFromValue(argv[0], ar);
            pObj->getPropsWithArray(ar, oRes );
        }else
        {
            oRes.setArgError(L"Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }else if ( argc >= 2 )
    {
        if ( !rho_ruby_is_string(argv[1]) )
        {
            oRes.setArgError(L"Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(bCallInUIThread);
        oRes.setRubyCallback( getStringFromValue(argv[1]) );
        if ( argc >= 3 )
        {
            if ( !rho_ruby_is_string(argv[2]) )
            {
                oRes.setArgError(L"Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[2]) );
        }

        rho::common::IRhoRunnable* pFunctor = 0;
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pFunctor = new rho::common::CInstanceClassFunctor1<IBarcode1*, void (IBarcode1::*)(CMethodResult&), CMethodResult>
                ( pObj, &IBarcode1::getProps, oRes );
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            oRes.setStringParam(getStringFromValue(argv[0]));
            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::StringW&, CMethodResult&), rho::StringW, CMethodResult>
                ( pObj, &IBarcode1::getPropsWithString, convertToStringW(getStringFromValue(argv[0])), oRes );

        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::StringW> ar;
            getStringArrayFromValue(argv[0], ar);

            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::Vector<rho::StringW>&, CMethodResult&), rho::Vector<rho::StringW>, CMethodResult>
                ( pObj, &IBarcode1::getPropsWithArray, ar, oRes );
        }else
        {
            oRes.setArgError(L"Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else if ( bCallInSeparateThread )
            CBarcode1FactoryBase::getBarcode1SingletonS()->callCommandInThread( pFunctor );
        else
            CBarcode1FactoryBase::getBarcode1SingletonS()->addCommandToQueue( pFunctor );


    }else
    {
        oRes.setArgError(L"wrong # of arguments(%d for 2)", argc );
    }

    return oRes.toRuby();
}

VALUE rb_barcode1_s_getprops(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcode1FactoryBase::getBarcode1SingletonS()->getDefaultID();
    IBarcode1* pObj = CBarcode1FactoryBase::getInstance()->getModuleByID(strDefaultID);

    return barcode1_getprops(argc, argv, pObj);
}

static VALUE barcode1_setprops(int argc, VALUE *argv, IBarcode1* pObj)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    bool bCallInUIThread = false;
    bool bCallInThread = false;
    //TODO:barcode1_setprops

    CMethodResult oRes;
/*
    if ( argc == 0 )
    {
        pObj->getProps(oRes);
    }else if ( argc == 1 )
    {
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pObj->getProps(oRes);
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            pObj->getPropsWithString(getStringFromValue(argv[0]), oRes);
        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);
            pObj->getPropsWithArray(ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }else if ( argc >= 2 )
    {
        if ( !rho_ruby_is_string(argv[1]) )
        {
            oRes.setArgError("Type error: argument 2 should be String"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(bCallInUIThread);
        oRes.setRubyCallback( getStringFromValue(argv[1]) );
        if ( argc >= 3 )
        {
            if ( !rho_ruby_is_string(argv[2]) )
            {
                oRes.setArgError("Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[2]) );
        }

        rho::common::IRhoRunnable* pFunctor = 0;
        if ( rho_ruby_is_NIL(argv[0]) )
        {
            pFunctor = new rho::common::CInstanceClassFunctor1<IBarcode1*, void (IBarcode1::*)(CMethodResult&), CMethodResult>
                ( pObj, &IBarcode1::getProps, oRes );
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            oRes.setStringParam(getStringFromValue(argv[0]));
            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::String&, CMethodResult&), rho::String, CMethodResult>
                ( pObj, &IBarcode1::getPropsWithString, getStringFromValue(argv[0]), oRes );

        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);

            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::Vector<rho::String>&, CMethodResult&), rho::Vector<rho::String>, CMethodResult>
                ( pObj, &IBarcode1::getPropsWithArray, ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else //call in separate thread
            CBarcode1FactoryBase::getBarcode1SingletonS()->addCommandToQueue( pFunctor );

    }else
    {
        oRes.setArgError("wrong # of arguments(%d for 2)", argc );
    }
*/
    return oRes.toRuby();
}

VALUE rb_barcode1_getprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    IBarcode1* pObj =  CBarcode1FactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return barcode1_getprops(argc, argv, pObj);
}

VALUE rb_barcode1_s_setprops(int argc, VALUE *argv)
{
    rho::StringW strDefaultID = CBarcode1FactoryBase::getBarcode1SingletonS()->getDefaultID();
    IBarcode1* pObj = CBarcode1FactoryBase::getInstance()->getModuleByID(strDefaultID);

    return barcode1_setprops(argc, argv, pObj);
}

VALUE rb_barcode1_setprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    IBarcode1* pObj =  CBarcode1FactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return barcode1_setprops(argc, argv, pObj);
}

}