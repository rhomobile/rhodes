#include "BarcodeController.h"
#include "../../../shared/generated/cpp/BarcodeBase.h"
#include <QDebug>
namespace rho {

using namespace apiGenerator;

class CBarcodeImpl: public CBarcodeBase
{
    QString cameraID;
public:
    CBarcodeImpl(const rho::String& strID): CBarcodeBase(){
        cameraID = QString::fromStdString(strID);
        rho::apiGenerator::CMethodResult result;
        rho::Hashtable<rho::String, rho::String> propsHash;
        propsHash.put(barcode::PROPERTY_SCANNER_TYPE, "Camera");
        propsHash.put(barcode::PROPERTY_FRIENDLY_NAME, getFriendlyName());
        setProperties(propsHash, result);
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
        qDebug() << "GETTINGPROPSFROM " + cameraID;

        rho::Hashtable<rho::String, rho::String> propsHash = oResult.getStringHash();
        propsHash.put(barcode::PROPERTY_SCANNER_TYPE, "Camera");
        propsHash.put(barcode::PROPERTY_FRIENDLY_NAME, getFriendlyName());
        oResult.set(propsHash);
    }

    rho::String getFriendlyName(){
        if (!cameraID.isEmpty()){
            return QString("Camera "+ cameraID).toStdString();
        }else{
            return "";
        }
    }

    virtual void getFriendlyName(rho::apiGenerator::CMethodResult& oResult){
        oResult.set(getFriendlyName());
    }

    void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){
        BarCodeController::openDialog(oResult, cameraID);

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
    #ifdef OS_SAILFISH
    BarcodeQMLModel::getInstance();
    #endif
    BarCodeController::refresh();
    static IBarcodeSingleton* instance = new CBarcodeSingleton();
    return instance;
}

void CBarcodeSingleton::enumerate(CMethodResult& oResult)
{
    BarCodeController::getInstance()->enumerate(oResult);
}

rho::String CBarcodeSingleton::getInitialDefaultID()
{
    if (!BarCodeController::getIDs().isEmpty()){
        return BarCodeController::getIDs().first().toStdString();
    }else{
        return "";
    }
}

}
