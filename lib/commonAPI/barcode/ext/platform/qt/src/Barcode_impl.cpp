#include "../../../shared/generated/cpp/BarcodeBase.h"

namespace rho {

using namespace apiGenerator;

class CBarcodeImpl: public CBarcodeBase
{
public:
    CBarcodeImpl(const rho::String& strID): CBarcodeBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("Win32");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }

};

class CBarcodeSingleton: public CBarcodeSingletonBase
{
    ~CBarcodeSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
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

}