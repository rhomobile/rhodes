#ifndef CAMERAREFRESHER_H
#define CAMERAREFRESHER_H

#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include <QCameraInfo>
#include <QMessageBox>
#include <QString>
#include <QList>
#include <QTimer>
#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QEventLoop>
#include <QtGui>

class CameraRefresher : public IExecutable
{
    Q_OBJECT
public:
    explicit CameraRefresher(QWidget *parent):IExecutable(parent){


    }
private:

public slots:
    void execute(){
        QWidget * par = qobject_cast<QWidget *>(this->parent());

        /*QString fileNameToOpen = QFileDialog::getOpenFileName(0, QString::number(QCameraInfo::availableCameras().size()),
                                                              QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last()
                                                              , "Image Files (*.JPG *.PNG *.BMP *.GIF)");*/
        /*QMessageBox::information(0,"Mess",
                              QString::number(quint32(parent()->thread()->currentThreadId()), 16) + " " +
                              QString::number(quint32(this->thread()->currentThreadId()), 16));*/



        deleteLater();
    }
};

#endif // CAMERAREFRESHER_H
