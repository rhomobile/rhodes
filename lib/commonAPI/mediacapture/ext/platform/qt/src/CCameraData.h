#ifndef CCAMERADATA_H
#define CCAMERADATA_H
#include <QObject>
#include <QCameraInfo>
#include <QCamera>
#include <QMultimedia>
#include <QMediaRecorder>
#include <QCameraImageCapture>
#include <QEventLoopLocker>
#include "CCapturer.h"
#include "CCameraDialogWindows.h"
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

class CCameraData : public QThread{
    Q_OBJECT
private:
    QCamera * cameraObject;
    QString cameraType;
    QString cameraID;
    QThread * cameraThread;
    CCapturer * capturer;
    QCameraInfo info;
    CCameraData();
    CCameraData(QCameraInfo &info);

    static QHash<QString, CCameraData *> camerasKeeper;

public:

    ~CCameraData(){delete cameraObject;}
    static const CCameraData *addNewCamera(QCameraInfo &info);
    static void cleanAll();
    static const QList<QString> getKeys();
    static const bool isEmpty();
    static CCameraData *getCameraData(QString &ID);

    const QString getCameraType() const;
    const QString getCameraID() const;
    void takeAPicture(rho::apiGenerator::CMethodResult &oResult);
    QCamera *getCameraObject();
    static QMutex * getMutex();

public slots:
    void run();
signals:
    void capture(rho::apiGenerator::CMethodResult oResult);

};

#endif // CCAMERADATA_H
