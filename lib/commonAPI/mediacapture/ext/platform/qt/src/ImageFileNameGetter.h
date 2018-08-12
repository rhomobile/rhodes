#ifndef IMAGEFILENAMEGETTER_H
#define IMAGEFILENAMEGETTER_H

#include <QString>
#include <QList>
#include <QStandardPaths>
#include <QImage>
#include <QTimer>
#include "ImageFilenameGetterResult.h"
#ifndef OS_SAILFISH
#include <QFileDialog>
#include "CameraDialogView.h"
#endif
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
#ifndef OS_SAILFISH
        QString fileNameToOpen = QFileDialog::getOpenFileName(0, "Open Image",
                                                              CameraDialogView::getImageDir().absolutePath()
                                                              , "Image Files (*.JPG *.PNG *.BMP *.GIF)");
#else
        QString fileNameToOpen;
#endif
        ImageFileNameGetterResult * getterResult = new ImageFileNameGetterResult(result, fileNameToOpen, threadOwner);
        getterResult->execute();
        deleteLater();
    }

};

#endif // IMAGEFILENAMEGETTER_H
