#include "Barcode1.h"
//#include "..\..\..\common\ruby_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"


extern "C"
{

VALUE rb_barcode1_s_enumerate(VALUE klass)
{
    MethodResultJni resJni = CBarcode1::enumerate();
    return resJni.enumerateRubyObjects();
}

VALUE rb_barcode1_s_default(VALUE klass)
{
    rho::String strDefaultID = rho_cast<rho::String>(CBarcode1::getDefaultID());
    return rho_create_object_with_id(klass, strDefaultID.c_str());
}

VALUE rb_barcode1_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_get_object_id(valObj);
    CBarcode1::setDefaultID(rho_cast<jhstring>(szID));

    return rho_ruby_get_NIL();
}

static VALUE barcode1_getprops(int argc, VALUE *argv, jhobject jhObject)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    boolean bCallInUIThread = false;
    boolean bCallInThread = false;

    //TODO: Initialize the MethodResultJni instance
    MethodResultJni result(0);

    CBarcode1 barcode(jhObject, bCallInUIThread);
    if(argc == 0)
    {
        barcode.getProps(result);
    }
    result.toRuby();

#if 0

    CMethodResult oRes;
    if ( argc == 0 )
    {
        pObj->getProps(oRes);
    }else if ( argc == 1 )
    {
        if ( rho_ruby_is_string(argv[0]) )
        {
            pObj->getProps(getStringFromValue(argv[0]), oRes);
        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);
            pObj->getProps(ar, oRes );
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
            pFunctor = new CObjCallbackFunctor1<IBarcode1*, void (IBarcode1::*)(CMethodResult&), CMethodResult>
                ( pObj, &IBarcode1::getProps, oRes );
        }else if ( rho_ruby_is_string(argv[0]) )
        {
            oRes.setStringParam(getStringFromValue(argv[0]));
            pFunctor = new CObjCallbackFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::String&, CMethodResult&), rho::String, CMethodResult>
                ( pObj, &IBarcode1::getProps, getStringFromValue(argv[0]), oRes );

        }else if ( rho_ruby_is_array(argv[0]) )
        {
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);

            pFunctor = new CObjCallbackFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::Vector<rho::String>&, CMethodResult&), rho::Vector<rho::String>, CMethodResult>
                ( pObj, &IBarcode1::getProps, ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toRuby();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else //call in separate thread
            CBarcode1::addCommandToQueue( pFunctor );

    }else
    {
        oRes.setArgError("wrong # of arguments(%d for 2)", argc );
    }

    return oRes.toRuby();
#endif
}

VALUE rb_barcode1_s_getprops(int argc, VALUE *argv)
{
    jhobject jhBarcode = CBarcode1::getDefault();
    return barcode1_getprops(argc, argv, jhBarcode);
}

VALUE rb_barcode1_getprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_get_object_id(valObj);
    jhobject jhBarcode = CBarcode1::create(rho_cast<jhstring>(szID));
    return barcode1_getprops(argc, argv, jhBarcode);
}

}
