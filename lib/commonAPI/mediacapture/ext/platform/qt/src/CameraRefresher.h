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

class CameraRefresher : public IExecutable
{
    Q_OBJECT
public:
    explicit CameraRefresher(QWidget *parent):IExecutable(parent){}
private:

public slots:
    void execute(){
        QWidget * par = qobject_cast<QWidget *>(this->parent());

        /*QString fileNameToOpen = QFileDialog::getOpenFileName(0, "Open Image",
                                                              QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last()
                                                              , "Image Files (*.JPG *.PNG *.BMP *.GIF)");*/

        //QMessageBox::critical(qobject_cast<QWidget *>(this->parent()),"Mess",QString::number(QCameraInfo::availableCameras().size()));
        QEventLoop * loop = new QEventLoop(par);
        QTimer::singleShot(10,loop,SLOT(quit()));
        loop->exec();
        loop->deleteLater();

        deleteLater();
    }
};

#endif // CAMERAREFRESHER_H
