#include "../../../shared/generated/cpp/BarcodeBase.h"

namespace rho {

using namespace apiGenerator;

class CBarcodeImpl: public CBarcodeBase
{
public:
    CBarcodeImpl(const rho::String& strID): CBarcodeBase()
    {
        /*m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");*/
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
    {
        int i = 0;
    }
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
};

class CBarcodeSingleton: public CBarcodeSingletonBase
{
    ~CBarcodeSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
    virtual void myTest( const rho::String& obj1, rho::apiGenerator::CMethodResult& oResult);
};

class CBarcodeFactory: public CBarcodeFactoryBase
{
    ~CBarcodeFactory(){}
    virtual IBarcodeSingleton* createModuleSingleton();
    virtual IBarcode* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Barcode_extension()
{
    CBarcodeFactory::setInstance( new CBarcodeFactory() );
    Init_Barcode_API();
}

IBarcode* CBarcodeFactory::createModuleByID(const rho::String& strID)
{
    return new CBarcodeImpl(strID);
}

IBarcodeSingleton* CBarcodeFactory::createModuleSingleton()
{
    return new CBarcodeSingleton();
}

void CBarcodeSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CBarcodeSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}


void CBarcodeSingleton::myTest( const rho::String& obj1, rho::apiGenerator::CMethodResult& oResult)
{
    int i = 0;
}

}