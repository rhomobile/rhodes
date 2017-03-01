#ifndef CCAMERADATA_H
#define CCAMERADATA_H
#include <QObject>
#include <QCameraInfo>
#include <QCamera>
#include <QMultimedia>
#include <QMediaRecorder>
#include <QCameraImageCapture>
#include <QEventLoopLocker>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "../../platform/shared/qt/rhodes/QtMainWindow.h"
#include "../../platform/shared/qt/rhodes/impl/MainWindowImpl.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "ImageFileNameGetter.h"
#include "CameraDialogController.h"
#include "CameraDialogBuilder.h"

class CCameraData : public CameraDialogController{
    Q_OBJECT
private:
    QString cameraType;
    QString cameraID;
    QCameraInfo info;

    CCameraData();
    CCameraData(QCameraInfo &info);

    static QHash<QString, CCameraData *> camerasKeeper;

public:
    ~CCameraData() {cleanAll();}
    static const CCameraData *addNewCamera(QCameraInfo &info);
    static void cleanAll();
    static const QList<QString> getKeys();
    static const bool isEmpty();
    static CCameraData *getCameraData(QString &ID);

    const QString getCameraType() const;
    const QString getCameraID() const;

    void takeAPicture(rho::apiGenerator::CMethodResult &oResult);
    static QMutex * getMutex();

    void showView(rho::apiGenerator::CMethodResult &oResult);
    static QtMainWindow *getQMainWindow();
    static void choosePicture(rho::apiGenerator::CMethodResult &oResult);
public slots:


};

#endif // CCAMERADATA_H
