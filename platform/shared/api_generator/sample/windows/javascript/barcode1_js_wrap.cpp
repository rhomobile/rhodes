#include "..\Barcode1.h"
#include "..\..\..\Common\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho::json;

rho::String js_barcode1_enumerate(const rho::String& strID, CJSONArrayIterator& oParams)
{
    CMethodResult oRes;

    if ( !oParams.isEnd() )
    {
        oRes.setError("Barcode1::enumerate - wrong number of arguments");
        return oRes.toJSON();
    }

    CBarcode1SingletonBase::getInstance()->enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();

    for( int i = 0; i < arIDs.size(); i++ )
    {
        if ( !CBarcode1SingletonBase::getInstance()->getModules().containsKey(arIDs[i]) )
        {
            IBarcode1* pObj = CBarcode1SingletonBase::getInstance()->create(arIDs[i]);
            CBarcode1SingletonBase::getInstance()->getModules().put(arIDs[i], pObj );
        }
    }

    return oRes.toJSON();
}

rho::String js_barcode1_getProps(const rho::String& strID, CJSONArrayIterator& oParams)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    boolean bCallInUIThread = false;
    boolean bCallInThread = false;

    rho::String strObjID = strID;
    if ( strObjID.length() == 0 )
        strObjID = CBarcode1SingletonBase::getInstance()->getDefaultIDEx();

    IBarcode1* pObj = CBarcode1SingletonBase::getInstance()->getModules()[strObjID];

    CMethodResult oRes;
    if ( oParams.isEnd() )
    {
        pObj->getProps(oRes);
        return oRes.toJSON();
    }

    CJSONEntry oParam1 = oParams.getCurItem();
    oParams.next();

    if ( oParams.isEnd() )
    {
        if ( oParam1.isNull() )
        {
            pObj->getProps(oRes);
        }else if ( oParam1.isString() )
        {
            pObj->getProps(oParam1.getString(), oRes);
        }else if ( oParam1.isArray() )
        {
            rho::Vector<rho::String> ar;
            CJSONArrayIterator arParam(oParam1);
            for( ; !arParam.isEnd(); arParam.next() )
            {
                ar.addElement( arParam.getCurItem().getString() );
            }

            pObj->getProps(ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
        }
    }else
    {
        CJSONEntry oParam2 = oParams.getCurItem();

        oRes.setCallInUIThread(bCallInUIThread);
        oRes.setJSCallback( oParam2.getString() );

        oParams.next();
        if ( !oParams.isEnd() )
        {
            CJSONEntry oParam3 = oParams.getCurItem();
            if ( !oParam3.isString() )
            {
                oRes.setArgError("Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
                return oRes.toJSON();
            }

            oRes.setCallbackParam( oParam3.getString() );

        }

        rho::common::IRhoRunnable* pFunctor = 0;
        if ( oParam1.isNull() )
        {
            pFunctor = new rho::common::CInstanceClassFunctor1<IBarcode1*, void (IBarcode1::*)(CMethodResult&), CMethodResult>
                ( pObj, &IBarcode1::getProps, oRes );
        }else if ( oParam1.isString() )
        {
            oRes.setStringParam( oParam1.getString() );
            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::String&, CMethodResult&), rho::String, CMethodResult>
                ( pObj, &IBarcode1::getProps, oParam1.getString(), oRes );

        }else if ( oParam1.isArray() )
        {
            rho::Vector<rho::String> ar;
            CJSONArrayIterator arParam(oParam1);
            for( ; !arParam.isEnd(); arParam.next() )
            {
                ar.addElement( arParam.getCurItem().getString() );
            }

            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::Vector<rho::String>&, CMethodResult&), rho::Vector<rho::String>, CMethodResult>
                ( pObj, &IBarcode1::getProps, ar, oRes );
        }else
        {
            oRes.setArgError("Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toJSON();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else //call in separate thread
            CBarcode1SingletonBase::getInstance()->addCommandToQueue( pFunctor );
    }

    return oRes.toJSON();
}
