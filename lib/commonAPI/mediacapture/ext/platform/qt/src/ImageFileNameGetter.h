#ifndef IMAGEFILENAMEGETTER_H
#define IMAGEFILENAMEGETTER_H

#include <QString>
#include <QList>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImage>
#include <QTimer>
#include "ImageFilenameGetterResult.h"
#include "CameraDialogView.h"

class ImageFileNameGetter : public IExecutable
{
    Q_OBJECT
public:
    explicit ImageFileNameGetter(QThread * owner, rho::apiGenerator::CMethodResult &oResult, QObject * parent):IExecutable(parent){
        threadOwner = owner;
        result = oResult;
    }
private:
    QThread * threadOwner;
    rho::apiGenerator::CMethodResult result;

signals:

public slots:
    void execute(){
        QString fileNameToOpen = QFileDialog::getOpenFileName(0, "Open Image",
                                                              //QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last()
                                                              CameraDialogView::getImageDir().absolutePath()
                                                              , "Image Files (*.JPG *.PNG *.BMP *.GIF)");
        ImageFileNameGetterResult * getterResult = new ImageFileNameGetterResult(result, fileNameToOpen, threadOwner);
        getterResult->execute();
        deleteLater();
    }

};

#endif // IMAGEFILENAMEGETTER_H
