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
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "ImageFileNameGetter.h"
#include "CameraDialogController.h"
#ifndef OS_SAILFISH
#include "CameraDialogBuilder.h"
#endif

class CCameraData : public CameraDialogController{
    Q_OBJECT
private:
    QString cameraType;
    QString cameraID;
    QCameraInfo info;

    CCameraData();
    CCameraData(QCameraInfo &info);

    static QHash<QString, CCameraData *> camerasKeeper;
#ifdef OS_SAILFISH
    rho::apiGenerator::CMethodResult captureResult;
    bool hasCallback = false;
#endif

public:
    ~CCameraData() {}//cleanAll();}
    static const CCameraData *addNewCamera(QCameraInfo &info);
    static void cleanAll();
    static const QList<QString> getKeys();
    static const QList<CCameraData *> getValues();
    static const bool isEmpty();
    static CCameraData *getCameraData(QString ID);

    const QString getCameraType() const;
    const QString getCameraID() const;

    //void takeAPicture(rho::apiGenerator::CMethodResult &oResult);
    static QMutex * getMutex();

    void showView(rho::apiGenerator::CMethodResult &oResult);
    static QtMainWindow * getQMainWindow();
    static void choosePicture(rho::apiGenerator::CMethodResult &oResult);
    QString targetFileName;
    QString getTargetFileName() const;
    void setTargetFileName(const QString &value);

signals:
    void openCameraDialog(QString qmlDocument);
public slots:
#ifdef OS_SAILFISH
    void captured(QString fileName);
#endif

};

#endif // CCAMERADATA_H
