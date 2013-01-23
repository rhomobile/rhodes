#include "..\Barcode1.h"
#include "api_generator\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;

rho::String js_barcode1_enumerate(const rho::String& strID, CJSONArrayIterator& oParams)
{
    CMethodResult oRes;

    if ( !oParams.isEnd() )
    {
        oRes.setError(L"Barcode1::enumerate - wrong number of arguments");
        return oRes.toJSON();
    }

    CBarcode1FactoryBase::getBarcode1SingletonS()->enumerate(oRes);
    return oRes.toJSON();
}

rho::String js_barcode1_getProps(const rho::String& strID, CJSONArrayIterator& oParams)
{
    //If method has call_in_ui_thread attribute, then call method in UI thread if no return value or callback present
    //If method has call_in_thread attribute, then call method in separate thread if no return value or callback present
    //If method calles with callback, then call method in separate thread
    bool bCallInUIThread = false;
    bool bCallInThread = false;

    rho::StringW strObjID = convertToStringW(strID);
    if ( strObjID.length() == 0 )
        strObjID = CBarcode1FactoryBase::getBarcode1SingletonS()->getDefaultID();

    IBarcode1* pObj = CBarcode1FactoryBase::getInstance()->getModuleByID(strObjID);

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
            pObj->getPropsWithString(convertToStringW(oParam1.getString()), oRes);
        }else if ( oParam1.isArray() )
        {
            rho::Vector<rho::StringW> ar;
            CJSONArrayIterator arParam(oParam1);
            for( ; !arParam.isEnd(); arParam.next() )
            {
                ar.addElement( convertToStringW(arParam.getCurItem().getString()) );
            }

            pObj->getPropsWithArray(ar, oRes );
        }else
        {
            oRes.setArgError(L"Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
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
                oRes.setArgError(L"Type error: argument 3 should be String"); //see SWIG Ruby_Format_TypeError
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
            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::StringW&, CMethodResult&), rho::StringW, CMethodResult>
                ( pObj, &IBarcode1::getPropsWithString, convertToStringW(oParam1.getString()), oRes );

        }else if ( oParam1.isArray() )
        {
            rho::Vector<rho::StringW> ar;
            CJSONArrayIterator arParam(oParam1);
            for( ; !arParam.isEnd(); arParam.next() )
            {
                ar.addElement( convertToStringW(arParam.getCurItem().getString()) );
            }

            pFunctor = new rho::common::CInstanceClassFunctor2<IBarcode1*, void (IBarcode1::*)(const rho::Vector<rho::StringW>&, CMethodResult&), rho::Vector<rho::StringW>, CMethodResult>
                ( pObj, &IBarcode1::getPropsWithArray, ar, oRes );
        }else
        {
            oRes.setArgError(L"Type error: argument 1 should be String or Array"); //see SWIG Ruby_Format_TypeError
            return oRes.toJSON();
        }

        if ( bCallInUIThread )
            rho_wm_impl_performOnUiThread( pFunctor );
        else //call in separate thread
            CBarcode1FactoryBase::getBarcode1SingletonS()->addCommandToQueue( pFunctor );
    }

    return oRes.toJSON();
}
