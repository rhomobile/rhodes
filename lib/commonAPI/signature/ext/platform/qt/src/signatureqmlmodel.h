#ifndef SIGNATUREQMLMODEL_H
#define SIGNATUREQMLMODEL_H

#include <QObject>
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../ext/shared/generated/cpp/SignatureBase.h"

#include <QQuickPaintedItem>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <QQmlImageProviderBase>
#include <QQuickImageProvider>
#include <QQmlContext>
#include <QQmlEngine>

class SignatureQMLModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ getFileName CONSTANT)

public:
    QString fileName;

    static SignatureQMLModel * getInstance(){
        static SignatureQMLModel * instance = nullptr;
        if(instance != nullptr){

        }else{
            instance = new SignatureQMLModel(QtMainWindow::getLastInstance());
            QList<QObject *> objectList;
            objectList.append(instance);
            QtMainWindow::setContextProperty("signatureModel", objectList);
        }
        return instance;
    }



private:
    rho::apiGenerator::CMethodResult oResult;
    explicit SignatureQMLModel(QObject *parent = 0);

    ~SignatureQMLModel(){ }
signals:
    void fileNameChanged();
public slots:
    QString getFileName();
    void setFileName(QString value);


    void done();
    void cancel();

    void setOResult(QString filePath, rho::apiGenerator::CMethodResult &oResult);

};

#endif // SIGNATUREQMLMODEL_H
