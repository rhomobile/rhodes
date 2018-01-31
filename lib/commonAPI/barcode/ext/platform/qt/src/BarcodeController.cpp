#include "BarcodeController.h"

QHash<QString, QCameraInfo> BarCodeController::camerasKeeper;
QMutex BarCodeController::localMutex;

BarCodeController::BarCodeController() : QObject(QApplication::instance())
{
    qRegisterMetaType<QList<QCameraInfo> >("QList<QCameraInfo>");
    qRegisterMetaType<rho::apiGenerator::CMethodResult>("rho::apiGenerator::CMethodResult");
}

void BarCodeController::enumerate(rho::apiGenerator::CMethodResult& oResult) {
    refresh();
    rho::Vector<rho::String> arIDs = oResult.getStringArray();
    if(!camerasKeeper.isEmpty()){
        foreach (QString value, camerasKeeper.keys()) {
            arIDs.addElement(value.toStdString());
        }
        oResult.set(arIDs);
    }
}

void BarCodeController::openDialog(rho::apiGenerator::CMethodResult &oResult, QString cameraId)
{
    if (camerasKeeper.contains(cameraId)){
#ifndef OS_SAILFISH
        BarcodeDialogBuilder * builder = new BarcodeDialogBuilder(camerasKeeper.value(cameraId),
                                                                  oResult, IExecutable::getMainWindow());
        emit builder->run();
#else

        BarcodeQMLModel::getInstance()->setOResult(oResult);
        BarcodeQMLModel::getInstance()->restart();
        QtMainWindow::getLastInstance()->openQMLDocument("BarcodePage.qml");

#endif
    }
}

void BarCodeController::refresh(){
    QMutexLocker locker(&localMutex);
    static bool refreshed = false;
    if (!refreshed) {
        refreshed = true;
        getInstance()->getCameraInfo();
    }
}

QList<QString> BarCodeController::getIDs()
{
    refresh();
    return camerasKeeper.keys();
}

void BarCodeController::getCameraInfo(){
#ifndef OS_SAILFISH
    QMetaObject::invokeMethod(GuiThreadFuncHelper::getInstance(), "availableCameras",
                              Qt::QueuedConnection, Q_ARG(QObject *, this));
    QTimer::singleShot(500, &(this->loop), SLOT(quit()));
    loop.exec();
#else
    QCameraInfo cameraInfo;
    camerasKeeper.insert("back", cameraInfo);
#endif
}

void BarCodeController::availableCameras(QList<QCameraInfo> info){
#ifndef OS_SAILFISH
    if(camerasKeeper.isEmpty()){
        int deviceCounter = 0;
        foreach (QCameraInfo cameraInfo, info) {
            camerasKeeper.insert(QString::number(++deviceCounter), cameraInfo);
        }
    }
    loop.quit();
#endif
}
