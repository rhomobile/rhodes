#include "..\Barcode1.h"
#include "..\..\..\common\ruby_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"

rho::Hashtable<rho::String,IBarcode1*> CBarcode1::m_hashBarcodes;
rho::String CBarcode1::m_strDefaultID;
rho::common::CAutoPtr<CGeneratorQueue> CBarcode1::m_pCommandQueue;


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
    }else if ( m_ResType == eStringHash )
    {
        CHoldRubyValue valHash(rho_ruby_createHash());

        for ( rho::Hashtable<rho::String, rho::String>::iterator it = m_hashStrRes.begin(); it != m_hashStrRes.end(); ++it)
        {
            addStrToHash( valHash, it->first.c_str(), it->second.c_str() );
        }

        return valHash;
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

class CRubyCallbackResult : public rho::ICallbackObject
{
    CMethodResult m_oResult;
public:
    CRubyCallbackResult(const CMethodResult& oResult) : m_oResult(oResult){}
    ~CRubyCallbackResult(){}

    virtual unsigned long getObjectValue()
    {
        m_oResult.convertStringParamToHash();
        return m_oResult.toRuby();
    }

};

void CMethodResult::callCallback()
{
    //TODO: support Ruby and JSON callbacks

    if ( m_ResType != eNone && m_strRubyCallback.length() != 0 )
    {
        //TODO: support callback param
        rho::String strResBody = "rho_callback=1&" + RHODESAPP().addCallbackObject( new CRubyCallbackResult( *this ), "body");

        //TODO: call in async mode
        getNetRequest().pushData( RHODESAPP().canonicalizeRhoUrl(m_strRubyCallback), strResBody, null );

        m_ResType = eNone;
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

extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement(szVal);
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

static VALUE barcode1_getprops(int argc, VALUE *argv, IBarcode1* pObj)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    boolean bCallInUIThread = false;
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