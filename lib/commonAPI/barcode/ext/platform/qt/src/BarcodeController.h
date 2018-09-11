#ifndef BARCODECONTROLLER_H
#define BARCODECONTROLLER_H

#include <QObject>
#include <QCameraInfo>
#include <QString>
#include <QList>
#include <QEventLoop>
#include <QtGui>
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>
#include <QHash>
#ifndef OS_SAILFISH
#include "BarcodeDialogBuilder.h"
#else
#include "barcodeqmlmodel.h"
#endif

#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../../../../platform/shared/common/RhoStd.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "../../platform/shared/qt/rhodes/guithreadfunchelper.h"


class BarCodeController : public QObject
{
    Q_OBJECT
private:
    explicit BarCodeController();
    QEventLoop loop;
    static QMutex localMutex;
public:
    static BarCodeController * getInstance(){
        static BarCodeController instance;
        return &instance;
    }
    static void enumerate(rho::apiGenerator::CMethodResult &oResult);
    static void openDialog(rho::apiGenerator::CMethodResult &oResult, QString cameraId);
    static void refresh();
    static QHash<QString, QCameraInfo> camerasKeeper;
    static QList<QString> getIDs();

public slots:
    void getCameraInfo();
    void availableCameras(QList<QCameraInfo> info);
};

#endif // BARCODECONTROLLER_H
