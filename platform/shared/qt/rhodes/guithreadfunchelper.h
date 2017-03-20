#ifndef GUITHREADFUNCHELPER_H
#define GUITHREADFUNCHELPER_H

#include <QObject>
#include <QCameraInfo>
#include <QDebug>
#include <QMetaObject>

class GuiThreadFuncHelper : public QObject
{
    Q_OBJECT

    explicit GuiThreadFuncHelper(QObject *parent = 0):QObject(parent){}
public:
    static GuiThreadFuncHelper * getInstance(QObject *parent = 0){
        static GuiThreadFuncHelper instance(parent);
        return &instance;
    }

signals:

public slots:
    void availableCameras(QObject * getter){
        QMetaObject::invokeMethod(getter, "availableCameras", Qt::QueuedConnection,
        Q_ARG(QList<QCameraInfo>, QCameraInfo::availableCameras()));
    }
};

#endif // GUITHREADFUNCHELPER_H
