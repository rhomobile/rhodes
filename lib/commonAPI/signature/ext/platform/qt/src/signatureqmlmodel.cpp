#include "signatureqmlmodel.h"

QString SignatureQMLModel::getFileName()
{
    return fileName;
}

void SignatureQMLModel::setFileName(QString value)
{
    fileName = value;
}

SignatureQMLModel::SignatureQMLModel(QObject *parent) : QObject(parent)
{
    fileName = "signature.jpg";
}

void SignatureQMLModel::done(){
    rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
    mapRes[rho::signature::HK_STATUS] = "ok";
    mapRes[rho::signature::HK_IMAGE_URI] = fileName.toStdString();
    QFileInfo image(fileName);
    mapRes["format"] = image.completeSuffix().toStdString();
    oResult.set(mapRes);
    //qDebug() << (image.exists() ? "Image exists" : "Image not exists");
}

void SignatureQMLModel::cancel(){
    rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
    mapRes[rho::signature::HK_STATUS] = "cancel";
    oResult.set(mapRes);
}

void SignatureQMLModel::setOResult(rho::apiGenerator::CMethodResult &oResult){
    this->oResult = oResult;
}
