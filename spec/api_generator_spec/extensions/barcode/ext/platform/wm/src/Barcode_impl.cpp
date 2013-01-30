#include "../generated/IBarcode.h"

class CBarcodeImpl: public CModuleBase<IBarcode>
{
public:
    CBarcodeImpl(const rho::StringW& strID): CModuleBase<IBarcode>(strID)
    {
        m_hashProps.put( L"display", L"LCD");
        m_hashProps.put( L"sound", L"Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}

};

class CBarcodeSingleton: public CModuleSingletonBase<IBarcodeSingleton>
{
    ~CBarcodeSingleton(){}
    virtual rho::StringW getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBarcodeFactory: public CBarcodeFactoryBase
{
    ~CBarcodeFactory(){}
    virtual IBarcodeSingleton* createModuleSingleton();
    virtual IBarcode* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_Barcode_extension()
{
    CBarcodeFactory::setInstance( new CBarcodeFactory() );
    Init_Barcode_API();
}

IBarcode* CBarcodeFactory::createModuleByID(const rho::StringW& strID)
{
    return new CBarcodeImpl(strID);
}

IBarcodeSingleton* CBarcodeFactory::createModuleSingleton()
{
    return new CBarcodeSingleton();
}

void CBarcodeSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::StringW> arIDs;
    arIDs.addElement(L"SC1");
    arIDs.addElement(L"SC2");

    oResult.set(arIDs);
}

rho::StringW CBarcodeSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

/*
extern "C" void Init_JSAPI_Barcode()
{
}*/