#ifndef IMAGEFILENAMEGETTERRESULT_H
#define IMAGEFILENAMEGETTERRESULT_H

#include "../../platform/shared/qt/rhodes/iexecutable.h"
#ifndef OS_SAILFISH
#include "CameraDialogView.h"
#endif
#include <QFile>
#include <QFileInfo>
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
        QFileInfo fInfo(fileName);
#ifndef OS_SAILFISH
        QString newFileName = CameraDialogView::getImageDir().absolutePath() + "/" + fInfo.fileName();
#else
        QString newFileName;
#endif
        if(fInfo.isFile() && !newFileName.isEmpty() && QFile::copy(fileName, newFileName)){
#ifndef OS_SAILFISH
            CameraDialogView::getImageData(mapRes, newFileName);
#else
#endif
        }else{
            mapRes["status"] = "cancel";
            mapRes["message"] = "Open file dialog has been canceled";
        }

        result.set(mapRes);
        deleteLater();
    }
};

#endif // IMAGEFILENAMEGETTERRESULT_H
