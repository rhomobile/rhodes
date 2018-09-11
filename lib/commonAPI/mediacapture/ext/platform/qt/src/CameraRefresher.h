#ifndef CAMERAREFRESHER_H
#define CAMERAREFRESHER_H

#include <QObject>
#include <QCameraInfo>
#include <QString>
#include <QList>
#include <QEventLoop>
#include <QtGui>
#ifndef OS_SAILFISH
#include <QApplication>
#endif
#include "CCameraData.h"
#include "../../platform/shared/qt/rhodes/guithreadfunchelper.h"

class CameraRefresher : public QObject
{
    Q_OBJECT

    explicit CameraRefresher(QObject *parent) : QObject(parent){
        qRegisterMetaType<QList<QCameraInfo> >("QList<QCameraInfo>");
        qRegisterMetaType<rho::apiGenerator::CMethodResult>("rho::apiGenerator::CMethodResult");
    }
    QEventLoop loop;
public:
    static CameraRefresher * getInstance(QObject * parent = 0){
        static CameraRefresher instance(parent);
        return &instance;
    }
    static void refresh(){
        getInstance()->getCameraInfo();
    }
    ~CameraRefresher(){
        CCameraData::cleanAll();
    }

public slots:
    void getCameraInfo(){
        QMetaObject::invokeMethod(GuiThreadFuncHelper::getInstance(), "availableCameras",
                                  Qt::QueuedConnection, Q_ARG(QObject *, this));
        QTimer::singleShot(100, &(this->loop), SLOT(quit()));
        loop.exec();
    }

    void availableCameras(QList<QCameraInfo> info){
        QMutexLocker locker(CCameraData::getMutex());
        if(CCameraData::isEmpty()){
            foreach (QCameraInfo cameraInfo, info) {
                const CCameraData * data = CCameraData::addNewCamera(cameraInfo);
            }
        }
        loop.quit();
    }
};

#endif // CAMERAREFRESHER_H
