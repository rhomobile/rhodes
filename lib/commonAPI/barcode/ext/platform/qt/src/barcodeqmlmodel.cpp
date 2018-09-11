#include "barcodeqmlmodel.h"

bool BarcodeQMLModel::getIsActive()
{
    return isActive;
}

bool BarcodeQMLModel::getIsGrabbing()
{
    return isGrabbing;
}

BarcodeQMLModel::BarcodeQMLModel(QObject *parent) : QObject(parent)
{
    decThread = new DecoderThread(this);
    isActive = false;
    lastId = 0;
}

void BarcodeQMLModel::setResult(QString value)
{
    result = value;
    emit resultChanged();
}

QString BarcodeQMLModel::getResult()
{
    return result;
}

void BarcodeQMLModel::scanningProcessMsg(){
    if (decThread->isRunning()) {return;}
    if (isActive){
        if (lastId == decThread->getLastSavedValue().id){
            if (result.size() < 12){
                setResult(result + " .");
            }else{
                setResult("");
            }
            QImage image = QtMainWindow::getView()->grabWindow();
            decThread->imageCaptured(image.copy(0, (image.height()-image.width())/2, image.width(), image.width()));
        }else{
            isActive = false;
            lastId = decThread->getLastSavedValue().id;
            setResult("Encoded format " + decThread->getLastSavedValue().format +
                      ": " + decThread->getLastSavedValue().result);

            //QSound::play(":/barcodesounds/scanner_sound.wav");

        }

        qDebug() << "scanningProcessMsg: " + result;
    }
}

void BarcodeQMLModel::restart(){
    if (!isActive){isActive = true;}
}

void BarcodeQMLModel::done(){
    isActive = false;
    QTimer::singleShot(2000, [=](){
        rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
        mapRes[rho::barcode::HK_STATUS] = "ok";
        mapRes[rho::barcode::HK_BARCODE] = decThread->getLastSavedValue().result.toStdString();
        mapRes["format"] = decThread->getLastSavedValue().format.toStdString();
        oResult.set(mapRes);
    });
}

void BarcodeQMLModel::cancel(){
    isActive = false;
    rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
    mapRes[rho::barcode::HK_STATUS] = "cancel";
    oResult.set(mapRes);
}

void BarcodeQMLModel::capture(){
    if (isActive){scanningProcessMsg();}
}

void BarcodeQMLModel::setOResult(rho::apiGenerator::CMethodResult &oResult){
    this->oResult = oResult;
}
