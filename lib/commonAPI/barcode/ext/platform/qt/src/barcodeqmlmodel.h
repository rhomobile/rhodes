#ifndef BARCODEQMLMODEL_H
#define BARCODEQMLMODEL_H

#include <QObject>
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../ext/shared/generated/cpp/BarcodeBase.h"
#include "BarcodeDialogView.h"
#include <QQuickPaintedItem>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "decoderthread.h"

#include <QQmlImageProviderBase>
#include <QQuickImageProvider>
#include <QQmlContext>
#include <QQmlEngine>

class BarcodeQMLModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString result READ getResult WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(bool isActive READ getIsActive CONSTANT)
    Q_PROPERTY(bool isGrabbing READ getIsGrabbing CONSTANT)
public:
    QString result;
    static BarcodeQMLModel * getInstance(){
        static BarcodeQMLModel * instance = nullptr;
        if(instance != nullptr){

        }else{
            instance = new BarcodeQMLModel(QtMainWindow::getLastInstance());
            QList<QObject *> objectList;
            objectList.append(instance);
            QtMainWindow::setContextProperty("barcodeModel", objectList);
        }
        return instance;
    }





private:
    DecoderThread * decThread;
    rho::apiGenerator::CMethodResult oResult;
    bool isActive;
    bool isGrabbing;
    QTimer * timer;

    quint32 lastId;
    explicit BarcodeQMLModel(QObject *parent = 0);

    ~BarcodeQMLModel(){ }

    QString lastReceivedResult;
signals:
    void barcodeResult(QString);
    void imageCaptured(QImage);
    void resultChanged();

public slots:
    void setResult(QString value);
    QString getResult();
    bool getIsActive();
    bool getIsGrabbing();

    void scanningProcessMsg();
    void restart();
    void done();
    void cancel();
    void capture();
    void setOResult(rho::apiGenerator::CMethodResult &oResult);

};

#endif // BARCODEQMLMODEL_H
