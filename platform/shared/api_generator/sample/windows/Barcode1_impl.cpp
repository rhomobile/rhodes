#include "Barcode1.h"

class CBarcode1Impl: public CBarcode1ImplBase
{
public:
    CBarcode1Impl(const rho::String& strID): CBarcode1ImplBase(strID)
    {
        m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");
    }
};

/*static*/ IBarcode1* CBarcode1::create(const rho::String& strID)
{
    return new CBarcode1Impl(strID);
}

/*static*/ void CBarcode1::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

/*static*/ void CBarcode1::initDefaultID()
{
    CMethodResult oRes;
    CBarcode1::enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    CBarcode1::setDefaultID(arIDs[0]);
}
