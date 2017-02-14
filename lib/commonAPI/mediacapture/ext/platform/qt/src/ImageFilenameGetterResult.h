#ifndef IMAGEFILENAMEGETTERRESULT_H
#define IMAGEFILENAMEGETTERRESULT_H

#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "CCapturer.h"
#include <QDebug>

class ImageFileNameGetterResult : public IExecutable
{
    Q_OBJECT
public:
    explicit ImageFileNameGetterResult(rho::apiGenerator::CMethodResult &oResult, QString &fileName, QObject * parent):IExecutable(parent){
        result = oResult;
        this->fileName = fileName;

    }
private:
    rho::apiGenerator::CMethodResult result;
    QString fileName;

public slots:
    void execute(){

        rho::Hashtable<rho::String,rho::String>& mapRes = result.getStringHash();
        if (fileName.isEmpty()){
            mapRes["status"] = "cancel";
            mapRes["message"] = "Open file dialog has been canceled";
            return;
        }

        CCapturer::getImageData(mapRes, fileName);
        result.set(mapRes);

        qDebug() << "File name: " + fileName;

        deleteLater();
    }
};

#endif // IMAGEFILENAMEGETTERRESULT_H
