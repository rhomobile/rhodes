#include "..\Barcode1.h"
#include "..\..\..\Common\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

using namespace rho::json;

rho::String js_barcode1_enumerate(const rho::String& strID, CJSONArrayIterator& oParams)
{
    CMethodResult oRes;

    if ( !oParams.isEnd() )
    {
        oRes.setError("Barcode1::enumerate - wrong number of arguments");
        return oRes.toJSON();
    }

    CBarcode1::enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();

    for( int i = 0; i < arIDs.size(); i++ )
    {
        if ( !CBarcode1::getBarcodes().containsKey(arIDs[i]) )
        {
            IBarcode1* pObj = CBarcode1::create(arIDs[i]);
            CBarcode1::getBarcodes().put(arIDs[i], pObj );
        }
    }

    return oRes.toJSON();
}

rho::String js_barcode1_getProps(const rho::String& strID, CJSONArrayIterator& oParams)
{
    rho::String strObjID = strID;
    if ( strObjID.length() == 0 )
        strObjID = CBarcode1::getDefaultID();

    IBarcode1* pObj = CBarcode1::getBarcodes()[strObjID];

    CMethodResult oRes;
    if ( oParams.isEnd() )
    {
        pObj->getProps(oRes);
        return oRes.toJSON();
    }

    CJSONEntry& oParam1 = oParams.getCurItem();
    oParams.next();

    if ( oParams.isEnd() )
    {
        if ( oParam1.isString() )
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
        }
    }else
    {
        CJSONEntry& oParam2 = oParams.getCurItem();
    }

    return "{}";
}
