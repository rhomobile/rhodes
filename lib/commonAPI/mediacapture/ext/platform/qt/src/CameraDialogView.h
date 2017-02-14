#ifndef CAMERADIALOGVIEW_H
#define CAMERADIALOGVIEW_H

#include <QDialog>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#include <QVideoWidget>
#include <QEventLoop>
#include <QThread>
#include "../../platform/shared/qt/rhodes/QtMainWindow.h"
#include "../../platform/shared/qt/rhodes/impl/MainWindowImpl.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"

class CameraDialogView : public QDialog
{
    Q_OBJECT
public:
    explicit CameraDialogView(QCamera * camera, QWidget *parent) : QDialog(parent){
        cameraObject = camera;

        setMinimumHeight(200);
        setMinimumWidth(300);

        QVBoxLayout * vblay = new QVBoxLayout(this);
        videoWidget = new QVideoWidget(this);
        vblay->addWidget(videoWidget);

        cameraObject->setCaptureMode(QCamera::CaptureViewfinder);
        cameraObject->setViewfinder(videoWidget);

        connect(this, SIGNAL(rejected()), this, SLOT(buttonReject()));

        cameraObject->start();
    }

private:
    QCamera * cameraObject;
    QVideoWidget * videoWidget;

signals:

public slots:
};

#endif // CAMERADIALOGVIEW_H
