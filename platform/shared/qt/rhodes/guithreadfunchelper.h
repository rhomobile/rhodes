#ifndef GUITHREADFUNCHELPER_H
#define GUITHREADFUNCHELPER_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QMetaObject>
#if QT_VERSION >= 0x060000
#include <QMediaDevices>
#include <QCameraDevice>
#else
#include <QCameraInfo>
#endif
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
                QStringList names;
    #if QT_VERSION >= 0x060000
        const auto devices = QMediaDevices::videoInputs();
        for (const QCameraDevice &d : devices)
            names << d.description();
    #else
        const auto infos = QCameraInfo::availableCameras();
        for (const QCameraInfo &i : infos)
            names << i.description();
    #endif
        QMetaObject::invokeMethod(getter, "availableCameraNames",
                                  Qt::QueuedConnection,
                                  Q_ARG(QStringList, names));
    }
};

#endif // GUITHREADFUNCHELPER_H
