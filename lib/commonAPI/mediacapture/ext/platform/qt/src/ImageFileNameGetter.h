#ifndef IMAGEFILENAMEGETTER_H
#define IMAGEFILENAMEGETTER_H

#include <QString>
#include <QList>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImage>

#include "ImageFilenameGetterResult.h"

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
                                                              QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last()
                                                              , "Image Files (*.JPG *.PNG *.BMP *.GIF)");
        ImageFileNameGetterResult * getterResult = new ImageFileNameGetterResult(result, fileNameToOpen, threadOwner);
        //emit getterResult->run();
        getterResult->execute();

        deleteLater();
    }

};

#endif // IMAGEFILENAMEGETTER_H
