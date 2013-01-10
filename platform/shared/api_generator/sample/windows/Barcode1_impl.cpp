#include "Barcode1.h"

class CBarcode1Impl: public CModuleBase<IBarcode1>
{
public:
    CBarcode1Impl(const rho::String& strID): CModuleBase<IBarcode1>(strID)
    {
        m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");
    }
};

class CBarcode1Singleton: public CBarcode1SingletonBase
{
    ~CBarcode1Singleton(){}
    virtual IBarcode1* create(const rho::String& strID);
    virtual rho::String getDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};


extern "C" void Init_Barcode1()
{
    CBarcode1Singleton::setInstance(new CBarcode1Singleton());

    Init_RubyAPI_Barcode1();
    Init_JSAPI_Barcode1();
}


IBarcode1* CBarcode1Singleton::create(const rho::String& strID)
{
    return new CBarcode1Impl(strID);
}

void CBarcode1Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CBarcode1Singleton::getDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}
