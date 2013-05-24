#include "../generated/Barcode1.h"

class CBarcode1Impl: public CModuleBase<IBarcode1>
{
public:
    CBarcode1Impl(const rho::StringW& strID): CModuleBase<IBarcode1>(strID)
    {
        /*m_hashProps.put( L"display", L"LCD");
        m_hashProps.put( L"sound", L"Dolby");*/
    }
};

class CBarcode1Singleton: public CModuleSingletonBase<IBarcode1Singleton>
{
    ~CBarcode1Singleton(){}
    virtual rho::StringW getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBarcode1Factory: public CBarcode1FactoryBase
{
    ~CBarcode1Factory(){}
    virtual IBarcode1Singleton* createModuleSingleton();
    virtual IBarcode1* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_Barcode1()
{
    CBarcode1Factory::setInstance( new CBarcode1Factory() );
    Init_Barcode1_API();
}

IBarcode1* CBarcode1Factory::createModuleByID(const rho::StringW& strID)
{
    return new CBarcode1Impl(strID);
}

IBarcode1Singleton* CBarcode1Factory::createModuleSingleton()
{
    return new CBarcode1Singleton();
}

void CBarcode1Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::StringW> arIDs;
    arIDs.addElement(L"SC1");
    arIDs.addElement(L"SC2");

    oResult.set(arIDs);
}

rho::StringW CBarcode1Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}
