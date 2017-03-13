#ifndef CAMERAREFRESHER_H
#define CAMERAREFRESHER_H

#include "CCameraData.h"
#include <QObject>
#include <QCameraInfo>
#include <QString>
#include <QList>
#include <QEventLoop>
#include <QtGui>
#include <QApplication>
#include "../../platform/shared/qt/rhodes/guithreadfunchelper.h"

class CameraRefresher : public QObject
{
    Q_OBJECT

    explicit CameraRefresher(QObject *parent) : QObject(parent){
        qRegisterMetaType<QList<QCameraInfo> >("QList<QCameraInfo>");
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

public slots:
    void getCameraInfo(){
        qDebug () << "MARKCAM 1";
        QMetaObject::invokeMethod(GuiThreadFuncHelper::getInstance(), "availableCameras", Qt::QueuedConnection, Q_ARG(QObject *, this));
        QTimer::singleShot(100, &(this->loop), SLOT(quit()));
        loop.exec();

    }

    void availableCameras(QList<QCameraInfo> info){
        QMutexLocker locker(CCameraData::getMutex());
        if(CCameraData::isEmpty()){
            qDebug () << "MARKCAM 3";
            foreach (QCameraInfo cameraInfo, info) {
                const CCameraData * data = CCameraData::addNewCamera(cameraInfo);
            }
        }
        loop.quit();
    }
};

#endif // CAMERAREFRESHER_H
