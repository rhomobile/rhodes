#include "../../../shared/generated/cpp/BarcodeBase.h"

namespace rho {

using namespace apiGenerator;

class CBarcodeImpl: public CBarcodeBase
{
public:
    CBarcodeImpl(const rho::String& strID): CBarcodeBase(){

    }

    void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){

    }

    void registerBluetoothStatus(rho::apiGenerator::CMethodResult& oResult){

    }

    void start(rho::apiGenerator::CMethodResult& oResult){

    }

    void stop(rho::apiGenerator::CMethodResult& oResult){

    }

    void disable(rho::apiGenerator::CMethodResult& oResult){

    }

    void barcode_recognize( const rho::String& imageFilePath, rho::apiGenerator::CMethodResult& oResult){

    }

    void getSupportedProperties(rho::apiGenerator::CMethodResult& oResult){

    }

    void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){


    }

    void take_barcode( const rho::String& rubyCallbackURL,  const rho::Hashtable<rho::String,
                               rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){

    }

    void commandRemoteScanner( const rho::String& command, rho::apiGenerator::CMethodResult& oResult){


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
