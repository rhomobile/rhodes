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

class CBarcode1Singleton: public CModuleSingletonBase<IBarcode1Singleton>
{
    ~CBarcode1Singleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBarcode1Factory: public CBarcode1FactoryBase
{
    ~CBarcode1Factory(){}
    virtual IBarcode1Singleton* createModuleSingleton();
    virtual IBarcode1* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Barcode1()
{
    CBarcode1Factory::setInstance( new CBarcode1Factory() );

    Init_RubyAPI_Barcode1();
    Init_JSAPI_Barcode1();
}

IBarcode1* CBarcode1Factory::createModuleByID(const rho::String& strID)
{
    return new CBarcode1Impl(strID);
}

IBarcode1Singleton* CBarcode1Factory::createModuleSingleton()
{
    return new CBarcode1Singleton();
}

void CBarcode1Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CBarcode1Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}
