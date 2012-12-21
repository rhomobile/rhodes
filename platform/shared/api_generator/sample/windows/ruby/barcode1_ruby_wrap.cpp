#include "..\Barcode1.h"
#include "..\..\..\common\ruby_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

#include "ext/rho/rhoruby.h"

rho::Hashtable<rho::String,IBarcode1*> CBarcode1::m_hashBarcodes;
rho::String CBarcode1::m_strDefaultID;

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2 >
class CObjCallbackFunctor2 : public rho::common::CInstanceClassFunctor2<OBJTYPE, FUNCTYPE, PARAMTYPE1, PARAMTYPE2>
{
public:
    CObjCallbackFunctor2( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2 ) : CInstanceClassFunctor2( obj, pFunc, param1, param2 ){}

    virtual void runObject()
    { 
        rho::common::CInstanceClassFunctor2<OBJTYPE, FUNCTYPE, PARAMTYPE1, PARAMTYPE2>::runObject();

        m_param2.callCallback();
    }
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE>
static void rho_callObjInUIThread1( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE param )
{
    rho::common::IRhoRunnable* pFunctor = new CObjCallbackFunctor2<OBJTYPE, FUNCTYPE, PARAMTYPE>( obj, pFunc, param );
    rho_wm_impl_performOnUiThread( pFunctor );
    //pFunctor->runObject();
}

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2>
static void rho_callObjInUIThread2( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2  )
{
    rho::common::IRhoRunnable* pFunctor = new CObjCallbackFunctor2<OBJTYPE, FUNCTYPE, PARAMTYPE1, PARAMTYPE2>( obj, pFunc, param1, param2 );

    rho_wm_impl_performOnUiThread( pFunctor );
    //pFunctor->runObject();
}

VALUE CMethodResult::toRuby()
{
    if ( m_ResType == eStringArray )
    {
        CHoldRubyValue valArray(rho_ruby_create_array());

        for( int i = 0; i < m_arStrRes.size(); i++ )
        {
            VALUE valObj = rho_ruby_create_string( m_arStrRes[i].c_str() );
            rho_ruby_add_to_array( valArray, valObj );
        }
        
        return valArray;
    }else if ( m_ResType == eString)
    {
        return rho_ruby_create_string(m_strRes.c_str());
    }else if ( m_ResType == eStringHash)
    {
        CHoldRubyValue valHash(rho_ruby_createHash());


        return valHash;
    }else if ( m_ResType == eArgError)
    {
        rho_ruby_raise_argerror(m_strError.c_str());
    }else if ( m_ResType == eError)
    {
        rho_ruby_raise_runtime(m_strError.c_str());
    }


    return rho_ruby_get_NIL();
}

void CMethodResult::callCallback()
{
    if ( m_ResType != eNone && m_strCallback.length() != 0 )
    {
        //TODO: support Ruby and JSON callbacks
        //TODO: convert parameters 

        //TODO: call in async mode
        //getNetRequest().pushData( m_strCallback, m_strRes, null );
    }
}

extern "C"
{

VALUE rb_barcode1_s_enumerate(VALUE klass)
{
    CMethodResult oRes;
    CBarcode1::enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();

    CHoldRubyValue valArray(rho_ruby_create_array());
    for( int i = 0; i < arIDs.size(); i++ )
    {
        if ( !CBarcode1::getBarcodes().containsKey(arIDs[i]) )
        {
            IBarcode1* pObj = CBarcode1::create(arIDs[i]);
            CBarcode1::getBarcodes().put(arIDs[i], pObj );
        }

        VALUE valObj = rho_create_object_with_id( klass, arIDs[i].c_str() );
        rho_ruby_add_to_array( valArray, valObj );
    }

    return valArray;
}

VALUE rb_barcode1_s_default(VALUE klass)
{
    rho::String strDefaultID = CBarcode1::getDefaultID();

    return rho_create_object_with_id( klass, strDefaultID.c_str() );
}

VALUE rb_barcode1_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_get_object_id( valObj );
    CBarcode1::setDefaultID(szID);

    return rho_ruby_get_NIL();
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    //TODO: getStringArrayFromValue
}

static VALUE barcode1_getprops(int argc, VALUE *argv, IBarcode1* pObj)
{
    //Sample how to call method in UI thread
/*    if ( argc >= 1 )
    {
        rho_callObjInUIThread1<IBarcode1*, rho::String (IBarcode1::*)(const rho::String&), rho::String>
            ( pObj, &IBarcode1::getProps, "Test" );
    }
*/
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    boolean bCallInUIThread = true;
    boolean bCallInThread = false;

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

        oRes.setCallback( getStringFromValue(argv[1]) );
        if ( argc >= 3 )
        {
            if ( !rho_ruby_is_string(argv[2]) )
            {
                oRes.setArgError("Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[2]) );
        }

        if ( rho_ruby_is_string(argv[0]) )
        {
            if ( bCallInUIThread )
            {
                rho_callObjInUIThread2<IBarcode1*, void (IBarcode1::*)(const rho::String&, CMethodResult&), rho::String, CMethodResult>
                    ( pObj, &IBarcode1::getProps, getStringFromValue(argv[0]), oRes );
            }else //call in separate thread
            {
                pObj->getProps( getStringFromValue(argv[0]), oRes );
            }

        }else if ( rho_ruby_is_array(argv[0]) )
        {
            //TODO: run in UI thread or in thread queue
            rho::Vector<rho::String> ar;
            getStringArrayFromValue(argv[0], ar);
            pObj->getProps( ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }

    }else
    {
        oRes.setArgError("wrong # of arguments(%d for 2)", argc );
    }

//    CHoldRubyValue valHash(rho_ruby_createHash());

//    return valHash;

    return oRes.toRuby();
}

VALUE rb_barcode1_s_getprops(int argc, VALUE *argv)
{
    rho::String strDefaultID = CBarcode1::getDefaultID();
    IBarcode1* pObj = CBarcode1::getBarcodes()[strDefaultID];

    return barcode1_getprops(argc, argv, pObj);
}

VALUE rb_barcode1_getprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_get_object_id( valObj );
    IBarcode1* pObj = CBarcode1::getBarcodes()[szID];

    return barcode1_getprops(argc, argv, pObj);
}

}