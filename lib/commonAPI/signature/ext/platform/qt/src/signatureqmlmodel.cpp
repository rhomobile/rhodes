#include "signatureqmlmodel.h"

QString SignatureQMLModel::getFileName()
{
    return fileName;
}

void SignatureQMLModel::setFileName(QString value)
{
    fileName = value;
}

extern "C" {
    const char* rho_native_rhopath();
}

SignatureQMLModel::SignatureQMLModel(QObject *parent) : QObject(parent)
{
    QString targetPath = QtMainWindow::getLastInstance()->getDbFilesDir() ;
    QDir tempDir(targetPath);
    tempDir.mkpath(targetPath);
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

void SignatureQMLModel::setOResult(QString filePath, rho::apiGenerator::CMethodResult &oResult){

    this->oResult = oResult;

    QString targetPath = filePath;
    if(!targetPath.endsWith(".jpg", Qt::CaseInsensitive) || !targetPath.endsWith(".png", Qt::CaseInsensitive)){
        targetPath.append(".jpg");
    }
    if(!targetPath.startsWith(QtMainWindow::getLastInstance()->getDbFilesDir(), Qt::CaseInsensitive)){
        targetPath.prepend(QtMainWindow::getLastInstance()->getDbFilesDir());
    }

    setFileName(targetPath);
}
