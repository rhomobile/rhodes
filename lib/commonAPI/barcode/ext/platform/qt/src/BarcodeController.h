#ifndef BARCODECONTROLLER_H
#define BARCODECONTROLLER_H

#include <QObject>
#include <QCameraInfo>
#include <QString>
#include <QList>
#include <QEventLoop>
#include <QtGui>
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>
#include <QHash>
#include "BarcodeDialogBuilder.h"

#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../../../../platform/shared/common/RhoStd.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "../../platform/shared/qt/rhodes/guithreadfunchelper.h"

class BarCodeController : public QObject
{
    Q_OBJECT
private:
    explicit BarCodeController();
    QEventLoop loop;
    static QMutex localMutex;
public:
    static BarCodeController * getInstance(){
        static BarCodeController instance;
        return &instance;
    }
    static void enumerate(rho::apiGenerator::CMethodResult &oResult);
    static void openDialog(rho::apiGenerator::CMethodResult &oResult, QString cameraId);
    static void refresh();
    static QHash<QString, QCameraInfo> camerasKeeper;

public slots:
    void getCameraInfo(){
        QMetaObject::invokeMethod(GuiThreadFuncHelper::getInstance(), "availableCameras",
                                  Qt::QueuedConnection, Q_ARG(QObject *, this));
        QTimer::singleShot(100, &(this->loop), SLOT(quit()));
        loop.exec();
    }

    void availableCameras(QList<QCameraInfo> info){
        if(camerasKeeper.isEmpty()){
            int deviceCounter = 0;
            foreach (QCameraInfo cameraInfo, info) {
                camerasKeeper.insert(QString::number(++deviceCounter), cameraInfo);
            }
        }
        loop.quit();
    }
};

#endif // BARCODECONTROLLER_H
