#include "Barcode1.h"

class CBarcode1Impl: public CBarcode1ImplBase
{
public:
    CBarcode1Impl(const rho::String& strID): CBarcode1ImplBase(strID)
    {
    }
};

/*static*/ IBarcode1* CBarcode1::create(const rho::String& strID)
{
    return new CBarcode1Impl(strID);
}

/*static*/ rho::Vector<rho::String> CBarcode1::enumerate()
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    return arIDs;
}

/*static*/ void CBarcode1::initDefaultID()
{
    rho::Vector<rho::String> arIDs = CBarcode1::enumerate();
    CBarcode1::setDefaultID(arIDs[0]);
}
